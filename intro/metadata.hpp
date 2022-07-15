#pragma once

#include <string>
#include <tuple>
#include <type_traits>

#include <experimental/type_traits>

#include "intro/macros.hpp"

namespace intro::metadata {

namespace detail {

#define INTRO_FIELD_METADATA(FIELD) INTRO_CAT(FIELD, _IntroFieldMeta)

/**
 * @brief Generated metadata for the specified field.
 */
#define INTRO_GEN_FIELD_METADATA(FIELD)                 \
    struct INTRO_FIELD_METADATA(FIELD)                  \
    {                                                   \
        static std::string name()                       \
        {                                               \
            return INTRO_STR(FIELD);                    \
        }                                               \
                                                        \
        template<typename T>                            \
        static constexpr decltype(auto) getField(T &&t) \
        {                                               \
            return (std::forward<T>(t).FIELD);          \
        }                                               \
    };

#define INTRO_FIELD_METADATA_COMMA(FIELD) INTRO_FIELD_METADATA(FIELD){},

}  // namespace detail

/**
 * @brief Generated metadata for the specified fields.
 *
 * Usage:
 *
 *     class Car
 *     {
 *         int numberOfGears;
 *
 *         INTRO_GEN_METADATA(numberOfGears)
 *     };
 */
#define INTRO_GEN_METADATA(...)                        \
    INTRO_MAP(INTRO_GEN_FIELD_METADATA, ##__VA_ARGS__) \
    using MetadataFields = decltype(                   \
        std::tuple{INTRO_MAP(INTRO_FIELD_METADATA_COMMA, ##__VA_ARGS__)});

/**
 * @brief Metadata fields of the specified object.
 *
 * @tparam Obj The object for which we want to get meta field.
 */
template<typename Obj>
using Fields = typename Obj::MetadataFields;

template<typename Obj>
concept HasMetadata = requires
{
    typename intro::metadata::Fields<Obj>;
};

/**
 * @brief This function creates a compiletime for each loop
 *        over object field metadata.
 *
 * @tparam Obj The type of object for which to loop over fields metadata.
 * @tparam F The type of the function.
 * @param func The functions executed for every field metadata.
 */
template<typename Obj, typename F>
constexpr void forEachField(F &&func)
{
    auto f = [&func](auto &&...metadatas) { (func(metadatas), ...); };
    std::apply(f, intro::metadata::Fields<Obj>{});
}

template<typename Obj, typename F>
constexpr void forEachField(Obj &&obj, F &&func)
{
    auto f = [&]<typename Metadata>(Metadata) {
        func(Metadata::getField(obj), Metadata::name());
    };
    intro::metadata::forEachField<std::remove_cvref_t<Obj>>(f);
}

}  // namespace intro::metadata
