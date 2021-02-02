#include <iostream>
#include <conio.h>
#include <fstream>
#include <string>
#include <algorithm>
#include "Reservation.h"
#include "TableReservation.h"

using namespace std;

void homePage();
string asterisk_password(); 
int generate_custID(); // custID of each customer assigned by system in sequence
void custSignup(); //get data to add new customer
void custLogin();
void custMenu(TableReservation t);
void adminLogin();
void adminMenu();


void homePage(){
    system("title Western Hut - Online Reservation System");
    system("cls");
    int choice;

    cout << "\n\t\t\tWelcome to Western Hut!" << endl << endl;
    cout << "\t\t_____________________________________________________" << endl;
    cout << "\t\t|                                                   |" << endl;
    cout << "\t\t|       Online Restaurant Reservation System        |" << endl;
    cout << "\t\t|___________________________________________________|" << endl << endl;
    cout << "\t\t\t-1-  Customer Login" << endl;
    cout << "\t\t\t-2-  Customer Sign Up" << endl;
    cout << "\t\t\t-3-  Admin Mode" << endl;
    cout << "\t\t\t-0-  Exit" << endl << endl;
    cout << "\t\t\tSelect -> ";
    cin >> choice;

    switch(choice){
        case 1: custLogin(); break;
        case 2: custSignup(); break;
        case 3: adminLogin(); break;
        case 0: exit(0); break;
        default: homePage(); break;
    }
}


string asterisk_password(){
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
    return true_password;
}


int generate_custID(){
    ifstream custIDCounterFile("custIDCounter.txt");
    int cID=1000; //if no customer exist, id=1000
    string line;

    if(custIDCounterFile.is_open()){
        if(!custIDCounterFile.eof()){
            getline(custIDCounterFile, line);
            int a = stoi(line);
            cID = a + 1; //increment last used id by 1 to become new id
        }
    }
    //else
        //cout << "\n\t\tUnable to open custIDCounter.txt" << endl;

    custIDCounterFile.close();
    return cID;
}


void custSignup(){
    system("title Western Hut - Sign Up");
    system("cls");

    TableReservation t;
    int cID, cContact; 
    string cName, cEmail, cPassword, cConfirmPassword; 

    cout << "\n\t\t__________________________________" << endl << endl;
    cout << "\t\t         Customer Sign Up         " << endl;
    cout << "\t\t__________________________________" << endl;
    cout << "\t\t     --enter 0 to go back--    " << endl << endl;

    cout << "\t\tName: ";
    cin.ignore();
    getline(cin, cName);
    if(cName == "0") homePage();

    cout << endl << "\t\tContact Number: ";
    cin >> cContact;
    if(cContact == 0) homePage();

    cout << endl << "\t\tEmail: ";
    cin >> cEmail;
    if(cEmail == "0") homePage();

    cout << endl << "\t\tPassword (max. 20 characters): ";
    cPassword = asterisk_password();
    if(cPassword == "0") homePage();

    cout << endl << "\t\tConfirm Password: ";
    cConfirmPassword = asterisk_password();
    if(cConfirmPassword == "0") homePage();
    while(cPassword != cConfirmPassword){
        cout << endl;
        cout << "\t\tYour password does not match! Please enter again. " << endl;

        cout << "\t\tPassword (max. 20 characters): ";
        cPassword = asterisk_password();
        if(cPassword == "0") homePage();

        cout << "\t\tConfirm Password: ";
        cConfirmPassword = asterisk_password();
        if(cConfirmPassword == "0") homePage();
    }

    cID = generate_custID();
    cout << endl << "\t\tYour Customer ID is " << cID << endl;
    t.set_custID(cID); //set customer ID for further actions

    t.tmp_custList.push_back(Customer(cName, cID, cContact, cEmail, cPassword)); //add to list
    t.addCust(); //add customer
    cout << "\t\tYou will now be directed to the Main Menu." << endl << endl;

    system("pause");
    custMenu(t); //redirect to customer menu under current customer ID
}


void custLogin(){
    system("title Western Hut - Log In");
    system("cls");

    int cID;
    string cPassword;
    TableReservation t;
    list<Customer>::iterator it_custList;
    t.load_custList();

    cout << "\n\t\t_______________________________" << endl << endl;
    cout << "\t\t         Customer Login        " << endl;
    cout << "\t\t_______________________________" << endl;
    cout << "\t\t     --enter 0 to go back--    " << endl << endl;

    cout << "\t\tCustomer ID: ";
    cin >> cID;
    if(cID == 0) homePage();

    cout << endl << "\t\tPassword: ";
    cPassword = asterisk_password();
    if(cPassword == "0") homePage();

    // check whether ID and password valid from list
    int flag=0;
    for(it_custList = t.all_custList.begin(); it_custList != t.all_custList.end(); ++it_custList){
        if(it_custList->custID==cID){ //if ID present
            if(it_custList->custPassword==cPassword){ //if password match with that ID
                flag=1; //true
            }
        }
    }

    if(flag==0){ //false
        cout << "\n\t\tID or password not found or not match! Try again." << endl << endl;
        system("pause");
        custLogin();
    }

    t.set_custID(cID);
    custMenu(t);
}


void adminLogin(){
    system("cls");
    string adminPassword, adminPasswordReal = "admin123";
    int attempts=1;

    cout << "\n\t\t_______________________________" << endl << endl;
    cout << "\t\t          Admin Login          " << endl;
    cout << "\t\t_______________________________" << endl;
    cout << "\t\t    --enter 0 to go back--     " << endl << endl;
    cout << "\t\tEnter password to enter admin mode, you have maximum 3 attempts." << endl << endl;
    do{
        cout << "\t\tAdmin password -> ";
        adminPassword=asterisk_password();
        if (adminPassword == "0"){
            homePage(); //if 0, go back
            break;
        }
        else if (adminPassword == adminPasswordReal){
            adminMenu(); //if match enter admin mode
            break;
        }
        attempts++; //if not match, increment attempt by 1
    }while(adminPassword!=adminPasswordReal && attempts<=3);

    if(attempts>3){ //if no more attempt
        cout << "\n\t\tSorry, wrong admin password. Try again later." << endl << endl;
        system("pause");
        homePage(); //go back
    }
}


void custMenu(TableReservation t){
    system("title Western Hut - Main Menu");
    system("cls");
    int choice;
    cout << "\n\t\t_____________________________________________________" << endl;
    cout << "\t\t|                                                   |" << endl;
    cout << "\t\t|                     Main Menu                     |" << endl;
    cout << "\t\t|___________________________________________________|" << endl << endl;
    cout << "\t\t\t-1- New reservation" << endl;
    cout << "\t\t\t-2- Display all reservation" << endl;
    cout << "\t\t\t-3- Search reservation" << endl;
    cout << "\t\t\t-4- Modify reservation" << endl;
    cout << "\t\t\t-5- Delete reservation" << endl;
    cout << "\t\t\t-0- Log Out" << endl << endl;
    cout << "\t\t\tSelect -> ";
    cin >> choice;

    switch(choice){
        case 1: {
            system("cls");
            system("title Western Hut - Add Reservation");
            cout << "\n\n\t\t    Add Reservation" << endl;
            t.addReserv();
            break;
        }
        case 2: t.displayReserv(); break;
        case 3: t.searchReserv(); break;
        case 4: t.modifyReserv(); break;
        case 5: t.deleteReserv(); break;
        case 0: { //log out
            system("cls");
            char flag;
            cout << "\n\t\tThanks for using our system." << endl;
            cout << "\t\tProceed to feedback before exit? (Y/N) -> ";
            cin >> flag;
            if(flag == 'y' || flag=='Y') t.addFeedback();

            cout << "\n\n\t\tSee you again :D" << endl << endl;
            system("pause");
            homePage();
            break;
        }
        default: custMenu(t); break;
    }
    custMenu(t);
}


void adminMenu(){
    system("title Western Hut - Admin Mode");
    system("cls");
    int choice;
    cout << "\n\t\t_____________________________________________________" << endl;
    cout << "\t\t|                                                   |" << endl;
    cout << "\t\t|                    Admin Mode                     |" << endl;
    cout << "\t\t|___________________________________________________|" << endl << endl;
    cout << "\t\t\t(A) Reservation" << endl;
    cout << "\t\t\t\t-1- New reservation" << endl;
    cout << "\t\t\t\t-2- Display all reservation" << endl;
    cout << "\t\t\t\t-3- Search reservation" << endl;
    cout << "\t\t\t\t-4- Modify reservation" << endl;
    cout << "\t\t\t\t-5- Delete reservation" << endl << endl;
    cout << "\t\t\t(B) Customer" << endl;
    cout << "\t\t\t\t-6- Display all customer record" << endl;
    cout << "\t\t\t\t-7- Search customer record" << endl ;
    cout << "\t\t\t\t-8- Modify customer record" << endl;
    cout << "\t\t\t\t-9- Delete customer record" << endl << endl;
    cout << "\t\t\t-0- Back" << endl << endl << endl;
    cout << "\t\t\tSelect -> ";
    cin >> choice;

    TableReservation t;
    switch(choice){
        case 1: {
            system("cls");
            cout << "\n\n\t\t    Add Reservation" << endl;
            t.addReserv();
            break;
        }
        case 2: t.displayReserv(); break;
        case 3: t.searchReserv(); break;
        case 4: t.modifyReserv(); break;
        case 5: t.deleteReserv(); break;
        case 6: t.displayCust(); break;
        case 7: t.searchCust(); break;
        case 8: t.modifyCust(); break;
        case 9: t.deleteCust(); break;
        case 0: homePage(); break;
        default: break;
    }
    adminMenu();
}


int main(){
    system("title Western Hut - Online Reservation System");
    homePage();
}
