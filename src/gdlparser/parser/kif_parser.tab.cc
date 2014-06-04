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
      case 33: // Term
      case 34: // Terms
      case 35: // CTerm
      case 36: // CTerms
      case 37: // Num_var
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
      case 33: // Term
      case 34: // Terms
      case 35: // CTerm
      case 36: // CTerms
      case 37: // Num_var
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
      case 33: // Term
      case 34: // Terms
      case 35: // CTerm
      case 36: // CTerms
      case 37: // Num_var
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
#line 109 "kif_parser.yy" // lalr1.cc:846
    {
                            driver.AddFact(yystack_[1].value.as< TokenValue > ()); yystack_[0].value.as< TokenValue > ();
                        }
#line 636 "kif_parser.tab.cc" // lalr1.cc:846
    break;

  case 4:
#line 112 "kif_parser.yy" // lalr1.cc:846
    {
                            driver.AddClause(yystack_[1].value.as< TokenValue > (), yystack_[1].location); yystack_[0].value.as< TokenValue > ();
                        }
#line 644 "kif_parser.tab.cc" // lalr1.cc:846
    break;

  case 5:
#line 115 "kif_parser.yy" // lalr1.cc:846
    { }
#line 650 "kif_parser.tab.cc" // lalr1.cc:846
    break;

  case 6:
#line 117 "kif_parser.yy" // lalr1.cc:846
    {
                                                        yylhs.value.as< TokenValue > () = yystack_[3].value.as< TokenValue > () + " " + yystack_[2].value.as< TokenValue > () + " " + yystack_[1].value.as< TokenValue > () + " " + yystack_[0].value.as< TokenValue > ();
                                                        yylhs.value.as< TokenValue > ().Type() = TokenValue::Relation;
                                                        yylhs.value.as< TokenValue > ().Command() = yystack_[2].value.as< TokenValue > ().Value();
                                                        yylhs.value.as< TokenValue > ().AddArgument(yystack_[1].value.as< TokenValue > ());
                                                    }
#line 661 "kif_parser.tab.cc" // lalr1.cc:846
    break;

  case 7:
#line 123 "kif_parser.yy" // lalr1.cc:846
    {
                                                        yylhs.value.as< TokenValue > () = yystack_[3].value.as< TokenValue > () + " " + yystack_[2].value.as< TokenValue > () + " " + yystack_[1].value.as< TokenValue > () + " " + yystack_[0].value.as< TokenValue > ();
                                                        yylhs.value.as< TokenValue > ().Type() = TokenValue::Relation;
                                                        yylhs.value.as< TokenValue > ().Command() = yystack_[2].value.as< TokenValue > ().Value();
                                                        yylhs.value.as< TokenValue > ().AddArgument(yystack_[1].value.as< TokenValue > ());
                                                    }
#line 672 "kif_parser.tab.cc" // lalr1.cc:846
    break;

  case 8:
#line 129 "kif_parser.yy" // lalr1.cc:846
    {
                                                                    yylhs.value.as< TokenValue > () = yystack_[4].value.as< TokenValue > () + " " + yystack_[3].value.as< TokenValue > () + " " + yystack_[2].value.as< TokenValue > () + " " + yystack_[1].value.as< TokenValue > () + " " + yystack_[0].value.as< TokenValue > ();
                                                                    yylhs.value.as< TokenValue > ().Type() = TokenValue::Relation;
                                                                    yylhs.value.as< TokenValue > ().Command() = yystack_[3].value.as< TokenValue > ().Value();
                                                                    yylhs.value.as< TokenValue > ().AddArgument(yystack_[2].value.as< TokenValue > ());
                                                                    yylhs.value.as< TokenValue > ().AddArgument(yystack_[1].value.as< TokenValue > ());
                                                                }
#line 684 "kif_parser.tab.cc" // lalr1.cc:846
    break;

  case 9:
#line 136 "kif_parser.yy" // lalr1.cc:846
    {
                                                        yylhs.value.as< TokenValue > () = yystack_[3].value.as< TokenValue > () + " " + yystack_[2].value.as< TokenValue > () + " " + yystack_[1].value.as< TokenValue > () + " " + yystack_[0].value.as< TokenValue > ();
                                                        yylhs.value.as< TokenValue > ().Type() = TokenValue::Relation;
                                                        yylhs.value.as< TokenValue > ().Command() = yystack_[2].value.as< TokenValue > ().Value();
                                                        yylhs.value.as< TokenValue > ().AddArgument(yystack_[1].value.as< TokenValue > ());
                                                    }
#line 695 "kif_parser.tab.cc" // lalr1.cc:846
    break;

  case 10:
#line 142 "kif_parser.yy" // lalr1.cc:846
    {
                                                                    yylhs.value.as< TokenValue > () = yystack_[4].value.as< TokenValue > () + " " + yystack_[3].value.as< TokenValue > () + " " + yystack_[2].value.as< TokenValue > () + " " + yystack_[1].value.as< TokenValue > () + " " + yystack_[0].value.as< TokenValue > ();
                                                                    yylhs.value.as< TokenValue > ().Type() = TokenValue::Relation;
                                                                    yylhs.value.as< TokenValue > ().Command() = yystack_[3].value.as< TokenValue > ().Value();
                                                                    yylhs.value.as< TokenValue > ().AddArgument(yystack_[2].value.as< TokenValue > ());
                                                                    yylhs.value.as< TokenValue > ().AddArgument(yystack_[1].value.as< TokenValue > ());
                                                                }
#line 707 "kif_parser.tab.cc" // lalr1.cc:846
    break;

  case 11:
#line 149 "kif_parser.yy" // lalr1.cc:846
    {
                                                        yylhs.value.as< TokenValue > () = yystack_[3].value.as< TokenValue > () + " " + yystack_[2].value.as< TokenValue > () + " " + yystack_[1].value.as< TokenValue > () + " " + yystack_[0].value.as< TokenValue > ();
                                                        yylhs.value.as< TokenValue > ().Type() = TokenValue::Relation;
                                                        yylhs.value.as< TokenValue > ().Command() = yystack_[2].value.as< TokenValue > ().Value();
                                                        yylhs.value.as< TokenValue > ().AddArgument(yystack_[1].value.as< TokenValue > ());
                                                    }
#line 718 "kif_parser.tab.cc" // lalr1.cc:846
    break;

  case 12:
#line 155 "kif_parser.yy" // lalr1.cc:846
    {
                                                            yylhs.value.as< TokenValue > () = yystack_[4].value.as< TokenValue > () + " " + yystack_[3].value.as< TokenValue > () + " " + yystack_[2].value.as< TokenValue > () + " " + yystack_[1].value.as< TokenValue > () + " " + yystack_[0].value.as< TokenValue > ();
                                                            yylhs.value.as< TokenValue > ().Type() = TokenValue::Relation;
                                                            yylhs.value.as< TokenValue > ().Command() = yystack_[3].value.as< TokenValue > ().Value();
                                                            yylhs.value.as< TokenValue > ().AddArgument(yystack_[2].value.as< TokenValue > ());
                                                            yylhs.value.as< TokenValue > ().AddArgument(yystack_[1].value.as< TokenValue > ());
                                                        }
#line 730 "kif_parser.tab.cc" // lalr1.cc:846
    break;

  case 13:
#line 162 "kif_parser.yy" // lalr1.cc:846
    {
                            yylhs.value.as< TokenValue > () = yylhs.value.as< TokenValue > () + yystack_[0].value.as< TokenValue > ();
                            yylhs.value.as< TokenValue > ().Type() = TokenValue::Relation;
                            yylhs.value.as< TokenValue > ().Command() = yystack_[0].value.as< TokenValue > ().Value();
                        }
#line 740 "kif_parser.tab.cc" // lalr1.cc:846
    break;

  case 14:
#line 167 "kif_parser.yy" // lalr1.cc:846
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
#line 758 "kif_parser.tab.cc" // lalr1.cc:846
    break;

  case 15:
#line 180 "kif_parser.yy" // lalr1.cc:846
    {
                            yylhs.value.as< TokenValue > () = yylhs.value.as< TokenValue > () + yystack_[0].value.as< TokenValue > ();
                            yylhs.value.as< TokenValue > ().Type() = TokenValue::Relation;
                            yylhs.value.as< TokenValue > ().Command() = yystack_[0].value.as< TokenValue > ().Value();
                            std::string msg;
                            int res = driver.AddEntry(yylhs.value.as< TokenValue > (), true, 0, yystack_[0].location, msg);
                            if(res == 1) error(yystack_[0].location, msg);
                            else if(res == 2) driver.Warn(yystack_[0].location, msg);
                        }
#line 772 "kif_parser.tab.cc" // lalr1.cc:846
    break;

  case 16:
#line 190 "kif_parser.yy" // lalr1.cc:846
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
#line 790 "kif_parser.tab.cc" // lalr1.cc:846
    break;

  case 17:
#line 203 "kif_parser.yy" // lalr1.cc:846
    {
                            yylhs.value.as< TokenValue > () = yylhs.value.as< TokenValue > () + yystack_[0].value.as< TokenValue > ();
                            yylhs.value.as< TokenValue > ().Type() = TokenValue::Function;
                            yylhs.value.as< TokenValue > ().Command() = yystack_[0].value.as< TokenValue > ().Value();
                            std::string msg;
                            int res = driver.AddEntry(yylhs.value.as< TokenValue > (), false, 0, yystack_[0].location, msg);
                            if(res == 1) error(yystack_[0].location, msg);
                            else if(res == 2) driver.Warn(yystack_[0].location, msg);
                        }
#line 804 "kif_parser.tab.cc" // lalr1.cc:846
    break;

  case 18:
#line 213 "kif_parser.yy" // lalr1.cc:846
    {
                                            yylhs.value.as< TokenValue > () = yylhs.value.as< TokenValue > () + yystack_[1].value.as< TokenValue > ();
                                            if(yystack_[0].value.as< TokenValue > ().Value() != "") yylhs.value.as< TokenValue > () = yylhs.value.as< TokenValue > () + " " + yystack_[0].value.as< TokenValue > ();
                                            yylhs.value.as< TokenValue > ().Command() = "multiple terms";
                                            yylhs.value.as< TokenValue > ().AddArgument(yystack_[1].value.as< TokenValue > ());
                                            yylhs.value.as< TokenValue > ().AddArguments(yystack_[0].value.as< TokenValue > ().Arguments());
                                        }
#line 816 "kif_parser.tab.cc" // lalr1.cc:846
    break;

  case 19:
#line 220 "kif_parser.yy" // lalr1.cc:846
    { }
#line 822 "kif_parser.tab.cc" // lalr1.cc:846
    break;

  case 20:
#line 222 "kif_parser.yy" // lalr1.cc:846
    {
                        yylhs.value.as< TokenValue > () = yylhs.value.as< TokenValue > () + yystack_[0].value.as< TokenValue > ();
                        yylhs.value.as< TokenValue > ().Command() = yystack_[0].value.as< TokenValue > ().Value();
                    }
#line 831 "kif_parser.tab.cc" // lalr1.cc:846
    break;

  case 21:
#line 226 "kif_parser.yy" // lalr1.cc:846
    {
                        yylhs.value.as< TokenValue > () = yylhs.value.as< TokenValue > () + yystack_[0].value.as< TokenValue > ();
                        yylhs.value.as< TokenValue > ().Command() = yystack_[0].value.as< TokenValue > ().Value();
                    }
#line 840 "kif_parser.tab.cc" // lalr1.cc:846
    break;

  case 22:
#line 231 "kif_parser.yy" // lalr1.cc:846
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
#line 855 "kif_parser.tab.cc" // lalr1.cc:846
    break;

  case 23:
#line 242 "kif_parser.yy" // lalr1.cc:846
    {
                                                yylhs.value.as< TokenValue > () = yystack_[3].value.as< TokenValue > () + " " + yystack_[2].value.as< TokenValue > () + " " + yystack_[1].value.as< TokenValue > () + " " + yystack_[0].value.as< TokenValue > ();
                                                yylhs.value.as< TokenValue > ().Command() = yystack_[2].value.as< TokenValue > ().Value();
                                                yylhs.value.as< TokenValue > ().Type() = TokenValue::Relation;
                                                yylhs.value.as< TokenValue > ().AddArgument(yystack_[1].value.as< TokenValue > ());
                                            }
#line 866 "kif_parser.tab.cc" // lalr1.cc:846
    break;

  case 24:
#line 248 "kif_parser.yy" // lalr1.cc:846
    {
                                                yylhs.value.as< TokenValue > () = yystack_[3].value.as< TokenValue > () + " " + yystack_[2].value.as< TokenValue > () + " " + yystack_[1].value.as< TokenValue > () + " " + yystack_[0].value.as< TokenValue > ();
                                                yylhs.value.as< TokenValue > ().Command() = yystack_[2].value.as< TokenValue > ().Value();
                                                yylhs.value.as< TokenValue > ().Type() = TokenValue::Relation;
                                                yylhs.value.as< TokenValue > ().AddArgument(yystack_[1].value.as< TokenValue > ());
                                            }
#line 877 "kif_parser.tab.cc" // lalr1.cc:846
    break;

  case 25:
#line 254 "kif_parser.yy" // lalr1.cc:846
    {
                                                    yylhs.value.as< TokenValue > () = yystack_[4].value.as< TokenValue > () + " " + yystack_[3].value.as< TokenValue > () + " " + yystack_[2].value.as< TokenValue > () + " " + yystack_[1].value.as< TokenValue > () + " " + yystack_[0].value.as< TokenValue > ();
                                                    yylhs.value.as< TokenValue > ().Command() = yystack_[3].value.as< TokenValue > ().Value();
                                                    yylhs.value.as< TokenValue > ().Type() = TokenValue::Relation;
                                                    yylhs.value.as< TokenValue > ().AddArgument(yystack_[2].value.as< TokenValue > ());
                                                    yylhs.value.as< TokenValue > ().AddArguments(yystack_[1].value.as< TokenValue > ().Arguments());
                                                }
#line 889 "kif_parser.tab.cc" // lalr1.cc:846
    break;

  case 26:
#line 261 "kif_parser.yy" // lalr1.cc:846
    {
                                                yylhs.value.as< TokenValue > () = yystack_[3].value.as< TokenValue > () + " " + yystack_[2].value.as< TokenValue > () + " " + yystack_[1].value.as< TokenValue > () + " " + yystack_[0].value.as< TokenValue > ();
                                                yylhs.value.as< TokenValue > ().Command() = yystack_[2].value.as< TokenValue > ().Value();
                                                yylhs.value.as< TokenValue > ().Type() = TokenValue::Relation;
                                                yylhs.value.as< TokenValue > ().AddArgument(yystack_[1].value.as< TokenValue > ());
                                            }
#line 900 "kif_parser.tab.cc" // lalr1.cc:846
    break;

  case 27:
#line 267 "kif_parser.yy" // lalr1.cc:846
    {
                                                    yylhs.value.as< TokenValue > () = yystack_[4].value.as< TokenValue > () + " " + yystack_[3].value.as< TokenValue > () + " " + yystack_[2].value.as< TokenValue > () + " " + yystack_[1].value.as< TokenValue > () + " " + yystack_[0].value.as< TokenValue > ();
                                                    yylhs.value.as< TokenValue > ().Command() = yystack_[3].value.as< TokenValue > ().Value();
                                                    yylhs.value.as< TokenValue > ().Type() = TokenValue::Relation;
                                                    yylhs.value.as< TokenValue > ().AddArgument(yystack_[2].value.as< TokenValue > ());
                                                    yylhs.value.as< TokenValue > ().AddArguments(yystack_[1].value.as< TokenValue > ().Arguments());
                                                }
#line 912 "kif_parser.tab.cc" // lalr1.cc:846
    break;

  case 28:
#line 274 "kif_parser.yy" // lalr1.cc:846
    {
                                                yylhs.value.as< TokenValue > () = yystack_[3].value.as< TokenValue > () + " " + yystack_[2].value.as< TokenValue > () + " " + yystack_[1].value.as< TokenValue > () + " " + yystack_[0].value.as< TokenValue > ();
                                                yylhs.value.as< TokenValue > ().Command() = yystack_[2].value.as< TokenValue > ().Value();
                                                yylhs.value.as< TokenValue > ().Type() = TokenValue::Relation;
                                                yylhs.value.as< TokenValue > ().AddArgument(yystack_[1].value.as< TokenValue > ());
                                            }
#line 923 "kif_parser.tab.cc" // lalr1.cc:846
    break;

  case 29:
#line 280 "kif_parser.yy" // lalr1.cc:846
    {
                                                        yylhs.value.as< TokenValue > () = yystack_[4].value.as< TokenValue > () + " " + yystack_[3].value.as< TokenValue > () + " " + yystack_[2].value.as< TokenValue > () + " " + yystack_[1].value.as< TokenValue > () + " " + yystack_[0].value.as< TokenValue > ();
                                                        yylhs.value.as< TokenValue > ().Command() = yystack_[3].value.as< TokenValue > ().Value();
                                                        yylhs.value.as< TokenValue > ().Type() = TokenValue::Relation;
                                                        yylhs.value.as< TokenValue > ().AddArgument(yystack_[2].value.as< TokenValue > ());
                                                        yylhs.value.as< TokenValue > ().AddArguments(yystack_[1].value.as< TokenValue > ().Arguments());
                                                    }
#line 935 "kif_parser.tab.cc" // lalr1.cc:846
    break;

  case 30:
#line 287 "kif_parser.yy" // lalr1.cc:846
    {
                            yylhs.value.as< TokenValue > () = yylhs.value.as< TokenValue > () + yystack_[0].value.as< TokenValue > ();
                            yylhs.value.as< TokenValue > ().Command() = yystack_[0].value.as< TokenValue > ().Value();
                        }
#line 944 "kif_parser.tab.cc" // lalr1.cc:846
    break;

  case 31:
#line 291 "kif_parser.yy" // lalr1.cc:846
    {
                                                        yylhs.value.as< TokenValue > () = yystack_[4].value.as< TokenValue > () + " " + yystack_[3].value.as< TokenValue > () + " " + yystack_[2].value.as< TokenValue > ();
                                                        if(yystack_[1].value.as< TokenValue > () != "") yylhs.value.as< TokenValue > () = yylhs.value.as< TokenValue > () + " " + yystack_[1].value.as< TokenValue > ();
                                                        yylhs.value.as< TokenValue > () = yylhs.value.as< TokenValue > () + " " + yystack_[0].value.as< TokenValue > ();
                                                        yylhs.value.as< TokenValue > ().Command() = yystack_[3].value.as< TokenValue > ().Value();
                                                        yylhs.value.as< TokenValue > ().Type() = TokenValue::Relation;
                                                        yylhs.value.as< TokenValue > ().AddArgument(yystack_[2].value.as< TokenValue > ());
                                                        yylhs.value.as< TokenValue > ().AddArguments(yystack_[1].value.as< TokenValue > ().Arguments());
                                                        std::string msg;
                                                        int res = driver.AddEntry(yystack_[3].value.as< TokenValue > (), true, yystack_[1].value.as< TokenValue > ().Count() + 1, yystack_[3].location, msg);
                                                        if(res == 1) error(yystack_[3].location, msg);
                                                        else if(res == 2) driver.Warn(yystack_[3].location, msg);
                                                    }
#line 962 "kif_parser.tab.cc" // lalr1.cc:846
    break;

  case 32:
#line 304 "kif_parser.yy" // lalr1.cc:846
    {
                            yylhs.value.as< TokenValue > () = yylhs.value.as< TokenValue > () + yystack_[0].value.as< TokenValue > ();
                            yylhs.value.as< TokenValue > ().Command() = yystack_[0].value.as< TokenValue > ().Value();
                            yylhs.value.as< TokenValue > ().Type() = TokenValue::Relation;
                            std::string msg;
                            int res = driver.AddEntry(yystack_[0].value.as< TokenValue > (), true, 0, yystack_[0].location, msg);
                            if(res == 1) error(yystack_[0].location, msg);
                            else if(res == 2) driver.Warn(yystack_[0].location, msg);
                        }
#line 976 "kif_parser.tab.cc" // lalr1.cc:846
    break;

  case 33:
#line 314 "kif_parser.yy" // lalr1.cc:846
    {
                                                        yylhs.value.as< TokenValue > () = yystack_[4].value.as< TokenValue > () + " " + yystack_[3].value.as< TokenValue > () + " " + yystack_[2].value.as< TokenValue > () + " " + yystack_[1].value.as< TokenValue > () + " " + yystack_[0].value.as< TokenValue > ();
                                                        yylhs.value.as< TokenValue > ().Command() = yystack_[3].value.as< TokenValue > ().Value();
                                                        yylhs.value.as< TokenValue > ().Type() = TokenValue::Relation;
                                                        yylhs.value.as< TokenValue > ().AddArgument(yystack_[2].value.as< TokenValue > ());
                                                        yylhs.value.as< TokenValue > ().AddArguments(yystack_[1].value.as< TokenValue > ().Arguments());
                                                    }
#line 988 "kif_parser.tab.cc" // lalr1.cc:846
    break;

  case 34:
#line 321 "kif_parser.yy" // lalr1.cc:846
    {
                                                yylhs.value.as< TokenValue > () = yystack_[3].value.as< TokenValue > () + " " + yystack_[2].value.as< TokenValue > () + " " + yystack_[1].value.as< TokenValue > () + " " + yystack_[0].value.as< TokenValue > ();
                                                yylhs.value.as< TokenValue > ().Command() = yystack_[2].value.as< TokenValue > ().Value();
                                                yylhs.value.as< TokenValue > ().Type() = TokenValue::Relation;
                                                yylhs.value.as< TokenValue > ().AddArgument(yystack_[1].value.as< TokenValue > ());
                                            }
#line 999 "kif_parser.tab.cc" // lalr1.cc:846
    break;

  case 35:
#line 327 "kif_parser.yy" // lalr1.cc:846
    {
                                                    yylhs.value.as< TokenValue > () = yystack_[3].value.as< TokenValue > () + " " + yystack_[2].value.as< TokenValue > () + " " + yystack_[1].value.as< TokenValue > () + " " + yystack_[0].value.as< TokenValue > ();
                                                    yylhs.value.as< TokenValue > ().PerformNot();
                                                    yylhs.value.as< TokenValue > ().Command() = yystack_[2].value.as< TokenValue > ().Value();
                                                    yylhs.value.as< TokenValue > ().Type() = TokenValue::Relation;
                                                    yylhs.value.as< TokenValue > ().AddArgument(yystack_[1].value.as< TokenValue > ());
                                                }
#line 1011 "kif_parser.tab.cc" // lalr1.cc:846
    break;

  case 36:
#line 334 "kif_parser.yy" // lalr1.cc:846
    {
                                                                yylhs.value.as< TokenValue > () = yystack_[4].value.as< TokenValue > () + " " + yystack_[3].value.as< TokenValue > () + " " + yystack_[2].value.as< TokenValue > () + " " + yystack_[1].value.as< TokenValue > ().Value() + " " + yystack_[0].value.as< TokenValue > ();
                                                                yylhs.value.as< TokenValue > ().OrRestrict(yystack_[1].value.as< TokenValue > ());
                                                                yylhs.value.as< TokenValue > ().Command() = yystack_[3].value.as< TokenValue > ().Value();
                                                                yylhs.value.as< TokenValue > ().Type() = TokenValue::Relation;
                                                                yylhs.value.as< TokenValue > ().AddArgument(yystack_[2].value.as< TokenValue > ());
                                                                yylhs.value.as< TokenValue > ().AddArguments(yystack_[1].value.as< TokenValue > ().Arguments());
                                                            }
#line 1024 "kif_parser.tab.cc" // lalr1.cc:846
    break;

  case 37:
#line 342 "kif_parser.yy" // lalr1.cc:846
    {
                                                            yylhs.value.as< TokenValue > () = yystack_[4].value.as< TokenValue > () + " " + yystack_[3].value.as< TokenValue > () + " " + yystack_[2].value.as< TokenValue > () + " " + yystack_[1].value.as< TokenValue > () + " " + yystack_[0].value.as< TokenValue > ();
                                                            yylhs.value.as< TokenValue > ().PerformNot();
                                                            yylhs.value.as< TokenValue > ().Command() = yystack_[3].value.as< TokenValue > ().Value();
                                                            yylhs.value.as< TokenValue > ().Type() = TokenValue::Relation;
                                                            yylhs.value.as< TokenValue > ().AddArgument(yystack_[2].value.as< TokenValue > ());
                                                            yylhs.value.as< TokenValue > ().AddArguments(yystack_[1].value.as< TokenValue > ().Arguments());
                                                        }
#line 1037 "kif_parser.tab.cc" // lalr1.cc:846
    break;

  case 38:
#line 350 "kif_parser.yy" // lalr1.cc:846
    {
                                                yylhs.value.as< TokenValue > () = yystack_[3].value.as< TokenValue > () + " " + yystack_[2].value.as< TokenValue > () + " " + yystack_[1].value.as< TokenValue > () + " " + yystack_[0].value.as< TokenValue > ();
                                                yylhs.value.as< TokenValue > ().Command() = yystack_[2].value.as< TokenValue > ().Value();
                                                yylhs.value.as< TokenValue > ().Type() = TokenValue::Relation;
                                                yylhs.value.as< TokenValue > ().AddArgument(yystack_[1].value.as< TokenValue > ());
                                            }
#line 1048 "kif_parser.tab.cc" // lalr1.cc:846
    break;

  case 39:
#line 356 "kif_parser.yy" // lalr1.cc:846
    {
                                                yylhs.value.as< TokenValue > () = yystack_[3].value.as< TokenValue > () + " " + yystack_[2].value.as< TokenValue > () + " " + yystack_[1].value.as< TokenValue > () + " " + yystack_[0].value.as< TokenValue > ();
                                                yylhs.value.as< TokenValue > ().Command() = yystack_[2].value.as< TokenValue > ().Value();
                                                yylhs.value.as< TokenValue > ().Type() = TokenValue::Relation;
                                                yylhs.value.as< TokenValue > ().AddArgument(yystack_[1].value.as< TokenValue > ());
                                            }
#line 1059 "kif_parser.tab.cc" // lalr1.cc:846
    break;

  case 40:
#line 362 "kif_parser.yy" // lalr1.cc:846
    {
                                                        yylhs.value.as< TokenValue > () = yystack_[4].value.as< TokenValue > () + " " + yystack_[3].value.as< TokenValue > () + " " + yystack_[2].value.as< TokenValue > () + " " + yystack_[1].value.as< TokenValue > () + " " + yystack_[0].value.as< TokenValue > ();
                                                        yylhs.value.as< TokenValue > ().Command() = yystack_[3].value.as< TokenValue > ().Value();
                                                        yylhs.value.as< TokenValue > ().Type() = TokenValue::Relation;
                                                        yylhs.value.as< TokenValue > ().AddArgument(yystack_[2].value.as< TokenValue > ());
                                                        yylhs.value.as< TokenValue > ().AddArguments(yystack_[1].value.as< TokenValue > ().Arguments());
                                                    }
#line 1071 "kif_parser.tab.cc" // lalr1.cc:846
    break;

  case 41:
#line 369 "kif_parser.yy" // lalr1.cc:846
    {
                                                        yylhs.value.as< TokenValue > () = yystack_[4].value.as< TokenValue > () + " " + yystack_[3].value.as< TokenValue > () + " " + yystack_[2].value.as< TokenValue > () + " " + yystack_[1].value.as< TokenValue > () + " " + yystack_[0].value.as< TokenValue > ();
                                                        yylhs.value.as< TokenValue > ().Command() = yystack_[3].value.as< TokenValue > ().Value();
                                                        yylhs.value.as< TokenValue > ().Type() = TokenValue::Relation;
                                                        yylhs.value.as< TokenValue > ().AddArgument(yystack_[2].value.as< TokenValue > ());
                                                        yylhs.value.as< TokenValue > ().AddArguments(yystack_[1].value.as< TokenValue > ().Arguments());
                                                    }
#line 1083 "kif_parser.tab.cc" // lalr1.cc:846
    break;

  case 42:
#line 376 "kif_parser.yy" // lalr1.cc:846
    {
                                                yylhs.value.as< TokenValue > () = yystack_[3].value.as< TokenValue > () + " " + yystack_[2].value.as< TokenValue > () + " " + yystack_[1].value.as< TokenValue > () + " " + yystack_[0].value.as< TokenValue > ();
                                                yylhs.value.as< TokenValue > ().Command() = yystack_[2].value.as< TokenValue > ().Value();
                                                yylhs.value.as< TokenValue > ().Type() = TokenValue::Relation;
                                                yylhs.value.as< TokenValue > ().AddArgument(yystack_[1].value.as< TokenValue > ());
                                            }
#line 1094 "kif_parser.tab.cc" // lalr1.cc:846
    break;

  case 43:
#line 382 "kif_parser.yy" // lalr1.cc:846
    {
                                                        yylhs.value.as< TokenValue > () = yystack_[4].value.as< TokenValue > () + " " + yystack_[3].value.as< TokenValue > () + " " + yystack_[2].value.as< TokenValue > () + " " + yystack_[1].value.as< TokenValue > () + " " + yystack_[0].value.as< TokenValue > ();
                                                        yylhs.value.as< TokenValue > ().Command() = yystack_[3].value.as< TokenValue > ().Value();
                                                        yylhs.value.as< TokenValue > ().Type() = TokenValue::Relation;
                                                        yylhs.value.as< TokenValue > ().AddArgument(yystack_[2].value.as< TokenValue > ());
                                                        yylhs.value.as< TokenValue > ().AddArguments(yystack_[1].value.as< TokenValue > ().Arguments());
                                                    }
#line 1106 "kif_parser.tab.cc" // lalr1.cc:846
    break;

  case 44:
#line 389 "kif_parser.yy" // lalr1.cc:846
    {
                            yylhs.value.as< TokenValue > () = yylhs.value.as< TokenValue > () + yystack_[0].value.as< TokenValue > ();
                            yylhs.value.as< TokenValue > ().Command() = yystack_[0].value.as< TokenValue > ().Value();
                        }
#line 1115 "kif_parser.tab.cc" // lalr1.cc:846
    break;

  case 45:
#line 393 "kif_parser.yy" // lalr1.cc:846
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
#line 1133 "kif_parser.tab.cc" // lalr1.cc:846
    break;

  case 46:
#line 406 "kif_parser.yy" // lalr1.cc:846
    {
                            yylhs.value.as< TokenValue > () = yylhs.value.as< TokenValue > () + yystack_[0].value.as< TokenValue > ();
                            yylhs.value.as< TokenValue > ().Command() = yystack_[0].value.as< TokenValue > ().Value();
                            yylhs.value.as< TokenValue > ().Type() = TokenValue::Relation;
                            std::string msg;
                            int res = driver.CheckEntry(yystack_[0].value.as< TokenValue > (), true, 0, yystack_[0].location, msg);
                            if(res == 1) error(yystack_[0].location, msg);
                            else if(res == 2) driver.Warn(yystack_[0].location, msg);
                        }
#line 1147 "kif_parser.tab.cc" // lalr1.cc:846
    break;

  case 47:
#line 416 "kif_parser.yy" // lalr1.cc:846
    {
                                        yylhs.value.as< TokenValue > () = yylhs.value.as< TokenValue > () + yystack_[1].value.as< TokenValue > ();
                                        if(yystack_[0].value.as< TokenValue > () != "") yylhs.value.as< TokenValue > () = yylhs.value.as< TokenValue > () + " " + yystack_[0].value.as< TokenValue > ();
                                        yylhs.value.as< TokenValue > ().Command() = "Multiple relations";
                                        yylhs.value.as< TokenValue > ().AddArgument(yystack_[1].value.as< TokenValue > ());
                                        yylhs.value.as< TokenValue > ().AddArguments(yystack_[0].value.as< TokenValue > ().Arguments());
                                    }
#line 1159 "kif_parser.tab.cc" // lalr1.cc:846
    break;

  case 48:
#line 423 "kif_parser.yy" // lalr1.cc:846
    { }
#line 1165 "kif_parser.tab.cc" // lalr1.cc:846
    break;

  case 49:
#line 425 "kif_parser.yy" // lalr1.cc:846
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
#line 1183 "kif_parser.tab.cc" // lalr1.cc:846
    break;

  case 50:
#line 438 "kif_parser.yy" // lalr1.cc:846
    {
                            yylhs.value.as< TokenValue > () = yylhs.value.as< TokenValue > () + yystack_[0].value.as< TokenValue > ();
                            yylhs.value.as< TokenValue > ().Command() = yystack_[0].value.as< TokenValue > ().Value();
                            yylhs.value.as< TokenValue > ().Type() = TokenValue::Function;
                            std::string msg;
                            int res = driver.AddEntry(yystack_[0].value.as< TokenValue > (), false, 0, yystack_[0].location, msg);
                            if(res == 1) error(yystack_[0].location, msg);
                            else if(res == 2) driver.Warn(yystack_[0].location, msg);
                        }
#line 1197 "kif_parser.tab.cc" // lalr1.cc:846
    break;

  case 51:
#line 447 "kif_parser.yy" // lalr1.cc:846
    {
                        yylhs.value.as< TokenValue > () = yylhs.value.as< TokenValue > () + yystack_[0].value.as< TokenValue > ();
                        yylhs.value.as< TokenValue > ().AddVariable(yystack_[0].value.as< TokenValue > ());
                        yylhs.value.as< TokenValue > ().Command() = yystack_[0].value.as< TokenValue > ().Value();
                        yylhs.value.as< TokenValue > ().Type() = TokenValue::Var;
                    }
#line 1208 "kif_parser.tab.cc" // lalr1.cc:846
    break;

  case 52:
#line 454 "kif_parser.yy" // lalr1.cc:846
    {
                                yylhs.value.as< TokenValue > () = yylhs.value.as< TokenValue > () + yystack_[1].value.as< TokenValue > ();
                                if(yystack_[0].value.as< TokenValue > () != "") yylhs.value.as< TokenValue > () = yylhs.value.as< TokenValue > () + " " + yystack_[0].value.as< TokenValue > ();
                                yylhs.value.as< TokenValue > ().Command() = "Multiple terms";
                                yylhs.value.as< TokenValue > ().AddArgument(yystack_[1].value.as< TokenValue > ());
                                yylhs.value.as< TokenValue > ().AddArguments(yystack_[0].value.as< TokenValue > ().Arguments());
                            }
#line 1220 "kif_parser.tab.cc" // lalr1.cc:846
    break;

  case 53:
#line 462 "kif_parser.yy" // lalr1.cc:846
    { }
#line 1226 "kif_parser.tab.cc" // lalr1.cc:846
    break;

  case 54:
#line 464 "kif_parser.yy" // lalr1.cc:846
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
#line 1244 "kif_parser.tab.cc" // lalr1.cc:846
    break;

  case 55:
#line 477 "kif_parser.yy" // lalr1.cc:846
    {
                            yylhs.value.as< TokenValue > () = yylhs.value.as< TokenValue > () + yystack_[0].value.as< TokenValue > ();
                            yylhs.value.as< TokenValue > ().Command() = yystack_[0].value.as< TokenValue > ().Value();
                            yylhs.value.as< TokenValue > ().Type() = TokenValue::Function;
                            std::string msg;
                            int res = driver.CheckEntry(yystack_[0].value.as< TokenValue > (), false, 0, yystack_[0].location, msg);
                            if(res == 1) error(yystack_[0].location, msg);
                            else if(res == 2) driver.Warn(yystack_[0].location, msg);
                        }
#line 1258 "kif_parser.tab.cc" // lalr1.cc:846
    break;

  case 56:
#line 486 "kif_parser.yy" // lalr1.cc:846
    {
                        yylhs.value.as< TokenValue > () = yylhs.value.as< TokenValue > () + yystack_[0].value.as< TokenValue > ();
                        yylhs.value.as< TokenValue > ().AddVariable(yystack_[0].value.as< TokenValue > ());
                        yylhs.value.as< TokenValue > ().Command() = yystack_[0].value.as< TokenValue > ().Value();
                        yylhs.value.as< TokenValue > ().Type() = TokenValue::Var;
                    }
#line 1269 "kif_parser.tab.cc" // lalr1.cc:846
    break;

  case 57:
#line 493 "kif_parser.yy" // lalr1.cc:846
    {
                                yylhs.value.as< TokenValue > () = yylhs.value.as< TokenValue > () + yystack_[1].value.as< TokenValue > ();
                                if(yystack_[0].value.as< TokenValue > ().Value() != "") yylhs.value.as< TokenValue > () = yylhs.value.as< TokenValue > () + " " + yystack_[0].value.as< TokenValue > ();
                                yylhs.value.as< TokenValue > ().Command() = "Multiple terms";
                                yylhs.value.as< TokenValue > ().Type() = TokenValue::Function;
                                yylhs.value.as< TokenValue > ().AddArgument(yystack_[1].value.as< TokenValue > ());
                                yylhs.value.as< TokenValue > ().AddArguments(yystack_[0].value.as< TokenValue > ().Arguments());
                            }
#line 1282 "kif_parser.tab.cc" // lalr1.cc:846
    break;

  case 58:
#line 502 "kif_parser.yy" // lalr1.cc:846
    { }
#line 1288 "kif_parser.tab.cc" // lalr1.cc:846
    break;

  case 59:
#line 504 "kif_parser.yy" // lalr1.cc:846
    {
                        yylhs.value.as< TokenValue > () = yylhs.value.as< TokenValue > () + yystack_[0].value.as< TokenValue > ();
                        yylhs.value.as< TokenValue > ().Command() = yystack_[0].value.as< TokenValue > ().Value();
                        yylhs.value.as< TokenValue > ().Type() = TokenValue::Function;
                    }
#line 1298 "kif_parser.tab.cc" // lalr1.cc:846
    break;

  case 60:
#line 509 "kif_parser.yy" // lalr1.cc:846
    {
                        yylhs.value.as< TokenValue > () = yylhs.value.as< TokenValue > () + yystack_[0].value.as< TokenValue > ();
                        yylhs.value.as< TokenValue > ().AddVariable(yystack_[0].value.as< TokenValue > ());
                        yylhs.value.as< TokenValue > ().Type() = TokenValue::Var;
                    }
#line 1308 "kif_parser.tab.cc" // lalr1.cc:846
    break;


#line 1312 "kif_parser.tab.cc" // lalr1.cc:846
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
      57,   153,  -121,  -121,  -121,     5,    20,    57,  -121,    57,
      21,     1,     1,     1,     1,     1,     1,    82,     1,  -121,
    -121,  -121,  -121,    23,    21,     1,  -121,    26,    39,     1,
      50,    48,   180,  -121,  -121,    85,     1,  -121,     1,    54,
    -121,  -121,    61,  -121,    62,    34,    34,    34,    34,    34,
      34,    34,    34,   164,  -121,  -121,    85,     1,    67,     1,
    -121,  -121,  -121,    21,  -121,  -121,    80,    34,    90,    94,
      34,    98,    27,    34,   125,   125,   125,   125,   125,   125,
     125,   125,    85,    85,   125,   125,    85,   100,  -121,  -121,
     101,    34,  -121,   110,  -121,  -121,   114,  -121,  -121,  -121,
     115,    34,   129,    21,  -121,  -121,   130,   125,   131,   125,
     133,    27,   125,   134,   138,    85,   125,   125,  -121,  -121,
    -121,    34,  -121,  -121,  -121,  -121,  -121,   125,  -121,   139,
    -121,   141,  -121,   142,   143,  -121,  -121,   146,   147,   125,
     148,   149,   125,  -121,  -121,  -121,  -121,  -121,  -121,  -121,
    -121,  -121,   150,  -121
  };

  const unsigned char
  KIFParser::yydefact_[] =
  {
       5,     0,    20,    21,    13,     0,     0,     5,    15,     5,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     1,
       2,     3,     4,     0,     0,     0,    17,     0,     0,     0,
       0,     0,     0,    30,    32,     0,    19,     6,     0,     0,
       9,     7,     0,    11,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    44,    46,    48,    19,     0,    19,
       8,    10,    12,     0,    51,    50,     0,     0,     0,     0,
       0,     0,     0,    53,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    48,     0,    18,    14,
       0,     0,    23,     0,    26,    24,     0,    28,    60,    59,
       0,    53,     0,     0,    56,    55,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    48,     0,    58,    47,    22,
      16,    53,    25,    27,    29,    52,    31,     0,    38,     0,
      39,     0,    42,     0,     0,    34,    35,     0,     0,    58,
       0,     0,    58,    40,    41,    43,    33,    36,    37,    57,
      45,    49,     0,    54
  };

  const signed char
  KIFParser::yypgoto_[] =
  {
    -121,  -121,    35,  -121,   111,    36,     0,  -121,  -121,   -14,
     -84,    29,   -98,    32,  -120,   -50
  };

  const short int
  KIFParser::yydefgoto_[] =
  {
      -1,     5,     6,     7,    57,    58,   105,     9,    35,    86,
      87,   101,   102,   139,   140,   100
  };

  const unsigned char
  KIFParser::yytable_[] =
  {
       8,    18,   118,   125,    24,    19,     2,     8,     3,     8,
      23,    26,    26,    26,    26,    26,    26,    34,    26,   149,
      20,    56,   152,   141,    38,    26,     2,    37,     3,    26,
      40,   137,    52,    98,    99,    55,    26,    63,    26,     2,
      64,     3,    21,    41,    22,    65,    65,    65,    65,    65,
      65,    65,    65,    85,    43,    44,    55,    26,    60,    26,
       1,   133,     2,    91,     3,    61,    62,    65,   114,   115,
      65,    89,     4,    65,    66,    67,    68,    69,    70,    71,
      72,    73,    55,    55,    92,    32,    55,     2,    53,     3,
       2,    65,     3,    88,    94,    90,    93,    33,    95,    96,
      54,    65,    97,   127,   119,   120,   106,   107,   108,   109,
     110,   111,   112,   113,   122,    55,   116,   117,   123,   124,
     121,    65,    25,    27,    28,    29,    30,    31,   103,    36,
       2,   104,     3,   126,   128,   130,    39,   132,   135,   129,
      42,   131,   136,   143,   134,   144,   145,   146,   138,    59,
     147,   148,   150,   151,   153,     0,     0,     0,     2,   142,
       3,    10,    11,    12,    13,    14,    15,    16,     0,     2,
       0,     3,    74,    75,    17,    76,    77,    78,    79,     0,
      80,    81,    82,    83,    84,     2,     0,     3,    45,    46,
      47,    48,    49,    50,    51
  };

  const short int
  KIFParser::yycheck_[] =
  {
       0,     1,    86,   101,     3,     0,     5,     7,     7,     9,
      10,    11,    12,    13,    14,    15,    16,    17,    18,   139,
       0,    35,   142,   121,    24,    25,     5,     4,     7,    29,
       4,   115,    32,     6,     7,    35,    36,     3,    38,     5,
       6,     7,     7,     4,     9,    45,    46,    47,    48,    49,
      50,    51,    52,    53,     4,     7,    56,    57,     4,    59,
       3,   111,     5,    63,     7,     4,     4,    67,    82,    83,
      70,     4,    15,    73,    45,    46,    47,    48,    49,    50,
      51,    52,    82,    83,     4,     3,    86,     5,     3,     7,
       5,    91,     7,    57,     4,    59,    67,    15,     4,    70,
      15,   101,     4,   103,     4,     4,    74,    75,    76,    77,
      78,    79,    80,    81,     4,   115,    84,    85,     4,     4,
      91,   121,    11,    12,    13,    14,    15,    16,     3,    18,
       5,     6,     7,     4,     4,     4,    25,     4,     4,   107,
      29,   109,     4,     4,   112,     4,     4,     4,   116,    38,
       4,     4,     4,     4,     4,    -1,    -1,    -1,     5,   127,
       7,     8,     9,    10,    11,    12,    13,    14,    -1,     5,
      -1,     7,     8,     9,    21,    11,    12,    13,    14,    -1,
      16,    17,    18,    19,    20,     5,    -1,     7,     8,     9,
      10,    11,    12,    13,    14
  };

  const unsigned char
  KIFParser::yystos_[] =
  {
       0,     3,     5,     7,    15,    23,    24,    25,    28,    29,
       8,     9,    10,    11,    12,    13,    14,    21,    28,     0,
       0,    24,    24,    28,     3,    26,    28,    26,    26,    26,
      26,    26,     3,    15,    28,    30,    26,     4,    28,    26,
       4,     4,    26,     4,     7,     8,     9,    10,    11,    12,
      13,    14,    28,     3,    15,    28,    31,    26,    27,    26,
       4,     4,     4,     3,     6,    28,    33,    33,    33,    33,
      33,    33,    33,    33,     8,     9,    11,    12,    13,    14,
      16,    17,    18,    19,    20,    28,    31,    32,    27,     4,
      27,    28,     4,    33,     4,     4,    33,     4,     6,     7,
      37,    33,    34,     3,     6,    28,    35,    35,    35,    35,
      35,    35,    35,    35,    31,    31,    35,    35,    32,     4,
       4,    33,     4,     4,     4,    34,     4,    28,     4,    35,
       4,    35,     4,    37,    35,     4,     4,    32,    35,    35,
      36,    34,    35,     4,     4,     4,     4,     4,     4,    36,
       4,     4,    36,     4
  };

  const unsigned char
  KIFParser::yyr1_[] =
  {
       0,    22,    23,    24,    24,    24,    25,    25,    25,    25,
      25,    25,    25,    25,    25,    25,    26,    26,    27,    27,
      28,    28,    29,    30,    30,    30,    30,    30,    30,    30,
      30,    30,    30,    31,    31,    31,    31,    31,    31,    31,
      31,    31,    31,    31,    31,    31,    31,    32,    32,    33,
      33,    33,    34,    34,    35,    35,    35,    36,    36,    37,
      37
  };

  const unsigned char
  KIFParser::yyr2_[] =
  {
       0,     2,     2,     2,     2,     0,     4,     4,     5,     4,
       5,     4,     5,     1,     5,     1,     5,     1,     2,     0,
       1,     1,     6,     4,     4,     5,     4,     5,     4,     5,
       1,     5,     1,     5,     4,     4,     5,     5,     4,     4,
       5,     5,     4,     5,     1,     5,     1,     2,     0,     5,
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
  "ORelations", "Term", "Terms", "CTerm", "CTerms", "Num_var", YY_NULL
  };

#if YYDEBUG
  const unsigned short int
  KIFParser::yyrline_[] =
  {
       0,   107,   107,   109,   112,   115,   117,   123,   129,   136,
     142,   149,   155,   162,   167,   180,   190,   203,   213,   220,
     222,   226,   231,   242,   248,   254,   261,   267,   274,   280,
     287,   291,   304,   314,   321,   327,   334,   342,   350,   356,
     362,   369,   376,   382,   389,   393,   406,   416,   423,   425,
     438,   447,   454,   462,   464,   477,   486,   493,   502,   504,
     509
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
#line 1785 "kif_parser.tab.cc" // lalr1.cc:1156
#line 515 "kif_parser.yy" // lalr1.cc:1157


  typedef gdlparser::parser::yy::KIFParser KIFParser;

  // Mandatory error function
  void KIFParser::error (const KIFParser::location_type& loc, const std::string& msg)
  {
    driver.Error(loc, msg);
  }
