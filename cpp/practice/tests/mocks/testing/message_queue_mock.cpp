
#include <cassert>

#include "message_queue_mock.h"

namespace
{
MessageQueueMock* mock = nullptr;
}

MessageQueueMock::MessageQueueMock()
{
    mock = this;
}

MessageQueueMock::~MessageQueueMock()
{
    mock = nullptr;
}

// NOTE: compilation error occurs when it's not compiled in C.
// C++ mangles the names, so it doesn't match with expected ones.
// This is one way to mock system functions.
extern "C"
{
    // NOLINTBEGIN
    mqd_t __wrap_mq_open(const char* name, int oflag, mode_t mode, struct mq_attr* attr)
    {
        assert(mock);
        return mock->mq_open(name, oflag, mode, attr);
    }

    int __wrap_mq_send(mqd_t mqdes, const char* msg_ptr, size_t msg_len, unsigned int msg_prio)
    {
        assert(mock);
        return mock->mq_send(mqdes, msg_ptr, msg_len, msg_prio);
    }

    ssize_t __wrap_mq_close(mqd_t mqdes)
    {
        assert(mock);
        return mock->mq_close(mqdes);
    }
    // NOLINTEND
}
