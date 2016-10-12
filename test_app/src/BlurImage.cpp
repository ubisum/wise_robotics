#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <iostream>

using namespace std;
using namespace cv;

#define NUM_CLUSTERS 5

int main( int argc, char** argv )
{
	// read image
	Mat img = imread( argv[1], 1 );
	
	// reshape input
	int n = img.rows * img.cols;
    Mat data = img.reshape(1, n);
	
	// change input's data type
    data.convertTo(data, CV_32F);

	// apply k-means
    vector<int> labels;
    Mat1f colors;
    kmeans(data, NUM_CLUSTERS, labels, cv::TermCriteria(), 1, cv::KMEANS_PP_CENTERS, colors);

	// update with centers' colors
    for (int i = 0; i < n; ++i)
    {
        data.at<float>(i, 0) = colors(labels[i], 0);
        data.at<float>(i, 1) = colors(labels[i], 1);
        data.at<float>(i, 2) = colors(labels[i], 2);
    }
	
	// reshape result
    Mat reduced = data.reshape(3, img.rows);
    reduced.convertTo(reduced, CV_8U);

	// show results
    imshow("Reduced", reduced);
    waitKey();	

    return 0;
}