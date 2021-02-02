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
#include "TableReservation.h"

using namespace std;

void TableReservation::load_dateAvailabilityMap(){
    string rDate, rTime, tableCount; //helper variables
    int int_rDate, int_rTime, int_tableCount;
    vector<int> rDateVec, rTimeVec, tableCountVec; //helper ds
    int row=0;
    ifstream dateAvailabilityFile("dateAvailability2020.csv"); //dateAvailability.csv shall be present in project folder (created by createdate.cpp)

    if(dateAvailabilityFile.is_open())
    {
        string line;
        getline(dateAvailabilityFile, line); //ignore first line
        while(!dateAvailabilityFile.eof()){
            getline(dateAvailabilityFile, rDate, ',');
            stringstream geek1(rDate);
            geek1 >> int_rDate;
            rDateVec.push_back(int_rDate);

            getline(dateAvailabilityFile, rTime, ',');
            stringstream geek2(rTime);
            geek2 >> int_rTime;
            rTimeVec.push_back(int_rTime);

            getline(dateAvailabilityFile, tableCount, '\n');
            stringstream geek3(tableCount);
            geek3 >> int_tableCount;
            tableCountVec.push_back(int_tableCount);

            row++;
        }
        dateAvailabilityFile.close();

        //load into map
        for(int k=0; k<row-1; k++){
            dateAvailabilityMap[rDateVec[k]].insert(make_pair(rTimeVec[k],tableCountVec[k]));
        }
    }
    else
    {
        cout << "Error! dateAvailability2020.csv File is not opened!" << endl;
    }
}


void TableReservation::save_dateAvailabilityMap(){
    //load both ds
    load_dateAvailabilityMap();
    load_reservList();

    //iterators
    map<int, int>::iterator it_innerMap;
    map<int, map<int, int>>::iterator it_outerMap;
    list<TableReservation>::iterator it_reservList;

    int rDate, rTime, tableCount; //temporary variable
    ofstream dateAvailabilityFile("dateAvailability2020.csv");

    for(it_outerMap = dateAvailabilityMap.begin(); it_outerMap != dateAvailabilityMap.end(); it_outerMap++){ //check outer map
        rDate = it_outerMap->first; //store date into temporary variable

        for (it_innerMap=it_outerMap->second.begin(); it_innerMap!=it_outerMap->second.end(); it_innerMap++){ //check inner map
            rTime = it_innerMap->first; //store time into temporary variable
            tableCount=0; //tableCount=0 by default

            for(it_reservList=all_reservList.begin(); it_reservList!=all_reservList.end(); ++it_reservList){ //check reservList
                if((it_reservList->reservDate==rDate) && (it_reservList->reservTime==rTime)) //if found date and timein list
                    tableCount++; //increment tableCount by 1
            }
            //write into file to update new tableCount
            dateAvailabilityFile << it_outerMap->first << "," << it_innerMap->first << "," << tableCount << endl;
        }
    }
}


void TableReservation::load_reservList(){
    string rID, cID, rDate, rTime, rTableNo, rType, rPplNo; //helper variables
    int int_rID, int_cID, int_rDate, int_rTime, int_rTableNo, int_rPplNo;

    vector<int> rIDVec, cIDVec, rDateVec, rTimeVec, rTableNoVec, rPplNoVec; //helper ds
    vector<string> rTypeVec; //helper ds
    int row = 0;
    TableReservation t;
    ifstream reservListFile("reservList.csv");

    if(reservListFile.is_open()){
        if(all_reservList.empty()){ //if all_reservList is empty
            string line;
            getline(reservListFile, line); //ignore first line
            while(!reservListFile.eof()){ //read all record in file
                getline(reservListFile, rID, ',');
                stringstream geek1(rID);
                geek1 >> int_rID;
                rIDVec.push_back(int_rID);

                getline(reservListFile, cID, ',');
                stringstream geek2(cID);
                geek2 >> int_cID;
                cIDVec.push_back(int_cID);

                getline(reservListFile, rDate, ',');
                stringstream geek3(rDate);
                geek3 >> int_rDate;
                rDateVec.push_back(int_rDate);

                getline(reservListFile, rTime, ',');
                stringstream geek4(rTime);
                geek4 >> int_rTime;
                rTimeVec.push_back(int_rTime);

                getline(reservListFile, rTableNo, ',');
                stringstream geek5(rTableNo);
                geek5 >> int_rTableNo;
                rTableNoVec.push_back(int_rTableNo);

                getline(reservListFile, rType, ',');
                rTypeVec.push_back(rType);

                getline(reservListFile, rPplNo, '\n');
                stringstream geek7(rPplNo);
                geek7 >> int_rPplNo;
                rPplNoVec.push_back(int_rPplNo);

                row++;
            }
            reservListFile.close();

             //add records in file into list
            for(int j = 0; j <row-1; j++){
                t.reservID = rIDVec[j];
                t.custID = cIDVec[j];
                t.reservDate = rDateVec[j];
                t.reservTime = rTimeVec[j];
                t.reservTableNo = rTableNoVec[j];
                t.reservType = rTypeVec[j];
                t.reservPplNo = rPplNoVec[j];
                all_reservList.push_back(t);
            }
        }
        //if all_reservList list have previous records
        if(!tmp_reservList.empty()){ //if present list having record
            list<TableReservation>::iterator it_reservList;
            for(it_reservList = tmp_reservList.begin(); it_reservList != tmp_reservList.end(); ++it_reservList){ //add present record to all_reservList list
                t.reservID = it_reservList->reservID;
                t.custID = it_reservList->custID;
                t.reservDate = it_reservList->reservDate;
                t.reservTime = it_reservList->reservTime;
                t.reservTableNo = it_reservList->reservTableNo;
                t.reservType = it_reservList->reservType;
                t.reservPplNo = it_reservList->reservPplNo;
                all_reservList.push_back(t);
            }
        }
    }
    else{
        cout<< "Unable to open reservList.csv"<< endl;
    }
}


void TableReservation::save_reservList(){// for remove n edit function
    fstream reservListFile("reservList.csv", ios::out);//both read n append to file functions
    if(!reservListFile.is_open()) //file not opened, error
    {
        cout << "Error! reservList.csv is not opened!" << endl;
    }
    else{//save into file
        reservListFile << "Reservation ID,Customer ID,Date,Time,Table Number,Reservation Type,Number of People" <<endl;
        list<TableReservation> :: iterator it_reservList;
        for(it_reservList = all_reservList.begin(); it_reservList != all_reservList.end(); ++it_reservList){//add to file
            reservListFile << it_reservList->reservID << ","
                           << it_reservList->custID << ","
                           << it_reservList->reservDate << ","
                           << it_reservList->reservTime << ","
                           << it_reservList->reservTableNo << ","
                           << it_reservList->reservType << ","
                           << it_reservList->reservPplNo << endl;
        }
    }
    reservListFile.close();
}


void TableReservation::checkAvailability(int& inputDate, int& inputTime, int& rTableNo, bool& flag){
    /*check: (1) date entered valid
             (2) date entered is future date
             (3) date entered not a day before reservation
             (4) time entered valid
             (5) table available for date and time entered
    */

    load_dateAvailabilityMap();
    map<int, int>::iterator it_innerMap;
    map<int, map<int, int>>::iterator it_outerMap;

    int year, month, day, numOfDays, countYear, temp;
    string stringDate;

    struct tm reserved_date_struct = {0};
    time_t reserved_date_t;
    double dif_time = 0;
    time_t now = time(0); // current system time
    char* dt = ctime(&now); //convert now to string form

    //check (1)
    do{
        countYear = 0;
        cout << "\n\t\tEnter year: " ;
        cin >> year;
        endl(cout);

        temp = year;
        while(temp != 0){
            temp = temp /10;
            ++ countYear;
        }
    }while(countYear != 4);

    cout << "\t\tEnter month: " ;
    cin >> month;
    endl(cout);
    while(month <= 0 || month >12){
        cout << "\t\tPlease enter month in the range of 1 to 12."<< endl;
        cout << "\t\tEnter month: " ;
        cin >> month;
        endl(cout);
    }
    if (month == 2){
        bool leapYear = false;
        if ((year % 4 == 0 && year % 100 != 0) || ( year % 400 == 0))
            leapYear = true;
        numOfDays = leapYear ? 29 : 28; // if leap year true = 29 days, else false = 28
    }
    else if (month == 3 || month == 6 || month == 9 || month == 11){
        numOfDays = 30;
    }
    else{
        numOfDays = 31;
    }

    cout << "\t\tEnter Date: " ;
    cin >> day;
    endl(cout);
    while(day <= 0 || day > numOfDays){
        cout<< "\t\tKindly re-enter a valid Date that is in between 1 and " << numOfDays << " of month " << month << " " << year << endl;
        cout << "\t\tEnter Date: " ;
        cin >> day;
        endl(cout);
    }

    //check (2)
    reserved_date_struct.tm_year = year -1900;// start counting from year 1900
    reserved_date_struct.tm_mon = month -1; //index 0 - 11
    reserved_date_struct.tm_mday = day;

    reserved_date_t = mktime(&reserved_date_struct);//convert to time_t using mktime() function
    dif_time = difftime(reserved_date_t, now); // compare user input Date with now
    if(dif_time < 0){ //no reservation allowed on past day
        cout << "\t\tPlease enter a reservation Date after " << dt << endl;
    }
    //check (3)
    else if (dif_time <= 86400){ // no reservation allowed on next day
        cout << "\t\tCustomer is not allowed to reserve the table a day before. Thank you for your understanding. "<< endl;
    }
    else{
        //add "0" infront of the one-digit number to maintain YYYYMMDD format
        string stringMonth = to_string(month);
        string stringDay = to_string(day);
        if(stringMonth.size()==1)
            stringMonth = "0"+stringMonth;
        if(stringDay.size()==1)
            stringDay = "0"+stringDay;

        stringDate = to_string(year) + stringMonth + stringDay;
        inputDate = stoi(stringDate);

        //check (4)
        cout << "\t\tEnter time (1700/1800/1900/2000/2100/2200/2300): ";
        cin >> inputTime;
        while( (inputTime!=1700) && (inputTime!=1800) && (inputTime!=1900) && (inputTime!=2000) && (inputTime!=2100) && (inputTime!=2200) && (inputTime!=2300) ){
            cout << "\t\tPlease enter 1700, 1800, 1900, 2000, 2100, 2200, or 2300 only" << endl;
            cout << "\t\tEnter time: ";
            cin >> inputTime;
        }
        //check (5)
        //find given date and time from map, and retrive the table count
        for(auto it_outerMap = dateAvailabilityMap.find(inputDate); it_outerMap != dateAvailabilityMap.end(); it_outerMap++){
            if(it_outerMap->first==inputDate){
                for (it_innerMap=it_outerMap->second.find(inputTime); it_innerMap!=it_outerMap->second.end(); it_innerMap++){
                    //if found given Date and time
                    if(it_innerMap->second<5){ //and if still have table available
                        //allow reservation
                        rTableNo=it_innerMap->second + 1; //if number of occupied table is 3, assign new reservation to table 4
                        cout << "\t\tTime valid, reserving table " << rTableNo << endl;
                        flag = true;
                        break;
                    }
                    else{
                        //return 0 to loop back
                        cout << "\t\tFull Reservation. Try other slots." << endl;
                        break;
                    }
                }
            }
        }
    }
}


int TableReservation::generate_reservID(){
    ifstream reservIDCounterFile("reservIDCounter.txt");
    int rID=2000; //if no reservation exist, rID=2000
    string line;

    if(reservIDCounterFile.is_open()){
        if(!reservIDCounterFile.eof()){
            getline(reservIDCounterFile, line);
            int a = stoi(line);
            rID = a + 1; //increment last used rID by 1 to become new rID
        }
    }
    //else
        //cout << "\n\t\tUnable to open reservIDCounter.txt" << endl;

    reservIDCounterFile.close();
    return rID;
}


void TableReservation::addReserv(){
    string rType;
    int rID, cID, rDate, rTime, rTableNo, rPplNo;

    //specify custID
    if(custID==0){ //if admin
        cout << "\n\t\tEnter customer ID: ";
        cin >> cID;
        set_custID(cID);
    }
    else //if not admin
        cID=custID;

    //specify reservDate, time, reservTableNo
    bool flag = false;
    do{
        checkAvailability(rDate, rTime, rTableNo, flag);
    }while(!flag);

    //specify reservType, reservPplNo
    cout << "\n\t\tEnter type(individual/group): ";
    cin >> rType;
    while(rType!="individual" && rType!="group"){
        cout << "\t\tPlease enter \"individual\" or \"group\" only." << endl;
        cout << "\n\t\tEnter type(individual/group): ";
        cin >> rType;
    }

    if(rType=="group"){
        cout << "\n\t\tEnter number of people: ";
        cin >> rPplNo;
    }
    else if(rType=="individual")
        rPplNo=1;

    //specify reservID
    rID = generate_reservID();

    //add to tmp list
    tmp_reservList.push_back(TableReservation(rID, cID, rDate, rTime, rTableNo, rType, rPplNo));

    fstream reservListFile("reservList.csv", ios::app);
    ofstream reservIDCounterFile("reservIDCounter.txt"); //a file storing last reservID used
    list<TableReservation> :: iterator it_reservList;

    if(!reservListFile.is_open()){
        cout << "Error! reservList.csv File is not opened!" << endl;
    }
    else{
        reservListFile.seekg(0, ios::end);
        if (reservListFile.tellg() != 0) // If record file have record, load the existed record into the list
            load_reservList();
        else{// if the file is empty, column names are added
            reservListFile << "Reservation ID,Customer ID,Date,Time,Table Number,Reservation Type,Number of People" <<endl;
        }

        for(it_reservList = tmp_reservList.begin(); it_reservList != tmp_reservList.end(); ++it_reservList){ //add to file
            if(it_reservList->reservID==rID){
                cout << "\n\n\t\tReserved Table " << it_reservList->reservTableNo << " with customer ID " << it_reservList->custID << ". " << endl;
                reservListFile << it_reservList->reservID << ","
                               << it_reservList->custID << ","
                               << it_reservList->reservDate << ","
                               << it_reservList->reservTime << ","
                               << it_reservList->reservTableNo << ","
                               << it_reservList->reservType << ","
                               << it_reservList->reservPplNo << endl;
                reservIDCounterFile << it_reservList->reservID;
            }
        }
        tmp_reservList.clear(); //clear tmp list, so that it always store one record, data have been write into file
    }
    reservListFile.close();
    reservIDCounterFile.close();

    load_reservList(); //load list again with newly added record in file
    confirmReserv(rID, cID); //print receipt
    save_dateAvailabilityMap(); //update table count

    system("pause");
}


void TableReservation::displayReserv(){
    system("cls");
    system("title Western Hut - Display Reservation");
    load_reservList();
    list<TableReservation> :: iterator it_reservList;

    cout << "\n\n\t\tList of Reservations" << endl << endl;
    //print column name
    printf("%*s \t", 10, reservCaptions[0].c_str());
    printf("%*s \t", 12, reservCaptions[1].c_str());
    printf("%*s \t", 8, reservCaptions[2].c_str());
    printf("%*s \t", 4, reservCaptions[3].c_str());
    printf("%*s \t", 9, reservCaptions[4].c_str());
    printf("%*s \t", 10, reservCaptions[5].c_str());
    printf("%*s \n", 10, reservCaptions[6].c_str());

    if(custID==0){ //if is admin, display all_reservList
        for(it_reservList = all_reservList.begin(); it_reservList != all_reservList.end(); ++it_reservList){
            //print details
            printf("%*d \t", 10, it_reservList->reservID );
            printf("%*d \t", 10, it_reservList->custID);
            printf("%*d \t", 10, it_reservList->reservDate);
            printf("%*-d \t", 6, it_reservList->reservTime);
            printf("%*d \t", 8, it_reservList->reservTableNo);
            printf("%*s \t", 10, it_reservList->reservType.c_str());
            printf("%*d \n", 10, it_reservList->reservPplNo);
        }
        //prompt to proceed sort only for admin mode
        char proceedSort;
        cout << "\n\t\tSort? (Y/N) ";
        cin >> proceedSort;
        if(proceedSort=='Y' || proceedSort=='y') sortReserv();
    }
    else{ //if not admin
        for(it_reservList = all_reservList.begin(); it_reservList != all_reservList.end(); ++it_reservList){
            if(it_reservList->custID==custID){ //display only record with current custID
                //print details
                printf("%*d \t", 10, it_reservList->reservID );
                printf("%*d \t", 10, it_reservList->custID);
                printf("%*d \t", 10, it_reservList->reservDate);
                printf("%*-d \t", 6, it_reservList->reservTime);
                printf("%*d \t", 8, it_reservList->reservTableNo);
                printf("%*s \t", 10, it_reservList->reservType.c_str());
                printf("%*d \n", 10, it_reservList->reservPplNo);
            }
        }
        //return for customer mode
        cout << endl;
        system("pause");
    }
}


void TableReservation::searchReserv(){
    system("cls");
    system("title Western Hut - Search Reservation");
    load_reservList();
    int rID, flag, choice;

    cout << "\n\n\t\t  Search Reservation" << endl;
    cout << "\t\t--enter 0 to go back--" << endl << endl;
    cout << "\n\t\tEnter reservation ID to search: ";
    cin >> rID;
    if(rID==0) return;

    list<TableReservation> :: iterator it_reservList;
    cout << "\n\n\t\tSearch Result" << endl << endl;

    if(custID==0){ //admin
        for(it_reservList = all_reservList.begin(); it_reservList != all_reservList.end(); ++it_reservList){
            if(rID==it_reservList->reservID){ //if reservation ID exist and match
                //print column name
                printf("%*s \t", 10, reservCaptions[0].c_str());
                printf("%*s \t", 12, reservCaptions[1].c_str());
                printf("%*s \t", 8, reservCaptions[2].c_str());
                printf("%*s \t", 4, reservCaptions[3].c_str());
                printf("%*s \t", 9, reservCaptions[4].c_str());
                printf("%*s \t", 10, reservCaptions[5].c_str());
                printf("%*s \n", 10, reservCaptions[6].c_str());

                //print search result
                printf("%*d \t", 10, it_reservList->reservID );
                printf("%*d \t", 10, it_reservList->custID);
                printf("%*d \t", 10, it_reservList->reservDate);
                printf("%*-d \t", 6, it_reservList->reservTime);
                printf("%*d \t", 8, it_reservList->reservTableNo);
                printf("%*s \t", 10, it_reservList->reservType.c_str());
                printf("%*d \n", 10, it_reservList->reservPplNo);
                flag=1;
                break;
            }
            else
                flag=0;
        }
    }
    else{ //customer
        for(it_reservList = all_reservList.begin(); it_reservList != all_reservList.end(); ++it_reservList){
            if( (rID==it_reservList->reservID) && (it_reservList->custID==custID) ){ //if reservation ID exist and match, with current customer ID
                //print column name
                printf("%*s \t", 10, reservCaptions[0].c_str());
                printf("%*s \t", 12, reservCaptions[1].c_str());
                printf("%*s \t", 8, reservCaptions[2].c_str());
                printf("%*s \t", 4, reservCaptions[3].c_str());
                printf("%*s \t", 9, reservCaptions[4].c_str());
                printf("%*s \t", 10, reservCaptions[5].c_str());
                printf("%*s \n", 10, reservCaptions[6].c_str());

                //print search result
                printf("%*d \t", 10, it_reservList->reservID );
                printf("%*d \t", 10, it_reservList->custID);
                printf("%*d \t", 10, it_reservList->reservDate);
                printf("%*-d \t", 6, it_reservList->reservTime);
                printf("%*d \t", 8, it_reservList->reservTableNo);
                printf("%*s \t", 10, it_reservList->reservType.c_str());
                printf("%*d \n", 10, it_reservList->reservPplNo);
                flag=1;
                break;
            }
            else
                flag=0;
        }
    }
    //if no reservation ID found
    if(flag==0){
        cout << "\n\t\tID not found" << endl;
    }
    cout << endl;
    system("pause");
}


//<---------Merge Sort Functions--------->
void mergeArray(TableReservation tempArray[], int left, int middle, int right){
    int i, j, k;
    int n1 = middle - left + 1;
    int n2 = right - middle;

    TableReservation L[n1], R[n2];

    /* Copy data to temp arrays L[] and R[] */
    for (i = 0; i < n1; i++)
        L[i] = tempArray[left + i];
    for (j = 0; j < n2; j++)
        R[j] = tempArray[middle + 1 + j];

    /* Merge the temp arrays back into arr[l..r]*/
    i = 0; // Initial index of first subarray
    j = 0; // Initial index of second subarray
    k = left; // Initial index of merged subarray
    while (i < n1 && j < n2) {
        if (L[i].get_reservDate() <= R[j].get_reservDate()) {
            tempArray[k] = L[i];
            i++;
        }
        else {
            tempArray[k] = R[j];
            j++;
        }
        k++;
    }

    while (i < n1) {
        tempArray[k] = L[i];
        i++;
        k++;
    }

    while (j < n2) {
        tempArray[k] = R[j];
        j++;
        k++;
    }
}

void mergeSort(TableReservation tempArray[], int left, int right){
    if (left < right){
        int mid = left + (right - left) / 2;
        mergeSort(tempArray, left, mid);
        mergeSort(tempArray, mid + 1, right);

        mergeArray(tempArray, left, mid, right);
    }
}

void TableReservation::sortReserv(){
    system("cls");
    system("title Western Hut - Sort Reservation");
    list<TableReservation> :: iterator it_reservList;
    int sortType = 0;
    cout << "\n\n\t\tSelect Sort Type" << endl;
    cout << "\t\t(1) Descending Reservation ID" << endl;
    cout << "\t\t(2) Ascending Date" << endl;
    cout << "\t\t(3) Descending Date" << endl << endl;
    cout << "\t\tSelect -> ";
    cin >> sortType;

    system("cls");

    if(sortType == 1){
        stack<TableReservation> descendingSortStack;
        //push records in list into stack
        for(it_reservList = all_reservList.begin(); it_reservList != all_reservList.end(); ++it_reservList){
            descendingSortStack.push(*it_reservList);
        }
        cout << "\n\n\t\tSort Reservation by Descending Reservation ID" << endl << endl;
        //print column name
        printf("%*s \t", 10, reservCaptions[0].c_str());
        printf("%*s \t", 12, reservCaptions[1].c_str());
        printf("%*s \t", 8, reservCaptions[2].c_str());
        printf("%*s \t", 4, reservCaptions[3].c_str());
        printf("%*s \t", 9, reservCaptions[4].c_str());
        printf("%*s \t", 10, reservCaptions[5].c_str());
        printf("%*s \n", 10, reservCaptions[6].c_str());

        while(!descendingSortStack.empty()){
            //print stack
            printf("%*d \t", 10, descendingSortStack.top().reservID );
            printf("%*d \t", 10, descendingSortStack.top().custID);
            printf("%*d \t", 10, descendingSortStack.top().reservDate);
            printf("%*-d \t", 6, descendingSortStack.top().reservTime);
            printf("%*d \t", 8, descendingSortStack.top().reservTableNo);
            printf("%*s \t", 10, descendingSortStack.top().reservType.c_str());
            printf("%*d \n", 10, descendingSortStack.top().reservPplNo);
            descendingSortStack.pop();
        }
    }else{
        TableReservation mergeSortMainArray[all_reservList.size()];
        int i = 0;
        //push records in list into array
        for(it_reservList = all_reservList.begin() ; it_reservList != all_reservList.end(); ++it_reservList, i++){
            mergeSortMainArray[i] = *it_reservList;
        }
        //merge sort
        mergeSort(mergeSortMainArray, 0, (sizeof(mergeSortMainArray)/sizeof(mergeSortMainArray[0])) - 1);

        if(sortType == 2){
            cout << "\n\n\t\tSort Reservation by Ascending Date" << endl << endl;
            //print column name
            printf("%*s \t", 10, reservCaptions[0].c_str());
            printf("%*s \t", 12, reservCaptions[1].c_str());
            printf("%*s \t", 8, reservCaptions[2].c_str());
            printf("%*s \t", 4, reservCaptions[3].c_str());
            printf("%*s \t", 9, reservCaptions[4].c_str());
            printf("%*s \t", 10, reservCaptions[5].c_str());
            printf("%*s \n", 10, reservCaptions[6].c_str());

            for(int i = 0; i < (sizeof(mergeSortMainArray)/sizeof(mergeSortMainArray[0])); i++){
                //print array
                printf("%*d \t", 10, mergeSortMainArray[i].reservID );
                printf("%*d \t", 10, mergeSortMainArray[i].custID);
                printf("%*d \t", 10, mergeSortMainArray[i].reservDate);
                printf("%*-d \t", 6, mergeSortMainArray[i].reservTime);
                printf("%*d \t", 8, mergeSortMainArray[i].reservTableNo);
                printf("%*s \t", 10, mergeSortMainArray[i].reservType.c_str());
                printf("%*d \n", 10, mergeSortMainArray[i].reservPplNo);
            }
        }else if(sortType == 3){
            stack<TableReservation> descendingSortStack;
            //push array into stack
            for(int i = 0; i < (sizeof(mergeSortMainArray)/sizeof(mergeSortMainArray[0])); i++){
                descendingSortStack.push(mergeSortMainArray[i]);
            }
            cout << "\n\n\t\tSort Reservation by Descending Date" << endl << endl;
            //print column name
            printf("%*s \t", 10, reservCaptions[0].c_str());
            printf("%*s \t", 12, reservCaptions[1].c_str());
            printf("%*s \t", 8, reservCaptions[2].c_str());
            printf("%*s \t", 4, reservCaptions[3].c_str());
            printf("%*s \t", 9, reservCaptions[4].c_str());
            printf("%*s \t", 10, reservCaptions[5].c_str());
            printf("%*s \n", 10, reservCaptions[6].c_str());

            while(!descendingSortStack.empty()){
                //print stack
                printf("%*d \t", 10, descendingSortStack.top().reservID );
                printf("%*d \t", 10, descendingSortStack.top().custID);
                printf("%*d \t", 10, descendingSortStack.top().reservDate);
                printf("%*-d \t", 6, descendingSortStack.top().reservTime);
                printf("%*d \t", 8, descendingSortStack.top().reservTableNo);
                printf("%*s \t", 10, descendingSortStack.top().reservType.c_str());
                printf("%*d \n", 10, descendingSortStack.top().reservPplNo);
                descendingSortStack.pop();
            }
        }
    }
    cout << endl;
    system("pause");
}
//<---------Full Sort Ends--------->


void TableReservation::modifyReserv(){
    system("cls");
    system("title Western Hut - Modify Reservation");
    load_reservList();
    string rType;
    int rID;
    bool flag = false;

    cout << "\n\n\t\t  Modify Reservation" << endl;
    cout << "\n\t\tEnter reservation ID to be modified: " ;
    cin >> rID;

    list<TableReservation> clone_reservList;// a temporary linked list

    for (list<TableReservation>::iterator it_reservList = all_reservList.begin(); it_reservList != all_reservList.end(); )
    {
        if (it_reservList->reservID != rID){// if different rID, store into clone
            clone_reservList.push_back(*it_reservList);
        }
        else {
            flag = true;
        }
        it_reservList++;
    }
    if (!flag){// id is not found
        cout << "\n\t\t" << rID << " not found! "<< endl << endl;
        system("pause");
    }
    else{
        all_reservList.clear();//clear the all reservation record
        all_reservList.assign(clone_reservList.begin(),clone_reservList.end());// copy all contents from clone to allReservation
        save_reservList();// save into file
        if(flag){
            system("cls");
            system("title Western Hut - Modify Reservation");
            cout << "\n\n\t\t  Modify Reservation" << endl;
            addReserv();
        }
    }

}


void TableReservation::deleteReserv(){
    system("cls");
    system("title Western Hut - Delete Reservation");
    load_reservList();
    int rID;
    bool flag = false;

    cout << "\n\n\t\t  Delete Reservation" << endl;
    cout << "\t\t--enter 0 to go back--" << endl << endl;
    cout << "\n\t\tEnter reservation ID to be removed : " ;
    cin >> rID;
    if(rID==0) return;

    list<TableReservation> clone_reservList;// a temporary linked list
    for (list<TableReservation>::iterator it_reservList = all_reservList.begin(); it_reservList != all_reservList.end(); )
    {
        if (it_reservList->reservID != rID){// if different rID, store into clone
            clone_reservList.push_back(*it_reservList);
        }
        else if(it_reservList->reservID == rID){
            flag = true;
        }
        it_reservList++;
    }
    if (!flag){// id is not found
        cout << "\n\t\t" << rID << " not found! "<< endl << endl;
    }
    else{
        all_reservList.clear();//clear the all reservation record
        all_reservList.assign(clone_reservList.begin(),clone_reservList.end());// copy all contents from clone to allReservation
        save_reservList();
        cout << "\n\t\tReservation deleted." << endl << endl;
    }
    system("pause");
}


void TableReservation::addFeedback(){
    fstream feedbackListFile ("feedbackList.csv",ios::app);
    string comment;
    int cID, serviceRating, foodRating, priceRating;
    system("Title Western Hut - Feedback Session");
    system("cls");

    cout << "\n\n\t\tFeedback Session" << endl;

    cout << "\n\t\tWhat is your Customer ID: " ;
    cin >> cID;

    //cout << "\t\tWhat is your Reservation ID: " ;
    //cin >> rID;

    cout << "\t\tHow would you rate our service (1-5): " ;
    cin >> serviceRating;

    cout << "\t\tHow would you rate our food (1-5): " ;
    cin >> foodRating;

    cout << "\t\tHow would you rate our food price (1-5): " ;
    cin >> priceRating;

    cout << "\t\tAny comment you would like to add on? :";
    cin.ignore();
    getline(cin, comment);

    if(!feedbackListFile.is_open()) //file not opened, error
    {
        cout << "Error! feedbackList.csv File is not opened!" << endl;
    }
    else{//save feedback result info into file
        feedbackListFile.seekg(0, ios::end);
        if (feedbackListFile.tellg() != 0) {// If record file is !empty

        }else{// if the file is empty, column names are added
            feedbackListFile << "Customer ID,Service Rating,Food Rating,Price Rating,Extra Comment" <<endl;
        }
        feedbackListFile << cID << "," << serviceRating << "," << foodRating << "," << priceRating << "," << comment << endl;
    }
}


void TableReservation::confirmReserv(int rID, int cID){
    TableReservation t;
    t.load_custList();
    list<Customer>::iterator it_custList;
    list<TableReservation>::iterator it_reservList;
    string custName;

    //retrieve custName from customer list
    for(it_custList = t.all_custList.begin(); it_custList != t.all_custList.end(); ++it_custList){
        if(it_custList->custID==cID)
            custName = it_custList->custName;
    }

    //create file with filename "<reservID>receipt.txt"
    ofstream receiptFile;
    string filename=to_string(rID);
    filename.append("receipt.txt");
    receiptFile.open(filename, ios::out);

    //print data into receipt
    for(it_reservList = all_reservList.begin(); it_reservList != all_reservList.end(); ++it_reservList){
        if(it_reservList->reservID==rID){ //if found current reservID
            receiptFile << "==============================" << "\n";
            receiptFile << "\t\tReceipt" << endl;
            receiptFile << "==============================" << "\n\n";
            receiptFile << "Reservation ID: " << it_reservList->reservID << "\n";
            receiptFile << "Customer ID: " << it_reservList->custID << "\n";
            receiptFile << "Full Name: " << custName << "\n\n";
            receiptFile << "Date: " << it_reservList->reservDate << "\t";
            receiptFile << "Time: " << it_reservList->reservTime << "\n\n";
            receiptFile << "Table No: " << it_reservList->reservTableNo << "\n";
            receiptFile << "Reservation Type: " << it_reservList->reservType << "\n";
            receiptFile << "No of ppl: " << it_reservList->reservPplNo << "\n";
            receiptFile << "==============================";
        }
    }
    cout << "\t\tReceipt is generated with filename \"" << filename << "\"." << endl << endl;
    receiptFile.close();
    system(filename.c_str()); //open receipt window
}
