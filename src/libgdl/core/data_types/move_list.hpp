#ifndef LIBGDL_CORE_DATA_TYPES_OVE_LIST_HPP_INCLUDED
#define LIBGDL_CORE_DATA_TYPES_OVE_LIST_HPP_INCLUDED

#include <iostream>
#include <list>
#include <vector>
#include <boost/intrusive_ptr.hpp>

#include "intrusive_list.hpp"

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
class MoveList : public boost::intrusive_ptr<core::IntrusiveList<Move> >
{
  template<typename T>
  using IntrusiveList = core::IntrusiveList<T>;

 public:
  //! Creates MoveList from IntrusiveList pointer
  //!
  //! \param m pointer to list
  //!
  //!
  MoveList(IntrusiveList<Move>* m)
    : boost::intrusive_ptr<IntrusiveList<Move> >(m) {}

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

}


#endif // LIBGDL_CORE_DATA_TYPES_OVE_LIST_HPP_INCLUDED
