#ifndef GGH_GALAXYCORE_MODELS_GALACTIC_DATE_H
#define GGH_GALAXYCORE_MODELS_GALACTIC_DATE_H

#include <string>

namespace ggh::GalaxyCore::models {
/**
 * @class GalacticDate
 * @brief Represents a date in the Galactic calendar system.
 */
class GalacticDate {
public:
    GalacticDate(int year, int month, int day, std::string_view date_postfix = "G") 
        : m_year(year), m_month(month), m_day(day), m_date_postfix(date_postfix) {}

    // Getters
    int year() const noexcept { return m_year; }
    int month() const noexcept { return m_month; }
    int day() const noexcept { return m_day; }
    std::string_view datePostfix() const noexcept { return m_date_postfix; }

    // Setters
    void setYear(int year) noexcept { m_year = year; }
    void setMonth(int month) noexcept { m_month = month; }
    void setDay(int day) noexcept { m_day = day; }
    void setDatePostfix(std::string_view date_postfix) noexcept { m_date_postfix = date_postfix; }

    // String representation
    std::string toString() const {
        return std::to_string(m_year) + "-" +
               (m_month < 10 ? "0" : "") + std::to_string(m_month) + "-" +
               (m_day < 10 ? "0" : "") + std::to_string(m_day) + " " + m_date_postfix;
    
            }
    
    // XML representation
    std::string toXml() const {
        return "<GalacticDate year=\"" + std::to_string(m_year) +
               "\" month=\"" + std::to_string(m_month) +
               "\" day=\"" + std::to_string(m_day) +
               "\" datePostfix=\"" + m_date_postfix + "\" />";
    }

    // Static method to get the XML tag name
    static std::string xmlTag() noexcept {
        return "GalacticDate";
    }

    static GalacticDate fromString(const std::string& dateStr) {
        int year, month, day;
        std::string postfix;
        sscanf(dateStr.c_str(), "%d-%d-%d %s", &year, &month, &day, &postfix[0]);
        return GalacticDate(year, month, day, postfix);
    }

    int operator-(const GalacticDate& other) const {
        return (m_year - other.m_year) * 365 + (m_month - other.m_month) * 30 + (m_day - other.m_day);
    }

    GalacticDate& operator+=(int days) {
        m_day += days;
        while (m_day > 30) { // Assuming 30 days per month for simplicity
            m_day -= 30;
            m_month++;
            if (m_month > 12) {
                m_month = 1;
                m_year++;
            }
        }
        return *this;
    }

    GalacticDate& operator-=(int days) {
        m_day -= days;
        while (m_day <= 0) {
            m_month--;
            if (m_month <= 0) {
                m_month = 12;
                m_year--;
            }
            m_day += 30; // Assuming 30 days per month for simplicity
        }
        return *this;
    }

    friend bool operator==(const GalacticDate& lhs, const GalacticDate& rhs) {
        return lhs.m_year == rhs.m_year && lhs.m_month == rhs.m_month && lhs.m_day == rhs.m_day;
    }

    friend bool operator!=(const GalacticDate& lhs, const GalacticDate& rhs) {
        return !(lhs == rhs);
    }

private:
    int m_year; ///< The year in the Galactic calendar
    int m_month; ///< The month in the Galactic calendar
    int m_day; ///< The day in the Galactic calendar
    std::string m_date_postfix; ///< Postfix for the date, e.g., "G" for Galactic
};
} // namespace ggh::GalaxyCore::models
#endif // GGH_GALAXYCORE_MODELS_GALACTIC_DATE_H