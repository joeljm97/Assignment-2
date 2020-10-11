#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include<iterator>

using namespace std;

typedef map<string,string> stringmap;
string path = "Results.html";

class APP2
{
private:
    ifstream fs;
    stringmap m_pass,m_fail,m_abort;
    
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

    void map_to_file(stringmap mp, ofstream& fo)
    {
        stringmap::reverse_iterator itr;
        for (itr = mp.rbegin(); itr != mp.rend(); ++itr)
        {
            fo << itr->first<< itr->second << '\n';
        }
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
                        m_pass.insert({ date, str });
                    }
                    else if (line == "FAILED")
                    {
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
                        cout << str<<endl;
                        m_fail.insert({ date, str });
                    }
                    else if (line == "ABORTED")
                    {
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
                        m_abort.insert({ date, str });
                    }

                }
                date = line;
            }
        }
        map_to_file(m_pass, pass);
        pass.close();
        map_to_file(m_fail, fail);
        fail.close();
        map_to_file(m_abort, abort);
        abort.close();
    }
};

    int main()
    {
        APP2 A1;
        A1.mainFunc();

    }
