#include <bits/stdc++.h>

using namespace std;

typedef long double ld;

const int limit_streamsize = 1000;

// This query string will store information about the design of pumps
string query;

void Read_query_directly()
{
    // discards the input buffer,this will make sure string input functionality works properly.
    cin.ignore(limit_streamsize,'\n');

    string str;

    cout<<"Enter Query String: ";
    getline(cin,str);

    query = str;

    return;
}

void print_spaces(int num)
{
    // This is used for proper spacing in interactive input.
    while(num--)
        cout<<"\t";
}

string Read_query_interactively(int len_space)
{
    // This function will take input about the design of pumps and
    // build the query string recursively.

    string res = "";

    char type;
    print_spaces(len_space);
    cout<<"Enter connection type (S/P): ";
    cin>>type;

    if(type=='S')res+='S';
    else if(type=='P') res+='P';
    else
    {
        cout<<"Warning : Wrong connection type, Assuming connection in parallel(P)."<<endl;
        res+='P';
    }
    res+='(';

    int num;
    print_spaces(len_space);
    cout<<"Enter number of components: ";
    cin>>num;

    for(int i=0;i<num;i++)
    {
        string val;

        char response;
        print_spaces(len_space);
        cout<<i+1<<". Does this component contains multiple pumps (Y/N): ";
        cin>>response;

        if(response == 'Y')
        {
            val = Read_query_interactively(len_space+1);
        }
        else
        {
            print_spaces(len_space);
            cout<<i+1<<". Enter Conductance of pump: ";
            cin>>val;
        }

        if(i>0)res+=',';
        res+=val;
    }

    res+=')';

    return res;
}

bool check_query_string()
{
    // This function will check whether brackets of query string are balanced or not

    stack<char>stk;

    for(int i=0;i<(int)query.length();i++)
    {
        if(query[i]=='(')
        {
            stk.push('(');
        }
        else if(query[i]==')')
        {
            if(stk.empty())
            {
                return false;
            }
            else
            {
                stk.pop();
            }
        }
        else
        {
            continue;
        }
    }

    return stk.empty();
}

ld series_conductance(vector<ld>v)
{
    // This function will return equivalent conductance of pumps
    // which are connected in series

    ld equivalent = 0;

    for(int i=0;i<(int)v.size();i++)
    {
        equivalent += 1.0/v[i];
    }

    equivalent = 1.0/equivalent;

    return equivalent;
}

ld parallel_conductance(vector<ld>v)
{
    // This function will return equivalent conductance of pumps
    // which are connected in parallel
    ld equivalent = 0;

    for(int i=0;i<(int)v.size();i++)
    {
        equivalent += v[i];
    }

    return equivalent;
}

pair<int,ld> solve_section(int i)
{
    // This function will recursively compute conductance value for every section
    // and it will return two parameters. first parameter will give the index in
    // query string till which conductance is already computed and second parameter
    // will give equivalent conductance for current section.

    // Defining a vector that will store conductances for current section
    vector <ld> temp;

    // This will help in getting value of conductance from query string
    string curr="";

    // position i has information about connecting type of this section (Series/parallel)
    char connection_type = query[i];

    // Iterating through query string from position after section type
    i++;

    while(i<(int)query.length())
    {
        if(query[i]==' ' || query[i]=='(')
        {
            i++;
            continue;
        }
        else if(query[i]==')')
        {
            if(curr!="")
            {
                ld val = stold(curr);
                temp.push_back(val);
            }
            break;
        }
        else if(query[i]==',')
        {
            if(curr!="")
            {
                // Converting string into long double data type.
                ld val = stold(curr);
                temp.push_back(val);
            }
            curr="";
        }
        else if(query[i]=='S' || query[i]=='P')
        {
            // Calculating Conductance value for current found section recursively
            pair<int,ld> val = solve_section(i);
            temp.push_back(val.second);
            i=val.first;
        }
        else
        {
            curr += query[i];
        }
        i++;
    }

    pair<int,ld>res;

    res.first = i;

    if(connection_type=='S')
    {
        res.second = series_conductance(temp);
    }
    else
    {
        res.second = parallel_conductance(temp);
    }

    return res;
}

ld calculate()
{
    int i=0;
    // Removing whitespace from beginning(if any) from query string
    while(i<(int)query.length() && query[i]==' ')i++;

    // Calculating Equivalent conductance for complete design
    pair<int,ld> res = solve_section(i);

    return res.second;
}

int main()
{
    // Running this loop for multiple queries of conductance calculation.
    // User will be given an option to exit after processing each query.
    while(true)
    {
        // Selecting type of inputs.
        int type;
        cout<<"Select one of these Options:"<<endl;
        cout<<"1: Enter query string directly\t2: Enter conductance values in interactive way"<<endl;
        cout<<"Enter your option (1/2): ";
        cin>>type;

        // Taking inputs in desired way.
        if(type==1)Read_query_directly();
        else
        {
            query = Read_query_interactively(0);
            cout<<"Query string created by interactive input is: "<<query<<endl;
        }

        // Check if query string is in correct format or not
        bool IsBalanced = check_query_string();

        if(IsBalanced == false)
        {
            cout<<"ERROR : Provided Query String is not in correct expected format"<<endl;
            cout<<"Try Again...\n"<<endl;
            continue;
        }

        // Calculating equivalent conductance value
        ld ans = calculate();

        // Printing Equivalent Conductance
        cout<<"Equivalent Conductance of the system is: ";
        cout<<fixed<<setprecision(10)<<ans<<endl;

        // Selecting options to either continue or exit the program.
        int operation;
        cout<<"\nSelect one of these Options:"<<endl;
        cout<<"1: Calculate Conductance\t2: Exit"<<endl;
        cout<<"Enter your option (1/2): ";
        cin>>operation;
        if(operation==1)
        {
            continue;
        }
        else
        {
            break;
        }
    }

    return 0;
}
