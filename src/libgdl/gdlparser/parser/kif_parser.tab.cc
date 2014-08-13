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
#line 49 "kif_parser.yy" // lalr1.cc:407

#include <algorithm>
#include <iostream>
#include <iterator>
#include <sstream>

// driver class declaration.
#include "kif_driver.hpp"

// use yylex function in scanner class instead of predefiend yylex
#undef yylex
#define yylex scanner.lex


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
namespace libgdl { namespace gdlparser { namespace parser { namespace yy {
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
      case 22: // "line mark"
      case 24: // Start
      case 25: // S
      case 26: // Fact
      case 27: // Ground_term
      case 28: // Ground_terms
      case 29: // Id_num
      case 30: // Clause
      case 31: // Head
      case 32: // ORelation
      case 33: // ORelations
      case 34: // Literal
      case 35: // Term
      case 36: // Terms
      case 37: // Basic_term
      case 38: // CTerm
      case 39: // CTerms
      case 40: // Basic_cterm
      case 41: // Num_var
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
      case 22: // "line mark"
      case 24: // Start
      case 25: // S
      case 26: // Fact
      case 27: // Ground_term
      case 28: // Ground_terms
      case 29: // Id_num
      case 30: // Clause
      case 31: // Head
      case 32: // ORelation
      case 33: // ORelations
      case 34: // Literal
      case 35: // Term
      case 36: // Terms
      case 37: // Basic_term
      case 38: // CTerm
      case 39: // CTerms
      case 40: // Basic_cterm
      case 41: // Num_var
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
      case 22: // "line mark"
      case 24: // Start
      case 25: // S
      case 26: // Fact
      case 27: // Ground_term
      case 28: // Ground_terms
      case 29: // Id_num
      case 30: // Clause
      case 31: // Head
      case 32: // ORelation
      case 33: // ORelations
      case 34: // Literal
      case 35: // Term
      case 36: // Terms
      case 37: // Basic_term
      case 38: // CTerm
      case 39: // CTerms
      case 40: // Basic_cterm
      case 41: // Num_var
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
#line 115 "kif_parser.yy" // lalr1.cc:846
    {
                            driver.AddFact(yystack_[1].value.as< TokenValue > (), yystack_[1].location); yystack_[0].value.as< TokenValue > ();
                        }
#line 648 "kif_parser.tab.cc" // lalr1.cc:846
    break;

  case 4:
#line 118 "kif_parser.yy" // lalr1.cc:846
    {
                            driver.AddClause(yystack_[1].value.as< TokenValue > (), yystack_[1].location); yystack_[0].value.as< TokenValue > ();
                        }
#line 656 "kif_parser.tab.cc" // lalr1.cc:846
    break;

  case 5:
#line 121 "kif_parser.yy" // lalr1.cc:846
    {
                                driver.AddLineMark(yystack_[1].value.as< TokenValue > ());
                            }
#line 664 "kif_parser.tab.cc" // lalr1.cc:846
    break;

  case 6:
#line 124 "kif_parser.yy" // lalr1.cc:846
    { }
#line 670 "kif_parser.tab.cc" // lalr1.cc:846
    break;

  case 7:
#line 126 "kif_parser.yy" // lalr1.cc:846
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
#line 685 "kif_parser.tab.cc" // lalr1.cc:846
    break;

  case 8:
#line 136 "kif_parser.yy" // lalr1.cc:846
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
#line 700 "kif_parser.tab.cc" // lalr1.cc:846
    break;

  case 9:
#line 146 "kif_parser.yy" // lalr1.cc:846
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
#line 716 "kif_parser.tab.cc" // lalr1.cc:846
    break;

  case 10:
#line 157 "kif_parser.yy" // lalr1.cc:846
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
#line 731 "kif_parser.tab.cc" // lalr1.cc:846
    break;

  case 11:
#line 167 "kif_parser.yy" // lalr1.cc:846
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
#line 747 "kif_parser.tab.cc" // lalr1.cc:846
    break;

  case 12:
#line 178 "kif_parser.yy" // lalr1.cc:846
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
#line 762 "kif_parser.tab.cc" // lalr1.cc:846
    break;

  case 13:
#line 188 "kif_parser.yy" // lalr1.cc:846
    {
                                                            yystack_[1].value.as< TokenValue > ().Type() = TokenValue::Function;
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
#line 781 "kif_parser.tab.cc" // lalr1.cc:846
    break;

  case 14:
#line 202 "kif_parser.yy" // lalr1.cc:846
    {
                            yylhs.value.as< TokenValue > () = yylhs.value.as< TokenValue > () + yystack_[0].value.as< TokenValue > ();
                            yylhs.value.as< TokenValue > ().Type() = TokenValue::Relation;
                            yylhs.value.as< TokenValue > ().Command() = yystack_[0].value.as< TokenValue > ().Value();
                            std::string msg;
                            int res = driver.AddEntry(yylhs.value.as< TokenValue > (), true, 0, yystack_[0].location, msg);
                            if(res == 1) error(yystack_[0].location, msg);
                            else if(res == 2) driver.Warn(yystack_[0].location, msg);
                        }
#line 795 "kif_parser.tab.cc" // lalr1.cc:846
    break;

  case 15:
#line 211 "kif_parser.yy" // lalr1.cc:846
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
#line 813 "kif_parser.tab.cc" // lalr1.cc:846
    break;

  case 16:
#line 224 "kif_parser.yy" // lalr1.cc:846
    {
                            yylhs.value.as< TokenValue > () = yylhs.value.as< TokenValue > () + yystack_[0].value.as< TokenValue > ();
                            yylhs.value.as< TokenValue > ().Type() = TokenValue::Relation;
                            yylhs.value.as< TokenValue > ().Command() = yystack_[0].value.as< TokenValue > ().Value();
                            std::string msg;
                            int res = driver.AddEntry(yylhs.value.as< TokenValue > (), true, 0, yystack_[0].location, msg);
                            if(res == 1) error(yystack_[0].location, msg);
                            else if(res == 2) driver.Warn(yystack_[0].location, msg);
                        }
#line 827 "kif_parser.tab.cc" // lalr1.cc:846
    break;

  case 17:
#line 234 "kif_parser.yy" // lalr1.cc:846
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
#line 845 "kif_parser.tab.cc" // lalr1.cc:846
    break;

  case 18:
#line 247 "kif_parser.yy" // lalr1.cc:846
    {
                            yylhs.value.as< TokenValue > () = yylhs.value.as< TokenValue > () + yystack_[0].value.as< TokenValue > ();
                            yylhs.value.as< TokenValue > ().Type() = TokenValue::Function;
                            yylhs.value.as< TokenValue > ().Command() = yystack_[0].value.as< TokenValue > ().Value();
                            std::string msg;
                            int res = driver.AddEntry(yylhs.value.as< TokenValue > (), false, 0, yystack_[0].location, msg);
                            if(res == 1) error(yystack_[0].location, msg);
                            else if(res == 2) driver.Warn(yystack_[0].location, msg);
                        }
#line 859 "kif_parser.tab.cc" // lalr1.cc:846
    break;

  case 19:
#line 257 "kif_parser.yy" // lalr1.cc:846
    {
                                            yylhs.value.as< TokenValue > () = yylhs.value.as< TokenValue > () + yystack_[1].value.as< TokenValue > ();
                                            if(yystack_[0].value.as< TokenValue > ().Value() != "") yylhs.value.as< TokenValue > () = yylhs.value.as< TokenValue > () + " " + yystack_[0].value.as< TokenValue > ();
                                            yylhs.value.as< TokenValue > ().Command() = "multiple terms";
                                            yylhs.value.as< TokenValue > ().AddArgument(yystack_[1].value.as< TokenValue > ());
                                            yylhs.value.as< TokenValue > ().AddArguments(yystack_[0].value.as< TokenValue > ().Arguments());
                                        }
#line 871 "kif_parser.tab.cc" // lalr1.cc:846
    break;

  case 20:
#line 264 "kif_parser.yy" // lalr1.cc:846
    { }
#line 877 "kif_parser.tab.cc" // lalr1.cc:846
    break;

  case 21:
#line 266 "kif_parser.yy" // lalr1.cc:846
    {
                        yylhs.value.as< TokenValue > () = yylhs.value.as< TokenValue > () + yystack_[0].value.as< TokenValue > ();
                        yylhs.value.as< TokenValue > ().Type() = TokenValue::Function;
                        yylhs.value.as< TokenValue > ().Command() = yystack_[0].value.as< TokenValue > ().Value();
                    }
#line 887 "kif_parser.tab.cc" // lalr1.cc:846
    break;

  case 22:
#line 271 "kif_parser.yy" // lalr1.cc:846
    {
                        yylhs.value.as< TokenValue > () = yylhs.value.as< TokenValue > () + yystack_[0].value.as< TokenValue > ();
                        yylhs.value.as< TokenValue > ().Type() = TokenValue::Function;
                        yylhs.value.as< TokenValue > ().Command() = yystack_[0].value.as< TokenValue > ().Value();
                    }
#line 897 "kif_parser.tab.cc" // lalr1.cc:846
    break;

  case 23:
#line 277 "kif_parser.yy" // lalr1.cc:846
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
#line 912 "kif_parser.tab.cc" // lalr1.cc:846
    break;

  case 24:
#line 288 "kif_parser.yy" // lalr1.cc:846
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
#line 927 "kif_parser.tab.cc" // lalr1.cc:846
    break;

  case 25:
#line 298 "kif_parser.yy" // lalr1.cc:846
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
#line 942 "kif_parser.tab.cc" // lalr1.cc:846
    break;

  case 26:
#line 308 "kif_parser.yy" // lalr1.cc:846
    {
                                                            yylhs.value.as< TokenValue > () = yystack_[4].value.as< TokenValue > () + " " + yystack_[3].value.as< TokenValue > () + " " + yystack_[2].value.as< TokenValue > () + " " + yystack_[1].value.as< TokenValue > () + " " + yystack_[0].value.as< TokenValue > ();
                                                            yylhs.value.as< TokenValue > ().Command() = yystack_[3].value.as< TokenValue > ().Value();
                                                            yylhs.value.as< TokenValue > ().Type() = TokenValue::Relation;
                                                            yylhs.value.as< TokenValue > ().AddArgument(yystack_[2].value.as< TokenValue > ());
                                                            yylhs.value.as< TokenValue > ().AddArgument(yystack_[1].value.as< TokenValue > ());
                                                            std::string msg;
                                                            int res = driver.AddEntry(yystack_[3].value.as< TokenValue > (), true, yylhs.value.as< TokenValue > ().Count(), yystack_[3].location, msg);
                                                            if(res == 1) error(yystack_[3].location, msg);
                                                            else if(res == 2) driver.Warn(yystack_[3].location, msg);
                                                        }
#line 958 "kif_parser.tab.cc" // lalr1.cc:846
    break;

  case 27:
#line 319 "kif_parser.yy" // lalr1.cc:846
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
#line 973 "kif_parser.tab.cc" // lalr1.cc:846
    break;

  case 28:
#line 329 "kif_parser.yy" // lalr1.cc:846
    {
                                                            yylhs.value.as< TokenValue > () = yystack_[4].value.as< TokenValue > () + " " + yystack_[3].value.as< TokenValue > () + " " + yystack_[2].value.as< TokenValue > () + " " + yystack_[1].value.as< TokenValue > () + " " + yystack_[0].value.as< TokenValue > ();
                                                            yylhs.value.as< TokenValue > ().Command() = yystack_[3].value.as< TokenValue > ().Value();
                                                            yylhs.value.as< TokenValue > ().Type() = TokenValue::Relation;
                                                            yylhs.value.as< TokenValue > ().AddArgument(yystack_[2].value.as< TokenValue > ());
                                                            yylhs.value.as< TokenValue > ().AddArgument(yystack_[1].value.as< TokenValue > ());
                                                            std::string msg;
                                                            int res = driver.AddEntry(yystack_[3].value.as< TokenValue > (), true, yylhs.value.as< TokenValue > ().Count(), yystack_[3].location, msg);
                                                            if(res == 1) error(yystack_[3].location, msg);
                                                            else if(res == 2) driver.Warn(yystack_[3].location, msg);
                                                        }
#line 989 "kif_parser.tab.cc" // lalr1.cc:846
    break;

  case 29:
#line 340 "kif_parser.yy" // lalr1.cc:846
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
#line 1004 "kif_parser.tab.cc" // lalr1.cc:846
    break;

  case 30:
#line 350 "kif_parser.yy" // lalr1.cc:846
    {
                                                                yylhs.value.as< TokenValue > () = yystack_[4].value.as< TokenValue > () + " " + yystack_[3].value.as< TokenValue > () + " " + yystack_[2].value.as< TokenValue > () + " " + yystack_[1].value.as< TokenValue > () + " " + yystack_[0].value.as< TokenValue > ();
                                                                yylhs.value.as< TokenValue > ().Command() = yystack_[3].value.as< TokenValue > ().Value();
                                                                yylhs.value.as< TokenValue > ().Type() = TokenValue::Relation;
                                                                yylhs.value.as< TokenValue > ().AddArgument(yystack_[2].value.as< TokenValue > ());
                                                                yylhs.value.as< TokenValue > ().AddArgument(yystack_[1].value.as< TokenValue > ());
                                                                std::string msg;
                                                                int res = driver.AddEntry(yystack_[3].value.as< TokenValue > (), true, yylhs.value.as< TokenValue > ().Count(), yystack_[3].location, msg);
                                                                if(res == 1) error(yystack_[3].location, msg);
                                                                else if(res == 2) driver.Warn(yystack_[3].location, msg);
                                                            }
#line 1020 "kif_parser.tab.cc" // lalr1.cc:846
    break;

  case 31:
#line 361 "kif_parser.yy" // lalr1.cc:846
    {
                            yylhs.value.as< TokenValue > () = yylhs.value.as< TokenValue > () + yystack_[0].value.as< TokenValue > ();
                            yylhs.value.as< TokenValue > ().Command() = yystack_[0].value.as< TokenValue > ().Value();
                            yylhs.value.as< TokenValue > ().Type() = TokenValue::Relation;
                            std::string msg;
                            int res = driver.AddEntry(yystack_[0].value.as< TokenValue > (), true, 0, yystack_[0].location, msg);
                            if(res == 1) error(yystack_[0].location, msg);
                            else if(res == 2) driver.Warn(yystack_[0].location, msg);
                        }
#line 1034 "kif_parser.tab.cc" // lalr1.cc:846
    break;

  case 32:
#line 370 "kif_parser.yy" // lalr1.cc:846
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
#line 1052 "kif_parser.tab.cc" // lalr1.cc:846
    break;

  case 33:
#line 383 "kif_parser.yy" // lalr1.cc:846
    {
                            yylhs.value.as< TokenValue > () = yylhs.value.as< TokenValue > () + yystack_[0].value.as< TokenValue > ();
                            yylhs.value.as< TokenValue > ().Command() = yystack_[0].value.as< TokenValue > ().Value();
                            yylhs.value.as< TokenValue > ().Type() = TokenValue::Relation;
                            std::string msg;
                            int res = driver.AddEntry(yystack_[0].value.as< TokenValue > (), true, 0, yystack_[0].location, msg);
                            if(res == 1) error(yystack_[0].location, msg);
                            else if(res == 2) driver.Warn(yystack_[0].location, msg);
                        }
#line 1066 "kif_parser.tab.cc" // lalr1.cc:846
    break;

  case 34:
#line 393 "kif_parser.yy" // lalr1.cc:846
    { yylhs.value.as< TokenValue > () = yystack_[0].value.as< TokenValue > (); }
#line 1072 "kif_parser.tab.cc" // lalr1.cc:846
    break;

  case 35:
#line 394 "kif_parser.yy" // lalr1.cc:846
    {
                                                    yylhs.value.as< TokenValue > () = yystack_[3].value.as< TokenValue > () + " " + yystack_[2].value.as< TokenValue > () + " " + yystack_[1].value.as< TokenValue > () + " " + yystack_[0].value.as< TokenValue > ();
                                                    yylhs.value.as< TokenValue > ().PerformNot();
                                                    yylhs.value.as< TokenValue > ().Command() = yystack_[2].value.as< TokenValue > ().Value();
                                                    yylhs.value.as< TokenValue > ().Type() = TokenValue::Relation;
                                                    yylhs.value.as< TokenValue > ().AddArgument(yystack_[1].value.as< TokenValue > ());
                                                }
#line 1084 "kif_parser.tab.cc" // lalr1.cc:846
    break;

  case 36:
#line 401 "kif_parser.yy" // lalr1.cc:846
    {
                                        yylhs.value.as< TokenValue > () = yylhs.value.as< TokenValue > () + yystack_[1].value.as< TokenValue > ();
                                        if(yystack_[0].value.as< TokenValue > () != "") yylhs.value.as< TokenValue > () = yylhs.value.as< TokenValue > () + " " + yystack_[0].value.as< TokenValue > ();
                                        yylhs.value.as< TokenValue > ().Command() = "Multiple relations";
                                        yylhs.value.as< TokenValue > ().AddArgument(yystack_[1].value.as< TokenValue > ());
                                        yylhs.value.as< TokenValue > ().AddArguments(yystack_[0].value.as< TokenValue > ().Arguments());
                                    }
#line 1096 "kif_parser.tab.cc" // lalr1.cc:846
    break;

  case 37:
#line 408 "kif_parser.yy" // lalr1.cc:846
    { }
#line 1102 "kif_parser.tab.cc" // lalr1.cc:846
    break;

  case 38:
#line 411 "kif_parser.yy" // lalr1.cc:846
    {
                                                            yylhs.value.as< TokenValue > () = yystack_[4].value.as< TokenValue > () + " " + yystack_[3].value.as< TokenValue > () + " " + yystack_[2].value.as< TokenValue > () + " " + yystack_[1].value.as< TokenValue > () + " " + yystack_[0].value.as< TokenValue > ();
                                                            yylhs.value.as< TokenValue > ().Command() = yystack_[3].value.as< TokenValue > ().Value();
                                                            yylhs.value.as< TokenValue > ().Type() = TokenValue::Relation;
                                                            yylhs.value.as< TokenValue > ().AddArgument(yystack_[2].value.as< TokenValue > ());
                                                            yylhs.value.as< TokenValue > ().AddArgument(yystack_[1].value.as< TokenValue > ());
                                                        }
#line 1114 "kif_parser.tab.cc" // lalr1.cc:846
    break;

  case 39:
#line 418 "kif_parser.yy" // lalr1.cc:846
    {
                                                        yylhs.value.as< TokenValue > () = yystack_[3].value.as< TokenValue > () + " " + yystack_[2].value.as< TokenValue > () + " " + yystack_[1].value.as< TokenValue > () + " " + yystack_[0].value.as< TokenValue > ();
                                                        yylhs.value.as< TokenValue > ().Command() = yystack_[2].value.as< TokenValue > ().Value();
                                                        yylhs.value.as< TokenValue > ().Type() = TokenValue::Relation;
                                                        yylhs.value.as< TokenValue > ().AddArgument(yystack_[1].value.as< TokenValue > ());
                                                    }
#line 1125 "kif_parser.tab.cc" // lalr1.cc:846
    break;

  case 40:
#line 424 "kif_parser.yy" // lalr1.cc:846
    {
                                                                yylhs.value.as< TokenValue > () = yystack_[4].value.as< TokenValue > () + " " + yystack_[3].value.as< TokenValue > () + " " + yystack_[2].value.as< TokenValue > () + " " + yystack_[1].value.as< TokenValue > ().Value() + " " + yystack_[0].value.as< TokenValue > ();
                                                                yylhs.value.as< TokenValue > ().OrRestrict(yystack_[1].value.as< TokenValue > ());
                                                                yylhs.value.as< TokenValue > ().Command() = yystack_[3].value.as< TokenValue > ().Value();
                                                                yylhs.value.as< TokenValue > ().Type() = TokenValue::Relation;
                                                                yylhs.value.as< TokenValue > ().AddArgument(yystack_[2].value.as< TokenValue > ());
                                                                yylhs.value.as< TokenValue > ().AddArguments(yystack_[1].value.as< TokenValue > ().Arguments());
                                                            }
#line 1138 "kif_parser.tab.cc" // lalr1.cc:846
    break;

  case 41:
#line 432 "kif_parser.yy" // lalr1.cc:846
    {
                                                            yylhs.value.as< TokenValue > () = yystack_[4].value.as< TokenValue > () + " " + yystack_[3].value.as< TokenValue > () + " " + yystack_[2].value.as< TokenValue > () + " " + yystack_[1].value.as< TokenValue > () + " " + yystack_[0].value.as< TokenValue > ();
                                                            yylhs.value.as< TokenValue > ().PerformNot();
                                                            yylhs.value.as< TokenValue > ().Command() = yystack_[3].value.as< TokenValue > ().Value();
                                                            yylhs.value.as< TokenValue > ().Type() = TokenValue::Relation;
                                                            yylhs.value.as< TokenValue > ().AddArgument(yystack_[2].value.as< TokenValue > ());
                                                            yylhs.value.as< TokenValue > ().AddArgument(yystack_[1].value.as< TokenValue > ());
                                                        }
#line 1151 "kif_parser.tab.cc" // lalr1.cc:846
    break;

  case 42:
#line 440 "kif_parser.yy" // lalr1.cc:846
    {
                                                        yylhs.value.as< TokenValue > () = yystack_[3].value.as< TokenValue > () + " " + yystack_[2].value.as< TokenValue > () + " " + yystack_[1].value.as< TokenValue > () + " " + yystack_[0].value.as< TokenValue > ();
                                                        yylhs.value.as< TokenValue > ().Command() = yystack_[2].value.as< TokenValue > ().Value();
                                                        yylhs.value.as< TokenValue > ().Type() = TokenValue::Relation;
                                                        yylhs.value.as< TokenValue > ().AddArgument(yystack_[1].value.as< TokenValue > ());
                                                    }
#line 1162 "kif_parser.tab.cc" // lalr1.cc:846
    break;

  case 43:
#line 446 "kif_parser.yy" // lalr1.cc:846
    {
                                                yylhs.value.as< TokenValue > () = yystack_[3].value.as< TokenValue > () + " " + yystack_[2].value.as< TokenValue > () + " " + yystack_[1].value.as< TokenValue > () + " " + yystack_[0].value.as< TokenValue > ();
                                                yylhs.value.as< TokenValue > ().Command() = yystack_[2].value.as< TokenValue > ().Value();
                                                yylhs.value.as< TokenValue > ().Type() = TokenValue::Relation;
                                                yylhs.value.as< TokenValue > ().AddArgument(yystack_[1].value.as< TokenValue > ());
                                            }
#line 1173 "kif_parser.tab.cc" // lalr1.cc:846
    break;

  case 44:
#line 452 "kif_parser.yy" // lalr1.cc:846
    {
                                                            yylhs.value.as< TokenValue > () = yystack_[4].value.as< TokenValue > () + " " + yystack_[3].value.as< TokenValue > () + " " + yystack_[2].value.as< TokenValue > () + " " + yystack_[1].value.as< TokenValue > () + " " + yystack_[0].value.as< TokenValue > ();
                                                            yylhs.value.as< TokenValue > ().Command() = yystack_[3].value.as< TokenValue > ().Value();
                                                            yylhs.value.as< TokenValue > ().Type() = TokenValue::Relation;
                                                            yylhs.value.as< TokenValue > ().AddArgument(yystack_[2].value.as< TokenValue > ());
                                                            yylhs.value.as< TokenValue > ().AddArgument(yystack_[1].value.as< TokenValue > ());
                                                        }
#line 1185 "kif_parser.tab.cc" // lalr1.cc:846
    break;

  case 45:
#line 459 "kif_parser.yy" // lalr1.cc:846
    {
                                                            yylhs.value.as< TokenValue > () = yystack_[4].value.as< TokenValue > () + " " + yystack_[3].value.as< TokenValue > () + " " + yystack_[2].value.as< TokenValue > () + " " + yystack_[1].value.as< TokenValue > () + " " + yystack_[0].value.as< TokenValue > ();
                                                            yylhs.value.as< TokenValue > ().Command() = yystack_[3].value.as< TokenValue > ().Value();
                                                            yylhs.value.as< TokenValue > ().Type() = TokenValue::Relation;
                                                            yylhs.value.as< TokenValue > ().AddArgument(yystack_[2].value.as< TokenValue > ());
                                                            yylhs.value.as< TokenValue > ().AddArgument(yystack_[1].value.as< TokenValue > ());
                                                        }
#line 1197 "kif_parser.tab.cc" // lalr1.cc:846
    break;

  case 46:
#line 466 "kif_parser.yy" // lalr1.cc:846
    {
                                                                yylhs.value.as< TokenValue > () = yystack_[4].value.as< TokenValue > () + " " + yystack_[3].value.as< TokenValue > () + " " + yystack_[2].value.as< TokenValue > () + " " + yystack_[1].value.as< TokenValue > () + " " + yystack_[0].value.as< TokenValue > ();
                                                                yylhs.value.as< TokenValue > ().Command() = yystack_[3].value.as< TokenValue > ().Value();
                                                                yylhs.value.as< TokenValue > ().Type() = TokenValue::Relation;
                                                                yylhs.value.as< TokenValue > ().AddArgument(yystack_[2].value.as< TokenValue > ());
                                                                yylhs.value.as< TokenValue > ().AddArgument(yystack_[1].value.as< TokenValue > ());
                                                            }
#line 1209 "kif_parser.tab.cc" // lalr1.cc:846
    break;

  case 47:
#line 473 "kif_parser.yy" // lalr1.cc:846
    {
                            yylhs.value.as< TokenValue > () = yylhs.value.as< TokenValue > () + yystack_[0].value.as< TokenValue > ();
                            yylhs.value.as< TokenValue > ().Command() = yystack_[0].value.as< TokenValue > ().Value();
                        }
#line 1218 "kif_parser.tab.cc" // lalr1.cc:846
    break;

  case 48:
#line 477 "kif_parser.yy" // lalr1.cc:846
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
#line 1236 "kif_parser.tab.cc" // lalr1.cc:846
    break;

  case 49:
#line 490 "kif_parser.yy" // lalr1.cc:846
    {
                            yylhs.value.as< TokenValue > () = yylhs.value.as< TokenValue > () + yystack_[0].value.as< TokenValue > ();
                            yylhs.value.as< TokenValue > ().Command() = yystack_[0].value.as< TokenValue > ().Value();
                            yylhs.value.as< TokenValue > ().Type() = TokenValue::Relation;
                            std::string msg;
                            int res = driver.CheckEntry(yystack_[0].value.as< TokenValue > (), true, 0, yystack_[0].location, msg);
                            if(res == 1) error(yystack_[0].location, msg);
                            else if(res == 2) driver.Warn(yystack_[0].location, msg);
                        }
#line 1250 "kif_parser.tab.cc" // lalr1.cc:846
    break;

  case 50:
#line 501 "kif_parser.yy" // lalr1.cc:846
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
#line 1268 "kif_parser.tab.cc" // lalr1.cc:846
    break;

  case 51:
#line 514 "kif_parser.yy" // lalr1.cc:846
    {
                            yylhs.value.as< TokenValue > () = yylhs.value.as< TokenValue > () + yystack_[0].value.as< TokenValue > ();
                            yylhs.value.as< TokenValue > ().Command() = yystack_[0].value.as< TokenValue > ().Value();
                            yylhs.value.as< TokenValue > ().Type() = TokenValue::Function;
                            std::string msg;
                            int res = driver.AddEntry(yystack_[0].value.as< TokenValue > (), false, 0, yystack_[0].location, msg);
                            if(res == 1) error(yystack_[0].location, msg);
                            else if(res == 2) driver.Warn(yystack_[0].location, msg);
                        }
#line 1282 "kif_parser.tab.cc" // lalr1.cc:846
    break;

  case 52:
#line 523 "kif_parser.yy" // lalr1.cc:846
    {
                        yylhs.value.as< TokenValue > () = yylhs.value.as< TokenValue > () + yystack_[0].value.as< TokenValue > ();
                        yylhs.value.as< TokenValue > ().AddVariable(yystack_[0].value.as< TokenValue > ());
                        yylhs.value.as< TokenValue > ().Command() = yystack_[0].value.as< TokenValue > ().Value();
                        yylhs.value.as< TokenValue > ().Type() = TokenValue::Var;
                    }
#line 1293 "kif_parser.tab.cc" // lalr1.cc:846
    break;

  case 53:
#line 530 "kif_parser.yy" // lalr1.cc:846
    {
                                yylhs.value.as< TokenValue > () = yylhs.value.as< TokenValue > () + yystack_[1].value.as< TokenValue > ();
                                if(yystack_[0].value.as< TokenValue > () != "") yylhs.value.as< TokenValue > () = yylhs.value.as< TokenValue > () + " " + yystack_[0].value.as< TokenValue > ();
                                yylhs.value.as< TokenValue > ().Command() = "Multiple terms";
                                yylhs.value.as< TokenValue > ().AddArgument(yystack_[1].value.as< TokenValue > ());
                                yylhs.value.as< TokenValue > ().AddArguments(yystack_[0].value.as< TokenValue > ().Arguments());
                            }
#line 1305 "kif_parser.tab.cc" // lalr1.cc:846
    break;

  case 54:
#line 538 "kif_parser.yy" // lalr1.cc:846
    { }
#line 1311 "kif_parser.tab.cc" // lalr1.cc:846
    break;

  case 55:
#line 540 "kif_parser.yy" // lalr1.cc:846
    {
                            yylhs.value.as< TokenValue > () = yylhs.value.as< TokenValue > () + yystack_[0].value.as< TokenValue > ();
                            yylhs.value.as< TokenValue > ().Command() = yystack_[0].value.as< TokenValue > ().Value();
                            yylhs.value.as< TokenValue > ().Type() = TokenValue::Function;
                            std::string msg;
                            int res = driver.AddEntry(yystack_[0].value.as< TokenValue > (), false, 0, yystack_[0].location, msg);
                            if(res == 1) error(yystack_[0].location, msg);
                            else if(res == 2) driver.Warn(yystack_[0].location, msg);
                        }
#line 1325 "kif_parser.tab.cc" // lalr1.cc:846
    break;

  case 56:
#line 549 "kif_parser.yy" // lalr1.cc:846
    {
                        yylhs.value.as< TokenValue > () = yylhs.value.as< TokenValue > () + yystack_[0].value.as< TokenValue > ();
                        yylhs.value.as< TokenValue > ().AddVariable(yystack_[0].value.as< TokenValue > ());
                        yylhs.value.as< TokenValue > ().Command() = yystack_[0].value.as< TokenValue > ().Value();
                        yylhs.value.as< TokenValue > ().Type() = TokenValue::Var;
                    }
#line 1336 "kif_parser.tab.cc" // lalr1.cc:846
    break;

  case 57:
#line 556 "kif_parser.yy" // lalr1.cc:846
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
#line 1354 "kif_parser.tab.cc" // lalr1.cc:846
    break;

  case 58:
#line 569 "kif_parser.yy" // lalr1.cc:846
    {
                            yylhs.value.as< TokenValue > () = yylhs.value.as< TokenValue > () + yystack_[0].value.as< TokenValue > ();
                            yylhs.value.as< TokenValue > ().Command() = yystack_[0].value.as< TokenValue > ().Value();
                            yylhs.value.as< TokenValue > ().Type() = TokenValue::Function;
                            std::string msg;
                            int res = driver.CheckEntry(yystack_[0].value.as< TokenValue > (), false, 0, yystack_[0].location, msg);
                            if(res == 1) error(yystack_[0].location, msg);
                            else if(res == 2) driver.Warn(yystack_[0].location, msg);
                        }
#line 1368 "kif_parser.tab.cc" // lalr1.cc:846
    break;

  case 59:
#line 578 "kif_parser.yy" // lalr1.cc:846
    {
                        yylhs.value.as< TokenValue > () = yylhs.value.as< TokenValue > () + yystack_[0].value.as< TokenValue > ();
                        yylhs.value.as< TokenValue > ().AddVariable(yystack_[0].value.as< TokenValue > ());
                        yylhs.value.as< TokenValue > ().Command() = yystack_[0].value.as< TokenValue > ().Value();
                        yylhs.value.as< TokenValue > ().Type() = TokenValue::Var;
                    }
#line 1379 "kif_parser.tab.cc" // lalr1.cc:846
    break;

  case 60:
#line 585 "kif_parser.yy" // lalr1.cc:846
    {
                                yylhs.value.as< TokenValue > () = yylhs.value.as< TokenValue > () + yystack_[1].value.as< TokenValue > ();
                                if(yystack_[0].value.as< TokenValue > ().Value() != "") yylhs.value.as< TokenValue > () = yylhs.value.as< TokenValue > () + " " + yystack_[0].value.as< TokenValue > ();
                                yylhs.value.as< TokenValue > ().Command() = "Multiple terms";
                                yylhs.value.as< TokenValue > ().Type() = TokenValue::Function;
                                yylhs.value.as< TokenValue > ().AddArgument(yystack_[1].value.as< TokenValue > ());
                                yylhs.value.as< TokenValue > ().AddArguments(yystack_[0].value.as< TokenValue > ().Arguments());
                            }
#line 1392 "kif_parser.tab.cc" // lalr1.cc:846
    break;

  case 61:
#line 594 "kif_parser.yy" // lalr1.cc:846
    { }
#line 1398 "kif_parser.tab.cc" // lalr1.cc:846
    break;

  case 62:
#line 596 "kif_parser.yy" // lalr1.cc:846
    {
                            yylhs.value.as< TokenValue > () = yylhs.value.as< TokenValue > () + yystack_[0].value.as< TokenValue > ();
                            yylhs.value.as< TokenValue > ().Command() = yystack_[0].value.as< TokenValue > ().Value();
                            yylhs.value.as< TokenValue > ().Type() = TokenValue::Function;
                            std::string msg;
                            int res = driver.CheckEntry(yystack_[0].value.as< TokenValue > (), false, 0, yystack_[0].location, msg);
                            if(res == 1) error(yystack_[0].location, msg);
                            else if(res == 2) driver.Warn(yystack_[0].location, msg);
                        }
#line 1412 "kif_parser.tab.cc" // lalr1.cc:846
    break;

  case 63:
#line 605 "kif_parser.yy" // lalr1.cc:846
    {
                        yylhs.value.as< TokenValue > () = yylhs.value.as< TokenValue > () + yystack_[0].value.as< TokenValue > ();
                        yylhs.value.as< TokenValue > ().AddVariable(yystack_[0].value.as< TokenValue > ());
                        yylhs.value.as< TokenValue > ().Command() = yystack_[0].value.as< TokenValue > ().Value();
                        yylhs.value.as< TokenValue > ().Type() = TokenValue::Var;
                    }
#line 1423 "kif_parser.tab.cc" // lalr1.cc:846
    break;

  case 64:
#line 612 "kif_parser.yy" // lalr1.cc:846
    {
                        yylhs.value.as< TokenValue > () = yylhs.value.as< TokenValue > () + yystack_[0].value.as< TokenValue > ();
                        yylhs.value.as< TokenValue > ().Command() = yystack_[0].value.as< TokenValue > ().Value();
                        yylhs.value.as< TokenValue > ().Type() = TokenValue::Function;
                    }
#line 1433 "kif_parser.tab.cc" // lalr1.cc:846
    break;

  case 65:
#line 617 "kif_parser.yy" // lalr1.cc:846
    {
                        yylhs.value.as< TokenValue > () = yylhs.value.as< TokenValue > () + yystack_[0].value.as< TokenValue > ();
                        yylhs.value.as< TokenValue > ().AddVariable(yystack_[0].value.as< TokenValue > ());
                        yylhs.value.as< TokenValue > ().Type() = TokenValue::Var;
                    }
#line 1443 "kif_parser.tab.cc" // lalr1.cc:846
    break;


#line 1447 "kif_parser.tab.cc" // lalr1.cc:846
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


  const signed char KIFParser::yypact_ninf_ = -121;

  const signed char KIFParser::yytable_ninf_ = -1;

  const short int
  KIFParser::yypact_[] =
  {
      59,   136,  -121,  -121,  -121,    59,     9,    22,    59,  -121,
      59,    38,    38,    54,    54,    38,    54,    38,    86,    54,
    -121,  -121,  -121,  -121,  -121,    25,    54,    38,    32,  -121,
      61,    54,    63,    69,   179,  -121,  -121,    97,    54,  -121,
      73,    54,  -121,  -121,    81,  -121,    90,    66,    66,   131,
     131,    66,   131,    66,   131,   147,  -121,  -121,    97,  -121,
      54,    95,  -121,    54,  -121,  -121,  -121,  -121,    99,   131,
      38,  -121,  -121,   102,   105,   131,   127,    -3,   131,   199,
     199,   191,   199,   199,   199,   191,   111,    97,   191,   191,
      97,   129,  -121,  -121,   135,  -121,   149,   131,  -121,  -121,
     156,  -121,  -121,  -121,   158,   131,   165,  -121,  -121,   169,
     191,    38,  -121,  -121,   172,   191,    -3,   191,   174,   163,
     177,    97,   191,   191,  -121,  -121,  -121,  -121,   131,  -121,
    -121,  -121,  -121,  -121,   181,   191,  -121,   196,   203,   204,
    -121,  -121,   205,   206,   191,   207,   208,  -121,   191,  -121,
    -121,  -121,  -121,  -121,  -121,  -121,  -121,   209,  -121
  };

  const unsigned char
  KIFParser::yydefact_[] =
  {
       6,     0,    21,    22,    14,     6,     0,     0,     6,    16,
       6,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       5,     1,     2,     3,     4,     0,     0,     0,     0,    18,
       0,     0,     0,     0,     0,    31,    33,     0,    20,     7,
       0,     0,    10,     8,     0,    12,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    47,    49,    37,    34,
      20,     0,     9,    20,    11,    13,    56,    55,     0,     0,
       0,    52,    51,     0,     0,     0,     0,     0,    54,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      37,     0,    19,    15,     0,    24,     0,     0,    27,    25,
       0,    29,    65,    64,     0,    54,     0,    63,    62,     0,
       0,     0,    59,    58,     0,     0,     0,     0,     0,     0,
       0,    37,     0,    61,    36,    23,    17,    26,    54,    28,
      30,    53,    32,    42,     0,     0,    43,     0,     0,     0,
      39,    35,     0,     0,    61,     0,     0,    44,    61,    45,
      46,    38,    40,    41,    60,    48,    50,     0,    57
  };

  const signed char
  KIFParser::yypgoto_[] =
  {
    -121,  -121,    34,  -121,    94,   -28,     0,  -121,  -121,   -31,
     -88,    12,   -29,   -98,    79,     7,  -120,   119,    -1
  };

  const short int
  KIFParser::yydefgoto_[] =
  {
      -1,     6,     7,     8,    60,    61,   113,    10,    37,    90,
      91,    59,   105,   106,    68,   144,   145,   109,   104
  };

  const unsigned char
  KIFParser::yytable_[] =
  {
       9,    19,   124,   102,   103,     9,    58,   131,     9,    21,
       9,    25,    26,    29,    29,    31,    29,    33,    36,    29,
      73,    74,    22,    76,   154,    78,    29,    41,   157,    39,
     146,    29,    92,   142,    54,    94,    42,    57,    29,    20,
      96,    29,    23,     2,    24,     3,   100,    67,    67,    72,
      72,    67,    72,    67,    72,    89,   121,    27,    57,     2,
      29,     3,     1,    29,     2,    43,     3,    45,   128,    72,
      97,     2,    66,     3,     4,    72,    46,    62,    72,   108,
     108,     5,   108,   108,   108,    64,    57,    57,   114,    34,
      57,     2,   118,     3,    65,   122,   123,    72,   120,    93,
      55,    35,     2,    95,     3,    72,    98,    28,    30,    99,
      32,   135,    56,    38,   119,   138,     2,   134,     3,    89,
      40,    57,   137,     0,   139,    44,    56,    69,    72,   143,
      75,   101,    77,   125,    70,    63,     2,    71,     3,   126,
       0,     2,   148,     3,    11,    12,    13,    14,    15,    16,
      17,     0,     2,   127,     3,    79,    80,    18,    81,    82,
     129,    83,   130,    84,    85,    86,    87,    88,     2,   132,
       3,    79,    80,   133,    81,    82,   136,    83,   140,    84,
      85,   141,    87,    88,     2,   147,     3,    47,    48,    49,
      50,    51,    52,    53,   111,     0,     2,   112,     3,   110,
     149,   115,   116,   117,     2,   107,     3,   150,   151,   152,
     153,   155,   156,   158
  };

  const short int
  KIFParser::yycheck_[] =
  {
       0,     1,    90,     6,     7,     5,    37,   105,     8,     0,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      49,    50,     0,    52,   144,    54,    26,    27,   148,     4,
     128,    31,    60,   121,    34,    63,     4,    37,    38,     5,
      69,    41,     8,     5,    10,     7,    75,    47,    48,    49,
      50,    51,    52,    53,    54,    55,    87,     3,    58,     5,
      60,     7,     3,    63,     5,     4,     7,     4,    97,    69,
      70,     5,     6,     7,    15,    75,     7,     4,    78,    79,
      80,    22,    82,    83,    84,     4,    86,    87,    81,     3,
      90,     5,    85,     7,     4,    88,    89,    97,    86,     4,
       3,    15,     5,     4,     7,   105,     4,    13,    14,     4,
      16,   111,    15,    19,     3,   116,     5,   110,     7,   119,
      26,   121,   115,    -1,   117,    31,    15,    48,   128,   122,
      51,     4,    53,     4,     3,    41,     5,     6,     7,     4,
      -1,     5,   135,     7,     8,     9,    10,    11,    12,    13,
      14,    -1,     5,     4,     7,     8,     9,    21,    11,    12,
       4,    14,     4,    16,    17,    18,    19,    20,     5,     4,
       7,     8,     9,     4,    11,    12,     4,    14,     4,    16,
      17,     4,    19,    20,     5,     4,     7,     8,     9,    10,
      11,    12,    13,    14,     3,    -1,     5,     6,     7,    80,
       4,    82,    83,    84,     5,     6,     7,     4,     4,     4,
       4,     4,     4,     4
  };

  const unsigned char
  KIFParser::yystos_[] =
  {
       0,     3,     5,     7,    15,    22,    24,    25,    26,    29,
      30,     8,     9,    10,    11,    12,    13,    14,    21,    29,
      25,     0,     0,    25,    25,    29,    29,     3,    27,    29,
      27,    29,    27,    29,     3,    15,    29,    31,    27,     4,
      27,    29,     4,     4,    27,     4,     7,     8,     9,    10,
      11,    12,    13,    14,    29,     3,    15,    29,    32,    34,
      27,    28,     4,    27,     4,     4,     6,    29,    37,    37,
       3,     6,    29,    35,    35,    37,    35,    37,    35,     8,
       9,    11,    12,    14,    16,    17,    18,    19,    20,    29,
      32,    33,    28,     4,    28,     4,    35,    29,     4,     4,
      35,     4,     6,     7,    41,    35,    36,     6,    29,    40,
      40,     3,     6,    29,    38,    40,    40,    40,    38,     3,
      34,    32,    38,    38,    33,     4,     4,     4,    35,     4,
       4,    36,     4,     4,    38,    29,     4,    38,    41,    38,
       4,     4,    33,    38,    38,    39,    36,     4,    38,     4,
       4,     4,     4,     4,    39,     4,     4,    39,     4
  };

  const unsigned char
  KIFParser::yyr1_[] =
  {
       0,    23,    24,    25,    25,    25,    25,    26,    26,    26,
      26,    26,    26,    26,    26,    26,    26,    27,    27,    28,
      28,    29,    29,    30,    31,    31,    31,    31,    31,    31,
      31,    31,    31,    31,    32,    32,    33,    33,    34,    34,
      34,    34,    34,    34,    34,    34,    34,    34,    34,    34,
      35,    35,    35,    36,    36,    37,    37,    38,    38,    38,
      39,    39,    40,    40,    41,    41
  };

  const unsigned char
  KIFParser::yyr2_[] =
  {
       0,     2,     2,     2,     2,     2,     0,     4,     4,     5,
       4,     5,     4,     5,     1,     5,     1,     5,     1,     2,
       0,     1,     1,     6,     4,     4,     5,     4,     5,     4,
       5,     1,     5,     1,     1,     4,     2,     0,     5,     4,
       5,     5,     4,     4,     5,     5,     5,     1,     5,     1,
       5,     1,     1,     2,     0,     1,     1,     5,     1,     1,
       2,     0,     1,     1,     1,     1
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
  "\"distinct\"", "\"clause command\"", "\"line mark\"", "$accept",
  "Start", "S", "Fact", "Ground_term", "Ground_terms", "Id_num", "Clause",
  "Head", "ORelation", "ORelations", "Literal", "Term", "Terms",
  "Basic_term", "CTerm", "CTerms", "Basic_cterm", "Num_var", YY_NULL
  };

#if YYDEBUG
  const unsigned short int
  KIFParser::yyrline_[] =
  {
       0,   113,   113,   115,   118,   121,   124,   126,   136,   146,
     157,   167,   178,   188,   202,   211,   224,   234,   247,   257,
     264,   266,   271,   277,   288,   298,   308,   319,   329,   340,
     350,   361,   370,   383,   393,   394,   401,   408,   411,   418,
     424,   432,   440,   446,   452,   459,   466,   473,   477,   490,
     501,   514,   523,   530,   538,   540,   549,   556,   569,   578,
     585,   594,   596,   605,   612,   617
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
} } } } // libgdl::gdlparser::parser::yy
#line 1925 "kif_parser.tab.cc" // lalr1.cc:1156
#line 623 "kif_parser.yy" // lalr1.cc:1157


  typedef libgdl::gdlparser::parser::yy::KIFParser KIFParser;

  // Mandatory error function
  void KIFParser::error (const KIFParser::location_type& loc, const std::string& msg)
  {
    driver.Error(loc, msg);
  }
