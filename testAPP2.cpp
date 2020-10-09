#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include<iterator>

using namespace std;

typedef vector<string> stringvec;
string path = "Results.html";

class APP2
{
private:
    ifstream fs;
    stringvec::iterator ptr;
    ofstream pass, fail, abort;

    int indexOf(string s, string s1) //get the index position of a character or substring in a String
    {
        string::size_type loc = s.find(s1);
        if ((int)loc < s.length())
        {
            return int(loc);
        }
        else
        {
            return -1;
        }
    }

    string removeTag(string str)
    {
        int beg, end;
        string s1;
        beg = indexOf(str, "<td>");
        beg = beg + 4;
        end = indexOf(str, "</td>");
        end = end;
        s1 = string(&str[beg], &str[end]);
        return (s1);
    }
    string formatLogLoc(string str)
    {
        int beg, end;
        string s1, s2 = "";
        beg = indexOf(str, "file:/");
        end = indexOf(str, "><code>") - 1;
        s1 = string(&str[beg], &str[end]);
        return(s1);
    }
    string formatError(string str)
    {
        string s1;
        for (int i = 0; i < str.length(); i++)
        {
            if (str[i] != '"')
                s1 = s1 + str[i];
            else
                s1 = s1 + "\"\"";
        }
        s1 = "\"" + s1 + "\"";
        return (s1);
    }
    void skip(ifstream & f, int n)
    {
        string line;
        int count = 0;
        while (getline(f, line))
        {
            count++;
            if (count == n + 1)
                break;
        }
    }

    public:void initializeFiles()
    {
        pass.open("pass.csv");
        fail.open("fail.csv");
        abort.open("abort.csv");
        pass << string("Date_Time,Test_Name,Parameters,Log_Location\n");
        fail << string("Date_Time,Test_Name,Parameters,Log_Location,Error_Description\n");
        abort << string("Date_Time,Test_Name,Parameters,Log_Location,Error_Description\n");
        pass.close();
        fail.close();
        abort.close();
    }

    void mainFunc()
    {

        string fileLoc(path), line, date = "";
        initializeFiles();
        pass.open("pass.csv", ios::app);
        fail.open("fail.csv", ios::app);
        abort.open("abort.csv", ios::app);
        //getline(cin, fileLoc);
        fs.open(fileLoc);
        skip(fs, 3);
        if (!fs)
        {
            cout << "Failed to open file";
        }
        else
        {
            while (getline(fs, line))
            {
                int linelen = line.length();
                if (line.find("</table>") < linelen)
                    break;
                if (line.find("<tr>") > linelen && line.find("</tr>") > linelen)
                {
                    line = removeTag(line);
                    if (line == "PASSED")
                    {
                        pass << date;
                        for (int i = 0; i < 4; i++)
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
                            pass << line;
                        }
                        pass << "\n";
                    }
                    else if (line == "FAILED")
                    {
                        fail << date;
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
                            fail << "," << line;
                        }
                        fail << "\n";
                    }

                }
                date = line;
            }
        }

    }
};

    int main()
    {
        APP2 A1;
        A1.mainFunc();

    }
