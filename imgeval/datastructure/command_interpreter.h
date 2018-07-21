#ifndef COMMAND_INTERPRETER
#define COMMAND_INTERPRETER
#include<map>
#include<vector>
class imgdata;    //2d images 
using namespace std;
typedef curvedata vector<float>
//class curvedata;  //class that contains 1d curves


/*! operates on maps of 2d,1d and 0d information. 
Creates and transforms it according to the commands given

the commands are given as a list of strings. 
Some commands consist of multiple strings depemding on the number of arguments
 */
class command_Interpreter
{
 public:
  command_Interpreter(map<string,imgdata>* imgdat = NULL,
		      map<string,curvedata>* curvedat = NULL,
		      map<string,float>* floatdat = NULL);
  execute(vector<string> commandlist); //executes the commands given in the list
 private:
  map<string,imgdata>* imgmap;
  map<string,curvedata>* curvmap;
  map<string,float>* floatmap;
};

#endif
