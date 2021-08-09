/* Copyright 2019 The TensorFlow Authors. All Rights Reserved.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
==============================================================================
Modified by Nick Bild (nick.bild@gmail.com)
August 2021
https://github.com/nickbild/doc_insight_v2
*/

#include "main_functions.h"

#include "detection_responder.h"
#include "image_provider.h"
#include "model_settings.h"
#include "person_detect_model_data.h"
#include "tensorflow/lite/micro/kernels/micro_ops.h"
#include "tensorflow/lite/micro/micro_error_reporter.h"
#include "tensorflow/lite/micro/micro_interpreter.h"
#include "tensorflow/lite/micro/micro_mutable_op_resolver.h"
#include "tensorflow/lite/micro/all_ops_resolver.h"
#include "tensorflow/lite/schema/schema_generated.h"
#include "tensorflow/lite/version.h"
#include "../../components/quirc/lib/quirc.h"
#include "esp/app_camera_esp.h"
#include <ctime>
#include <map>
#include <string>
#include <iostream>
#include <sstream>
#include "driver/gpio.h"

#define GPIO_D1     (gpio_num_t)19   // SPI CS
#define GPIO_D2      (gpio_num_t)2   // SPI DIN
#define GPIO_D3     (gpio_num_t)12   // SPI DOUT
#define GPIO_OUTPUT_PIN_SEL  ((1ULL<<GPIO_CS) | (1ULL<<GPIO_DIN) | (1ULL<<GPIO_DOUT))

namespace {

  tflite::ErrorReporter* error_reporter = nullptr;
  const tflite::Model* model = nullptr;
  tflite::MicroInterpreter* interpreter = nullptr;
  TfLiteTensor* input = nullptr;

  constexpr int kTensorArenaSize = 93 * 1024;
  static uint8_t tensor_arena[kTensorArenaSize];

  struct quirc *qr;
  uint8_t *image;

  int kNumCols_quirc;
  int kNumRows_quirc;

  long int last_hand_wash = 1;
  std::string last_patient_seen = "";
  std::string last_patient_seen_allergies = "";

  // Substitute a DB connection for practical uses.

  // PaientID: PatientFirstName ; PatientLastName ; PatientAge ; PatientGender ; MedicationAllergies
  std::map<std::string, std::string> patients = {
      { "P123456789", "Jane Doe 46 F M984738903" },
      { "P125437897", "Greg Bay 55 M NA" },
      { "P839839767", "Linda Johnson 23 F M843987775" }
  };

  // MedicationID: MedicationName
  std::map<std::string, std::string> medications = {
      { "M984738903", "Atorvastatin" },
      { "M843987775", "Amlodipine" }
  };

  // PatientID: MedicationID
  std::map<std::string, std::string> prescribed_medications = {
      { "P125437897", "M984738903" },
      { "P123456789", "M843987775" }
  };

}

void setup() {
  // QR

  qr = quirc_new();
  if (!qr) {
      perror("Failed to allocate memory");
      abort();
  }

  if (quirc_resize(qr, kNumCols, kNumRows) < 0) {
      perror("Failed to allocate video memory");
      abort();
  }

  kNumCols_quirc = 96;
  kNumRows_quirc = 96;

  // ML

  static tflite::MicroErrorReporter micro_error_reporter;
  error_reporter = &micro_error_reporter;

  model = tflite::GetModel(g_person_detect_model_data);
  if (model->version() != TFLITE_SCHEMA_VERSION) {
    TF_LITE_REPORT_ERROR(error_reporter,
                         "Model provided is schema version %d not equal "
                         "to supported version %d.",
                         model->version(), TFLITE_SCHEMA_VERSION);
    return;
  }

  static tflite::MicroInterpreter static_interpreter(
      model, tflite::AllOpsResolver(), tensor_arena, kTensorArenaSize, error_reporter);
  interpreter = &static_interpreter;

  // Allocate memory from the tensor_arena for the model's tensors.
  TfLiteStatus allocate_status = interpreter->AllocateTensors();
  if (allocate_status != kTfLiteOk) {
    TF_LITE_REPORT_ERROR(error_reporter, "AllocateTensors() failed");
    return;
  }

  input = interpreter->input(0);
}

void loop() {
  process_qr_code((unsigned char*)"P123456789");
  process_qr_code((unsigned char*)"M843987775");

  // QR

  image = quirc_begin(qr, &kNumCols_quirc, &kNumRows_quirc);

  if (kTfLiteOk != GetImage(error_reporter, kNumCols, kNumRows, kNumChannels, image)) {
    TF_LITE_REPORT_ERROR(error_reporter, "QR Image capture failed.");
  }

  quirc_end(qr);

  int num_codes = quirc_count(qr);
  for (int i = 0; i < num_codes; i++) {
      struct quirc_code code;
      struct quirc_data data;
      
      quirc_extract(qr, i, &code);

      if (!quirc_decode(&code, &data)) {
        process_qr_code(data.payload);
      } else {
        printf("QR decoding failed.\n");
      }
  }

  // ML
  
  if (kTfLiteOk != GetImage(error_reporter, kNumCols, kNumRows, kNumChannels,
                            input->data.uint8)) {
    TF_LITE_REPORT_ERROR(error_reporter, "Image capture failed.");
  }

  // Run the model on this input and make sure it succeeds.
  if (kTfLiteOk != interpreter->Invoke()) {
    TF_LITE_REPORT_ERROR(error_reporter, "Invoke failed.");
  }

  TfLiteTensor* output = interpreter->output(0);

  // Process the inference results.
  uint8_t wash_score = output->data.uint8[kPersonIndex];
  uint8_t no_wash_score = output->data.uint8[kNotAPersonIndex];
  
  // Hand washing detected?
  if (wash_score > (no_wash_score * 1.25)) {
    time_t now = time(0);
    printf("Handwashing detected at: %ld\n", now);
    last_hand_wash = now;
  }

}

void process_qr_code(unsigned char* qr_code) {
  std::string line(reinterpret_cast<char*>(qr_code));

  if (line[0] == 'P') { // Patient
    std::string ptid(reinterpret_cast<char*>(qr_code));
    std::string pt = patients[ptid];
    std::string arr[5];
    int i = 0;
    std::stringstream ssin(pt);
    while (ssin.good() && i < 5){
        ssin >> arr[i];
        ++i;
    }
    
    last_patient_seen = line;
    last_patient_seen_allergies = arr[4];
    patient_found(line, arr[0], arr[1], arr[2], arr[3], arr[4]);

  } else if (line[0] == 'M') { // Medication
    std::string medid(reinterpret_cast<char*>(qr_code));
    med_found(medid, medications[medid]);
  }

}

void patient_found(std::string PatientID, std::string PatientFirstName, std::string PatientLastName, std::string PatientAge, std::string PatientGender, std::string MedicationAllergies) {
  // Show patient info on display.
  if (PatientID == "P125437897") {
    gpio_set_level(GPIO_D1, 1);
    gpio_set_level(GPIO_D2, 1);
    gpio_set_level(GPIO_D3, 0);
  } else if (PatientID == "P123456789") {
    gpio_set_level(GPIO_D1, 1);
    gpio_set_level(GPIO_D2, 0);
    gpio_set_level(GPIO_D3, 0);
  }
  
  // Alert if hands not washed prior to seeing patient.
  time_t now = time(0);
  int diff = now - last_hand_wash;
  if (diff > 120) {
    printf("Hands have not been washed within 2 minutes of encountering patient!\n");
    gpio_set_level(GPIO_D1, 1);
    gpio_set_level(GPIO_D2, 1);
    gpio_set_level(GPIO_D3, 1);
  }
  
}

void med_found(std::string MedicationID, std::string MedicationName) {
  // printf("%s\n", MedicationName.c_str());
  if (last_patient_seen != "") {
    // printf("%s\n", last_patient_seen.c_str());
    
    // Check for allergies.
    if (MedicationID == last_patient_seen_allergies) {
      printf("Patient is allergic to this med!\n");
      gpio_set_level(GPIO_D1, 0);
      gpio_set_level(GPIO_D2, 1);
      gpio_set_level(GPIO_D3, 1);
    }

    // Check if med was prescribed to patient.
    if (prescribed_medications[last_patient_seen] != MedicationID) {
      printf("Patient has not been prescribed this medication!\n");
      gpio_set_level(GPIO_D1, 0);
      gpio_set_level(GPIO_D2, 0);
      gpio_set_level(GPIO_D3, 0);
    }
  }
}
