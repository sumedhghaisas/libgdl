namespace libgdl
{
namespace gdlreasoner
{

inline logicbase::Answer* KnowledgeBase::GetAnswer(const Argument& question,
                                                   const VariableMap& v_map,
                                                   const std::set<size_t>& visited) const
{
  //core::SymbolDecodeStream sds(symbol_table);
  //sds << question << std::endl;

  if(cached_maps.find(question.Hash()) != cached_maps.end())
  {
    return new Answer(Answer::CACHE, question, v_map, *this, visited, NULL);
  }

  Answer *ans = NULL;

  if(question.value == SymbolTable::OrID)
    ans = new Answer(Answer::OR, question, v_map, *this, visited, NULL);
  else if(question.value == SymbolTable::DistinctID)
    ans = new Answer(Answer::DISTINCT, question, v_map, *this, visited, NULL);
  else if(question.value == SymbolTable::NotID)
    ans = new Answer(Answer::NOT, question, v_map, *this, visited, NULL);
  else ans = new Answer(Answer::CLAUSE, question, v_map, *this, visited, NULL);

  if(logicbase::Unify::IsGroundQuestion(&question, v_map))
  {
    ans = new Answer(Answer::GROUND, question, v_map, *this, visited, ans);
  }

  return new Answer(Answer::DECODER, question, v_map, *this, visited, ans);
}

}
}
