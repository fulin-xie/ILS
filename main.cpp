#include <iostream>
#include <math.h>
#include <time.h>
#include <string>
#include "ILS.h"

using namespace std;

int main()
{
    string FileDirectory = "C:/C++/VRPTW//heuristics/InstancesR104.txt";

    ILS model(FileDirectory);
    model.RunModel();



    return 0;
}

