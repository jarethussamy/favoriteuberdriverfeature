#include "Driver.h"
#include <cmath>
#include <random>
#include <sstream>
#include <iomanip>
#include <algorithm>

// Default constructor
Driver::Driver() 
    : m_id(""), m_name(""), m_phoneNumber(""), m_email(""), m_profilePhoto(""),
      m_rating(0.0), m_completedTrips(0), m_status(Status::OFFLINE),
      m_currentLocation(0.0, 0.0), m_vehicle(), 
      m_lastActiveTime(std::chrono::system_clock::now()), m_isVerified(false) {
}

// Parameterized constructor
Driver::Driver(const std::string& id, const std::string& name, const std::string& phone)
    : m_id(id), m_name(name), m_phoneNumber(phone), m_email(""), m_profilePhoto(""),
      m_rating(5.0), m_completedTrips(0), m_status(Status::OFFLINE),
      m_currentLocation(0.0, 0.0), m_vehicle(),
      m_lastActiveTime(std::chrono::system_clock::now()), m_isVerified(false) {
}

// Copy constructor
Driver::Driver(const Driver& other)
    : m_id(other.m_id), m_name(other.m_name), m_phoneNumber(other.m_phoneNumber),
      m_email(other.m_email), m_profilePhoto(other.m_profilePhoto),
      m_rating(other.m_rating), m_completedTrips(other.m_completedTrips),
      m_status(other.m_status), m_currentLocation(other.m_currentLocation),
      m_vehicle(other.m_vehicle), m_lastActiveTime(other.m_lastActiveTime),
      m_isVerified(other.m_isVerified) {
}

// Copy assignment operator
Driver& Driver::operator=(const Driver& other) {
    if (this != &other) {
        m_id = other.m_id;
        m_name = other.m_name;
        m_phoneNumber = other.m_phoneNumber;
        m_email = other.m_email;
        m_profilePhoto = other.m_profilePhoto;
        m_rating = other.m_rating;
        m_completedTrips = other.m_completedTrips;
        m_status = other.m_status;
        m_currentLocation = other.m_currentLocation;
        m_vehicle = other.m_vehicle;
        m_lastActiveTime = other.m_lastActiveTime;
        m_isVerified = other.m_isVerified;
    }
    return *this;
}

// Move constructor
Driver::Driver(Driver&& other) noexcept
    : m_id(std::move(other.m_id)), m_name(std::move(other.m_name)),
      m_phoneNumber(std::move(other.m_phoneNumber)), m_email(std::move(other.m_email)),
      m_profilePhoto(std::move(other.m_profilePhoto)), m_rating(other.m_rating),
      m_completedTrips(other.m_completedTrips), m_status(other.m_status),
      m_currentLocation(std::move(other.m_currentLocation)), m_vehicle(std::move(other.m_vehicle)),
      m_lastActiveTime(other.m_lastActiveTime), m_isVerified(other.m_isVerified) {
}

// Move assignment operator
Driver& Driver::operator=(Driver&& other) noexcept {
    if (this != &other) {
        m_id = std::move(other.m_id);
        m_name = std::move(other.m_name);
        m_phoneNumber = std::move(other.m_phoneNumber);
        m_email = std::move(other.m_email);
        m_profilePhoto = std::move(other.m_profilePhoto);
        m_rating = other.m_rating;
        m_completedTrips = other.m_completedTrips;
        m_status = other.m_status;
        m_currentLocation = std::move(other.m_currentLocation);
        m_vehicle = std::move(other.m_vehicle);
        m_lastActiveTime = other.m_lastActiveTime;
        m_isVerified = other.m_isVerified;
    }
    return *this;
}

// Setter methods with validation
void Driver::setRating(double rating) {
    m_rating = std::max(0.0, std::min(5.0, rating));
}

void Driver::setStatus(Status status) {
    m_status = status;
    updateLastActiveTime();
}

// Business logic methods
void Driver::updateRating(double newRating) {
    if (m_completedTrips == 0) {
        m_rating = newRating;
    } else {
        // Weighted average with existing rating
        double totalRating = m_rating * m_completedTrips + newRating;
        m_rating = totalRating / (m_completedTrips + 1);
    }
}

void Driver::incrementCompletedTrips() {
    m_completedTrips++;
    updateLastActiveTime();
}

void Driver::goOnline() {
    m_status = Status::ONLINE;
    updateLastActiveTime();
}

void Driver::goOffline() {
    m_status = Status::OFFLINE;
    updateLastActiveTime();
}

void Driver::updateLocation(double latitude, double longitude) {
    m_currentLocation = Location(latitude, longitude);
    updateLastActiveTime();
}

double Driver::calculateDistanceFrom(const Location& otherLocation) const {
    // Haversine formula for calculating distance between two points on Earth
    const double R = 6371.0; // Earth's radius in kilometers
    
    double lat1 = m_currentLocation.latitude * M_PI / 180.0;
    double lat2 = otherLocation.latitude * M_PI / 180.0;
    double deltaLat = (otherLocation.latitude - m_currentLocation.latitude) * M_PI / 180.0;
    double deltaLng = (otherLocation.longitude - m_currentLocation.longitude) * M_PI / 180.0;
    
    double a = sin(deltaLat / 2) * sin(deltaLat / 2) +
               cos(lat1) * cos(lat2) * sin(deltaLng / 2) * sin(deltaLng / 2);
    double c = 2 * atan2(sqrt(a), sqrt(1 - a));
    
    return R * c;
}

int Driver::getEstimatedArrivalTime(const Location& destination) const {
    double distance = calculateDistanceFrom(destination);
    // Assume average speed of 30 km/h in city traffic
    double timeInHours = distance / 30.0;
    return static_cast<int>(timeInHours * 60); // Convert to minutes
}

std::string Driver::getStatusString() const {
    switch (m_status) {
        case Status::OFFLINE: return "Offline";
        case Status::ONLINE: return "Online";
        case Status::BUSY: return "Busy";
        case Status::ON_TRIP: return "On Trip";
        default: return "Unknown";
    }
}

std::string Driver::getLastSeenString() const {
    auto now = std::chrono::system_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::minutes>(now - m_lastActiveTime);
    
    if (duration.count() < 60) {
        return std::to_string(duration.count()) + " minutes ago";
    } else if (duration.count() < 1440) { // Less than 24 hours
        return std::to_string(duration.count() / 60) + " hours ago";
    } else {
        return std::to_string(duration.count() / 1440) + " days ago";
    }
}

// Comparison operators
bool Driver::operator==(const Driver& other) const {
    return m_id == other.m_id;
}

bool Driver::operator!=(const Driver& other) const {
    return !(*this == other);
}

bool Driver::operator<(const Driver& other) const {
    // Sort by rating in descending order (higher rating first)
    return m_rating > other.m_rating;
}

// Utility methods
bool Driver::isNearby(const Location& userLocation, double radiusKm) const {
    return calculateDistanceFrom(userLocation) <= radiusKm;
}

void Driver::updateLastActiveTime() {
    m_lastActiveTime = std::chrono::system_clock::now();
}

std::chrono::minutes Driver::getTimeSinceLastActive() const {
    auto now = std::chrono::system_clock::now();
    return std::chrono::duration_cast<std::chrono::minutes>(now - m_lastActiveTime);
}

// Serialization methods
std::string Driver::toJson() const {
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(6);
    
    oss << "{\n";
    oss << "  \"id\": \"" << m_id << "\",\n";
    oss << "  \"name\": \"" << m_name << "\",\n";
    oss << "  \"phoneNumber\": \"" << m_phoneNumber << "\",\n";
    oss << "  \"email\": \"" << m_email << "\",\n";
    oss << "  \"profilePhoto\": \"" << m_profilePhoto << "\",\n";
    oss << "  \"rating\": " << m_rating << ",\n";
    oss << "  \"completedTrips\": " << m_completedTrips << ",\n";
    oss << "  \"status\": \"" << getStatusString() << "\",\n";
    oss << "  \"currentLocation\": {\n";
    oss << "    \"latitude\": " << m_currentLocation.latitude << ",\n";
    oss << "    \"longitude\": " << m_currentLocation.longitude << "\n";
    oss << "  },\n";
    oss << "  \"vehicle\": {\n";
    oss << "    \"make\": \"" << m_vehicle.make << "\",\n";
    oss << "    \"model\": \"" << m_vehicle.model << "\",\n";
    oss << "    \"color\": \"" << m_vehicle.color << "\",\n";
    oss << "    \"plateNumber\": \"" << m_vehicle.plateNumber << "\",\n";
    oss << "    \"year\": " << m_vehicle.year << "\n";
    oss << "  },\n";
    oss << "  \"isVerified\": " << (m_isVerified ? "true" : "false")
