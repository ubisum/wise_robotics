#include "cmd_utils.h"

#include <iostream>

#define PICS_PATH "/home/biagio/Documenti/wise_robotics/annotations/build/data/image_folder"
#define ANNOTATIONS_PATH "/home/biagio/Documenti/wise_robotics/annotations/build/data/annotations.txt"
#define ANNOTATIONS_CMD "opencv_annotation --images=" PICS_PATH " --annotations=" ANNOTATIONS_PATH

using namespace std;

int main()
{
    remove(ANNOTATIONS_PATH);
    execute_command(ANNOTATIONS_CMD);
}
