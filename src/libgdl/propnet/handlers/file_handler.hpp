#ifndef LIBGDL_PROPNET_FILE_HANDLER_HPP_INCLUDED
#define LIBGDL_PROPNET_FILE_HANDLER_HPP_INCLUDED

#include <string>
#include <list>
#include <sstream>
#include <stdio.h>
#include <iostream>

namespace libgdl
{
namespace propnet
{

struct FileHandler
{
  static FileHandler& GetMasterFileHandler()
  {
    static FileHandler singleton;
    return singleton;
  }

  void AddFile(const std::string& file)
  {
    files.push_back(file);
  }

  std::string exec(const std::string& cmd)
  {
      FILE* pipe = popen(cmd.c_str(), "r");
      if (!pipe) return "ERROR";
      char buffer[128];
      std::string result = "";
      while(!feof(pipe))
      {
        if(fgets(buffer, 128, pipe) != NULL)
          result += buffer;
      }
      pclose(pipe);
      return result;
  }

  void GenerateSharedObject()
  {
    std::string com_prefix = "g++ -O3 -std=c++11 -fPIC -Isrc";

    for(auto it : files)
    {
      std::stringstream stream;
      stream << com_prefix << " -c " << it << ".cpp -o " << it << ".o";
      exec(stream.str());
      std::cout << "Done file " << it << std::endl;
    }

    std::stringstream stream;
    stream << "g++ -O3 -shared -fPIC";

    for(auto it : files)
    {
      stream << " " << it << ".o";
    }
    stream << " -o state_machine/state_machine.so";
    exec(stream.str());
    std::cout << "StateMachine shared object created." << std::endl;
  }

  std::list<std::string> files;
};

}
}


#endif // LIBGDL_PROPNET_FILE_HANDLER_HPP_INCLUDED
