#include <map>
#include <string>

#include <CLI/CLI.hpp>
#include <fmt/ostream.h>
#include <yaml-cpp/yaml.h>

#include "intro/cli.hpp"
#include "intro/json.hpp"
#include "intro/metadata.hpp"
#include "intro/yaml.hpp"

#include "examples/http/types.hpp"

template<template<typename...> typename W, typename T>
static constexpr bool isTemplateInstantiationOf = false;

template<template<typename...> typename W, typename... T>
static constexpr bool isTemplateInstantiationOf<W, W<T...>> = true;

template<typename Obj>
struct CrudMap
{
    int id;
    Obj map;

    INTRO_GEN_METADATA(id, map)
};

template<typename State, typename PrefixFn>
auto add_crud_arguments(::CLI::App &app, State &state, PrefixFn urlPrefix)
{
    app.add_subcommand("get")->callback(
        [urlPrefix] { fmt::print("curl -X GET {}", urlPrefix()); });

    if constexpr (isTemplateInstantiationOf<CrudMap, State>) {
        auto del = app.add_subcommand("del");
        del->add_option("--id", state.id)->required();
        del->callback([urlPrefix, &state] {
            fmt::print("curl -X DELETE {}/{}'\n", urlPrefix(), state.id);
        });

        auto post = app.add_subcommand("post");
        intro::cli::addCliArgs(*post, state.map);
        post->callback([urlPrefix, &state] {
            fmt::print(
                "curl -X POST {} -H 'Content-Type: application/json' -d '{}'\n",
                urlPrefix(),
                state.map);
        });

        auto at = app.add_subcommand("at");
        at->add_option("--id", state.id)->required();
        add_crud_arguments(*at, state.map, [urlPrefix, &state] {
            return fmt::format("{}/{}", urlPrefix(), state.id);
        });
    }
    else {
        auto put = app.add_subcommand("put");
        intro::cli::addCliArgs(*put, state);
        put->callback([urlPrefix, &state] {
            fmt::print(
                "curl -X PUT {} -H 'Content-Type: application/json' -d '{}'\n",
                urlPrefix(),
                state);
        });

        if constexpr (intro::metadata::HasMetadata<State>) {
            intro::metadata::forEachField(state, [&](auto &field, auto name) {
                auto fieldSubApp = app.add_subcommand(name);
                add_crud_arguments(*fieldSubApp, field, [urlPrefix, name] {
                    return urlPrefix() + "/" + name;
                });
            });
        }
    }
}

struct CrudState
{
    CrudMap<User> users;
    CrudMap<Location> locations;

    INTRO_GEN_METADATA(users, locations)
};

int main(int argc, const char *argv[])
{
    ::CLI::App app{"CRUD CLI"};
    std::string addr = "localhost:8080/";
    app.add_option("--addr", addr);

    CrudState crudState{};

    add_crud_arguments(app, crudState, [&] { return addr; });

    intro::cli::parse(app, argc, argv);

    return 0;
}
