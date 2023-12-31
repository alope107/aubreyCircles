/*
 * Copyright (c) 2020-2023 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef CIRCLES_FIXED_POINT_H
#define CIRCLES_FIXED_POINT_H

/**
 * @file
 * bn::fixed_point_t<Precision> header file.
 *
 * @ingroup math
 */

#include "bn_fixed.h"
#include "bn_point.h"

namespace circles
{

/**
 * @brief Defines a two-dimensional point using bn::fixed_t<Precision> point precision.
 *
 * @ingroup math
 */
template<int Precision>
class fixed_point_t
{

public:
    /**
     * @brief Default constructor.
     */
    constexpr fixed_point_t() = default;

    /**
     * @brief Constructor.
     * @param x Horizontal coordinate.
     * @param y Vertical coordinate.
     */
    constexpr fixed_point_t(bn::fixed_t<Precision> x, bn::fixed_t<Precision> y) :
        _x(x),
        _y(y)
    {
    }

    /**
     * @brief Constructor.
     * @param point Integer point.
     */
    constexpr fixed_point_t(const bn::point& point) :
        _x(point.x()),
        _y(point.y())
    {
    }

    constexpr fixed_point_t(const bn::fixed_point point) :
        _x(point.x()),
        _y(point.y())
    {
    }

    /**
     * @brief Returns the horizontal coordinate.
     */
    [[nodiscard]] constexpr bn::fixed_t<Precision> x() const
    {
        return _x;
    }

    /**
     * @brief Sets the horizontal coordinate.
     */
    constexpr void set_x(bn::fixed_t<Precision> x)
    {
        _x = x;
    }

    /**
     * @brief Returns the vertical coordinate.
     */
    [[nodiscard]] constexpr bn::fixed_t<Precision> y() const
    {
        return _y;
    }

    /**
     * @brief Sets the vertical coordinate.
     */
    constexpr void set_y(bn::fixed_t<Precision> y)
    {
        _y = y;
    }

    /**
     * @brief Returns the multiplication of this point by the given integer value.
     */
    [[nodiscard]] constexpr fixed_point_t<Precision> multiplication(int value) const
    {
        return fixed_point_t(_x.multiplication(value), _y.multiplication(value));
    }

    /**
     * @brief Returns the multiplication of this point by the given bn::fixed_t<Precision> point value,
     * using half precision to try to avoid overflow.
     */
    [[nodiscard]] constexpr fixed_point_t<Precision> multiplication(bn::fixed_t<Precision> value) const
    {
        return fixed_point_t(_x.multiplication(value), _y.multiplication(value));
    }

    /**
     * @brief Returns the multiplication of this point by the given integer value.
     */
    [[nodiscard]] constexpr fixed_point_t<Precision> safe_multiplication(int value) const
    {
        return fixed_point_t(_x.safe_multiplication(value), _y.safe_multiplication(value));
    }

    /**
     * @brief Returns the multiplication of this point by the given bn::fixed_t<Precision> point value,
     * casting them to int64_t to try to avoid overflow.
     */
    [[nodiscard]] constexpr fixed_point_t<Precision> safe_multiplication(bn::fixed_t<Precision> value) const
    {
        return fixed_point_t(_x.safe_multiplication(value), _y.safe_multiplication(value));
    }

    /**
     * @brief Returns the multiplication of this point by the given integer value.
     */
    [[nodiscard]] constexpr fixed_point_t<Precision> unsafe_multiplication(int value) const
    {
        return fixed_point_t<Precision>(_x.unsafe_multiplication(value), _y.unsafe_multiplication(value));
    }

    /**
     * @brief Returns the multiplication of this point by the given bn::fixed_t<Precision> point value
     * without trying to avoid overflow.
     */
    [[nodiscard]] constexpr fixed_point_t<Precision> unsafe_multiplication(bn::fixed_t<Precision> value) const
    {
        return fixed_point_t<Precision>(_x.unsafe_multiplication(value), _y.unsafe_multiplication(value));
    }

    /**
     * @brief Returns the division of this point by the given integer value.
     */
    [[nodiscard]] constexpr fixed_point_t<Precision> division(int value) const
    {
        return fixed_point_t<Precision>(_x.division(value), _y.division(value));
    }

    /**
     * @brief Returns the division of this point by the given bn::fixed_t<Precision> point value,
     * using half precision to try to avoid overflow.
     */
    [[nodiscard]] constexpr fixed_point_t<Precision> division(bn::fixed_t<Precision> value) const
    {
        return fixed_point_t<Precision>(_x.division(value), _y.division(value));
    }

    /**
     * @brief Returns the division of this value by the given integer value.
     */
    [[nodiscard]] constexpr fixed_point_t<Precision> safe_division(int value) const
    {
        return fixed_point_t<Precision>(_x.safe_division(value), _y.safe_division(value));
    }

    /**
     * @brief Returns the division of this point by the given bn::fixed_t<Precision> point value,
     * casting them to int64_t to try to avoid overflow.
     */
    [[nodiscard]] constexpr fixed_point_t<Precision> safe_division(bn::fixed_t<Precision> value) const
    {
        return fixed_point_t<Precision>(_x.safe_division(value), _y.safe_division(value));
    }

    /**
     * @brief Returns the division of this value by the given integer value.
     */
    [[nodiscard]] constexpr fixed_point_t<Precision> unsafe_division(int value) const
    {
        return fixed_point_t<Precision>(_x.unsafe_division(value), _y.unsafe_division(value));
    }

    /**
     * @brief Returns the division of this point by the given bn::fixed_t<Precision> point value
     * without trying to avoid overflow.
     */
    [[nodiscard]] constexpr fixed_point_t<Precision> unsafe_division(bn::fixed_t<Precision> value) const
    {
        return fixed_point_t<Precision>(_x.unsafe_division(value), _y.unsafe_division(value));
    }

    /**
     * @brief Returns a fixed_point_t<Precision> that is formed by changing the sign of both coordinates.
     */
    [[nodiscard]] constexpr fixed_point_t<Precision> operator-() const
    {
        return fixed_point_t<Precision>(-_x, -_y);
    }

    /**
     * @brief Adds the given fixed_point_t<Precision> to this one.
     * @param other fixed_point_t<Precision> to add.
     * @return Reference to this.
     */
    constexpr fixed_point_t<Precision>& operator+=(const fixed_point_t<Precision>& other)
    {
        _x += other._x;
        _y += other._y;
        return *this;
    }

    /**
     * @brief Subtracts the given fixed_point_t<Precision> to this one.
     * @param other fixed_point_t<Precision> to subtract.
     * @return Reference to this.
     */
    constexpr fixed_point_t<Precision>& operator-=(const fixed_point_t<Precision>& other)
    {
        _x -= other._x;
        _y -= other._y;
        return *this;
    }

    /**
     * @brief Multiplies both coordinates by the given factor.
     * @param value Integer multiplication factor.
     * @return Reference to this.
     */
    constexpr fixed_point_t<Precision>& operator*=(int value)
    {
        _x *= value;
        _y *= value;
        return *this;
    }

    /**
     * @brief Multiplies both coordinates by the given factor.
     * @param value bn::fixed_t<Precision> point multiplication factor.
     * @return Reference to this.
     */
    constexpr fixed_point_t<Precision>& operator*=(bn::fixed_t<Precision> value)
    {
        _x *= value;
        _y *= value;
        return *this;
    }

    /**
     * @brief Divides both coordinates by the given divisor.
     * @param value Valid integer divisor (!= 0).
     * @return Reference to this.
     */
    constexpr fixed_point_t<Precision>& operator/=(int value)
    {
        _x /= value;
        _y /= value;
        return *this;
    }

    /**
     * @brief Divides both coordinates by the given divisor.
     * @param value Valid bn::fixed_t<Precision> point divisor (!= 0).
     * @return Reference to this.
     */
    constexpr fixed_point_t<Precision>& operator/=(bn::fixed_t<Precision> value)
    {
        _x /= value;
        _y /= value;
        return *this;
    }

    /**
     * @brief Returns the sum of a and b.
     */
    [[nodiscard]] constexpr friend fixed_point_t<Precision> operator+(const fixed_point_t<Precision>& a, const fixed_point_t<Precision>& b)
    {
        return fixed_point_t<Precision>(a._x + b._x, a._y + b._y);
    }

    /**
     * @brief Returns b subtracted from a.
     */
    [[nodiscard]] constexpr friend fixed_point_t<Precision> operator-(const fixed_point_t<Precision>& a, const fixed_point_t<Precision>& b)
    {
        return fixed_point_t<Precision>(a._x - b._x, a._y - b._y);
    }

    /**
     * @brief Returns a multiplied by b.
     */
    [[nodiscard]] constexpr friend fixed_point_t<Precision> operator*(const fixed_point_t<Precision>& a, int b)
    {
        return fixed_point_t<Precision>(a._x * b, a._y * b);
    }

    /**
     * @brief Returns a multiplied by b.
     */
    [[nodiscard]] constexpr friend fixed_point_t<Precision> operator*(const fixed_point_t<Precision>& a, bn::fixed_t<Precision> b)
    {
        return fixed_point_t<Precision>(a._x * b, a._y * b);
    }

    /**
     * @brief Returns a divided by b.
     */
    [[nodiscard]] constexpr friend fixed_point_t<Precision> operator/(const fixed_point_t<Precision>& a, int b)
    {
        return fixed_point_t<Precision>(a._x / b, a._y / b);
    }

    /**
     * @brief Returns a divided by b.
     */
    [[nodiscard]] constexpr friend fixed_point_t<Precision> operator/(const fixed_point_t<Precision>& a, bn::fixed_t<Precision> b)
    {
        return fixed_point_t<Precision>(a._x / b, a._y / b);
    }

    /**
     * @brief Default equal operator.
     */
    [[nodiscard]] constexpr friend bool operator==(const fixed_point_t<Precision>& a, const fixed_point_t<Precision>& b) = default;


    operator bn::fixed_point() const {
        return bn::fixed_point(_x, _y);
    }
private:
    bn::fixed_t<Precision> _x = 0;
    bn::fixed_t<Precision> _y = 0;
};

// TODO(auberon): Enable?
// /**
//  * @brief Hash support for fixed_point.
//  *
//  * @ingroup math
//  * @ingroup functional
//  */
// template<>
// struct hash<fixed_point>
// {
//     /**
//      * @brief Returns the hash of the given fixed_point.
//      */
//     [[nodiscard]] constexpr unsigned operator()(const fixed_point_t& value) const
//     {
//         unsigned result = make_hash(value.x());
//         hash_combine(value.y(), result);
//         return result;
//     }
// };

}

#endif
