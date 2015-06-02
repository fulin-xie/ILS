#include <iostream>
#include <iomanip>
#include <math.h>
#include "data.h"

double VehicleCapacity = 200;
double** DistanceMatrix;

// default constructor
Node::Node():ExternalId0(0), xcoord0(0), ycoord0(0), demand0(0),
    ReadyTime0(0), DueDate0(0), timeAtNode0(0), id0(0), ArrivalTime0(0),
    StartTime0(0), tardiness0(0), CompletionTime0(0){}

// constructor and functions for the class 'Node'
Node::Node (int Id, double x, double y, double demand,double ReadyT,
            double DueT, double ServiceT, int InternalId): ExternalId0(Id), xcoord0(x),
    ycoord0(y), demand0(demand),ReadyTime0(ReadyT), DueDate0(DueT), timeAtNode0(ServiceT),
    id0(InternalId), ArrivalTime0(0),StartTime0(0), tardiness0(0), CompletionTime0(0){}

//*******************************************************************************************
//ROUTE
Route::Route(Depot DepotStart, Depot DepotEnd, std::vector<Customer> Customers):VehicleId0(0),
    CapacityViolation0(0),DurationViolation0(0), TimeWindowViolation0(0), TimeUsed0(0),
    DriveDistance0(0),TotalLoad0(0), ObjectiveValue0(0), DepotStart0(DepotStart),
    DepotEnd0(DepotEnd),CustomerList0(Customers){
    //generate the visit list, which includes the start and end depot
    std::vector<Node> NodeList;
    NodeList.push_back(DepotStart);
    for(int i=0; i<(int)Customers.size(); ++i){
        NodeList.push_back(Customers[i]);
    }
    NodeList.push_back(DepotEnd);
    this->VisitList0 = NodeList;
    //calculate the drive distance, total load, and violation
    GetTotalLoad();
    GetDriveDistance();
    GetTimeInformation();
}

Route::Route():VehicleId0(0),CapacityViolation0(0),DurationViolation0(0),
    TimeWindowViolation0(0), TimeUsed0(0),DriveDistance0(0),TotalLoad0(0),
    ObjectiveValue0(0), CustomerList0(){}

void Route::GetTotalLoad()
{
    double TotalLoad = 0;
    for(int i=0; i<(int)VisitList0.size(); ++i){
        Node* node = &VisitList0[i];
        TotalLoad = TotalLoad + node->demand();
    }

    this->TotalLoad0 = TotalLoad;
    if(TotalLoad > VehicleCapacity){
        this->CapacityViolation0 = TotalLoad - VehicleCapacity;
    }
}

void Route::GetDriveDistance()
{
    double TotalDistance = 0;
    if(VisitList0.size() > 1){
        for(int i=0; i<((int)VisitList0.size()-1);++i){
            Node* node1 = &VisitList0[i];
            Node* node2 = &VisitList0[i+1];
            TotalDistance += DistanceMatrix[node1->id()][node2->id()];
        }
    }
    this->DriveDistance0 = TotalDistance;
}

void Route::GetTimeInformation()
{
    double TotalTardiness = 0;

    if(VisitList0.size() > 1){
        for(int i=1; i<(int) VisitList0.size(); ++i){
            Node* node1 = &VisitList0[i-1];
            Node* node2 = &VisitList0[i];
            node2->SetArrivalTime(node1->CompletionTime()+
                                 DistanceMatrix[node1->id()][node2->id()]);
            if(node2->ArrivalTime() < node2->ReadyTime()){
                node2->SetStartTime(node2->ReadyTime());
            }
            else{
                node2->SetStartTime(node2->ArrivalTime());
            }
            if(node2->ArrivalTime() > node2->DueDate()){
                node2->SetTardiness(node2->ArrivalTime()-node2->DueDate());
                TotalTardiness += node2->tardiness();
            }
            node2->SetCompletionTime(node2->StartTime()+node2->ServiceTime());
        }
    }
    //the last node is the depot
    Node* LastNode = &VisitList0.back();
    this->TimeWindowViolation0 = TotalTardiness;
    this->TimeUsed0 = LastNode->CompletionTime();
    if(TimeUsed0 > LastNode->DueDate()){
        this->DurationViolation0 = this->TimeUsed0 - LastNode->DueDate();
    }
}

void Route::GetObjectiveValue(double alpha, double beta, double gamma)
{
    this->ObjectiveValue0 = DriveDistance0 + alpha*CapacityViolation0 +
            beta*DurationViolation0 + gamma*TimeWindowViolation0;
}

void Route::DisplayInfo()
{
    std::cout << std::endl << "Path id: " << this->VehicleId0 << std::endl;
    std::cout << "Stops : depot->";
    // the output id is the external id which is the id from the external file
    for (std::vector<Customer>::iterator it = CustomerList0.begin();
         it != CustomerList0.end(); it ++)
    {
        Customer c = *it;
        std::cout << (c.ExternalId() - 1) << " -> "; // make the depot has id 0, customer Id start from 1
    }
    std::cout << std::endl << "DriveDistance: " << std::setprecision(6)<< DriveDistance0 // << "  Time Used: " << TimeUsed0
              << " Capacity Violation: " << this->CapacityViolation()
              << " Duration Violation: " << this->DurationViolation()
              << " Time Window Violation: " << this->TimeWindowViolation() <<std::endl;
}

//***********************************************************************************************
//SOLUTION
Solution::Solution(): TotalDriveDistance0(0), TotalCapacityViolation0(0),
    TotalDurationViolation0(0),TotalTimeWindowViolation0(0), RouteList0(){}

Solution::Solution(std::vector<Route *> Routes): TotalDriveDistance0(0), TotalCapacityViolation0(0),
    TotalDurationViolation0(0),TotalTimeWindowViolation0(0), RouteList0(Routes){

    double DriveDistance = 0;
    if(RouteList0.empty()==false){
        for(int i=0; i<(int)RouteList0.size();i++){
            DriveDistance += RouteList0[i]->DriveDistance();
        }
    }
    this->TotalDriveDistance0 = DriveDistance;
}

void Solution::GetObjectiveValue(double alpha, double beta, double gamma)
{
    double CapacityViolation = 0,DurationViolation = 0,
            TimeWindowViolation = 0;

    if(RouteList0.empty()==false){
        for(int i=0; i<(int)RouteList0.size();i++){
             CapacityViolation += RouteList0[i]->CapacityViolation();
             DurationViolation += RouteList0[i]->DurationViolation();
             TimeWindowViolation += RouteList0[i]->TimeWindowViolation();
        }
    }
    this->TotalCapacityViolation0 = CapacityViolation;
    this->TotalDurationViolation0 = DurationViolation;
    this->TotalTimeWindowViolation0 = TimeWindowViolation;

    double objective = 0;
    objective = TotalDriveDistance0 + alpha*TotalCapacityViolation0 +
            beta*TotalDurationViolation0 + gamma*TotalTimeWindowViolation0;

    //objective = alpha*TotalCapacityViolation0 + beta*TotalDurationViolation0
    //+ gamma*TotalTimeWindowViolation0;
    this->ObjectiveValue0 = objective;
}

double Solution::TotalViolation()
{
    double violations = 0;
    violations = TotalCapacityViolation0 + TotalDurationViolation0
            + TotalTimeWindowViolation0;
    return violations;
}

void Solution::DisplaySolution()
{
    int TotalCustomer = 0;

    for(int i=0; i<(int)RouteList0.size(); i++){
        RouteList0[i]->DisplayInfo();
        std::vector<Customer> CustomersList = RouteList0[i]->CustomerList();
        TotalCustomer += (int)CustomersList.size();
    }

    std::cout << std::endl << "Total Drive Distance: " << std::setprecision(6)
              << TotalDriveDistance0 << std::endl
         // << "Total Time Used: " << TotalTimeUsed0 << std::endl
            << "Total Capacity Violation: " << TotalCapacityViolation0 << std::endl
               << "Total Duration Violation: " << TotalDurationViolation0 << std::endl
               << "Total Time window Violation: " << TotalTimeWindowViolation0 << std::endl;

    //std::cout << std::endl << TotalCustomer << std::endl;
}
























