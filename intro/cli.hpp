#include <CLI/CLI.hpp>
#include <fmt/ostream.h>

#include "intro/metadata.hpp"

namespace intro::cli {

/**
 * @brief Reimplementation of CLI11_PARSE macro as a function.
 */
inline void parse(::CLI::App &app, int argc, const char *argv[])
{
    try {
        app.parse(argc, argv);
    }
    catch (const ::CLI::Success &e) {
        fmt::print(
            "CLI requested exit by throwing success of name {}", e.get_name());
        std::exit(app.exit(e));
    }
    catch (const ::CLI::ParseError &e) {
        fmt::print(
            "Bad command line arguments. Error is {}. Exiting!", e.get_name());
        std::exit(app.exit(e));
    }
}

void addCliArgs(::CLI::App &app, auto &obj, std::string name = "")
{
    app.add_option(name == "" ? "value" : ("--" + name), obj);
}

/**
 * @brief Add support for adding cli options for any type which has
 * MetadataFields defined.
 */
template<intro::metadata::HasMetadata Obj>
void addCliArgs(::CLI::App &app, Obj &obj, std::string name = "")
{
    auto add_arg = [&](auto &field, auto fieldName) {
        auto separator = name == "" ? "" : ".";
        addCliArgs(app, field, name + separator + fieldName);
    };
    intro::metadata::forEachField(obj, add_arg);
}

/**
 * @brief Parses CLI arguments specified by the provided type.
 */
template<typename CliArguments>
auto parseArguments(::CLI::App &app, int argc, const char *argv[])
{
    CliArguments cliArgs{};

    addCliArgs(app, cliArgs);

    parse(app, argc, argv);

    return cliArgs;
}

}  // namespace intro::cli
