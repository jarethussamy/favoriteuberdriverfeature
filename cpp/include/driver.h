#ifndef DRIVER_H
#define DRIVER_H

#include <string>
#include <chrono>
#include <memory>

/**
 * @brief Represents a driver in the Uber system
 * 
 * This class encapsulates all driver-related information including
 * personal details, vehicle information, ratings, and availability status.
 */
class Driver {
public:
    enum class Status {
        OFFLINE,
        ONLINE,
        BUSY,
        ON_TRIP
    };

    struct Location {
        double latitude;
        double longitude;
        std::chrono::system_clock::time_point timestamp;
        
        Location(double lat = 0.0, double lng = 0.0) 
            : latitude(lat), longitude(lng), timestamp(std::chrono::system_clock::now()) {}
    };

    struct Vehicle {
        std::string make;
        std::string model;
        std::string color;
        std::string plateNumber;
        int year;
        
        Vehicle(const std::string& make = "", const std::string& model = "", 
                const std::string& color = "", const std::string& plate = "", int year = 0)
            : make(make), model(model), color(color), plateNumber(plate), year(year) {}
    };

private:
    std::string m_id;
    std::string m_name;
    std::string m_phoneNumber;
    std::string m_email;
    std::string m_profilePhoto;
    double m_rating;
    int m_completedTrips;
    Status m_status;
    Location m_currentLocation;
    Vehicle m_vehicle;
    std::chrono::system_clock::time_point m_lastActiveTime;
    bool m_isVerified;

public:
    // Constructors
    Driver();
    Driver(const std::string& id, const std::string& name, const std::string& phone);
    
    // Destructor
    virtual ~Driver() = default;
    
    // Copy constructor and assignment operator
    Driver(const Driver& other);
    Driver& operator=(const Driver& other);
    
    // Move constructor and assignment operator
    Driver(Driver&& other) noexcept;
    Driver& operator=(Driver&& other) noexcept;

    // Getters
    const std::string& getId() const { return m_id; }
    const std::string& getName() const { return m_name; }
    const std::string& getPhoneNumber() const { return m_phoneNumber; }
    const std::string& getEmail() const { return m_email; }
    const std::string& getProfilePhoto() const { return m_profilePhoto; }
    double getRating() const { return m_rating; }
    int getCompletedTrips() const { return m_completedTrips; }
    Status getStatus() const { return m_status; }
    const Location& getCurrentLocation() const { return m_currentLocation; }
    const Vehicle& getVehicle() const { return m_vehicle; }
    std::chrono::system_clock::time_point getLastActiveTime() const { return m_lastActiveTime; }
    bool isVerified() const { return m_isVerified; }
    bool isOnline() const { return m_status == Status::ONLINE; }
    bool isAvailable() const { return m_status == Status::ONLINE; }

    // Setters
    void setName(const std::string& name) { m_name = name; }
    void setPhoneNumber(const std::string& phone) { m_phoneNumber = phone; }
    void setEmail(const std::string& email) { m_email = email; }
    void setProfilePhoto(const std::string& photo) { m_profilePhoto = photo; }
    void setRating(double rating);
    void setStatus(Status status);
    void setCurrentLocation(const Location& location) { m_currentLocation = location; }
    void setVehicle(const Vehicle& vehicle) { m_vehicle = vehicle; }
    void setVerified(bool verified) { m_isVerified = verified; }

    // Business logic methods
    void updateRating(double newRating);
    void incrementCompletedTrips();
    void goOnline();
    void goOffline();
    void updateLocation(double latitude, double longitude);
    double calculateDistanceFrom(const Location& otherLocation) const;
    int getEstimatedArrivalTime(const Location& destination) const;
    std::string getStatusString() const;
    std::string getLastSeenString() const;
    
    // Comparison operators
    bool operator==(const Driver& other) const;
    bool operator!=(const Driver& other) const;
    bool operator<(const Driver& other) const; // For sorting by rating

    // Serialization
    std::string toJson() const;
    static Driver fromJson(const std::string& json);
    
    // Utility methods
    bool isNearby(const Location& userLocation, double radiusKm = 10.0) const;
    void updateLastActiveTime();
    std::chrono::minutes getTimeSinceLastActive() const;
};

#endif // DRIVER_H
