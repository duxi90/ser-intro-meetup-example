#include <string>

#include <CLI/CLI.hpp>
#include <fmt/ostream.h>
#include <yaml-cpp/yaml.h>

#include "intro/cli.hpp"
#include "intro/json.hpp"
#include "intro/metadata.hpp"
#include "intro/yaml.hpp"

struct Config
{
    std::string name;
    int setting;

    INTRO_GEN_METADATA(name, setting)
};

struct CliArgs
{
    std::string path;

    INTRO_GEN_METADATA(path);
};

int main(int argc, const char *argv[])
{
    ::CLI::App app{"YAML to JSON example"};

    auto [path] = intro::cli::parseArguments<CliArgs>(app, argc, argv);

    Config c = YAML::LoadFile(path).as<Config>();

    fmt::print("{}\n", c);

    return 0;
}
