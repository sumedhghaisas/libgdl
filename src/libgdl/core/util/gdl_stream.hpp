#ifndef _LIBGDL_CORE_UTIL_GDL_STREAM_HPP_INCLUDED
#define _LIBGDL_CORE_UTIL_GDL_STREAM_HPP_INCLUDED

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

namespace libgdl
{
namespace util
{

class GDLStream
{
 public:
   GDLStream(const std::string& name, std::ifstream* stream)
     : name(name), stream(stream) {}

   const std::string& Name() const { return name; }
   std::string& Name() { return name; }

   std::istream* Stream() { return stream; }

 private:
  std::string name;
  std::istream* stream;
}; // class GDLStream

}; // namespace util
}; // namespace libgdl


#endif // _LIBGDL_CORE_UTIL_GDL_STREAM_HPP_INCLUDED
