/**
 * @file sfinae_utility.hpp
 * @author Sumedh Ghaisas
 *
 * Declaration of some useful SFINAE utilities.
 */
#ifndef _LIBGDL_CORE_UTIL_SFINAE_UTILITY_HPP_INCLUDED
#define _LIBGDL_CORE_UTIL_SFINAE_UTILITY_HPP_INCLUDED

#include <array>
#include <iostream>
#include <boost/utility/enable_if.hpp>
#include <boost/type_traits.hpp>
#include <boost/mpl/range_c.hpp>
#include <boost/mpl/for_each.hpp>
#include <boost/bind.hpp>

namespace libgdl
{
namespace sfinae
{

/**
 * Checks whether a given class implements std::string() operator.
 *
 * /tparam T Class to check
 */
template <class T>
struct SupportsStringOperator
{
  template <class U>
  static auto string_test(const U* u) -> decltype(std::string(*u), char(0))
  { }

  static std::array<char, 2> string_test(...) { return std::array<char, 2>(); }

  static const bool value = (sizeof(string_test((T*)0)) == 1);
}; // class SupportsStringOperator

// A helper metafunction for adding const to a type
template<class M, class T>
struct make_const
{
    typedef T type;
};

template<class M, class T>
struct make_const<const M, T>
{
    typedef typename boost::add_const<T>::type type;
};

namespace reflection
{

struct SimpleFieldPrint
{
  static SimpleFieldPrint& GetGlobalObject()
  {
    static SimpleFieldPrint singleton;
    return singleton;
  }

  template<typename T>
  void PrintField(const std::string& name, const T& t)
  {
    std::cout << name << " = " << t << std::endl;
  }
};

struct Reflector
{
  //Get field_data at index N
  template<int N, class T>
  static typename T::template field_data<N, T> GetFieldData(T& x)
  {
    return typename T::template field_data<N, T>(x);
  }

  // Get the number of fields
  template<class T>
  struct NumFields
  {
    static const int value = T::fields_n;
  };

  struct FieldVisitor
  {
    template<class C, class Visitor, class T, class PrintClass>
    void operator()(C& c, Visitor v, T, PrintClass& pc)
    {
      v(GetFieldData<T::value>(c), pc);
    }
  };

  template<class C, class Visitor, class PrintClass>
  static void VisitEach(C & c, Visitor v, PrintClass& pc)
  {
    typedef boost::mpl::range_c<int,0, NumFields<C>::value> range;
    boost::mpl::for_each<range>(boost::bind<void>(FieldVisitor(), boost::ref(c), v, _1, pc));
  }

  struct PrintVisitor
  {
    template<class FieldData, class PrintClass>
    void operator()(FieldData f, PrintClass& pc)
    {
      pc.PrintField(f.name(), f.get());
    }
  };

  template<class T, class PrintClass = SimpleFieldPrint>
  static void PrintFields(T & x, PrintClass& pc = SimpleFieldPrint::GetGlobalObject())
  {
    VisitEach(x, PrintVisitor(), pc);
  }
};

}; // namespace reflection
}; // namespace sfinae
}; // namespace libgdl

#endif // _LIBGDL_CORE_UTIL_SFINAE_UTILITY_HPP_INCLUDED
