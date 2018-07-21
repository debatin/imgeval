#ifndef DATASET_H
#define DATASET_H
#include <map>
#include <iostream>
#include <vector>
using namespace std;
typedef unsigned int uint;

//! provides different types of data
/*! wrapper class to store different types of data
a value should be set via the set functions.
currently strings double and vector<double> are supported.
The data can ba accessed directly.
(I know bad style but this is a small class and .d is shorter than .d() )
 
*/
class m_entry
{
 public:
  m_entry();
  void set(const string &s,char category='d');
  void set(const double &d,char category='d');
  void set(const int &i,char category='d');
  void set(const vector<double> &vd,char category='d');
  void set(const vector<string> &vs,char category='d');
  void setStr(string str,char category='d');
  string str();
  string s;
  double d;
  int i;
  vector<double> vd;
  vector<string> vs;
  char type;  //'s' string; 'd' double, 'i' integer, 'v' vector<double>, 'w' vector<string>
  char category; //'d' default, 't' temporary' ,'r' results,'s' Programm_settings, 'l' layout
};


//!container for various central data
/*!The idea of this class is to provide a flexible container for data
There are currently 2 containers used in the program one for the central settings which do not change from dataset to dataset and one for the data exracted from the pictures.
The data in the container is identified by its name. Data should be stored via the store function. It can be retrieved via the [] operator as in any other std::map. This operator returns an 'm_entry' object 
Example: 
data.store("myname",d);
double e = data["myname"].d

For storage and loading of the data the functions toFile and FromFile arprovided.
an optional style tag can be used to define the file format.
in case one wants different File Formats for the data one just has to modify these functions. 

 */


class Dataset :public  std::map<std::string,m_entry> 
{
public:
    Dataset();
    void fromFile(const char* filename);
    void toFile(const char*,std::string style ="");
    void display();
    void store(string name, double d,char category='u');
    void store(string name, int  i,char category='u');
    void store(string name, string s,char category='u');
    void store(string name, vector<double> vd,char category='u');
    void store(string name, vector<string> vs,char category='u');
    void storeStr(string name, string data);
    void join(Dataset* j);  //<adds all entries of the other dataset to this one
    void setCategory(char c){basecat = c;}; //< base category of this list.  All entries changed will get this cat.
    m_entry& get(string name){return (*this)[name];}
    template<typename T>  m_entry& get(string name, T d,char category='u');
    bool exist(string name){return !(find(name)==end());} ///<check whether an entry exists
private:
    char basecat;
};

//! get a value for the tag name. If it doesn't exist create with default value d

template<class T> inline
m_entry& Dataset::get(string name, T d)
{
    if(!exist(name)) store(name,d);
    return (*this)[name];
}


///! Dataset constants:





#endif
