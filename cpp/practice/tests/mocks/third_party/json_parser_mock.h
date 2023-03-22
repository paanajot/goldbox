#pragma once

#include <gmock/gmock.h>
#include <mqueue.h>
#include <sys/types.h>

struct JsonParserMock
{
    using Mode = uint32_t;
    using Value = std::string;

    MOCK_METHOD(void, ctor, (const std::string& file_name));
    MOCK_METHOD(Value, parse, (const std::string& key));
    MOCK_METHOD(void, set_options, (JsonParserMock::Mode mode));
    MOCK_METHOD(bool, is_valid, (const std::string& file_name));

    JsonParserMock(const JsonParserMock&) = delete;
    void operator=(const JsonParserMock&) = delete;

    static JsonParserMock& get_instance()
    {
        static ::testing::NiceMock<JsonParserMock> instance;
        return instance;
    }

private:
    JsonParserMock() = default;
    friend ::testing::NiceMock<JsonParserMock>;
};
