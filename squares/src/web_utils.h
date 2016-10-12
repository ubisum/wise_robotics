#ifndef WEB_UTILS
#define WEB_UTILS

#include <iostream>
#include <curl/curl.h>
#include <algorithm>

using namespace std;

namespace utilities
{
	typedef pair<float, float> coordinate;

	enum OUTPUT_TYPE
	{
		OUTPUT_PNG = 0,
		OUTPUT_JPG = 1,
		OUTPUT_GIF = 2
	};

	void getImagesFromCoordinates(const vector<coordinate>& centers, string output_path, int zoom_level, string bing_key, OUTPUT_TYPE type = OUTPUT_JPG)
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
			url_ss << zoom_level << "?&";

			// insert key
			url_ss << "key=" << bing_key;

			// prepare output file's name
			stringstream file_name;

			string first_string = to_string(current_coord.first);
			string second_string = to_string(current_coord.second);
			replace(first_string.begin(), first_string.end(), '.', 'x');
			replace(second_string.begin(), second_string.end(), '.', 'x');

			file_name << output_path << "/" << first_string << "_" << second_string;

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
