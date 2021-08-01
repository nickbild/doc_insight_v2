import os


MODEL_TFLITE = "doc_insight.lite.quant.pb"
MODEL_TFLITE_MICRO = "doc_insight.micro.quant.pb"

os.system("xxd -i {0} > {1}".format(MODEL_TFLITE, MODEL_TFLITE_MICRO))
# Update variable names
REPLACE_TEXT = MODEL_TFLITE.replace('/', '_').replace('.', '_')
os.system("sed -i 's/'{0}'/g_model/g' {1}".format(REPLACE_TEXT, MODEL_TFLITE_MICRO))

