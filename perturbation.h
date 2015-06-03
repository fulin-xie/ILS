#ifndef PERTURBATION_H
#define PERTURBATION_H

#include "data.h"

class Perturbation
{
public:
    void RandomSwap(Solution CurrentSolution, std::vector<Solution>& AllNeighbors,
                    std::list<Route>& TemRouteList, double alpha, double beta,
                    double gamma);



    //Constructor
    Perturbation();
    ~Perturbation();
};

#endif // PERTURBATION_H
