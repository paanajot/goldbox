#pragma once

#include <gmock/gmock.h>
#include <mqueue.h>
#include <sys/types.h>

struct MessageQueueMock
{
    MessageQueueMock();
    ~MessageQueueMock();

    MOCK_METHOD(mqd_t, mq_open, (const char* name, int oflag, mode_t mode, struct mq_attr* attr));
    MOCK_METHOD(int,
                mq_send,
                (mqd_t mqdes, const char* msg_ptr, size_t msg_len, unsigned int msg_prio));
    MOCK_METHOD(int, mq_close, (mqd_t mqdes));
};
