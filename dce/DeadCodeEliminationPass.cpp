#include <cstddef>
#include <iostream>
#include <fstream>
#include <json/json.h>
#include <json/value.h>
#include <json/writer.h>
#include <memory>
#include <unordered_set>

std::unordered_set<std::string> used;

void unuseddcepass(const Json::Value& instrs)
{
  /* DCE pass to remove any code which is not used in the program */
  Json::Value dce_instrs(Json::arrayValue);
  for(int i=0; i<instrs.size(); i++)
  {
    /* first pass */
    if(instrs[i]["args"] != Json::nullValue)
    {
      /* add any literals which have been called in a function */
      for(int j=0; j<instrs[i]["args"].size(); j++)
        used.insert(instrs[i]["args"][j].asString());
    }
  }



  for (int i=0; i<instrs.size(); i++)
  {
    /* second pass */
    if(used.find(instrs[i]["dest"].asString()) != used.end())
    {
    dce_instrs.append(instrs[i]);
    }
    else {
      std::cout<<"removed element" << std::endl;
    }
  }

  std::ofstream dcefile;
  dcefile.open("generated-dce.json");

  Json::StyledWriter styledwriter;
  dcefile << styledwriter.write(dce_instrs);

  dcefile.close();

}

void localdcepass(const Json::Value& instrs)
{
    unuseddcepass(instrs);
}


int main()
{
    Json::Value read_instrs;
    std::cin >> read_instrs;
    if(read_instrs==Json::nullValue)
    {
      std::cout << "No input piped";
      return 1;
    }
     
    // If you like the defaults, you can insert directly into a stream.
    const Json::Value& functionlist =  read_instrs["functions"];

    for (int i = 0; i<functionlist.size(); i++)
    {
      const Json::Value& instrs = functionlist[i]["instrs"];
      localdcepass(instrs);
    }
     
    // If desired, remember to add a linefeed and flush.
    std::cout << std::endl;
    return 0;
  }
