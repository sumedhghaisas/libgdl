/**
 * @file setop.cpp
 * @author Sumedh Ghaisas
 *
 * Implementation of set operation functions.
 */

#include "setop.hpp"

#include <algorithm>

using namespace gdlparser::util;

/// intersect operator
setop::StringSet setop::setIntersection (const StringSet & a, const StringSet & b)
{
    StringSet intersection;
    std::set_intersection(a.begin(), a.end(), b.begin(), b.end(),
                          std::inserter(intersection, intersection.begin()));
    return intersection;
}

/// union operator
setop::StringSet setop::setUnion (const StringSet & a, const StringSet & b)
{
    StringSet u;
    std::set_union(a.begin(), a.end(), b.begin(), b.end(),
                   std::inserter(u, u.begin()));
    return u;
}

/// checks whether a is a subset of b
bool setop::isSubset (const StringSet & a, const StringSet & b)
{
    return std::includes(b.begin(), b.end(), a.begin(), a.end());
}

/// returns set difference
setop::StringSet setop::setDifference(const StringSet& a, const StringSet& b)
{

    StringSet difference;
    std::set_difference(a.begin(),a.end(),b.begin(),b.end(),
                        std::inserter(difference,difference.begin()));
    return difference;
}
