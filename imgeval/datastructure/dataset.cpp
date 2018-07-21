#include "dataset.h"
#include <sstream>
#include <fstream>
#include<iomanip>
using namespace std;

/*! ********************************************************************
\brief standard constructor
\author Markus Debatin
\date 09-2010
*************************************************************************/
Dataset::Dataset()
:std::map<std::string,m_entry>()
{
  basecat = 'd';

} 

/*! ********************************************************************
\brief creates a map with translations of the categories
\author Markus Debatin
\date 09-2010
*************************************************************************/
const map<char,string> create_categories()
    {
        map<char,string> cat;
        cat['d'] = "temporary";
        cat['r'] = "results";
        cat['s'] = "program_Settings";
        cat['p'] = "physical";
        return cat;
}


//! reads data from a file
/*! ********************************************************************
currently only one format is supported for reading data from file
The format is the native saving format. 
Tag type data.
types are
d double
i int
s string
v vector<double>
w vector<string>
if data is a vector it starts with the number of entries in the vector followed by tab separated values:
"jim" s "some string till end of line"
"joe" d 1212.2e-10
"jhon" v 
"johanna" w  2 first name
seconde name
"jelka" d 23
\param filename file to be read
\author Markus Debatin
\date 09-2010
*************************************************************************/
void Dataset::fromFile(const char* filename)
{
  string tag,s;
  char type;
  double d;
  int i;
  //bool b;
  ifstream file(filename);
  file>>skipws>>tag>>type;
  while(file.good()){
    switch (type){
    case 's':
      file.ignore(1);
      getline(file,s);
      store(tag,s);
      //cout<<"Dataset: "<<s<<endl;
      break;
    case 'd':
      file>>d;
      if(tag == "Rbdetuning")
          cout<<"Dataset: "<<tag<<" "<<d<<"\n";
      store(tag,d);
      break;
    case 'i':
      file>>i;
      store(tag,i);
      break;
    case 'v':{
      int size;
      file>>size;
      vector<double> v;
      for(int i=0;i<size;i++){
	file>>d; v.push_back(d);
      }
      store(tag,v);
    }
      break;
    case 'w':{  //vector of strings
      int size;
      file>>size;
      vector<string> w;
      getline(file,s);
       for(int i=0;i<size;i++){
//            file.ignore(1);
            getline(file,s);
            w.push_back(s);
        }
       store(tag,w);
        }
    break;
    }
    //cout<<"Dataset: "<<tag<<endl;
    file>>skipws>>tag>>type;

  }
  file.close();
}

//! writes dataset to file
/*! stores the data in dataset into a file
currently two file formats are supported. One format where there is one line per entry consisting of the tag or name, the types of the data and the data. 
On saving the old file is replaced. 
The other style called "Gost" is similar to the way the experimental data is stored in the Gost experiment.
There all the data is stored to one single line in a tab separated format of alternating tag data tag data .....
The new data is appended to an existing file
\param filename file to be written
\param style "Gost" or anything else for Gost all params are stored in a single line
for the standard the fileformat is as for the read in
\author Markus Debatin
\date 09-2010
*************************************************************************/
void Dataset::toFile(const char* filename,std::string style)
{
  cout<<style<<";\n";
  if(style=="Gost"){
    ofstream file(filename, ios_base::app);
    //file<<style;
    file<<setprecision(7);
    file<<"scan"<<"\t";
    file<<"Messnr"<<"\t"<<get("Messnr").s<<"\t";
    for( map<string, m_entry>::iterator iter = this->begin(); iter != this->end(); ++iter ) {
      if((*iter).second.type=='d')
        file << (*iter).first << "\t" << setprecision(9)<<(*iter).second.d << "\t";
      if((*iter).second.type=='i')
        file << (*iter).first << "\t" << (*iter).second.i << "\t";
    }
    file<<endl;
    file.close();
    return;
  }

  //else style=""

  ofstream file(filename, ios_base::trunc);
  file<<setprecision(7);
  //file<<style; //debug only
  for( map<string, m_entry>::iterator iter = this->begin(); iter != this->end(); ++iter ) {
    if( (*iter).second.category != 'r'){
      file << (*iter).first << "\t" << (*iter).second.type << "\t";

      switch((*iter).second.type){
      case 's':
        file<<(*iter).second.s<<endl;
        break;
      case 'd':
        if ((*iter).second.d == 1.0/0) {file<<0<<endl;}
        else if ((*iter).second.d == - 1.0/0) {file<<0<<endl;}
        else
        file<<(*iter).second.d<<endl;
        break;
      case 'i':
        file<<(*iter).second.i<<endl;
        break;
      case'v':
        file<<(*iter).second.vd.size();
        for(uint i=0;i<(*iter).second.vd.size();i++)
          file<<"\t"<<(*iter).second.vd[i];
        file<<endl;
        break;
      case'w':
        file<<(*iter).second.vs.size()<<endl;
        for(uint i=0;i<(*iter).second.vs.size();i++)
          file<<(*iter).second.vs[i]<<endl;
        break;
     case 'u':
        file<<endl;
        break;
      }
    }
  }
  file.close();
}


//! not working
void Dataset::display()
{
  for( map<string, m_entry>::iterator iter = this->begin();
  iter != this->end(); ++iter ){
}
}

//! store double values to the dataset
/*! ********************************************************************
\param s tagname
\param d value
\param category letter indicating the category \see create_categories())
\author Markus Debatin
\date 09-2010
*************************************************************************/
void Dataset::store(string tag, double d)
{
 (*this)[tag].set(d,basecat);
}
void Dataset::store(string tag, int i)
{
 (*this)[tag].set(i,basecat);
}

//! store string values to dataset
void Dataset::store(string tag, string sd)
{
 (*this)[tag].set(sd,basecat);
}
//! store vectors of double to dataset
void Dataset::store(string s, vector<double> vd)
{
 (*this)[s].set(vd,basecat);
}
//! store vectors of strings to dataset
void Dataset::store(string s, vector<string> vs)
{
 (*this)[s].set(vs,basecat);
}

/*! ********************************************************************
\brief
\author Markus Debatin
\date 09-2010
*************************************************************************/
void Dataset::storeStr(string s, string data){

(*this)[s].setStr(data,basecat);

}

/*! ********************************************************************
\brief adds the data in the dataset to this one
\param j data to be added to this dataset
\author Markus Debatin
\date 09-2010
*************************************************************************/
void Dataset::join(Dataset* j)
{
    if (j==NULL) return;
    for( map<string, m_entry>::iterator iter = j->begin(); iter != j->end(); ++iter ) {
            (*this)[(*iter).first]=(*iter).second;
        }
}


//! constructor of entry class sets s='' and d=0
/*! ********************************************************************
\brief
\author Markus Debatin
\date 09-2010
*************************************************************************/
m_entry::m_entry()
{
  s=""; d=0; i=0; type='u';

}
/*! ********************************************************************
\brief sets the stringvalue of this entry
\param s value to be set
\param c categorie
\author Markus Debatin
\date 09-2010
*************************************************************************/
void m_entry::set(const string& s,char c)
{ 
  this->s=s;
  type='s';
  category = c;
}
/*! ********************************************************************
\brief
\author Markus Debatin
\date 09-2010
*************************************************************************/
void m_entry::set(const double& d,char c)
{
  this->d=d;
  i=(int)d;
  type='d';
  category = c;
}
/*! ********************************************************************
\brief
\author Markus Debatin
\date 09-2010
*************************************************************************/
void m_entry::set(const int& i,char c)
{
  this->i=i;
  d=i;
  type='i';
  category = c;
}
/*! ********************************************************************
\brief
\author Markus Debatin
\date 09-2010
*************************************************************************/
void m_entry::set(const vector<double>& vd,char c)
{
  this->vd=vd;
  type='v';
  category = c;
}
/*! ********************************************************************
\brief
\author Markus Debatin
\date 09-2010
*************************************************************************/
void m_entry::set(const vector<string>& vs,char c)
{
  this->vs=vs;
  type='w';
  category = c;
}

/* converts the string to the datatype stored perviously in this entry and stores it
\brief
\author Markus Debatin
\date 09-2010
*************************************************************************/
void m_entry::setStr(string str,char c)
{
    double d;
    int i;

    istringstream iss;
    iss.str(str);
    switch (type){
    case 's':
      this->s=str;
        break;
    case 'd':
      iss>>d;
      this->d=d;
      break;
    case 'i':
      iss>>i;
      this->i=i;
      break;
    case 'v':{
      int size;
      iss>>size;
      vector<double> v;
      for(int i=0;i<size;i++){
        iss>>d; v.push_back(d);
      }
      this->vd=v;
    }
      break;
    case 'w':{  //vector of strings
      int size;
      iss>>size;
      vector<string> w;
      getline(iss,s);
       for(int i=0;i<size;i++){
//            file.ignore(1);
            getline(iss,s);
            w.push_back(s);
        }
       this->vs=w;
        }
    break;
    }
  category = c;

}

//! displays the values of the entry as string
/*! ********************************************************************
\brief returns a string corresponding to the current value
\author Markus Debatin
\date 09-2010
*************************************************************************/
string m_entry::str()
{
  ostringstream os;
  switch (type){    
  case 's': 
    return s;
  case 'd':
    os<<d;
    return os.str();
  case 'i':
    os<<i;
    return os.str();
  case 'v':
    for(uint i = 0;i<vd.size();i++)
      os<<vd[i]<<" ";
    return os.str();
  case 'w':
    for(uint i = 0;i<vs.size();i++)
      os<<vs[i]<<" ";
    return os.str();
  }
  return "";
}



