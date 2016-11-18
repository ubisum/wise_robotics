#ifndef DETECTION_H
#define DETECTION_H

#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#include <string>
#include <stdio.h>
#include <iostream>
#include <vector>
#include "macros.h"
#include "utilities.h"

using namespace cv;
using namespace std;
using namespace utilities;

void drawRectanglesOverImage(const Mat& image, string file_name, const vector<Rect>& vectorOfRectangles)
{
    // make a copy of input
    Mat input_copy;
    image.copyTo(input_copy);

    // loop over rectangles
    for(int index = 0; index < vectorOfRectangles.size(); index++)
    {
        // select current rectangle
        Rect current_rect = vectorOfRectangles[index];

        // draw rectangle
        rectangle(input_copy, current_rect, Scalar(0,0,255), 2);
    }

    // prepare parameters for storing image
    vector<int> compression_params;
    compression_params.push_back(CV_IMWRITE_JPEG_QUALITY);
    compression_params.push_back(95);

    // revove extension from file's name
    char no_extension[file_name.length()];
    file_name.copy(no_extension, file_name.length());
    char* pointer = strtok(no_extension, ".");

    // prepare output path
    stringstream output_path;
    output_path << DETECT_OUTPUT_FOLDER << "/" << pointer << ".jpg";

    // store image
    imwrite(output_path.str().c_str(), input_copy, compression_params);

}

void detectObjects(string classifier_name)
{
    // retrieve objects in input folder
    vector<string> files_list = getFilesList(DETECTION_FOLDER);
    if(files_list.size() == 0){
        cout << "No files in folder " << DETECTION_FOLDER << ". Program will be terminated." << endl;
        exit(-1);
    }

    // create a classifier
    CascadeClassifier classifier(classifier_name);

    // loop over images
    for(int index = 0; index < files_list.size(); index++)
    {
        // get a file's name
        string current_file = files_list[index];

        // try to load image
        stringstream image_path;
        image_path << DETECTION_FOLDER << "/" << current_file;
        //cout << DETECTION_FOLDER + '/' + current_file << endl;
        Mat image = imread(image_path.str());
        if(image.data == NULL)
        {
            cout << "Cannot load image from file " << current_file << endl << endl;
            continue;
        }

        else
            cout << "Loaded image " << current_file << endl;

        // convert image to grayscale
        Mat image_gray;
        cvtColor(image, image_gray, CV_BGR2GRAY);
        //equalizeHist(image_gray, image_gray);

        // create a vector of Rect
        vector<Rect> returned_rects;

        // detect objects
        classifier.detectMultiScale(image_gray, returned_rects, 1.1, 3, 0/*|CASCADE_SCALE_IMAGE*/, Size(30, 30), Size(50, 50));

        // print number of objects
        cout << "Found " << returned_rects.size() << " objects in image " << current_file << endl;

        // if at least an object has been returned, show it on image and save the latter
        if(returned_rects.size() > 0)
            drawRectanglesOverImage(image, current_file, returned_rects);

    }



}


#endif // DETECTION_H

