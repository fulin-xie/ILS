#include "dataInitialise.h"
#include <fstream>
#include <sstream>
#include <math.h>
#include <iostream>

using namespace std;

DataInitialise::DataInitialise()
{
    this->NodeCount = 0;
}

DataInitialise::~DataInitialise()
{
}

void DataInitialise::ImportNodeData(std::string FileDirectory)
{
    ifstream InputFile;
    InputFile.open(FileDirectory);
    int LineCount = 0;
    // count the number of nodes
    if(InputFile.is_open()){
        string str;
        while(!InputFile.eof()){
            getline(InputFile, str);
            LineCount++;
            this->NodeCount = LineCount;
        }
        if(LineCount > 0){
            InputFile.close();
        }
        else{
            cout << "The file is empty" << endl;
            exit(1);
        }
    }
    else{
        cout << "File could not be opened." << endl;
        exit(1);
    }

    // read date into map
    InputFile.open(FileDirectory);
    if(InputFile.is_open()){
        // define 2D dynamic array
        double** items = new double* [LineCount];
        for(int i=0; i<LineCount; i++){
            items[i] = new double[7];
        }
        while(!InputFile.eof()){
            for(int i=0; i<LineCount; i++){
                for(int j=0; j<7; j++){
                   InputFile >> items[i][j];
                }
            }
        }
        for(int i=0; i<LineCount; i++){
            //for the internal id, the depot id is 0, customer id start from 1
            Node node((int)items[i][0],items[i][1],items[i][2],
                    items[i][3],items[i][4],items[i][5],items[i][6],
                    (int)NodeList0.size());
            this->NodeList0.push_back(node);
        }
        Node DepotStart = NodeList0[0];
        Node DummyDepot(DepotStart.ExternalId(), DepotStart.xcoord(), DepotStart.ycoord(),
                        DepotStart.demand(), DepotStart.ReadyTime(), DepotStart.DueDate(),
                        DepotStart.ServiceTime(), DepotStart.id());
        this->NodeList0.push_back(DummyDepot);
        // clean up
        for(int i=0; i<LineCount; i++){
            delete []items[i];
        }
        delete [] items;
    }
}

void DataInitialise::GetDistanceMatrix()
{
    // set up the distance matrix
    int NodeCount = (int)this->NodeList0.size();
    DistanceMatrix = new double* [NodeCount];
    for(int i=0; i<NodeCount;++i){
        DistanceMatrix[i] = new double[NodeCount];
    }
    //initial values of the array, first get distance between customers
    for(int m=0; m<NodeCount; ++m){
        for(int j=0; j<NodeCount; ++j){
            double distance;
            if(m==j){
                distance = 0;}
            else{
                Node n1 = this->NodeList0[m];
                Node n2= this->NodeList0[j];
                distance = sqrt(pow(n1.xcoord()-n2.xcoord(), 2)
                                + pow(n1.ycoord()-n2.ycoord(), 2));
            }
            DistanceMatrix[m][j] = distance;
        }
    }
}

void DataInitialise::DisplayData()
{
    for(int i=0; i<NodeCount; ++i){
        for(int j=0; j<NodeCount; ++j){
            cout << i << " & " << j << " : "
                 <<DistanceMatrix[i][j] << endl;
        }
    }
}



