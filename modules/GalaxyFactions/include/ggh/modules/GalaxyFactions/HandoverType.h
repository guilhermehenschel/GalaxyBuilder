#ifndef GGH_MODULES_GALAXYFACTIONS_HANDOVER_TYPE_H
#define GGH_MODULES_GALAXYFACTIONS_HANDOVER_TYPE_H

#include <string>

namespace ggh::Galaxy::Factions {

class HandoverType
{
public:
    HandoverType(int id, const std::string& name, const std::string& description);

    // Getters
    int id() const { return m_id; }
    std::string name() const { return m_name; }
    std::string description() const { return m_description; }

    // Setters
    void setId(int id);
    void setName(const std::string& name);
    void setDescription(const std::string& description);

private:
    int m_id;
    std::string m_name;
    std::string m_description;
};

} // namespace GalaxyFactions
#endif // GGH_MODULES_GALAXYFACTIONS_HANDOVER_TYPE_H
