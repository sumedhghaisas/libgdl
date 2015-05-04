#ifndef LIBGDL_CORE_DATA_TYPES_OVE_LIST_HPP_INCLUDED
#define LIBGDL_CORE_DATA_TYPES_OVE_LIST_HPP_INCLUDED

#include <iostream>
#include <list>
#include <vector>
#include <boost/intrusive_ptr.hpp>
#include <boost/unordered_set.hpp>

#include "intrusive_wrapper.hpp"

#include "a_move.hpp"

namespace libgdl
{

/**
 * Represents the list of moves. Supports intrusive pointers.
 * This is used in storing list of legal moves in GDL.
 * This class is implemented in such a way that the intrusive pointer part is
 * totally abstracted from the user. For user MoveList can act like a normal
 * class rather than a smart pointer.
 *
 * @see IntrusiveList, BMove, GDL
 */
template<typename Move>
class MoveList : public boost::intrusive_ptr<core::IntrusiveWrapper<std::list<Move>>>
{
  template<typename T>
  using IntrusiveList = core::IntrusiveWrapper<std::list<T>>;

 public:
  //! Creates MoveList from IntrusiveList pointer
  //!
  //! \param m pointer to list
  //!
  //!
  MoveList(IntrusiveList<Move>* m = NULL)
    : boost::intrusive_ptr<IntrusiveList<Move>>(m) {}

  //! implements its own iterator
  typedef typename IntrusiveList<Move>::iterator iterator;
  //! implements its own const_iterator
  typedef typename IntrusiveList<Move>::const_iterator const_iterator;

  //! Returns const_iterator pointing to the beginning of the list
  //!
  //! \return const_iterator
  //!
  //!
  const_iterator begin() const { return this->get()->begin(); }

  //! Returns iterator pointing to the beginning of the list
  //!
  //! \return iterator
  //!
  //!
  iterator begin() { return this->get()->begin(); }

  //! Returns const_iterator pointing to the end of the list
  //!
  //! \return const_iterator
  //!
  //!
  const_iterator end() const { return this->get()->end(); }

  //! Returns iterator pointing to the end of the list
  //!
  //! \return iterator
  //!
  //!
  iterator end() { return this->get()->end(); }

  //! Returns size of the list
  //!
  //! \return size_t
  //!
  //!
  size_t size() { return this->get()->size(); }

}; // class MoveList

template<>
class MoveList<AMove> : public boost::intrusive_ptr<core::IntrusiveWrapper<std::list<AMove>>>
{
  template<typename T>
  using IntrusiveList = core::IntrusiveWrapper<std::list<T>>;

 public:
  //! Creates MoveList from IntrusiveList pointer
  //!
  //! \param m pointer to list
  //!
  //!
  MoveList(IntrusiveList<AMove>* m = NULL)
    : boost::intrusive_ptr<IntrusiveList<AMove>>(m) {}

  MoveList(const std::string&)
    : boost::intrusive_ptr<IntrusiveList<AMove>>(new IntrusiveList<AMove>()) {}

  MoveList(const std::list<size_t>* result, size_t n_roles)
    : boost::intrusive_ptr<IntrusiveList<AMove>>(new IntrusiveList<AMove>())
  {
    std::list<size_t>::const_iterator* it = new std::list<size_t>::const_iterator[n_roles];
    for(size_t i = 0;i < n_roles;i++)
      it[i] = result[i].begin();

    while(true)
    {
      std::list<size_t> moves;
      for(size_t i = 0;i < n_roles;i++)
      {
        moves.push_back(*it[i]);
      }
      (*this)->push_back(AMove(moves));

      it[0]++;
      size_t index = 1;
      if(it[0] == result[0].end())
      {
        it[0] = result[0].begin();

        while(true)
        {
          if(index == n_roles ||
            (it[index] == (--result[index].end()) && index == n_roles - 1))
          {
            delete[] it;
            return;
          }
          else if(it[index] == (--result[index].end()))
          {
            it[index] = result[index].begin();
            index++;
          }
          else
          {
            it[index]++;
            break;
          }
        }
      }
    }
  }

  MoveList(const std::set<size_t>* result, size_t n_roles)
    : boost::intrusive_ptr<IntrusiveList<AMove>>(new IntrusiveList<AMove>())
  {
    std::set<size_t>::const_iterator* it = new std::set<size_t>::const_iterator[n_roles];
    for(size_t i = 0;i < n_roles;i++)
      it[i] = result[i].begin();

    std::set<size_t>::const_iterator temp;

    while(true)
    {
      std::list<size_t> moves;
      for(size_t i = 0;i < n_roles;i++)
      {
        moves.push_back(*it[i]);
      }
      (*this)->push_back(AMove(moves));

      it[0]++;
      size_t index = 1;
      if(it[0] == result[0].end())
      {
        it[0] = result[0].begin();

        while(true)
        {
          temp = it[index];

          if(index == n_roles ||
            (++temp == result[index].end() && index == n_roles - 1))
          {
            delete[] it;
            return;
          }
          else if(++temp == result[index].end())
          {
            it[index] = result[index].begin();
            index++;
          }
          else
          {
            it[index]++;
            break;
          }
        }
      }
    }
  }

  void push_back(const AMove& m)
  {
    get()->push_back(m);
  }

  //! implements its own iterator
  typedef typename IntrusiveList<AMove>::iterator iterator;
  //! implements its own const_iterator
  typedef typename IntrusiveList<AMove>::const_iterator const_iterator;

  //! Returns const_iterator pointing to the beginning of the list
  //!
  //! \return const_iterator
  //!
  //!
  const_iterator begin() const { return this->get()->begin(); }

  //! Returns iterator pointing to the beginning of the list
  //!
  //! \return iterator
  //!
  //!
  iterator begin() { return this->get()->begin(); }

  //! Returns const_iterator pointing to the end of the list
  //!
  //! \return const_iterator
  //!
  //!
  const_iterator end() const { return this->get()->end(); }

  //! Returns iterator pointing to the end of the list
  //!
  //! \return iterator
  //!
  //!
  iterator end() { return this->get()->end(); }

  //! Returns size of the list
  //!
  //! \return size_t
  //!
  //!
  size_t size() { return this->get()->size(); }

}; // class MoveList

inline std::ostream& operator<<(std::ostream& stream, const MoveList<AMove>& m)
{
  stream << "MoveList {" << std::endl;
  for(auto it : m)
  {
    stream << it << std::endl;
  }
  stream << "}";
  return stream;
}

}


#endif // LIBGDL_CORE_DATA_TYPES_OVE_LIST_HPP_INCLUDED
