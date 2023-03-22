#pragma once

#include "json_parser_mock.h"

struct JsonParser
{
    using Mode = JsonParserMock::Mode;
    using Value = JsonParserMock::Value;

    explicit JsonParser(const std::string& file_name)
    {
        JsonParserMock::get_instance().ctor(file_name);
    }

    Value parse(const std::string& key) const
    {
        return JsonParserMock::get_instance().parse(key);
    }

    void set_options(Mode mode)
    {
        JsonParserMock::get_instance().set_options(mode);
    }
};

// NOTE: it could be mocked in the same way as JsonParser class,
// however I want to show that --wrap option also works with functions compiled in c++
extern bool is_valid([[maybe_unused]] const std::string&);

// NOLINTBEGIN
extern "C"
{
    // The following line generates output from which we can read is_valid mangled name.
    // echo -e "#include <string>\n bool is_valid(const std::string&){} " | /usr/bin/g++-10 -x c++ -S - -o-
    bool __wrap__Z8is_validRKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEE(
        const std::string& file_name)
    {
        return JsonParserMock::get_instance().is_valid(file_name);
    }
}
// NOLINTEND
