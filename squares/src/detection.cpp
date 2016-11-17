#include "detection.h"
#include <vector>
#include <iostream>
#include <sys/stat.h>
#include "utilities.h"

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

/*
 * Rect(col, row, num_rows, num_cols)
 */

using namespace std;
using namespace cv;
using namespace utilities;

int main(int argn, char* argv[])
{
    if(argn < 2)
    {
        cout << "Please, insert path to a valid .xml file." << endl;
        exit(-1);
    }

    detectObjects(argv[1]);
}
