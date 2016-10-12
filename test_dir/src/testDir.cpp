#include "dirent.h"
#include "stddef.h"
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include "stdio.h"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

using namespace std;
using namespace cv;

DIR *dir;
struct dirent *ent;

int main()
{
	int counter = 0;
	
	if ((dir = opendir("/Users/biagiomontesano/Documents/wise_robotics/test_dir/src/images")) != NULL) 
	{
		/* print all the files and directories within directory */
		while ((ent = readdir(dir)) != NULL)
		{
			printf ("%s\n", ent->d_name);
			stringstream ss_name;
			ss_name << "/Users/biagiomontesano/Documents/wise_robotics/test_dir/src/images";
			ss_name << "/";
			ss_name << ent->d_name;
			Mat image = imread(ss_name.str());
			
			if(!image.data)
				cout << "NULL" << ss_name << endl;
				
			else
				imshow(ent->d_name, image);
			
			counter++;
		}
		
		waitKey(0);
		
		closedir (dir);
	}
	
	else 
	{
		/* could not open directory */
		perror ("");
		return EXIT_FAILURE;
	}
	
	cout << "Counter: " << counter << endl;
}