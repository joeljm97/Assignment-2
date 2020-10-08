#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include<iterator>

using namespace std;

typedef vector<string> stringvec;
string path = "D:\\file.html";
class APP2
{
private:
    ifstream fs;
    stringvec::iterator ptr;
    ofstream pass, fail, abort;

    string removeTag(string str)
    {
        for (int i = 0;i < str.length();i++)
        {
            int beg,end;
            string s1;
            beg = indexOf(str, "<td>");
            beg = beg + 4;
            end = indexOf(str, "</td>");
            s1 = string(&str[beg], &str[end]);
            return (s1);
        }

    }
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
    void skip(ifstream& f,int n)
    {
        string line;
        int count = 0;
        while (getline(f, line))
        {
            count++;
            if (count == n+1)
                break;
        }
    }
    void initializeFiles()
    {
        pass.open("pass.csv");
        fail.open("fail.csv");
        abort.open("abort.csv");
        pass << string("Date_Time,Log_Location,Parameters\n");
        fail << string("Date_Time,Log_Location,Parameters,Error_Description\n");
        abort << string("Date_Time,Log_Location,Parameters,Error_Description\n");
        pass.close();
        fail.close();
        abort.close();
    }

    public:void mainFunc() 
    {
        
        string fileLoc(path),line,date="";
        
        //getline(cin, fileLoc);
        fs.open(fileLoc);
        skip(fs,3);
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
                if (line.find("<tr>")>linelen && line.find("</tr>") > linelen)
                {
                    line=removeTag(line);
                    if (line == "Pass")
                    {

                    }
                    pass<< line << endl;
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
