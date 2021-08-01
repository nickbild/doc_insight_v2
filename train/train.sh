make_image_classifier \
  --image_dir data \
  --tfhub_module https://tfhub.dev/google/imagenet/mobilenet_v1_025_128/feature_vector/5 \
  --image_size 96 \
  --saved_model_dir trained_model \
  --labels_output_file class_labels.txt \
  --tflite_output_file doc_insight.tflite \
  --summaries_dir tf_logs

# "trained_model/saved_model.pb" TF SavedModel has been created by training on input images.

python3 quantize.py

# SavedModel converted to quantized TF Lite model: "doc_insight.lite.quant.pb"

python3 gen_micro_model.py

# Final output generated: "doc_insight.micro.quant.pb"
