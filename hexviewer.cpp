/*
 *  Coded By : Mohamed El Sayed
 *  m.elsayed4420@gmail.com
 */

#include <iomanip>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <algorithm>

using namespace std;

string print_address();
string convert(const int);
void help();
bool file_is_good(const char*);

int main(int argc, char* argv[])
{
    int temp[16];
    int buff;
    ifstream read;
    bool dump_out = false;
    ofstream write;
    string file_path, all_flags, dump_temp, dump_path;
    
    bool dump_in_dec = false;

    if(argc < 2)
    {
        cout<<"insufficient parameters !\n\n";
        help();
        return 0;
    } 
    else if(argc > 4)
    {
        cout<<"Too many parameters !\n\n";
        help();
        return 0;
    }

    file_path = argv[1];

    if(argc >= 2)
    {
        for(int i = 2; i <  argc; i++)
        {
            all_flags += argv[i];
            all_flags += " ";
        }

        size_t dump_to_file = all_flags.find("-o");
        size_t dump_in_dec_t = all_flags.find("-dd");

        if(dump_to_file != string::npos)
        {
            all_flags.erase(dump_to_file, 2);
            dump_out = true;
            dump_path = file_path.c_str();
            dump_path += "-dump.txt";

            write.open(dump_path.c_str(), ofstream::out | ofstream::app);
        }
        else
            dump_out = false;

        if(dump_in_dec_t != string::npos)
        {
            all_flags.erase(dump_in_dec_t, 3);
            dump_in_dec = true;
        }
        else
            dump_in_dec = false;

        for(string::iterator it = all_flags.begin(); it != all_flags.end(); ++it)
        {
            if((int)* it != 32)
            {
                cout<<"Undefined flags :"<<all_flags<<endl<<endl;
                help();
                return 0;
            }
        }
    }

    read.open(file_path.c_str(), ios::in | ios::binary);

    if(read.good() == false)
    {
        cout<<"The file is not exists, enter a valid file path\n";
        return 0;
    }

    if(file_is_good(file_path.c_str()))
    {
        cout<<"Error : cannot dump a directory, pass a file.\n\n";
        help();
        return 0;
    }

    while(!read.eof()){

        cout<<"[0x"<<setw(6)<<setfill('0')<<print_address()<<"]\t";
        write<<"[0x"<<setw(6)<<setfill('0')<<print_address()<<"]\t";

        for(int i = 0; i < 16; i++)
        {
            buff=read.get();
            buff < 0 ? buff = 0 : buff = buff;
            if(dump_in_dec)
            {
                cout<<setw(3)<<buff<<"  ";
                write<<setw(3)<<buff<<"  ";
            }
            else
            {
                cout<<setw(3)<<convert(buff)<<"  ";
                write<<setw(3)<<convert(buff)<<"  ";
            }
            temp[i]=buff;
        }
        cout<<"\t\t";
        write<<"\t\t";
        for(x:temp)
        {
            if(x >= 127 || x < 32)
            {
                cout<<'.';
                write<<'.';
            }
            else
            {
                write<<(char)x;
                cout<<(char)x;
            }
        }

        cout<<endl;
        write<<"\n";
    }

    if(dump_out)
        cout<<endl<<endl<<"Your dumping output file saved at : "<<dump_path<<endl;

    write.close();
    read.close();
    return 0;
}


string print_address()
{
    static int value = -8;
    value += 8;
    stringstream ss;
    ss << hex << value;
    string op = ss.str();
    transform(op.begin(), op.end(), op.begin(), ::toupper);

    return op;
}

string convert(const int value)
{
    stringstream ss;
    ss << hex << value;
    string op = ss.str();
    transform(op.begin(), op.end(), op.begin(), ::toupper);

    return op;
}

bool file_is_good(const char* path)
{
    ifstream check;
    int sum = 0;
    check.open(path, ios::in | ios::binary);

    for(int i = 0; i < 10; ++i)
    {
        int temp = check.get();
        sum += temp;
    }
    
    check.close();

    if(sum <= 0)
        return true;
    else
        return false;
}

void help()
{
    cout<<"usage : <filename> [flags]\n";
    cout<<"\nflags : \n-o  : dump the target binary file out into an external text file.\n";
    cout<<"-dd : print the dumpped values in decimal instead of hexdecimal (the default is hex).\n";
}

