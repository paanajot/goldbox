
#include "core/engine_supervisor.h"

EngineSupervisor::EngineSupervisor()
    : m_engine_task(EngineTask::OPTIMIZE)
{ }

EngineTask EngineSupervisor::fetch_task()
{
    return m_engine_task.exchange(EngineTask::NONE);
}

EngineTask EngineSupervisor::get_current_task() const
{
    return m_current_task;
}

void EngineSupervisor::set_task(EngineTask task)
{
    m_engine_task = task;
    m_current_task = task;
}

bool EngineSupervisor::is_none() const
{
    return m_engine_task == EngineTask::NONE;
}
