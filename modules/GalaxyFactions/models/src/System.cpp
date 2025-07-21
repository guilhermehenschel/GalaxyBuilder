#include "ggh/modules/GalaxyFactions/models/System.h"
#include <algorithm>

namespace ggh::Galaxy::Factions::models {

/**
 * @brief Constructs a System wrapper around a GalaxyCore StarSystemModel
 * @param starSystemModel The core star system model to wrap
 */
System::System(std::shared_ptr<GalaxyCore::models::StarSystemModel> starSystemModel)
    : m_starSystemModel(std::move(starSystemModel))
{
}

/**
 * @brief Gets the system ID from the core model
 * @return The system ID
 */
int System::id() const
{
    return m_starSystemModel ? static_cast<int>(m_starSystemModel->getId()) : 0;
}

/**
 * @brief Gets the system name from the core model
 * @return The system name
 */
std::string System::name() const
{
    return m_starSystemModel ? m_starSystemModel->getName() : std::string{};
}

/**
 * @brief Gets the system description (uses name as description since core model doesn't have description)
 * @return The system description
 */
std::string System::description() const
{
    return m_starSystemModel ? m_starSystemModel->getName() : std::string{};
}

/**
 * @brief Gets all resource bonuses for this system
 * @return Vector of resource bonuses
 */
std::vector<SystemResourceBonus> System::resourceBonuses() const
{
    return m_resourceBonuses;
}

/**
 * @brief Gets all notes for this system
 * @return Vector of system notes
 */
std::vector<SystemNotes> System::notes() const
{
    return m_notes;
}

/**
 * @brief Sets the system ID (not supported by core model)
 * @param id The new ID
 */
void System::setId(int id)
{
    // StarSystemModel doesn't support changing ID after construction
    // This is a no-op for compatibility
}

/**
 * @brief Sets the system name in the core model
 * @param name The new name
 */
void System::setName(const std::string& name)
{
    if (m_starSystemModel) {
        m_starSystemModel->setName(name);
    }
}

/**
 * @brief Sets the system description (not supported by core model)
 * @param description The new description
 */
void System::setDescription(const std::string& description)
{
    // StarSystemModel doesn't have a separate description field
    // This is a no-op for compatibility
}

/**
 * @brief Adds a resource bonus to the system
 * @param bonus The resource bonus to add
 */
void System::addResourceBonus(const SystemResourceBonus& bonus)
{
    m_resourceBonuses.push_back(bonus);
}

/**
 * @brief Adds a resource bonus for a specific resource type
 * @param resourceType The resource type
 * @param bonusAmount The bonus amount
 */
void System::addResourceBonus(const ResourceType& resourceType, int bonusAmount)
{
    SystemResourceBonus bonus;
    bonus.setSystemId(id());
    bonus.setResourceType(resourceType);
    bonus.setBonusAmount(static_cast<float>(bonusAmount));
    m_resourceBonuses.push_back(bonus);
}

/**
 * @brief Removes resource bonuses for a specific resource type
 * @param resourceType The resource type to remove bonuses for
 */
void System::removeResourceBonus(const ResourceType& resourceType)
{
    m_resourceBonuses.erase(
        std::remove_if(m_resourceBonuses.begin(), m_resourceBonuses.end(),
            [&resourceType](const SystemResourceBonus& bonus) {
                return bonus.resourceType().id() == resourceType.id();
            }),
        m_resourceBonuses.end()
    );
}

/**
 * @brief Adds a note to the system
 * @param note The note to add
 */
void System::addNote(const SystemNotes& note)
{
    m_notes.push_back(note);
}

/**
 * @brief Removes a note from the system by ID
 * @param noteId The ID of the note to remove
 */
void System::removeNote(int noteId)
{
    m_notes.erase(
        std::remove_if(m_notes.begin(), m_notes.end(),
            [noteId](const SystemNotes& note) {
                return note.id() == noteId;
            }),
        m_notes.end()
    );
}

/**
 * @brief Gets all resource bonuses for this system
 * @return Vector of resource bonuses
 */
std::vector<SystemResourceBonus> System::getResourceBonuses() const
{
    return m_resourceBonuses;
}

/**
 * @brief Gets all notes for this system
 * @return Vector of system notes
 */
std::vector<SystemNotes> System::getNotes() const
{
    return m_notes;
}

/**
 * @brief Gets the total bonus amount for a specific resource type
 * @param resourceTypeId The resource type ID
 * @return The total bonus amount
 */
int System::getResourceBonusByType(int resourceTypeId) const
{
    int totalBonus = 0;
    for (const auto& bonus : m_resourceBonuses) {
        if (bonus.resourceType().id() == resourceTypeId) {
            totalBonus += static_cast<int>(bonus.bonusAmount());
        }
    }
    return totalBonus;
}

/**
 * @brief Gets the handover history for this system
 * @return Vector of faction handovers
 */
std::vector<FactionHandover> System::getHandoverHistory() const
{
    return m_handoverHistory;
}

} // namespace ggh::Galaxy::Factions
