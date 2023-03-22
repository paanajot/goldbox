
#pragma once

#include <optional>
#include <vector>

#include "common/types.h"
#include "core/engine_supervisor.h"

class SetEngine
{
public:
    Image run(const Image& source_img);
    void run(uint8_t* image_buffer, int width, int height);

private:
    void optimize_filter(const Image& img);
    void generate_set(const Image& img);
    Image update_set(const Image& img);
    void draw_limit_lines(const Image& frame, int width, int height);

    std::optional<Boundaries> m_boundaries{};
    std::vector<Centroid> m_set{};
    size_t m_number_of_cards{};

    EngineSupervisor m_supervisor{};
};
