#pragma once

struct PersonalDetails
{
    std::string name;
    int age;

    INTRO_GEN_METADATA(name, age)
};

struct User
{
    std::string username;
    PersonalDetails details;

    INTRO_GEN_METADATA(username, details)
};

struct Location
{
    std::string name;
    int lat;
    int lon;

    INTRO_GEN_METADATA(name, lat, lon)
};
