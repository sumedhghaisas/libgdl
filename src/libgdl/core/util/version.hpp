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
#define _LIBGDL_VERSION_MAJOR_ "0"
#define _LIBGDL_VERSION_MINOR_ "0"
#define _LIBGDL_VERSION_PATCH_ "0"
#define _LIBGDL_VERSION_NAME_ "master"
#define _LIBGDL_VERSION_QUOTE_ \
"A human being(or intelligent robot) should be able to change a diaper,\n\
plan an invasion, butcher a hog, conn a ship, design a building,\n\
write a sonnet, balance accounts, build a wall, set a bone, \n\
comfort the dying, take orders, give orders, cooperate, act alone,\n\
solve equations, analyze a new problem, pitch manure, program a computer,\n\
cook a tasty meal, fight efficiently, die gallantly.\n\
Specialization is for insects.\n\
                          - Robert Heinlein"

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
      << "." << _LIBGDL_VERSION_PATCH_ << std::endl;
  o << _LIBGDL_VERSION_QUOTE_ << std::endl;
  return o.str();
}

}; // namespace util
}; // namespace libgdl

#endif // _LIBGDL_CORE_UTIL_VERSION_HPP_INCLUDED
