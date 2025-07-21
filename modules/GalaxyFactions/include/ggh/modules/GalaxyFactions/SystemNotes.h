#ifndef GGH_MODULES_GALAXYFACTIONS_SYSTEM_NOTES_H
#define GGH_MODULES_GALAXYFACTIONS_SYSTEM_NOTES_H

#include <string>

namespace ggh::Galaxy::Factions {

class SystemNotes
{
public:
    SystemNotes();
    SystemNotes(int id, int systemId, const std::string& note);

    // Getters
    int id() const { return m_id; }
    int systemId() const { return m_systemId; }
    std::string note() const { return m_note; }

    // Setters
    void setId(int id);
    void setSystemId(int systemId);
    void setNote(const std::string& note);

private:
    int m_id;
    int m_systemId;
    std::string m_note;
};

} // namespace GalaxyFactions

#endif // !GGH_MODULES_GALAXYFACTIONS_SYSTEM_NOTES_H
