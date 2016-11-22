#ifndef WEB_UTILS
#define WEB_UTILS

#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/filereadstream.h"

#include <iostream>
#include <fstream>
#include <stdio.h>
#include <string>
#include <curl/curl.h>
#include <algorithm>
#include <sstream>
#include "macros.h"

#define CHECK_ZOOM_LEVEL_URL_STRING "http://dev.virtualearth.net/REST/v1/Imagery/Metadata/Aerial/"

using namespace std;
using namespace rapidjson;

namespace utilities
{
    typedef pair<float, float> coordinate;

    enum OUTPUT_TYPE
    {
            OUTPUT_PNG = 0,
            OUTPUT_JPG = 1,
            OUTPUT_GIF = 2
    };

    Document extractDocumentFromJSON(string file_name)
    {
        // declare two file pointers
        streampos begin,end;

        // open file stream
        ifstream myfile (file_name.c_str(), ios::binary);

        // save initial position
        begin = myfile.tellg();

        // move index to the end of file
        myfile.seekg(0, ios::end);

        // save pointer to end of file
        end = myfile.tellg();

        // close stream
        myfile.close();

        // get a file handler
        FILE* fid = fopen(file_name.c_str(), "r");

        // create a buffer
        char readBuffer[end-begin];

        // open a FileStream
        FileReadStream is(fid, readBuffer, sizeof(readBuffer));

        // create and initialise a document
        Document json_doc;
        json_doc.ParseStream(is);

        // close file handler
        fclose(fid);

        return json_doc;

    }

    int checkZoomLevel(coordinate center)
    {

        // output value
        int allowed_zoom = -1;

        // iterate over possible zoom levels
        for(int current_level = ZOOM_LEVEL; current_level>=MIN_ZOOM_LEVEL; current_level--)
        {
            // prepare file to host response
            remove("result.json");
            FILE* fid_json = fopen("result.json", "a");
            if(fid_json == NULL)
            {
                cout << "Could not open file to store JSON response. Program will be terminated" << endl;
                exit(-1);
            }


            // compose URL
            stringstream ss_url;
            ss_url << CHECK_ZOOM_LEVEL_URL_STRING << to_string(center.first) << "," <<
                      to_string(center.second) << "?&zoomLevel=" << current_level <<
                      "&key=" << BING_KEY;

            // prepare connection
            CURL* curl;
            CURLcode res;
            curl = curl_easy_init();

            // start connection
            if(curl)
            {
                // connection options
                curl_easy_setopt(curl, CURLOPT_URL, ss_url.str().c_str());
                curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void*)fid_json);

                // perform request
                res = curl_easy_perform(curl);

                // error management
                if(res != CURLE_OK)
                {
                    fprintf(stderr, "Zoom check error: curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
                    exit(-1);
                }

                // cleanup
                curl_easy_cleanup(curl);
            }

            // close file
            fclose(fid_json);

            // convert JSON to object
            Document json_doc = extractDocumentFromJSON("result.json");

            // check zoom availability
            if(json_doc["resourceSets"][0]["resources"][0]["vintageEnd"].IsNull())
            {
                cout << "Could not download image at (" << to_string(center.first) << ", " << to_string(center.second) <<
                        ") at zoom level " << current_level << endl;
                continue;
            }
            else
            {
                allowed_zoom = current_level;
                cout << "Found image at (" << to_string(center.first) << ", " << to_string(center.second) <<
                        ") at zoom level " << current_level << endl;
                break;
            }


        }

        // return result
        return allowed_zoom;

    }

    void getImagesFromCoordinates(const vector<coordinate>& centers, string output_path, /*int zoom_level,*/ string bing_key, OUTPUT_TYPE type = OUTPUT_JPG)
    {
            // loop aver centers' coordinates
            for(int centers_counter = 0; centers_counter < centers.size(); centers_counter++)
            {
                    // get a coordinate
                    coordinate current_coord = centers[centers_counter];

                    // prepare URL structure
                    stringstream url_ss;

                    // insert left part of URL
                    string url_left_part = "http://dev.virtualearth.net/REST/V1/Imagery/Map/Aerial/";
                    url_ss << url_left_part;

                    // insert center's coordinate
                    url_ss << current_coord.first << "," << current_coord.second << "/";

                    // insert zoom level
                    int available_level = checkZoomLevel(current_coord);
                    if(available_level == -1)
                    {
                        cout << "Could not download image at a zoom level between " << MIN_ZOOM_LEVEL << " and " << ZOOM_LEVEL <<
                                endl;
                        continue;
                    }

                    else
                        url_ss << available_level << "?&";

                    // insert key
                    url_ss << "key=" << bing_key;

                    // prepare output file's name
                    stringstream file_name;

                    string first_string = to_string(current_coord.first);
                    string second_string = to_string(current_coord.second);
                    replace(first_string.begin(), first_string.end(), '.', 'x');
                    replace(second_string.begin(), second_string.end(), '.', 'x');

                    file_name << output_path << "/" << first_string << "_" << second_string << "_" << available_level;

                    // add extension
                    switch(type)
                    {
                        case OUTPUT_GIF:
                            file_name << ".gif";
                            break;
                        case OUTPUT_JPG:
                            file_name << ".jpg";
                            break;
                        case OUTPUT_PNG:
                            file_name << ".png";
                            break;
                    }

                    // open file
                    FILE* fid = fopen(file_name.str().c_str(), "a");
                    if(fid == NULL)
                            cout << "Could not open file " << file_name.str().c_str() << endl;

                    // prepare connection
                    CURL *curl;
                    CURLcode res;
                    curl = curl_easy_init();


                    // start connection
                    if(curl)
                    {
                            // connection options
                            curl_easy_setopt(curl, CURLOPT_URL, url_ss.str().c_str());
                            curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void*)fid);

                            // perform request
                            res = curl_easy_perform(curl);

                            // error management
                            if(res != CURLE_OK)
                                    fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));

                            // cleanup
                            curl_easy_cleanup(curl);

                            // close file
                            fclose(fid);
                    }

            }

    }

}

#endif
