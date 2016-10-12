#ifndef CSV_H
#define CSV_H

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <cstring>

using namespace std;

namespace utilities
{
	typedef pair<string, bool> csvRow;
	
	void writeCSV(vector<string> images, vector<bool> responses, string file_path)
	{
		// open file
		FILE* fid = fopen(file_path.c_str(), "a");
		
		for(int counter = 0; counter<images.size(); counter++)
		{
			// prepare a string
			stringstream ss;
			
			// add image's name
			ss << images[counter] << ",";
			
			// add response for i-th image
			if(responses[counter])
				ss << "tree,";
			else
				ss << "nil,";
				
			// add new line
			if(counter != images.size()-1)
			ss << "\n";
			
			// write to file
			fputs(ss.str().c_str(), fid);
		}
		
		// close file
		fclose(fid);
	}
	
	vector<csvRow> readCSV(string file_path)
	{
		// open file
		ifstream csv_fid(file_path.c_str());
		
		// create a support string
		string support;
		
		// output file
		vector<csvRow> output;
		
		// read file's line
		while(getline(csv_fid, support))
		{
			// prepare a pair
			csvRow row;
			cout << support << endl;
			
			// get first string
			char converted_string[support.length()];
			support.copy(converted_string, support.length());
			char* current_string = strtok(converted_string, ",");
			cout << current_string << endl;
			
			if(current_string != NULL)
				row.first = string(current_string);
				
			else
			{
				cout << "Could not read file " << file_path << endl;
				exit(-1);
			}
			
			current_string = strtok(NULL, ",");
			cout << string(current_string) << endl;
			
			if(current_string != NULL)
			{
				if(strcmp(current_string, "tree") == 0)
					row.second = true;
					
				else
					row.second = false;
			}
			
			else
			{
				cout << "Could not read file " << file_path << endl;
				exit(-1);
			}
			
			output.push_back(row);
		}
		
		return output;
	}
}

#endif