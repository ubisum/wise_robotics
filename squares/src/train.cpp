#include "train.h"
#include "macros.h"
#include "utilities.h"

#include <stdio.h>
#include <string>
#include <iostream>
#include <sstream>

using namespace std;
using namespace utilities;

int main()
{
    // remove data from previous executions
    remove("positives.txt");
    remove("negatives.txt");
    remove_directory("samples");


    // create text file about positive and negative samples
    cout << "Creating samples text files..." << endl;
    execute_command(POS_SAMPLES_TXT);
    execute_command(NEG_SAMPLES_TXT);
    cout << "Done." << endl << endl;

    //  create .vec files command
    cout << "Creating .vec files..." << endl;
    stringstream vec_command;
    vec_command << PERL_SCRIPT_CMD;
    vec_command << NUM_SAMPLES << " \"opencv_createsamples -bgcolor " << BGCOLOR << " -bgthresh " << BGTHRESH <<
                   " -maxxangle " << MAX_X_ANGLE << " -maxyangle " << MAX_Y_ANGLE <<
                   " maxzangle "  << MAX_Z_ANGLE << " -maxidev " << MAX_I_DEV << " -w " << SQUARE_SIDE <<
                   " -h " << SQUARE_SIDE << "\"";

    // create .vec files
    execute_command(vec_command.str().c_str());
    cout << "Done."  << endl << endl;

    // merging .vec files
    cout << "Merging .vec files..." << endl;
    execute_command(MERGE_VEC_CMD);
    cout << "Done." << endl << endl;

    // training
    cout << "Traininig..." << endl;
    vector<string> filesList = getFilesList("negative_samples");
    stringstream training_cmd;
    training_cmd <<  "opencv_traincascade -data classifier -vec samples.vec -bg negatives.txt" <<
                     " -numStages " << NUM_STAGES << " -minHitRate " << MIN_HIT_RATE <<
                     " -maxFalseAlarmRate " << MAX_FALSE_ALARM_RATE << " -numPos " << NUM_POS <<
                     " -numNeg " << /*NUM_NEG*/ filesList.size() << " -w " << SQUARE_SIDE << " -h " << SQUARE_SIDE <<
                     " -mode ALL -precalcValBufSize " << PRE_CALC_VAL_BUF_SIZE <<
                     " -precalcIdxBufSize " << PRE_CALC_IDX_BUF_SIZE;

    cout << training_cmd.str() << endl << endl;

    //execute_command(TRAINING_CMD);
    cout << "Done." << endl << endl;
}
