#ifndef _LIBGDL_BENCHMARK_PMEASURE_HPP_INCLUDED
#define _LIBGDL_BENCHMARK_PMEASURE_HPP_INCLUDED

namespace libgdl
{
namespace benchmark
{

struct PMeasure
{
  PMeasure()
    : min(0), max(0), avg(0) {}

  PMeasure(double min,
           double max,
           double avg)
    : min(min), max(max), avg(avg) {}

  double min;
  double max;
  double avg;
};

}
}


#endif // _LIBGDL_BENCHMARK_PMEASURE_HPP_INCLUDED
