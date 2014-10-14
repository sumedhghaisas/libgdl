/**
 * @file pmeasure.hpp
 * @author Sumedh Ghaisas
 *
 * Declaration of PMeasure class.
 */
#ifndef _LIBGDL_BENCHMARK_PMEASURE_HPP_INCLUDED
#define _LIBGDL_BENCHMARK_PMEASURE_HPP_INCLUDED

namespace libgdl
{
namespace benchmark
{

/**
 * This class stores information about processed benchmarking task. This
 * structure stores the minimum, maximum and the average time associated with
 * a certain task.
 */
struct PMeasure
{
  //! Default constructible
  //!
  //!
  //!
  PMeasure()
    : min(0), max(0), avg(0) {}

  //! Constructs PMeasure object with given min, max and avg
  //!
  //! \param min Minimum time noted
  //! \param max Maximum time noted
  //! \param avg Average time noted
  //! \return
  //!
  //!
  PMeasure(double min,
           double max,
           double avg)
    : min(min), max(max), avg(avg) {}

  //! Compare to PMeasure objects
  //! Comparison is done solely on the basis of average time. Tolerance is
  //! taken into consideration while comparing the two objects. Function returns
  //! -1 if first object is less than the second and 1 for vise versa. Function
  //! returns 0 in the case of equality.
  //!
  //! \param pm1 First object
  //! \param pm2 Second object
  //! \param tol Tolerance
  //! \return int
  //!
  //!
  static int Compare(const PMeasure& pm1,
                     const PMeasure& pm2,
                     double tol)
  {
    if(pm1.avg > pm2.avg && pm1.avg - pm2.avg > tol)
      return 1;

    if(pm2.avg > pm1.avg && pm2.avg - pm1.avg > tol)
      return -1;

    return 0;
  }

  //! Minimum time noted
  double min;
  //! Maximum time noted
  double max;
  //! Average time
  double avg;
}; // struct PMeasure

}; // namespace benchmark
}; // namespace libgdl


#endif // _LIBGDL_BENCHMARK_PMEASURE_HPP_INCLUDED
