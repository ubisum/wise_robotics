#ifndef UTILITIES_H
#define UTILITIES_H

#include "dirent.h"
#include <string>
#include <vector>
#include <iostream>
#include <cstdlib>
#include <unistd.h>
#include <sys/stat.h>
#include <cstring>

#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

using namespace std;
using namespace cv;

namespace utilities
{
typedef vector<Mat> vectorOfMats;

int remove_directory(const char *path)
{
    // global variables
    DIR *d = opendir(path);
    size_t path_len = strlen(path);
    int r = -1;

    // directory was correcty opened
    if (d)
    {
        struct dirent *p;
        r = 0;

        while (!r && (p=readdir(d)))
        {
            int r2 = -1;
            char *buf;
            size_t len;

            /* Skip the names "." and ".." as we don't want to recurse on them. */
            if (!strcmp(p->d_name, ".") || !strcmp(p->d_name, ".."))
            {
                continue;
            }

            len = path_len + strlen(p->d_name) + 2;
            buf = (char*)malloc(len);

            if (buf)
            {
                 struct stat statbuf;

                 snprintf(buf, len, "%s/%s", path, p->d_name);

                 if (!stat(buf, &statbuf))
                 {
                        if (S_ISDIR(statbuf.st_mode))
                        {
                           r2 = remove_directory(buf);
                        }
                        else
                        {
                           r2 = unlink(buf);
                        }
                 }

                 free(buf);
            }

            r = r2;
        }

        closedir(d);
    }

    if (!r)
    {
          r = rmdir(path);
    }

    return r;
}
	
vector<string> getFilesList(string path)
{
        // data for access to folder
        DIR *dir;
        struct dirent *ent;

        // output
        vector<string> listOfFiles;

        // try to open folder at specified path
        if ((dir = opendir(path.c_str())) != NULL)
        {
                // iterate over elements in folder
                while ((ent = readdir(dir)) != NULL)
                        // check i-the element is a valid file on Mac OS's filesystem
                        if(strcmp(".", ent->d_name) != 0 && strcmp("..", ent->d_name) != 0)
                                // add valid file's name to list
                                listOfFiles.push_back(ent->d_name);
        }

        // print error message if folder couldn't be opened
        else
        {
                cout << "Impossible to open folder "  << path.c_str() << endl << endl;
        }

        // return list of files
        return listOfFiles;
}
	
void getSquaresFromImage(string img_path, int square_side, int square_step_row, int square_step_col, string output_path, string prefix)
{

        // try to open image
        Mat input_image = imread(img_path.c_str());

        // image was opened
        if(input_image.data)
        {
                // check image can contain at least a windows of the specified size
                if(input_image.rows >= square_side && input_image.cols >= square_side)
                {
                        // prepare parameters for storing image
                        vector<int> compression_params;
                        compression_params.push_back(CV_IMWRITE_JPEG_QUALITY);
                        compression_params.push_back(95);

                        // image counter
                        int squaresCounter = 0;

                        // until it's possible to move window horizontally
                        for(int i = 0; i+square_side<input_image.rows; i+=square_side)
                        {
                                // until it's possible to move window vertically
                                for(int j = 0; j+square_side<input_image.cols; j+=square_side)
                                {
                                        // cut a submatrix
                                        Mat subimage(input_image, Rect(j,i,square_side, square_side));

                                        // create the path to output file
                                        stringstream file_name;
                                        file_name << output_path << "/" << prefix << "_" << squaresCounter << "_" << i << "_" << j << "_" << square_side << ".jpg";

                                        // write image
                                        imwrite(file_name.str(), subimage, compression_params);

                                        // increment counter
                                        squaresCounter++;

                                        // try first shift
                                        if(i+square_side+square_step_row<input_image.rows && j+square_side+square_step_col<input_image.cols)
                                        {
                                                // create submatrix
                                                Mat shift1 = Mat(input_image, Rect(j+square_step_col, i+square_step_row, square_side, square_side));

                                                // prepare image's name
                                                stringstream shift1_name;
                                                shift1_name << output_path << "/" << prefix << "_" << squaresCounter << "_" << i+square_step_row <<
                                                                        "_" << j+square_step_col << "_" << square_side << ".jpg";

                                                // store image
                                                imwrite(shift1_name.str(), shift1, compression_params);

                                                // increment counter
                                                squaresCounter++;
                                        }

                                        // try second shift
                                        if(i+square_side+2*square_step_row<input_image.rows && j+square_side+2*square_step_col<input_image.cols)
                                        {
                                                // create submatrix
                                                Mat shift2 = Mat(input_image, Rect(j+2*square_step_col, i+2*square_step_row, square_side, square_side));

                                                // prepare image's name
                                                stringstream shift2_name;
                                                shift2_name << output_path << "/" << prefix << "_" << squaresCounter << "_" << i+2*square_step_row <<
                                                                        "_" << j+2*square_step_col << "_" << square_side << ".jpg";

                                                // store image
                                                imwrite(shift2_name.str(), shift2, compression_params);

                                                // increment counter
                                                squaresCounter++;
                                        }

                                }
                        }

                        // cover right stripe
                        if(input_image.cols % square_side != 0)
                        {
                                // computing index of starting column
                                int column_index = input_image.cols - 1 - square_side;

                                // iterate along column
                                for(int counter_i = 0; counter_i+square_side<input_image.rows; counter_i+=square_side)
                                {
                                        // create submatrix
                                        Mat left_subimg = Mat(input_image, Rect(column_index, counter_i, square_side, square_side));

                                        // create the path to output file
                                        stringstream file_name;
                                        file_name << output_path << "/" << prefix << "_" << squaresCounter << "_" << counter_i << "_" <<
                                                          column_index << "_" << square_side << ".jpg";

                                        // store image
                                        imwrite(file_name.str(), left_subimg, compression_params);

                                        // increment counter
                                        squaresCounter++;
                                }
                        }

                        // cover lower stripe
                        if(input_image.rows % square_side != 0)
                        {
                                // compute starting index or row
                                int row_index = input_image.rows - 1 - square_side;

                                // iterate along row
                                for(int counter_j = 0; counter_j+square_side<input_image.cols; counter_j+=square_side)
                                {
                                        // create submatrix
                                        Mat lower_subimg = Mat(input_image, Rect(counter_j, row_index, square_side, square_side));

                                        // create the path to output file
                                        stringstream file_name;
                                        file_name << output_path << "/" << prefix << "_" << squaresCounter << "_" << row_index << "_" <<
                                                          counter_j << "_" << square_side << ".jpg";

                                        // store image
                                        imwrite(file_name.str(), lower_subimg, compression_params);

                                        // increment counter
                                        squaresCounter++;
                                }
                        }


                }

                // image can't host any window
                else
                {
                        cout << "Image " << img_path << " has size " << input_image.rows << "x" <<
                                 input_image.cols << " and can't host squares " << square_side << "x" << square_side << endl << endl;
                }

        }

        // image couldn't be read
        else
        {
                cout << "Could not load image " << img_path << endl << endl;
        }
}
	
void getSquaresInFolder(string folder_path, int square_side, int square_step_row, int square_step_col, string output_path)
{
        // get files in requested folder
        vector<string> filesInFolder = getFilesList(folder_path);

        // extract squares iteratively
        for(int counter = 0; counter<filesInFolder.size(); counter++)
        {
                // get a files's name
                string current_string = filesInFolder[counter];
                cout << "Processing image " << folder_path << "/" << current_string << " ..." << endl;

                // get file's name without extension
                char converted_string[current_string.length()];
                current_string.copy(converted_string, current_string.length());
                char* no_extension = strtok(converted_string, ".");

                // compute squares
                stringstream complete_name;
                complete_name << folder_path << "/" << current_string;
                getSquaresFromImage(complete_name.str(), square_side, square_step_row, square_step_col, output_path, string(no_extension));

                // notify end of iteration
                cout << "Done" << endl;

        }
}
}

#endif
