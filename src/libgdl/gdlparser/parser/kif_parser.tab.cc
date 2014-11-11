// A Bison parser, made by GNU Bison 3.0.2.

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

#line 37 "kif_parser.tab.cc" // lalr1.cc:399

# ifndef YY_NULLPTR
#  if defined __cplusplus && 201103L <= __cplusplus
#   define YY_NULLPTR nullptr
#  else
#   define YY_NULLPTR 0
#  endif
# endif

#include "kif_parser.tab.hh"

// User implementation prologue.

#line 51 "kif_parser.tab.cc" // lalr1.cc:407
// Unqualified %code blocks.
#line 83 "kif_parser.yy" // lalr1.cc:408

#include <algorithm>
#include <iostream>
#include <iterator>
#include <sstream>

#include "syntax_tree_types_includes.hpp"
#include <libgdl/core/util/to_string.hpp>
#include <libgdl/core/symbol_table/symbol_table.hpp>
#include <libgdl/core/data_types/str_var_map.hpp>
#include <libgdl/core/data_types/error_type.hpp>

// driver class declaration.
#include "kif_driver.hpp"

// use yylex function in scanner class instead of predefiend yylex
#undef yylex
#define yylex scanner.lex


#line 74 "kif_parser.tab.cc" // lalr1.cc:408


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

#line 23 "kif_parser.yy" // lalr1.cc:474
namespace libgdl { namespace gdlparser { namespace parser { namespace yy {
#line 160 "kif_parser.tab.cc" // lalr1.cc:474

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

  inline
  KIFParser::syntax_error::syntax_error (const location_type& l, const std::string& m)
    : std::runtime_error (m)
    , location (l)
  {}

  // basic_symbol.
  template <typename Base>
  inline
  KIFParser::basic_symbol<Base>::basic_symbol ()
    : value ()
  {}

  template <typename Base>
  inline
  KIFParser::basic_symbol<Base>::basic_symbol (const basic_symbol& other)
    : Base (other)
    , value ()
    , location (other.location)
  {
    value = other.value;
  }


  template <typename Base>
  inline
  KIFParser::basic_symbol<Base>::basic_symbol (typename Base::kind_type t, const semantic_type& v, const location_type& l)
    : Base (t)
    , value (v)
    , location (l)
  {}


  /// Constructor for valueless symbols.
  template <typename Base>
  inline
  KIFParser::basic_symbol<Base>::basic_symbol (typename Base::kind_type t, const location_type& l)
    : Base (t)
    , value ()
    , location (l)
  {}

  template <typename Base>
  inline
  KIFParser::basic_symbol<Base>::~basic_symbol ()
  {
  }

  template <typename Base>
  inline
  void
  KIFParser::basic_symbol<Base>::move (basic_symbol& s)
  {
    super_type::move(s);
    value = s.value;
    location = s.location;
  }

  // by_type.
  inline
  KIFParser::by_type::by_type ()
     : type (empty)
  {}

  inline
  KIFParser::by_type::by_type (const by_type& other)
    : type (other.type)
  {}

  inline
  KIFParser::by_type::by_type (token_type t)
    : type (yytranslate_ (t))
  {}

  inline
  void
  KIFParser::by_type::move (by_type& that)
  {
    type = that.type;
    that.type = empty;
  }

  inline
  int
  KIFParser::by_type::type_get () const
  {
    return type;
  }


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
    value = that.value;
    // that is emptied.
    that.type = empty;
  }

  inline
  KIFParser::stack_symbol_type&
  KIFParser::stack_symbol_type::operator= (const stack_symbol_type& that)
  {
    state = that.state;
    value = that.value;
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

    // User destructor.
    YYUSE (yysym.type_get ());
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
  KIFParser::yy_lr_goto_state_ (state_type yystate, int yysym)
  {
    int yyr = yypgoto_[yysym - yyntokens_] + yystate;
    if (0 <= yyr && yyr <= yylast_ && yycheck_[yyr] == yystate)
      return yytable_[yyr];
    else
      return yydefgoto_[yysym - yyntokens_];
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
    /// Length of the RHS of the rule being reduced.
    int yylen = 0;

    // Error handling.
    int yynerrs_ = 0;
    int yyerrstatus_ = 0;

    /// The lookahead symbol.
    symbol_type yyla;

    /// The locations where the error started and ended.
    stack_symbol_type yyerror_range[3];

    /// The return value of parse ().
    int yyresult;

    // FIXME: This shoud be completely indented.  It is not yet to
    // avoid gratuitous conflicts when merging into the master branch.
    try
      {
    YYCDEBUG << "Starting parse" << std::endl;


    // User initialization code.
    #line 34 "kif_parser.yy" // lalr1.cc:725
{
    // initialize the initial location object
    yyla.location.begin.filename = yyla.location.end.filename = &scanner.CurrentFile();
}

#line 505 "kif_parser.tab.cc" // lalr1.cc:725

    /* Initialize the stack.  The initial state will be set in
       yynewstate, since the latter expects the semantical and the
       location values to have been already stored, initialize these
       stacks with a primary value.  */
    yystack_.clear ();
    yypush_ (YY_NULLPTR, 0, yyla);

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
            yyla.type = yytranslate_ (yylex (&yyla.value, &yyla.location));
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
    {
      stack_symbol_type yylhs;
      yylhs.state = yy_lr_goto_state_(yystack_[yylen].state, yyr1_[yyn]);
      /* If YYLEN is nonzero, implement the default value of the
         action: '$$ = $1'.  Otherwise, use the top of the stack.

         Otherwise, the following line sets YYLHS.VALUE to garbage.
         This behavior is undocumented and Bison users should not rely
         upon it.  */
      if (yylen)
        yylhs.value = yystack_[yylen - 1].value;
      else
        yylhs.value = yystack_[0].value;

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
  case 2:
#line 136 "kif_parser.yy" // lalr1.cc:847
    {
                        core::StrVarMap v_map;
                        (yystack_[1].value.node)->CodeGen(driver, v_map);
                        delete (yystack_[1].value.node);
                      }
#line 623 "kif_parser.tab.cc" // lalr1.cc:847
    break;

  case 3:
#line 141 "kif_parser.yy" // lalr1.cc:847
    {
                      (yylhs.value.node) = NULL;
                    }
#line 631 "kif_parser.tab.cc" // lalr1.cc:847
    break;

  case 4:
#line 145 "kif_parser.yy" // lalr1.cc:847
    {
                    (yylhs.value.node) = (yystack_[0].value.sentence);
                  }
#line 639 "kif_parser.tab.cc" // lalr1.cc:847
    break;

  case 5:
#line 148 "kif_parser.yy" // lalr1.cc:847
    {
                      (yylhs.value.node) = (yystack_[0].value.node);
                    }
#line 647 "kif_parser.tab.cc" // lalr1.cc:847
    break;

  case 6:
#line 152 "kif_parser.yy" // lalr1.cc:847
    {
                        (yylhs.value.sentence) = (yystack_[0].value.sentence);
                      }
#line 655 "kif_parser.tab.cc" // lalr1.cc:847
    break;

  case 7:
#line 157 "kif_parser.yy" // lalr1.cc:847
    {
                        if((yystack_[1].value.premisses) != NULL)
                        {
                          ClauseConstruct* temp = new ClauseConstruct((yystack_[2].value.sentence), yylhs.location);
                          temp->AddArgument(*(yystack_[1].value.premisses));
                          delete (yystack_[1].value.premisses);
                          (yylhs.value.node) = temp;
                        }
                        else
                        {
                          libgdl::core::ErrorType war;
                          war.AddEntry("Clause considered as fact as it contains no premisses.", yylhs.location);
                          driver.Warn(war);
                          (yylhs.value.node) = (yystack_[2].value.sentence);
                        }
                      }
#line 676 "kif_parser.tab.cc" // lalr1.cc:847
    break;

  case 8:
#line 174 "kif_parser.yy" // lalr1.cc:847
    {
                        (yylhs.value.premiss) = new Premiss(new std::string(""), yylhs.location);
                        (yylhs.value.premiss)->AddArgument((yystack_[0].value.sentence));
                      }
#line 685 "kif_parser.tab.cc" // lalr1.cc:847
    break;

  case 9:
#line 179 "kif_parser.yy" // lalr1.cc:847
    {
                      (yylhs.value.premiss) = new Premiss(new std::string("not"), yylhs.location);
                      (yylhs.value.premiss)->AddArgument((yystack_[1].value.sentence));
                    }
#line 694 "kif_parser.tab.cc" // lalr1.cc:847
    break;

  case 10:
#line 184 "kif_parser.yy" // lalr1.cc:847
    {
                      (yylhs.value.premiss) = new Premiss(new std::string("or"), yylhs.location);
                      (yylhs.value.premiss)->AddArgument((yystack_[2].value.sentence));
                      if((yystack_[1].value.sentences) != NULL) (yylhs.value.premiss)->AddArgument(*(yystack_[1].value.sentences));
                      delete (yystack_[1].value.sentences);
                    }
#line 705 "kif_parser.tab.cc" // lalr1.cc:847
    break;

  case 11:
#line 191 "kif_parser.yy" // lalr1.cc:847
    {
                                if((yystack_[0].value.premisses) == NULL)
                                  (yylhs.value.premisses) = new std::list<Premiss*>();
                                else (yylhs.value.premisses) = (yystack_[0].value.premisses);
                                (yylhs.value.premisses)->push_front((yystack_[1].value.premiss));
                              }
#line 716 "kif_parser.tab.cc" // lalr1.cc:847
    break;

  case 12:
#line 197 "kif_parser.yy" // lalr1.cc:847
    {
                      (yylhs.value.premisses) = NULL;
                    }
#line 724 "kif_parser.tab.cc" // lalr1.cc:847
    break;

  case 13:
#line 201 "kif_parser.yy" // lalr1.cc:847
    {
                      (yylhs.value.sentence) = new Sentence((yystack_[0].value.stringVal), yylhs.location);
                    }
#line 732 "kif_parser.tab.cc" // lalr1.cc:847
    break;

  case 14:
#line 204 "kif_parser.yy" // lalr1.cc:847
    {
                                        (yylhs.value.sentence) = new Sentence((yystack_[1].value.stringVal), yylhs.location);
                                        libgdl::core::ErrorType war;
                                        war.AddEntry("Relation symbol of arity 0 is used inside brackets.", yylhs.location);
                                        driver.Warn(war);
                                      }
#line 743 "kif_parser.tab.cc" // lalr1.cc:847
    break;

  case 15:
#line 210 "kif_parser.yy" // lalr1.cc:847
    {
                                                    (yylhs.value.sentence) = new Sentence((yystack_[3].value.stringVal), yylhs.location);
                                                    (yylhs.value.sentence)->AddArgument((yystack_[2].value.term));
                                                    if((yystack_[1].value.terms) != NULL)
                                                      (yylhs.value.sentence)->AddArgument(*(yystack_[1].value.terms));
                                                    delete (yystack_[1].value.terms);
                                                  }
#line 755 "kif_parser.tab.cc" // lalr1.cc:847
    break;

  case 16:
#line 218 "kif_parser.yy" // lalr1.cc:847
    {
                                  if((yystack_[0].value.sentences) == NULL)
                                    (yylhs.value.sentences) = new std::list<Sentence*>();
                                  else (yylhs.value.sentences) = (yystack_[0].value.sentences);
                                  (yylhs.value.sentences)->push_front((yystack_[1].value.sentence));
                                }
#line 766 "kif_parser.tab.cc" // lalr1.cc:847
    break;

  case 17:
#line 224 "kif_parser.yy" // lalr1.cc:847
    {
                      (yylhs.value.sentences) = NULL;
                    }
#line 774 "kif_parser.tab.cc" // lalr1.cc:847
    break;

  case 18:
#line 228 "kif_parser.yy" // lalr1.cc:847
    {
                      (yylhs.value.term) = new Term((yystack_[0].value.stringVal), yylhs.location);
                    }
#line 782 "kif_parser.tab.cc" // lalr1.cc:847
    break;

  case 19:
#line 231 "kif_parser.yy" // lalr1.cc:847
    {
                        (yylhs.value.term) = new Term((yystack_[0].value.stringVal), yylhs.location);
                      }
#line 790 "kif_parser.tab.cc" // lalr1.cc:847
    break;

  case 20:
#line 234 "kif_parser.yy" // lalr1.cc:847
    {
                                                    (yylhs.value.term) = new Term((yystack_[3].value.stringVal), yylhs.location);
                                                    (yylhs.value.term)->AddArgument((yystack_[2].value.term));
                                                    if((yystack_[1].value.terms) != NULL)
                                                      (yylhs.value.term)->AddArgument(*(yystack_[1].value.terms));
                                                    delete (yystack_[1].value.terms);
                                                  }
#line 802 "kif_parser.tab.cc" // lalr1.cc:847
    break;

  case 21:
#line 242 "kif_parser.yy" // lalr1.cc:847
    {
                          if((yystack_[0].value.terms) == NULL)
                            (yylhs.value.terms) = new std::list<Term*>();
                          else (yylhs.value.terms) = (yystack_[0].value.terms);
                          (yylhs.value.terms)->push_front((yystack_[1].value.term));
                        }
#line 813 "kif_parser.tab.cc" // lalr1.cc:847
    break;

  case 22:
#line 248 "kif_parser.yy" // lalr1.cc:847
    {
                      (yylhs.value.terms) = NULL;
                    }
#line 821 "kif_parser.tab.cc" // lalr1.cc:847
    break;

  case 23:
#line 252 "kif_parser.yy" // lalr1.cc:847
    {
                  (yylhs.value.stringVal) = (yystack_[0].value.stringVal);
                }
#line 829 "kif_parser.tab.cc" // lalr1.cc:847
    break;

  case 24:
#line 255 "kif_parser.yy" // lalr1.cc:847
    {
                      (yylhs.value.stringVal) = new std::string(ToString((yystack_[0].value.num)));
                    }
#line 837 "kif_parser.tab.cc" // lalr1.cc:847
    break;


#line 841 "kif_parser.tab.cc" // lalr1.cc:847
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
      yypush_ (YY_NULLPTR, yylhs);
    }
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
          yy_destroy_ (YY_NULLPTR, yyla);

        while (1 < yystack_.size ())
          {
            yy_destroy_ (YY_NULLPTR, yystack_[0]);
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

    char const* yyformat = YY_NULLPTR;
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


  const signed char KIFParser::yypact_ninf_ = -25;

  const signed char KIFParser::yytable_ninf_ = -1;

  const signed char
  KIFParser::yypact_[] =
  {
      45,     0,   -25,   -25,     1,    45,   -25,   -25,   -25,   -25,
      46,    35,   -25,   -25,     7,    51,     7,   -25,   -25,    40,
     -25,    27,    51,     2,   -25,    40,    40,    15,    46,    46,
     -25,   -25,    40,   -25,   -25,    17,    46,    22,   -25,    46,
      23,   -25,   -25,   -25
  };

  const unsigned char
  KIFParser::yydefact_[] =
  {
       3,     0,    23,    24,     0,     3,     4,     5,     6,    13,
       0,     0,     1,     2,     0,    12,     0,    14,    19,    22,
      18,     0,    12,     0,     8,     0,    22,     0,     0,     0,
      11,     7,    22,    21,    15,     0,    17,     0,     9,    17,
       0,    20,    16,    10
  };

  const signed char
  KIFParser::yypgoto_[] =
  {
     -25,    24,   -25,   -25,   -25,   -25,     6,    -6,    -4,    -8,
     -24,    -1
  };

  const signed char
  KIFParser::yydefgoto_[] =
  {
      -1,     4,     5,     6,     7,    22,    23,     8,    40,    26,
      27,     9
  };

  const unsigned char
  KIFParser::yytable_[] =
  {
      11,    12,    33,    19,    15,     2,    31,     3,    37,    24,
      20,    10,     2,    11,     3,    25,    24,    32,    20,    34,
      11,    38,    35,    36,    20,    20,    41,    43,    30,    13,
      39,    20,     2,    39,     3,    42,    28,    29,    16,    17,
       2,    18,     3,    16,     0,     2,    18,     3,     1,    14,
       2,     2,     3,     3,    21,     0,     2,     0,     3
  };

  const signed char
  KIFParser::yycheck_[] =
  {
       1,     0,    26,    11,    10,     5,     4,     7,    32,    15,
      11,    11,     5,    14,     7,    16,    22,    25,    19,     4,
      21,     4,    28,    29,    25,    26,     4,     4,    22,     5,
      36,    32,     5,    39,     7,    39,     9,    10,     3,     4,
       5,     6,     7,     3,    -1,     5,     6,     7,     3,     3,
       5,     5,     7,     7,     3,    -1,     5,    -1,     7
  };

  const unsigned char
  KIFParser::yystos_[] =
  {
       0,     3,     5,     7,    14,    15,    16,    17,    20,    24,
      11,    24,     0,    14,     3,    20,     3,     4,     6,    22,
      24,     3,    18,    19,    20,    24,    22,    23,     9,    10,
      19,     4,    22,    23,     4,    20,    20,    23,     4,    20,
      21,     4,    21,     4
  };

  const unsigned char
  KIFParser::yyr1_[] =
  {
       0,    13,    14,    14,    15,    15,    16,    17,    18,    18,
      18,    19,    19,    20,    20,    20,    21,    21,    22,    22,
      22,    23,    23,    24,    24
  };

  const unsigned char
  KIFParser::yyr2_[] =
  {
       0,     2,     2,     0,     1,     1,     1,     5,     1,     4,
       5,     2,     0,     1,     3,     5,     2,     0,     1,     1,
       5,     2,     0,     1,     1
  };



  // YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
  // First, the terminals, then, starting at \a yyntokens_, nonterminals.
  const char*
  const KIFParser::yytname_[] =
  {
  "\"end of file\"", "error", "$undefined", "\"opening bracket\"",
  "\"closing bracket\"", "\"symbol\"", "\"variable\"", "\"number\"",
  "\"true\"", "\"not\"", "\"or\"", "\"clause command\"", "\"line mark\"",
  "$accept", "start", "S", "Fact", "Clause", "Premiss", "Premisses",
  "Sentence", "Sentences", "Term", "Terms", "Command", YY_NULLPTR
  };

#if YYDEBUG
  const unsigned char
  KIFParser::yyrline_[] =
  {
       0,   136,   136,   141,   145,   148,   152,   156,   174,   178,
     183,   191,   197,   201,   204,   210,   218,   224,   228,   231,
     234,   242,   248,   252,   255
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

  // Symbol number corresponding to token number t.
  inline
  KIFParser::token_number_type
  KIFParser::yytranslate_ (int t)
  {
    static
    const token_number_type
    translate_table[] =
    {
     0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12
    };
    const unsigned int user_token_number_max_ = 267;
    const token_number_type undef_token_ = 2;

    if (static_cast<int>(t) <= yyeof_)
      return yyeof_;
    else if (static_cast<unsigned int> (t) <= user_token_number_max_)
      return translate_table[t];
    else
      return undef_token_;
  }

#line 23 "kif_parser.yy" // lalr1.cc:1155
} } } } // libgdl::gdlparser::parser::yy
#line 1284 "kif_parser.tab.cc" // lalr1.cc:1155
#line 258 "kif_parser.yy" // lalr1.cc:1156


typedef libgdl::gdlparser::parser::yy::KIFParser KIFParser;

// Mandatory error function
void KIFParser::error (const KIFParser::location_type& loc, const std::string& msg)
{
  libgdl::core::ErrorType error;
  error.AddEntry(msg, loc);
  driver.Error(error);
}
