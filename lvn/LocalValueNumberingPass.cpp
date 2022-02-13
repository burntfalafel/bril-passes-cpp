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

class Table
{
  private:
    std::string dest_var;
    unsigned int num;
    std::string value;
    std::string cann_var;
}; 
int createtable()
{


}

void lvnpass()
{
  std::vector<std::string> dest_map;
  for (auto instr: dce_instrs)
  {
    dest_map.push_back(instr["dest"].asString());
  }

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

void locallvnpass(Json::Value& instrs)
{
  writetofile("generated-lvn.json");
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
     
    Json::Value& functionlist =  read_instrs["functions"];

    for (int i = 0; i<functionlist.size(); i++)
    {
      Json::Value& instrs = functionlist[i]["instrs"];
      locallvnpass(instrs);
    }
     
    return 0;
  }
