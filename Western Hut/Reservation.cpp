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
#include <conio.h>
#include "Reservation.h"

using namespace std;


void Reservation::load_custList(){
    string cName, cID, cContact, cEmail, cPassword;// helper variables
    vector<string> cNameVec, cEmailVec, cPasswordVec; // helper DS
    vector<int> cIDVec, cContactVec; // helper DS
    int row = 0;
    int int_cID, int_cContact;

    ifstream custRecordFile("custRecord.csv");
    if(custRecordFile.is_open()){//check file open or not

        string line;
        getline(custRecordFile, line); // not need to store first line, the caption
        while(!custRecordFile.eof()){ // if end of file, jump out of while loop
            getline(custRecordFile, cName, ',');
            cNameVec.push_back(cName);

            getline(custRecordFile, cID, ',');
            int_cID =stoi(cID); // change from string to int then store into customer id vector
            cIDVec.push_back(int_cID);

            getline(custRecordFile, cContact, ',');
            int_cContact = stoi(cContact);
            cContactVec.push_back(int_cContact);

            getline(custRecordFile, cEmail, ',');
            cEmailVec.push_back(cEmail);

            getline(custRecordFile, cPassword, '\n');
            cPasswordVec.push_back(cPassword);
            row += 1;
        }

        custRecordFile.close();
        if(!tmp_custList.empty()){
            all_custList.assign(tmp_custList.begin(),tmp_custList.end());// copy new Customer info to existed Customer record, all_custList
        }
        Customer c;
        for(int j = 0; j <row-1; j++){// load existed Customer into list
            c.custName = cNameVec[j];
            c.custID = cIDVec[j];
            c.custContact = cContactVec[j];
            c.custEmail = cEmailVec[j];
            c.custPassword = cPasswordVec[j];
            all_custList.push_back(c);
        }
        cout << endl;
    }
    else{
        cout << "Unable to open custRecord.csv" << endl;
    }
}

void Reservation::save_custList(){// for modify() function
    fstream custRecordFile("custRecord.csv", ios::out);//overwrite file functions
    if(!custRecordFile.is_open()) //file not opened, error
    {
        cout << "Error! custRecord.csv is not opened!" << endl;
    }
    else{//save_custList into file
        custRecordFile << "Name,Customer ID,Contact Number,Email,Password" <<endl;
        list<Customer> :: iterator it_custList;
        for(it_custList = all_custList.begin(); it_custList != all_custList.end(); ++it_custList){//add latest dynamic customer record to file
            custRecordFile << it_custList->custName << ","
                           << it_custList->custID << ","
                           << it_custList->custContact << ","
                           << it_custList->custEmail <<","
                           << it_custList->custPassword << endl;
        }
    }
    custRecordFile.close();
}

void Reservation::addCust(){//add person in linked list n file
    fstream custRecordFile("custRecord.csv", ios::app);
    ofstream custIDCounterFile("custIDCounter.txt"); //a file storing last ID used

    list<Customer> :: iterator it_custList;
    if(!custRecordFile.is_open())
    {
        cout << "Error! custRecord.csv is not opened!" << endl;
    }
    else
    {
        custRecordFile.seekg(0, ios::end); // check file empty or not
        if (custRecordFile.tellg() != 0) { // If record file is !empty, load the existed Customer into the list
            load_custList();
        }else{ // if the file is empty, column names are added
            custRecordFile << "Name,Customer ID,Contact Number,Email,Password" <<endl;
        }
        for(it_custList = tmp_custList.begin(); it_custList != tmp_custList.end(); ++it_custList){//add customer to file
            custRecordFile << it_custList->custName << ","
                           << it_custList->custID << ","
                           << it_custList->custContact << ","
                           << it_custList->custEmail << ","
                           << it_custList->custPassword << endl;
            custIDCounterFile << it_custList->custID;
        }
        cout << "\t\tCustomer record added." << endl << endl;
    }
    custRecordFile.close();
    custIDCounterFile.close();

}


void Reservation::displayCust(){
    system("cls");
    load_custList(); //make sure all customer records in file are loaded into ds
    list<Customer> :: iterator it_custList;
    cout << "\n\n\t\tList of Customers" << endl << endl;

    //print column name
    printf("%*s \t", 8, custCaptions[0].c_str());
    printf("%*s \t", 15, custCaptions[1].c_str());
    printf("%*s \t", 10, custCaptions[2].c_str());
    printf("%*s \n", 10, custCaptions[3].c_str());

    for(it_custList = all_custList.begin(); it_custList != all_custList.end(); ++it_custList){
        //print customer details
        printf("%*d \t",8, it_custList->custID);
        printf("%*s \t",15, it_custList->custName.c_str());
        printf("%*d \t",10, it_custList->custContact);
        printf("%*s \n",15, it_custList->custEmail.c_str());
    }

    //prompt to proceed sort or not
    char proceedSort;
    cout << "\n\t\tSort? (Y/N) ";
    cin >> proceedSort;
    if(proceedSort=='Y' || proceedSort=='y') sortCust();
}


void Reservation::searchCust(){
    system("cls");
    load_custList();
    int cID, flag, choice;

    cout << "\n\n\t\t   Search Customer" << endl;
    cout << "\t\t--enter 0 to go back--" << endl << endl;
    cout << "\n\n\t\tEnter ID to search: ";
    cin >> cID;
    if(cID==0) return;

    list<Customer> :: iterator it_custList;
    cout << "\n\n\t\tSearch Result" << endl << endl;

    for(it_custList = all_custList.begin(); it_custList != all_custList.end(); ++it_custList){
        if(cID == it_custList->custID){ //if customer ID in list same with customer ID entered
            //print column name
            printf("%*s \t", 8, custCaptions[0].c_str());
            printf("%*s \t", 15, custCaptions[1].c_str());
            printf("%*s \t", 10, custCaptions[2].c_str());
            printf("%*s \n", 10, custCaptions[3].c_str());

            //print search result
            printf("%*d \t",8, it_custList->custID);
            printf("%*s \t",15, it_custList->custName.c_str());
            printf("%*d \t",10, it_custList->custContact);
            printf("%*s \n",15, it_custList->custEmail.c_str());
            flag=1;
            break;
        }
        else //else print ID not found
            flag=0;
    }

    if(flag==0){
        cout << "\t\tID not found" << endl;
    }
    cout << endl;
    system("pause");
}


//<---------Merge Sort Functions--------->
void mergeArrayByDescendingCustomerID(Customer tempArray[], int left, int middle, int right)
{
    int i, j, k;
    int n1 = middle - left + 1;
    int n2 = right - middle;

    Customer L[n1], R[n2];

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
        if (L[i].custID <= R[j].custID) {
            tempArray[k] = R[j];
            j++;
        }
        else {
            tempArray[k] = L[i];
            i++;
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

void mergeArraybyAscendingCustomerName(Customer tempArray[], int left, int middle, int right)
{
    int i, j, k;
    int n1 = middle - left + 1;
    int n2 = right - middle;

    Customer L[n1], R[n2];

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
        if (L[i].custName <= R[j].custName) {
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

void mergeSort(string mode, Customer tempArray[], int left, int right){
    if (left < right){
        int mid = left + (right - left) / 2;

        mergeSort(mode, tempArray, left, mid);
        mergeSort(mode, tempArray, mid + 1, right);

        if (mode == "descendingCustomerID"){
            mergeArrayByDescendingCustomerID(tempArray, left, mid, right);
        }else if (mode == "ascendingCustomerName"){
            mergeArraybyAscendingCustomerName(tempArray, left, mid, right);
        }
    }
}


//<---------Main Call Function--------->
void Reservation::sortCust(){
    system("cls");
    list<Customer> :: iterator it_custList;
    int sortType = 0;
    cout << "\n\n\t\tSelect Sort Type" << endl;
    cout << "\t\t(1) Descending Customer ID" << endl;
    cout << "\t\t(2) Ascending Customer Name" << endl;
    cout << "\t\t(3) Descending Customer Name" << endl << endl;
    cout << "\t\tSelect -> ";
    cin >> sortType;

    system("cls");

    Customer mergeSortMainArray[all_custList.size()];
    int i = 0;
    //push records in list into array
    for(it_custList = all_custList.begin() ; it_custList != all_custList.end(); ++it_custList, i++){
        mergeSortMainArray[i] = *it_custList;
        mergeSortMainArray[i].custName[0] = toupper(mergeSortMainArray[i].custName[0]); //change first letter of customer name to capital letter
    }

    if(sortType == 1){
        //merge sort
        mergeSort("descendingCustomerID", mergeSortMainArray, 0, (sizeof(mergeSortMainArray)/sizeof(mergeSortMainArray[0])) - 1);

        cout << "\n\n\t\tSort Customer by Descending Customer ID" << endl << endl;
        //print column name
        printf("%*s \t", 8, custCaptions[0].c_str());
        printf("%*s \t", 15, custCaptions[1].c_str());
        printf("%*s \t", 10, custCaptions[2].c_str());
        printf("%*s \n", 10, custCaptions[3].c_str());

        for(int i = 0; i < (sizeof(mergeSortMainArray)/sizeof(mergeSortMainArray[0])); i++){
            //print array
            printf("%*d \t",8, mergeSortMainArray[i].custID);
            printf("%*s \t",15, mergeSortMainArray[i].custName.c_str());
            printf("%*d \t",10, mergeSortMainArray[i].custContact);
            printf("%*s \n",15, mergeSortMainArray[i].custEmail.c_str());
        }
    }else{
        //merge sort
        mergeSort("ascendingCustomerName", mergeSortMainArray, 0, (sizeof(mergeSortMainArray)/sizeof(mergeSortMainArray[0])) - 1);

        if(sortType == 2){
            cout << "\n\n\t\tSort Customer by Ascending Customer Name" << endl << endl;
            //print column name
            printf("%*s \t", 8, custCaptions[0].c_str());
            printf("%*s \t", 15, custCaptions[1].c_str());
            printf("%*s \t", 10, custCaptions[2].c_str());
            printf("%*s \n", 10, custCaptions[3].c_str());

            for(int i = 0; i < (sizeof(mergeSortMainArray)/sizeof(mergeSortMainArray[0])); i++){
                //print array
                printf("%*d \t",8, mergeSortMainArray[i].custID);
                printf("%*s \t",15, mergeSortMainArray[i].custName.c_str());
                printf("%*d \t",10, mergeSortMainArray[i].custContact);
                printf("%*s \n",15, mergeSortMainArray[i].custEmail.c_str());
            }
        }else if(sortType == 3){
            stack<Customer> descendingSortStack;
            //push array into stack
            for(int i = 0; i < (sizeof(mergeSortMainArray)/sizeof(mergeSortMainArray[0])); i++){
                descendingSortStack.push(mergeSortMainArray[i]);
            }
            cout << "\n\n\t\tSort Customer by Descending Customer Name" << endl << endl;
            //print column name
            printf("%*s \t", 8, custCaptions[0].c_str());
            printf("%*s \t", 15, custCaptions[1].c_str());
            printf("%*s \t", 10, custCaptions[2].c_str());
            printf("%*s \n", 10, custCaptions[3].c_str());

            while(!descendingSortStack.empty()){
                //print stack
                printf("%*d \t",8, descendingSortStack.top().custID);
                printf("%*s \t",15, descendingSortStack.top().custName.c_str());
                printf("%*d \t",10, descendingSortStack.top().custContact);
                printf("%*s \n",15, descendingSortStack.top().custEmail.c_str());
                descendingSortStack.pop();
            }
        }
    }
    cout << endl;
    system("pause");
}
//<---------Full Sort Ends--------->


void Reservation::modifyCust(){
    system("cls");
    load_custList();
    Customer c;
    int cID = 0, cContact;
    string cName, cEmail, cPassword;
    bool flag = false;

    cout << "\n\n\t\t  Modify Customer" << endl;
    cout << "\n\t\tEnter customer ID to be modified: " ;
    cin >> cID;

    list<Customer> clone_custList ;// a temporary linked list

    for (list<Customer>::iterator it_custList = all_custList.begin(); it_custList != all_custList.end(); )
    {
        if (it_custList->custID != cID){// if different CID, store into clone
            clone_custList.push_back(*it_custList);
        }
        else {
            flag = true;
        }
        it_custList++;
    }
    if (!flag){// id is not found
        cout << "\n\t\t" << cID << " not found! "<< endl;
    }
    else{
        system("cls");
        cout << "\n\n\t\t  Modify Customer" << endl;
        all_custList.clear();//clear the all customer record
        all_custList.assign(clone_custList.begin(),clone_custList.end());// copy all contents from clone to allCustomer
        save_custList();// save into file
        if(flag){ //enter new details
            cout << "\n\t\tName: ";
            cin.ignore();
            getline(cin, cName);

            cout << endl << "\t\tContact Number: ";
            cin >> cContact;

            cout << endl << "\t\tEmail: ";
            cin >> cEmail;

            cout << endl << "\t\tPassword (max. 20 characters): ";

            //--start to asterisk password--
            char get_character;
            char password[20] = {0};
            int i = 0;
            do{
                get_character = getch();

                if(get_character != '\b'){ //check if backspace or not, normal character case
                    password[i] = get_character;
                    cout << "*";
                    i++;
                }else if(get_character == '\b'){ //backspace case
                    password[--i] = {0};
                    cout << '\b';
                }
            }while(get_character != '\r' && i != 20);
            cout << string ((20-i), '*') << endl;
            char true_password[i]={0};
            i--;

            for(int j=0; j<i; j++){
                true_password[j] = password[j];
            }
            cPassword=true_password;
            //--end of asterisk password--

            c.custID = cID;
            c.custName = cName;
            c.custContact = cContact;
            c.custEmail = cEmail;
            c.custPassword = cPassword;

            tmp_custList.push_back(c);
            addCust(); //add new details
        }
    }
    cout << endl;
    system("pause");
}


void Reservation::deleteCust(){
    system("cls");
    load_custList();
    int cID;
    bool flag = false;

    cout << "\n\n\t\t  Delete Customer" << endl;
    cout << "\t\t--enter 0 to go back--" << endl << endl;
    cout << "\n\t\tEnter customer ID to be removed: " ;
    cin >> cID;
    if(cID==0) return;

    list<Customer> clone_custList ;
    for (list<Customer>::iterator it_custList = all_custList.begin(); it_custList != all_custList.end(); )
    {
        if (it_custList->custID != cID){// if different Customer ID, store into clone
            clone_custList.push_back(*it_custList);
        }
        else if(it_custList->custID == cID){
            flag = true;
        }
        it_custList++;
    }

    if (!flag){// id is not found
        cout << "\n\t\t" << cID << "is not found! "<< endl;
    }
    else{
        all_custList.clear();//clear the all customer record
        all_custList.assign(clone_custList.begin(),clone_custList.end());// copy all contents from clone to allCustomer
        save_custList();// save into file
        cout << "\n\t\tCustomer Record deleted." <<endl;
    }
    cout << endl;
    system("pause");
}
