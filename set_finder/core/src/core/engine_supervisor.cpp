
#include "core/engine_supervisor.h"

EngineSupervisor::EngineSupervisor()
    : m_engine_task(EngineTask::OPTIMIZE)
{ }

EngineTask EngineSupervisor::fetch_task()
{
    return m_engine_task.exchange(EngineTask::NONE);
}

void EngineSupervisor::set_task(EngineTask task)
{
    m_engine_task = task;
}

bool EngineSupervisor::is_none() const
{
    return m_engine_task == EngineTask::NONE;
}
