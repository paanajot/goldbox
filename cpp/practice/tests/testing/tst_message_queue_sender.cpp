#include <fcntl.h> // NOLINT
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <stddef.h>
#include <string>
#include <system_error>

#include "json_parser_mock.h"
#include "message_queue_mock.h"
#include "message_queue_sender.h"

namespace
{
const std::string q_name = "/howl";
const std::string file_name = "meow.json";
const std::string msg = "Hello World!";
constexpr mode_t permission = 0664;
} // namespace

using namespace ::testing;

struct MsgQueueFixture : Test
{
    NiceMock<MessageQueueMock> m_mock{};
};

TEST_F(MsgQueueFixture, constructor_test)
{
    EXPECT_CALL(m_mock, mq_open(StrEq(q_name), O_WRONLY | O_CREAT, permission, NULL))
        .WillOnce(Return(0));

    MessageQueueSender{q_name, file_name};
}

TEST_F(MsgQueueFixture, constructor_fail_test)
{
    EXPECT_CALL(m_mock, mq_open(StrEq(q_name), O_WRONLY | O_CREAT, permission, NULL))
        .WillOnce(Return(-1));

    EXPECT_THROW((MessageQueueSender{q_name, file_name}), std::system_error);
}

TEST_F(MsgQueueFixture, destructor_test)
{
    EXPECT_CALL(m_mock, mq_close(_)).WillOnce(Return(0));

    MessageQueueSender{q_name, file_name};
}

TEST_F(MsgQueueFixture, send_message_test)
{
    EXPECT_CALL(m_mock, mq_send(_, StrEq(msg), msg.size(), 0)).WillOnce(Return(0));

    MessageQueueSender sender{q_name, file_name};
    EXPECT_TRUE(sender.send_message(msg));
}

TEST_F(MsgQueueFixture, send_message_fail_test)
{
    EXPECT_CALL(m_mock, mq_send(_, StrEq(msg), msg.size(), 0)).WillOnce(Return(-1));

    MessageQueueSender sender{q_name, file_name};
    EXPECT_FALSE(sender.send_message(msg));
}

TEST_F(MsgQueueFixture, json_parser_constructor)
{
    auto& parser_mock = JsonParserMock::get_instance();
    EXPECT_CALL(parser_mock, ctor(StrEq(file_name))).Times(1);

    MessageQueueSender{q_name, file_name};
}

TEST_F(MsgQueueFixture, json_parser_parse_and_set_options)
{
    const std::string key = "first_message";
    const JsonParserMock::Mode mode = 1;

    auto& parser_mock = JsonParserMock::get_instance();
    EXPECT_CALL(parser_mock, ctor(StrEq(file_name))).Times(1);
    EXPECT_CALL(parser_mock, set_options(mode)).Times(1);
    EXPECT_CALL(parser_mock, parse(StrEq(key))).WillOnce(Return(msg));

    MessageQueueSender sender{q_name, file_name};
    EXPECT_EQ(sender.get_message(key, mode), msg);
}

TEST_F(MsgQueueFixture, is_valid_file_name)
{
    const std::string not_valid_file_name = "this file doesn't exist";

    auto& parser_mock = JsonParserMock::get_instance();
    {
        InSequence seq;
        EXPECT_CALL(parser_mock, is_valid(StrEq(file_name))).WillOnce(Return(true));
        EXPECT_CALL(parser_mock, is_valid(StrEq(not_valid_file_name))).WillOnce(Return(false));
    }

    MessageQueueSender sender{q_name, {}};
    EXPECT_TRUE(sender.is_valid_file_name(file_name));
    EXPECT_FALSE(sender.is_valid_file_name(not_valid_file_name));
}
