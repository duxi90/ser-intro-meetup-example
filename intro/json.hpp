#pragma once

#include <ostream>
#include <type_traits>

#include <nlohmann/json.hpp>

#include "intro/metadata.hpp"

namespace nlohmann {

/**
 * @brief Adds support for conversion from and to nlohmann::json for any type
 * which has MetadataFields defined.
 */
template<typename Obj>
struct adl_serializer<Obj, std::void_t<intro::metadata::Fields<Obj>>>
{
    static void to_json(::nlohmann::json &j, const Obj &obj)
    {
        intro::metadata::forEachField(
            obj, [&](auto &field, auto name) { j[name] = field; });
    }

    static void from_json(const nlohmann::json &j, Obj &obj)
    {
        intro::metadata::forEachField(
            obj, [&](auto &field, auto name) { j.at(name).get_to(field); });
    }
};

}  // namespace nlohmann

/**
 * @brief Adds support for ostream operator for any type which has
 * MetadataFields defined.
 */
template<typename T, intro::metadata::Fields<T> * = nullptr>
std::ostream &operator<<(std::ostream &out, const T &t)
{
    return out << nlohmann::json(t).dump();
}
