#ifndef DATACLASS_H
#define DATACLASS_H
#include <vector>
#include <map>

extern double VehicleCapacity;
extern double** DistanceMatrix;

// the class of all notes
class Node
 {
    int ExternalId0; // the id from the raw data source
    double xcoord0;
    double ycoord0;
    double demand0;
    double ReadyTime0;
    double DueDate0;
    double timeAtNode0; //the time spent at the node, usually the service time

    int id0; // this is the node id in the NodeList, Depot id is -1, customer id start from 0
    double ArrivalTime0, StartTime0, tardiness0, CompletionTime0;

public:
    void SetTardiness(double tardiness){this->tardiness0=tardiness;}
    void SetCompletionTime(double CompletionTime){this->CompletionTime0=CompletionTime;}
    void SetStartTime(double StartTime){this->StartTime0=StartTime;}
    void SetArrivalTime(double ArrivalTime){this->ArrivalTime0=ArrivalTime;}

    int id() const {return this->id0;}
    int ExternalId(){return this->ExternalId0;}
    double xcoord(){return this->xcoord0;}
    double ycoord(){return this->ycoord0;}
    double demand(){return this->demand0;}
    double ReadyTime() {return this->ReadyTime0;}
    double DueDate() {return this->DueDate0;}
    double ServiceTime() {return this->timeAtNode0;}
    double tardiness(){return this->tardiness0;}
    double CompletionTime(){return this->CompletionTime0;}
    double StartTime(){return this->StartTime0;}
    double ArrivalTime(){return this->ArrivalTime0;}
    //constructor
    Node();
    Node (int Id, double x, double y, double demand,
          double ReadyT, double DueT, double ServiceTime, int InternalId);
};

class Depot : public Node{
public:
    Depot(int Id, double x, double y, double demand,double ReadyT,
          double DueT, double ServiceTime, int InternalId)
        :Node(Id,  x, y, demand, ReadyT, DueT, ServiceTime, InternalId){}
    Depot():Node(){}
};

class Customer : public Node{
private:
    double angle0;

public:
    void SetAngle(double angle){angle0=angle;}
    double angle()const{return angle0;}
    //constructor
    Customer(int Id, double x, double y, double demand,double ReadyT,
             double DueT, double ServiceTime, int InternalId)
        :Node(Id, x, y, demand, ReadyT, DueT, ServiceTime, InternalId){
        angle0 = 0;}
};


class Route
{
private:
    int VehicleId0;
    double CapacityViolation0, DurationViolation0, TimeWindowViolation0;
    double TimeUsed0; // the total time used by this route
    double DriveDistance0;
    double TotalLoad0;
    double ObjectiveValue0;
    Depot DepotStart0;
    Depot DepotEnd0;
    //the list of id that visited by this route
    std::vector<Node> VisitList0;
    std::vector<Customer> CustomerList0;

    // calculate the total cost of this path
    void GetTotalLoad();
    void GetDriveDistance();
    // calculate the vehicle arrival time, service complete time of each customer
    void GetTimeInformation();
public:
    int VehicleId(){return this->VehicleId0;}
    double CapacityViolation(){return this->CapacityViolation0;}
    double DurationViolation(){return this->DurationViolation0;}
    double TimeWindowViolation(){return this->TimeWindowViolation0;}
    double TimeUsed(){return this->TimeUsed0;}
    double ObjectiveValue(){return this->ObjectiveValue0;}
    double DriveDistance(){return this->DriveDistance0;}
    double TotalLoad(){return this->TotalLoad0;}
    Depot DepotStart(){return this->DepotStart0;}
    Depot DepotEnd(){return this->DepotEnd0;}
    std::vector<Customer> CustomerList(){return this->CustomerList0;}

    void SetVehicleId(int VehicleId){this->VehicleId0 = VehicleId;}
    void GetObjectiveValue(double alpha, double beta, double gamma);
    void DisplayInfo();
    //constructor
    Route(Depot DepotStart, Depot DepotEnd, std::vector<Customer> CustomerList);
    Route();
};

class Solution{
private:
    double ObjectiveValue0;
    std::vector<Route*> RouteList0;
    double TotalDriveDistance0;
    double TotalCapacityViolation0, TotalDurationViolation0,
    TotalTimeWindowViolation0;

public:
    void SetRouteList(std::vector<Route*> Routes){
        this->RouteList0 = Routes;}
    std::vector<Route*> RouteList(){return this->RouteList0;}
    double TotalDriveDistance(){return this->TotalDriveDistance0;}
    double ObjectiveValue(){return this->ObjectiveValue0;}
    double TotalCapacityViolation(){return this->TotalCapacityViolation0;}
    double TotalDurationViolation(){return this->TotalDurationViolation0;}
    double TotalTimeWindowViolation(){return this->TotalTimeWindowViolation0;}
    void GetObjectiveValue(double alpha, double beta, double gamma);
    double TotalViolation();
    void DisplaySolution();

    //constructor
    Solution(std::vector<Route*> Routes);
    Solution();
};


















































#endif // DATA_H
