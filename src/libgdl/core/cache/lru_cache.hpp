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
namespace cache
{
/**
 * LRU Cache module.
 * Hit time : around 2 microseconds (highly dependent on the machine and the
 *                                   compiler)
 *
 * key_type should implement a getHash() function which returns a size_t hash
 * value or overload of Get function can be used which accepts a function
 * pointer of hash calculation function. This hash calculation function should
 * take const key_type as input and should return size_t (hash value).
 *
 * Constructor accepts a boost function as a default function to call when there
 * is a miss. This default miss function can be overridden with appropriate Get
 * function overload. The function should accept a constant Key_type reference
 * and return pointer to the value_type object. Member functions can be passed
 * by binding. (for example see the use of LRUCache in GDL class)
 *
 * @code
 * boost::function<B* (const A&)> f(boost::bind(&A::GetVal, this, _1))
 * LRUCache<A, B> cache(f, 1024);
 * A test;
 * B* temp = cache.Get(test);
 * @endcode
 *
 */

template<class key_type, class value_type>
class LRUCache
{
 public:
  //! Constructor
  //!
  //! \param default_f const boost::function<value_type* (const key_type&)>&
  //! : default miss calling function
  //! \param capacity unsigned short : number of entries to cache
  //!
  //!
  LRUCache(const boost::function<value_type* (const key_type&)> default_f,
           unsigned short capacity = 1024);

  //! Constructor
  //!
  //! \param unsigned short capacity = 1024d
  //!
  //!
  LRUCache(unsigned short capacity = 1024);

  ~LRUCache()
  {
    for(size_t i = 0;i < capacity;i++) delete values[i];
    delete[] values;
    delete[] hashs;
    delete[] forward_pointing;
    delete[] backward_pointing;
  }

  //! Returns the value associated with the given key.
  //! The key is identified by its hash value obtained by function getHash()
  //! Default function is called if miss.
  //!
  //! \param key const key_type&
  //! \return value_type*
  //!
  //!
  value_type* Get(const key_type& key);

  //! Returns the value associated with the given key.
  //! The key is identified by its hash value obtained by function getHash()
  //! Given function is called if miss.
  //!
  //! \param key const key_type&
  //! \param f_override const boost::function<value_type* (const key_type&)>&
  //!                                 : override for default miss function
  //! \return value_type*
  //!
  //!
  value_type* Get(const key_type& key,
                  boost::function<value_type* (const key_type&)>& f_override);

  //! Returns the value associated with the given key.
  //! The key is identified by its hash value obtained by boost function
  //! given as the third argument
  //! Given miss function is called if miss is detected.
  //!
  //! \param key const key_type&
  //! \param f_override const boost::function<value_type* (const key_type&)>&
  //!         : override for default miss function
  //! \param hash_funct_override const boost::function<size_t (const key_type&)>&
  //!         : function to compute hash
  //! \return value_type*
  //!
  //!
  value_type* Get(const key_type& key,
                  boost::function<value_type* (const key_type&)>& f_override,
                  boost::function<size_t (const key_type&)>& hash_funct_override);

  value_type* Query(const key_type& key);

  value_type* Query(const key_type& key,
                    boost::function<size_t (const key_type&)>& hash_funct_override);

  //! sets cache is with default miss function
  //!
  //! \param key_type& boost::function<value_type* (const
  //! \return void
  //!
  //!
  void SetDefaultFunction(boost::function<value_type* (const key_type&)> default_f);

  Log& GetLog() { return log; }

 private:
  //! default function to call when miss detected
  const boost::function<value_type* (const key_type&)> default_f;

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
