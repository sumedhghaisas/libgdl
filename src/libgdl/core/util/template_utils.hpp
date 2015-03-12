#ifndef LIBGDL_CORE_UTIL_TEMPLATE_UTILS_HPP_INCLUDED
#define LIBGDL_CORE_UTIL_TEMPLATE_UTILS_HPP_INCLUDED

#include <tuple>

namespace libgdl
{
namespace core
{
namespace template_utils
{
namespace helper
{

template<int>
struct template_placeholder;

}
}
}
}

namespace std
{
  //! Make template_placeholder class a valid standard placeholder
  template<int N>
  struct is_placeholder<libgdl::core::template_utils::helper::template_placeholder<N>>
    : integral_constant<int, N> // the one is important
  {};
}

namespace libgdl
{
namespace core
{
namespace template_utils
{

namespace helper
{

template<int... Args>
struct Hold
{
};

template<int N, int... Args>
Hold<Args..., N> Combine(Hold<Args...>, Hold<N>)
{
  return Hold<Args..., N>();
}

template<int>
struct template_placeholder
{

};

template<int N>
struct SGenerate
{
  typedef decltype(Combine(typename SGenerate<N - 1>::type(), Hold<N>())) type;
};

template<>
struct SGenerate<1>
{
  typedef Hold<1> type;
};

template<typename RType, typename F, typename O, int... S>
RType BindToObject2(F fun, const O* o, Hold<S...>)
{
  return std::bind(fun, o, template_placeholder<S>()...);
}

template<typename RType, typename F, typename O, int... S>
RType BindToObject2(F fun, O* o, Hold<S...>)
{
  return std::bind(fun, o, template_placeholder<S>()...);
}

template<typename RType, typename F, typename O>
RType BindToObject2(F fun, const O* o)
{
  return std::bind(fun, o);
}

template<typename RType, typename F, typename O>
RType BindToObject2(F fun, O* o)
{
  return std::bind(fun, o);
}

}

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

template<typename RType, typename C, typename... Args>
std::function<RType(Args...)> BindToObject(RType (C::*fun)(Args...), C* c)
{
  return helper::BindToObject2<std::function<RType(Args...)>>(fun, c, typename helper::SGenerate<sizeof...(Args)>::type());
}

}
}
}

#endif // LIBGDL_CORE_UTIL_TEMPLATE_UTILS_HPP_INCLUDED
