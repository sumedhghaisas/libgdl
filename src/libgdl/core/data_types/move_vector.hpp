#ifndef LIBGDL_CORE_DATA_TYPES_MOVE_VECTOR_HPP_INCLUDED
#define LIBGDL_CORE_DATA_TYPES_MOVE_VECTOR_HPP_INCLUDED

#include <iostream>
#include <vector>
#include <boost/intrusive_ptr.hpp>

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
class MoveVector : public boost::intrusive_ptr<core::IntrusiveWrapper<std::vector<Move>>>
{
  template<typename T>
  using IntrusiveVector = core::IntrusiveWrapper<std::vector<T>>;

 public:
  //! Creates MoveList from IntrusiveList pointer
  //!
  //! \param m pointer to list
  //!
  //!
  MoveVector(IntrusiveVector<Move>* m)
    : boost::intrusive_ptr<IntrusiveVector<Move>>(m) {}

  Move operator[](size_t index)
  {
    return (*this->get())[index];
  }

  //! implements its own iterator
  typedef typename IntrusiveVector<Move>::iterator iterator;
  //! implements its own const_iterator
  typedef typename IntrusiveVector<Move>::const_iterator const_iterator;

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
class MoveVector<AMove> : public boost::intrusive_ptr<core::IntrusiveWrapper<std::vector<AMove>>>
{
  template<typename T>
  using IntrusiveVector = core::IntrusiveWrapper<std::vector<T>>;

 public:
  //! Creates MoveList from IntrusiveList pointer
  //!
  //! \param m pointer to list
  //!
  //!
  MoveVector(IntrusiveVector<AMove>* m = NULL)
    : boost::intrusive_ptr<IntrusiveVector<AMove> >(m) {}

  MoveVector(const std::list<size_t>* result, size_t n_roles)
    : boost::intrusive_ptr<IntrusiveVector<AMove>>(new IntrusiveVector<AMove>())
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

  AMove operator[](size_t index)
  {
    return (*this->get())[index];
  }

  template<typename... Args>
  bool ForwardToEmplaceBack(Args... args)
  {
    (*this)->emplace_back(args...);
    return true;
  }

  //! implements its own iterator
  typedef typename IntrusiveVector<AMove>::iterator iterator;
  //! implements its own const_iterator
  typedef typename IntrusiveVector<AMove>::const_iterator const_iterator;

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

inline std::ostream& operator<<(std::ostream& stream, const MoveVector<AMove>& m)
{
  stream << "MoveVector {" << std::endl;
  for(auto it : m)
  {
    stream << it << std::endl;
  }
  stream << "}";
  return stream;
}

}

#endif // LIBGDL_CORE_DATA_TYPES_MOVE_VECTOR_HPP_INCLUDED
