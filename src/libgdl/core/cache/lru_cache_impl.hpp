/**
 * @file lru_cache_impl.hpp
 * @author Sumedh Ghaisas
 *
 * Implementation of LRUCache.
 */
#include <boost/bind.hpp>

namespace libgdl
{
namespace cache
{

template<class value_type, typename... key_types>
LRUCache<value_type, key_types...>::
  LRUCache(const MissFunction& f,
           const HashFunction& hf,
           unsigned short cap,
           const Log& log)
    : default_f(f),
    default_hf(hf),
    capacity(cap + 1),
    log(log)
{
  hashs = new size_t[capacity];
  values = new value_type*[capacity];
  for(size_t i = 0;i < capacity;i++) values[i] = NULL;
  forward_pointing = new unsigned short[capacity];
  backward_pointing = new unsigned short[capacity];

  // 0th index is used as the loopback index
  next_empty = 1;
  isFull = false;

  // initialize all the slots
  for(size_t i = 0;i < capacity;i++)
  {
    forward_pointing[i] = 0;
    backward_pointing[i] = 0;
  }
}

template<class value_type, typename... key_types>
LRUCache<value_type, key_types...>::~LRUCache()
{
  for(size_t i = 0;i < capacity;i++) delete values[i];
  delete[] values;
  delete[] hashs;
  delete[] forward_pointing;
  delete[] backward_pointing;
}

template<class value_type, typename... key_types>
value_type* LRUCache<value_type, key_types...>::Get(const key_types&... keyin)
{
  return Get(default_f, default_hf, keyin...);
}

template<class value_type, typename... key_types>
value_type* LRUCache<value_type, key_types...>::
  Get(const MissFunction& f_override,
      const key_types&... keyin)
{
  return Get(f_override, default_hf, keyin...);
}

template<class value_type, typename... key_types>
value_type* LRUCache<value_type, key_types...>::
  Get(const MissFunction& f_override,
      const HashFunction& hash_funct,
      const key_types&... keyin)
{
  // get hash value of the key
  size_t hash = hash_funct(keyin...);

  // check for a hit
  boost::unordered_map<size_t, unsigned short>::const_iterator it =
                                                      slot_map.find(hash);

  // if it is a hit
  if(it != slot_map.end())
  {
    const short& slot = it->second;
    const short& next = forward_pointing[slot];
    const short& back = backward_pointing[slot];

    // remove the element from the link list and join the back and next element
    forward_pointing[back] = next;
    backward_pointing[next] = back;

    // put the element at the start
    forward_pointing[slot] = forward_pointing[0];
    backward_pointing[forward_pointing[0]] = slot;
    forward_pointing[0] = slot;
    backward_pointing[slot] = 0;

    // return the value associated with that slot
    return values[slot];
  }
  // if it is a miss but cache is not yet full
  else if(!isFull)
  {
    // get next non empty slot and increment the next_empty
    const short slot = next_empty++;

    // flag the cache being full if next_empty equals capacity
    if(next_empty == capacity) isFull = true;

    // put hash value of the key into the slot
    hashs[slot] = hash;
    // put the entry of the key in the map
    slot_map[hash] = slot;

    // put the newly entry at the start of the link list
    forward_pointing[slot] = forward_pointing[0];
    backward_pointing[forward_pointing[0]] = slot;
    forward_pointing[0] = slot;
    backward_pointing[slot] = 0;

    // call miss function and update the value
    value_type* temp = f_override(keyin...);
    values[slot] = temp;
    // return the calculated value
    return temp;
  }
  else
  {
    // the the least recently used position
    // which is the tail of the link list
    const short& slot = backward_pointing[0];
    const short& next = forward_pointing[slot];
    const short& back = backward_pointing[slot];

    // erase the entry against that slot from the map
    slot_map.erase(slot_map.find(hashs[slot]));

    // update the hash of the provided key in that slot
    hashs[slot] = hash;
    // add the entry of that slot in the map against new hash
    slot_map[hash] = slot;

    // remove the element from the link list and join the back and next element
    forward_pointing[back] = next;
    backward_pointing[next] = back;

    // add the entry at the start of the link list
    forward_pointing[slot] = forward_pointing[0];
    backward_pointing[forward_pointing[0]] = slot;
    forward_pointing[0] = slot;
    backward_pointing[slot] = 0;

    // call the miss function and get the update the value
    value_type* temp = f_override(keyin...);
    // delete the existing value
    delete values[slot];
    // replace it with new value
    values[slot] = temp;
    // return the calculated value
    return temp;
  }
  return NULL; // to avoid warning
}

template<class value_type, typename... key_types>
value_type* LRUCache<value_type, key_types...>::Query(const key_types&... keyin)
{
  return Query(default_hf, keyin...);
}

template<class value_type, typename... key_types>
value_type* LRUCache<value_type, key_types...>::
  Query(const HashFunction& hash_funct,
        const key_types&... key)
{
  // get hash value of the key
  size_t hash = hash_funct(key...);

  // check for a hit
  boost::unordered_map<size_t, unsigned short>::const_iterator it =
                                                          slot_map.find(hash);

  if(it != slot_map.find(hash)) return values[it->second];
  else return NULL;
}

template<class value_type, typename... key_types>
void LRUCache<value_type, key_types...>::
    SetDefaultFunction(const MissFunction& f)
{
  default_f = f;
}

}; // namespace cache
}; // namespace libgdl
