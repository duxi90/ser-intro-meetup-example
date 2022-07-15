#pragma once

#include <utility>

namespace intro {

/**
 * @brief Perfectly forwards a variable.
 */
#define INTRO_FWD(T) std::forward<decltype(T)>(T)

/**
 * @brief Macro based on RANGES_DECLTYPE_AUTO_RETURN_NOEXCEPT to remove same
 * code repetition.
 * @see
 * https://github.com/ericniebler/range-v3/blob/master/include/range/v3/detail/config.hpp
 *
 * Example:
 *
 * @code
 *     auto func(int x) INTRO_RETURNS(calc(x))
 * @endcode
 *
 * Produces:
 *
 * @code
 *     auto func(int x) noexcept(noexcept(decltype(calc(x))(calc(x))))
 *                      -> decltype(calc(x))
 *     {
 *          return calc(x);
 *     }
 * @endcode
 */
#define INTRO_RETURNS(...)                                 \
    noexcept(noexcept(decltype(__VA_ARGS__)(__VA_ARGS__))) \
        ->decltype(__VA_ARGS__)                            \
    {                                                      \
        return (__VA_ARGS__);                              \
    }

}  // namespace intro
