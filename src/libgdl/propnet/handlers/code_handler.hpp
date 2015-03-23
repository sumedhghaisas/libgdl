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
              const std::string& helper_sig,
              const std::string& helper_call,
              size_t limit = 10000)
    : r_type(r_type),
    name(name),
    sig(sig),
    helper_sig(helper_sig),
    helper_call(helper_call),
    stream(NULL),
    helper(NULL),
    limit(limit),
    toDelStream(true){}

  CodeHandler(const std::string& r_type,
              const std::string& name,
              const std::string& sig,
              const std::string& helper_sig,
              const std::string& helper_call,
              std::ostream& stream,
              size_t limit = 10000)
    : r_type(r_type),
    name(name),
    sig(sig),
    helper_sig(helper_sig),
    helper_call(helper_call),
    stream(&stream),
    helper(NULL),
    limit(limit),
    toDelStream(false){}

  ~CodeHandler()
  {
    delete helper;
    if(toDelStream)
      delete stream;
  }

  void AddEntry(const std::string& str)
  {
    s_entries.push_back(str);
  }

  void GenerateCode(FileHandler& fh = FileHandler::GetMasterFileHandler());

  std::string r_type;
  std::string name;
  std::string sig;
  std::string helper_sig;
  std::string helper_call;

  std::stringstream init_ss;
  std::stringstream fun_init_ss;
  std::stringstream fun_deinit_ss;

  std::list<std::string> s_entries;

  std::ostream* stream;

  CodeHandler* helper;
  size_t limit;

  bool toDelStream;
};

}
}


#endif // LIBGDL_PROPNET_CODE_HANDLER_HPP_INCLUDED
