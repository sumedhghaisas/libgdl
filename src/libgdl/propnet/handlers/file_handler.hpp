#ifndef LIBGDL_PROPNET_FILE_HANDLER_HPP_INCLUDED
#define LIBGDL_PROPNET_FILE_HANDLER_HPP_INCLUDED

#include <string>
#include <list>

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

  std::list<std::string> files;
};

}
}


#endif // LIBGDL_PROPNET_FILE_HANDLER_HPP_INCLUDED
