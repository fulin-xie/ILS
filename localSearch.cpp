#include "localSearch.h"

using namespace std;

LocalSearch::LocalSearch():epsilon(0.000001)
{

}

LocalSearch::~LocalSearch()
{

}

void LocalSearch::relocate(Solution CurrentSolution, vector<Solution>& AllNeighbors,
                           list<Route>& TemRouteList,double alpha,  double beta, double gamma)
{
    int RouteCount = (int)CurrentSolution.RouteList().size();
    vector<Route*> RouteListCurrent = CurrentSolution.RouteList();
    // loop 1, select a path in which the customer will be removed
    for(int i=0; i< RouteCount; ++i){
        //the route in which the customer will remove
        Route RouteOut = *RouteListCurrent[i];
        vector<Customer> CustomerListRouteOut = RouteOut.CustomerList();

        // loop 2, select a customer to remove from the path selected
        for(int j=0; j<(int)CustomerListRouteOut.size(); j++){
            Customer CustomerOut = CustomerListRouteOut[j];
            vector<Customer> NewCustomerListOut = CustomerListRouteOut;
            //delete the removed customer
            NewCustomerListOut.erase(NewCustomerListOut.begin()+j);
            Route NewRouteOut(RouteOut.DepotStart(), RouteOut.DepotEnd(), NewCustomerListOut);
            NewRouteOut.SetVehicleId(RouteOut.VehicleId());

            // loop 3, select the path to insert the customer
            for(int m=0; m<RouteCount; m++){
                if(m != i){
                    vector<Route*> NewRouteList = RouteListCurrent;
                    // delete the pathout from the NewPathList
                    NewRouteList.erase(NewRouteList.begin()+i);
                    //the path that will be inserted the customer
                    Route RouteIn = *RouteListCurrent[m];
                    vector<Customer> CustomerListRouteIn = RouteIn.CustomerList();
                    if(m < i){
                        NewRouteList.erase(NewRouteList.begin()+m);
                     }
                     else{
                         NewRouteList.erase(NewRouteList.begin() + (m-1));
                     }
                     //determine the best insert positon wich minimise the objective function
                     Route NewRouteIn; // the path with the best insert position
                     if(CustomerListRouteIn.empty()==true){
                         vector<Customer> NewCustomersList = CustomerListRouteIn;
                         NewCustomersList.push_back(CustomerOut);
                         Route route(RouteIn.DepotStart(), RouteIn.DepotEnd(), NewCustomersList);
                         NewRouteIn = route;
                         NewRouteIn.SetVehicleId(RouteIn.VehicleId());
                     }
                     else{
                         //determine the insert position, possible paths
                         NewRouteIn = MinCostInsertion(RouteIn, CustomerOut, alpha, beta, gamma);
                         NewRouteIn.SetVehicleId(RouteIn.VehicleId());

                     }

                     TemRouteList.push_back(NewRouteOut);
                     TemRouteList.push_back(NewRouteIn);
                     list<Route>::iterator it = TemRouteList.end();
                     --it;
                     Route* RouteOnePointer = & *it;
                     NewRouteList.push_back(RouteOnePointer);
                     --it;
                     Route* RouteTwoPointer = & *it;
                     NewRouteList.push_back(RouteTwoPointer);
                     Solution NewNeighbor(NewRouteList);
                     NewNeighbor.GetObjectiveValue(alpha, beta, gamma);
                     AllNeighbors.push_back(NewNeighbor);
                     if(CurrentSolution.ObjectiveValue()
                             -NewNeighbor.ObjectiveValue() > epsilon){
                         goto TheEnd;
                     }
                }
            }
        }
    }
TheEnd:{}
}

Route LocalSearch::MinCostInsertion(Route InsertedRoute, Customer InsertedCustomer,
                                      double alpha, double beta, double gamma)
{
    vector<Customer> CustomerList = InsertedRoute.CustomerList();

    //determine the insert position, possible paths
    Route* PossibleRoute = new Route[CustomerList.size()+1];

    for(int n=0; n<(int)CustomerList.size()+1; ++n){
        vector<Customer> NewCustomersList = CustomerList;
        vector<Customer>::iterator it = NewCustomersList.begin();
        advance(it,n);
        NewCustomersList.insert(it,InsertedCustomer);
        Route route(InsertedRoute.DepotStart(), InsertedRoute.DepotEnd(),
                    NewCustomersList);
        PossibleRoute[n] = route;
    }

    double MinObjValue = 9999999999999; //the path with the min objective value
    int RouteWithMinObjValue;
    for(int n=0; n<(int)CustomerList.size()+1; ++n){
        PossibleRoute[n].GetObjectiveValue(alpha, beta, gamma);
        /* display information of possible paths
        Path pathcopy = PossiblePath[n];
        pathcopy.DisplayInfo();
        std::cout << "Objective value:  " << pathcopy.ObjectiveValue()<<std::endl;
        */
        if((MinObjValue-PossibleRoute[n].ObjectiveValue()) > epsilon ){
            MinObjValue = PossibleRoute[n].ObjectiveValue();
            RouteWithMinObjValue = n;
        }
    }
    Route MinCostRoute = PossibleRoute[RouteWithMinObjValue];

    //clean up
    delete []PossibleRoute;

    return MinCostRoute;
}


void LocalSearch::swap(Solution CurrentSolution, std::vector<Solution> &AllNeighbors,
                       std::list<Route> &TemRouteList, double alpha, double beta,
                       double gamma)
{
    int RouteCount = (int)CurrentSolution.RouteList().size();
    vector<Route*> RouteListCurrent = CurrentSolution.RouteList();
    // loop1, select a path that will be involved into the cross exchange operator
    for(int i=0; i<(RouteCount-1); i++){
        Route RouteOne = *RouteListCurrent[i];
        vector<Customer> CustomerListOne = RouteOne.CustomerList();

        // loop2, select a different path as the second path
        for(int j=i+1; j<RouteCount; j++){
            Route RouteTwo = *RouteListCurrent[j];
            vector<Customer> CustomerListTwo = RouteTwo.CustomerList();
            vector<Route*> NewRouteList = RouteListCurrent;
            //remove this path from the solution
            NewRouteList.erase(NewRouteList.begin()+i);
            //remove the second path from the solution
            NewRouteList.erase(NewRouteList.begin()+(j-1));

            if(CustomerListOne.empty() == false && CustomerListTwo.empty() == false){
                //loop3 select a customer from the path one
                for(int m=0; m<(int)CustomerListOne.size(); m++){
                    Customer CustomerOne = CustomerListOne[m];
                    vector<Customer> NewCusListOne = CustomerListOne;
                    NewCusListOne.erase(NewCusListOne.begin()+m);

                    //loop 4 select another customer from the path two
                    for(int n=0; n<(int)CustomerListTwo.size();n++){
                        Customer CustomerTwo = CustomerListTwo[n];
                        vector<Customer> NewCusListTwo = CustomerListTwo;
                        NewCusListTwo.erase(NewCusListTwo.begin()+n);

                        //get copy of NewPathList and NewCusListOne
                        vector<Route*> NewRouteListCopy = NewRouteList;
                        vector<Customer> NewCusListOneCopy = NewCusListOne;

                        NewCusListOneCopy.insert(NewCusListOneCopy.begin()+m, CustomerTwo);
                        NewCusListTwo.insert(NewCusListTwo.begin()+n, CustomerOne);

                        Route NewRouteOne(RouteOne.DepotStart(), RouteOne.DepotEnd(),
                                        NewCusListOneCopy);
                        NewRouteOne.SetVehicleId(RouteOne.VehicleId());
                        Route NewRouteTwo(RouteTwo.DepotStart(), RouteTwo.DepotEnd(),
                                          NewCusListTwo);
                        NewRouteTwo.SetVehicleId(RouteTwo.VehicleId());

                        TemRouteList.push_back(NewRouteOne);
                        TemRouteList.push_back(NewRouteTwo);
                        list<Route>::iterator it = TemRouteList.end();
                        --it;
                        NewRouteListCopy.push_back(& *it);
                        --it;
                        NewRouteListCopy.push_back(& *it);

                        Solution NewNeighbor(NewRouteListCopy);
                        NewNeighbor.GetObjectiveValue(alpha, beta, gamma);
                        AllNeighbors.push_back(NewNeighbor);
                    }
                }
            }
        }
    }
}

void LocalSearch::TwoOptAsterisk(Solution CurrentSolution, std::vector<Solution> &AllNeighbors,
                                 std::list<Route> &TemRouteList, double alpha, double beta,
                                 double gamma)
{
    int RouteCount = (int)CurrentSolution.RouteList().size();
    vector<Route*> RouteListCurrent = CurrentSolution.RouteList();

    //loop1, select a path from the path list of the current solution
    for(int i=0; i<(RouteCount-1); i++){
        Route RouteOne = *RouteListCurrent[i];
        vector<Customer> CustomerListOne = RouteOne.CustomerList();

        //Loop2, select the second path from the path list
        for(int j=i+1; j<RouteCount; j++){
            Route RouteTwo = *RouteListCurrent[j];
            vector<Customer> CustomerListTwo = RouteTwo.CustomerList();
            vector<Route*> NewRouteList = RouteListCurrent;
            // remove the first path from the list
            NewRouteList.erase(NewRouteList.begin()+i);
            // remove the second path from the solution
            NewRouteList.erase(NewRouteList.begin()+(j-1));
            //check if both of two selected paths are empty
            if(CustomerListOne.empty()==false && CustomerListTwo.empty()==false){

                //loop3 select a customer from the path
                for(int m=0; m<(int)CustomerListOne.size(); m++){
                    vector<Customer> Segment1, Segment2;
                    for(int p=0; p<m; p++){
                        Segment1.push_back(CustomerListOne[p]);
                    }
                    for(int q=m; q<(int)CustomerListOne.size(); q++){
                        Segment2.push_back(CustomerListOne[q]);
                    }
                    // loop4 select a node place to split the customer list
                    for(int n=0; n<(int)CustomerListTwo.size(); n++){
                        vector<Customer> Segment3, Segment4;
                        for(int p=0; p<n; p++){
                            Segment3.push_back(CustomerListTwo[p]);
                        }
                        for(int q=n; q<(int)CustomerListTwo.size();q++){
                            Segment4.push_back(CustomerListTwo[q]);
                        }
                        vector<Customer> NewCustomerList1, NewCustomerList2;
                        NewCustomerList1 = Segment1;
                        NewCustomerList1.insert(NewCustomerList1.end(),
                                                Segment4.begin(), Segment4.end());
                        NewCustomerList2 = Segment3;
                        NewCustomerList2.insert(NewCustomerList2.end(),
                                                Segment2.begin(), Segment2.end());

                        vector<Route*> NewRouteListCopy = NewRouteList;

                        Route NewRouteOne(RouteOne.DepotStart(), RouteOne.DepotEnd(),
                                          NewCustomerList1);
                        NewRouteOne.SetVehicleId(RouteOne.VehicleId());
                        Route NewRouteTwo(RouteTwo.DepotStart(), RouteTwo.DepotEnd(),
                                          NewCustomerList2);
                        NewRouteTwo.SetVehicleId(RouteTwo.VehicleId());

                        TemRouteList.push_back(NewRouteOne);
                        TemRouteList.push_back(NewRouteTwo);

                        list<Route>::iterator it = TemRouteList.end();
                        --it;
                        NewRouteListCopy.push_back(& *it);
                        --it;
                        NewRouteListCopy.push_back(& *it);

                        Solution NewNeighbor(NewRouteListCopy);
                        NewNeighbor.GetObjectiveValue(alpha, beta, gamma);
                        AllNeighbors.push_back(NewNeighbor);
                    }
                }
            }
        }
    }
}

void LocalSearch::CrossExchange(Solution CurrentSolution, std::vector<Solution> &AllNeighbors,
                                std::list<Route> &TemRouteList, double alpha, double beta,
                                double gamma)
{
    int SegmentMaxLength = 2;
    int RouteCount = (int)CurrentSolution.RouteList().size();
    vector<Route*> RouteListCurrent = CurrentSolution.RouteList();

    // loop1, select a path that will be involved into the cross exchange operator
    for(int i=0; i<(RouteCount-1); i++){
        Route RouteOne = *RouteListCurrent[i];
        vector<Customer> CustomerListOne = RouteOne.CustomerList();
        //loop2, select a different path as the second path involved in the cross-exchange
        for(int j=i+1; j<RouteCount; j++){
            Route RouteTwo = *RouteListCurrent[j];
            vector<Customer> CustomerListTwo = RouteTwo.CustomerList();
            vector<Route*> NewRouteList = RouteListCurrent;
            NewRouteList.erase(NewRouteList.begin()+i);
            NewRouteList.erase(NewRouteList.begin()+(j-1));

            // Question: if PathOne is empty, PathTwo is not empty,
            //some combinations will be ignored

            // loop3, select the start of the first segment
            for(int k=0; k<(int)CustomerListOne.size(); k++){

                // loop 4, select the start of the second segment
                for(int l=0; l<(int)CustomerListTwo.size(); l++){

                    // loop 5, select the end of the first segment
                    for(int m=k; (m<(int)CustomerListOne.size())&&(m<(k+SegmentMaxLength)); m++){
                        vector<Customer> NewCustomerListOne = CustomerListOne;
                        vector<Customer> SegmentOne;
                        //take the first segment, and erase the removed customers
                        for(int p=k; p<(m+1); ++p){
                            SegmentOne.push_back(CustomerListOne[p]);
                            // delete the removed customers from the list
                            NewCustomerListOne.erase(NewCustomerListOne.begin()+k);
                        }

                        // loop 6, select the end of the seconde segment
                        for(int n=l; (n<(int)CustomerListTwo.size())&&(n<(l+SegmentMaxLength)); n++){
                            vector<Customer> NewCustomerListTwo = CustomerListTwo;
                            vector<Customer> SegmentTwo;
                            // did not consider of a segment with size 0
                            // build the second segment, and erase the removed customers
                            for(int q=l; q<(n+1); ++q){
                                SegmentTwo.push_back(CustomerListTwo[q]);
                                NewCustomerListTwo.erase(NewCustomerListTwo.begin()+l);
                            }
                            vector<Customer> NewCustomerListOneCopy = NewCustomerListOne;
                            NewCustomerListOneCopy.insert(NewCustomerListOneCopy.begin()+k,
                                                          SegmentTwo.begin(), SegmentTwo.end());
                            NewCustomerListTwo.insert(NewCustomerListTwo.begin()+l,
                                                      SegmentOne.begin(), SegmentOne.end());

                            Route NewRouteOne(RouteOne.DepotStart(), RouteOne.DepotEnd(),
                                              NewCustomerListOneCopy);
                            NewRouteOne.SetVehicleId(RouteOne.VehicleId());
                            Route NewRouteTwo(RouteTwo.DepotStart(), RouteTwo.DepotEnd(),
                                              NewCustomerListTwo);
                            NewRouteTwo.SetVehicleId(RouteTwo.VehicleId());

                            vector<Route*> NewRouteListCopy = NewRouteList;

                            TemRouteList.push_back(NewRouteOne);
                            TemRouteList.push_back(NewRouteTwo);

                            list<Route>::iterator it = TemRouteList.end();
                            --it;
                            NewRouteListCopy.push_back(& *it);
                            --it;
                            NewRouteListCopy.push_back(& *it);

                            Solution NewNeighbor(NewRouteListCopy);
                            NewNeighbor.GetObjectiveValue(alpha, beta, gamma);
                            AllNeighbors.push_back(NewNeighbor);
                        }
                    }
                }
            }
        }
    }
}






















