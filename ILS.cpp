#include "ILS.h"
#include "dataInitialise.h"
#include "initialSolution.h"
#include "localSearch.h"
#include <iostream>

using namespace std;

ILS::ILS(std::string FileDirectory):alpha(1), beta(1), gamma(1),
    MaxIterNum(100), epsilon(0.000001)
{
    Initialisation(FileDirectory);
}
ILS::~ILS()
{
    //clear the distance matrix
    for(int i=0; i<this->NodeCount; ++i){
        delete[] DistanceMatrix[i];
    }
    delete[] DistanceMatrix;

}

void ILS::Initialisation(std::string FileDirectory)
{
    this->VehicleCount = 10;
    DataInitialise DataReader;
    DataReader.ImportNodeData(FileDirectory);
    this->NodeList = DataReader.NodeList();
    //get the first node and last node
    Node node1 = NodeList[0];
    Node node2 = NodeList[NodeList.size()-1];

    DepotStart = new Depot(node1.id(), node1.xcoord(), node1.ycoord(),
                           node1.demand(), node1.ReadyTime(), node1.DueDate(),
                           node1.ServiceTime(), node1.id());
    DepotEnd = new Depot(node2.id(), node2.xcoord(), node2.ycoord(),
                         node2.demand(), node2.ReadyTime(), node2.DueDate(),
                         node2.ServiceTime(), node2.id());
    // calculate distance matrix
    DataReader.GetDistanceMatrix();
    this->NodeCount = (int)this->NodeList.size();
    //in the node list, the first and last node are depot
    this->CustomerCount = NodeCount - 2;
    DataReader.DisplayData();
}


void ILS::RunModel()
{
    InitialSolution InitialModel(DepotStart, DepotEnd, &NodeList,VehicleCount,
                                 CustomerCount, alpha, beta, gamma);
    InitialModel.GetInitialSolution(CustomerSorted, AllRouteList, SolutionList);
    Solution InitialSolution = SolutionList.front();
    InitialSolution.DisplaySolution();
    RunILS();
}





void ILS::RunILS()
{
    Solution CurrentSolution = SolutionList[0];
    int IterNum = 0; //current iteration number
    //initialisation
    Solution BestSolution = CurrentSolution;

    while(StopCriteriaMet(IterNum) == false){
        Solution BestNeighbor = RunLocalSearch(CurrentSolution);
        if(CurrentSolution.ObjectiveValue() - BestNeighbor.ObjectiveValue() > epsilon){
          CurrentSolution = BestNeighbor;
        }
        std::cout << "Iteration: " << IterNum << "  Objective:  "
                  << CurrentSolution.ObjectiveValue() << endl;
        ++ IterNum;
    }

    CurrentSolution.DisplaySolution();
}

Solution ILS::RunLocalSearch(Solution CurrentSolution)
{
    Solution BestNeighbor;
    vector<Solution> NeighborSolutions;
    list<Route> TemRouteList;

    LocalSearch LS;
    LS.swap(CurrentSolution, NeighborSolutions, TemRouteList, alpha, beta, gamma);
    double MinObjValue = 9999999999999; //initialise with a huge value
    for(int i=0; i<(int) NeighborSolutions.size(); ++i){
        if(MinObjValue - NeighborSolutions[i].ObjectiveValue()>epsilon){
            BestNeighbor = NeighborSolutions[i];
            MinObjValue = BestNeighbor.ObjectiveValue();
        }
    }

    vector<Route*> RouteList = BestNeighbor.RouteList();
    //get two new generated routes
    Route RouteOne = *RouteList[RouteList.size()-1];
    Route RouteTwo = *RouteList[RouteList.size()-2];
    //insert to the AllRouteList;
    this->AllRouteList.push_back(RouteOne);
    this->AllRouteList.push_back(RouteTwo);
    RouteList.pop_back();
    RouteList.pop_back();

    list<Route>::iterator it = this->AllRouteList.end();
    --it;
    RouteList.push_back(& *it);
    --it;
    RouteList.push_back(& *it);
    BestNeighbor.SetRouteList(RouteList);

    return BestNeighbor;
}


bool ILS::StopCriteriaMet(int IterNum)
{
    if(IterNum > MaxIterNum){return true;}
    else{return false;}
}




























