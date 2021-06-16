#ifndef REAL_NUMBER_H
#define REAL_NUMBER_H

#include <cmath>

class Real_t
{
public:
    Real_t() = default;
    Real_t(const double val) {m_value = val;}
    Real_t(const int val) {m_value = static_cast<double>(val);}
    Real_t(const uint val) {m_value = static_cast<double>(val);}

    double Val() const
    {
        return m_value;
    }

    double& Val()
    {
        return m_value;
    }

    operator double()
    {
        return m_value;
    }

    operator int()
    {
        return static_cast<int>(m_value);
    }

    Real_t& operator=(const double &v)
    {
        m_value = v;
        return *this;
    }

    Real_t& operator+=(const Real_t& rv)
    {
        m_value += rv.m_value;
        return *this;
    }

    Real_t operator+(const Real_t& rhs) const
    {
        return Real_t(*this) += rhs;
    }

    Real_t& operator-=(const Real_t& rv)
    {
        m_value -= rv.m_value;
        return *this;
    }

    Real_t operator-(const Real_t& rhs) const
    {
        return Real_t(*this) -= rhs;
    }

    Real_t operator-() const
    {
        return Real_t(-m_value);
    }

    Real_t& operator*=(const Real_t& rv)
    {
        m_value *= rv.m_value;
        return *this;
    }

    Real_t operator*(const Real_t& rhs) const
    {
        return Real_t(*this) *= rhs;
    }

    Real_t& operator*=(const double rv)
    {
        m_value *= rv;
        return *this;
    }

    Real_t operator*(const double rhs) const
    {
        return Real_t(*this) *= rhs;
    }

    Real_t& operator/=(const Real_t& rhs)
    {
        m_value /= rhs.m_value;
        return *this;
    }

    Real_t operator/(const Real_t& rhs) const
    {
        return Real_t(*this) /= rhs;
    }

    Real_t& operator/=(const double& rhs)
    {
        m_value /= rhs;
        return *this;
    }

    Real_t operator/(const double& rhs) const
    {
        return Real_t(*this) /= rhs;
    }

    bool operator==(const Real_t &rhs) const
    {
        return (fabs(m_value - rhs.m_value) < 1E-9);
    }

    bool operator!=(const Real_t &rhs) const
    {
        return (not (*this == rhs));
    }

    bool operator==(const double rhs) const
    {
        return (fabs(m_value - rhs) < 1E-9);
    }

    bool operator<(const Real_t& rhs) const
    {
        return (m_value < rhs.m_value);
    }

    bool operator>(const Real_t& rhs) const
    {
        return (m_value > rhs.m_value);
    }

    bool operator<(const double& rhs) const
    {
        return (m_value < rhs);
    }

    bool operator>(const double& rhs) const
    {
        return (m_value > rhs);
    }

    bool operator<(const float& rhs) const
    {
        return (static_cast<float>(m_value) < rhs);
    }

    bool operator>(const float& rhs) const
    {
        return (static_cast<float>(m_value) > rhs);
    }

    bool operator<=(const Real_t& rhs) const
    {
        return ((*this < rhs) or (*this == rhs));
    }

    bool operator<=(const double rhs) const
    {
        return ((*this < rhs) or (*this == rhs));
    }

    bool operator>=(const Real_t& rhs) const
    {
        return ((*this > rhs) or (*this == rhs));
    }

    bool operator>=(const double rhs) const
    {
        return ((*this > rhs) or (*this == rhs));
    }

    bool IsNul() const
    {
        return (fabs(m_value) < 1E-9);
    }

private:
    double  m_value = 0.0;
};

#endif // REAL_NUMBER_H
