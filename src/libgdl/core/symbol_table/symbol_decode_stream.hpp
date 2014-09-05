/**
 * @file symbol_decode_stream.hpp
 * @author Sumedh Ghaisas
 *
 * Declaration of SymbolDecodeStream
 */
#ifndef _LIBGDL_GDLPARSER_PARSER_SYMBOL_DECODE_STREAM_HPP_INCLUDED
#define _LIBGDL_GDLPARSER_PARSER_SYMBOL_DECODE_STREAM_HPP_INCLUDED

#include <iostream>
#include <iomanip>
#include <string>
#include <streambuf>
#include <string>

#include <boost/lexical_cast.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/type_traits.hpp>

#include <libgdl/core/util/sfinae_utility.hpp>
#include <libgdl/core/util/prefixedoutstream.hpp>

#include "symbol_table.hpp"

namespace libgdl
{

class SymbolDecodeStream
{
  template<typename T>
  using Sig_DecodeToString = std::string (T::*)(const SymbolTable& symbol_table) const;

  HAS_MEM_FUNC(DecodeToString, Has_DecodeToString);
 public:
  SymbolDecodeStream(const SymbolTable* symbol_table,
                     const util::PrefixedOutStream& stream = std::cout)
    : symbol_table(symbol_table), stream(stream) {}

  //! Write a streambuf to the stream.
  SymbolDecodeStream& operator<<(std::streambuf* sb);
  //! Write an ostream manipulator function to the stream.
  SymbolDecodeStream& operator<<(std::ostream& (*pf)(std::ostream&));
  //! Write an ios manipulator function to the stream.
  SymbolDecodeStream& operator<<(std::ios& (*pf)(std::ios&));
  //! Write an ios_base manipulator function to the stream.
  SymbolDecodeStream& operator<<(std::ios_base& (*pf)(std::ios_base&));
  SymbolDecodeStream& operator<<(const char[]);

  template<typename T>
  SymbolDecodeStream& operator<<(const T& t)
  {
    BaseLogic(t);
    return *this;
  }

 private:
  template<typename T> void
  BaseLogic(const T& t,
            typename boost::enable_if<Has_DecodeToString<T, Sig_DecodeToString<T> > >::type* = 0)
  {
    stream << t.DecodeToString(*symbol_table);
  }

  template<typename T> void
  BaseLogic(const T& t,
            typename boost::disable_if<Has_DecodeToString<T, Sig_DecodeToString<T> > >::type* = 0)
  {
    stream << t;
  }

  const SymbolTable* symbol_table;
  util::PrefixedOutStream stream;
};

}


#endif // _LIBGDL_GDLPARSER_PARSER_SYMBOL_DECODE_STREAM_HPP_INCLUDED
