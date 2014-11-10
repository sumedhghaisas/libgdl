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
namespace core
{

/**
 * Represents stream which can be used to decode the objects which are encoded
 * by SymbolTable. SymbolDecodeStream binds the given pointer to SymbolTable
 * object to the stream. The class should implement 'DecodeToString' which
 * accepts 'const SymbolTable&' as a parameter and return string. If the class
 * implements such a function them it will be called by this stream, else
 * operator<< overload will be called. If even operator<< does not exist for the
 * given class then compiler error will be thrown. For example look at 'Argument'
 * structure and implemented 'DecodeToString' function.
 *
 * Sample usage -
 * @code
 * SymbolTable sym;
 * Argument arg("(test x)", sym);
 * SymbolDecodeStream stream(&sym, std::cout);
 * stream << arg << std::endl;
 * @endcode
 *
 * This will print "(test x)" on standard output.
 *
 * @see SymbolTable, Argument
 *
 */
class SymbolDecodeStream
{
  //! Signature of 'DecodeToString' function
  template<typename T>
  using Sig_DecodeToString =
                      std::string (T::*)(const SymbolTable& symbol_table) const;

  HAS_MEM_FUNC(DecodeToString, Has_DecodeToString);

  //! typedef to be used by boost::enable_if and disable_if
  template<typename T>
  using Has_sig = Has_DecodeToString<T, Sig_DecodeToString<T> >;

 public:
  //! Empty constructor
  //!
  //! \param symbol_table Pointer to SymbolTable to bind
  //! \param stream Output stream
  //!
  //!
  SymbolDecodeStream(const SymbolTable symbol_table,
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
  //! Write const char[] to the stream
  SymbolDecodeStream& operator<<(const char[]);
  //! Write number to the stream (sometimes gcc produces substitution failure)
  SymbolDecodeStream& operator<<(const size_t& num)
  {
    stream << num;
    return *this;
  }

  //! Write anything else to the streamt("(cell 2 3 4)",

  template<typename T>
  SymbolDecodeStream& operator<<(const T& t)
  {
    BaseLogic(t);
    return *this;
  }

 private:

  //! Basic logic behind forwarding
  //! This function is enabled only when the type implements DecodeToString
  //! function with correct signature.
  //!
  //! \tparam T Type of the object to print
  //! \param t Object to print
  //!
  //!
  template<typename T> void
  BaseLogic(const T& t,
            typename boost::enable_if<Has_sig<T>>::type* = 0)
  {
    stream << t.DecodeToString(symbol_table);
  }

  //! Basic logic behind forwarding
  //! This function is enabled only when the type DOES NOT implements
  //! DecodeToString function.
  //!
  //! \tparam T Type of the object to print
  //! \param t Object to print
  //!
  //!
  template<typename T> void
  BaseLogic(const T& t,
            typename boost::disable_if<Has_sig<T>>::type* = 0)
  {
    stream << t;
  }

  //! Pointer to SymbolTable
  const SymbolTable symbol_table;
  //! Stream to output
  util::PrefixedOutStream stream;
}; // class SymbolDecodeStream

}; // namespace core
}; // namespace libgdl

#endif // _LIBGDL_GDLPARSER_PARSER_SYMBOL_DECODE_STREAM_HPP_INCLUDED
