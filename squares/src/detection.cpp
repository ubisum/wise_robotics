#include "detection.h"
#include <vector>
#include <iostream>
#include <sys/stat.h>
#include "utilities.h"
#include "web_utils.h"

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "rapidjson/document.h"

/*
 * Rect(col, row, num_rows, num_cols)
 */

using namespace std;
using namespace cv;
using namespace utilities;

int main(int argn, char* argv[])
{
    if(argn < 2)
    {
        cout << "Please, insert path to a valid .xml file." << endl;
        exit(-1);
    }

    // create output folder
    remove_directory(DETECT_OUTPUT_FOLDER);
    remove_directory(DETECT_JSON_FOLDER);
    mkdir(DETECT_OUTPUT_FOLDER, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH | S_IWOTH);
    mkdir(DETECT_JSON_FOLDER, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH | S_IWOTH);

    detectObjects(argv[1]);

    vector<Rect> test_vec;
    test_vec.push_back(Rect(0,0,50, 20));
    test_vec.push_back(Rect(100, 44, 10, 60));
    test_vec.push_back(Rect(60,70, 30, 70));

    createJSONFromRects("41x908436_12x540253_20.jpg", test_vec);
    Document doc = extractDocumentFromJSON("json_folder/41x908436_12x540253_20.json");
    assert(doc.HasMember("file_name"));
    assert(doc.HasMember("original_lat"));
    assert(doc.HasMember("original_long"));
    assert(doc.HasMember("zoom_level"));
    assert(doc.HasMember("objects"));

    cout << doc["objects"][1]["center_geo_lat"].GetFloat() << endl;

    //assert(doc["ojects"][0].HasMember("rect_x"));
    //assert(doc["ojects"][0].HasMember("rect_y"));
    //assert(doc["ojects"][0].HasMember("rect_center_x"));
    //assert(doc["ojects"][0].HasMember("rect_center_y"));
    //assert(doc["ojects"][0].HasMember("center_geo_lat"));
    //assert(doc["ojects"][0].HasMember("center_geo_lon"));

    stringstream training_cmd;
    vector<string> filesList = getFilesList("negative_samples");
    training_cmd <<  "opencv_traincascade -data classifier -vec samples.vec -bg negatives.txt" <<
                     " -numStages " << NUM_STAGES << " -minHitRate " << MIN_HIT_RATE <<
                     " -maxFalseAlarmRate " << MAX_FALSE_ALARM_RATE << " -numPos " << NUM_POS <<
                     " -numNeg " << /*NUM_NEG*/ filesList.size() << " -w " << SQUARE_SIDE << " -h " << SQUARE_SIDE <<
                     " -mode ALL -precalcValBufSize " << PRE_CALC_VAL_BUF_SIZE <<
                     " -precalcIdxBufSize " << PRE_CALC_IDX_BUF_SIZE;

    cout << training_cmd.str() << endl << endl;

}
