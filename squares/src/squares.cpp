#include "utilities.h"
#include "web_utils.h"
#include "csv.h"

#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

#include <iostream>
#include <vector>

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif


#define SQUARE_SIDE 50
#define SQUARE_STEP_ROW 15
#define SQUARE_STEP_COL 15
#define ZOOM_LEVEL 18
#define OUTPUT_FOLDER "output_images"
#define DOWNLOAD_FOLDER "download_folder"
#define BING_KEY "AkNXb92bZ-WiPqH4BmlhYj_piINlOW_rpon5CPEZ8xuhNXUdxhsKeKw6NRpN0ACC"

using namespace std;
using namespace cv;
using namespace utilities;

int main()
{
	// set source directory
	string source_dir = "images";

	// remove output directories from previous execution
	remove_directory(OUTPUT_FOLDER);
	remove_directory(DOWNLOAD_FOLDER);

	// recreate output directory
	mkdir(OUTPUT_FOLDER, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH | S_IWOTH);
	mkdir(DOWNLOAD_FOLDER, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH | S_IWOTH);

	vector<coordinate> centers;
	centers.push_back(coordinate(47.6101, -122.107));
	centers.push_back(coordinate(41.9106, 12.5409));
	centers.push_back(coordinate(41.9106849, 12.5430949));

	getImagesFromCoordinates(centers, DOWNLOAD_FOLDER, ZOOM_LEVEL, BING_KEY);

	// compute squares in images
	getSquaresInFolder(DOWNLOAD_FOLDER, SQUARE_SIDE, SQUARE_STEP_ROW, SQUARE_STEP_COL, OUTPUT_FOLDER);

	vector<string> images;
	images.push_back("albero");
	images.push_back("casa");
	images.push_back("lago");


	vector<bool> resp;
	resp.push_back(true);
	resp.push_back(false);
	resp.push_back(false);


	remove("output_images/world.csv");
	writeCSV(images, resp, "output_images/world.csv");
	readCSV("output_images/world.csv");




}
