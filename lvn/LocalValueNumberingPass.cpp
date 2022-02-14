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
std::vector<std::string> opcomm = {"mul", "add"};

class Table
{
  private:
    std::vector<std::string> dest_var;
    std::vector<std::string> value_op;
    std::vector<std::vector<std::string>> value_args;
    std::vector<std::string> cann_var;
    int tableentries=0;
  public:
    void createentry(std::string new_var, std::string new_value_op, std::vector<std::string> new_value_args)
    {
      dest_var[tableentries] = new_var;
      if (std::find(cann_var.begin(), cann_var.end(), new_var) != cann_var.end())
      {
        std::cout << "Found duplicate";
      }

      value_op[tableentries] = new_value_op;
      tableentries++;
    }
}; 

int createtable()
{


}

void lvnpass()
{
  Table table;
  for (auto instr: dce_instrs)
  {
    std::vector<std::string> new_value_args;
    for (auto iter: instr["args"])
      new_value_args.push_back(iter.asString());
    if(std::find(opcomm.begin(), opcomm.end(), instr["op"].asString()) != opcomm.end())
    {
      std::sort (new_value_args.begin(), new_value_args.end());
    }
    table.createentry(instr["dest"].asString(), instr["op"].asString(), new_value_args);
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
  lvnpass();
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
