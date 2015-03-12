#ifndef LIBGDL_CORE_UTIL_TRACE_HANDLER_HPP_INCLUDED
#define LIBGDL_CORE_UTIL_TRACE_HANDLER_HPP_INCLUDED

#include <list>
#include <string>

#include "log.hpp"
#include "logid.hpp"

namespace libgdl
{
namespace util
{

class TraceHandler : public std::list<std::string>
{
 public:
  TraceHandler(Log log = GLOBAL_LOG)
    : log(log), isInitialized(false) {}

  bool LoadTrace(const std::string& filename);

  bool IsInitialized() const
  {
    return isInitialized;
  }

 private:
  mutable Log log;

  bool isInitialized;
};

}
}


#endif // LIBGDL_CORE_UTIL_TRACE_HANDLER_HPP_INCLUDED
