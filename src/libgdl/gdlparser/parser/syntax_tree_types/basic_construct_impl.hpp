/**
 * @file basic_construct_impl.hpp
 * @author Sumedh Ghaisas
 *
 * Implementation of BasicConstruct class.
 */
namespace libgdl
{
namespace gdlparser
{
namespace parser
{

template <class Policy, class Head, class Hold>
BasicConstruct<Policy, Head, Hold>::operator std::string() const
{
  std::string o = "";
  if(args.size() > 0)
  {
    o += "(" + ToString(*command);
    for(typename std::list<Hold*>::const_iterator it = args.begin();
                                                        it != args.end(); it++)
    {
      o += " " + ToString(**it);
    }
    o += ")";
  }
  else o += ToString(*command);
  return o;
}

template <class Policy, class Head>
BasicConstruct<Policy, Head, TerminalHold>::operator std::string() const
{
  std::string o = "";
  if(args.size() > 0)
  {
    o += "(" + ToString(*command);
    for(typename std::list<BasicConstruct<Policy, Head, TerminalHold>*>::
                      const_iterator it = args.begin(); it != args.end(); it++)
    {
      o += " " + ToString(**it);
    }
    o += ")";
  }
  else o += ToString(*command);
  return o;
}

}; // namespace parser
}; // namespace gdlparser
}; // namespace libgdl
