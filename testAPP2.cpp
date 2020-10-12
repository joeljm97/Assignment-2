#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <iterator>
#include "logmanager.h"

using namespace std;

typedef map<string,string> stringmap;

class APP2
{
private:
    ifstream fs;
    stringmap m_pass,m_fail,m_abort;
    
    ofstream pass, fail, abort;

    string removeTag(string str) //gets string in between a <td></td> tag
    {
        writetolog(__LINE__, "Entered: " + funcName(__FUNCTION__));
        string::size_type beg, end;
        string s1;
        beg = str.find("<td>");
        beg = beg + 4;
        end = str.find("</td>");
        end = end;
        s1 = string(&str[beg], &str[end]);
        writetolog(__LINE__, "Returned value : "+s1+" From " + funcName(__FUNCTION__));
        return (s1);
    }

    string formatLogLoc(string str) //Formats log location string by removing additional <code> tags
    {
        writetolog(__LINE__, "Entered: " + funcName(__FUNCTION__));
        string::size_type beg, end;
        string s1;
        beg = str.find("file:/");
        end = str.find("><code>") - 1;
        s1 = string(&str[beg], &str[end]);
        writetolog(__LINE__, "Returned value : " + s1 + " From " + funcName(__FUNCTION__));
        return(s1);
    }

    string formatError(string str) //Formats error code so that it can be added in a csv file without issues 
    {
        writetolog(__LINE__, "Entered: " + funcName(__FUNCTION__));
        string s1;
        for (int i = 0; i < str.length(); i++)
        {
            if (str[i] != '"')
                s1 = s1 + str[i];
            else
                s1 = s1 + "\"\"";
        }
        s1 = "\"" + s1 + "\"";
        writetolog(__LINE__, "Returned value : " + s1 + " From "  + funcName(__FUNCTION__));
        return (s1);
    }

    void skip(ifstream& f, int n) //function to skip n number of lines in a file 
    {
        writetolog(__LINE__, "Entered: " + funcName(__FUNCTION__));
        string line;
        int count = 0;
        while (getline(f, line))
        {
            count++;
            if (count == n + 1)
                break;
        }
        writetolog(__LINE__, "Exited: " + funcName(__FUNCTION__));
    }

    void initializeFiles() // function to initialize the three csv files with column names
    {
        writetolog(__LINE__, "Entered: " + funcName(__FUNCTION__));
        pass.open("pass.csv");
        fail.open("fail.csv");
        abort.open("abort.csv");
        pass << string("Date_Time,Test_Name,Parameters,Log_Location\n"); writetolog(__LINE__, "pass.csv intialized");
        fail << string("Date_Time,Test_Name,Parameters,Log_Location,Error_Description\n"); writetolog(__LINE__, "fail.csv intialized");
        abort << string("Date_Time,Test_Name,Parameters,Log_Location,Error_Description\n"); writetolog(__LINE__, "abort.csv intialized");
        pass.close();
        fail.close();
        abort.close();
        writetolog(__LINE__, "Exited: " + funcName(__FUNCTION__));
    }

    void map_to_file(stringmap mp, ofstream& fo) //function to convert a STL::MAP to file
    {
        writetolog(__LINE__, "Entered: " + funcName(__FUNCTION__));
        stringmap::reverse_iterator itr;
        for (itr = mp.rbegin(); itr != mp.rend(); ++itr)
        {
            fo << itr->first<< itr->second << '\n';
        }
        writetolog(__LINE__, "Exited : " + funcName(__FUNCTION__));
    }

    public:void mainFunc()  
    {
        writetolog(__LINE__, "Entered: " + funcName(__FUNCTION__));
        string fileLoc, line, date = "";
        cout << "Enter file Location: ";
        writetolog(__LINE__, "waiting for user input");
        getline(cin, fileLoc);
        writetolog(__LINE__, "user entered :"+fileLoc);
        fs.open(fileLoc); //open results.html file
        skip(fs, 3);
        while (!fs)
        {
            cout << "\n\nUnable to open file\nTry again!!!!!!!\n\n";
            cout << "Enter file Location: ";
            getline(cin, fileLoc);
            fs.open(fileLoc);
        }
        if (!fs) //if file opening fails
        {
            cout << "Failed to open file\n";
        }

        else //if file opening is successful
        {

            initializeFiles();
            pass.open("pass.csv", ios::app);
            fail.open("fail.csv", ios::app);
            abort.open("abort.csv", ios::app);

            while (getline(fs, line)) //get each line from Results.html
            {
                
                int linelen = line.length();
                if (line.find("</table>") < linelen) // break while loop when it reaches </table> i.e, end of data
                    break;
                if (line.find("<tr>") > linelen && line.find("</tr>") > linelen)
                {
                    line = removeTag(line);
                    if (line == "PASSED")
                    {
                        writetolog(__LINE__, "Passed case met");
                        string str = "";
                        for (int i = 0; i < 3; i++)
                        {
                            getline(fs, line);
                            if (i != 2)
                            {
                                line = "," + removeTag(line);
                            }
                            else
                            {
                                line = removeTag(line);
                                line = "," + formatLogLoc(line);
                            }
                            str = str + line;
                        }
                        writetolog(__LINE__, "inserting to map: m_pass");
                        m_pass.insert({ date, str });
                    }
                    else if (line == "FAILED")
                    {
                        writetolog(__LINE__, "Failed case met");
                        string str = "";
                        for (int i = 0; i < 4; i++)
                        {
                            getline(fs, line);
                            line = removeTag(line);
                            if (i == 2)
                            {
                                line = formatLogLoc(line);
                            }
                            else if(i==3)
                            {
                                line = formatError(line);
                            }
                            str = str +","+ line;
                        }
                        writetolog(__LINE__, "inserting to map: m_fail");
                        m_fail.insert({ date, str });
                    }
                    else if (line == "ABORTED")
                    {
                        writetolog(__LINE__, "Aborted case met");
                        string str = "";
                        for (int i = 0; i < 4; i++)
                        {
                            getline(fs, line);
                            line = removeTag(line);
                            if (i == 2)
                            {
                                line = formatLogLoc(line);
                            }
                            else if (i == 3)
                            {
                                line = formatError(line);
                            }
                            str = str+ "," + line;
                        }
                        writetolog(__LINE__, "inserting to map: m_abort");
                        m_abort.insert({ date, str });
                    }

                }
                date = line; 
            }
            cout << "\nData has been separated.\n\n";
        }
        map_to_file(m_pass, pass);
        pass.close();
        map_to_file(m_fail, fail);
        fail.close();
        map_to_file(m_abort, abort);
        abort.close();
        
        writetolog(__LINE__, "Exited : " + funcName(__FUNCTION__));
    }
};

int main()
{
    initializeLog();
    APP2 A1; 
    writetolog(__LINE__, "Entered: " + funcName(__FUNCTION__));
    A1.mainFunc(); 
    endlog();
    system("pause");
}
