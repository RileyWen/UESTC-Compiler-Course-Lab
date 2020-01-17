// A Bison parser, made by GNU Bison 3.2.

// Skeleton interface for Bison LALR(1) parsers in C++

// Copyright (C) 2002-2015, 2018 Free Software Foundation, Inc.

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
 ** \file ./myparser.hh
 ** Define the yy::parser class.
 */

// C++ LALR(1) parser skeleton written by Akim Demaille.

// Undocumented macros, especially those whose name start with YY_,
// are private implementation details.  Do not rely on them.

#ifndef YY_YY_MYPARSER_HH_INCLUDED
# define YY_YY_MYPARSER_HH_INCLUDED
// //                    "%code requires" blocks.
#line 9 "./parser.yy" // lalr1.cc:403

# include <string>
//  class driver;

#line 53 "./myparser.hh" // lalr1.cc:403

# include <cassert>
# include <cstdlib> // std::abort
# include <iostream>
# include <stdexcept>
# include <string>
# include <vector>

// Support move semantics when possible.
#if defined __cplusplus && 201103L <= __cplusplus
# define YY_MOVE           std::move
# define YY_MOVE_OR_COPY   move
# define YY_MOVE_REF(Type) Type&&
# define YY_RVREF(Type)    Type&&
# define YY_COPY(Type)     Type
#else
# define YY_MOVE
# define YY_MOVE_OR_COPY   copy
# define YY_MOVE_REF(Type) Type&
# define YY_RVREF(Type)    const Type&
# define YY_COPY(Type)     const Type&
#endif

# include "location.hh"
#include <typeinfo>

#ifndef YYASSERT

# include <cassert>

# define YYASSERT assert
#endif


#ifndef YY_ATTRIBUTE
# if (defined __GNUC__                                               \
      && (2 < __GNUC__ || (__GNUC__ == 2 && 96 <= __GNUC_MINOR__)))  \
 || defined __SUNPRO_C && 0x5110 <= __SUNPRO_C
#  define YY_ATTRIBUTE(Spec) __attribute__(Spec)
# else
#  define YY_ATTRIBUTE(Spec) /* empty */
# endif
#endif

#ifndef YY_ATTRIBUTE_PURE
# define YY_ATTRIBUTE_PURE   YY_ATTRIBUTE ((__pure__))
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# define YY_ATTRIBUTE_UNUSED YY_ATTRIBUTE ((__unused__))
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if !defined lint || defined __GNUC__
# define YYUSE(E) ((void) (E))
#else
# define YYUSE(E) /* empty */
#endif

#if defined __GNUC__ && !defined __ICC && 407 <= __GNUC__ * 100 + __GNUC_MINOR__
/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN \
    _Pragma ("GCC diagnostic push") \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")\
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# define YY_IGNORE_MAYBE_UNINITIALIZED_END \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif

# ifndef YY_NULLPTR
#  if defined __cplusplus
#   if 201103L <= __cplusplus
#    define YY_NULLPTR nullptr
#   else
#    define YY_NULLPTR 0
#   endif
#  else
#   define YY_NULLPTR ((void*)0)
#  endif
# endif

/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 1
#endif


namespace yy {
#line 147 "./myparser.hh" // lalr1.cc:403

    /// A stack with random access from its top.
    template<typename T, typename S = std::vector<T> >
    class stack {
    public:
        // Hide our reversed order.
        typedef typename S::reverse_iterator iterator;
        typedef typename S::const_reverse_iterator const_iterator;
        typedef typename S::size_type size_type;

        stack(size_type n = 200)
                : seq_(n) {}

        /// Random access.
        ///
        /// Index 0 returns the topmost element.
        T &
        operator[](size_type i) {
            return seq_[size() - 1 - i];
        }

        /// Random access.
        ///
        /// Index 0 returns the topmost element.
        T &
        operator[](int i) {
            return operator[](size_type(i));
        }

        /// Random access.
        ///
        /// Index 0 returns the topmost element.
        const T &
        operator[](size_type i) const {
            return seq_[size() - 1 - i];
        }

        /// Random access.
        ///
        /// Index 0 returns the topmost element.
        const T &
        operator[](int i) const {
            return operator[](size_type(i));
        }

        /// Steal the contents of \a t.
        ///
        /// Close to move-semantics.
        void
        push(YY_MOVE_REF (T)t) {
            seq_.push_back(T());
            operator[](0).move(t);
        }

        void
        pop(int n = 1) {
            for (; 0 < n; --n)
                seq_.pop_back();
        }

        void
        clear() {
            seq_.clear();
        }

        size_type
        size() const {
            return seq_.size();
        }

        const_iterator
        begin() const {
            return seq_.rbegin();
        }

        const_iterator
        end() const {
            return seq_.rend();
        }

    private:
        stack(const stack &);

        stack &operator=(const stack &);

        /// The wrapped container.
        S seq_;
    };

    /// Present a slice of the top of a stack.
    template<typename T, typename S = stack<T> >
    class slice {
    public:
        slice(const S &stack, int range)
                : stack_(stack), range_(range) {}

        const T &
        operator[](int i) const {
            return stack_[range_ - i];
        }

    private:
        const S &stack_;
        int range_;
    };


    /// A char[S] buffer to store and retrieve objects.
    ///
    /// Sort of a variant, but does not keep track of the nature
    /// of the stored data, since that knowledge is available
    /// via the current state.
    template<size_t S>
    struct variant {
        /// Type of *this.
        typedef variant<S> self_type;

        /// Empty construction.
        variant()
                : yybuffer_(), yytypeid_(YY_NULLPTR) {}

        /// Construct and fill.
        template<typename T>
        variant(YY_RVREF (T)t)
                : yytypeid_(&typeid(T)) {
                    YYASSERT (sizeof(T) <= S);
            new(yyas_<T>()) T(YY_MOVE(t));
        }

        /// Destruction, allowed only if empty.
        ~variant() {
                    YYASSERT (!yytypeid_);
        }

        /// Instantiate an empty \a T in here.
        template<typename T>
        T &
        emplace() {
                    YYASSERT (!yytypeid_);
                    YYASSERT (sizeof(T) <= S);
            yytypeid_ = &typeid(T);
            return *new(yyas_<T>()) T();
        }

# if defined __cplusplus && 201103L <= __cplusplus

        /// Instantiate a \a T in here from \a t.
        template<typename T, typename U>
        T &
        emplace(U &&u) {
                    YYASSERT (!yytypeid_);
                    YYASSERT (sizeof(T) <= S);
            yytypeid_ = &typeid(T);
            return *new(yyas_<T>()) T(std::forward<U>(u));
        }

# else
        /// Instantiate a \a T in here from \a t.
        template <typename T>
        T&
        emplace (const T& t)
        {
          YYASSERT (!yytypeid_);
          YYASSERT (sizeof (T) <= S);
          yytypeid_ = & typeid (T);
          return *new (yyas_<T> ()) T (t);
        }
# endif

        /// Instantiate an empty \a T in here.
        /// Obsolete, use emplace.
        template<typename T>
        T &
        build() {
            return emplace<T>();
        }

        /// Instantiate a \a T in here from \a t.
        /// Obsolete, use emplace.
        template<typename T>
        T &
        build(const T &t) {
            return emplace<T>(t);
        }

        /// Accessor to a built \a T.
        template<typename T>
        T &
        as() {
                    YYASSERT (yytypeid_);
                    YYASSERT (*yytypeid_ == typeid(T));
                    YYASSERT (sizeof(T) <= S);
            return *yyas_<T>();
        }

        /// Const accessor to a built \a T (for %printer).
        template<typename T>
        const T &
        as() const {
                    YYASSERT (yytypeid_);
                    YYASSERT (*yytypeid_ == typeid(T));
                    YYASSERT (sizeof(T) <= S);
            return *yyas_<T>();
        }

        /// Swap the content with \a other, of same type.
        ///
        /// Both variants must be built beforehand, because swapping the actual
        /// data requires reading it (with as()), and this is not possible on
        /// unconstructed variants: it would require some dynamic testing, which
        /// should not be the variant's responsibility.
        /// Swapping between built and (possibly) non-built is done with
        /// variant::move ().
        template<typename T>
        void
        swap(self_type &other) {
                    YYASSERT (yytypeid_);
                    YYASSERT (*yytypeid_ == *other.yytypeid_);
            std::swap(as<T>(), other.as<T>());
        }

        /// Move the content of \a other to this.
        ///
        /// Destroys \a other.
        template<typename T>
        void
        move(self_type &other) {
# if defined __cplusplus && 201103L <= __cplusplus
            emplace<T>(std::move(other.as<T>()));
# else
            emplace<T> ();
            swap<T> (other);
# endif
            other.destroy<T>();
        }

# if defined __cplusplus && 201103L <= __cplusplus

        /// Move the content of \a other to this.
        template<typename T>
        void
        move(self_type &&other) {
            emplace<T>(std::move(other.as<T>()));
            other.destroy<T>();
        }

#endif

        /// Copy the content of \a other to this.
        template<typename T>
        void
        copy(const self_type &other) {
            emplace<T>(other.as<T>());
        }

        /// Destroy the stored \a T.
        template<typename T>
        void
        destroy() {
            as<T>().~T();
            yytypeid_ = YY_NULLPTR;
        }

    private:
        /// Prohibit blind copies.
        self_type &operator=(const self_type &);

        variant(const self_type &);

        /// Accessor to raw memory as \a T.
        template<typename T>
        T *
        yyas_() {
            void *yyp = yybuffer_.yyraw;
            return static_cast<T *> (yyp);
        }

        /// Const accessor to raw memory as \a T.
        template<typename T>
        const T *
        yyas_() const {
            const void *yyp = yybuffer_.yyraw;
            return static_cast<const T *> (yyp);
        }

        union {
            /// Strongest alignment constraints.
            long double yyalign_me;
            /// A buffer large enough to store any of the semantic values.
            char yyraw[S];
        } yybuffer_;

        /// Whether the content is built: if defined, the name of the stored type.
        const std::type_info *yytypeid_;
    };


    /// A Bison parser.
    class parser {
    public:
#ifndef YYSTYPE
        /// An auxiliary type to compute the largest semantic type.
        union union_type {
            // NUM
            char dummy1[sizeof(int)];

            // ID
            // STR
            char dummy2[sizeof(std::string)];
        };

        /// Symbol semantic values.
        typedef variant<sizeof(union_type)> semantic_type;
#else
        typedef YYSTYPE semantic_type;
#endif
        /// Symbol locations.
        typedef location location_type;

        /// Syntax errors thrown from user actions.
        struct syntax_error : std::runtime_error {
            syntax_error(const location_type &l, const std::string &m);

            location_type location;
        };

        /// Tokens.
        struct token {
            enum yytokentype {
                TOK_END = 0,
                TOK_ASSIGN = 258,
                TOK_ADD_ASSIGN = 259,
                TOK_SUB_ASSIGN = 260,
                TOK_MUL_ASSIGN = 261,
                TOK_DIV_ASSIGN = 262,
                TOK_MOD_ASSIGN = 263,
                TOK_MINUS = 264,
                TOK_ADD = 265,
                TOK_STAR = 266,
                TOK_SLASH = 267,
                TOK_MOD = 268,
                TOK_LPAREN = 269,
                TOK_RPAREN = 270,
                TOK_LSQUARE = 271,
                TOK_RSQUARE = 272,
                TOK_LBRACE = 273,
                TOK_RBRACE = 274,
                TOK_EQ = 275,
                TOK_NE = 276,
                TOK_LT = 277,
                TOK_GT = 278,
                TOK_COMMA = 279,
                TOK_SEMICOLON = 280,
                TOK_TYPE_STR = 281,
                TOK_TYPE_INT = 282,
                TOK_TYPE_VOID = 283,
                TOK_WHILE = 284,
                TOK_RETURN = 285,
                TOK_IF = 286,
                TOK_ELSE = 287,
                TOK_SCAN = 288,
                TOK_PRINT = 289,
                TOK_ID = 290,
                TOK_STR = 291,
                TOK_NUM = 292
            };
        };

        /// (External) token type, as returned by yylex.
        typedef token::yytokentype token_type;

        /// Symbol type: an internal symbol number.
        typedef int symbol_number_type;

        /// The symbol type number to denote an empty symbol.
        enum {
            empty_symbol = -2
        };

        /// Internal symbol number for tokens (subsumed by symbol_number_type).
        typedef unsigned char token_number_type;

        /// A complete symbol.
        ///
        /// Expects its Base type to provide access to the symbol type
        /// via type_get ().
        ///
        /// Provide access to semantic value and location.
        template<typename Base>
        struct basic_symbol : Base {
            /// Alias to Base.
            typedef Base super_type;

            /// Default constructor.
            basic_symbol();

            /// Move or copy constructor.
            basic_symbol(YY_RVREF (basic_symbol)other);


            /// Constructor for valueless symbols, and symbols from each type.
            basic_symbol(typename Base::kind_type t, YY_RVREF (location_type)l);

            basic_symbol(typename Base::kind_type t, YY_RVREF (int)v, YY_RVREF (location_type)l);

            basic_symbol(typename Base::kind_type t, YY_RVREF (std::string)v, YY_RVREF (location_type)l);


            /// Destroy the symbol.
            ~basic_symbol();

            /// Destroy contents, and record that is empty.
            void clear();

            /// Whether empty.
            bool empty() const;

            /// Destructive move, \a s is emptied into this.
            void move(basic_symbol &s);

            /// The semantic value.
            semantic_type value;

            /// The location.
            location_type location;

        private:
#if defined __cplusplus && __cplusplus < 201103L
            /// Assignment operator.
            basic_symbol& operator= (const basic_symbol& other);
#endif
        };

        /// Type access provider for token (enum) based symbols.
        struct by_type {
            /// Default constructor.
            by_type();

            /// Copy constructor.
            by_type(const by_type &other);

            /// The symbol type as needed by the constructor.
            typedef token_type kind_type;

            /// Constructor from (external) token numbers.
            by_type(kind_type t);

            /// Record that this symbol is empty.
            void clear();

            /// Steal the symbol type from \a that.
            void move(by_type &that);

            /// The (internal) type number (corresponding to \a type).
            /// \a empty when empty.
            symbol_number_type type_get() const;

            /// The token.
            token_type token() const;

            /// The symbol type.
            /// \a empty_symbol when empty.
            /// An int, not token_number_type, to be able to store empty_symbol.
            int type;
        };

        /// "External" symbols: returned by the scanner.
        typedef basic_symbol<by_type> symbol_type;

        /// Build a parser object.
        parser();

        virtual ~parser();

        /// Parse.  An alias for parse ().
        /// \returns  0 iff parsing succeeded.
        int operator()();

        /// Parse.
        /// \returns  0 iff parsing succeeded.
        virtual int parse();

#if YYDEBUG

        /// The current debugging stream.
        std::ostream &debug_stream() const YY_ATTRIBUTE_PURE;

        /// Set the current debugging stream.
        void set_debug_stream(std::ostream &);

        /// Type for debugging levels.
        typedef int debug_level_type;

        /// The current debugging level.
        debug_level_type debug_level() const YY_ATTRIBUTE_PURE;

        /// Set the current debugging level.
        void set_debug_level(debug_level_type l);

#endif

        /// Report a syntax error.
        /// \param loc    where the syntax error is found.
        /// \param msg    a description of the syntax error.
        virtual void error(const location_type &loc, const std::string &msg);

        /// Report a syntax error.
        void error(const syntax_error &err);

        // Symbol constructors declarations.
        static
        symbol_type
        make_END(YY_COPY (location_type) l);

        static
        symbol_type
        make_ASSIGN(YY_COPY (location_type) l);

        static
        symbol_type
        make_ADD_ASSIGN(YY_COPY (location_type) l);

        static
        symbol_type
        make_SUB_ASSIGN(YY_COPY (location_type) l);

        static
        symbol_type
        make_MUL_ASSIGN(YY_COPY (location_type) l);

        static
        symbol_type
        make_DIV_ASSIGN(YY_COPY (location_type) l);

        static
        symbol_type
        make_MOD_ASSIGN(YY_COPY (location_type) l);

        static
        symbol_type
        make_MINUS(YY_COPY (location_type) l);

        static
        symbol_type
        make_ADD(YY_COPY (location_type) l);

        static
        symbol_type
        make_STAR(YY_COPY (location_type) l);

        static
        symbol_type
        make_SLASH(YY_COPY (location_type) l);

        static
        symbol_type
        make_MOD(YY_COPY (location_type) l);

        static
        symbol_type
        make_LPAREN(YY_COPY (location_type) l);

        static
        symbol_type
        make_RPAREN(YY_COPY (location_type) l);

        static
        symbol_type
        make_LSQUARE(YY_COPY (location_type) l);

        static
        symbol_type
        make_RSQUARE(YY_COPY (location_type) l);

        static
        symbol_type
        make_LBRACE(YY_COPY (location_type) l);

        static
        symbol_type
        make_RBRACE(YY_COPY (location_type) l);

        static
        symbol_type
        make_EQ(YY_COPY (location_type) l);

        static
        symbol_type
        make_NE(YY_COPY (location_type) l);

        static
        symbol_type
        make_LT(YY_COPY (location_type) l);

        static
        symbol_type
        make_GT(YY_COPY (location_type) l);

        static
        symbol_type
        make_COMMA(YY_COPY (location_type) l);

        static
        symbol_type
        make_SEMICOLON(YY_COPY (location_type) l);

        static
        symbol_type
        make_TYPE_STR(YY_COPY (location_type) l);

        static
        symbol_type
        make_TYPE_INT(YY_COPY (location_type) l);

        static
        symbol_type
        make_TYPE_VOID(YY_COPY (location_type) l);

        static
        symbol_type
        make_WHILE(YY_COPY (location_type) l);

        static
        symbol_type
        make_RETURN(YY_COPY (location_type) l);

        static
        symbol_type
        make_IF(YY_COPY (location_type) l);

        static
        symbol_type
        make_ELSE(YY_COPY (location_type) l);

        static
        symbol_type
        make_SCAN(YY_COPY (location_type) l);

        static
        symbol_type
        make_PRINT(YY_COPY (location_type) l);

        static
        symbol_type
        make_ID(YY_COPY (std::string) v, YY_COPY (location_type) l);

        static
        symbol_type
        make_STR(YY_COPY (std::string) v, YY_COPY (location_type) l);

        static
        symbol_type
        make_NUM(YY_COPY (int) v, YY_COPY (location_type) l);


    private:
        /// This class is not copyable.
        parser(const parser &);

        parser &operator=(const parser &);

        /// State numbers.
        typedef int state_type;

        /// Generate an error message.
        /// \param yystate   the state where the error occurred.
        /// \param yyla      the lookahead token.
        virtual std::string yysyntax_error_(state_type yystate,
                                            const symbol_type &yyla) const;

        /// Compute post-reduction state.
        /// \param yystate   the current state
        /// \param yysym     the nonterminal to push on the stack
        state_type yy_lr_goto_state_(state_type yystate, int yysym);

        /// Whether the given \c yypact_ value indicates a defaulted state.
        /// \param yyvalue   the value to check
        static bool yy_pact_value_is_default_(int yyvalue);

        /// Whether the given \c yytable_ value indicates a syntax error.
        /// \param yyvalue   the value to check
        static bool yy_table_value_is_error_(int yyvalue);

        static const signed char yypact_ninf_;
        static const signed char yytable_ninf_;

        /// Convert a scanner token number \a t to a symbol number.
        static token_number_type yytranslate_(token_type t);

        // Tables.
        // YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
        // STATE-NUM.
        static const short yypact_[];

        // YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
        // Performed when YYTABLE does not specify something else to do.  Zero
        // means the default is an error.
        static const unsigned char yydefact_[];

        // YYPGOTO[NTERM-NUM].
        static const short yypgoto_[];

        // YYDEFGOTO[NTERM-NUM].
        static const signed char yydefgoto_[];

        // YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
        // positive, shift that token.  If negative, reduce the rule whose
        // number is the opposite.  If YYTABLE_NINF, syntax error.
        static const unsigned char yytable_[];

        static const short yycheck_[];

        // YYSTOS[STATE-NUM] -- The (internal number of the) accessing
        // symbol of state STATE-NUM.
        static const unsigned char yystos_[];

        // YYR1[YYN] -- Symbol number of symbol that rule YYN derives.
        static const unsigned char yyr1_[];

        // YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.
        static const unsigned char yyr2_[];


        /// Convert the symbol name \a n to a form suitable for a diagnostic.
        static std::string yytnamerr_(const char *n);


        /// For a symbol, its name in clear.
        static const char *const yytname_[];
#if YYDEBUG
        // YYRLINE[YYN] -- Source line where rule number YYN was defined.
        static const unsigned char yyrline_[];

        /// Report on the debug stream that the rule \a r is going to be reduced.
        virtual void yy_reduce_print_(int r);

        /// Print the state stack on the debug stream.
        virtual void yystack_print_();

        // Debugging.
        int yydebug_;
        std::ostream *yycdebug_;

        /// \brief Display a symbol type, value and location.
        /// \param yyo    The output stream.
        /// \param yysym  The symbol.
        template<typename Base>
        void yy_print_(std::ostream &yyo, const basic_symbol<Base> &yysym) const;

#endif

        /// \brief Reclaim the memory associated to a symbol.
        /// \param yymsg     Why this token is reclaimed.
        ///                  If null, print nothing.
        /// \param yysym     The symbol.
        template<typename Base>
        void yy_destroy_(const char *yymsg, basic_symbol<Base> &yysym) const;

    private:
        /// Type access provider for state based symbols.
        struct by_state {
            /// Default constructor.
            by_state();

            /// The symbol type as needed by the constructor.
            typedef state_type kind_type;

            /// Constructor.
            by_state(kind_type s);

            /// Copy constructor.
            by_state(const by_state &other);

            /// Record that this symbol is empty.
            void clear();

            /// Steal the symbol type from \a that.
            void move(by_state &that);

            /// The (internal) type number (corresponding to \a state).
            /// \a empty_symbol when empty.
            symbol_number_type type_get() const;

            /// The state number used to denote an empty symbol.
            enum {
                empty_state = -1
            };

            /// The state.
            /// \a empty when empty.
            state_type state;
        };

        /// "Internal" symbol: element of the stack.
        struct stack_symbol_type : basic_symbol<by_state> {
            /// Superclass.
            typedef basic_symbol<by_state> super_type;

            /// Construct an empty symbol.
            stack_symbol_type();

            /// Move or copy construction.
            stack_symbol_type(YY_RVREF (stack_symbol_type)that);

            /// Steal the contents from \a sym to build this.
            stack_symbol_type(state_type s, YY_MOVE_REF (symbol_type)sym);

#if defined __cplusplus && __cplusplus < 201103L
            /// Assignment, needed by push_back by some old implementations.
            /// Moves the contents of that.
            stack_symbol_type& operator= (stack_symbol_type& that);
#endif
        };

        /// Stack type.
        typedef stack<stack_symbol_type> stack_type;

        /// The stack.
        stack_type yystack_;

        /// Push a new state on the stack.
        /// \param m    a debug message to display
        ///             if null, no trace is output.
        /// \param sym  the symbol
        /// \warning the contents of \a s.value is stolen.
        void yypush_(const char *m, YY_MOVE_REF (stack_symbol_type)sym);

        /// Push a new look ahead token on the state on the stack.
        /// \param m    a debug message to display
        ///             if null, no trace is output.
        /// \param s    the state
        /// \param sym  the symbol (for its value and location).
        /// \warning the contents of \a sym.value is stolen.
        void yypush_(const char *m, state_type s, YY_MOVE_REF (symbol_type)sym);

        /// Pop \a n symbols from the stack.
        void yypop_(int n = 1);

        /// Constants.
        enum {
            yyeof_ = 0,
            yylast_ = 207,     ///< Last index in yytable_.
            yynnts_ = 33,  ///< Number of nonterminal symbols.
            yyfinal_ = 9, ///< Termination state number.
            yyterror_ = 1,
            yyerrcode_ = 256,
            yyntokens_ = 54  ///< Number of tokens.
        };


    };

    // Symbol number corresponding to token number t.
    inline
    parser::token_number_type
    parser::yytranslate_(token_type t) {
        static
        const token_number_type
                translate_table[] =
                {
                        0, 2, 2, 2, 2, 2, 2, 2, 2, 2,
                        2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
                        2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
                        2, 2, 2, 2, 2, 2, 2, 53, 2, 2,
                        43, 44, 51, 49, 39, 50, 2, 52, 2, 2,
                        2, 2, 2, 2, 2, 2, 2, 2, 2, 38,
                        2, 40, 2, 2, 2, 2, 2, 2, 2, 2,
                        2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
                        2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
                        2, 45, 2, 46, 2, 2, 2, 2, 2, 2,
                        2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
                        2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
                        2, 2, 2, 41, 2, 42, 2, 2, 2, 2,
                        2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
                        2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
                        2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
                        2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
                        2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
                        2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
                        2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
                        2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
                        2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
                        2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
                        2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
                        2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
                        2, 2, 2, 2, 2, 2, 1, 2, 3, 4,
                        5, 6, 7, 8, 9, 10, 11, 12, 13, 14,
                        15, 16, 17, 18, 19, 20, 21, 22, 23, 24,
                        25, 26, 27, 28, 29, 30, 31, 32, 33, 34,
                        35, 36, 37, 47, 48
                };
        const unsigned user_token_number_max_ = 294;
        const token_number_type undef_token_ = 2;

        if (static_cast<int> (t) <= yyeof_)
            return yyeof_;
        else if (static_cast<unsigned> (t) <= user_token_number_max_)
            return translate_table[t];
        else
            return undef_token_;
    }

    inline
    parser::syntax_error::syntax_error(const location_type &l, const std::string &m)
            : std::runtime_error(m), location(l) {}

    // basic_symbol.
    template<typename Base>
    parser::basic_symbol<Base>::basic_symbol()
            : value(), location() {}

    template<typename Base>
    parser::basic_symbol<Base>::basic_symbol(YY_RVREF (basic_symbol)other)
            : Base(YY_MOVE(other)), value(), location(YY_MOVE(other.location)) {
        switch (other.type_get()) {
            case 37: // NUM
                value.YY_MOVE_OR_COPY<int>(YY_MOVE(other.value));
                break;

            case 35: // ID
            case 36: // STR
                value.YY_MOVE_OR_COPY<std::string>(YY_MOVE(other.value));
                break;

            default:
                break;
        }

    }


    // Implementation of basic_symbol constructor for each type.
    template<typename Base>
    parser::basic_symbol<Base>::basic_symbol(typename Base::kind_type t, YY_RVREF (location_type)l)
            : Base(t), location(YY_MOVE(l)) {}

    template<typename Base>
    parser::basic_symbol<Base>::basic_symbol(typename Base::kind_type t, YY_RVREF (int)v, YY_RVREF (location_type)l)
            : Base(t), value(YY_MOVE(v)), location(YY_MOVE(l)) {}

    template<typename Base>
    parser::basic_symbol<Base>::basic_symbol(typename Base::kind_type t, YY_RVREF (std::string)v,
                                             YY_RVREF (location_type)l)
            : Base(t), value(YY_MOVE(v)), location(YY_MOVE(l)) {}


    template<typename Base>
    parser::basic_symbol<Base>::~basic_symbol() {
        clear();
    }

    template<typename Base>
    void
    parser::basic_symbol<Base>::clear() {
        // User destructor.
        symbol_number_type yytype = this->type_get();
        basic_symbol<Base> &yysym = *this;
        (void) yysym;
        switch (yytype) {
            default:
                break;
        }

        // Type destructor.
        switch (yytype) {
            case 37: // NUM
                value.template destroy<int>();
                break;

            case 35: // ID
            case 36: // STR
                value.template destroy<std::string>();
                break;

            default:
                break;
        }

        Base::clear();
    }

    template<typename Base>
    bool
    parser::basic_symbol<Base>::empty() const {
        return Base::type_get() == empty_symbol;
    }

    template<typename Base>
    void
    parser::basic_symbol<Base>::move(basic_symbol &s) {
        super_type::move(s);
        switch (this->type_get()) {
            case 37: // NUM
                value.move<int>(YY_MOVE(s.value));
                break;

            case 35: // ID
            case 36: // STR
                value.move<std::string>(YY_MOVE(s.value));
                break;

            default:
                break;
        }

        location = YY_MOVE(s.location);
    }

    // by_type.
    inline
    parser::by_type::by_type()
            : type(empty_symbol) {}

    inline
    parser::by_type::by_type(const by_type &other)
            : type(other.type) {}

    inline
    parser::by_type::by_type(token_type t)
            : type(yytranslate_(t)) {}

    inline
    void
    parser::by_type::clear() {
        type = empty_symbol;
    }

    inline
    void
    parser::by_type::move(by_type &that) {
        type = that.type;
        that.clear();
    }

    inline
    int
    parser::by_type::type_get() const {
        return type;
    }

    inline
    parser::token_type
    parser::by_type::token() const {
        // YYTOKNUM[NUM] -- (External) token number corresponding to the
        // (internal) symbol number NUM (which must be that of a token).  */
        static
        const unsigned short
                yytoken_number_[] =
                {
                        0, 256, 257, 258, 259, 260, 261, 262, 263, 264,
                        265, 266, 267, 268, 269, 270, 271, 272, 273, 274,
                        275, 276, 277, 278, 279, 280, 281, 282, 283, 284,
                        285, 286, 287, 288, 289, 290, 291, 292, 59, 44,
                        61, 123, 125, 40, 41, 91, 93, 293, 294, 43,
                        45, 42, 47, 37
                };
        return static_cast<token_type> (yytoken_number_[type]);
    }

    // Implementation of make_symbol for each symbol type.
    inline
    parser::symbol_type
    parser::make_END(YY_COPY (location_type) l) {
        return symbol_type(token::TOK_END, YY_MOVE(l));
    }

    inline
    parser::symbol_type
    parser::make_ASSIGN(YY_COPY (location_type) l) {
        return symbol_type(token::TOK_ASSIGN, YY_MOVE(l));
    }

    inline
    parser::symbol_type
    parser::make_ADD_ASSIGN(YY_COPY (location_type) l) {
        return symbol_type(token::TOK_ADD_ASSIGN, YY_MOVE(l));
    }

    inline
    parser::symbol_type
    parser::make_SUB_ASSIGN(YY_COPY (location_type) l) {
        return symbol_type(token::TOK_SUB_ASSIGN, YY_MOVE(l));
    }

    inline
    parser::symbol_type
    parser::make_MUL_ASSIGN(YY_COPY (location_type) l) {
        return symbol_type(token::TOK_MUL_ASSIGN, YY_MOVE(l));
    }

    inline
    parser::symbol_type
    parser::make_DIV_ASSIGN(YY_COPY (location_type) l) {
        return symbol_type(token::TOK_DIV_ASSIGN, YY_MOVE(l));
    }

    inline
    parser::symbol_type
    parser::make_MOD_ASSIGN(YY_COPY (location_type) l) {
        return symbol_type(token::TOK_MOD_ASSIGN, YY_MOVE(l));
    }

    inline
    parser::symbol_type
    parser::make_MINUS(YY_COPY (location_type) l) {
        return symbol_type(token::TOK_MINUS, YY_MOVE(l));
    }

    inline
    parser::symbol_type
    parser::make_ADD(YY_COPY (location_type) l) {
        return symbol_type(token::TOK_ADD, YY_MOVE(l));
    }

    inline
    parser::symbol_type
    parser::make_STAR(YY_COPY (location_type) l) {
        return symbol_type(token::TOK_STAR, YY_MOVE(l));
    }

    inline
    parser::symbol_type
    parser::make_SLASH(YY_COPY (location_type) l) {
        return symbol_type(token::TOK_SLASH, YY_MOVE(l));
    }

    inline
    parser::symbol_type
    parser::make_MOD(YY_COPY (location_type) l) {
        return symbol_type(token::TOK_MOD, YY_MOVE(l));
    }

    inline
    parser::symbol_type
    parser::make_LPAREN(YY_COPY (location_type) l) {
        return symbol_type(token::TOK_LPAREN, YY_MOVE(l));
    }

    inline
    parser::symbol_type
    parser::make_RPAREN(YY_COPY (location_type) l) {
        return symbol_type(token::TOK_RPAREN, YY_MOVE(l));
    }

    inline
    parser::symbol_type
    parser::make_LSQUARE(YY_COPY (location_type) l) {
        return symbol_type(token::TOK_LSQUARE, YY_MOVE(l));
    }

    inline
    parser::symbol_type
    parser::make_RSQUARE(YY_COPY (location_type) l) {
        return symbol_type(token::TOK_RSQUARE, YY_MOVE(l));
    }

    inline
    parser::symbol_type
    parser::make_LBRACE(YY_COPY (location_type) l) {
        return symbol_type(token::TOK_LBRACE, YY_MOVE(l));
    }

    inline
    parser::symbol_type
    parser::make_RBRACE(YY_COPY (location_type) l) {
        return symbol_type(token::TOK_RBRACE, YY_MOVE(l));
    }

    inline
    parser::symbol_type
    parser::make_EQ(YY_COPY (location_type) l) {
        return symbol_type(token::TOK_EQ, YY_MOVE(l));
    }

    inline
    parser::symbol_type
    parser::make_NE(YY_COPY (location_type) l) {
        return symbol_type(token::TOK_NE, YY_MOVE(l));
    }

    inline
    parser::symbol_type
    parser::make_LT(YY_COPY (location_type) l) {
        return symbol_type(token::TOK_LT, YY_MOVE(l));
    }

    inline
    parser::symbol_type
    parser::make_GT(YY_COPY (location_type) l) {
        return symbol_type(token::TOK_GT, YY_MOVE(l));
    }

    inline
    parser::symbol_type
    parser::make_COMMA(YY_COPY (location_type) l) {
        return symbol_type(token::TOK_COMMA, YY_MOVE(l));
    }

    inline
    parser::symbol_type
    parser::make_SEMICOLON(YY_COPY (location_type) l) {
        return symbol_type(token::TOK_SEMICOLON, YY_MOVE(l));
    }

    inline
    parser::symbol_type
    parser::make_TYPE_STR(YY_COPY (location_type) l) {
        return symbol_type(token::TOK_TYPE_STR, YY_MOVE(l));
    }

    inline
    parser::symbol_type
    parser::make_TYPE_INT(YY_COPY (location_type) l) {
        return symbol_type(token::TOK_TYPE_INT, YY_MOVE(l));
    }

    inline
    parser::symbol_type
    parser::make_TYPE_VOID(YY_COPY (location_type) l) {
        return symbol_type(token::TOK_TYPE_VOID, YY_MOVE(l));
    }

    inline
    parser::symbol_type
    parser::make_WHILE(YY_COPY (location_type) l) {
        return symbol_type(token::TOK_WHILE, YY_MOVE(l));
    }

    inline
    parser::symbol_type
    parser::make_RETURN(YY_COPY (location_type) l) {
        return symbol_type(token::TOK_RETURN, YY_MOVE(l));
    }

    inline
    parser::symbol_type
    parser::make_IF(YY_COPY (location_type) l) {
        return symbol_type(token::TOK_IF, YY_MOVE(l));
    }

    inline
    parser::symbol_type
    parser::make_ELSE(YY_COPY (location_type) l) {
        return symbol_type(token::TOK_ELSE, YY_MOVE(l));
    }

    inline
    parser::symbol_type
    parser::make_SCAN(YY_COPY (location_type) l) {
        return symbol_type(token::TOK_SCAN, YY_MOVE(l));
    }

    inline
    parser::symbol_type
    parser::make_PRINT(YY_COPY (location_type) l) {
        return symbol_type(token::TOK_PRINT, YY_MOVE(l));
    }

    inline
    parser::symbol_type
    parser::make_ID(YY_COPY (std::string) v, YY_COPY (location_type) l) {
        return symbol_type(token::TOK_ID, YY_MOVE(v), YY_MOVE(l));
    }

    inline
    parser::symbol_type
    parser::make_STR(YY_COPY (std::string) v, YY_COPY (location_type) l) {
        return symbol_type(token::TOK_STR, YY_MOVE(v), YY_MOVE(l));
    }

    inline
    parser::symbol_type
    parser::make_NUM(YY_COPY (int) v, YY_COPY (location_type) l) {
        return symbol_type(token::TOK_NUM, YY_MOVE(v), YY_MOVE(l));
    }


} // yy
#line 1513 "./myparser.hh" // lalr1.cc:403


#endif // !YY_YY_MYPARSER_HH_INCLUDED
