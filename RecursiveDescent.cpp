/*
AIM: To construct a Recursive Descent Parser
Author: Arnab Chaudhury
Date: 6/5/2023
*/

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
using namespace std;

vector<string> Productions;
vector<string> nonLeftProductions;
vector<char> states;

// To check if the given character is present in the given vector
bool containsVectChar(vector<char> vect, char k)
{
    for(int i=0;i<vect.size();i++)
    {
        if(vect[i]==k)
            return true;
    }
    return false;
}

// To check if the given string contains a state
bool containsState(string line)
{
    for(int i=0;i<line.length();i++)
    {
        if(containsVectChar(states,line[i]))
            return true;
    }
    return false;
}

// To find the index of the first state in the given line
int firstStateIndex(string line)
{
    for(int i=0;i<line.length();i++)
    {
        if((int)line[i]>=65 && (int)line[i]<=90)
            return i;
    }
    return line.length();
}

// To calculate the number of productions froma state
int prodPerStateCount(char k)
{
    int count=0;
    for(int i=0;i<nonLeftProductions.size();i++)
    {
        if(nonLeftProductions[i][0]==k)
            count++;
    }
    return count;
}

// To find the productions from a state
string* prodsPerState(char k)
{
    int j=0;
    string* strArr=new string[prodPerStateCount(k)];
    for(int i=0;i<nonLeftProductions.size();i++)
    {
        if(nonLeftProductions[i][0]==k)
            strArr[j++]=nonLeftProductions[i].substr(1);
    }
    return strArr;
}

// To get a new dummy state
char getNewState()
{
    static int i=65;
    while(containsVectChar(states, (char)i))
        i++;
    states.push_back((char)i);

    return (char)i;
}   

// To remove left recursion from the given production
void leftRecursive()
{
    for(int i=0;i<Productions.size();i++)
    {   
        if(Productions[i][0]==Productions[i][1])
        {
            char newState=getNewState();
            string NewState(1,newState);
            string changedProd=Productions[i];
            changedProd.erase(changedProd.begin());
            changedProd.erase(changedProd.begin());
            changedProd=newState+changedProd+newState;
            
            int lastJ=0;
            for(int j=i+1;j<Productions.size();j++)
            {
                if(Productions[j][0]==Productions[i][0])
                {
                    nonLeftProductions.push_back(Productions[j]+newState);
                    lastJ=j;
                }
            }
            nonLeftProductions.push_back(changedProd);
            nonLeftProductions.push_back(NewState+'^');            
            
            i=lastJ;
        }
        else
            nonLeftProductions.push_back(Productions[i]);
    }

    cout<<"After removal of Left Recursion :"<<endl;
    for(int i=0;i<nonLeftProductions.size();i++)
    {
        cout<<nonLeftProductions[i].substr(0,1)+"->"+nonLeftProductions[i].substr(1)<<endl;
    }
    cout<<"\n\n";
}

// To process the given productions fot easier manipulation
void processProductions(ifstream &file)
{
    string line;
    while(getline(file,line))
    {
        for(int i=0;i<line.length();i++)
        {
            if(line[i]=='-' && line[i+1]=='>')
            {
                line.erase(line.begin()+i);
                line.erase(line.begin()+i);
            }
            if(!containsVectChar(states,line[i]) && ((int)line[i]>=65 && (int)line[i]<=90))
                states.push_back(line[i]);
        }

        string part_prod="";
        for(int i=1;i<line.length();i++)
        {
            if(line[i]=='|')
            {
                Productions.push_back(line[0]+part_prod);
                part_prod="";
            }
            else if(i==line.length()-1)
            {
                Productions.push_back(line[0]+part_prod+line[i]);
                part_prod="";
            }
            else
                part_prod+=line[i];
        }
    }

    cout<<"Productions :"<<endl;
    for(int i=0;i<Productions.size();i++)
    {
        cout<<Productions[i].substr(0,1)+"->"+Productions[i].substr(1)<<endl;
    }
    cout<<"\n\n";
}

// To parse the given string using recursive descent
void recursiveDescent(string input)
{
    string treeLineBuffer(1,nonLeftProductions[0][0]);
    int used[]={0,0,0,0,1,0,0,0,1,1,1};


    for(int i=0;i<sizeof(used)/sizeof(int);i++)
    {
        int firstStateInd=firstStateIndex(treeLineBuffer);
        char firstState=treeLineBuffer[firstStateIndex(treeLineBuffer)];
        treeLineBuffer.erase(treeLineBuffer.begin()+firstStateInd);
        if(prodsPerState(firstState)[used[i]].compare("^")==0)
            treeLineBuffer=treeLineBuffer.substr(0,firstStateInd)+treeLineBuffer.substr(firstStateInd);
        else
            treeLineBuffer=treeLineBuffer.substr(0,firstStateInd)+prodsPerState(firstState)[used[i]]+treeLineBuffer.substr(firstStateInd);
        cout<<treeLineBuffer<<endl;
    }
}

// Driver code
int main()
{
    ifstream file;
    file.open("grammar2.txt");

    string input_string;
    cin>>input_string;

    processProductions(file);
    leftRecursive();
    recursiveDescent(input_string);

    file.close();
    return 0;
}

