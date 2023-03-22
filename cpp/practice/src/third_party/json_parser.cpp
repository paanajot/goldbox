#include <string>

#include "json_parser.h"

JsonParser::JsonParser([[maybe_unused]] const std::string& file_name) { }

JsonParser::Value JsonParser::parse([[maybe_unused]] const std::string& key) const
{
    return {};
}

void JsonParser::set_options([[maybe_unused]] Mode mode) { }

bool is_valid([[maybe_unused]] const std::string& file_name)
{
    return {};
}
