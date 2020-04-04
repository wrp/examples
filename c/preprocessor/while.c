/*

From stackoverflow.com/a/10542793/140750

If you want to implement a while loop, you will need to use recursion in the preprocessor. The easiest way to do recursion is to use a deferred expression. A deferred expression is an expression that requires more scans to fully expand:

#define EMPTY()
#define DEFER(id) id EMPTY()
#define OBSTRUCT(id) id DEFER(EMPTY)()
#define EXPAND(...) __VA_ARGS__

#define A() 123
A() // Expands to 123
DEFER(A)() // Expands to A () because it requires one more scan to fully expand
EXPAND(DEFER(A)()) // Expands to 123, because the EXPAND macro forces another scan
Why is this important? Well when a macro is scanned and expanding, it creates a disabling context. This disabling context will cause a token, that refers to the currently expanding macro, to be painted blue. Thus, once its painted blue, the macro will no longer expand. This is why macros don't expand recursively. However, a disabling context only exists during one scan, so by deferring an expansion we can prevent our macros from becoming painted blue. We will just need to apply more scans to the expression. We can do that using this EVAL macro:

#define EVAL(...)  EVAL1(EVAL1(EVAL1(__VA_ARGS__)))
#define EVAL1(...) EVAL2(EVAL2(EVAL2(__VA_ARGS__)))
#define EVAL2(...) EVAL3(EVAL3(EVAL3(__VA_ARGS__)))
#define EVAL3(...) EVAL4(EVAL4(EVAL4(__VA_ARGS__)))
#define EVAL4(...) EVAL5(EVAL5(EVAL5(__VA_ARGS__)))
#define EVAL5(...) __VA_ARGS__
Next, we define some operators for doing some logic(such as if, etc):

#define CAT(a, ...) PRIMITIVE_CAT(a, __VA_ARGS__)
#define PRIMITIVE_CAT(a, ...) a ## __VA_ARGS__

#define CHECK_N(x, n, ...) n
#define CHECK(...) CHECK_N(__VA_ARGS__, 0,)

#define NOT(x) CHECK(PRIMITIVE_CAT(NOT_, x))
#define NOT_0 ~, 1,

#define COMPL(b) PRIMITIVE_CAT(COMPL_, b)
#define COMPL_0 1
#define COMPL_1 0

#define BOOL(x) COMPL(NOT(x))

#define IIF(c) PRIMITIVE_CAT(IIF_, c)
#define IIF_0(t, ...) __VA_ARGS__
#define IIF_1(t, ...) t

#define IF(c) IIF(BOOL(c))
Now with all these macros we can write a recursive WHILE macro. We use a WHILE_INDIRECT macro to refer back to itself recursively. This prevents the macro from being painted blue, since it will expand on a different scan(and using a different disabling context). The WHILE macro takes a predicate macro, an operator macro, and a state(which is the variadic arguments). It keeps applying this operator macro to the state until the predicate macro returns false(which is 0).

#define WHILE(pred, op, ...) \
    IF(pred(__VA_ARGS__)) \
    ( \
        OBSTRUCT(WHILE_INDIRECT) () \
        ( \
            pred, op, op(__VA_ARGS__) \
        ), \
        __VA_ARGS__ \
    )
#define WHILE_INDIRECT() WHILE
For demonstration purposes, we are just going to create a predicate that checks when number of arguments are 1:

#define NARGS_SEQ(_1,_2,_3,_4,_5,_6,_7,_8,N,...) N
#define NARGS(...) NARGS_SEQ(__VA_ARGS__, 8, 7, 6, 5, 4, 3, 2, 1)

#define IS_1(x) CHECK(PRIMITIVE_CAT(IS_1_, x))
#define IS_1_1 ~, 1,

#define PRED(x, ...) COMPL(IS_1(NARGS(__VA_ARGS__)))
Next we create an operator, which we will just concat two tokens. We also create a final operator(called M) that will process the final output:

#define OP(x, y, ...) CAT(x, y), __VA_ARGS__
#define M(...) CAT(__VA_ARGS__)
Then using the WHILE macro:

M(EVAL(WHILE(PRED, OP, x, y, z))) //Expands to xyz
Of course, any kind of predicate or operator can be passed to it.
*/
