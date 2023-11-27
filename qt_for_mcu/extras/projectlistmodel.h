#pragma once

#include <platforminterface/allocator.h>
#include <qul/model.h>

#include <string>
#include <vector>

struct ProjectListData
{
    std::string projectName{""};
    uint32_t status{0};
};

inline bool operator==(const ProjectListData& lhs, const ProjectListData& rhs)
{
    return lhs.projectName == rhs.projectName && lhs.status == rhs.status;
}

struct ProjectListModel : Qul::ListModel<ProjectListData>
{
public:
    ProjectListModel()
    {
        m_data.emplace_back(ProjectListData{"project_1", 1});
        m_data.emplace_back(ProjectListData{"project_2", 2});
        m_data.emplace_back(ProjectListData{"project_3", 3});
        m_data.emplace_back(ProjectListData{"project_4", 3});
        m_data.emplace_back(ProjectListData{"project_5", 3});
        m_data.emplace_back(ProjectListData{"project_6", 3});
        m_data.emplace_back(ProjectListData{"project_7", 3});
    }

    int count() const QUL_DECL_OVERRIDE
    {
        return m_data.size();
    }

    ProjectListData data(int index) const QUL_DECL_OVERRIDE
    {
        return m_data[index];
    }

private:
    std::vector<ProjectListData, Qul::PlatformInterface::Allocator<ProjectListData>> m_data;
};
