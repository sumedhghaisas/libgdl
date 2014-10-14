#ifndef _LIBGDL_BENCHMARK_PMEASURE_HPP_INCLUDED
#define _LIBGDL_BENCHMARK_PMEASURE_HPP_INCLUDED

#define TOLERANCE 10

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

  static int Compare(const PMeasure& pm1,
                     const PMeasure& pm2,
                     double tol = TOLERANCE)
  {
    if(pm1.avg > pm2.avg && pm1.avg - pm2.avg > tol)
      return 1;

    if(pm2.avg > pm1.avg && pm2.avg - pm1.avg > tol)
      return -1;

    return 0;
  }

  double min;
  double max;
  double avg;
};

}
}


#endif // _LIBGDL_BENCHMARK_PMEASURE_HPP_INCLUDED
