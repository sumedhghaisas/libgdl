/**
 * @file setop.hpp
 * @author Sumedh Ghaisas
 *
 * Declaration of set opertion functions.
 */
#ifndef _GDLPARSER_UTIL_SETOP_HPP_INCLUDED
#define _GDLPARSER_UTIL_SETOP_HPP_INCLUDED

#include <set>
#include <string>

namespace gdlparser
{
namespace util
{

/**
 * This class implements the useful set operations performed on set of strings.
 */

class setop
{
    //! some useful typedefs
    typedef std::set<std::string> StringSet;
public:

    //! Returns the intersection of set a and b
    //!
    //! \param a const StringSet&
    //! \param b const StringSet&
    //! \return StringSet
    //!
    //!
    static StringSet setIntersection(const StringSet & a, const StringSet & b);

    //! Returns the union of set a and b
    //!
    //! \param a const StringSet&
    //! \param b const StringSet&
    //! \return StringSet
    //!
    //!
    static StringSet setUnion(const StringSet & a, const StringSet & b);

    //! checks whether a is a subset of b
    //!
    //! \param a const StringSet&
    //! \param b const StringSet&
    //! \return bool
    //!
    //!
    static bool isSubset(const StringSet & a, const StringSet & b);

    //! Returns set difference
    //!
    //! \param a const StringSet&
    //! \param b const StringSet&
    //! \return StringSet
    //!
    //!
    static StringSet setDifference(const StringSet& a, const StringSet& b);
};

}
}

#endif // _GDLPARSER_UTIL_SETOP_HPP_INCLUDED
