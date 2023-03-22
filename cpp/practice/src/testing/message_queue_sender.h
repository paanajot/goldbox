#pragma once

#include <cstdint>
#include <mqueue.h>
#include <string>
#include <sys/stat.h>

#include "json_parser.h"

// NOTE: dummy class to show how to mock system functions and members that are not injected
class MessageQueueSender
{
public:
    explicit MessageQueueSender(const std::string& q_name, const std::string& file_name)
        : m_parser(file_name)
    {
        const mode_t permission = 0664;
        m_descriptor = mq_open(q_name.c_str(), O_WRONLY | O_CREAT, permission, NULL);
        if(m_descriptor == -1)
        {
            throw std::system_error(errno, std::system_category());
        }
    }

    bool send_message(const std::string& msg) const
    {
        constexpr uint32_t msg_priority = 0;
        const auto send_result = mq_send(m_descriptor, msg.c_str(), msg.size(), msg_priority);
        return send_result == 0;
    }

    std::string get_message(const std::string& key, JsonParser::Mode mode = 0)
    {
        m_parser.set_options(mode);
        return m_parser.parse(key);
    }

    bool is_valid_file_name(const std::string& file_name)
    {
        return is_valid(file_name);
    }

    ~MessageQueueSender()
    {
        mq_close(m_descriptor);
    }

private:
    mqd_t m_descriptor{};

    // NOTE: To mock it, JsonParser could be injected by a template argument, however this introduce additional
    // code / complexity to the 'production' code. Injecting object by constructor would not work here, since
    // the class doesn't consist of virtual methods. In tst_message_queue_sender.cpp I show how to test it
    // without additional code.
    JsonParser m_parser;
};
