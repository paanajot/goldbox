
#pragma once

#include "rectangle.h"
#include "square.h"

// NOTE: Converts one interface to another so that it matches what the client is expecting.
struct SquareToRectangleAdapter : Rectangle
{
    explicit SquareToRectangleAdapter(const Square& square)
        : m_sq(square)
    { }

    virtual int width() const
    {
        return m_sq.m_side;
    }
    virtual int height() const
    {
        return m_sq.m_side;
    }

private:
    Square m_sq;
};
