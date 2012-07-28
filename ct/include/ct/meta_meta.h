/************************************************************************/
/*                                                                      */
/*  This file is a part of the example programs from the book:          */
/*                                                                      */
/*  Krzysztof Czarnecki and Ulrich Eisenecker.                          */
/*  Generative Programming: Methods, Tools, and Applications.           */
/*  Addison-Wesley Longman, 2000                                        */
/*                                                                      */
/*  You may use, modify, and distribute this software according         */
/*  to the terms stated in the LICENSE file included in this            */
/*  distribution.                                                       */
/*                                                                      */
/*  Copyright 1998-2000 by Krzysztof Czarnecki and Ulrich W. Eisenecker */
/*  E-mail: czarnecki@acm.org and ulrich.eisenecker@t-online.de         */
/*                                                                      */
/*  Please direct questions, bug reports, and contributions to the      */
/*  authors.                                                            */
/*                                                                      */
/*  THIS SOFTWARE IS PROVIDED AS IS AND WITHOUT ANY EXPRESSED OR        */
/*  IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED      */
/*  WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE. */
/*                                                                      */
/************************************************************************/

/************************************************************************/
/* Acknowledgement: Johannes Knaupp contributed to the implementation   */
/* of EVAL_DEPENDENCY_TABLE                                             */
/************************************************************************/

// July 23 2003: Bing-Chang Lai: Removed implicit typenames
// August 14 2003: Bing-Chang Lai: Added EFOR3, EFOR4, EFOR5

#ifndef META_H_
#define META_H_

#ifdef _MSC_VER
  #if (_MSC_VER<1100)
    #error "Need Visual C++ 5.0 or later"
  #endif
  // MS VC++5.0 does not like WHILE, DO, and FOR
  #define NO_STATIC_LOOPS
  // MS VC++5.0 does not like typename in certain places
  #define TYPENAME_
#else //_MSC_VER
  // C++-Standard-compliant compilers
  #define TYPENAME_ typename
#endif //_MSC_VER

namespace meta
{

/**
 * @name Static selection constructs
 * @memo IF, SWITCH, and EVAL_DEPENDENCY_TABLE
 * @type
 */
//@{


// BEGIN of IF -----------------

// helper selelection templates -----------------
namespace intimate
{ //selector template selecting ThenType
  struct SelectThen 
  { template<class ThenType, class ElseType>
    struct Result
    { typedef ThenType RET; };
  }; 

  //selector template selecting ElseType
  struct SelectElse
  { template<class ThenType, class ElseType>
    struct Result
    { typedef ElseType RET; };
  };

  //choose selector template depending on condition
  template<bool condition>
  struct ChooseSelector
  { typedef SelectThen RET; };

  template<>
  struct ChooseSelector<false>
  { typedef SelectElse RET; };

}; //end of namespace intimate


/**
 * Selects one of two argument types depending on the condition.
 * {\tt Then} if condition is true and {\tt Else} otherwise.
 * 
 * {\bf Usage:}
 *   
 * Include File:
 * \URL[boost/meta.hpp]{../../../boost/meta.hpp}
 *   
 * \begin{verbatim}
     IF<(1+2>4), short, int>::RET i; //the type of i is int!
   \end{verbatim}
 *
 * See Example:
 * \URL[example/if.cpp]{../examples/if.cpp}
 */
template<bool condition, class Then, class Else>
class IF
{ typedef typename intimate::ChooseSelector<condition>::RET Selector;
public:
  typedef typename Selector::template Result<Then,Else>::RET RET;
};

/* The following is a simpler implementation of IF<>,
 * but it requires partial template specialization:
 *
 * template<bool condition, class Then, class Else>
 * struct IF
 * { typedef Then RET;
 * };
 *
 * //specialization for condition==false
 * template<class Then, class Else>
 * struct IF<false, Then, Else>
 * { typedef Else RET;
 * };
 *
 */
// END of IF -----------------


// BEGIN of SWITCH -----------------

// case list -----------------

const int DEFAULT  = ~(~0u >> 1); //initialize with the smallest int
const int nilValue = DEFAULT+1;

struct NilCase 
{ enum {tag = nilValue};
  typedef NilCase RET;
}; 

template <int tag_,class Type_,class Next_ = NilCase>
struct CASE
{ enum {tag = tag_};
  typedef Type_ Type;
  typedef Next_ Next;
};


/**
 * Selects one of several types depending on tag.
 * {\tt Case} is a list of cases.
 * 
 * {\bf Usage:}
 *   
 * Include File:
 * \URL[boost/meta.hpp]{../../../boost/meta.hpp}
 *   
 * \begin{verbatim}
     SWITCH<(1+1-2),
         CASE<1,char,
         CASE<2,short,
         CASE<DEFAULT,int > > >
     >::RET i; //the DEFAULT case is selected and thus the type of i is int
   \end{verbatim}
 *
 * See Example:
 * \URL[examples/switch.cpp]{../examples/switch.cpp}
 *
 *
 */
template <int tag,class Case>
class SWITCH
{ typedef typename Case::Next NextCase;
  enum { caseTag = Case::tag,    
         nextTag = NextCase::tag,
         found   = (caseTag == tag ||
                    caseTag == DEFAULT) 
       };
  typedef typename IF<(nextTag == nilValue),
             NilCase,
             SWITCH<tag,NextCase> 
            >::RET NextSwitch;
public:
  typedef typename IF<found,
             typename Case::Type,  
             typename NextSwitch::RET
            >::RET RET;
};

// END of SWITCH -----------------


// BEGIN of EVAL_DEPENDENCY_TABLE -----------------

// helper structs -----------------

enum { endValue = ~(~0u >> 1),         // samallest signed integer value
       anyValue = endValue + 1         // second smallest int
     };


struct End
{
   enum    { value = endValue };
   typedef End Head;
   typedef End Tail;
};

namespace intimate {

struct _Nil_ {};

template< int found_, class ResultList_ >
    struct ResultOfRowEval
    {
        enum    { found = found_ };
        typedef ResultList_ ResultList;
    };
};

struct ERROR__NoMatchingTableRow
{
   typedef ERROR__NoMatchingTableRow ResultType;
};


// syntax elements for expressing table elements -----------------

template< class ThisRow, class FollowingRows = End >
struct ROW
{
   typedef ThisRow         Head;
   typedef FollowingRows   Tail;
};


template< int value_, class FurtherCells = End >
struct CELL
{
   enum    { value = value_ };
   typedef FurtherCells  Tail;
};


template< class ThisResultType, class FurtherResultTypes = End >
struct RET
{
   typedef ThisResultType        ResultType;
   typedef FurtherResultTypes    Tail;
   enum    { value = endValue };
};


// metafunction EVAL_DEPENDENCY_TABLE for matching the head row against one body row -----------------
namespace intimate {

template< class HeadRow, class TestRow >
class EVAL_ROW
{
      typedef typename HeadRow::Tail HeadTail;
      typedef typename TestRow::Tail RowTail;

      enum { headValue = HeadRow::value,
             testValue = TestRow::value,
             isLast    = (HeadTail::value == endValue),
             isMatch   = (testValue == anyValue) || (testValue == headValue)
           };

      typedef typename IF< isLast, 
                  intimate::ResultOfRowEval< true, RowTail >,
                  typename EVAL_ROW< HeadTail, RowTail >::RET
                >::RET ResultOfFollowingCols;

   public:
      typedef typename IF< isMatch,
                  ResultOfFollowingCols,
                  intimate::ResultOfRowEval< false, ERROR__NoMatchingTableRow >
                >::RET RET;
};

template<>
class EVAL_ROW< End, End >
{
public:
   typedef intimate::_Nil_ RET;
};

};//namespace intimate

// metafunction EVAL_DEPENDENCY_TABLE **********************
/**
 * Selects one of several table rows.
 * {\tt HeadRow} is the row to be mached against the rows
 * in {\tt TableBody}. {\tt TableBody} is a list of rows having one column
 * more than the {\tt HeadRow}. The last colum in {\tt TableBody} contains
 * the return values.
 * {\tt EVAL_DEPENDENCY_TABLE} takes {\tt HeadRow} and compares it with
 * the rows in {\tt TableBody} in a top-down order. When the first matching
 * row is found, the corresponding return value is returned. The special
 * value {\tt anyValue} matches any value. The {\tt RET} element for the
 * return column can be chained into lists in order to return several types
 * at once. If no matching row is found, a compile-time error is reported.
 * 
 * {\bf Usage:}
 *   
 * Include File:
 * \URL[boost/meta.hpp]{../../../boost/meta.hpp}
 *   
 * \begin{verbatim}
    //this just for testing
    template< int val_ >
    struct Num
    {  enum { val = val_ };
    };

    //a couple of types for testing
    typedef Num< 1 > One;
    typedef Num< 2 > Two;
    typedef Num< 3 > Three;
    typedef Num< 4 > Four;

    void main ()
    {  // test table with a single return type column
      typedef EVAL_DEPENDENCY_TABLE
          //*********************************************
          <      CELL<  1,  CELL<  2                  > >
          , ROW< CELL<  1,  CELL<  3,  RET<  One    > > >
          , ROW< CELL<  1,  CELL<  3,  RET<  Two    > > >
          , ROW< CELL<  1,  CELL<  2,  RET<  Three  > > >
          , ROW< CELL<  1,  CELL<  3,  RET<  Four   > > >
          //*********************************************
          > > > > >::RET Table_1_Return;

       cout << Table_1_Return::val << endl; //prints "3"


      // test table with two return type columns
      typedef EVAL_DEPENDENCY_TABLE
          //*******************************************************************
          <      CELL<        4, CELL<   7                                  > >

          , ROW< CELL<        3, CELL<   7,      RET< Three, RET< Four  > > > >
          , ROW< CELL<        4, CELL<   5,      RET< Four,  RET< Three > > > >
          , ROW< CELL< anyValue, CELL<   7,      RET< One,   RET< Two   > > > >
          , ROW< CELL< anyValue, CELL< anyValue, RET< Two,   RET< One   > > > >
          //*******************************************************************
          > > > > >::RET_List ResultRow;

      typedef ResultRow::      ResultType ReturnType_1;
      typedef ResultRow::Tail::ResultType ReturnType_2;

      cout << ReturnType_1::val << '\t'
           << ReturnType_2::val << endl; //prints "1  2"
    }
   \end{verbatim}
 *
 * See Example:
 * \URL[examples/table.cpp]{../examples/table.cpp}
 *
 *
 */
template< class HeadRow, class TableBody >
class EVAL_DEPENDENCY_TABLE
{
      typedef typename intimate::EVAL_ROW< HeadRow, typename TableBody::Head >::RET RowResult;
      typedef typename RowResult::ResultList ResultList;
      typedef typename TableBody::Tail FurtherRows;
      typedef typename FurtherRows::Head FurtherRowsHead;

      enum { found     = RowResult::found, 
             isLastRow = (FurtherRowsHead::value == endValue)
           };

      typedef typename IF< isLastRow, End, HeadRow >::RET HeadRow_;
      typedef typename IF< isLastRow,
                  ERROR__NoMatchingTableRow,
                  typename EVAL_DEPENDENCY_TABLE< HeadRow_, FurtherRows >::RET_List
                >::RET NextTry;

   public:
      typedef typename IF< found, ResultList, NextTry >::RET RET_List;
      typedef typename RET_List::ResultType RET;
};

template<>
class EVAL_DEPENDENCY_TABLE< End, End >
{
public:
   typedef intimate::_Nil_ RET_List;
};

//end of static selection metafunctions
//@}


#ifndef NO_STATIC_LOOPS

/**
 * @name Static loops
 * @memo WHILE, DO, and FOR
 */
//@{

namespace intimate
{
  template<class Statement>
  struct STOP
  { typedef Statement RET;
  };
}; //end of namespace intimate


/**
 * Executes {\tt Statement} until {\tt Condition} is satisfied.
//@ "is" inserted at the end
 * {\tt Statement} is expected to provide a member type named {\tt Next}.
 * {\tt Condition} is a class template taking the current index
 * as its parameter and providing a member type called {\tt Code}
 * with the member constant {\tt RET}, which is the termination condition.
 * {\tt WHILE} is useful for implementing template metafunctions requiring iteration. 
 * 
 * {\bf Usage:}
 *   
 * Include File:
 * \URL[boost/meta.hpp]{../../../boost/meta.hpp}
 *
 * {\bf Example:} computing Fibonacci numbers
 * \begin{verbatim}
    //iteration statement for Fib<>
    template<int i_, int x_, int y>
    struct FibStat
    { enum { i = i_,
             x = x_
           };
      typedef FibStat<i+1, x+y, x> Next;
    };

    //continue condition for Fib<>
    template<int n>
    struct FibCond
    { template<class Statement>
      struct Code
      { enum { RET = Statement::i < n };
      };
    };

    //Fib<>
    template<int n>
    struct Fib
    { enum { RET = WHILE<FibCond<n>,FibStat<1,1,0> >::RET::x };
    };


    void main()
    {
      cout << Fib<8>::RET << endl;
    }

   \end{verbatim}
 *
 * See Example:
 * \URL[examples/while.cpp]{../examples/while.cpp}
 *
 *
 */
template<class Condition, class Statement>
struct WHILE
{ typedef typename IF<Condition::template Code<Statement>::RET,
                      WHILE<Condition,typename Statement::Next>,
                      intimate::STOP<Statement>
                     >::RET::RET RET;
};


/**
 * Executes {\tt Statement} at least once and repeats the execution
 * until {\tt Condition} satisfied.
 * {\tt Statement} is expected to provide a member type named {\tt Next}.
 * {\tt Condition} is a class template taking the current index
 * as its parameter and providing a member type called {\tt Code}
 * with the member constant {\tt RET}, which is the termination condition.
 * {\tt WHILE} is useful for implementing template metafunctions requiring iteration. 
 * 
 * {\bf Usage:}
 *   
 * Include File:
 * \URL[boost/meta.hpp]{../../../boost/meta.hpp}
 *
 * {\bf Example:} computing the integral part of the square root of an integer
 * \begin{verbatim}
    //iteration statement for Sqrt<>
    template<int n_, int p_>
    struct SqrtStat
    { enum { p = p_,
             n = n_
           };
      typedef SqrtStat<n-p-2, p+2> Next;
    };

    //continue condition for Sqrt<>
    struct SqrtCond
    { template<class Statement>
      struct Code
      { enum { RET = Statement::n >= 0 };
      };
    };

    //Sqrt<>
    template<int n>
    struct Sqrt
    { enum { p = DO<SqrtStat<n,-1>,SqrtCond>::RET::p,
             RET = p/2
           };
    };


    void main()
    { cout << "sqrt(16) = " << Sqrt<16>::RET << endl;       
    }
   \end{verbatim}
 *
 * See Example:
 * \URL[examples/do.cpp]{../examples/do.cpp}
 *
 *
 */
template<class Statement, class Condition>
class DO
{ typedef typename Statement::Next NewStatement;
public:
  typedef typename IF<Condition::template Code<NewStatement>::RET,
                      DO<NewStatement,Condition>,
                      intimate::STOP<NewStatement>
                     >::RET::RET RET;
};


/**
 * Executes {\tt Statement} for a given number of times.
 * {\tt Statement} is executed until {\tt n Compare to}.
 * {\tt Compare} is one of the \URL[comparators]{FOR.html} {\tt Greater}, {\tt GreaterEqual},
 * {\tt Less}, or {\tt LessEqual}. The value of {\tt n} starts with
 * {\tt from} and is advanced by {\tt by} in each iteration.
 * {\tt Statement} is expected to provide a member class template
 * {\tt Code} expecting {\tt n} as its parameter and containing a member type
 * named {\tt Next}.
 * 
 * {\bf Usage:}
 *   
 * Include File:
 * \URL[boost/meta.hpp]{../../../boost/meta.hpp}
 *
 * {\bf Example:} computing Fibonacci numbers
 * \begin{verbatim}
     //statement for Fib<>
    template<int x_, int y>
    struct FibStat
    { template<int i>
      struct Code
      { enum { x = x_ }; //need to read out the result later
        typedef FibStat<x+y,x> Next;
      };
    };

    //Fib<>
    template<int n>
    struct Fib
    { enum { RET = FOR<1,Less,n,+1,FibStat<1,0> >::RET::x };
    };

    void main()
    { cout << Fib<8>::RET << endl;
    }
   \end{verbatim}
 *
 * See Example:
 * \URL[examples/for.cpp]{../examples/for.cpp}
 *
 *
 */
template<int from, class Compare, int to, int by, class Statement>
struct FOR
{ typedef typename IF<Compare::template Code<from,to>::RET,
                      FOR<from+by,Compare,to,by,typename Statement::template Code<from>::Next>,
                      intimate::STOP<typename Statement::template Code<from> >
                     >::RET::RET RET;
};


//end of static loops
//@}

#undef NO_STATIC_LOOPS
#endif //NO_STATIC_LOOPS


/**
 * @name Static execute-loops
 * @memo EWHILE, EDO, and EFOR
 * are loops for generating code by recursive inlining
 * (also useful for testing metafunctions).
 */
//@{

namespace intimate
{
  struct Stop
  { static void exec() {};
    template<class Env>
    static void exec(Env&) {};
	// Added by Bing-Chang Lai for EFOR2
	template<class Env1, class Env2>
	static void exec(Env1&, Env2&) {};
	// Added by Bing-Chang Lai for EFOR3
	template<class Env1, class Env2, class Env3>
	static void exec(Env1&, Env2&, Env3&) {};
	// Added by Bing-Chang Lai for EFOR4
	template<class Env1, class Env2, class Env3, class Env4>
	static void exec(Env1&, Env2&, Env3&, Env4&) {};
	// Added by Bing-Chang Lai for EFOR5
	template<class Env1, class Env2, class Env3, class Env4, class Env5>
	static void exec(Env1&, Env2&, Env3&, Env4&, Env5&) {};
  };
}; //end of namespace intimate


/**
 * EDO.
 * 
 * {\bf Usage:}
 *   
 * Include File:
 * \URL[boost/meta.hpp]{../../../boost/meta.hpp}
 *
 * See Example:
 * \URL[examples/edo.cpp]{../examples/edo.cpp}
 *
 */
template <class Statement, class Condition>
class EDO
{ typedef typename Statement::Next NextStatement;
public:
  static void exec()
  { Statement::exec();
    IF<Condition::template Code<NextStatement>::RET,
       EDO<NextStatement,Condition>,
       intimate::Stop
      >::RET::exec();
  }
};


/**
 * EDO1.
 * 
 * {\bf Usage:}
 *   
 * Include File:
 * \URL[boost/meta.hpp]{../../../boost/meta.hpp}
 *
 * See Example:
 * \URL[examples/edo.cpp]{../examples/edo.cpp}
 *
 */
template <class Statement, class Condition>
class EDO1
{ typedef typename Statement::Next NextStatement;
public:
  template<class Env>
  static void exec(Env& env)
  { Statement::exec(env);
    IF<Condition::template Code<NextStatement>::RET,
       EDO1<NextStatement,Condition>,
       intimate::Stop
      >::RET::exec(env);
  }
};


/**
 * EWHILE.
 * 
 * {\bf Usage:}
 *   
 * Include File:
 * \URL[boost/meta.hpp]{../../../boost/meta.hpp}
 *
 * See Example:
 * \URL[examples/ewhile.cpp]{../examples/ewhile.cpp}
 *
 */
template<class Condition, class Statement >
struct EWHILE
{ static void exec()
  { IF<Condition::template Code<Statement>::RET,
       EDO<Statement,Condition>,
       intimate::Stop
      >::RET::exec();
  }
};


/**
 * EWHILE1.
 * 
 * {\bf Usage:}
 *   
 * Include File:
 * \URL[boost/meta.hpp]{../../../boost/meta.hpp}
 *
 * See Example:
 * \URL[examples/ewhile.cpp]{../examples/ewhile.cpp}
 *
 */
template<class Condition, class Statement >
struct EWHILE1
{ template<class Env>
  static void exec(Env& env)
  { IF<Condition::template Code<Statement>::RET,
       EDO1<Statement,Condition>,
       intimate::Stop
      >::RET::exec(env);
  }
};


/**
 * EFOR.
 * 
 * {\bf Usage:}
 *   
 * Include File:
 * \URL[boost/meta.hpp]{../../../boost/meta.hpp}
 *
 * See Example:
 * \URL[examples/efor.cpp]{../examples/efor.cpp}
 *
 */
template <int from, class Compare, int to, int by, class Statement>
struct EFOR
{ static void exec()
  { IF<Compare::template Code<from,to>::RET,
       TYPENAME_ Statement::template Code<from>,
       intimate::Stop
      >::RET::exec();
    IF<Compare::template Code<from,to>::RET,
       EFOR<from+by,Compare,to,by,Statement>,
       intimate::Stop
      >::RET::exec();
  }
};


/**
 * EFOR1.
 * 
 * {\bf Usage:}
 *   
 * Include File:
 * \URL[boost/meta.hpp]{../../../boost/meta.hpp}
 *
 * See Example:
 * \URL[examples/efor.cpp]{../examples/efor.cpp}
 *
 */
template <int from, class Compare, int to, int by, class Statement>
struct EFOR1
{ template<class Env>
  static void exec(Env& env)
  { IF<Compare::template Code<from,to>::RET,
       TYPENAME_ Statement::template Code<from>,
       intimate::Stop
      >::RET::exec(env);
    IF<Compare::template Code<from,to>::RET,
           EFOR1<from+by,Compare,to,by,Statement>,
           intimate::Stop
      >::RET::exec(env);
  }
};

/**
 * EFOR2.
 * 
 * {\bf Usage:}
 *   
 * Include File:
 * \URL[boost/meta.hpp]{../../../boost/meta.hpp}
 *
 * See Example:
 * \URL[examples/efor.cpp]{../examples/efor.cpp}
 *
 * Added by Bing-Chang Lai (July 12 2003)
 *
 */
template <int from, class Compare, int to, int by, class Statement>
struct EFOR2
{ template<class Env1, class Env2>
  static void exec(Env1& env1, Env2& env2)
  { IF<Compare::template Code<from,to>::RET,
       TYPENAME_ Statement::template Code<from>,
       intimate::Stop
      >::RET::exec(env1, env2);
    IF<Compare::template Code<from,to>::RET,
           EFOR2<from+by,Compare,to,by,Statement>,
           intimate::Stop
      >::RET::exec(env1, env2);
  }
};

/**
 * EFOR3.
 * 
 * {\bf Usage:}
 *   
 * Include File:
 * \URL[boost/meta.hpp]{../../../boost/meta.hpp}
 *
 * See Example:
 * \URL[examples/efor.cpp]{../examples/efor.cpp}
 *
 * Added by Bing-Chang Lai (August 12 2003)
 *
 */
template <int from, class Compare, int to, int by, class Statement>
struct EFOR3
{ template<class Env1, class Env2, class Env3>
  static void exec(Env1& env1, Env2& env2, Env3& env3)
  { IF<Compare::template Code<from,to>::RET,
       TYPENAME_ Statement::template Code<from>,
       intimate::Stop
      >::RET::exec(env1, env2, env3);
    IF<Compare::template Code<from,to>::RET,
           EFOR3<from+by,Compare,to,by,Statement>,
           intimate::Stop
      >::RET::exec(env1, env2, env3);
  }
};

/**
 * EFOR4.
 * 
 * {\bf Usage:}
 *   
 * Include File:
 * \URL[boost/meta.hpp]{../../../boost/meta.hpp}
 *
 * See Example:
 * \URL[examples/efor.cpp]{../examples/efor.cpp}
 *
 * Added by Bing-Chang Lai (August 12 2003)
 *
 */
template <int from, class Compare, int to, int by, class Statement>
struct EFOR4
{ template<class Env1, class Env2, class Env3, class Env4>
  static void exec(Env1& env1, Env2& env2, Env3& env3, Env4& env4)
  { IF<Compare::template Code<from,to>::RET,
       TYPENAME_ Statement::template Code<from>,
       intimate::Stop
      >::RET::exec(env1, env2, env3, env4);
    IF<Compare::template Code<from,to>::RET,
           EFOR4<from+by,Compare,to,by,Statement>,
           intimate::Stop
      >::RET::exec(env1, env2, env3, env4);
  }
};

/**
 * EFOR5.
 * 
 * {\bf Usage:}
 *   
 * Include File:
 * \URL[boost/meta.hpp]{../../../boost/meta.hpp}
 *
 * See Example:
 * \URL[examples/efor.cpp]{../examples/efor.cpp}
 *
 * Added by Bing-Chang Lai (August 12 2003)
 *
 */
template <int from, class Compare, int to, int by, class Statement>
struct EFOR5
{ template<class Env1, class Env2, class Env3, class Env4, class Env5>
  static void exec(Env1& env1, Env2& env2, Env3& env3, Env4& env4, Env5& env5)
  { IF<Compare::template Code<from,to>::RET,
       TYPENAME_ Statement::template Code<from>,
       intimate::Stop
      >::RET::exec(env1, env2, env3, env4, env5);
    IF<Compare::template Code<from,to>::RET,
           EFOR5<from+by,Compare,to,by,Statement>,
           intimate::Stop
      >::RET::exec(env1, env2, env3, env4, env5);
  }
};

//end of static execute-loops
//@}


/**
 * @name Comparators for static for-loops (\Ref{FOR} and \Ref{EFOR})
 * @memo
 *
 * {\bf Usage:}
 *   
 * Include File:
 * \URL[boost/meta.hpp]{../../../boost/meta.hpp}
 *
 * See Example:
 * \URL[examples/for.cpp]{../examples/for.cpp}
 * \URL[examples/efor.cpp]{../examples/efor.cpp}
 *
 */
//@{

///
struct Greater
{ template<int x, int y>
  struct Code
  { enum { RET = x>y };
  };
};

///
struct GreaterEqual
{ template<int x, int y>
  struct Code
  { enum { RET = x>=y };
  };
};

///
struct Less
{ template<int x, int y>
  struct Code 
  { enum { RET = x<y };
  };
};

///
struct LessEqual
{ template<int x, int y>
  struct Code 
  { enum { RET = x<=y };
  };
};

//end of coditions for static for-loops
//@}


/**
 * @name Metafunctions for mapping indices
 * @memo Square, Rectangle, MapToRectangle, Triangle, StrictTriangle allow you to
 * iterate over two-dimensional shapes (e.g., the lower triangle of a matrix).
 *
 * {\bf Usage:}
 * Include File:
 * \URL[boost/meta.hpp]{../../../boost/meta.hpp}
 *
 * See Example:
 * \URL[examples/index_mapping.cpp]{../examples/index_mapping.cpp}
 *
 */
//@{

///
template <int width>
struct Square
{ enum { size = width*width };
};

///
template <int width, int hight>
struct Rectangle
{ enum { size = width*hight };
};

///
template <int width, int index>
struct MapToRectangle
{ enum { i = index / width,
         j = index % width
       };
};

///
template <int width>
struct Triangle
{ enum { size = width*(width + 1)/2 };
};

///
template <int width>
struct StrictTriangle
{ enum { size = Triangle<width-1>::size };
};

namespace intimate
{
  template <int width,int index>
  struct TriangleCalcI
  { enum { i = TriangleCalcI<(index>=width ? width-1     : 0),
                             (index>=width ? index-width : 0)
                            >::i+1
         };
  };

  template<>
  struct TriangleCalcI<0,0>
  { enum { i = -1 };
  };
}; //end of namespace intimate


///
template <int width,int index>
struct MapToUpperTriangle
{ enum { i = intimate::TriangleCalcI<width,index>::i,
         j = index + Triangle<i>::size - i*width
       };
};

///
template <int width,int index>
struct MapToStrictUpperTriangle
{ enum { i = MapToUpperTriangle<width-1,index>::i,
         j = MapToUpperTriangle<width-1,index>::j+1
       };
};

//end of metafunctions for mapping indices
//@}

}; //namespace meta

#undef TYPENAME_

#endif //META_H_