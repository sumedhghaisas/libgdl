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
  FileHandler(bool optimize_for_time = false)
    : optimize_for_time(optimize_for_time), is_primary(true) {}

  static FileHandler& GetMasterFileHandler()
  {
    static FileHandler singleton;
    return singleton;
  }

  void AddFile(const std::string& file)
  {
    if(is_primary)
      p_files.push_back(file);
    else
      s_files.push_back(file);
  }

  static std::string exec(const std::string& cmd);

  bool GenerateSharedObject(const std::string& shared_object = "state_machine/state_machine.so");

  std::list<std::string> p_files;

  std::list<std::string> s_files;

  bool optimize_for_time;
  bool is_primary;
};

}
}


#endif // LIBGDL_PROPNET_FILE_HANDLER_HPP_INCLUDED
