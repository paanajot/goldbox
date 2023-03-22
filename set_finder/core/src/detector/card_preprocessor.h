#pragma once

#include <vector>

#include "common/types.h"

class CardPreprocessor
{
public:
    void run(Image card);

    Image get_input_card() const;
    Image get_filtered_card() const;
    Contours get_contours() const;

private:
    Image remove_noises(Image card);
    Contours generate_contours(Image card);

    Image m_input_card{};
    Image m_filtered_card{};
    Contours m_contours{};
};
