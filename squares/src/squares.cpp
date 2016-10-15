#include "utilities.h"
#include "web_utils.h"
#include "csv.h"
#include "macros.h"

#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

#include <iostream>
#include <vector>

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif


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
	getImagesFromCoordinates(centers, DOWNLOAD_FOLDER, ZOOM_LEVEL, BING_KEY);

	// compute squares in images
	getSquaresInFolder(DOWNLOAD_FOLDER, SQUARE_SIDE, SQUARE_STEP_ROW, SQUARE_STEP_COL, OUTPUT_FOLDER);

}
