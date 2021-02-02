#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <list>
#include <sstream>
#include <ctime>
#include <stdio.h>
#include <stdlib.h>
#include <stack>
#include "Reservation.h"

using namespace std;

typedef map<int,map<int, int>> hashmap;
static string reservCaptions[] = {"Reservation ID","Customer ID", "Date", "Time", "Table No", "Reservation Type", "No of People"};

class TableReservation: public Reservation{
    private:
        int custID=0;
        int reservDate; //YYYYMDD
        int reservTime; //1700, 1800, 1900, 2000, 2100, 2200, 2300
        int reservID; //assigned by system
        int reservTableNo; //assigned by system
        string reservType; //"individual","group"
        int reservPplNo;
    public:
        TableReservation(){}
        TableReservation(int rID, int cID, int rDate, int rTime, int rTableNo, string rType, int rPplNo):
            reservID(rID),custID(cID),reservDate(rDate),reservTime(rTime),reservTableNo(rTableNo),reservType(rType),reservPplNo(rPplNo){}
        ~TableReservation(){}

        void set_custID(int cID){ //to set current logged in customer ID
            custID=cID;
        }

        int get_reservDate(){ //get reservDate, for sort function
            return reservDate;
        }

        hashmap dateAvailabilityMap; //map for reservTime, reservDate, tableCount
        list<TableReservation> tmp_reservList, all_reservList; //details of reservation

        void load_dateAvailabilityMap();// load the data from file into map
        void save_dateAvailabilityMap();// save data to file from map
        void load_reservList();// load the data from file into list
        void save_reservList();// save data to file from list

        void addReserv();//add new reservation
        void displayReserv();// display current reservations in record
        void searchReserv();// search customer based on reservID
        void sortReserv();// sort reservation in ascending / descending order
        void modifyReserv();// modify reservation record
        void deleteReserv();// delete particular reservation based on reservID
        void confirmReserv(int reservID, int custID);// print receipt as proof of confirmation

        void checkAvailability(int& inputDate, int& inputTime, int& rTableNo, bool& flag);// check the date, time, table availability
        int generate_reservID();// reservID of each reservation assigned by system in sequence
        void addFeedback();// feedback on the restaurant service
};





