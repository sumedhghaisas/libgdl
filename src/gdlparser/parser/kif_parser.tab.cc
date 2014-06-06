// A Bison parser, made by GNU Bison 3.0.

// Skeleton implementation for Bison LALR(1) parsers in C++

// Copyright (C) 2002-2013 Free Software Foundation, Inc.

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

// As a special exception, you may create a larger work that contains
// part or all of the Bison parser skeleton and distribute that work
// under terms of your choice, so long as that work isn't itself a
// parser generator using the skeleton or a modified version thereof
// as a parser skeleton.  Alternatively, if you modify or redistribute
// the parser skeleton itself, you may (at your option) remove this
// special exception, which will cause the skeleton and the resulting
// Bison output files to be licensed under the GNU General Public
// License without this special exception.

// This special exception was added by the Free Software Foundation in
// version 2.2 of Bison.


// First part of user declarations.

#line 37 "kif_parser.tab.cc" // lalr1.cc:398

# ifndef YY_NULL
#  if defined __cplusplus && 201103L <= __cplusplus
#   define YY_NULL nullptr
#  else
#   define YY_NULL 0
#  endif
# endif

#include "kif_parser.tab.hh"

// User implementation prologue.

#line 51 "kif_parser.tab.cc" // lalr1.cc:406
// Unqualified %code blocks.
#line 47 "kif_parser.yy" // lalr1.cc:407

#include <algorithm>
#include <iostream>
#include <iterator>
#include <sstream>

// driver class declaration.
#include "kif_driver.hpp"

// use yylex function in scanner class instead of predefiend yylex
#undef yylex
#define yylex scanner.yylex


#line 68 "kif_parser.tab.cc" // lalr1.cc:407


#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> // FIXME: INFRINGES ON USER NAME SPACE.
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif

#define YYRHSLOC(Rhs, K) ((Rhs)[K].location)
/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

# ifndef YYLLOC_DEFAULT
#  define YYLLOC_DEFAULT(Current, Rhs, N)                               \
    do                                                                  \
      if (N)                                                            \
        {                                                               \
          (Current).begin  = YYRHSLOC (Rhs, 1).begin;                   \
          (Current).end    = YYRHSLOC (Rhs, N).end;                     \
        }                                                               \
      else                                                              \
        {                                                               \
          (Current).begin = (Current).end = YYRHSLOC (Rhs, 0).end;      \
        }                                                               \
    while (/*CONSTCOND*/ false)
# endif


// Suppress unused-variable warnings by "using" E.
#define YYUSE(E) ((void) (E))

// Enable debugging if requested.
#if YYDEBUG

// A pseudo ostream that takes yydebug_ into account.
# define YYCDEBUG if (yydebug_) (*yycdebug_)

# define YY_SYMBOL_PRINT(Title, Symbol)         \
  do {                                          \
    if (yydebug_)                               \
    {                                           \
      *yycdebug_ << Title << ' ';               \
      yy_print_ (*yycdebug_, Symbol);           \
      *yycdebug_ << std::endl;                  \
    }                                           \
  } while (false)

# define YY_REDUCE_PRINT(Rule)          \
  do {                                  \
    if (yydebug_)                       \
      yy_reduce_print_ (Rule);          \
  } while (false)

# define YY_STACK_PRINT()               \
  do {                                  \
    if (yydebug_)                       \
      yystack_print_ ();                \
  } while (false)

#else // !YYDEBUG

# define YYCDEBUG if (false) std::cerr
# define YY_SYMBOL_PRINT(Title, Symbol)  YYUSE(Symbol)
# define YY_REDUCE_PRINT(Rule)           static_cast<void>(0)
# define YY_STACK_PRINT()                static_cast<void>(0)

#endif // !YYDEBUG

#define yyerrok         (yyerrstatus_ = 0)
#define yyclearin       (yyempty = true)

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYRECOVERING()  (!!yyerrstatus_)

#line 12 "kif_parser.yy" // lalr1.cc:473
namespace gdlparser { namespace parser { namespace yy {
#line 154 "kif_parser.tab.cc" // lalr1.cc:473

  /* Return YYSTR after stripping away unnecessary quotes and
     backslashes, so that it's suitable for yyerror.  The heuristic is
     that double-quoting is unnecessary unless the string contains an
     apostrophe, a comma, or backslash (other than backslash-backslash).
     YYSTR is taken from yytname.  */
  std::string
  KIFParser::yytnamerr_ (const char *yystr)
  {
    if (*yystr == '"')
      {
        std::string yyr = "";
        char const *yyp = yystr;

        for (;;)
          switch (*++yyp)
            {
            case '\'':
            case ',':
              goto do_not_strip_quotes;

            case '\\':
              if (*++yyp != '\\')
                goto do_not_strip_quotes;
              // Fall through.
            default:
              yyr += *yyp;
              break;

            case '"':
              return yyr;
            }
      do_not_strip_quotes: ;
      }

    return yystr;
  }


  /// Build a parser object.
  KIFParser::KIFParser (KIFScanner  &scanner_yyarg, KIFDriver  &driver_yyarg)
    :
#if YYDEBUG
      yydebug_ (false),
      yycdebug_ (&std::cerr),
#endif
      scanner (scanner_yyarg),
      driver (driver_yyarg)
  {}

  KIFParser::~KIFParser ()
  {}


  /*---------------.
  | Symbol types.  |
  `---------------*/



  // by_state.
  inline
  KIFParser::by_state::by_state ()
    : state (empty)
  {}

  inline
  KIFParser::by_state::by_state (const by_state& other)
    : state (other.state)
  {}

  inline
  void
  KIFParser::by_state::move (by_state& that)
  {
    state = that.state;
    that.state = empty;
  }

  inline
  KIFParser::by_state::by_state (state_type s)
    : state (s)
  {}

  inline
  KIFParser::symbol_number_type
  KIFParser::by_state::type_get () const
  {
    return state == empty ? 0 : yystos_[state];
  }

  inline
  KIFParser::stack_symbol_type::stack_symbol_type ()
  {}


  inline
  KIFParser::stack_symbol_type::stack_symbol_type (state_type s, symbol_type& that)
    : super_type (s, that.location)
  {
      switch (that.type_get ())
    {
      case 3: // "opening bracket"
      case 4: // "closing bracket"
      case 5: // "symbol"
      case 6: // "variable"
      case 7: // "number"
      case 8: // "role"
      case 9: // "input"
      case 10: // "init"
      case 11: // "base"
      case 12: // "legal"
      case 13: // "next"
      case 14: // "goal"
      case 15: // "terminal"
      case 16: // "does"
      case 17: // "true"
      case 18: // "not"
      case 19: // "or"
      case 20: // "distinct"
      case 21: // "clause command"
      case 23: // Start
      case 24: // S
      case 25: // Fact
      case 26: // Ground_term
      case 27: // Ground_terms
      case 28: // Id_num
      case 29: // Clause
      case 30: // Head
      case 31: // ORelation
      case 32: // ORelations
      case 33: // Literal
      case 34: // Term
      case 35: // Terms
      case 36: // CTerm
      case 37: // CTerms
      case 38: // Num_var
        value.move< TokenValue > (that.value);
        break;

      default:
        break;
    }

    // that is emptied.
    that.type = empty;
  }

  inline
  KIFParser::stack_symbol_type&
  KIFParser::stack_symbol_type::operator= (const stack_symbol_type& that)
  {
    state = that.state;
      switch (that.type_get ())
    {
      case 3: // "opening bracket"
      case 4: // "closing bracket"
      case 5: // "symbol"
      case 6: // "variable"
      case 7: // "number"
      case 8: // "role"
      case 9: // "input"
      case 10: // "init"
      case 11: // "base"
      case 12: // "legal"
      case 13: // "next"
      case 14: // "goal"
      case 15: // "terminal"
      case 16: // "does"
      case 17: // "true"
      case 18: // "not"
      case 19: // "or"
      case 20: // "distinct"
      case 21: // "clause command"
      case 23: // Start
      case 24: // S
      case 25: // Fact
      case 26: // Ground_term
      case 27: // Ground_terms
      case 28: // Id_num
      case 29: // Clause
      case 30: // Head
      case 31: // ORelation
      case 32: // ORelations
      case 33: // Literal
      case 34: // Term
      case 35: // Terms
      case 36: // CTerm
      case 37: // CTerms
      case 38: // Num_var
        value.copy< TokenValue > (that.value);
        break;

      default:
        break;
    }

    location = that.location;
    return *this;
  }


  template <typename Base>
  inline
  void
  KIFParser::yy_destroy_ (const char* yymsg, basic_symbol<Base>& yysym) const
  {
    if (yymsg)
      YY_SYMBOL_PRINT (yymsg, yysym);
  }

#if YYDEBUG
  template <typename Base>
  void
  KIFParser::yy_print_ (std::ostream& yyo,
                                     const basic_symbol<Base>& yysym) const
  {
    std::ostream& yyoutput = yyo;
    YYUSE (yyoutput);
    symbol_number_type yytype = yysym.type_get ();
    yyo << (yytype < yyntokens_ ? "token" : "nterm")
        << ' ' << yytname_[yytype] << " ("
        << yysym.location << ": ";
    YYUSE (yytype);
    yyo << ')';
  }
#endif

  inline
  void
  KIFParser::yypush_ (const char* m, state_type s, symbol_type& sym)
  {
    stack_symbol_type t (s, sym);
    yypush_ (m, t);
  }

  inline
  void
  KIFParser::yypush_ (const char* m, stack_symbol_type& s)
  {
    if (m)
      YY_SYMBOL_PRINT (m, s);
    yystack_.push (s);
  }

  inline
  void
  KIFParser::yypop_ (unsigned int n)
  {
    yystack_.pop (n);
  }

#if YYDEBUG
  std::ostream&
  KIFParser::debug_stream () const
  {
    return *yycdebug_;
  }

  void
  KIFParser::set_debug_stream (std::ostream& o)
  {
    yycdebug_ = &o;
  }


  KIFParser::debug_level_type
  KIFParser::debug_level () const
  {
    return yydebug_;
  }

  void
  KIFParser::set_debug_level (debug_level_type l)
  {
    yydebug_ = l;
  }
#endif // YYDEBUG

  inline KIFParser::state_type
  KIFParser::yy_lr_goto_state_ (state_type yystate, int yylhs)
  {
    int yyr = yypgoto_[yylhs - yyntokens_] + yystate;
    if (0 <= yyr && yyr <= yylast_ && yycheck_[yyr] == yystate)
      return yytable_[yyr];
    else
      return yydefgoto_[yylhs - yyntokens_];
  }

  inline bool
  KIFParser::yy_pact_value_is_default_ (int yyvalue)
  {
    return yyvalue == yypact_ninf_;
  }

  inline bool
  KIFParser::yy_table_value_is_error_ (int yyvalue)
  {
    return yyvalue == yytable_ninf_;
  }

  int
  KIFParser::parse ()
  {
    /// Whether yyla contains a lookahead.
    bool yyempty = true;

    // State.
    int yyn;
    int yylen = 0;

    // Error handling.
    int yynerrs_ = 0;
    int yyerrstatus_ = 0;

    /// The lookahead symbol.
    symbol_type yyla;

    /// The locations where the error started and ended.
    stack_symbol_type yyerror_range[3];

    /// $$ and @$.
    stack_symbol_type yylhs;

    /// The return value of parse ().
    int yyresult;

    // FIXME: This shoud be completely indented.  It is not yet to
    // avoid gratuitous conflicts when merging into the master branch.
    try
      {
    YYCDEBUG << "Starting parse" << std::endl;


    /* Initialize the stack.  The initial state will be set in
       yynewstate, since the latter expects the semantical and the
       location values to have been already stored, initialize these
       stacks with a primary value.  */
    yystack_.clear ();
    yypush_ (YY_NULL, 0, yyla);

    // A new symbol was pushed on the stack.
  yynewstate:
    YYCDEBUG << "Entering state " << yystack_[0].state << std::endl;

    // Accept?
    if (yystack_[0].state == yyfinal_)
      goto yyacceptlab;

    goto yybackup;

    // Backup.
  yybackup:

    // Try to take a decision without lookahead.
    yyn = yypact_[yystack_[0].state];
    if (yy_pact_value_is_default_ (yyn))
      goto yydefault;

    // Read a lookahead token.
    if (yyempty)
      {
        YYCDEBUG << "Reading a token: ";
        try
          {
            symbol_type yylookahead (yylex ());
            yyla.move (yylookahead);
          }
        catch (const syntax_error& yyexc)
          {
            error (yyexc);
            goto yyerrlab1;
          }
        yyempty = false;
      }
    YY_SYMBOL_PRINT ("Next token is", yyla);

    /* If the proper action on seeing token YYLA.TYPE is to reduce or
       to detect an error, take that action.  */
    yyn += yyla.type_get ();
    if (yyn < 0 || yylast_ < yyn || yycheck_[yyn] != yyla.type_get ())
      goto yydefault;

    // Reduce or error.
    yyn = yytable_[yyn];
    if (yyn <= 0)
      {
        if (yy_table_value_is_error_ (yyn))
          goto yyerrlab;
        yyn = -yyn;
        goto yyreduce;
      }

    // Discard the token being shifted.
    yyempty = true;

    // Count tokens shifted since error; after three, turn off error status.
    if (yyerrstatus_)
      --yyerrstatus_;

    // Shift the lookahead token.
    yypush_ ("Shifting", yyn, yyla);
    goto yynewstate;

  /*-----------------------------------------------------------.
  | yydefault -- do the default action for the current state.  |
  `-----------------------------------------------------------*/
  yydefault:
    yyn = yydefact_[yystack_[0].state];
    if (yyn == 0)
      goto yyerrlab;
    goto yyreduce;

  /*-----------------------------.
  | yyreduce -- Do a reduction.  |
  `-----------------------------*/
  yyreduce:
    yylen = yyr2_[yyn];
    yylhs.state = yy_lr_goto_state_(yystack_[yylen].state, yyr1_[yyn]);
    /* Variants are always initialized to an empty instance of the
       correct type. The default $$=$1 action is NOT applied when using
       variants.  */
      switch (yyr1_[yyn])
    {
      case 3: // "opening bracket"
      case 4: // "closing bracket"
      case 5: // "symbol"
      case 6: // "variable"
      case 7: // "number"
      case 8: // "role"
      case 9: // "input"
      case 10: // "init"
      case 11: // "base"
      case 12: // "legal"
      case 13: // "next"
      case 14: // "goal"
      case 15: // "terminal"
      case 16: // "does"
      case 17: // "true"
      case 18: // "not"
      case 19: // "or"
      case 20: // "distinct"
      case 21: // "clause command"
      case 23: // Start
      case 24: // S
      case 25: // Fact
      case 26: // Ground_term
      case 27: // Ground_terms
      case 28: // Id_num
      case 29: // Clause
      case 30: // Head
      case 31: // ORelation
      case 32: // ORelations
      case 33: // Literal
      case 34: // Term
      case 35: // Terms
      case 36: // CTerm
      case 37: // CTerms
      case 38: // Num_var
        yylhs.value.build< TokenValue > ();
        break;

      default:
        break;
    }


    // Compute the default @$.
    {
      slice<stack_symbol_type, stack_type> slice (yystack_, yylen);
      YYLLOC_DEFAULT (yylhs.location, slice, yylen);
    }

    // Perform the reduction.
    YY_REDUCE_PRINT (yyn);
    try
      {
        switch (yyn)
          {
  case 3:
#line 110 "kif_parser.yy" // lalr1.cc:846
    {
                            driver.AddFact(yystack_[1].value.as< TokenValue > (), yystack_[1].location); yystack_[0].value.as< TokenValue > ();
                        }
#line 639 "kif_parser.tab.cc" // lalr1.cc:846
    break;

  case 4:
#line 113 "kif_parser.yy" // lalr1.cc:846
    {
                            driver.AddClause(yystack_[1].value.as< TokenValue > (), yystack_[1].location); yystack_[0].value.as< TokenValue > ();
                        }
#line 647 "kif_parser.tab.cc" // lalr1.cc:846
    break;

  case 5:
#line 116 "kif_parser.yy" // lalr1.cc:846
    { }
#line 653 "kif_parser.tab.cc" // lalr1.cc:846
    break;

  case 6:
#line 118 "kif_parser.yy" // lalr1.cc:846
    {
                                                        yylhs.value.as< TokenValue > () = yystack_[3].value.as< TokenValue > () + " " + yystack_[2].value.as< TokenValue > () + " " + yystack_[1].value.as< TokenValue > () + " " + yystack_[0].value.as< TokenValue > ();
                                                        yylhs.value.as< TokenValue > ().Type() = TokenValue::Relation;
                                                        yylhs.value.as< TokenValue > ().Command() = yystack_[2].value.as< TokenValue > ().Value();
                                                        yylhs.value.as< TokenValue > ().AddArgument(yystack_[1].value.as< TokenValue > ());
                                                        std::string msg;
                                                        int res = driver.AddEntry(yystack_[2].value.as< TokenValue > (), true, yylhs.value.as< TokenValue > ().Count(), yystack_[2].location, msg);
                                                        if(res == 1) error(yystack_[2].location, msg);
                                                        else if(res == 2) driver.Warn(yystack_[2].location, msg);
                                                    }
#line 668 "kif_parser.tab.cc" // lalr1.cc:846
    break;

  case 7:
#line 128 "kif_parser.yy" // lalr1.cc:846
    {
                                                        yylhs.value.as< TokenValue > () = yystack_[3].value.as< TokenValue > () + " " + yystack_[2].value.as< TokenValue > () + " " + yystack_[1].value.as< TokenValue > () + " " + yystack_[0].value.as< TokenValue > ();
                                                        yylhs.value.as< TokenValue > ().Type() = TokenValue::Relation;
                                                        yylhs.value.as< TokenValue > ().Command() = yystack_[2].value.as< TokenValue > ().Value();
                                                        yylhs.value.as< TokenValue > ().AddArgument(yystack_[1].value.as< TokenValue > ());
                                                        std::string msg;
                                                        int res = driver.AddEntry(yystack_[2].value.as< TokenValue > (), true, yylhs.value.as< TokenValue > ().Count(), yystack_[2].location, msg);
                                                        if(res == 1) error(yystack_[2].location, msg);
                                                        else if(res == 2) driver.Warn(yystack_[2].location, msg);
                                                    }
#line 683 "kif_parser.tab.cc" // lalr1.cc:846
    break;

  case 8:
#line 138 "kif_parser.yy" // lalr1.cc:846
    {
                                                                    yylhs.value.as< TokenValue > () = yystack_[4].value.as< TokenValue > () + " " + yystack_[3].value.as< TokenValue > () + " " + yystack_[2].value.as< TokenValue > () + " " + yystack_[1].value.as< TokenValue > () + " " + yystack_[0].value.as< TokenValue > ();
                                                                    yylhs.value.as< TokenValue > ().Type() = TokenValue::Relation;
                                                                    yylhs.value.as< TokenValue > ().Command() = yystack_[3].value.as< TokenValue > ().Value();
                                                                    yylhs.value.as< TokenValue > ().AddArgument(yystack_[2].value.as< TokenValue > ());
                                                                    yylhs.value.as< TokenValue > ().AddArgument(yystack_[1].value.as< TokenValue > ());
                                                                    std::string msg;
                                                                    int res = driver.AddEntry(yystack_[3].value.as< TokenValue > (), true, yylhs.value.as< TokenValue > ().Count(), yystack_[3].location, msg);
                                                                    if(res == 1) error(yystack_[3].location, msg);
                                                                    else if(res == 2) driver.Warn(yystack_[3].location, msg);
                                                                }
#line 699 "kif_parser.tab.cc" // lalr1.cc:846
    break;

  case 9:
#line 149 "kif_parser.yy" // lalr1.cc:846
    {
                                                        yylhs.value.as< TokenValue > () = yystack_[3].value.as< TokenValue > () + " " + yystack_[2].value.as< TokenValue > () + " " + yystack_[1].value.as< TokenValue > () + " " + yystack_[0].value.as< TokenValue > ();
                                                        yylhs.value.as< TokenValue > ().Type() = TokenValue::Relation;
                                                        yylhs.value.as< TokenValue > ().Command() = yystack_[2].value.as< TokenValue > ().Value();
                                                        yylhs.value.as< TokenValue > ().AddArgument(yystack_[1].value.as< TokenValue > ());
                                                        std::string msg;
                                                        int res = driver.AddEntry(yystack_[2].value.as< TokenValue > (), true, yylhs.value.as< TokenValue > ().Count(), yystack_[2].location, msg);
                                                        if(res == 1) error(yystack_[2].location, msg);
                                                        else if(res == 2) driver.Warn(yystack_[2].location, msg);
                                                    }
#line 714 "kif_parser.tab.cc" // lalr1.cc:846
    break;

  case 10:
#line 159 "kif_parser.yy" // lalr1.cc:846
    {
                                                                    yylhs.value.as< TokenValue > () = yystack_[4].value.as< TokenValue > () + " " + yystack_[3].value.as< TokenValue > () + " " + yystack_[2].value.as< TokenValue > () + " " + yystack_[1].value.as< TokenValue > () + " " + yystack_[0].value.as< TokenValue > ();
                                                                    yylhs.value.as< TokenValue > ().Type() = TokenValue::Relation;
                                                                    yylhs.value.as< TokenValue > ().Command() = yystack_[3].value.as< TokenValue > ().Value();
                                                                    yylhs.value.as< TokenValue > ().AddArgument(yystack_[2].value.as< TokenValue > ());
                                                                    yylhs.value.as< TokenValue > ().AddArgument(yystack_[1].value.as< TokenValue > ());
                                                                    std::string msg;
                                                                    int res = driver.AddEntry(yystack_[3].value.as< TokenValue > (), true, yylhs.value.as< TokenValue > ().Count(), yystack_[3].location, msg);
                                                                    if(res == 1) error(yystack_[3].location, msg);
                                                                    else if(res == 2) driver.Warn(yystack_[3].location, msg);
                                                                }
#line 730 "kif_parser.tab.cc" // lalr1.cc:846
    break;

  case 11:
#line 170 "kif_parser.yy" // lalr1.cc:846
    {
                                                        yylhs.value.as< TokenValue > () = yystack_[3].value.as< TokenValue > () + " " + yystack_[2].value.as< TokenValue > () + " " + yystack_[1].value.as< TokenValue > () + " " + yystack_[0].value.as< TokenValue > ();
                                                        yylhs.value.as< TokenValue > ().Type() = TokenValue::Relation;
                                                        yylhs.value.as< TokenValue > ().Command() = yystack_[2].value.as< TokenValue > ().Value();
                                                        yylhs.value.as< TokenValue > ().AddArgument(yystack_[1].value.as< TokenValue > ());
                                                        std::string msg;
                                                        int res = driver.AddEntry(yystack_[2].value.as< TokenValue > (), true, yylhs.value.as< TokenValue > ().Count(), yystack_[2].location, msg);
                                                        if(res == 1) error(yystack_[2].location, msg);
                                                        else if(res == 2) driver.Warn(yystack_[2].location, msg);
                                                    }
#line 745 "kif_parser.tab.cc" // lalr1.cc:846
    break;

  case 12:
#line 180 "kif_parser.yy" // lalr1.cc:846
    {
                                                            yystack_[1].value.as< TokenValue > ().Command() = yystack_[1].value.as< TokenValue > ().Value();
                                                            yylhs.value.as< TokenValue > () = yystack_[4].value.as< TokenValue > () + " " + yystack_[3].value.as< TokenValue > () + " " + yystack_[2].value.as< TokenValue > () + " " + yystack_[1].value.as< TokenValue > () + " " + yystack_[0].value.as< TokenValue > ();
                                                            yylhs.value.as< TokenValue > ().Type() = TokenValue::Relation;
                                                            yylhs.value.as< TokenValue > ().Command() = yystack_[3].value.as< TokenValue > ().Value();
                                                            yylhs.value.as< TokenValue > ().AddArgument(yystack_[2].value.as< TokenValue > ());
                                                            yylhs.value.as< TokenValue > ().AddArgument(yystack_[1].value.as< TokenValue > ());
                                                            std::string msg;
                                                            int res = driver.AddEntry(yystack_[3].value.as< TokenValue > (), true, yylhs.value.as< TokenValue > ().Count(), yystack_[3].location, msg);
                                                            if(res == 1) error(yystack_[3].location, msg);
                                                            else if(res == 2) driver.Warn(yystack_[3].location, msg);

                                                        }
#line 763 "kif_parser.tab.cc" // lalr1.cc:846
    break;

  case 13:
#line 193 "kif_parser.yy" // lalr1.cc:846
    {
                            yylhs.value.as< TokenValue > () = yylhs.value.as< TokenValue > () + yystack_[0].value.as< TokenValue > ();
                            yylhs.value.as< TokenValue > ().Type() = TokenValue::Relation;
                            yylhs.value.as< TokenValue > ().Command() = yystack_[0].value.as< TokenValue > ().Value();
                            std::string msg;
                            int res = driver.AddEntry(yylhs.value.as< TokenValue > (), true, 0, yystack_[0].location, msg);
                            if(res == 1) error(yystack_[0].location, msg);
                            else if(res == 2) driver.Warn(yystack_[0].location, msg);
                        }
#line 777 "kif_parser.tab.cc" // lalr1.cc:846
    break;

  case 14:
#line 202 "kif_parser.yy" // lalr1.cc:846
    {
                                                                    yylhs.value.as< TokenValue > () = yystack_[4].value.as< TokenValue > () + " " + yystack_[3].value.as< TokenValue > () + " " + yystack_[2].value.as< TokenValue > ();
                                                                    if(yystack_[1].value.as< TokenValue > () != "") yylhs.value.as< TokenValue > () = yylhs.value.as< TokenValue > () + " " + yystack_[1].value.as< TokenValue > ();
                                                                    yylhs.value.as< TokenValue > () = yylhs.value.as< TokenValue > () + " " + yystack_[0].value.as< TokenValue > ();
                                                                    yylhs.value.as< TokenValue > ().Type() = TokenValue::Relation;
                                                                    yylhs.value.as< TokenValue > ().Command() = yystack_[3].value.as< TokenValue > ().Value();
                                                                    yylhs.value.as< TokenValue > ().AddArgument(yystack_[2].value.as< TokenValue > ());
                                                                    yylhs.value.as< TokenValue > ().AddArguments(yystack_[1].value.as< TokenValue > ().Arguments());
                                                                    std::string msg;
                                                                    int res = driver.AddEntry(yystack_[3].value.as< TokenValue > (), true, yylhs.value.as< TokenValue > ().Count(), yystack_[3].location, msg);
                                                                    if(res == 1) error(yystack_[3].location, msg);
                                                                    else if(res == 2) driver.Warn(yystack_[3].location, msg);
                                                                }
#line 795 "kif_parser.tab.cc" // lalr1.cc:846
    break;

  case 15:
#line 215 "kif_parser.yy" // lalr1.cc:846
    {
                            yylhs.value.as< TokenValue > () = yylhs.value.as< TokenValue > () + yystack_[0].value.as< TokenValue > ();
                            yylhs.value.as< TokenValue > ().Type() = TokenValue::Relation;
                            yylhs.value.as< TokenValue > ().Command() = yystack_[0].value.as< TokenValue > ().Value();
                            std::string msg;
                            int res = driver.AddEntry(yylhs.value.as< TokenValue > (), true, 0, yystack_[0].location, msg);
                            if(res == 1) error(yystack_[0].location, msg);
                            else if(res == 2) driver.Warn(yystack_[0].location, msg);
                        }
#line 809 "kif_parser.tab.cc" // lalr1.cc:846
    break;

  case 16:
#line 225 "kif_parser.yy" // lalr1.cc:846
    {
                                                                    yylhs.value.as< TokenValue > () = yystack_[4].value.as< TokenValue > () + " " + yystack_[3].value.as< TokenValue > () + " " + yystack_[2].value.as< TokenValue > ();
                                                                    if(yystack_[1].value.as< TokenValue > () != "") yylhs.value.as< TokenValue > () = yylhs.value.as< TokenValue > () + " " + yystack_[1].value.as< TokenValue > ();
                                                                    yylhs.value.as< TokenValue > () = yylhs.value.as< TokenValue > () + " " + yystack_[0].value.as< TokenValue > ();
                                                                    yylhs.value.as< TokenValue > ().Type() = TokenValue::Function;
                                                                    yylhs.value.as< TokenValue > ().Command() = yystack_[3].value.as< TokenValue > ().Value();
                                                                    yylhs.value.as< TokenValue > ().AddArgument(yystack_[2].value.as< TokenValue > ());
                                                                    yylhs.value.as< TokenValue > ().AddArguments(yystack_[1].value.as< TokenValue > ().Arguments());
                                                                    std::string msg;
                                                                    int res = driver.AddEntry(yystack_[3].value.as< TokenValue > (), false, yylhs.value.as< TokenValue > ().Count(), yystack_[3].location, msg);
                                                                    if(res == 1) error(yystack_[3].location, msg);
                                                                    else if(res == 2) driver.Warn(yystack_[3].location, msg);
                                                                }
#line 827 "kif_parser.tab.cc" // lalr1.cc:846
    break;

  case 17:
#line 238 "kif_parser.yy" // lalr1.cc:846
    {
                            yylhs.value.as< TokenValue > () = yylhs.value.as< TokenValue > () + yystack_[0].value.as< TokenValue > ();
                            yylhs.value.as< TokenValue > ().Type() = TokenValue::Function;
                            yylhs.value.as< TokenValue > ().Command() = yystack_[0].value.as< TokenValue > ().Value();
                            std::string msg;
                            int res = driver.AddEntry(yylhs.value.as< TokenValue > (), false, 0, yystack_[0].location, msg);
                            if(res == 1) error(yystack_[0].location, msg);
                            else if(res == 2) driver.Warn(yystack_[0].location, msg);
                        }
#line 841 "kif_parser.tab.cc" // lalr1.cc:846
    break;

  case 18:
#line 248 "kif_parser.yy" // lalr1.cc:846
    {
                                            yylhs.value.as< TokenValue > () = yylhs.value.as< TokenValue > () + yystack_[1].value.as< TokenValue > ();
                                            if(yystack_[0].value.as< TokenValue > ().Value() != "") yylhs.value.as< TokenValue > () = yylhs.value.as< TokenValue > () + " " + yystack_[0].value.as< TokenValue > ();
                                            yylhs.value.as< TokenValue > ().Command() = "multiple terms";
                                            yylhs.value.as< TokenValue > ().AddArgument(yystack_[1].value.as< TokenValue > ());
                                            yylhs.value.as< TokenValue > ().AddArguments(yystack_[0].value.as< TokenValue > ().Arguments());
                                        }
#line 853 "kif_parser.tab.cc" // lalr1.cc:846
    break;

  case 19:
#line 255 "kif_parser.yy" // lalr1.cc:846
    { }
#line 859 "kif_parser.tab.cc" // lalr1.cc:846
    break;

  case 20:
#line 257 "kif_parser.yy" // lalr1.cc:846
    {
                        yylhs.value.as< TokenValue > () = yylhs.value.as< TokenValue > () + yystack_[0].value.as< TokenValue > ();
                        yylhs.value.as< TokenValue > ().Command() = yystack_[0].value.as< TokenValue > ().Value();
                    }
#line 868 "kif_parser.tab.cc" // lalr1.cc:846
    break;

  case 21:
#line 261 "kif_parser.yy" // lalr1.cc:846
    {
                        yylhs.value.as< TokenValue > () = yylhs.value.as< TokenValue > () + yystack_[0].value.as< TokenValue > ();
                        yylhs.value.as< TokenValue > ().Command() = yystack_[0].value.as< TokenValue > ().Value();
                    }
#line 877 "kif_parser.tab.cc" // lalr1.cc:846
    break;

  case 22:
#line 266 "kif_parser.yy" // lalr1.cc:846
    {
                                                                                yylhs.value.as< TokenValue > () = yystack_[5].value.as< TokenValue > () + " " + yystack_[4].value.as< TokenValue > () + " " + yystack_[3].value.as< TokenValue > ().Value() + " " + yystack_[2].value.as< TokenValue > ();
                                                                                if(yystack_[1].value.as< TokenValue > () != "") yylhs.value.as< TokenValue > () = yylhs.value.as< TokenValue > () + " " + yystack_[1].value.as< TokenValue > ();
                                                                                yylhs.value.as< TokenValue > () = yylhs.value.as< TokenValue > () + " " + yystack_[0].value.as< TokenValue > ();
                                                                                yylhs.value.as< TokenValue > ().HeadRestrict(yystack_[3].value.as< TokenValue > ());
                                                                                yylhs.value.as< TokenValue > ().Command() = yystack_[4].value.as< TokenValue > ().Value();
                                                                                yylhs.value.as< TokenValue > ().AddArgument(yystack_[3].value.as< TokenValue > ());
                                                                                yylhs.value.as< TokenValue > ().AddArgument(yystack_[2].value.as< TokenValue > ());
                                                                                yylhs.value.as< TokenValue > ().AddArguments(yystack_[1].value.as< TokenValue > ().Arguments());
                                                                            }
#line 892 "kif_parser.tab.cc" // lalr1.cc:846
    break;

  case 23:
#line 277 "kif_parser.yy" // lalr1.cc:846
    {
                                                yylhs.value.as< TokenValue > () = yystack_[3].value.as< TokenValue > () + " " + yystack_[2].value.as< TokenValue > () + " " + yystack_[1].value.as< TokenValue > () + " " + yystack_[0].value.as< TokenValue > ();
                                                yylhs.value.as< TokenValue > ().Command() = yystack_[2].value.as< TokenValue > ().Value();
                                                yylhs.value.as< TokenValue > ().Type() = TokenValue::Relation;
                                                yylhs.value.as< TokenValue > ().AddArgument(yystack_[1].value.as< TokenValue > ());
                                                std::string msg;
                                                int res = driver.AddEntry(yystack_[2].value.as< TokenValue > (), true, yylhs.value.as< TokenValue > ().Count(), yystack_[2].location, msg);
                                                if(res == 1) error(yystack_[2].location, msg);
                                                else if(res == 2) driver.Warn(yystack_[2].location, msg);
                                            }
#line 907 "kif_parser.tab.cc" // lalr1.cc:846
    break;

  case 24:
#line 287 "kif_parser.yy" // lalr1.cc:846
    {
                                                yylhs.value.as< TokenValue > () = yystack_[3].value.as< TokenValue > () + " " + yystack_[2].value.as< TokenValue > () + " " + yystack_[1].value.as< TokenValue > () + " " + yystack_[0].value.as< TokenValue > ();
                                                yylhs.value.as< TokenValue > ().Command() = yystack_[2].value.as< TokenValue > ().Value();
                                                yylhs.value.as< TokenValue > ().Type() = TokenValue::Relation;
                                                yylhs.value.as< TokenValue > ().AddArgument(yystack_[1].value.as< TokenValue > ());
                                                std::string msg;
                                                int res = driver.AddEntry(yystack_[2].value.as< TokenValue > (), true, yylhs.value.as< TokenValue > ().Count(), yystack_[2].location, msg);
                                                if(res == 1) error(yystack_[2].location, msg);
                                                else if(res == 2) driver.Warn(yystack_[2].location, msg);
                                            }
#line 922 "kif_parser.tab.cc" // lalr1.cc:846
    break;

  case 25:
#line 297 "kif_parser.yy" // lalr1.cc:846
    {
                                                    yylhs.value.as< TokenValue > () = yystack_[4].value.as< TokenValue > () + " " + yystack_[3].value.as< TokenValue > () + " " + yystack_[2].value.as< TokenValue > () + " " + yystack_[1].value.as< TokenValue > () + " " + yystack_[0].value.as< TokenValue > ();
                                                    yylhs.value.as< TokenValue > ().Command() = yystack_[3].value.as< TokenValue > ().Value();
                                                    yylhs.value.as< TokenValue > ().Type() = TokenValue::Relation;
                                                    yylhs.value.as< TokenValue > ().AddArgument(yystack_[2].value.as< TokenValue > ());
                                                    yylhs.value.as< TokenValue > ().AddArguments(yystack_[1].value.as< TokenValue > ().Arguments());
                                                    std::string msg;
                                                    int res = driver.AddEntry(yystack_[3].value.as< TokenValue > (), true, yylhs.value.as< TokenValue > ().Count(), yystack_[3].location, msg);
                                                    if(res == 1) error(yystack_[3].location, msg);
                                                    else if(res == 2) driver.Warn(yystack_[3].location, msg);
                                                }
#line 938 "kif_parser.tab.cc" // lalr1.cc:846
    break;

  case 26:
#line 308 "kif_parser.yy" // lalr1.cc:846
    {
                                                yylhs.value.as< TokenValue > () = yystack_[3].value.as< TokenValue > () + " " + yystack_[2].value.as< TokenValue > () + " " + yystack_[1].value.as< TokenValue > () + " " + yystack_[0].value.as< TokenValue > ();
                                                yylhs.value.as< TokenValue > ().Command() = yystack_[2].value.as< TokenValue > ().Value();
                                                yylhs.value.as< TokenValue > ().Type() = TokenValue::Relation;
                                                yylhs.value.as< TokenValue > ().AddArgument(yystack_[1].value.as< TokenValue > ());
                                                std::string msg;
                                                int res = driver.AddEntry(yystack_[2].value.as< TokenValue > (), true, yylhs.value.as< TokenValue > ().Count(), yystack_[2].location, msg);
                                                if(res == 1) error(yystack_[2].location, msg);
                                                else if(res == 2) driver.Warn(yystack_[2].location, msg);
                                            }
#line 953 "kif_parser.tab.cc" // lalr1.cc:846
    break;

  case 27:
#line 318 "kif_parser.yy" // lalr1.cc:846
    {
                                                    yylhs.value.as< TokenValue > () = yystack_[4].value.as< TokenValue > () + " " + yystack_[3].value.as< TokenValue > () + " " + yystack_[2].value.as< TokenValue > () + " " + yystack_[1].value.as< TokenValue > () + " " + yystack_[0].value.as< TokenValue > ();
                                                    yylhs.value.as< TokenValue > ().Command() = yystack_[3].value.as< TokenValue > ().Value();
                                                    yylhs.value.as< TokenValue > ().Type() = TokenValue::Relation;
                                                    yylhs.value.as< TokenValue > ().AddArgument(yystack_[2].value.as< TokenValue > ());
                                                    yylhs.value.as< TokenValue > ().AddArguments(yystack_[1].value.as< TokenValue > ().Arguments());
                                                    std::string msg;
                                                    int res = driver.AddEntry(yystack_[3].value.as< TokenValue > (), true, yylhs.value.as< TokenValue > ().Count(), yystack_[3].location, msg);
                                                    if(res == 1) error(yystack_[3].location, msg);
                                                    else if(res == 2) driver.Warn(yystack_[3].location, msg);
                                                }
#line 969 "kif_parser.tab.cc" // lalr1.cc:846
    break;

  case 28:
#line 329 "kif_parser.yy" // lalr1.cc:846
    {
                                                yylhs.value.as< TokenValue > () = yystack_[3].value.as< TokenValue > () + " " + yystack_[2].value.as< TokenValue > () + " " + yystack_[1].value.as< TokenValue > () + " " + yystack_[0].value.as< TokenValue > ();
                                                yylhs.value.as< TokenValue > ().Command() = yystack_[2].value.as< TokenValue > ().Value();
                                                yylhs.value.as< TokenValue > ().Type() = TokenValue::Relation;
                                                yylhs.value.as< TokenValue > ().AddArgument(yystack_[1].value.as< TokenValue > ());
                                                std::string msg;
                                                int res = driver.AddEntry(yystack_[2].value.as< TokenValue > (), true, yylhs.value.as< TokenValue > ().Count(), yystack_[2].location, msg);
                                                if(res == 1) error(yystack_[2].location, msg);
                                                else if(res == 2) driver.Warn(yystack_[2].location, msg);
                                            }
#line 984 "kif_parser.tab.cc" // lalr1.cc:846
    break;

  case 29:
#line 339 "kif_parser.yy" // lalr1.cc:846
    {
                                                        yylhs.value.as< TokenValue > () = yystack_[4].value.as< TokenValue > () + " " + yystack_[3].value.as< TokenValue > () + " " + yystack_[2].value.as< TokenValue > () + " " + yystack_[1].value.as< TokenValue > () + " " + yystack_[0].value.as< TokenValue > ();
                                                        yylhs.value.as< TokenValue > ().Command() = yystack_[3].value.as< TokenValue > ().Value();
                                                        yylhs.value.as< TokenValue > ().Type() = TokenValue::Relation;
                                                        yylhs.value.as< TokenValue > ().AddArgument(yystack_[2].value.as< TokenValue > ());
                                                        yylhs.value.as< TokenValue > ().AddArguments(yystack_[1].value.as< TokenValue > ().Arguments());
                                                        std::string msg;
                                                        int res = driver.AddEntry(yystack_[3].value.as< TokenValue > (), true, yylhs.value.as< TokenValue > ().Count(), yystack_[3].location, msg);
                                                        if(res == 1) error(yystack_[3].location, msg);
                                                        else if(res == 2) driver.Warn(yystack_[3].location, msg);
                                                    }
#line 1000 "kif_parser.tab.cc" // lalr1.cc:846
    break;

  case 30:
#line 350 "kif_parser.yy" // lalr1.cc:846
    {
                            yylhs.value.as< TokenValue > () = yylhs.value.as< TokenValue > () + yystack_[0].value.as< TokenValue > ();
                            yylhs.value.as< TokenValue > ().Command() = yystack_[0].value.as< TokenValue > ().Value();
                            std::string msg;
                            int res = driver.AddEntry(yystack_[0].value.as< TokenValue > (), true, 0, yystack_[0].location, msg);
                            if(res == 1) error(yystack_[0].location, msg);
                            else if(res == 2) driver.Warn(yystack_[0].location, msg);
                        }
#line 1013 "kif_parser.tab.cc" // lalr1.cc:846
    break;

  case 31:
#line 358 "kif_parser.yy" // lalr1.cc:846
    {
                                                        yylhs.value.as< TokenValue > () = yystack_[4].value.as< TokenValue > () + " " + yystack_[3].value.as< TokenValue > () + " " + yystack_[2].value.as< TokenValue > ();
                                                        if(yystack_[1].value.as< TokenValue > () != "") yylhs.value.as< TokenValue > () = yylhs.value.as< TokenValue > () + " " + yystack_[1].value.as< TokenValue > ();
                                                        yylhs.value.as< TokenValue > () = yylhs.value.as< TokenValue > () + " " + yystack_[0].value.as< TokenValue > ();
                                                        yylhs.value.as< TokenValue > ().Command() = yystack_[3].value.as< TokenValue > ().Value();
                                                        yylhs.value.as< TokenValue > ().Type() = TokenValue::Relation;
                                                        yylhs.value.as< TokenValue > ().AddArgument(yystack_[2].value.as< TokenValue > ());
                                                        yylhs.value.as< TokenValue > ().AddArguments(yystack_[1].value.as< TokenValue > ().Arguments());
                                                        std::string msg;
                                                        int res = driver.AddEntry(yystack_[3].value.as< TokenValue > (), true, yylhs.value.as< TokenValue > ().Count(), yystack_[3].location, msg);
                                                        if(res == 1) error(yystack_[3].location, msg);
                                                        else if(res == 2) driver.Warn(yystack_[3].location, msg);
                                                    }
#line 1031 "kif_parser.tab.cc" // lalr1.cc:846
    break;

  case 32:
#line 371 "kif_parser.yy" // lalr1.cc:846
    {
                            yylhs.value.as< TokenValue > () = yylhs.value.as< TokenValue > () + yystack_[0].value.as< TokenValue > ();
                            yylhs.value.as< TokenValue > ().Command() = yystack_[0].value.as< TokenValue > ().Value();
                            yylhs.value.as< TokenValue > ().Type() = TokenValue::Relation;
                            std::string msg;
                            int res = driver.AddEntry(yystack_[0].value.as< TokenValue > (), true, 0, yystack_[0].location, msg);
                            if(res == 1) error(yystack_[0].location, msg);
                            else if(res == 2) driver.Warn(yystack_[0].location, msg);
                        }
#line 1045 "kif_parser.tab.cc" // lalr1.cc:846
    break;

  case 33:
#line 381 "kif_parser.yy" // lalr1.cc:846
    { yylhs.value.as< TokenValue > () = yystack_[0].value.as< TokenValue > (); }
#line 1051 "kif_parser.tab.cc" // lalr1.cc:846
    break;

  case 34:
#line 382 "kif_parser.yy" // lalr1.cc:846
    {
                                                    yylhs.value.as< TokenValue > () = yystack_[3].value.as< TokenValue > () + " " + yystack_[2].value.as< TokenValue > () + " " + yystack_[1].value.as< TokenValue > () + " " + yystack_[0].value.as< TokenValue > ();
                                                    yylhs.value.as< TokenValue > ().PerformNot();
                                                    yylhs.value.as< TokenValue > ().Command() = yystack_[2].value.as< TokenValue > ().Value();
                                                    yylhs.value.as< TokenValue > ().Type() = TokenValue::Relation;
                                                    yylhs.value.as< TokenValue > ().AddArgument(yystack_[1].value.as< TokenValue > ());
                                                }
#line 1063 "kif_parser.tab.cc" // lalr1.cc:846
    break;

  case 35:
#line 389 "kif_parser.yy" // lalr1.cc:846
    {
                                        yylhs.value.as< TokenValue > () = yylhs.value.as< TokenValue > () + yystack_[1].value.as< TokenValue > ();
                                        if(yystack_[0].value.as< TokenValue > () != "") yylhs.value.as< TokenValue > () = yylhs.value.as< TokenValue > () + " " + yystack_[0].value.as< TokenValue > ();
                                        yylhs.value.as< TokenValue > ().Command() = "Multiple relations";
                                        yylhs.value.as< TokenValue > ().AddArgument(yystack_[1].value.as< TokenValue > ());
                                        yylhs.value.as< TokenValue > ().AddArguments(yystack_[0].value.as< TokenValue > ().Arguments());
                                    }
#line 1075 "kif_parser.tab.cc" // lalr1.cc:846
    break;

  case 36:
#line 396 "kif_parser.yy" // lalr1.cc:846
    { }
#line 1081 "kif_parser.tab.cc" // lalr1.cc:846
    break;

  case 37:
#line 399 "kif_parser.yy" // lalr1.cc:846
    {
                                                        yylhs.value.as< TokenValue > () = yystack_[4].value.as< TokenValue > () + " " + yystack_[3].value.as< TokenValue > () + " " + yystack_[2].value.as< TokenValue > () + " " + yystack_[1].value.as< TokenValue > () + " " + yystack_[0].value.as< TokenValue > ();
                                                        yylhs.value.as< TokenValue > ().Command() = yystack_[3].value.as< TokenValue > ().Value();
                                                        yylhs.value.as< TokenValue > ().Type() = TokenValue::Relation;
                                                        yylhs.value.as< TokenValue > ().AddArgument(yystack_[2].value.as< TokenValue > ());
                                                        yylhs.value.as< TokenValue > ().AddArguments(yystack_[1].value.as< TokenValue > ().Arguments());
                                                    }
#line 1093 "kif_parser.tab.cc" // lalr1.cc:846
    break;

  case 38:
#line 406 "kif_parser.yy" // lalr1.cc:846
    {
                                                        yylhs.value.as< TokenValue > () = yystack_[3].value.as< TokenValue > () + " " + yystack_[2].value.as< TokenValue > () + " " + yystack_[1].value.as< TokenValue > () + " " + yystack_[0].value.as< TokenValue > ();
                                                        yylhs.value.as< TokenValue > ().Command() = yystack_[2].value.as< TokenValue > ().Value();
                                                        yylhs.value.as< TokenValue > ().Type() = TokenValue::Relation;
                                                        yylhs.value.as< TokenValue > ().AddArgument(yystack_[1].value.as< TokenValue > ());
                                                    }
#line 1104 "kif_parser.tab.cc" // lalr1.cc:846
    break;

  case 39:
#line 412 "kif_parser.yy" // lalr1.cc:846
    {
                                                                yylhs.value.as< TokenValue > () = yystack_[4].value.as< TokenValue > () + " " + yystack_[3].value.as< TokenValue > () + " " + yystack_[2].value.as< TokenValue > () + " " + yystack_[1].value.as< TokenValue > ().Value() + " " + yystack_[0].value.as< TokenValue > ();
                                                                yylhs.value.as< TokenValue > ().OrRestrict(yystack_[1].value.as< TokenValue > ());
                                                                yylhs.value.as< TokenValue > ().Command() = yystack_[3].value.as< TokenValue > ().Value();
                                                                yylhs.value.as< TokenValue > ().Type() = TokenValue::Relation;
                                                                yylhs.value.as< TokenValue > ().AddArgument(yystack_[2].value.as< TokenValue > ());
                                                                yylhs.value.as< TokenValue > ().AddArguments(yystack_[1].value.as< TokenValue > ().Arguments());
                                                            }
#line 1117 "kif_parser.tab.cc" // lalr1.cc:846
    break;

  case 40:
#line 420 "kif_parser.yy" // lalr1.cc:846
    {
                                                            yylhs.value.as< TokenValue > () = yystack_[4].value.as< TokenValue > () + " " + yystack_[3].value.as< TokenValue > () + " " + yystack_[2].value.as< TokenValue > () + " " + yystack_[1].value.as< TokenValue > () + " " + yystack_[0].value.as< TokenValue > ();
                                                            yylhs.value.as< TokenValue > ().PerformNot();
                                                            yylhs.value.as< TokenValue > ().Command() = yystack_[3].value.as< TokenValue > ().Value();
                                                            yylhs.value.as< TokenValue > ().Type() = TokenValue::Relation;
                                                            yylhs.value.as< TokenValue > ().AddArgument(yystack_[2].value.as< TokenValue > ());
                                                            yylhs.value.as< TokenValue > ().AddArguments(yystack_[1].value.as< TokenValue > ().Arguments());
                                                        }
#line 1130 "kif_parser.tab.cc" // lalr1.cc:846
    break;

  case 41:
#line 428 "kif_parser.yy" // lalr1.cc:846
    {
                                                yylhs.value.as< TokenValue > () = yystack_[3].value.as< TokenValue > () + " " + yystack_[2].value.as< TokenValue > () + " " + yystack_[1].value.as< TokenValue > () + " " + yystack_[0].value.as< TokenValue > ();
                                                yylhs.value.as< TokenValue > ().Command() = yystack_[2].value.as< TokenValue > ().Value();
                                                yylhs.value.as< TokenValue > ().Type() = TokenValue::Relation;
                                                yylhs.value.as< TokenValue > ().AddArgument(yystack_[1].value.as< TokenValue > ());
                                            }
#line 1141 "kif_parser.tab.cc" // lalr1.cc:846
    break;

  case 42:
#line 434 "kif_parser.yy" // lalr1.cc:846
    {
                                                yylhs.value.as< TokenValue > () = yystack_[3].value.as< TokenValue > () + " " + yystack_[2].value.as< TokenValue > () + " " + yystack_[1].value.as< TokenValue > () + " " + yystack_[0].value.as< TokenValue > ();
                                                yylhs.value.as< TokenValue > ().Command() = yystack_[2].value.as< TokenValue > ().Value();
                                                yylhs.value.as< TokenValue > ().Type() = TokenValue::Relation;
                                                yylhs.value.as< TokenValue > ().AddArgument(yystack_[1].value.as< TokenValue > ());
                                            }
#line 1152 "kif_parser.tab.cc" // lalr1.cc:846
    break;

  case 43:
#line 440 "kif_parser.yy" // lalr1.cc:846
    {
                                                        yylhs.value.as< TokenValue > () = yystack_[4].value.as< TokenValue > () + " " + yystack_[3].value.as< TokenValue > () + " " + yystack_[2].value.as< TokenValue > () + " " + yystack_[1].value.as< TokenValue > () + " " + yystack_[0].value.as< TokenValue > ();
                                                        yylhs.value.as< TokenValue > ().Command() = yystack_[3].value.as< TokenValue > ().Value();
                                                        yylhs.value.as< TokenValue > ().Type() = TokenValue::Relation;
                                                        yylhs.value.as< TokenValue > ().AddArgument(yystack_[2].value.as< TokenValue > ());
                                                        yylhs.value.as< TokenValue > ().AddArguments(yystack_[1].value.as< TokenValue > ().Arguments());
                                                    }
#line 1164 "kif_parser.tab.cc" // lalr1.cc:846
    break;

  case 44:
#line 447 "kif_parser.yy" // lalr1.cc:846
    {
                                                        yylhs.value.as< TokenValue > () = yystack_[4].value.as< TokenValue > () + " " + yystack_[3].value.as< TokenValue > () + " " + yystack_[2].value.as< TokenValue > () + " " + yystack_[1].value.as< TokenValue > () + " " + yystack_[0].value.as< TokenValue > ();
                                                        yylhs.value.as< TokenValue > ().Command() = yystack_[3].value.as< TokenValue > ().Value();
                                                        yylhs.value.as< TokenValue > ().Type() = TokenValue::Relation;
                                                        yylhs.value.as< TokenValue > ().AddArgument(yystack_[2].value.as< TokenValue > ());
                                                        yylhs.value.as< TokenValue > ().AddArguments(yystack_[1].value.as< TokenValue > ().Arguments());
                                                    }
#line 1176 "kif_parser.tab.cc" // lalr1.cc:846
    break;

  case 45:
#line 454 "kif_parser.yy" // lalr1.cc:846
    {
                                                        yylhs.value.as< TokenValue > () = yystack_[4].value.as< TokenValue > () + " " + yystack_[3].value.as< TokenValue > () + " " + yystack_[2].value.as< TokenValue > () + " " + yystack_[1].value.as< TokenValue > () + " " + yystack_[0].value.as< TokenValue > ();
                                                        yylhs.value.as< TokenValue > ().Command() = yystack_[3].value.as< TokenValue > ().Value();
                                                        yylhs.value.as< TokenValue > ().Type() = TokenValue::Relation;
                                                        yylhs.value.as< TokenValue > ().AddArgument(yystack_[2].value.as< TokenValue > ());
                                                        yylhs.value.as< TokenValue > ().AddArguments(yystack_[1].value.as< TokenValue > ().Arguments());
                                                    }
#line 1188 "kif_parser.tab.cc" // lalr1.cc:846
    break;

  case 46:
#line 461 "kif_parser.yy" // lalr1.cc:846
    {
                            yylhs.value.as< TokenValue > () = yylhs.value.as< TokenValue > () + yystack_[0].value.as< TokenValue > ();
                            yylhs.value.as< TokenValue > ().Command() = yystack_[0].value.as< TokenValue > ().Value();
                        }
#line 1197 "kif_parser.tab.cc" // lalr1.cc:846
    break;

  case 47:
#line 465 "kif_parser.yy" // lalr1.cc:846
    {
                                                            yylhs.value.as< TokenValue > () = yystack_[4].value.as< TokenValue > () + " " + yystack_[3].value.as< TokenValue > () + " " + yystack_[2].value.as< TokenValue > ();
                                                            if(yystack_[1].value.as< TokenValue > () != "") yylhs.value.as< TokenValue > () = yylhs.value.as< TokenValue > () + " " + yystack_[1].value.as< TokenValue > ();
                                                            yylhs.value.as< TokenValue > () = yylhs.value.as< TokenValue > () + " " + yystack_[0].value.as< TokenValue > ();
                                                            yylhs.value.as< TokenValue > ().Command() = yystack_[3].value.as< TokenValue > ().Value();
                                                            yylhs.value.as< TokenValue > ().Type() = TokenValue::Relation;
                                                            yylhs.value.as< TokenValue > ().AddArgument(yystack_[2].value.as< TokenValue > ());
                                                            yylhs.value.as< TokenValue > ().AddArguments(yystack_[1].value.as< TokenValue > ().Arguments());
                                                            std::string msg;
                                                            int res = driver.CheckEntry(yystack_[3].value.as< TokenValue > (), true, yystack_[1].value.as< TokenValue > ().Count() + 1, yystack_[3].location, msg);
                                                            if(res == 1) error(yystack_[3].location, msg);
                                                            else if(res == 2) driver.Warn(yystack_[3].location, msg);
                                                        }
#line 1215 "kif_parser.tab.cc" // lalr1.cc:846
    break;

  case 48:
#line 478 "kif_parser.yy" // lalr1.cc:846
    {
                            yylhs.value.as< TokenValue > () = yylhs.value.as< TokenValue > () + yystack_[0].value.as< TokenValue > ();
                            yylhs.value.as< TokenValue > ().Command() = yystack_[0].value.as< TokenValue > ().Value();
                            yylhs.value.as< TokenValue > ().Type() = TokenValue::Relation;
                            std::string msg;
                            int res = driver.CheckEntry(yystack_[0].value.as< TokenValue > (), true, 0, yystack_[0].location, msg);
                            if(res == 1) error(yystack_[0].location, msg);
                            else if(res == 2) driver.Warn(yystack_[0].location, msg);
                        }
#line 1229 "kif_parser.tab.cc" // lalr1.cc:846
    break;

  case 49:
#line 489 "kif_parser.yy" // lalr1.cc:846
    {
                                                        yylhs.value.as< TokenValue > () = yystack_[4].value.as< TokenValue > () + " " + yystack_[3].value.as< TokenValue > () + " " + yystack_[2].value.as< TokenValue > ();
                                                        if(yystack_[1].value.as< TokenValue > () != "") yylhs.value.as< TokenValue > () = yylhs.value.as< TokenValue > () + " " + yystack_[1].value.as< TokenValue > ();
                                                        yylhs.value.as< TokenValue > () = yylhs.value.as< TokenValue > () + " " + yystack_[0].value.as< TokenValue > ();
                                                        yylhs.value.as< TokenValue > ().Command() = yystack_[3].value.as< TokenValue > ().Value();
                                                        yylhs.value.as< TokenValue > ().Type() = TokenValue::Function;
                                                        yylhs.value.as< TokenValue > ().AddArgument(yystack_[2].value.as< TokenValue > ());
                                                        yylhs.value.as< TokenValue > ().AddArguments(yystack_[1].value.as< TokenValue > ().Arguments());
                                                        std::string msg;
                                                        int res = driver.AddEntry(yystack_[3].value.as< TokenValue > (), false, yylhs.value.as< TokenValue > ().Count(), yystack_[3].location, msg);
                                                        if(res == 1) error(yystack_[3].location, msg);
                                                        else if(res == 2) driver.Warn(yystack_[3].location, msg);
                                                    }
#line 1247 "kif_parser.tab.cc" // lalr1.cc:846
    break;

  case 50:
#line 502 "kif_parser.yy" // lalr1.cc:846
    {
                            yylhs.value.as< TokenValue > () = yylhs.value.as< TokenValue > () + yystack_[0].value.as< TokenValue > ();
                            yylhs.value.as< TokenValue > ().Command() = yystack_[0].value.as< TokenValue > ().Value();
                            yylhs.value.as< TokenValue > ().Type() = TokenValue::Function;
                            std::string msg;
                            int res = driver.AddEntry(yystack_[0].value.as< TokenValue > (), false, 0, yystack_[0].location, msg);
                            if(res == 1) error(yystack_[0].location, msg);
                            else if(res == 2) driver.Warn(yystack_[0].location, msg);
                        }
#line 1261 "kif_parser.tab.cc" // lalr1.cc:846
    break;

  case 51:
#line 511 "kif_parser.yy" // lalr1.cc:846
    {
                        yylhs.value.as< TokenValue > () = yylhs.value.as< TokenValue > () + yystack_[0].value.as< TokenValue > ();
                        yylhs.value.as< TokenValue > ().AddVariable(yystack_[0].value.as< TokenValue > ());
                        yylhs.value.as< TokenValue > ().Command() = yystack_[0].value.as< TokenValue > ().Value();
                        yylhs.value.as< TokenValue > ().Type() = TokenValue::Var;
                    }
#line 1272 "kif_parser.tab.cc" // lalr1.cc:846
    break;

  case 52:
#line 518 "kif_parser.yy" // lalr1.cc:846
    {
                                yylhs.value.as< TokenValue > () = yylhs.value.as< TokenValue > () + yystack_[1].value.as< TokenValue > ();
                                if(yystack_[0].value.as< TokenValue > () != "") yylhs.value.as< TokenValue > () = yylhs.value.as< TokenValue > () + " " + yystack_[0].value.as< TokenValue > ();
                                yylhs.value.as< TokenValue > ().Command() = "Multiple terms";
                                yylhs.value.as< TokenValue > ().AddArgument(yystack_[1].value.as< TokenValue > ());
                                yylhs.value.as< TokenValue > ().AddArguments(yystack_[0].value.as< TokenValue > ().Arguments());
                            }
#line 1284 "kif_parser.tab.cc" // lalr1.cc:846
    break;

  case 53:
#line 526 "kif_parser.yy" // lalr1.cc:846
    { }
#line 1290 "kif_parser.tab.cc" // lalr1.cc:846
    break;

  case 54:
#line 528 "kif_parser.yy" // lalr1.cc:846
    {
                                                        yylhs.value.as< TokenValue > () = yystack_[4].value.as< TokenValue > () + " " + yystack_[3].value.as< TokenValue > () + " " + yystack_[2].value.as< TokenValue > ();
                                                        if(yystack_[1].value.as< TokenValue > () != "") yylhs.value.as< TokenValue > () = yylhs.value.as< TokenValue > () + " " + yystack_[1].value.as< TokenValue > ();
                                                        yylhs.value.as< TokenValue > () = yylhs.value.as< TokenValue > () + " " + yystack_[0].value.as< TokenValue > ();
                                                        yylhs.value.as< TokenValue > ().Command() = yystack_[3].value.as< TokenValue > ().Value();
                                                        yylhs.value.as< TokenValue > ().Type() = TokenValue::Function;
                                                        yylhs.value.as< TokenValue > ().AddArgument(yystack_[2].value.as< TokenValue > ());
                                                        yylhs.value.as< TokenValue > ().AddArguments(yystack_[1].value.as< TokenValue > ().Arguments());
                                                        std::string msg;
                                                        int res = driver.CheckEntry(yystack_[3].value.as< TokenValue > (), false, yylhs.value.as< TokenValue > ().Count(), yystack_[3].location, msg);
                                                        if(res == 1) error(yystack_[3].location, msg);
                                                        else if(res == 2) driver.Warn(yystack_[3].location, msg);
                                                    }
#line 1308 "kif_parser.tab.cc" // lalr1.cc:846
    break;

  case 55:
#line 541 "kif_parser.yy" // lalr1.cc:846
    {
                            yylhs.value.as< TokenValue > () = yylhs.value.as< TokenValue > () + yystack_[0].value.as< TokenValue > ();
                            yylhs.value.as< TokenValue > ().Command() = yystack_[0].value.as< TokenValue > ().Value();
                            yylhs.value.as< TokenValue > ().Type() = TokenValue::Function;
                            std::string msg;
                            int res = driver.CheckEntry(yystack_[0].value.as< TokenValue > (), false, 0, yystack_[0].location, msg);
                            if(res == 1) error(yystack_[0].location, msg);
                            else if(res == 2) driver.Warn(yystack_[0].location, msg);
                        }
#line 1322 "kif_parser.tab.cc" // lalr1.cc:846
    break;

  case 56:
#line 550 "kif_parser.yy" // lalr1.cc:846
    {
                        yylhs.value.as< TokenValue > () = yylhs.value.as< TokenValue > () + yystack_[0].value.as< TokenValue > ();
                        yylhs.value.as< TokenValue > ().AddVariable(yystack_[0].value.as< TokenValue > ());
                        yylhs.value.as< TokenValue > ().Command() = yystack_[0].value.as< TokenValue > ().Value();
                        yylhs.value.as< TokenValue > ().Type() = TokenValue::Var;
                    }
#line 1333 "kif_parser.tab.cc" // lalr1.cc:846
    break;

  case 57:
#line 557 "kif_parser.yy" // lalr1.cc:846
    {
                                yylhs.value.as< TokenValue > () = yylhs.value.as< TokenValue > () + yystack_[1].value.as< TokenValue > ();
                                if(yystack_[0].value.as< TokenValue > ().Value() != "") yylhs.value.as< TokenValue > () = yylhs.value.as< TokenValue > () + " " + yystack_[0].value.as< TokenValue > ();
                                yylhs.value.as< TokenValue > ().Command() = "Multiple terms";
                                yylhs.value.as< TokenValue > ().Type() = TokenValue::Function;
                                yylhs.value.as< TokenValue > ().AddArgument(yystack_[1].value.as< TokenValue > ());
                                yylhs.value.as< TokenValue > ().AddArguments(yystack_[0].value.as< TokenValue > ().Arguments());
                            }
#line 1346 "kif_parser.tab.cc" // lalr1.cc:846
    break;

  case 58:
#line 566 "kif_parser.yy" // lalr1.cc:846
    { }
#line 1352 "kif_parser.tab.cc" // lalr1.cc:846
    break;

  case 59:
#line 568 "kif_parser.yy" // lalr1.cc:846
    {
                        yylhs.value.as< TokenValue > () = yylhs.value.as< TokenValue > () + yystack_[0].value.as< TokenValue > ();
                        yylhs.value.as< TokenValue > ().Command() = yystack_[0].value.as< TokenValue > ().Value();
                        yylhs.value.as< TokenValue > ().Type() = TokenValue::Function;
                    }
#line 1362 "kif_parser.tab.cc" // lalr1.cc:846
    break;

  case 60:
#line 573 "kif_parser.yy" // lalr1.cc:846
    {
                        yylhs.value.as< TokenValue > () = yylhs.value.as< TokenValue > () + yystack_[0].value.as< TokenValue > ();
                        yylhs.value.as< TokenValue > ().AddVariable(yystack_[0].value.as< TokenValue > ());
                        yylhs.value.as< TokenValue > ().Type() = TokenValue::Var;
                    }
#line 1372 "kif_parser.tab.cc" // lalr1.cc:846
    break;


#line 1376 "kif_parser.tab.cc" // lalr1.cc:846
          default:
            break;
          }
      }
    catch (const syntax_error& yyexc)
      {
        error (yyexc);
        YYERROR;
      }
    YY_SYMBOL_PRINT ("-> $$ =", yylhs);
    yypop_ (yylen);
    yylen = 0;
    YY_STACK_PRINT ();

    // Shift the result of the reduction.
    yypush_ (YY_NULL, yylhs);
    goto yynewstate;

  /*--------------------------------------.
  | yyerrlab -- here on detecting error.  |
  `--------------------------------------*/
  yyerrlab:
    // If not already recovering from an error, report this error.
    if (!yyerrstatus_)
      {
        ++yynerrs_;
        error (yyla.location, yysyntax_error_ (yystack_[0].state,
                                           yyempty ? yyempty_ : yyla.type_get ()));
      }


    yyerror_range[1].location = yyla.location;
    if (yyerrstatus_ == 3)
      {
        /* If just tried and failed to reuse lookahead token after an
           error, discard it.  */

        // Return failure if at end of input.
        if (yyla.type_get () == yyeof_)
          YYABORT;
        else if (!yyempty)
          {
            yy_destroy_ ("Error: discarding", yyla);
            yyempty = true;
          }
      }

    // Else will try to reuse lookahead token after shifting the error token.
    goto yyerrlab1;


  /*---------------------------------------------------.
  | yyerrorlab -- error raised explicitly by YYERROR.  |
  `---------------------------------------------------*/
  yyerrorlab:

    /* Pacify compilers like GCC when the user code never invokes
       YYERROR and the label yyerrorlab therefore never appears in user
       code.  */
    if (false)
      goto yyerrorlab;
    yyerror_range[1].location = yystack_[yylen - 1].location;
    /* $$ was initialized before running the user action.  */
    YY_SYMBOL_PRINT ("Error: discarding", yylhs);
    yylhs.~stack_symbol_type();
    /* Do not reclaim the symbols of the rule whose action triggered
       this YYERROR.  */
    yypop_ (yylen);
    yylen = 0;
    goto yyerrlab1;

  /*-------------------------------------------------------------.
  | yyerrlab1 -- common code for both syntax error and YYERROR.  |
  `-------------------------------------------------------------*/
  yyerrlab1:
    yyerrstatus_ = 3;   // Each real token shifted decrements this.
    {
      stack_symbol_type error_token;
      for (;;)
        {
          yyn = yypact_[yystack_[0].state];
          if (!yy_pact_value_is_default_ (yyn))
            {
              yyn += yyterror_;
              if (0 <= yyn && yyn <= yylast_ && yycheck_[yyn] == yyterror_)
                {
                  yyn = yytable_[yyn];
                  if (0 < yyn)
                    break;
                }
            }

          // Pop the current state because it cannot handle the error token.
          if (yystack_.size () == 1)
            YYABORT;

          yyerror_range[1].location = yystack_[0].location;
          yy_destroy_ ("Error: popping", yystack_[0]);
          yypop_ ();
          YY_STACK_PRINT ();
        }

      yyerror_range[2].location = yyla.location;
      YYLLOC_DEFAULT (error_token.location, yyerror_range, 2);

      // Shift the error token.
      error_token.state = yyn;
      yypush_ ("Shifting", error_token);
    }
    goto yynewstate;

    // Accept.
  yyacceptlab:
    yyresult = 0;
    goto yyreturn;

    // Abort.
  yyabortlab:
    yyresult = 1;
    goto yyreturn;

  yyreturn:
    if (!yyempty)
      yy_destroy_ ("Cleanup: discarding lookahead", yyla);

    /* Do not reclaim the symbols of the rule whose action triggered
       this YYABORT or YYACCEPT.  */
    yypop_ (yylen);
    while (1 < yystack_.size ())
      {
        yy_destroy_ ("Cleanup: popping", yystack_[0]);
        yypop_ ();
      }

    return yyresult;
  }
    catch (...)
      {
        YYCDEBUG << "Exception caught: cleaning lookahead and stack"
                 << std::endl;
        // Do not try to display the values of the reclaimed symbols,
        // as their printer might throw an exception.
        if (!yyempty)
          yy_destroy_ (YY_NULL, yyla);

        while (1 < yystack_.size ())
          {
            yy_destroy_ (YY_NULL, yystack_[0]);
            yypop_ ();
          }
        throw;
      }
  }

  void
  KIFParser::error (const syntax_error& yyexc)
  {
    error (yyexc.location, yyexc.what());
  }

  // Generate an error message.
  std::string
  KIFParser::yysyntax_error_ (state_type yystate, symbol_number_type yytoken) const
  {
    std::string yyres;
    // Number of reported tokens (one for the "unexpected", one per
    // "expected").
    size_t yycount = 0;
    // Its maximum.
    enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
    // Arguments of yyformat.
    char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];

    /* There are many possibilities here to consider:
       - If this state is a consistent state with a default action, then
         the only way this function was invoked is if the default action
         is an error action.  In that case, don't check for expected
         tokens because there are none.
       - The only way there can be no lookahead present (in yytoken) is
         if this state is a consistent state with a default action.
         Thus, detecting the absence of a lookahead is sufficient to
         determine that there is no unexpected or expected token to
         report.  In that case, just report a simple "syntax error".
       - Don't assume there isn't a lookahead just because this state is
         a consistent state with a default action.  There might have
         been a previous inconsistent state, consistent state with a
         non-default action, or user semantic action that manipulated
         yyla.  (However, yyla is currently not documented for users.)
       - Of course, the expected token list depends on states to have
         correct lookahead information, and it depends on the parser not
         to perform extra reductions after fetching a lookahead from the
         scanner and before detecting a syntax error.  Thus, state
         merging (from LALR or IELR) and default reductions corrupt the
         expected token list.  However, the list is correct for
         canonical LR with one exception: it will still contain any
         token that will not be accepted due to an error action in a
         later state.
    */
    if (yytoken != yyempty_)
      {
        yyarg[yycount++] = yytname_[yytoken];
        int yyn = yypact_[yystate];
        if (!yy_pact_value_is_default_ (yyn))
          {
            /* Start YYX at -YYN if negative to avoid negative indexes in
               YYCHECK.  In other words, skip the first -YYN actions for
               this state because they are default actions.  */
            int yyxbegin = yyn < 0 ? -yyn : 0;
            // Stay within bounds of both yycheck and yytname.
            int yychecklim = yylast_ - yyn + 1;
            int yyxend = yychecklim < yyntokens_ ? yychecklim : yyntokens_;
            for (int yyx = yyxbegin; yyx < yyxend; ++yyx)
              if (yycheck_[yyx + yyn] == yyx && yyx != yyterror_
                  && !yy_table_value_is_error_ (yytable_[yyx + yyn]))
                {
                  if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
                    {
                      yycount = 1;
                      break;
                    }
                  else
                    yyarg[yycount++] = yytname_[yyx];
                }
          }
      }

    char const* yyformat = YY_NULL;
    switch (yycount)
      {
#define YYCASE_(N, S)                         \
        case N:                               \
          yyformat = S;                       \
        break
        YYCASE_(0, YY_("syntax error"));
        YYCASE_(1, YY_("syntax error, unexpected %s"));
        YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
        YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
        YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
        YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
#undef YYCASE_
      }

    // Argument number.
    size_t yyi = 0;
    for (char const* yyp = yyformat; *yyp; ++yyp)
      if (yyp[0] == '%' && yyp[1] == 's' && yyi < yycount)
        {
          yyres += yytnamerr_ (yyarg[yyi++]);
          ++yyp;
        }
      else
        yyres += *yyp;
    return yyres;
  }


  const short int KIFParser::yypact_ninf_ = -134;

  const signed char KIFParser::yytable_ninf_ = -1;

  const short int
  KIFParser::yypact_[] =
  {
      52,    97,  -134,  -134,  -134,     4,    43,    52,  -134,    52,
      14,    23,    23,    23,    23,    23,    23,    58,    23,  -134,
    -134,  -134,  -134,    62,    14,    23,  -134,    65,    66,    23,
      68,    20,   191,  -134,  -134,    72,    23,  -134,    23,    77,
    -134,  -134,    80,  -134,    81,    34,    34,    34,    34,    34,
      34,    34,    34,   159,  -134,  -134,    72,  -134,    23,    85,
      23,  -134,  -134,  -134,    14,  -134,  -134,    86,    34,    88,
     108,    34,   110,    27,    34,   138,   138,   138,   138,   138,
     138,   138,    73,    72,   138,   138,    72,   113,  -134,  -134,
     116,    34,  -134,   125,  -134,  -134,   126,  -134,  -134,  -134,
     142,    34,   144,    14,  -134,  -134,   145,   138,   146,   138,
      27,   138,   148,   175,   149,    72,   138,   138,  -134,  -134,
    -134,    34,  -134,  -134,  -134,  -134,  -134,   138,  -134,   151,
    -134,   153,   155,   157,  -134,  -134,   158,   161,   138,   165,
     168,   138,  -134,  -134,  -134,  -134,  -134,  -134,  -134,  -134,
    -134,   177,  -134
  };

  const unsigned char
  KIFParser::yydefact_[] =
  {
       5,     0,    20,    21,    13,     0,     0,     5,    15,     5,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     1,
       2,     3,     4,     0,     0,     0,    17,     0,     0,     0,
       0,     0,     0,    30,    32,     0,    19,     6,     0,     0,
       9,     7,     0,    11,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    46,    48,    36,    33,    19,     0,
      19,     8,    10,    12,     0,    51,    50,     0,     0,     0,
       0,     0,     0,     0,    53,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    36,     0,    18,    14,
       0,     0,    23,     0,    26,    24,     0,    28,    60,    59,
       0,    53,     0,     0,    56,    55,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    36,     0,    58,    35,    22,
      16,    53,    25,    27,    29,    52,    31,     0,    41,     0,
      42,     0,     0,     0,    38,    34,     0,     0,    58,     0,
       0,    58,    43,    44,    45,    37,    39,    40,    57,    47,
      49,     0,    54
  };

  const short int
  KIFParser::yypgoto_[] =
  {
    -134,  -134,    13,  -134,   122,   -16,     0,  -134,  -134,   -29,
     -84,   -20,    48,   -98,    47,  -133,    32
  };

  const short int
  KIFParser::yydefgoto_[] =
  {
      -1,     5,     6,     7,    58,    59,   105,     9,    35,    86,
      87,    57,   101,   102,   138,   139,   100
  };

  const unsigned char
  KIFParser::yytable_[] =
  {
       8,    18,   118,   125,    19,   148,    56,     8,   151,     8,
      23,    26,    26,    26,    26,    26,    26,    34,    26,     2,
      21,     3,    22,   140,    38,    26,    24,    44,     2,    26,
       3,   136,    52,    98,    99,    55,    26,    64,    26,     2,
      65,     3,    88,    20,    90,    66,    66,    66,    66,    66,
      66,    66,    66,    85,   115,     1,    55,     2,    26,     3,
      26,    32,   114,     2,    91,     3,    37,     4,    66,    40,
      41,    66,    43,    33,    66,    53,   113,     2,     2,     3,
       3,    61,    55,    55,    62,    63,    55,    54,    54,    89,
      92,    66,    94,    67,    68,    69,    70,    71,    72,    73,
      74,    66,     2,   127,     3,    10,    11,    12,    13,    14,
      15,    16,    95,    85,    97,    55,    93,   119,    17,    96,
     120,    66,   106,   107,   108,   109,   110,   111,   112,   122,
     123,   116,   117,    25,    27,    28,    29,    30,    31,   121,
      36,   103,   132,     2,   104,     3,   124,    39,   126,   128,
     130,    42,   134,   135,   129,   142,   131,   143,   133,   144,
      60,   145,   146,   137,     2,   147,     3,    75,    76,   149,
      77,    78,   150,    79,   141,    80,    81,    82,    83,    84,
       2,   152,     3,    75,    76,     0,    77,    78,     0,    79,
       0,    80,    81,     0,    83,    84,     2,     0,     3,    45,
      46,    47,    48,    49,    50,    51
  };

  const short int
  KIFParser::yycheck_[] =
  {
       0,     1,    86,   101,     0,   138,    35,     7,   141,     9,
      10,    11,    12,    13,    14,    15,    16,    17,    18,     5,
       7,     7,     9,   121,    24,    25,     3,     7,     5,    29,
       7,   115,    32,     6,     7,    35,    36,     3,    38,     5,
       6,     7,    58,     0,    60,    45,    46,    47,    48,    49,
      50,    51,    52,    53,    83,     3,    56,     5,    58,     7,
      60,     3,    82,     5,    64,     7,     4,    15,    68,     4,
       4,    71,     4,    15,    74,     3,     3,     5,     5,     7,
       7,     4,    82,    83,     4,     4,    86,    15,    15,     4,
       4,    91,     4,    45,    46,    47,    48,    49,    50,    51,
      52,   101,     5,   103,     7,     8,     9,    10,    11,    12,
      13,    14,     4,   113,     4,   115,    68,     4,    21,    71,
       4,   121,    75,    76,    77,    78,    79,    80,    81,     4,
       4,    84,    85,    11,    12,    13,    14,    15,    16,    91,
      18,     3,   110,     5,     6,     7,     4,    25,     4,     4,
       4,    29,     4,     4,   107,     4,   109,     4,   111,     4,
      38,     4,     4,   116,     5,     4,     7,     8,     9,     4,
      11,    12,     4,    14,   127,    16,    17,    18,    19,    20,
       5,     4,     7,     8,     9,    -1,    11,    12,    -1,    14,
      -1,    16,    17,    -1,    19,    20,     5,    -1,     7,     8,
       9,    10,    11,    12,    13,    14
  };

  const unsigned char
  KIFParser::yystos_[] =
  {
       0,     3,     5,     7,    15,    23,    24,    25,    28,    29,
       8,     9,    10,    11,    12,    13,    14,    21,    28,     0,
       0,    24,    24,    28,     3,    26,    28,    26,    26,    26,
      26,    26,     3,    15,    28,    30,    26,     4,    28,    26,
       4,     4,    26,     4,     7,     8,     9,    10,    11,    12,
      13,    14,    28,     3,    15,    28,    31,    33,    26,    27,
      26,     4,     4,     4,     3,     6,    28,    34,    34,    34,
      34,    34,    34,    34,    34,     8,     9,    11,    12,    14,
      16,    17,    18,    19,    20,    28,    31,    32,    27,     4,
      27,    28,     4,    34,     4,     4,    34,     4,     6,     7,
      38,    34,    35,     3,     6,    28,    36,    36,    36,    36,
      36,    36,    36,     3,    33,    31,    36,    36,    32,     4,
       4,    34,     4,     4,     4,    35,     4,    28,     4,    36,
       4,    36,    38,    36,     4,     4,    32,    36,    36,    37,
      35,    36,     4,     4,     4,     4,     4,     4,    37,     4,
       4,    37,     4
  };

  const unsigned char
  KIFParser::yyr1_[] =
  {
       0,    22,    23,    24,    24,    24,    25,    25,    25,    25,
      25,    25,    25,    25,    25,    25,    26,    26,    27,    27,
      28,    28,    29,    30,    30,    30,    30,    30,    30,    30,
      30,    30,    30,    31,    31,    32,    32,    33,    33,    33,
      33,    33,    33,    33,    33,    33,    33,    33,    33,    34,
      34,    34,    35,    35,    36,    36,    36,    37,    37,    38,
      38
  };

  const unsigned char
  KIFParser::yyr2_[] =
  {
       0,     2,     2,     2,     2,     0,     4,     4,     5,     4,
       5,     4,     5,     1,     5,     1,     5,     1,     2,     0,
       1,     1,     6,     4,     4,     5,     4,     5,     4,     5,
       1,     5,     1,     1,     4,     2,     0,     5,     4,     5,
       5,     4,     4,     5,     5,     5,     1,     5,     1,     5,
       1,     1,     2,     0,     5,     1,     1,     2,     0,     1,
       1
  };



  // YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
  // First, the terminals, then, starting at \a yyntokens_, nonterminals.
  const char*
  const KIFParser::yytname_[] =
  {
  "\"end of file\"", "error", "$undefined", "\"opening bracket\"",
  "\"closing bracket\"", "\"symbol\"", "\"variable\"", "\"number\"",
  "\"role\"", "\"input\"", "\"init\"", "\"base\"", "\"legal\"", "\"next\"",
  "\"goal\"", "\"terminal\"", "\"does\"", "\"true\"", "\"not\"", "\"or\"",
  "\"distinct\"", "\"clause command\"", "$accept", "Start", "S", "Fact",
  "Ground_term", "Ground_terms", "Id_num", "Clause", "Head", "ORelation",
  "ORelations", "Literal", "Term", "Terms", "CTerm", "CTerms", "Num_var", YY_NULL
  };

#if YYDEBUG
  const unsigned short int
  KIFParser::yyrline_[] =
  {
       0,   108,   108,   110,   113,   116,   118,   128,   138,   149,
     159,   170,   180,   193,   202,   215,   225,   238,   248,   255,
     257,   261,   266,   277,   287,   297,   308,   318,   329,   339,
     350,   358,   371,   381,   382,   389,   396,   399,   406,   412,
     420,   428,   434,   440,   447,   454,   461,   465,   478,   489,
     502,   511,   518,   526,   528,   541,   550,   557,   566,   568,
     573
  };

  // Print the state stack on the debug stream.
  void
  KIFParser::yystack_print_ ()
  {
    *yycdebug_ << "Stack now";
    for (stack_type::const_iterator
           i = yystack_.begin (),
           i_end = yystack_.end ();
         i != i_end; ++i)
      *yycdebug_ << ' ' << i->state;
    *yycdebug_ << std::endl;
  }

  // Report on the debug stream that the rule \a yyrule is going to be reduced.
  void
  KIFParser::yy_reduce_print_ (int yyrule)
  {
    unsigned int yylno = yyrline_[yyrule];
    int yynrhs = yyr2_[yyrule];
    // Print the symbols being reduced, and their result.
    *yycdebug_ << "Reducing stack by rule " << yyrule - 1
               << " (line " << yylno << "):" << std::endl;
    // The symbols being reduced.
    for (int yyi = 0; yyi < yynrhs; yyi++)
      YY_SYMBOL_PRINT ("   $" << yyi + 1 << " =",
                       yystack_[(yynrhs) - (yyi + 1)]);
  }
#endif // YYDEBUG


#line 12 "kif_parser.yy" // lalr1.cc:1156
} } } // gdlparser::parser::yy
#line 1851 "kif_parser.tab.cc" // lalr1.cc:1156
#line 579 "kif_parser.yy" // lalr1.cc:1157


  typedef gdlparser::parser::yy::KIFParser KIFParser;

  // Mandatory error function
  void KIFParser::error (const KIFParser::location_type& loc, const std::string& msg)
  {
    driver.Error(loc, msg);
  }
