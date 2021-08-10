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

Due to a lack of available GPIO pins on the ESP-EYE, an Adafruit QT Py microcontroller was included to control the Nokia 5110 LCD display.  The LCD was removed from the breakout board, and the backing was removed to make it transparent.  Wires were soldered to the pads to allow it to be controlled from the QT Py.  For this prototype, a structure was created from wooden dowels to extend the LCD sufficiently in front of the wearer's eye to bring the display into focus.

A 2000 mAh LiPo battery and an Adafruit PowerBoost 500 Charger were included to supply power for both microcontrollers.  All components were fit into a small plastic case that was attached to the frames of a pair of glasses.

## Media

The glasses, partially disassembled to show components:
![](https://raw.githubusercontent.com/nickbild/doc_insight_v2/main/media/top_opened_sm_annotate.jpg)

Our patient:<br/>
![Our patient](https://raw.githubusercontent.com/nickbild/doc_insight_v2/main/media/patient_alone_sm_annotate.jpg)

Our patient as seen by Doc InSight wearer:<br/>
![Our patient glasses](https://raw.githubusercontent.com/nickbild/doc_insight_v2/main/media/patient_2_sm_crop.jpg)

A medication bottle:<br/>
![](https://raw.githubusercontent.com/nickbild/doc_insight_v2/main/media/bottle_alone_sm.jpg)

Allergy alert example:<br/>
![](https://raw.githubusercontent.com/nickbild/doc_insight_v2/main/media/allergy_alert_sm.jpg)

Unprescribed medication warning:<br/>
![](https://raw.githubusercontent.com/nickbild/doc_insight_v2/main/media/not_prescribed_sm.jpg)

Front view:
![](https://raw.githubusercontent.com/nickbild/doc_insight_v2/main/media/full_glasses_angle_sm.jpg)

Close up:
![](https://raw.githubusercontent.com/nickbild/doc_insight_v2/main/media/glasses_camera_lcd_close_sm.jpg)

Another view:
![](https://raw.githubusercontent.com/nickbild/doc_insight_v2/main/media/lcd_back_angle_sm.jpg)

The prepared LCD:
![](https://raw.githubusercontent.com/nickbild/doc_insight_v2/main/media/nokia_5110_sm.jpg)

## Bill of Materials

## About the Author

[Nick A. Bild, MS](https://nickbild79.firebaseapp.com/#!/)
