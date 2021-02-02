#ifndef LINKEDLISTCUSTOMER
#define LINKEDLISTCUSTOMER

#include <iostream>
#include <list> //for linked list
#include <iterator>
#include <fstream> // file
#include <vector>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <stack>

using namespace std;
static string custCaptions[]={"Customer ID", "Name", "Contact Number", "Email"};

struct Customer{// a struct to hold all the basic info of customer
    string custName;
    int custID; //assigned by system
    int custContact;
    string custEmail;
    string custPassword;
    Customer(){}// default constructor
    Customer(string cName, int cID, int cContact, string cEmail, string cPassword):
        custName(cName),custID(cID),custContact(cContact),custEmail(cEmail),custPassword(cPassword){} // constructor with initialization list

};

class Reservation{
public:
    list<Customer> tmp_custList, all_custList;
    void load_custList(); // load the data from file
    void save_custList(); // save data to file
    void addCust(); // add new customer record
    void displayCust(); // display current customers in record
    void searchCust(); // search customer based on customerID
    void sortCust(); // sort customers in ascending / descending order
    void modifyCust(); // modify customer record
    void deleteCust(); // delete particular customer based on custID
    virtual void checkAvailability(int& inputDate, int& inputTime, int& rTableNo, bool& flag)=0;// pure virtual function
};

#endif


