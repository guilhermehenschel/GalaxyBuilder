#ifndef GGH_MODULES_GALAXYCORE_UTILITIES_COORDINATES_H
#define GGH_MODULES_GALAXYCORE_UTILITIES_COORDINATES_H

/**
 * @file Coordinates.h
 * @brief Contains utility functions for coordinate transformations and calculations.
 *
 * This file provides functions to convert between different coordinate systems,
 * calculate distances, and perform other geometric operations.
 */

#include <QPointF>
#include <cmath>
namespace ggh::GalaxyCore::utilities {
/**
 * @brief Converts galactic coordinates to Cartesian coordinates.
 *
 * This function takes galactic coordinates (longitude and latitude) and converts them
 * to Cartesian coordinates (x, y) based on the specified radius.
 *
 * @param longitude Galactic longitude in degrees.
 * @param latitude Galactic latitude in degrees.
 * @param radius The radius of the galaxy in coordinate units.
 * @return QPointF representing the Cartesian coordinates (x, y).
 */
#include <type_traits>

template <typename T>
    requires std::is_arithmetic_v<T>
struct CartesianCoordinates {
    T x;
    T y;

    CartesianCoordinates(T xCoord, T yCoord) : x(xCoord), y(yCoord) {}

    // Conversion to QPointF
    operator QPointF() const {
        return QPointF(static_cast<qreal>(x), static_cast<qreal>(y));
    }

    CartesianCoordinates(const CartesianCoordinates<T>& other) : x(other.x), y(other.y) {}
    CartesianCoordinates& operator=(const CartesianCoordinates<T>& other) {
        if (this != &other) {
            x = other.x;
            y = other.y;
        }
        return *this;
    }
    CartesianCoordinates&& operator=(CartesianCoordinates<T>&& other) noexcept {
        if (this != &other) {
            x = std::move(other.x);
            y = std::move(other.y);
        }
        return std::move(*this);
    }
    CartesianCoordinates(CartesianCoordinates<T>&& other) noexcept : x(std::move(other.x)), y(std::move(other.y)) {}

    constexpr bool operator==(const CartesianCoordinates<T>& other) const noexcept {
        return x == other.x && y == other.y;
    }
    constexpr bool operator!=(const CartesianCoordinates<T>& other) const noexcept {
        return !(*this == other);
    }

};

template <typename T>
    requires std::is_arithmetic_v<T>
struct PolarCoordinates {
    T radius;
    T angle; // in degrees

    PolarCoordinates(T radius_c, T angle_c) : radius(radius_c), angle(angle_c) {}

    // Conversion to QPointF
    operator QPointF() const {
        qreal rad = qDegreesToRadians(static_cast<qreal>(angle));
        return QPointF(static_cast<qreal>(radius) * qCos(rad), static_cast<qreal>(radius) * qSin(rad));
    }

    PolarCoordinates(const PolarCoordinates<T>& other) : radius(other.radius), angle(other.angle) {}
    PolarCoordinates& operator=(const PolarCoordinates<T>& other) {
        if (this != &other) {
            radius = other.radius;
            angle = other.angle;
        }
        return *this;
    }
    PolarCoordinates&& operator=(PolarCoordinates<T>&& other) noexcept {
        if (this != &other) {
            radius = std::move(other.radius);
            angle = std::move(other.angle);
        }
        return std::move(*this);
    }
    PolarCoordinates(PolarCoordinates<T>&& other) noexcept : radius(std::move(other.radius)), angle(std::move(other.angle)) {}
};

template <typename T>
    requires std::is_arithmetic_v<T>
struct Coordinates3D {
    T x;
    T y;
    T z;

    Coordinates3D(T xCoord, T yCoord, T zCoord) : x(xCoord), y(yCoord), z(zCoord) {}

    // Conversion to QPointF (ignoring z-coordinate)
    operator QPointF() const {
        return QPointF(static_cast<qreal>(x), static_cast<qreal>(y));
    }
};

template <typename T>
    requires std::is_arithmetic_v<T>
struct Coordinates3DPolar {
    T radius;
    T theta; // polar angle in degrees
    T phi;   // azimuthal angle in degrees

    Coordinates3DPolar(T r, T t, T p) : radius(r), theta(t), phi(p) {}

    // Conversion to QPointF (ignoring 3D coordinates)
    operator QPointF() const {
        return QPointF(static_cast<qreal>(radius * qCos(qDegreesToRadians(theta))),
                       static_cast<qreal>(radius * qSin(qDegreesToRadians(theta))));
    }
};

template <typename T>
    requires std::is_arithmetic_v<T>
PolarCoordinates<T> cartesianToPolar(const CartesianCoordinates<T>& cartesian) {
    // Convert degrees to radians
    T distance_to_origin = std::sqrt(cartesian.x * cartesian.x + cartesian.y * cartesian.y);
    T angle = std::atan2(cartesian.y, cartesian.x) * 180.0 / M_PI; // Convert radians to degree

    return PolarCoordinates<T>(distance_to_origin, angle);
}

template <typename T>
    requires std::is_arithmetic_v<T>
CartesianCoordinates<T> polarToCartesian(const PolarCoordinates<T>& polar) {
    // Convert angle from degrees to radians
    T rad = polar.angle * M_PI / 180.0;
    T x = polar.radius * std::cos(rad);
    T y = polar.radius * std::sin(rad);

    return CartesianCoordinates<T>(x, y);
}

template <typename T>
    requires std::is_arithmetic_v<T>
T calculateDistance(const CartesianCoordinates<T>& point1, const CartesianCoordinates<T>& point2) {
    T dx = point2.x - point1.x;
    T dy = point2.y - point1.y;
    return std::sqrt(dx * dx + dy * dy);
}

template <typename T>
    requires std::is_arithmetic_v<T>
T calculateDistance(const PolarCoordinates<T>& point1, const PolarCoordinates<T>& point2) {
    CartesianCoordinates<T> cart1 = polarToCartesian(point1);
    CartesianCoordinates<T> cart2 = polarToCartesian(point2);
    return calculateDistance(cart1, cart2);
}

} // namespace ggh::GalaxyCore::utilities

#endif // !GGH_MODULES_GALAXYCORE_UTILITIES_COORDINATES_H