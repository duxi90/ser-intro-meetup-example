#include <map>

#include <CLI/CLI.hpp>
#include <ctti/nameof.hpp>
#include <fmt/ostream.h>
#include <nlohmann/json.hpp>
#include <nonstd/expected.hpp>
#include <pistache/endpoint.h>
#include <pistache/router.h>

#include "intro/cli.hpp"
#include "intro/json.hpp"
#include "intro/metadata.hpp"

#include "examples/http/types.hpp"

using namespace Pistache;

struct LoggingMiddleware
{
    struct RequestLog
    {
        std::string url{};
        std::string body{};
        std::map<std::string, std::string> headers{};
        std::map<std::string, std::string> queryParams{};

        INTRO_GEN_METADATA(url, body, headers, queryParams)
    };

    bool operator()(const Http::Request &req, Http::ResponseWriter &response)
    {
        auto reqLog = RequestLog{
            .url = req.resource(),
            .body = req.body(),
            .queryParams =
                {
                    req.query().parameters_begin(),
                    req.query().parameters_end(),
                },
        };

        for (const auto &[name, header] : req.headers().rawList()) {
            reqLog.headers[header.name()] = header.value();
        }

        fmt::print("{}\n", reqLog);

        return true;
    }
};

template<typename Record>
struct CRUD
{
    static std::string defaultPrefix()
    {
        std::string prefix = ctti::nameof<Record>().cppstring() + "s";
        prefix[0] = tolower(prefix[0]);
        return prefix;
    }

    std::string prefix = defaultPrefix();
    std::map<int, Record> records;
    int next_id = 1;

    nonstd::expected<Record, std::exception_ptr> getRecord(const auto &req)
    {
        try {
            return nlohmann::json::parse(req.body()).template get<Record>();
        }
        catch (...) {
            return nonstd::make_unexpected(std::current_exception());
        }
    };

    auto getId(const auto &req)
    {
        return req.param(":id").template as<int>();
    }

    auto serializeRecord(int id)
    {
        nlohmann::json out(records[id]);
        out["id"] = id;
        return out.dump();
    }

    auto sendOk(auto &response, auto &&...args)
    {
        response.send(Pistache::Http::Code::Ok, args...);
        return Pistache::Rest::Route::Result::Ok;
    }

    auto sendBadRequest(auto &response, auto &&...args)
    {
        response.send(Pistache::Http::Code::Bad_Request, args...);
        return Pistache::Rest::Route::Result::Ok;
    }

    auto sendNotFound(auto &response, auto &&...args)
    {
        response.send(Pistache::Http::Code::Not_Found, args...);
        return Pistache::Rest::Route::Result::Ok;
    }

    void addToRouter(Pistache::Rest::Router &router)
    {
        router.post("/" + prefix, [&](const auto &req, auto response) {
            auto record = getRecord(req);
            if (!record) {
                return sendBadRequest(response);
            }
            auto id = next_id++;
            records[id] = *record;
            return sendOk(response, serializeRecord(id));
        });
        router.get("/" + prefix, [&](const auto &req, auto response) {
            return sendOk(response, nlohmann::json(records).dump());
        });
        router.get("/" + prefix + "/:id", [&](const auto &req, auto response) {
            auto id = getId(req);
            if (!records.contains(id)) {
                return sendNotFound(response);
            }
            return sendOk(response, serializeRecord(id));
        });
        router.put("/" + prefix + "/:id", [&](const auto &req, auto response) {
            auto record = getRecord(req);
            if (!record) {
                return sendBadRequest(response);
            }
            auto id = getId(req);
            records[id] = *record;
            return sendOk(response, serializeRecord(id));
        });
        router.del("/" + prefix + "/:id", [&](const auto &req, auto response) {
            auto id = getId(req);
            if (!records.contains(id)) {
                return sendNotFound(response);
            }
            records.erase(id);
            return sendOk(response);
        });
    }
};

struct Config
{
    std::size_t port = 8080;
    std::size_t numThreads = 1;

    INTRO_GEN_METADATA(port, numThreads)
};

int main(int argc, const char *argv[])
{
    ::CLI::App app{"HTTP Server"};
    auto config = intro::cli::parseArguments<Config>(app, argc, argv);

    fmt::print("{}\n\n", config);

    Pistache::Rest::Router router;
    router.addMiddleware(LoggingMiddleware{});

    CRUD<User> usersCRUD{};
    usersCRUD.addToRouter(router);
    CRUD<Location> locationCRUD{};
    locationCRUD.addToRouter(router);

    Http::Endpoint server(
        Pistache::Address(Pistache::Ipv4::any(), Pistache::Port(config.port)));
    server.init(Pistache::Http::Endpoint::options().threads(config.numThreads));
    server.setHandler(router.handler());

    server.serve();
}
