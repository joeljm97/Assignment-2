#ifndef logmanager
#define logmanager

#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <string>
#include <time.h>
#include <fstream>
#include <iomanip>
using namespace std;

string getTimeStr() // returns the timestamp
{
    time_t now = time(0);
    string dt = ctime(&now);
    string st = "";
    for (int i = 0;i < dt.length() - 1;i++)
    {
        st = st + dt[i];
    }
    return st;
}

string createlogname() //creates a formatted valid name for log file using time stamp
{
    int count = 0;
    string str = "log_" + getTimeStr(), str1 = "";
    for (int i = 0;i < str.length();i++)
    {
        if (str[i] == ' ')
            str1 = str1 + '_';
        else if (str[i] == ':')
            str1 = str1 + '-';
        else
            str1 = str1 + str[i];
    }
    str1 = str1 + ".log";
    return(str1);
}
string str = createlogname();
void initializeLog() // creating the log file and adding the column names
{
    
    ofstream log;
    log.open(str);
    log << "Start Time: " << getTimeStr() << "\n";
    if (!log)
        cout << "creation failed";
    else
    {
        log << left << setw(20) << setfill(' ') << "Serial number";
        log << left << setw(30) << setfill(' ') << "Timestamp";
        log << left << setw(20) << setfill(' ') << "Line number";
        log << left << setw(50) << setfill(' ') << "Event";
        log << "\n";
    }
    log.close();
}

string funcName(const char* func) //returns current function name as a string 
{
    string s;
    s.assign(func);
    s = s + "()";
    return s;
}

void writetolog(int line, string msg)
{
    int static serial = 1;
    ofstream log;
    log.open(str, ios::app);
    log << left << setw(20) << setfill(' ') << serial++;
    log << left << setw(30) << setfill(' ') << getTimeStr();
    log << left << setw(20) << setfill(' ') << line;
    log << left << setw(50) << setfill(' ') << msg;
    log << "\n";
}

void endlog()
{
    ofstream log;
    log.open(str, ios::app);
    log << "\nApplication ran successfully with no error\n";
    log << "\nEnd Time: " << getTimeStr()<<endl;
    log.close();
}

#endif