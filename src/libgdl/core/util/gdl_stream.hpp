/**
 * @file gdl_stream.hpp
 * @author Sumedh Ghaisas
 *
 * Declaration of GDLStream.
 */
#ifndef _LIBGDL_CORE_UTIL_GDL_STREAM_HPP_INCLUDED
#define _LIBGDL_CORE_UTIL_GDL_STREAM_HPP_INCLUDED

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

namespace libgdl
{
namespace util /** Core utilities of libGDL **/
{

/**
 * GDLStream represents input stream to the parser. This stream stores istream,
 * parent class of almost all input streams. Hence GDLStream can support many
 * different types of inputs. This helps to abstract the input from the parser.
 *
 * @see KIFParser, KIFScanner
 */
class GDLStream
{
 public:
   //! Constructs GDLStream from input file stream
   //!
   //! \param name Name of the input stream (Generally filename)
   //! \param stream File input stream
   //!
   //!
   GDLStream(const std::string& name, std::ifstream* stream)
     : name(name), stream(stream) {}

   //! Get name of the stream
   //! Name of the stream is generally used for debugging purposes
   //!
   //! \return const std::string&
   //!
   //!
   const std::string& Name() const { return name; }

   //! Set name of the stream
   //!
   //! \return std::string&
   //!
   //!
   std::string& Name() { return name; }

   //! Set stream
   //!
   //! \return std::istream*
   //!
   //!
   std::istream* Stream() { return stream; }

 private:
  //! Name of the stream
  std::string name;
  //! Stream to parse
  std::istream* stream;
}; // class GDLStream

}; // namespace util
}; // namespace libgdl


#endif // _LIBGDL_CORE_UTIL_GDL_STREAM_HPP_INCLUDED
