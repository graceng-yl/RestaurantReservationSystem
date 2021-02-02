//run to create dateAvailability<year>.csv
#include <iostream>
#include <fstream>
using namespace std;

int main(){
    ofstream datefile("dateAvailability2020.csv"); //change file name
    int y=2020, m1, m2, d1, d2; //change year value
    int numOfDays;

    for(int i=1; i<=12; i++){
        if (i<10){
            m1=0;
            m2=i;
        }
        else{
            m1=1;
            m2=i-10;
        }

        if ((m1==0)&&(m2==2)){ //month == 02
            bool leapYear = false;
            if (((y%4==0) && (y%100!=0)) || (y%400==0))
                leapYear = true;
            numOfDays = leapYear ? 29 : 28; // if leap year true = 29 days, else false = 28
        }
        else if ( ((m1==0)&&(m2==4)) || ((m1==0)&&(m2==6)) || ((m1==0)&&(m2==9)) || ((m1==1)&&(m2==1)) ){
            numOfDays = 30;
        }
        else{
            numOfDays = 31;
        }

        for(int j=1; j<=numOfDays; j++){
            if (j<10){
                d1=0;
                d2=j;
            }
            else if((j>=10)&&(j<20)){
                d1=1;
                d2=j-10;
            }
            else if((j>=20)&&(j<30)){
                d1=2;
                d2=j-20;
            }
            else{
                d1=3;
                d2=j-30;
            }
            for(int t=1700; t<=2300; t=t+100){
                datefile << y << m1 << m2 << d1 << d2 << "," << t << "," << 0 << endl;
            }
        }
    }
    datefile << endl;
}
