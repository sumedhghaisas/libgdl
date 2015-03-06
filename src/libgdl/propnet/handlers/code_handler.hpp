#ifndef LIBGDL_PROPNET_CODE_HANDLER_HPP_INCLUDED
#define LIBGDL_PROPNET_CODE_HANDLER_HPP_INCLUDED

#include <list>
#include <string>
#include <sstream>
#include <fstream>

#include "file_handler.hpp"

namespace libgdl
{
namespace propnet
{

struct CodeHandler
{
  CodeHandler(const std::string& r_type,
              const std::string& name,
              const std::string& sig,
              size_t limit = 15000)
    : r_type(r_type), name(name), limit(limit) {}

  void AddEntry(const std::string& str)
  {
    s_entries.push_back(str);
  }

  void GenerateCode();

  std::string r_type;
  std::string name;
  std::string sig;

  std::stringstream init_ss;
  std::stringstream fun_init_ss;
  std::stringstream fun_deinit_ss;

  std::list<std::string> s_entries;

  size_t c_helper;
  size_t limit;
};

}
}


#endif // LIBGDL_PROPNET_CODE_HANDLER_HPP_INCLUDED
