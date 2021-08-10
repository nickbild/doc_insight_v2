# Doc InSight

Doc InSight provides an augmented reality heads-up display for health care professionals. Relevant medical information from an electronic health record is displayed in the wearer's field of vision when environmental QR codes are detected. The information display is completely passive and hands-free -- no action is required of the user.  

A machine learning (ML) algorithm runs in the background to enhance patient safety.  In particular, two of the most common medical errors are treatment-related infections, and medication errors.  Doc InSight seeks to eliminate these types of errors without saddling health care providers with additional procedures to follow that reduce their time treating patients, and have also been shown to lead to physician burnout.

<p align="center">
<img src="https://raw.githubusercontent.com/nickbild/doc_insight_v2/main/media/patient_2_sm_crop.jpg">
</p>

## How It Works

An Espressif ESP-EYE ESP32-based development board with an OV2640 image sensor runs two primary algorithms: 1) a TensorFlow Lite for Microcontrollers neural network, and a QR code decoding algorithm ([code here](https://github.com/nickbild/doc_insight_v2/tree/main/main)).

Two types of QR codes are currently detected—those affixed to patients, and those affixed to medications.  When a patient is detected, their information (name, age, gender) is displayed on a transparent LCD within the wearer's field of vision.  Additionally, if the provider has not washed their hands (as determined by the ML algorithm) within two minutes of encountering the patient, a hand washing reminder is also displayed.

When medications are in view, they will be compared to both the medications prescribed to the last patient seen, as well as that patient's allergies.  In the former case, a "Not prescribed" warning will be displayed if they have not been prescribed that medication; in the latter case, an "Allergy Alert" is shown if they are allergic to the medication.

A custom convolutional neural network was developed and trained in TensorFlow Lite for handwashing recognition ([code here](https://github.com/nickbild/doc_insight_v2/tree/main/train)).

Due to a lack of available GPIO pins on the ESP-EYE, an Adafruit QT Py microcontroller was included to control the Nokia 5110 LCD display.  The LCD was removed from the breakout board, and the backing was removed to make it transparent.  Wires were soldered to the pads to allow it to be controlled from the QT Py.

A 2000 mAh LiPo battery and an Adafruit PowerBoost 500 Charger were included to supply power for both microcontrollers.

## Media

The glasses, partially disassembled to show components:
![](https://raw.githubusercontent.com/nickbild/doc_insight_v2/main/media/top_opened_sm.jpg)

Our patient:<br/><br/>
![Our patient](https://raw.githubusercontent.com/nickbild/doc_insight_v2/main/media/patient_alone_sm.jpg)

Our patient as seen by Doc InSight wearer:
![Our patient glasses](https://raw.githubusercontent.com/nickbild/doc_insight_v2/main/media/patient_2_sm_crop.jpg)



## Bill of Materials

## About the Author

[Nick A. Bild, MS](https://nickbild79.firebaseapp.com/#!/)
