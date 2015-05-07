#ifndef LIBGDL_CORE_UTIL_PREPROCESSOR_HPP_INCLUDED
#define LIBGDL_CORE_UTIL_PREPROCESSOR_HPP_INCLUDED

#include <boost/preprocessor.hpp>

#include "sfinae_utility.hpp"

/**
 * This implements uses advance SFIANE techniques to detect certain member
 * functions. his type of technique is described in the following StackOverflow
 * question
 *
 * <a href="http://stackoverflow.com/questions/257288/is-it-possible-to-write-a-c-template-to-check-for-a-functions-existence">Detection of class member function</a>
 */
#define LIBGDL_PP_HAS_MEM_FUNC(FUNC, NAME)                                               \
template<typename T, typename sig>                                             \
struct NAME {                                                                  \
  typedef char yes[1];                                                         \
  typedef char no [2];                                                         \
  template<typename U, U> struct type_check;                                   \
  template<typename _1> static yes &chk(type_check<sig, &_1::FUNC> *);         \
  template<typename   > static no  &chk(...);                                  \
  static bool const value = sizeof(chk<T>(0)) == sizeof(yes);                  \
};

#define LIBGDL_PP_CALLER(CALL, ...) CALL(__VA_ARGS__)
#define LIBGDL_PP_SEQ_CALLER(CALL, ...) CALL __VA_ARGS__

#define LIBGDL_PP_REM(...) __VA_ARGS__
#define LIBGDL_PP_EAT(...)

#define LIBGDL_PP_REM_WITH_COMMA(...) __VA_ARGS__ ,
#define LIBGDL_PP_EAT_FROM_SECOND(a, ...) a
#define LIBGDL_PP_EAT_FIRST(a, ...) __VA_ARGS__

//! Get second element in the sequence
#define LIBGDL_PP_SEQ_GET_SECOND(...) LIBGDL_PP_CALLER(LIBGDL_PP_EAT_FROM_SECOND, LIBGDL_PP_SEQ_CALLER(LIBGDL_PP_REM_WITH_COMMA, LIBGDL_PP_CALLER(LIBGDL_PP_EAT_FIRST, LIBGDL_PP_SEQ_CALLER(LIBGDL_PP_REM_WITH_COMMA, __VA_ARGS__))))
//! Get first element in the sequence
#define LIBGDL_PP_SEQ_GET_FIRST(...) LIBGDL_PP_CALLER(LIBGDL_PP_EAT_FROM_SECOND, LIBGDL_PP_SEQ_CALLER(LIBGDL_PP_REM_WITH_COMMA, __VA_ARGS__))
//! Get third element in the sequence
#define LIBGDL_PP_SEQ_GET_THIRD(...)  LIBGDL_SEQ_CALLER(LIBGDL_SEQ_REM, LIBGDL_PP_SEQ_CALLER(LIBGDL_PP_EAT, LIBGDL_PP_SEQ_CALLER(LIBGDL_PP_EAT, __VA_ARGS__)))

////////////////////////////////////////////////////////////////////////////////
/// The following macros help iterate through the class members.
/// For more information see
/// http://stackoverflow.com/questions/11031062/c-preprocessor-avoid-code-repetition-of-member-variable-list/11744832#11744832
////////////////////////////////////////////////////////////////////////////////

#define HELPER_LIBGDL_PP_SEQ_OPEN_3(a, b, c) a b c
#define HELPER_LIBGDL_PP_SEQ_OPEN_2(a, b, ...) a, b, LIBGDL_PP_SEQ_CALLER(LIBGDL_PP_REM, __VA_ARGS__)
#define HELPER_LIBGDL_PP_SEQ_OPEN_1(a, ...) a, LIBGDL_PP_SEQ_CALLER(LIBGDL_PP_REM_WITH_COMMA, __VA_ARGS__)
#define LIBGDL_PP_SEQ_OPEN(...) LIBGDL_PP_CALLER(HELPER_LIBGDL_PP_SEQ_OPEN_3, LIBGDL_PP_CALLER(HELPER_LIBGDL_PP_SEQ_OPEN_2, LIBGDL_PP_CALLER(HELPER_LIBGDL_PP_SEQ_OPEN_1, LIBGDL_PP_SEQ_CALLER(LIBGDL_PP_REM_WITH_COMMA, __VA_ARGS__))))

#define LIBGDL_PP_REFLECTABLE(...) \
static const size_t fields_n = BOOST_PP_VARIADIC_SIZE(__VA_ARGS__); \
friend struct libgdl::sfinae::reflection::Reflector; \
template<int N, class Self> \
struct field_data {}; \
BOOST_PP_SEQ_FOR_EACH_I(LIBGDL_PP_REFLECT_EACH, data, BOOST_PP_VARIADIC_TO_SEQ(__VA_ARGS__))

#define LIBGDL_PP_REFLECT_EACH(r, data, i, x) \
LIBGDL_PP_SEQ_OPEN(x); \
template<class Self> \
struct field_data<i, Self> \
{ \
    Self & self; \
    field_data(Self & self) : self(self) {} \
    \
    typename libgdl::sfinae::make_const<Self, LIBGDL_PP_SEQ_GET_FIRST(x)>::type & get() \
    { \
        return self.LIBGDL_PP_SEQ_GET_SECOND(x); \
    }\
    typename boost::add_const<LIBGDL_PP_SEQ_GET_FIRST(x)>::type & get() const \
    { \
        return self.LIBGDL_PP_SEQ_GET_SECOND(x); \
    }\
    const char * name() const \
    {\
        return BOOST_PP_STRINGIZE(LIBGDL_PP_SEQ_GET_SECOND(x)); \
    } \
}; \

#endif // LIBGDL_CORE_UTIL_PREPROCESSOR_HPP_INCLUDED
