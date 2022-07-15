#pragma once

#define INTRO_STR_IMPL(X) #X
#define INTRO_STR(X) INTRO_STR_IMPL(X)

#define INTRO_CAT_IMPL(A, B) A##B
#define INTRO_CAT(A, B) INTRO_CAT_IMPL(A, B)

/**
 * @brief Get element from variadic argument list.
 */
#define INTRO_GET_FIRST(FIRST, ...) FIRST
#define INTRO_GET_SECOND(FIRST, SECOND, ...) SECOND

/**
 * @brief Helper macros for implementing Basic Recursion
 *
 * Given:
 *
 * #define A(x) x B MAP_OUT (x)
 * #define B(x) x A MAP_OUT (x)
 *
 * Calling INTRO_EVAL (A (blah)) would produce 365 copies of the word blah,
 * followed by a final un-evaluated B (blah). This provides the basic framework
 * for recursion, at least within a certain stack depth.
 *
 * Note: Each level of the EVAL macro multiplies the effort of the previous
 * level by 3, but also adds one evaluation of its own. Invoking the macro as a
 * whole adds one more level, taking the total to:
 * 1 + (3 * (3 * (3 * (3 * (3 * (1) + 1) + 1) + 1) + 1) + 1) = 365
 *
 * @see https://stackoverflow.com/a/13459454
 * @see https://github.com/swansontec/map-macro#basic-recursion
 */
#define INTRO_EVAL0(...) __VA_ARGS__
#define INTRO_EVAL1(...) INTRO_EVAL0(INTRO_EVAL0(INTRO_EVAL0(__VA_ARGS__)))
#define INTRO_EVAL2(...) INTRO_EVAL1(INTRO_EVAL1(INTRO_EVAL1(__VA_ARGS__)))
#define INTRO_EVAL3(...) INTRO_EVAL2(INTRO_EVAL2(INTRO_EVAL2(__VA_ARGS__)))
#define INTRO_EVAL4(...) INTRO_EVAL3(INTRO_EVAL3(INTRO_EVAL3(__VA_ARGS__)))
#define INTRO_EVAL(...) INTRO_EVAL4(INTRO_EVAL4(INTRO_EVAL4(__VA_ARGS__)))

/**
 * @brief Defer expansion for one evaluation.
 *
 * @see
 * https://github.com/pfultz2/Cloak/wiki/C-Preprocessor-tricks,-tips,-and-idioms
 */
#define INTRO_EMPTY(...)
#define INTRO_DEFER0(...) __VA_ARGS__
#define INTRO_DEFER1(...) __VA_ARGS__ INTRO_DEFER0(INTRO_EMPTY)()
#define INTRO_DEFER2(...) __VA_ARGS__ INTRO_DEFER1(INTRO_EMPTY)()
#define INTRO_DEFER3(...) __VA_ARGS__ INTRO_DEFER2(INTRO_EMPTY)()
#define INTRO_DEFER4(...) __VA_ARGS__ INTRO_DEFER3(INTRO_EMPTY)()

/**
 * @brief Helper macros for implementing End Detection
 *
 * INTRO_CHECK_TERMINATION returns INTRO_EMPTY if test is () and INTRO_DEFER0
 * otherwise.
 *
 * @see https://stackoverflow.com/a/13459454
 * @see https://github.com/swansontec/map-macro#end-detection
 * @see
 * https://github.com/pfultz2/Cloak/wiki/C-Preprocessor-tricks,-tips,-and-idioms
 */
#define INTRO_MOVE_PARAM_IF_TERMINATION_IMPL() , INTRO_EMPTY
#define INTRO_MOVE_PARAM_IF_TERMINATION(...)                                \
    INTRO_GET_SECOND(, ##__VA_ARGS__, INTRO_MOVE_PARAM_IF_TERMINATION_IMPL) \
    (__VA_ARGS__)
#define INTRO_IF_NOT_TERMINATION(TEST) \
    INTRO_DEFER1(INTRO_GET_SECOND)     \
    (INTRO_MOVE_PARAM_IF_TERMINATION TEST, INTRO_DEFER0)

/**
 * @brief Helper macro for implementing tail recursion.
 *
 * In order for this macro to work <macro>_INDIRECTION and <macro>_IMPL have to
 * be implemented.
 *
 * @see
 * https://github.com/pfultz2/Cloak/wiki/C-Preprocessor-tricks,-tips,-and-idioms
 */
#define INTRO_RECURSION_NEXT_STEP(MACRO_INDIRECTION, ARGS, PEEK, ...) \
    INTRO_IF_NOT_TERMINATION(PEEK)                                    \
    (INTRO_DEFER3(MACRO_INDIRECTION)()(INTRO_DEFER0 ARGS, PEEK, __VA_ARGS__))

#define INTRO_TAIL_RECURSION_IMPL(MACRO_NAME, ARGS, VALUE, ...)            \
    INTRO_IF_NOT_TERMINATION(VALUE)                                        \
    (INTRO_DEFER3(INTRO_CAT(MACRO_NAME, _IMPL))(INTRO_DEFER0 ARGS, VALUE)) \
                                                                           \
        INTRO_RECURSION_NEXT_STEP(                                         \
            INTRO_CAT(MACRO_NAME, _INDIRECTION), ARGS, __VA_ARGS__)

#define INTRO_TAIL_RECURSION(...) INTRO_TAIL_RECURSION_IMPL(__VA_ARGS__, (), ())

/**
 * @brief Applies the function macro `f` to each of the remaining parameters.
 *
 * Example:
 *
 *     INTRO_MAP(
 *         A,
 *         ARG1,
 *         ARG2,
 *         ARG3
 *     )
 *
 * Produces:
 *
 *     A(ARG1)
 *     A(ARG2)
 *     A(ARG3)
 *
 * @see https://stackoverflow.com/a/13459454
 * @see https://github.com/swansontec/map-macro
 */
#define INTRO_MAP(...) INTRO_EVAL(INTRO_MAP_NO_EVAL(__VA_ARGS__))
#define INTRO_MAP_NO_EVAL(FUNC, ...) \
    INTRO_TAIL_RECURSION(INTRO_MAP, (FUNC), ##__VA_ARGS__)
#define INTRO_MAP_INDIRECTION() INTRO_MAP_NO_EVAL
#define INTRO_MAP_IMPL(FUNC, VALUE) INTRO_DEFER1(FUNC)(VALUE)
