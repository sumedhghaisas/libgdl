// A Bison parser, made by GNU Bison 3.0.

// Skeleton interface for Bison LALR(1) parsers in C++

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

/**
 ** \file kif_parser.tab.hh
 ** Define the gdlparser::parser::yy::parser class.
 */

// C++ LALR(1) parser skeleton written by Akim Demaille.

#ifndef YY_YY_KIF_PARSER_TAB_HH_INCLUDED
# define YY_YY_KIF_PARSER_TAB_HH_INCLUDED
// //                    "%code requires" blocks.
#line 24 "kif_parser.yy" // lalr1.cc:371

#include <list>
#include <string>

// custom value type
#include "token_value.hpp"

typedef std::list<std::string> strings_type;
typedef gdlparser::parser::TokenValue TokenValue;

namespace gdlparser {
    namespace parser {
            class KIFDriver;
            class KIFScanner;
        }
    }

#line 62 "kif_parser.tab.hh" // lalr1.cc:371


# include <vector>
# include <iostream>
# include <stdexcept>
# include <string>
# include "stack.hh"
# include "location.hh"

#ifndef YYASSERT
# include <cassert>
# define YYASSERT assert
#endif


/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 1
#endif

#line 12 "kif_parser.yy" // lalr1.cc:371
namespace gdlparser { namespace parser { namespace yy {
#line 85 "kif_parser.tab.hh" // lalr1.cc:371



  /// A char[S] buffer to store and retrieve objects.
  ///
  /// Sort of a variant, but does not keep track of the nature
  /// of the stored data, since that knowledge is available
  /// via the current state.
  template <size_t S>
  struct variant
  {
    /// Type of *this.
    typedef variant<S> self_type;

    /// Empty construction.
    variant ()
    {}

    /// Construct and fill.
    template <typename T>
    variant (const T& t)
    {
      YYASSERT (sizeof (T) <= S);
      new (yyas_<T> ()) T (t);
    }

    /// Destruction, allowed only if empty.
    ~variant ()
    {}

    /// Instantiate an empty \a T in here.
    template <typename T>
    T&
    build ()
    {
      return *new (yyas_<T> ()) T;
    }

    /// Instantiate a \a T in here from \a t.
    template <typename T>
    T&
    build (const T& t)
    {
      return *new (yyas_<T> ()) T (t);
    }

    /// Accessor to a built \a T.
    template <typename T>
    T&
    as ()
    {
      return *yyas_<T> ();
    }

    /// Const accessor to a built \a T (for %printer).
    template <typename T>
    const T&
    as () const
    {
      return *yyas_<T> ();
    }

    /// Swap the content with \a other, of same type.
    ///
    /// Both variants must be built beforehand, because swapping the actual
    /// data requires reading it (with as()), and this is not possible on
    /// unconstructed variants: it would require some dynamic testing, which
    /// should not be the variant's responsability.
    /// Swapping between built and (possibly) non-built is done with
    /// variant::move ().
    template <typename T>
    void
    swap (self_type& other)
    {
      std::swap (as<T> (), other.as<T> ());
    }

    /// Move the content of \a other to this.
    ///
    /// Destroys \a other.
    template <typename T>
    void
    move (self_type& other)
    {
      build<T> ();
      swap<T> (other);
      other.destroy<T> ();
    }

    /// Copy the content of \a other to this.
    template <typename T>
    void
    copy (const self_type& other)
    {
      build<T> (other.as<T> ());
    }

    /// Destroy the stored \a T.
    template <typename T>
    void
    destroy ()
    {
      as<T> ().~T ();
    }

  private:
    /// Prohibit blind copies.
    self_type& operator=(const self_type&);
    variant (const self_type&);

    /// Accessor to raw memory as \a T.
    template <typename T>
    T*
    yyas_ ()
    {
      void *yyp = yybuffer_.yyraw;
      return static_cast<T*> (yyp);
     }

    /// Const accessor to raw memory as \a T.
    template <typename T>
    const T*
    yyas_ () const
    {
      const void *yyp = yybuffer_.yyraw;
      return static_cast<const T*> (yyp);
     }

    union
    {
      /// Strongest alignment constraints.
      long double yyalign_me;
      /// A buffer large enough to store any of the semantic values.
      char yyraw[S];
    } yybuffer_;
  };


  /// A Bison parser.
  class KIFParser
  {
  public:
#ifndef YYSTYPE
    /// An auxiliary type to compute the largest semantic type.
    union union_type
    {
      // "opening bracket"
      // "closing bracket"
      // "symbol"
      // "variable"
      // "number"
      // "role"
      // "input"
      // "init"
      // "base"
      // "legal"
      // "next"
      // "goal"
      // "terminal"
      // "does"
      // "true"
      // "not"
      // "or"
      // "distinct"
      // "clause command"
      // "line mark"
      // Start
      // S
      // Fact
      // Ground_term
      // Ground_terms
      // Id_num
      // Clause
      // Head
      // ORelation
      // ORelations
      // Literal
      // Term
      // Terms
      // CTerm
      // CTerms
      // Num_var
      char dummy1[sizeof(TokenValue)];
};

    /// Symbol semantic values.
    typedef variant<sizeof(union_type)> semantic_type;
#else
    typedef YYSTYPE semantic_type;
#endif
    /// Symbol locations.
    typedef location location_type;

    /// Syntax errors thrown from user actions.
    struct syntax_error : std::runtime_error
    {
      syntax_error (const location_type& l, const std::string& m);
      location_type location;
    };

    /// Tokens.
    struct token
    {
      enum yytokentype
      {
        END = 0,
        Obracket = 258,
        Cbracket = 259,
        id = 260,
        var = 261,
        num = 262,
        role = 263,
        input = 264,
        init = 265,
        base = 266,
        legal = 267,
        next = 268,
        goal = 269,
        terminal = 270,
        does = 271,
        ttrue = 272,
        nnot = 273,
        oor = 274,
        distinct = 275,
        clause_command = 276,
        hash_line = 277
      };
    };

    /// (External) token type, as returned by yylex.
    typedef token::yytokentype token_type;

    /// Internal symbol number.
    typedef int symbol_number_type;

    /// Internal symbol number for tokens (subsumed by symbol_number_type).
    typedef unsigned char token_number_type;

    /// A complete symbol.
    ///
    /// Expects its Base type to provide access to the symbol type
    /// via type_get().
    ///
    /// Provide access to semantic value and location.
    template <typename Base>
    struct basic_symbol : Base
    {
      /// Alias to Base.
      typedef Base super_type;

      /// Default constructor.
      basic_symbol ();

      /// Copy constructor.
      basic_symbol (const basic_symbol& other);

      /// Constructor for valueless symbols, and symbols from each type.

  basic_symbol (typename Base::kind_type t, const location_type& l);

  basic_symbol (typename Base::kind_type t, const TokenValue v, const location_type& l);


      /// Constructor for symbols with semantic value.
      basic_symbol (typename Base::kind_type t,
                    const semantic_type& v,
                    const location_type& l);

      ~basic_symbol ();

      /// Destructive move, \a s is emptied into this.
      void move (basic_symbol& s);

      /// The semantic value.
      semantic_type value;

      /// The location.
      location_type location;

    private:
      /// Assignment operator.
      basic_symbol& operator= (const basic_symbol& other);
    };

    /// Type access provider for token (enum) based symbols.
    struct by_type
    {
      /// Default constructor.
      by_type ();

      /// Copy constructor.
      by_type (const by_type& other);

      /// The symbol type as needed by the constructor.
      typedef token_type kind_type;

      /// Constructor from (external) token numbers.
      by_type (kind_type t);

      /// Steal the symbol type from \a that.
      void move (by_type& that);

      /// The (internal) type number (corresponding to \a type).
      /// -1 when this symbol is empty.
      symbol_number_type type_get () const;

      /// The token.
      token_type token () const;

      enum { empty = 0 };

      /// The symbol type.
      /// -1 when this symbol is empty.
      token_number_type type;
    };

    /// "External" symbols: returned by the scanner.
    typedef basic_symbol<by_type> symbol_type;

    // Symbol constructors declarations.
    static inline
    symbol_type
    make_END (const location_type& l);

    static inline
    symbol_type
    make_Obracket (const TokenValue& v, const location_type& l);

    static inline
    symbol_type
    make_Cbracket (const TokenValue& v, const location_type& l);

    static inline
    symbol_type
    make_id (const TokenValue& v, const location_type& l);

    static inline
    symbol_type
    make_var (const TokenValue& v, const location_type& l);

    static inline
    symbol_type
    make_num (const TokenValue& v, const location_type& l);

    static inline
    symbol_type
    make_role (const TokenValue& v, const location_type& l);

    static inline
    symbol_type
    make_input (const TokenValue& v, const location_type& l);

    static inline
    symbol_type
    make_init (const TokenValue& v, const location_type& l);

    static inline
    symbol_type
    make_base (const TokenValue& v, const location_type& l);

    static inline
    symbol_type
    make_legal (const TokenValue& v, const location_type& l);

    static inline
    symbol_type
    make_next (const TokenValue& v, const location_type& l);

    static inline
    symbol_type
    make_goal (const TokenValue& v, const location_type& l);

    static inline
    symbol_type
    make_terminal (const TokenValue& v, const location_type& l);

    static inline
    symbol_type
    make_does (const TokenValue& v, const location_type& l);

    static inline
    symbol_type
    make_ttrue (const TokenValue& v, const location_type& l);

    static inline
    symbol_type
    make_nnot (const TokenValue& v, const location_type& l);

    static inline
    symbol_type
    make_oor (const TokenValue& v, const location_type& l);

    static inline
    symbol_type
    make_distinct (const TokenValue& v, const location_type& l);

    static inline
    symbol_type
    make_clause_command (const TokenValue& v, const location_type& l);

    static inline
    symbol_type
    make_hash_line (const TokenValue& v, const location_type& l);


    /// Build a parser object.
    KIFParser (KIFScanner  &scanner_yyarg, KIFDriver  &driver_yyarg);
    virtual ~KIFParser ();

    /// Parse.
    /// \returns  0 iff parsing succeeded.
    virtual int parse ();

#if YYDEBUG
    /// The current debugging stream.
    std::ostream& debug_stream () const;
    /// Set the current debugging stream.
    void set_debug_stream (std::ostream &);

    /// Type for debugging levels.
    typedef int debug_level_type;
    /// The current debugging level.
    debug_level_type debug_level () const;
    /// Set the current debugging level.
    void set_debug_level (debug_level_type l);
#endif

    /// Report a syntax error.
    /// \param loc    where the syntax error is found.
    /// \param msg    a description of the syntax error.
    virtual void error (const location_type& loc, const std::string& msg);

    /// Report a syntax error.
    void error (const syntax_error& err);

  private:
    /// This class is not copyable.
    KIFParser (const KIFParser&);
    KIFParser& operator= (const KIFParser&);

    /// State numbers.
    typedef int state_type;

    /// Generate an error message.
    /// \param yystate   the state where the error occurred.
    /// \param yytoken   the lookahead token type, or yyempty_.
    virtual std::string yysyntax_error_ (state_type yystate,
                                         symbol_number_type yytoken) const;

    /// Compute post-reduction state.
    /// \param yystate   the current state
    /// \param yylhs     the nonterminal to push on the stack
    state_type yy_lr_goto_state_ (state_type yystate, int yylhs);

    /// Whether the given \c yypact_ value indicates a defaulted state.
    /// \param yyvalue   the value to check
    static bool yy_pact_value_is_default_ (int yyvalue);

    /// Whether the given \c yytable_ value indicates a syntax error.
    /// \param yyvalue   the value to check
    static bool yy_table_value_is_error_ (int yyvalue);

    static const short int yypact_ninf_;
    static const signed char yytable_ninf_;

    /// Convert a scanner token number \a t to a symbol number.
    static token_number_type yytranslate_ (token_type t);

    // Tables.
  // YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
  // STATE-NUM.
  static const short int yypact_[];

  // YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
  // Performed when YYTABLE does not specify something else to do.  Zero
  // means the default is an error.
  static const unsigned char yydefact_[];

  // YYPGOTO[NTERM-NUM].
  static const short int yypgoto_[];

  // YYDEFGOTO[NTERM-NUM].
  static const short int yydefgoto_[];

  // YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
  // positive, shift that token.  If negative, reduce the rule whose
  // number is the opposite.  If YYTABLE_NINF, syntax error.
  static const unsigned char yytable_[];

  static const short int yycheck_[];

  // YYSTOS[STATE-NUM] -- The (internal number of the) accessing
  // symbol of state STATE-NUM.
  static const unsigned char yystos_[];

  // YYR1[YYN] -- Symbol number of symbol that rule YYN derives.
  static const unsigned char yyr1_[];

  // YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.
  static const unsigned char yyr2_[];


    /// Convert the symbol name \a n to a form suitable for a diagnostic.
    static std::string yytnamerr_ (const char *n);


    /// For a symbol, its name in clear.
    static const char* const yytname_[];
#if YYDEBUG
  // YYRLINE[YYN] -- Source line where rule number YYN was defined.
  static const unsigned short int yyrline_[];
    /// Report on the debug stream that the rule \a r is going to be reduced.
    virtual void yy_reduce_print_ (int r);
    /// Print the state stack on the debug stream.
    virtual void yystack_print_ ();

    // Debugging.
    int yydebug_;
    std::ostream* yycdebug_;

    /// \brief Display a symbol type, value and location.
    /// \param yyo    The output stream.
    /// \param yysym  The symbol.
    template <typename Base>
    void yy_print_ (std::ostream& yyo, const basic_symbol<Base>& yysym) const;
#endif

    /// \brief Reclaim the memory associated to a symbol.
    /// \param yymsg     Why this token is reclaimed.
    ///                  If null, print nothing.
    /// \param s         The symbol.
    template <typename Base>
    void yy_destroy_ (const char* yymsg, basic_symbol<Base>& yysym) const;

  private:
    /// Type access provider for state based symbols.
    struct by_state
    {
      /// Default constructor.
      by_state ();

      /// The symbol type as needed by the constructor.
      typedef state_type kind_type;

      /// Constructor.
      by_state (kind_type s);

      /// Copy constructor.
      by_state (const by_state& other);

      /// Steal the symbol type from \a that.
      void move (by_state& that);

      /// The (internal) type number (corresponding to \a state).
      /// "empty" when empty.
      symbol_number_type type_get () const;

      enum { empty = 0 };

      /// The state.
      state_type state;
    };

    /// "Internal" symbol: element of the stack.
    struct stack_symbol_type : basic_symbol<by_state>
    {
      /// Superclass.
      typedef basic_symbol<by_state> super_type;
      /// Construct an empty symbol.
      stack_symbol_type ();
      /// Steal the contents from \a sym to build this.
      stack_symbol_type (state_type s, symbol_type& sym);
      /// Assignment, needed by push_back.
      stack_symbol_type& operator= (const stack_symbol_type& that);
    };

    /// Stack type.
    typedef stack<stack_symbol_type> stack_type;

    /// The stack.
    stack_type yystack_;

    /// Push a new state on the stack.
    /// \param m    a debug message to display
    ///             if null, no trace is output.
    /// \param s    the symbol
    /// \warning the contents of \a s.value is stolen.
    void yypush_ (const char* m, stack_symbol_type& s);

    /// Push a new look ahead token on the state on the stack.
    /// \param m    a debug message to display
    ///             if null, no trace is output.
    /// \param s    the state
    /// \param sym  the symbol (for its value and location).
    /// \warning the contents of \a s.value is stolen.
    void yypush_ (const char* m, state_type s, symbol_type& sym);

    /// Pop \a n symbols the three stacks.
    void yypop_ (unsigned int n = 1);

    // Constants.
    enum
    {
      yyeof_ = 0,
      yylast_ = 205,           //< Last index in yytable_.
      yynnts_ = 17,  //< Number of nonterminal symbols.
      yyempty_ = -2,
      yyfinal_ = 19, //< Termination state number.
      yyterror_ = 1,
      yyerrcode_ = 256,
      yyntokens_ = 23    //< Number of tokens.
    };


    // User arguments.
    KIFScanner  &scanner;
    KIFDriver  &driver;
  };

  // Symbol number corresponding to token number t.
  inline
  KIFParser::token_number_type
  KIFParser::yytranslate_ (token_type t)
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
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22
    };
    const unsigned int user_token_number_max_ = 277;
    const token_number_type undef_token_ = 2;

    if (static_cast<int>(t) <= yyeof_)
      return yyeof_;
    else if (static_cast<unsigned int> (t) <= user_token_number_max_)
      return translate_table[t];
    else
      return undef_token_;
  }

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
      switch (other.type_get ())
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
      case 37: // CTerm
      case 38: // CTerms
      case 39: // Num_var
        value.copy< TokenValue > (other.value);
        break;

      default:
        break;
    }

  }


  template <typename Base>
  inline
  KIFParser::basic_symbol<Base>::basic_symbol (typename Base::kind_type t, const semantic_type& v, const location_type& l)
    : Base (t)
    , value ()
    , location (l)
  {
    (void) v;
      switch (this->type_get ())
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
      case 37: // CTerm
      case 38: // CTerms
      case 39: // Num_var
        value.copy< TokenValue > (v);
        break;

      default:
        break;
    }
}


  // Implementation of basic_symbol constructor for each type.

  template <typename Base>
  KIFParser::basic_symbol<Base>::basic_symbol (typename Base::kind_type t, const location_type& l)
    : Base (t)
    , value ()
    , location (l)
  {}

  template <typename Base>
  KIFParser::basic_symbol<Base>::basic_symbol (typename Base::kind_type t, const TokenValue v, const location_type& l)
    : Base (t)
    , value (v)
    , location (l)
  {}


  template <typename Base>
  inline
  KIFParser::basic_symbol<Base>::~basic_symbol ()
  {
    // User destructor.
    symbol_number_type yytype = this->type_get ();
    switch (yytype)
    {
   default:
      break;
    }

    // Type destructor.
    switch (yytype)
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
      case 37: // CTerm
      case 38: // CTerms
      case 39: // Num_var
        value.template destroy< TokenValue > ();
        break;

      default:
        break;
    }

  }

  template <typename Base>
  inline
  void
  KIFParser::basic_symbol<Base>::move (basic_symbol& s)
  {
    super_type::move(s);
      switch (this->type_get ())
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
      case 37: // CTerm
      case 38: // CTerms
      case 39: // Num_var
        value.move< TokenValue > (s.value);
        break;

      default:
        break;
    }

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

  inline
  KIFParser::token_type
  KIFParser::by_type::token () const
  {
    // YYTOKNUM[NUM] -- (External) token number corresponding to the
    // (internal) symbol number NUM (which must be that of a token).  */
    static
    const unsigned short int
    yytoken_number_[] =
    {
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277
    };
    return static_cast<token_type> (yytoken_number_[type]);
  }
  // Implementation of make_symbol for each symbol type.
  KIFParser::symbol_type
  KIFParser::make_END (const location_type& l)
  {
    return symbol_type (token::END, l);

  }

  KIFParser::symbol_type
  KIFParser::make_Obracket (const TokenValue& v, const location_type& l)
  {
    return symbol_type (token::Obracket, v, l);

  }

  KIFParser::symbol_type
  KIFParser::make_Cbracket (const TokenValue& v, const location_type& l)
  {
    return symbol_type (token::Cbracket, v, l);

  }

  KIFParser::symbol_type
  KIFParser::make_id (const TokenValue& v, const location_type& l)
  {
    return symbol_type (token::id, v, l);

  }

  KIFParser::symbol_type
  KIFParser::make_var (const TokenValue& v, const location_type& l)
  {
    return symbol_type (token::var, v, l);

  }

  KIFParser::symbol_type
  KIFParser::make_num (const TokenValue& v, const location_type& l)
  {
    return symbol_type (token::num, v, l);

  }

  KIFParser::symbol_type
  KIFParser::make_role (const TokenValue& v, const location_type& l)
  {
    return symbol_type (token::role, v, l);

  }

  KIFParser::symbol_type
  KIFParser::make_input (const TokenValue& v, const location_type& l)
  {
    return symbol_type (token::input, v, l);

  }

  KIFParser::symbol_type
  KIFParser::make_init (const TokenValue& v, const location_type& l)
  {
    return symbol_type (token::init, v, l);

  }

  KIFParser::symbol_type
  KIFParser::make_base (const TokenValue& v, const location_type& l)
  {
    return symbol_type (token::base, v, l);

  }

  KIFParser::symbol_type
  KIFParser::make_legal (const TokenValue& v, const location_type& l)
  {
    return symbol_type (token::legal, v, l);

  }

  KIFParser::symbol_type
  KIFParser::make_next (const TokenValue& v, const location_type& l)
  {
    return symbol_type (token::next, v, l);

  }

  KIFParser::symbol_type
  KIFParser::make_goal (const TokenValue& v, const location_type& l)
  {
    return symbol_type (token::goal, v, l);

  }

  KIFParser::symbol_type
  KIFParser::make_terminal (const TokenValue& v, const location_type& l)
  {
    return symbol_type (token::terminal, v, l);

  }

  KIFParser::symbol_type
  KIFParser::make_does (const TokenValue& v, const location_type& l)
  {
    return symbol_type (token::does, v, l);

  }

  KIFParser::symbol_type
  KIFParser::make_ttrue (const TokenValue& v, const location_type& l)
  {
    return symbol_type (token::ttrue, v, l);

  }

  KIFParser::symbol_type
  KIFParser::make_nnot (const TokenValue& v, const location_type& l)
  {
    return symbol_type (token::nnot, v, l);

  }

  KIFParser::symbol_type
  KIFParser::make_oor (const TokenValue& v, const location_type& l)
  {
    return symbol_type (token::oor, v, l);

  }

  KIFParser::symbol_type
  KIFParser::make_distinct (const TokenValue& v, const location_type& l)
  {
    return symbol_type (token::distinct, v, l);

  }

  KIFParser::symbol_type
  KIFParser::make_clause_command (const TokenValue& v, const location_type& l)
  {
    return symbol_type (token::clause_command, v, l);

  }

  KIFParser::symbol_type
  KIFParser::make_hash_line (const TokenValue& v, const location_type& l)
  {
    return symbol_type (token::hash_line, v, l);

  }


#line 12 "kif_parser.yy" // lalr1.cc:371
} } } // gdlparser::parser::yy
#line 1206 "kif_parser.tab.hh" // lalr1.cc:371




#endif // !YY_YY_KIF_PARSER_TAB_HH_INCLUDED
