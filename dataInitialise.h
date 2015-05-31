#ifndef DATAINITIALISE_H
#define DATAINITIALISE_H
#include <string>
#include <vector>
#include "data.h"

class DataInitialise
{
private:
    //the first node and last node are the depot
    std::vector<Node> NodeList0;
    int NodeCount;

public:
    //the distance matrix betweem nodes
    void DisplayData();
    std::vector<Node> NodeList(){return this->NodeList0;}
    DataInitialise();
    ~DataInitialise();
    void ImportNodeData(std::string FileDirectory);
    void GetDistanceMatrix();
};





#endif // DATAINITIALISATION_H
