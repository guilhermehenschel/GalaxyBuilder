#ifndef GGH_MODULES_GALAXYFACTIONS_RESOURCE_TYPE_H
#define GGH_MODULES_GALAXYFACTIONS_RESOURCE_TYPE_H

#include <string>

namespace ggh::Galaxy::Factions::models {

class ResourceType
{
public:
    ResourceType();
    ResourceType(int id, const std::string& name, const std::string& description);

    // Getters
    int id() const { return m_id; }
    std::string name() const { return m_name; }
    std::string description() const { return m_description; }

    // Setters
    void setId(int id);
    void setName(const std::string& name);
    void setDescription(const std::string& description);

    bool operator==(const ResourceType& other) const {
        return m_id == other.m_id && m_name == other.m_name && m_description == other.m_description;
    }

private:
    int m_id;
    std::string m_name;
    std::string m_description;
};

} // namespace ggh::Galaxy::Factions

// Hash specialization for ResourceType
namespace std {
    template<>
    struct hash<ggh::Galaxy::Factions::models::ResourceType> {
        std::size_t operator()(const ggh::Galaxy::Factions::models::ResourceType& r) const noexcept {
            std::size_t h1 = std::hash<int>{}(r.id());
            std::size_t h2 = std::hash<std::string>{}(r.name());
            std::size_t h3 = std::hash<std::string>{}(r.description());
            return h1 ^ (h2 << 1) ^ (h3 << 2);
        }
    };
}

#endif // GGH_MODULES_GALAXYFACTIONS_RESOURCE_TYPE_H