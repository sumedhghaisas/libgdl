/**
 * @file prefixedoutstream.cpp
 * @author Ryan Curtin
 * @author Matthew Amidon
 * @author Sumedh Ghaisas
 *
 * Implementation of PrefixedOutStream methods.
 */
#include <string>
#include <iostream>
#include <streambuf>
#include <string.h>
#include <stdlib.h>

#include "prefixedoutstream.hpp"

using namespace libgdl::util;

PrefixedOutStream& PrefixedOutStream::operator<<(std::streambuf* sb)
{
  BaseLogic<std::streambuf*>(sb);
  return *this;
}

PrefixedOutStream& PrefixedOutStream::operator<<(
    std::ostream& (*pf)(std::ostream&))
{
  BaseLogic<std::ostream& (*)(std::ostream&)>(pf);
  return *this;
}

PrefixedOutStream& PrefixedOutStream::operator<<(std::ios& (*pf)(std::ios&))
{
  BaseLogic<std::ios& (*)(std::ios&)>(pf);
  return *this;
}

PrefixedOutStream& PrefixedOutStream::operator<<(
    std::ios_base& (*pf) (std::ios_base&))
{
  BaseLogic<std::ios_base& (*)(std::ios_base&)>(pf);
  return *this;
}
