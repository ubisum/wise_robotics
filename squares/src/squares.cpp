#include "utilities.h"
#include "web_utils.h"
#include "csv.h"
#include "macros.h"

#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

#include <iostream>
#include <vector>

/*#include <wx/wxprec.h>
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif*/


using namespace std;
using namespace cv;
using namespace utilities;

int main(int argn, char* argv[])
{
        if(argn < 2)
        {
            cout << "Please, insert a path to a valid CSV file, with correct formatting" << endl;
            exit(-1);
        }

        // read centers' coordinates
        vector<coordinate> centers = readCoordinates(argv[1]);

	// remove output directories from previous execution
	remove_directory(OUTPUT_FOLDER);
	remove_directory(DOWNLOAD_FOLDER);

	// recreate output directory
	mkdir(OUTPUT_FOLDER, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH | S_IWOTH);
	mkdir(DOWNLOAD_FOLDER, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH | S_IWOTH);

        // download images
        cout << "Downloading images from Bing..." << endl;
        getImagesFromCoordinates(centers, DOWNLOAD_FOLDER, BING_KEY);

	// compute squares in images
        cout << endl << "Starting extraction of samples..." << endl;
	getSquaresInFolder(DOWNLOAD_FOLDER, SQUARE_SIDE, SQUARE_STEP_ROW, SQUARE_STEP_COL, OUTPUT_FOLDER);

        stringstream training_cmd;
        training_cmd <<  "opencv_traincascade -data classifier -vec samples.vec -bg negatives.txt" <<
                         " -numStages " << NUM_STAGES << " -minHitRate " << MIN_HIT_RATE <<
                         " -maxFalseAlarmRate " << MAX_FALSE_ALARM_RATE << " -numPos " << NUM_POS <<
                         " -numNeg " << NUM_NEG << " -w " << SQUARE_SIDE << " -h " << SQUARE_SIDE <<
                         " -mode ALL -precalcValBufSize " << PRE_CALC_VAL_BUF_SIZE <<
                         " -precalcIdxBufSize " << PRE_CALC_IDX_BUF_SIZE;

        cout << endl << training_cmd.str() << endl;

}
