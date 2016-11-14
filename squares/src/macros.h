#ifndef MACROS_H
#define MACROS_H

// squares extraction
#define SQUARE_SIDE 50
#define SQUARE_STEP_ROW 15
#define SQUARE_STEP_COL 15
#define OUTPUT_FOLDER "output_images"

// images download
#define MIN_ZOOM_LEVEL 15
#define ZOOM_LEVEL 21
#define DOWNLOAD_FOLDER "download_folder"
#define BING_KEY "AkNXb92bZ-WiPqH4BmlhYj_piINlOW_rpon5CPEZ8xuhNXUdxhsKeKw6NRpN0ACC"

// samples creation
#define POS_SAMPLES "positive_samples"
#define NEG_SAMPLES "negative_samples"
#define POS_SAMPLES_TXT "find ./positive_samples -iname " "*.jpg" "> positives.txt"
#define NEG_SAMPLES_TXT "find ./negative_samples -iname " "*.jpg" "> negatives.txt"
#define PERL_SCRIPT_CMD "perl tools/createsamples.pl positives.txt negatives.txt samples "

// samples parameters
#define NUM_SAMPLES 1500
#define BGCOLOR 0
#define BGTHRESH 0
#define MAX_X_ANGLE 1.1
#define MAX_Y_ANGLE 1.1
#define MAX_Z_ANGLE 0.5
#define MAX_I_DEV 40

// merging samples
#define MERGE_VEC_CMD "python ./tools/mergevec.py -v samples/ -o samples.vec"

// training
#define TRAINING_CMD "opencv_traincascade -data classifier -vec samples.vec -bg negatives.txt\
-numStages 20 -minHitRate 0.999 -maxFalseAlarmRate 0.5 -numPos 1000\
-numNeg 600 -w 80 -h 40 -mode ALL -precalcValBufSize 1024\
-precalcIdxBufSize 1024"

// miscellaneous
#define EARTH_RADIUS 6378137
#define LUT_NAME "lookupTable.yml"

#endif // MACROS_H

