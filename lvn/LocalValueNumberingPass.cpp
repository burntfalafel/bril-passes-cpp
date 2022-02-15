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

Json::Value bril_instrs(Json::arrayValue);
std::vector<std::string> opcomm = {"mul", "add"};

class Table
{
  private:
    std::map<int, std::vector<std::string>> dest_map;
    std::map<int, std::string> value_op;
    std::map<int, std::vector<std::string>> value_args;
    std::map<int, std::string> cann_var;
    int row=0;
  public:
    void createentry(std::string new_var, std::string new_value_op, std::vector<std::string> new_value_args)
    {
      dest_map[row].push_back(new_var);
      if ((std::find(value_op.begin(), value_op.end(), new_value_op) != value_op.end()) && (std::search(value_args[row].begin(),
              value_args[row].end(), new_value_args.begin(), new_value_args.end()) != value_args[row].end()))
      {
        /* other columns in table do not need update as it is a duplicate */
        return;
      }
      value_op[row]  = new_value_op;
      value_args[row] = new_value_args;
      cann_var[row] = new_value_op; /* assign cannonincal variable the same as referred */
      row++; /* next row */
    }
    void afterlvnir()
    {
      int destrow;
      for (int iterrow=0; iterrow<row; iterrow++)
      {
        for (auto arglist: value_args[iterrow])
          for (auto singlearg: arglist)
          {
            for (destrow=0; destrow<row; destrow++)
              if(std::find(dest_map[destrow].begin(), dest_map[destrow].end(), singlearg) != dest_map[destrow].end())
                break;
            //std::replace(dest_map[iterrow].begin(), dest_map[iterrow].end(), singlearg, std::to_string(destrow) );
            for(auto iter: dest_map[iterrow])
              if(iter.compare(singlearg))
                iter = destrow;
          }
      }
    }

}; 


void lvnpass()
{
  Table table;
  for (auto instr: bril_instrs)
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
  table.afterlvnir();
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
