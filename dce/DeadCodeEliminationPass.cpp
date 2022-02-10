#include <cstddef>
#include <iostream>
#include <fstream>
#include <json/json.h>
#include <json/value.h>
#include <json/writer.h>
#include <memory>
#include <unordered_set>
#include <algorithm>
#include <vector>

Json::Value dce_instrs(Json::arrayValue);
Json::Value dce_secondpas_instrs(Json::arrayValue);

int duplicatepass()
{
  /* DCE pass to remove any duplicant definitions in the program */
  int program_status=0;
  std::vector<std::string> last_def;
  for (auto instr: dce_instrs)
  {
    /* first loop to fill last_def mapping which has defined literals */
    if(instr["dest"] != Json::nullValue)
      last_def.push_back(instr["dest"].asString());
  }
  for (auto instr: dce_instrs)
  {
    /* second loop to check for uses */
    for (auto instrargs: instr["args"])
      if(std::find(last_def.begin(), last_def.end(),instrargs.asString()) != last_def.end())
      {
        auto del_arg = std::find(last_def.begin(), last_def.end(),instrargs.asString());
        if (del_arg != last_def.end()) 
          last_def.erase(del_arg);
      }
  }
  for (auto instr: dce_instrs)
  {
    /* third loop to check for definitions and append to final instructions */
    auto del_dest_itr = std::find(last_def.begin(), last_def.end(), instr["dest"].asString());
    if( del_dest_itr == last_def.end())
    {
      dce_secondpas_instrs.append(instr);
    }
    else {
      last_def.erase(del_dest_itr);
      program_status=1;
    }
  }
  return program_status;

}

int unuseddcepass(const Json::Value& instrs)
{
  /* DCE pass to remove any code which is not used in the program */
  std::unordered_set<std::string> used;
  int program_status=0;
  for(int i=0; i<instrs.size(); i++)
  {
    /* first loop */
    if(instrs[i]["args"] != Json::nullValue)
    {
      /* add any literals which have been called in a function */
      for(int j=0; j<instrs[i]["args"].size(); j++)
        used.insert(instrs[i]["args"][j].asString());
    }
  }

  for (int i=0; i<instrs.size(); i++)
  {
    /* second loop */
    if(used.find(instrs[i]["dest"].asString()) != used.end() // if destination is being used 
       || instrs[i]["dest"]==Json::nullValue) // added for not eliminating print statements
    {
      dce_instrs.append(instrs[i]);
    }
    else {
      program_status=1;
    }
  }
  return program_status;
}

void writetofile(std::string filename)
{
  std::ofstream dcefile;
  if(!filename.empty())
    dcefile.open(filename);

  Json::StyledWriter styledwriter;
  dcefile << styledwriter.write(dce_secondpas_instrs);

  dcefile.close();
}

void localdcepass(Json::Value& instrs)
{
  int unused_status = 1, duplicate_status = 1;
  while(unused_status | duplicate_status)
  {
    dce_instrs = Json::nullValue;
    dce_secondpas_instrs = Json::nullValue;
    unused_status = unuseddcepass(instrs);
    std::cout<<"unused_status: "<<unused_status<<std::endl;
    duplicate_status = duplicatepass();
    std::cout<<"duplicate_status: "<<duplicate_status<<std::endl;
    instrs = dce_secondpas_instrs;
  }
  writetofile("generated-dce.json");
}

int main()
{
    Json::Value read_instrs(Json::arrayValue);
    std::cin >> read_instrs;
    if(read_instrs==Json::nullValue)
    {
      std::cout << "No input piped";
      return 1;
    }
     
    // If you like the defaults, you can insert directly into a stream.
    Json::Value& functionlist =  read_instrs["functions"];

    for (int i = 0; i<functionlist.size(); i++)
    {
      Json::Value& instrs = functionlist[i]["instrs"];
      localdcepass(instrs);
    }
     
    // If desired, remember to add a linefeed and flush.
    std::cout << std::endl;
    return 0;
  }
