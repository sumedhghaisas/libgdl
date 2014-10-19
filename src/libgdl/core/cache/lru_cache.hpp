/**
 * @file lru_cache.hpp
 * @author Sumedh Ghaisas
 *
 * Declaration of LRUCache.
 */
#ifndef _LIBGDL_CORE_LRU_CACHE_HPP_INCLUDED
#define _LIBGDL_CORE_LRU_CACHE_HPP_INCLUDED

#include <boost/function.hpp>
#include <boost/unordered_map.hpp>

namespace libgdl
{
namespace cache /** Cache System of libGDL **/
{
/**
 * LRU Cache module.
 * Hit time : around 1 microseconds (highly dependent on the machine and the
 *                                   compiler)
 *
 * \tparam value_type The class of which data is stored and returned
 * \tparam key_value The list of values to pass to miss function
 *
 * The class supports multiple key_types via C++11 Variadic Templates. All
 * the provided values to the get function will be passed to the miss function.
 * The constructor accepts miss function along with the hash function. The hash
 * function is called to check hit or miss.
 *
 * For sample usage see use of LRUCache in GDL module. Check files
 * cache_tests.cpp and cache_benchmarks.cpp for more examples.
 *
 * @see GDL
 */
template<class value_type, typename... key_types>
class LRUCache
{
  typedef boost::function<value_type* (const key_types&...)> MissFunction;
  typedef boost::function<size_t (const key_types&...)> HashFunction;

 public:

  //! Constructor
  //!
  //! \param default_f Default miss function
  //! \param default_hf Default hash function
  //! \param capacity Number of entries to cache
  //! \param log Logging stream
  //!
  //!
  LRUCache(const MissFunction& default_f,
           const HashFunction& default_hf,
           unsigned short capacity = 1024,
           const Log& log = GLOBAL_LOG);

  //! Destructor
  //!
  //!
  //!
  ~LRUCache();

  //! Returns the value associated with the given keys.
  //! The keys are identified by their hash value obtained by default hash
  //! function. Default function is called in case of a miss.
  //!
  //! \param keys Input key values
  //! \return value_type*
  //!
  //!
  value_type* Get(const key_types&... keys);

  //! Returns the value associated with the given keys.
  //! The keys are identified by its hash value obtained by default hash
  //! function. Given function is called in case of a miss.
  //!
  //! \param f_override Override for default miss function
  //! \param keys input key values
  //! \return value_type*
  //!
  //!
  value_type* Get(const MissFunction& f_override,
                  const key_types&... keys);

  //! Returns the value associated with the given keys.
  //! The keys are identified by their hash value obtained by boost function
  //! given as the third argument. Given miss function is called in case of a
  //! miss.
  //!
  //! \param f_override Override for default miss function
  //! \param hash_funct_override Override for default hashing function
  //! \param keys Input key values
  //! \return value_type*
  //!
  //!
  value_type* Get(const MissFunction& f_override,
                  const HashFunction& hash_funct_override,
                  const key_types&... keys);

  //! Query if given key is present in cache
  //!
  //! \param key
  //! \return value_type*
  //!
  //!
  value_type* Query(const key_types&... key);

  //! Query if given key is present in cache with given hash function
  //!
  //! \param hash_funct_override Override for default hash function
  //! \param keys Input key values
  //! \return value_type*
  //!
  //!
  value_type* Query(const HashFunction& hash_funct_override,
                    const key_types&... keys);

  //! Sets cache with default miss function
  //!
  //! \param default_f deafult miss function to use
  //! \return void
  //!
  //!
  void SetDefaultFunction(const MissFunction& default_f);

  //! Returns logging stream
  Log& GetLog() { return log; }

 private:
  //! Default function to call when miss detected
  const MissFunction default_f;
  //! Default hash function
  const HashFunction default_hf;

  //! maximum entries to cache
  short capacity;

  //! array storing hash of the cached keys
  uint64_t *hashs;

  //! array of pointers pointing towards cached values
  value_type **values;

  //! basic implementation of circular link list (used for LRU)
  unsigned short* forward_pointing;
  unsigned short* backward_pointing;

  //! mapping of hash to slot in which value is stored
  boost::unordered_map<size_t, unsigned short> slot_map;

  //! provides the next empty location (used until cache is not full)
  unsigned short next_empty;

  //! status of the cache (full or not)
  bool isFull;

  //! logging stream
  mutable Log log;
}; // class LRUache

}; // namespace cache
}; // namespace libgdl


#include "lru_cache_impl.hpp"

#endif // _EPICURUS_CORE_LRU_CACHE_HPP_INCLUDED
