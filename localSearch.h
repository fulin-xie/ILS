#ifndef LOCALSEARCH_H
#define LOCALSEARCH_H
#include <vector>
#include <list>
#include "data.h"


class LocalSearch
{
private:
    const double epsilon;
    Route MinCostInsertion(Route InsertedRoute, Customer InsertedCustomer,
                           double alpha, double beta, double gamma);

public:
    // get all neighbors, and store in the AllNeighbors and TemRouteList;
    void relocate(Solution CurrentSolution, std::vector<Solution>& AllNeighbors,
                  std::list<Route>& TemRouteList, double alpha, double beta,
                  double gamma);

    void swap(Solution CurrentSolution, std::vector<Solution>& AllNeighbors,
              std::list<Route>& TemRouteList, double alpha, double beta,
              double gamma);


    void TwoOptAsterisk(Solution CurrentSolution, std::vector<Solution>& AllNeighbors,
                        std::list<Route>& TemRouteList, double alpha, double beta,
                        double gamma);

    void CrossExchange(Solution CurrentSolution, std::vector<Solution>& AllNeighbors,
                       std::list<Route>& TemRouteList, double alpha, double beta,
                       double gamma);



    //constructor
    LocalSearch();
    ~LocalSearch();
};

#endif // LOCALSEARCH_H
