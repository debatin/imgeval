#ifndef NTEMPL
#define NTEMPL
#include <limits>
#include<vector>
#include<fstream>
#include<iostream>
#include<sstream>
using namespace std;


#define SAVE_AFTER_EVALUATION 2
#define SAVE_ON_PROCEED 1


//just take the square
template<typename T>
T sq(const T &s){return s*s;}

//convert to string
template<typename T>
string str(const T &n){
    ostringstream os;
    os<<n;
    return os.str();
}

//smoothen a vector
template<typename T>
vector<T> smooth(const vector<T> &v,const int smooth)
{
  T sum=0;
  vector<T>v2=v;
  if(v.size()<smooth+2) return v2;

  for(int i=0;i<smooth;i++) sum+=v[i];
     for(int i = 0;i<v.size()-smooth;i++){
       v2[i+smooth/2]=sum/smooth;
       sum-=v[i]; sum+=v[i+smooth];
     }
   return v2;
}
//find maximum value of a vector
template<typename T>
T max(vector<T> v)
{
    if(v.size()==0)return 0;
    T m = v[v.size()-1];
    for(int i = v.size()-1;i>=0;i--)
        if(v[i]>m) m=v[i];
    return m;
}
//find minimum value of a vector
template<typename T>
T min(vector<T> v)
{
    if(v.size()==0)return 0;
    T m = v[v.size()-1];
    for(int i = v.size()-1;i>=0;i--)
        if(v[i]<m) m=v[i];
    return m;
}
#define DOLOG true
//#define DOLOG false
#define LOG write_logfile(string(__FILE__),__LINE__);

inline void write_logfile(std::string s,float i=0);
inline void write_logfile(std::string s,float i)
{
    if(DOLOG){
    std::ofstream of("giant_logfile.txt",std::ios::app);
    of<<s<<"; "<<i<<endl;
    of.close();
   }

}
inline void write_logfile(std::string s,int i)
{
    if(DOLOG){
    std::ofstream of("giant_logfile.txt",std::ios::app);
    of<<s<<"; "<<i<<endl;
    of.close();
   }

}


  /*
template<typename T>
inline bool isnan(T value)
{
return value != value;
}

template<typename T>
inline bool isinf(T value)
{
return std::numeric_limits<T>::has_infinity &&
value == std::numeric_limits<T>::infinity();
}
*/
#endif
