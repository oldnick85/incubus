#ifndef COLOR_H
#define COLOR_H

#include <memory>
#include "real_number.h"

class CColor
{
public:
    CColor() = default;
    ~CColor() = default;

    void SetRGBA(const Real_t red, const Real_t green, const Real_t blue, const Real_t alpha = 1.0)
    {
        m_red = red;
        m_green = green;
        m_blue = blue;
        m_alpha = alpha;
    }

    void SetAlpha(const Real_t alpha)
    {
        m_alpha = alpha;
    }

protected:
private:
    Real_t  m_red = 0.0;
    Real_t  m_green = 0.0;
    Real_t  m_blue = 0.0;
    Real_t  m_alpha = 1.0;
};
using CColorPtr = std::shared_ptr<CColor>;

#endif // COLOR_H
