#pragma once

#include <yaml-cpp/yaml.h>

#include "intro/core.hpp"
#include "intro/metadata.hpp"

namespace YAML {

template<typename T>
struct convert  // NOLINT: This is a CPO in YAML and has to be named like this.
{
    static Node encode(const T &t)
    {
        Node node;
        to_yaml(node, t);
        return node;
    }

    static bool decode(const Node &node, T &t)
    {
        from_yaml(node, t);
        return true;
    }
};

template<typename Field>
auto get_to(const Node &node, Field &field)
{
    field = node.template as<Field>();
}

/**
 * @brief Adds support for conversion to yaml for any type which has
 * MetadataFields defined.
 */
template<typename Obj, intro::metadata::Fields<Obj> * = nullptr>
void to_yaml(Node &node, const Obj &obj)
{
    intro::metadata::forEachField(
        obj, [&](auto &field, auto name) { node[name] = field; });
}

/**
 * @brief Adds support for conversion from yaml for any type which has
 * MetadataFields defined.
 */
template<typename Obj, intro::metadata::Fields<Obj> * = nullptr>
void from_yaml(const Node &node, Obj &obj)
{
    intro::metadata::forEachField(
        obj, [&](auto &field, auto name) { get_to(node[name], field); });
}

}  // namespace YAML
