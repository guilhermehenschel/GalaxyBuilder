/**
 * @file test_planetlistmodel_simple.cpp
 * @brief Simplified unit tests for the PlanetListModel class.
 */

#include <iostream>
#include <cassert>
#include <memory>
#include <QCoreApplication>

#include "PlanetListModel.h"
#include "Planet.h"

/**
 * @brief Simple test to verify PlanetListModel functionality
 */
int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    
    std::cout << "Testing PlanetListModel..." << std::endl;
    
    // Test 1: Model creation
    PlanetListModel model;
    std::cout << "✓ Model created successfully" << std::endl;
    
    // Test 2: Initial state
    assert(model.rowCount() == 0);
    std::cout << "✓ Initial row count is 0" << std::endl;
    
    // Test 3: Add planet using model method
    Planet* planet1 = model.addPlanet("Earth", 150e6);
    assert(planet1 != nullptr);
    planet1->setType(PlanetType::Rocky);
    planet1->setSize(12742.0);
    
    assert(model.rowCount() == 1);
    std::cout << "✓ Planet added successfully, row count is 1" << std::endl;
    
    // Test 4: Data retrieval
    QModelIndex index = model.index(0, 0);
    assert(index.isValid());
    
    QString name = model.data(index, PlanetListModel::NameRole).toString();
    assert(name == "Earth");
    std::cout << "✓ Planet name retrieved correctly: " << name.toStdString() << std::endl;
    
    // Test 5: Add another planet
    Planet* planet2 = model.addPlanet("Mars", 227e6);
    assert(planet2 != nullptr);
    planet2->setType(PlanetType::Rocky);
    planet2->setSize(6779.0);
    
    assert(model.rowCount() == 2);
    std::cout << "✓ Second planet added, row count is 2" << std::endl;
    
    // Test 6: Get planet by index
    Planet* retrievedPlanet = model.getPlanetAt(0);
    assert(retrievedPlanet != nullptr);
    std::cout << "✓ Planet retrieved by index" << std::endl;
    
    // Test 7: Remove planet
    bool removed = model.removePlanetAt(0);
    assert(removed);
    assert(model.rowCount() == 1);
    std::cout << "✓ Planet removed successfully, row count is 1" << std::endl;
    
    // Test 8: Clear all planets
    model.clearPlanets();
    assert(model.rowCount() == 0);
    std::cout << "✓ All planets cleared, row count is 0" << std::endl;
    
    std::cout << "\nAll tests passed! ✅" << std::endl;
    std::cout << "PlanetListModel is working correctly." << std::endl;
    
    return 0;
}
