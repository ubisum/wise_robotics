#include "utilities.h"

using namespace utilities;

int main()
{
    vector<fileInfo> info = getInfoFromImages("test");
    printCentersOfSquares(info, "centersOfSquares.csv");
}
