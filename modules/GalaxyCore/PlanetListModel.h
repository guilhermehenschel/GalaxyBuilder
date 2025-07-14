/**
 * @file PlanetListModel.h
 * @brief Model for managing planets within a star system.
 * 
 * This file contains the PlanetListModel class which provides a Qt model interface
 * for managing a collection of Planet objects within a star system. It inherits from
 * QAbstractListModel to provide seamless integration with QML ListView components.
 */

#pragma once

#include "Planet.h"
#include "galaxycore_global.h"
#include <QAbstractListModel>
#include <QQmlEngine>
#include <memory>
#include <vector>

/**
 * @class PlanetListModel
 * @brief Qt model for managing a list of planets in a star system.
 * 
 * The PlanetListModel class provides a Qt model interface for managing Planet objects
 * within a star system. It supports adding, removing, and modifying planets while
 * maintaining proper model/view synchronization.
 * 
 * Features:
 * - Automatic model updates when planet properties change
 * - QML integration through Q_INVOKABLE methods
 * - Proper memory management using unique_ptr
 * - Role-based data access for QML bindings
 * 
 * @see Planet
 * @see StarSystemQml
 */
class GALAXYCORE_EXPORT PlanetListModel : public QAbstractListModel {
    Q_OBJECT
    QML_ELEMENT

public:
    /**
     * @enum PlanetRoles
     * @brief Defines roles for accessing planet data in QML.
     * 
     * These roles are used by QML to access different properties of planets
     * in the model. Each role maps to a specific planet property.
     */
    enum PlanetRoles {
        PlanetRole = Qt::UserRole + 1,  ///< Direct access to Planet object
        NameRole,                       ///< Planet name
        TypeRole,                       ///< Planet type (enum value)
        SizeRole,                       ///< Planet size in Earth radii
        MassRole,                       ///< Planet mass in Earth masses
        MoonsRole,                      ///< Number of moons
        OrbitDistanceRole,              ///< Orbital distance from star
        TemperatureRole                 ///< Average temperature
    };

    /**
     * @brief Constructs a new PlanetListModel.
     * @param parent The parent QObject.
     */
    explicit PlanetListModel(QObject* parent = nullptr);
    
    /**
     * @brief Destructor.
     */
    ~PlanetListModel() override = default;

    // QAbstractListModel interface
    /**
     * @brief Returns the number of planets in the model.
     * @param parent The parent model index (unused for list models).
     * @return The number of planets.
     */
    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    
    /**
     * @brief Returns data for the specified index and role.
     * @param index The model index.
     * @param role The data role.
     * @return The requested data as QVariant.
     */
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    
    /**
     * @brief Returns the role names for QML bindings.
     * @return Hash map of role names.
     */
    QHash<int, QByteArray> roleNames() const override;

    // Planet management
    /**
     * @brief Adds a new planet with default properties.
     * @return Pointer to the newly created planet.
     */
    Q_INVOKABLE Planet* addPlanet();
    
    /**
     * @brief Adds a new planet with specified name and orbit distance.
     * @param name The name of the planet.
     * @param orbitDistance The orbital distance from the star.
     * @return Pointer to the newly created planet.
     */
    Q_INVOKABLE Planet* addPlanet(const QString& name, double orbitDistance);
    
    /**
     * @brief Removes the specified planet from the model.
     * @param planet Pointer to the planet to remove.
     * @return True if the planet was successfully removed.
     */
    Q_INVOKABLE bool removePlanet(Planet* planet);
    
    /**
     * @brief Removes the planet at the specified index.
     * @param index The index of the planet to remove.
     * @return True if the planet was successfully removed.
     */
    Q_INVOKABLE bool removePlanetAt(int index);
    
    /**
     * @brief Gets the planet at the specified index.
     * @param index The index of the planet.
     * @return Pointer to the planet, or nullptr if index is invalid.
     */
    Q_INVOKABLE Planet* getPlanetAt(int index) const;
    
    /**
     * @brief Gets the total number of planets.
     * @return The number of planets in the model.
     */
    Q_INVOKABLE int getPlanetCount() const;
    
    /**
     * @brief Removes all planets from the model.
     */
    Q_INVOKABLE void clearPlanets();

    // Direct access for C++ code
    /**
     * @brief Gets direct access to the planets collection.
     * @return Const reference to the internal planets vector.
     * @note This method is intended for C++ code only.
     */
    const std::vector<std::unique_ptr<Planet>>& getPlanets() const { return m_planets; }

public slots:
    /**
     * @brief Notifies that the model data has changed.
     * 
     * This slot can be called to trigger a data change notification
     * for all items in the model.
     */
    void notifyDataChanged();

private slots:
    /**
     * @brief Handles changes to planet properties.
     * 
     * This slot is automatically connected to planet property change signals
     * and updates the corresponding model index when a planet changes.
     */
    void onPlanetChanged();

private:
    std::vector<std::unique_ptr<Planet>> m_planets;  ///< The planets collection
    
    /**
     * @brief Connects planet signals to model update slots.
     * @param planet The planet to connect signals for.
     */
    void connectPlanetSignals(Planet* planet);
    
    /**
     * @brief Disconnects planet signals from model update slots.
     * @param planet The planet to disconnect signals for.
     */
    void disconnectPlanetSignals(Planet* planet);
};
