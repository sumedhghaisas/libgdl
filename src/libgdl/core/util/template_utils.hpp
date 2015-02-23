#ifndef LIBGDL_CORE_UTIL_TEMPLATE_UTILS_HPP_INCLUDED
#define LIBGDL_CORE_UTIL_TEMPLATE_UTILS_HPP_INCLUDED

#include <tuple>

namespace libgdl
{
namespace core
{
namespace template_utils
{

template<size_t N, typename T, typename... Args>
struct NTupleGenerator
{
  typedef typename NTupleGenerator<N - 1, T, Args... , T>::type type;
};
template<typename T, typename... Args>
struct NTupleGenerator<0, T, Args...>
{
  typedef typename std::tuple<Args...> type;
};

template<size_t... args>
struct SeriesHolder
{

};

template<size_t N, size_t... args>
struct SeriesGenerator
{
  typedef typename SeriesGenerator<N - 1, N, args...>::type type;
};

template<size_t... args>
struct SeriesGenerator<0, args...>
{
  typedef SeriesHolder<0, args...> type;
};

}
}
}

#endif // LIBGDL_CORE_UTIL_TEMPLATE_UTILS_HPP_INCLUDED
