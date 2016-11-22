#ifndef DETECTION_H
#define DETECTION_H

#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#include <string>
#include <algorithm>
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

void createJSONFromRects(string image_name, vector<Rect> objects_locations)
{
    // remove extension from file name
    char no_extension[image_name.length()];
    image_name.copy(no_extension, image_name.length());
    char* pointer = strtok(no_extension, ".");

    // declare stringstreams for JSON file
    stringstream json_name, json_content;

    // prepare file's name
    json_name << DETECT_JSON_FOLDER << "/" << pointer << ".json";
    cout << "JSON name: " << json_name.str() << endl;

    // open output file
    FILE* output_json = fopen(json_name.str().c_str(), "a");
    if(output_json == NULL)
    {
        cout << "Could not open file " << json_name.str() << ". Program will be terminated." << endl;
        exit(-1);
    }

    // start JSON
    json_content << "{";

    // add info about file's name
    json_content << "\"file_name\":\"" << image_name  << "\",";

    // retrieve coordinates of image's center
    char temporary_buffer[image_name.length()];
    image_name.copy(temporary_buffer, image_name.length());

    // latitude
    char* coord_ptr = strtok(temporary_buffer, "_");
    if(coord_ptr == NULL)
    {
        cout << "Detection error: image name is not in format lat_lon_zoom" << endl;
        exit(-1);
    }
    string latitude(coord_ptr);
    replace(latitude.begin(), latitude.end(), 'x', '.');

    // longitude
    coord_ptr = strtok(NULL, "_");
    if(coord_ptr == NULL)
    {
        cout << "Detection error: image name is not in format lat_lon_zoom" << endl;
        exit(-1);
    }

    string longitude(coord_ptr);
    replace(longitude.begin(), longitude.end(), 'x', '.');

    // add info about center's coordinates
    json_content << "\"original_lat\":" << latitude << ",\"original_long\":" << longitude << ",";

    // add info about zoom
    coord_ptr = strtok(NULL, ".");
    json_content << "\"zoom_level\":";
    if(coord_ptr == NULL)
        json_content << "null,";
    else
        json_content << string(coord_ptr) << ",";

    // check whether LUT has already been created
    FILE* check_lut = fopen(LUT_NAME, "r");

    // create LUT, if necessary
    if(check_lut == NULL)
    {
        cout << "No LUT found." << endl << "Creating LUT..." << endl;
        createLUT();
        cout << "Done." << endl << endl;
    }

    else
        fclose(check_lut);

    // get LUT
    Mat lookuptable = readLUT();

    // iterate over objects
    json_content << "\"objects\":[";
    for(int index = 0; index < objects_locations.size(); index++)
    {
        // start item
        json_content << "{";

        // get a Rect
        Rect current_rect = objects_locations[index];

        // add info about Rect's position
        json_content << "\"rect_x\":" << current_rect.x << ",\"rect_y\":" << current_rect.y << "," ;


        // compute approximate pixel coordinates of center
        int center_x = (int)floor((current_rect.x + current_rect.width)/2);
        int center_y = (int)floor((current_rect.y + current_rect.height)/2);

        // add info about Rect's center
        json_content << "\"rect_center_x\":" << center_x << ",\"rect_center_y\":"  << center_y << ",";

        // compute geographical coordinates of center
        coordinate center_geo = pixelToLatLong(coordinate((float)atof(latitude.c_str()), (float)atof(longitude.c_str())),
                                               current_rect.x, current_rect.y, lookuptable, (int)atoi(coord_ptr));

        /*coordinate center_geo = pixelToLatLong(coordinate((float)atof(latitude.c_str()), (float)atof(longitude.c_str())),
                                               current_rect.y, current_rect.x, lookuptable, (int)atoi(coord_ptr));*/

        // add info about geographical coordinates
        json_content << "\"center_geo_lat\":" << center_geo.first << ",\"center_geo_lon\":" << center_geo.second;

        // close current item in array
        json_content << "}";
        if(index < objects_locations.size()-1)
            json_content << ",";
    }

    // close array and JSON
    json_content << "]}";

    // write to file
    fputs(json_content.str().c_str(), output_json);

    // close file
    fclose(output_json);

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
        classifier.detectMultiScale(image_gray, returned_rects,  1.1, 2, 0|CV_HAAR_SCALE_IMAGE, Size(30, 30));

        // print number of objects
        cout << "Found " << returned_rects.size() << " objects in image " << current_file << endl;

        // if at least an object has been returned, show it on image and save the latter
        if(returned_rects.size() > 0)
            drawRectanglesOverImage(image, current_file, returned_rects);

    }



}


#endif // DETECTION_H

