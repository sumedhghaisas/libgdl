/**
 * @file node.hpp
 * @author Sumedh Ghaisas
 *
 * Declaration Node class.
 */
#ifndef _LIBGDL_GDLPARSER_PARSER_NODE_HPP_INCLUDED
#define _LIBGDL_GDLPARSER_PARSER_NODE_HPP_INCLUDED

#include <stack>
#include <string>
#include <iostream>
#include <list>
#include <map>

#include <libgdl/core/symbol_table/symbol_table.hpp>
#include <libgdl/gdlparser/parser/location.hh>

namespace libgdl
{

namespace gdlparser
{
namespace parser
{

class Driver;

class Node
{
  typedef gdlparser::parser::yy::location location;
 public:
  Node(const yy::location& loc)
    : loc(loc) {};

  virtual ~Node() {}

  virtual operator std::string() const
  {
    std::cout << "testing warning" << std::endl;
    return "testing";
  }

  template<class T>
  virtual T* CodeGen(T*& out,
                     SymbolTable& symbol_table,
                     Driver& driver) = 0;

 protected:
  location loc;
}; // class Node

//class NBlock : public Node
//{
//public:
//    NBlock(const yy::location& loc)
//        : Node(loc) {}
//
//    ~NBlock()
//    {
//        for(std::list<Node*>::iterator it = n_stack.begin();it != n_stack.end();it++)
//            delete *it;
//    }
//
//    void AddNode(Node* n)
//    {
//        n_stack.push_front(n);
//    }
//
//    virtual operator std::string() const
//    {
//        std::string o = "";
//        for(std::list<Node*>::const_iterator it = n_stack.begin();it != n_stack.end();it++)
//            o += std::string(**it) + "\n";
//        return o;
//    }
//
//    std::list<Node*>& Stack() { return n_stack; }
//
//    Object* CodeGen(SymbolTable& symbol_table, Driver& driver)
//    {
//        ListObjects* out = new ListObjects(yy::location());
//        for(std::list<Node*>::iterator it = n_stack.begin();it != n_stack.end();it++)
//        {
//            Object* temp = (*it)->CodeGen(symbol_table, driver);
//            if(temp != NULL) out->AddObject(temp);
//        }
//        return out;
//    }
//private:
//    std::list<Node*> n_stack;
//}; // class ode

}; // namespace parser
}; // namespace gdlparser
}; // namespace libgdl

#endif // _LIBGDL_GDLPARSER_PARSER_NODE_HPP_INCLUDED
