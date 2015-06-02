#include <iostream>
#include <math.h>
#include <time.h>
#include <string>
#include "ILS.h"

using namespace std;

int main()
{
    string FileDirectory = "C:/C++/ILS_Model/ILS/data/InstancesR104.txt";

    ILS model(FileDirectory);
    model.RunModel();



    return 0;
}

