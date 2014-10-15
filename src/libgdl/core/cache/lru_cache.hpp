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
 * Hit time : around 2 microseconds (highly dependent on the machine and the
 *                                   compiler)
 *
 * key_type should implement a getHash() function which returns a size_t hash
 * value or overload of 'Get' function can be used which accepts a boost
 * function to hash calculation. This hash calculation function should
 * take const key_type as input and should return size_t (hash value).
 *
 * Constructor accepts a boost function as a default function to call when there
 * is a miss. This default miss function can be overridden with appropriate
 * 'Get' function overload. The function should accept a constant Key_type
 * reference and return pointer to the value_type object. Member functions can
 * be passed by binding.
 *
 * @code
 * boost::function<B* (const A&)> f(boost::bind(&A::GetVal, this, _1))
 * LRUCache<A, B> cache(f, 1024);
 * A test;
 * B* temp = cache.Get(test);
 * @endcode
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
  //! \param default_f default miss function
  //! \param capacity number of entries to cache
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

  //! Returns the value associated with the given key.
  //! The key is identified by its hash value obtained by function getHash()
  //! Default function is called if miss.
  //!
  //! \param key input key value
  //! \return value_type*
  //!
  //!
  value_type* Get(const key_types&... keys);

  //! Returns the value associated with the given key.
  //! The key is identified by its hash value obtained by function getHash()
  //! Given function is called if miss.
  //!
  //! \param key input key value
  //! \param f_override override for default miss function
  //! \return value_type*
  //!
  //!
  value_type* Get(const MissFunction& f_override,
                  const key_types&... keys);

  //! Returns the value associated with the given key.
  //! The key is identified by its hash value obtained by boost function
  //! given as the third argument
  //! Given miss function is called if miss is detected.
  //!
  //! \param key
  //! \param f_override override for default miss function
  //! \param hash_funct_override function to compute hash
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

  //! Query if given key is present in cache
  //!
  //! \param key
  //! \param hash_funct_override
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
  //! default function to call when miss detected
  const MissFunction default_f;
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

  //! does default function exist
  bool isInitialized;

  //! logging stream
  mutable Log log;
}; // class LRUache

}; // namespace cache
}; // namespace libgdl


#include "lru_cache_impl.hpp"

#endif // _EPICURUS_CORE_LRU_CACHE_HPP_INCLUDED
