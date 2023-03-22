
#pragma once

#include <atomic>

enum class EngineTask
{
    NONE,
    OPTIMIZE,
    GENERATE_SET,
    UPDATE_CONTOURS
};

class EngineSupervisor
{
public:
    EngineSupervisor();

    EngineTask fetch_task();
    void set_task(EngineTask task);
    bool is_none() const;

private:
    std::atomic<EngineTask> m_engine_task{};
};
