#ifndef ILS_H
#define ILS_H
#include <vector>
#include <string>
#include <list>
#include "data.h"

class ILS
{
    //parameters
    const double alpha, beta, gamma;
    const int MaxIterNum;
    const double epsilon;

    //varialbes
    std::vector<Node> NodeList;
    std::vector<Customer> CustomerSorted;
    std::list<Route> AllRouteList;
    std::vector<Solution> SolutionList;
    Depot* DepotStart;
    Depot* DepotEnd;
    int NodeCount, CustomerCount, VehicleCount;

    //*******************************************************
    //methods
    void Initialisation(std::string FileDirectory);
    void RunILS();

    //the stop criteria
    bool StopCriteriaMet(int IterNum);
    Solution RunLocalSearch(Solution CurrentSolution);

public:
    //constructor
    ILS(std::string FileDirectory);
    ~ILS();

    void RunModel();
};

#endif // ILS_H
