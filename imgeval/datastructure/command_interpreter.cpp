#include"command_interpreter.h"

command_Interpreter::command_Interpreter(map<string,imgdata>* imgdat,
		      map<string,curvedata>* curvedat,
		      map<string,float>* floatdat)
{
  imgmap=imgdat;
  curvmap= curvdat;
  floatmap=floadat;
}


#define I1 (*imgmap)[arglist[1]]
#define I2 (*imgmap)[arglist[2]]
#define I3 (*imgmap)[arglist[3]]
#define I4 (*imgmap)[arglist[4]]
#define F1 (*floatmap)[arglist[1]]
#define F2 (*floatmap)[arglist[2]]
#define F3 (*floatmap)[arglist[3]]
#define F4 (*floatmap)[arglist[4]]
#define C1 (*curvmap)[arglist[1]]
#define C2 (*curvmap)[arglist[2]]
#define C3 (*curvmap)[arglist[3]]
#define C4 (*curvmap)[arglist[4]]
#define CHECK2 if(arglist.size()!= 2) break;
/*!


a command looks like: iicommand(argument1,argument2)
the letters in front of the command stand for the type of arguments
i image
c curve
f float
 */
command_Interpreter::execute(vector<string> commandlist)
{
  vector<string> arglist;
  for(int i = 0;i<commandlist.size();i++){
    arglist.clear();
    string s=commandlist.
      string command= s.substr(0,s.find("("));  //take first part
    s= s.substr(s.find("(")+1);
    while (s.find(",")){
      arglist.push_back( s= s.substr(0,s.find(",")+1))
	s= s.substr(s.find(",")+1);
    }
    if(command == "test"){
    }else if(command =="iiadd"){CHECK2 I1+=I2;
    }else if(command =="iisub"){CHECK2 I1-=I2;
    }else if(command =="iimul"){CHECK2 I1*=I2;
    }else if(command =="iidiv"){CHECK2 I1/=I2;
    }else if(command =="ifadd"){CHECK2 I1+=F2;
    }else if(command =="ifsub"){CHECK2 I1-=F2;
    }else if(command =="ifmul"){CHECK2 I1*=F2;
    }else if(command =="ifdiv"){CHECK2 I1/=F2;
    }else if(command ==""){ 
    }else if(command ==""){ 
    }else if(command ==""){ 
    }else if(command ==""){ 
    }else {}; 

  }


}
