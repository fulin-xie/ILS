#ifndef INITIALSOLUTION_H
#define INITIALSOLUTION_H
#include <list>
#include <vector>
#include "data.h"


class InitialSolution
{
private:
    static const double epsilon;

    Depot* DepotStart;
    Depot* DepotEnd;
    std::vector<Node>* NodeList;
    int VehicleCount, CustomerCount;
    double alpha, beta, gamma;

    void SortCustomer(std::vector<Customer>& CustomerSorted);
    static bool DataSortPredicate(const Customer& c1, const Customer& c2);
    bool DurationConstraint(Customer &InsertCustomer, int InsertPosition,
                       std::vector<Customer> &CustomersVisited);

public:
    InitialSolution(Depot* StartDepot, Depot* EndDepot, std::vector<Node>* NodeList1,
                    int VehicleNum, int CustomerNum, double alpha0,
                    double beta0, double gamma0);
    void GetInitialSolution(std::vector<Customer>& CustomerSorted,
                            std::list<Route>& AllRouteList,
                            std::vector<Solution>& SolutionList);


    ~InitialSolution();
};

#endif // INITIALSOLUTION_H
