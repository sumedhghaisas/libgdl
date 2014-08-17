/**
 * @file logid.hpp
 * @author Sumedh Ghaisas
 *
 * LOGID implementation
 */
#ifndef __LIBGDL_CORE_UTIL_LOGID_HPP_INCLUDED
#define __LIBGDL_CORE_UTIL_LOGID_HPP_INCLUDED

#include <sstream>

/// Caller for format_long
#define LOGIDXL libgdl::util::Logid::format_long (__FILE__, __LINE__, __PRETTY_FUNCTION__)
/// Caller for format_short
#define LOGID libgdl::util::Logid::format_short  (__FILE__, __LINE__, __PRETTY_FUNCTION__)

namespace libgdl
{
namespace util
{

/**
 * Implements function to print file name along with line number which helps
 * in debugging.
 *
 * @code
 * std::cout << LOGID << " something went wrong here" << std::endl;
 * @endcode
 *
 */
class Logid
{
public:
    /// Does a format like
    /// /home/nos/coding/cg_beleg/TetraViewer2/bindebug/build_dir/fine_drawer.cpp:47
    /// virtual void fine_drawer::render()
    static std::string format_long (const char * filename,
                                    int line,
                                    const char * function)
    {
        std::ostringstream os;
        os << filename << ":" << line << " " << function << " ";
        return os.str ();
    }

    /// Throws away filename/line number
    /// Looks like virtual void fine_drawer::render()
    static std::string format_short (const char * filename,
                                     int line,
                                     const char * function)
    {
        (void)function; // to remove unused warning
        std::ostringstream os;
        os << filename << "(" << line << "): ";
        return os.str ();
    }

};

}; // namespace util
}; // namespace libgdl

#endif
