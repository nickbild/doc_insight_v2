python3 models/research/slim/train_image_classifier.py \
    --train_dir=training_output \
    --dataset_split_name=train \
    --dataset_dir=data \
    --model_name=mobilenet_v1_025 \
    --preprocessing_name=mobilenet_v1 \
    --train_image_size=96 \
    --input_grayscale=True \
    --save_summaries_secs=300 \
    --learning_rate=0.045 \
    --label_smoothing=0.1 \
    --learning_rate_decay_factor=0.98 \
    --num_epochs_per_decay=2.5 \
    --moving_average_decay=0.9999 \
    --batch_size=32 \
    --max_number_of_steps=1000000



python3 models/research/slim/train_image_classifier.py \
    --train_dir=training_output \
    --dataset_name=hand_washing \
    --dataset_split_name=train \
    --dataset_dir=data \
    --model_name=mobilenet_v1_025 \
    --preprocessing_name=mobilenet_v1 \
    --train_image_size=96 \
    --input_grayscale=True \
    --save_summaries_secs=300 \
    --learning_rate=0.045 \
    --label_smoothing=0.1 \
    --learning_rate_decay_factor=0.98 \
    --num_epochs_per_decay=2.5 \
    --moving_average_decay=0.9999 \
    --batch_size=32 \
    --max_number_of_steps=1000000
    