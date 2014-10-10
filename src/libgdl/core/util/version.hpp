/**
 * @file version.hpp
 * @author Sumedh Ghaisas
 *
 * libGDL version.
 */
#ifndef _LIBGDL_CORE_UTIL_VERSION_HPP_INCLUDED
#define _LIBGDL_CORE_UTIL_VERSION_HPP_INCLUDED

#include <string>
#include <sstream>

// The version of libgdl.  If this is svn trunk, this will be a version with
// higher number than the most recent release.
#define _LIBGDL_VERSION_MAJOR_ "CMAKE_LVMA"
#define _LIBGDL_VERSION_MINOR_ "CMAKE_LVMI"
#define _LIBGDL_VERSION_PATCH_ "CMAKE_LVP"
#define _LIBGDL_VERSION_NAME_ "CMAKE_LVN"
#define _LIBGDL_VERSION_QUOTE_ "CMAKE_LVQ"

// The name of the version (for use by --version).
namespace libgdl
{
namespace util
{

/**
 * This will return either "libGDL x.y.z" or "libGDL git master" depending on
 * whether or not this is a stable version of libGDL or git branch code.
 */
inline std::string GetVersion()
{
  std::stringstream o;
  o << _LIBGDL_VERSION_NAME_ << std::endl;
  o << "libGDL " << _LIBGDL_VERSION_MAJOR_ << "." << _LIBGDL_VERSION_MINOR_
      << "." << _LIBGDL_VERSION_PATCH_;
  o << _LIBGDL_VERSION_QUOTE_ << std::endl;
  return o.str();
}

}; // namespace util
}; // namespace libgdl

#endif // _LIBGDL_CORE_UTIL_VERSION_HPP_INCLUDED
