#include <cstddef>
#include <iostream>
#include <json/json.h>
#include <json/value.h>
#include <memory>

void printins(unsigned int tabcount, const Json::Value& instrs)
{
  for(int i=0; i<instrs.size(); i++)
  {
    if(instrs[i]["value"] != Json::nullValue)
    {
      /* Literal assignment */
      /* insert indent for function ins */
      for (int tc=0; tc<tabcount; tc++)
        std::cout<<"\t";
      std::cout<<instrs[i]["dest"] << ": " << instrs[i]["value"] << std::endl;
    }
    else if(instrs[i]["funcs"] != Json::nullValue)
    {
     for(int j=0; j<instrs[i]["args"].size(); j++)
      { 
        /* Custom function call */
        /* insert indent for function ins */
        for (int tc=0; tc<tabcount; tc++)
          std::cout<<"\t";
        std::cout<<instrs[i]["args"][j];
        std::cout<<"-> "<<instrs[i]["funcs"][0]; /* 0 harcoded since there can only be one function call */
        std::cout<<std::endl;
      }
    }
    else if (instrs[1]["args"] != Json::nullValue)
    {
      /* Normal function call */
      /* insert indent for function ins */
      for (int tc=0; tc<tabcount; tc++)
        std::cout<<"\t";
      std::cout<<instrs[i]["dest"] << "<- "<<instrs[i]["op"]<<"(";
      for(int j=0; j<instrs[i]["args"].size(); j++)
        std::cout<<instrs[i]["args"][j];
      std::cout<<")"<<std::endl;
    }
    else
    {
      /* anything else, just print */
      /* insert indent for function ins */
      for (int tc=0; tc<tabcount; tc++)
        std::cout<<"\t";
      std::cout<<instrs[i]["name"] << std::endl;
    }
  }
}

void printfunction(const Json::Value& name, const Json::Value& args, const Json::Value& instrs)
{
    std::cout<<"diagraph "<<name<<" {"<<std::endl;
    printins(1, instrs);
    std::cout<<"}"<<std::endl;
}


int main()
  {
    Json::Value root;
    std::cin >> root;
    if(root==Json::nullValue)
    {
      std::cout << "No input piped";
      return 1;
    }
     
    // If you like the defaults, you can insert directly into a stream.
    const Json::Value& functionlist =  root["functions"];

    for (int i = 0; i<functionlist.size(); i++)
    {
      const Json::Value& funcargs = functionlist[i]["args"];
      const Json::Value& instrs = functionlist[i]["instrs"];
      const Json::Value& name = functionlist[i]["name"];
      /* discard type info */
      // const Json::Value& type = functionlist[i]["type"];
      printfunction(name, funcargs, instrs);
    }
     
    // If desired, remember to add a linefeed and flush.
    std::cout << std::endl;
    return 0;
  }
