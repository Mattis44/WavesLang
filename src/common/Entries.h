#pragma once
#include <string>
#include <vector>

struct ImportEntry {
    std::string name;
    std::string alias;
    std::string path;
};

struct ParamEntry {
    std::string name;
    std::string value;
};
