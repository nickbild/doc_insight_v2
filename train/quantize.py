import tensorflow as tf
from random import randint


MODEL_TF = "trained_model/"
MODEL_QUANT = "doc_insight.lite.quant.pb"

converter = tf.lite.TFLiteConverter.from_saved_model(MODEL_TF)

# Set the optimization flag.
converter.optimizations = [tf.lite.Optimize.OPTIMIZE_FOR_SIZE]
model_tflite = converter.convert()

# Save the model to disk
open(MODEL_QUANT, "wb").write(model_tflite)

