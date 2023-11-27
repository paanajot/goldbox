#pragma once

#include <platforminterface/allocator.h>
#include <qul/model.h>

#include <string>
#include <vector>

struct MealPlanData
{
    std::string type{};
    std::string name{};
    std::string sourceImg{};
    int cookingTime{};
    int healthScore{};
};

inline bool operator==(const MealPlanData& lhs, const MealPlanData& rhs)
{
    return lhs.type == rhs.type && lhs.name == rhs.name;
}

struct MealPlanListModel : Qul::ListModel<MealPlanData>
{
public:
    MealPlanListModel()
    {
        m_data.emplace_back(
            MealPlanData{"Snack", "Chicken Salad", "assets/onMenuImagePlaceholder.png", 10, 40});
        m_data.emplace_back(
            MealPlanData{"Dinner", "Fish Plate", "assets/onMenuImagePlaceholder.png", 30, 64});
        m_data.emplace_back(
            MealPlanData{"Breakfast", "Meat Plate", "assets/onMenuImagePlaceholder.png", 20, 11});
    }

    int count() const QUL_DECL_OVERRIDE
    {
        return m_data.size();
    }

    MealPlanData data(int index) const QUL_DECL_OVERRIDE
    {
        return m_data[index];
    }

private:
    std::vector<MealPlanData, Qul::PlatformInterface::Allocator<MealPlanData>> m_data;
};
