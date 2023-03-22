#pragma once

#include <cstdint>
#include <string>

// Just dummy JsonParser for testing practice
class JsonParser
{
public:
    using Mode = uint32_t;
    using Value = std::string;

    explicit JsonParser(const std::string& file_name);
    Value parse(const std::string& key) const;
    void set_options(Mode mode);
};

bool is_valid(const std::string& file_name);
