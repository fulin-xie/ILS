#include "initialSolution.h"

#include <time.h>
#include <algorithm>
#include <iostream>

using namespace std;

const double InitialSolution::epsilon = 0.000001;
InitialSolution::InitialSolution(Depot *StartDepot, Depot *EndDepot,std::vector<Node>
                                 *NodeList1,int VehicleNum,int CustomerNum,
                                 double alpha0, double beta0,double gamma0)
{
    this->DepotStart = StartDepot;
    this->DepotEnd = EndDepot;
    this->NodeList = NodeList1;
    this->VehicleCount= VehicleNum;
    this->CustomerCount = CustomerNum;
    this->alpha = alpha0;
    this->beta = beta0;
    this->gamma = gamma0;
}


InitialSolution::~InitialSolution()
{

}

void InitialSolution::GetInitialSolution(vector<Customer> &CustomerSorted,list<Route>
                                         &AllRouteList, vector<Solution>& SolutionList)
{
    //sort the customer based on the angle to the depot
    SortCustomer(CustomerSorted);

    srand(time(0));
    //generate a random number in the rand 0 and 99, it determines the first selected customer
    //int i = rand() % 100;
    int i = 39;
    int RandomNum = i;
    int NumOfInsertion = 0; // the number of customers have been inserted to vehicles' route
    int PathCount = 0;
    vector<Route*> PathsInNewSolution;
    while(NumOfInsertion<(int)CustomerSorted.size()){
        vector<Customer> CustomersInPath; // list of customers
        double PathLoad = 0;
        bool NewPathComplete = false;
        while((PathLoad <= VehicleCapacity) && NewPathComplete == false
              && (NumOfInsertion < (int)CustomerSorted.size())){
            //if the end of the list of sorted customers are met,
            //set the i = 0, which is the start of the list
            if(i==100){i=0;}
            Customer c= CustomerSorted[i]; // select a customer from the list
            NewPathComplete = true;
            Customer TempCustomer = c;
            vector<Customer> TempCustomersInPath = CustomersInPath;
            if((c.demand()+PathLoad)<VehicleCapacity){
                if(CustomersInPath.empty()){
                    if(DurationConstraint(TempCustomer,0,TempCustomersInPath)==true){
                        // push the copy of the customer into the vector
                        c = TempCustomer;
                        CustomersInPath = TempCustomersInPath;
                        PathLoad = PathLoad + c.demand();
                        i++;
                        NumOfInsertion++;
                        NewPathComplete = false;
                    }
                }
                else{
                    // find the first customer whose ready time is
                    // higher than the inserted customer
                    vector<Customer>::iterator it = CustomersInPath.begin();
                    int index = 0;
                    while(it !=CustomersInPath.end() &&
                          // if no such insertion place found,
                          //the insert customer to the end of the path
                          (c.ReadyTime() >= (*it).ReadyTime())){
                        it++;
                        index++;
                    }
                    if(DurationConstraint(TempCustomer, index, TempCustomersInPath) ==true){
                        c = TempCustomer;
                        CustomersInPath = TempCustomersInPath;
                        NumOfInsertion++;
                        PathLoad = PathLoad + c.demand();
                        i++;
                        NewPathComplete = false;
                    }
                }
                //the customer just inserted is the last customer
                if(NumOfInsertion == (int)CustomerSorted.size()){
                    NewPathComplete = true;
                }
            }
            if(NewPathComplete == true){ //the path capacity is full

                //if this path is the last vehicle, and there are customers left,
                //insert customer into this path
                if(((int)PathsInNewSolution.size() == (VehicleCount-1))
                        && (NumOfInsertion<(int)CustomerSorted.size())){
                    while(NumOfInsertion<(int)CustomerSorted.size()){
                        Customer CustomerLeft = CustomerSorted[i];
                        vector<Customer>::iterator it = CustomersInPath.begin();
                        while(it !=CustomersInPath.end() &&
                              (c.ReadyTime() >= (*it).ReadyTime())){
                            it++;
                        }
                        CustomersInPath.insert(it, CustomerLeft);
                        i++;
                        NumOfInsertion++;
                    }
                }
                Route NewRoute(*DepotStart, *DepotEnd, CustomersInPath);
                NewRoute.SetVehicleId(PathCount);
                AllRouteList.push_back(NewRoute);
                list<Route>::iterator it1 = AllRouteList.end();
                --it1; // iterator to the last item of the list
                 //store the address of the last item into the path list of the new solution
                PathsInNewSolution.push_back(& *it1);
                PathCount++;
            }
        }
    }
    //insert other empty routes into the solution
    while((int)PathsInNewSolution.size() < VehicleCount){
        Route NewRoute;
        NewRoute.SetVehicleId(PathCount);
        AllRouteList.push_back(NewRoute);
        list<Route>::iterator it2 = AllRouteList.end();
        --it2; // iterator to the last item of the list
        PathsInNewSolution.push_back(& *it2);
        PathCount++;
    }
    cout <<" Path Count:  " << PathsInNewSolution.size() << endl;
    Solution NewSolution(PathsInNewSolution);
    NewSolution.GetObjectiveValue(alpha, beta, gamma);
    SolutionList.push_back(NewSolution); // store the solution into the solution list
}

//Check if the duration constraints respect
bool InitialSolution::DurationConstraint(Customer &InsertCustomer, int InsertPosition,
                                    std::vector<Customer> &CustomersVisited){

    vector<Customer>::iterator it = CustomersVisited.begin();
    advance(it, InsertPosition);
    // insert customer into the path, at least one customer in the path
    CustomersVisited.insert(it,InsertCustomer);
    Route NewPath(*DepotStart, *DepotEnd, CustomersVisited);
    if(NewPath.DurationViolation() <= 0){return true;}
    else{return false;}
}

void InitialSolution::SortCustomer(vector<Customer>& CustomerSorted)
{
    const double PI=3.14159265;
    //sort the customer in by the increasing order of the anlgle they
    // made with depot and an arbitrary radius
    // calculate the angles

    for(int i=0; i<(int)NodeList->size(); ++i){
        Node node = (*NodeList)[i];
        //calculate customer angle to the depot, and push to the vector
        if(node.id() !=0 && node.id() != CustomerCount+1){
            Customer c = Customer(node.id(), node.xcoord(), node.ycoord(),
                                  node.demand(), node.ReadyTime(), node.DueDate(),
                                  node.ServiceTime(), node.id());
            double x, y, angle;
            x = c.xcoord() - DepotStart->xcoord();
            y = c.ycoord() - DepotStart->ycoord();
            angle = atan2(y,x)*180/PI;
            if(angle >= 0){ c.SetAngle(angle);}
            else{ c.SetAngle(360 + angle);}
            CustomerSorted.push_back(c);
        }
    }
    std::sort(CustomerSorted.begin(), CustomerSorted.end(),DataSortPredicate);
}


bool InitialSolution::DataSortPredicate(const Customer& c1, const Customer& c2){
    bool c1IsFront = false;
    if(c2.angle() - c1.angle() > epsilon){
        c1IsFront = true;
    }
    else if(c2.angle() == c1.angle()){ // these two customers have the same angle
        if(c1.id() < c2.id()){
            c1IsFront = true;
        }
    }
    else{
        c1IsFront = false;
    }

    return c1IsFront;
}

















