/**
 * @file StarSystem.h
 * @brief Represents a single star system in the galaxy.
 */

#pragma once

#include "Common.h"
#include <QPointF>
#include <QColor>
#include <QString>

/**
 * @class StarSystem
 * @brief Represents a star system with position, type, size, and name.
 */
class StarSystem {
public:
    /**
     * @brief Constructs a StarSystem with the given ID, position, and type.
     * @param id Unique identifier for the star system.
     * @param position The position of the star system in the galaxy.
     * @param type The type of the star (default is YellowStar).
     */
    StarSystem(SystemId id, const QPointF& position, StarType type = StarType::YellowStar);
    ~StarSystem() = default;

    // Copy and move constructors/operators
    StarSystem(const StarSystem&) = default;
    StarSystem& operator=(const StarSystem&) = default;
    StarSystem(StarSystem&&) = default;
    StarSystem& operator=(StarSystem&&) = default;

    /**
     * @brief Gets the unique identifier of the star system.
     * @return The unique identifier of the star system.
     */
    SystemId getId() const noexcept { return m_id; }

    /**
     * @brief Gets the position of the star system.
     * @return The position of the star system as a QPointF.
     */
    const QPointF& getPosition() const noexcept { return m_position; }

    /**
     * @brief Gets the type of the star in the system.
     * @return The type of the star.
     */
    StarType getStarType() const noexcept { return m_starType; }

    /**
     * @brief Gets the size of the star system.
     * @return The size of the star system.
     */
    SystemSize getSystemSize() const noexcept { return m_systemSize; }

    /**
     * @brief Gets the name of the star system.
     * @return The name of the star system as a QString.
     */
    const QString& getName() const noexcept { return m_name; }

    /**
     * @brief Gets the color of the star based on its type.
     * @return The color of the star as a QColor.
     */
    QColor getStarColor() const noexcept;

    /**
     * @brief Gets the influence radius of the star system.
     * @return The influence radius of the star system.
     */
    double getInfluenceRadius() const noexcept;

    /**
     * @brief Sets the position of the star system.
     * @param position The new position of the star system.
     */
    void setPosition(const QPointF& position) { m_position = position; }

    /**
     * @brief Sets the type of the star in the system.
     * @param type The new type of the star.
     */
    void setStarType(StarType type) { m_starType = type; }

    /**
     * @brief Sets the size of the star system.
     * @param size The new size of the star system.
     */
    void setSystemSize(SystemSize size) { m_systemSize = size; }

    /**
     * @brief Sets the name of the star system.
     * @param name The new name of the star system.
     */
    void setName(const QString& name) { m_name = name; }

    /**
     * @brief Calculates the distance to another star system.
     * @param other The other star system.
     * @return The distance to the other star system.
     */
    double distanceTo(const StarSystem& other) const;

    /**
     * @brief Checks if another star system is within a certain range.
     * @param other The other star system.
     * @param maxDistance The maximum distance to check.
     * @return True if the other star system is within range, false otherwise.
     */
    bool isWithinRange(const StarSystem& other, double maxDistance) const;

private:
    SystemId m_id;
    QPointF m_position;
    StarType m_starType;
    SystemSize m_systemSize;
    QString m_name;

    /**
     * @brief Generates a random name for the star system.
     */
    void generateName();

    /**
     * @brief Gets the color associated with a specific star type.
     * @param type The type of the star.
     * @return The color associated with the star type.
     */
    static QColor getColorForStarType(StarType type);
};
