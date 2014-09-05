#include "symbol_decode_stream.hpp"

#include <string>
#include <iostream>
#include <streambuf>
#include <string.h>
#include <stdlib.h>

using namespace std;
using namespace libgdl;

SymbolDecodeStream& SymbolDecodeStream::operator<<(std::streambuf* sb)
{
  stream << sb;
  return *this;
}

SymbolDecodeStream& SymbolDecodeStream::operator<<(
    std::ostream& (*pf)(std::ostream&))
{
  stream << pf;
  return *this;
}

SymbolDecodeStream& SymbolDecodeStream::operator<<(std::ios& (*pf)(std::ios&))
{
  stream << pf;
  return *this;
}

SymbolDecodeStream& SymbolDecodeStream::operator<<(
    std::ios_base& (*pf) (std::ios_base&))
{
  stream << pf;
  return *this;
}

SymbolDecodeStream& SymbolDecodeStream::operator<<(const char a[])
{
  stream << a;
  return *this;
}
