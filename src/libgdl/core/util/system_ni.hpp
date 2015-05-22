#ifndef LIBGDL_CORE_UTIL_SYSTEM_NI_HPP_INCLUDED
#define LIBGDL_CORE_UTIL_SYSTEM_NI_HPP_INCLUDED

#include <iostream>
#include <string>

namespace libgdl
{
namespace util
{

struct SystemNI
{
  static std::string exec(const std::string& cmd)
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
};

}
}


#endif // LIBGDL_CORE_UTIL_SYSTEM_NI_HPP_INCLUDED
