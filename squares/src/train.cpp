#include "train.h"
#include "macros.h"

#include <stdio.h>
#include <string>
#include <iostream>
#include <sstream>

using namespace std;

int main()
{
    // remove data from previous executions
    remove("positives.txt");
    remove("negatives.txt");

    // create text file about positive and negative samples
    execute_command(POS_SAMPLES_TXT);
    execute_command(NEG_SAMPLES_TXT);

    //  create .vec files command
    stringstream vec_command;
    vec_command << PERL_SCRIPT_CMD;
    vec_command << NUM_SAMPLES << " \"opencv_createsamples -bgcolor " << BGCOLOR << " -bgthresh " << BGTHRESH <<
                   " -maxxangle " << MAX_X_ANGLE << " -maxyangle " << MAX_Y_ANGLE <<
                   " maxzangle "  << MAX_Z_ANGLE << " -maxidev " << MAX_I_DEV << " -w " << SQUARE_SIDE <<
                   " -h " << SQUARE_SIDE << "\"";

    // create .vec files
    execute_command(vec_command.str().c_str());
}
