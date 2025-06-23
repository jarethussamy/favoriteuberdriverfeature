#ifndef FAVORITE_DRIVER_MANAGER_H
#define FAVORITE_DRIVER_MANAGER_H

#include "Driver.h"
#include "RideRequest.h"
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <functional>
#include <memory>
#include <mutex>

/**
 * @brief Manages favorite drivers for users and handles ride requests
 * 
 * This class provides functionality to manage user's favorite drivers,
 * prioritize ride requests, and handle the favorite driver booking flow.
 */
class FavoriteDriverManager {
public:
    using DriverRequestCallback = std::function<void(bool accepted, const std::string& reason)>;
    using NotificationCallback = std::function<void(const std::string& userId, const std::string& message)>;

private:
    // User ID -> Set of favorite driver IDs
    std::unordered_map<std::string, std::unordered_set<std::string>> m_userFavorites;
    
    // Driver ID -> Driver object
    std::unordered_map<std::string, std::shared_ptr<Driver>> m_drivers;
    
    // Active ride requests
    std::unordered_map<std::string, std::shared_ptr<RideRequest>> m_activeRequests;
    
    // Callbacks for notifications
    NotificationCallback m_notificationCallback;
    
    // Thread safety
    mutable std::mutex m_mutex;
    
    // Configuration
    static constexpr int MAX_FAVORITE_DRIVERS = 10;
    static constexpr int FAVORITE_REQUEST_TIMEOUT_SECONDS = 30;
    static constexpr double MAX_PICKUP_DISTANCE_KM = 15.0;

public:
    // Constructor and Destructor
    FavoriteDriverManager();
    virtual ~FavoriteDriverManager() = default;
    
    // Delete copy constructor and assignment operator
    FavoriteDriverManager(const FavoriteDriverManager&) = delete;
    FavoriteDriverManager& operator=(const FavoriteDriverManager&) = delete;

    // Favorite driver management
    bool addFavoriteDriver(const std::string& userId, const std::string& driverId);
    bool removeFavoriteDriver(const std::string& userId, const std::string& driverId);
    std::vector<std::shared_ptr<Driver>> getFavoriteDrivers(const std::string& userId) const;
    std::vector<std::shared_ptr<Driver>> getAvailableFavoriteDrivers(const std::string& userId) const;
    bool isFavoriteDriver(const std::string& userId, const std::string& driverId) const;
    int getFavoriteDriverCount(const std::string& userId) const;
    
    // Driver management
    bool addDriver(std::shared_ptr<Driver> driver);
    bool removeDriver(const std::string& driverId);
    std::shared_ptr<Driver> getDriver(const std::string& driverId) const;
    std::vector<std::shared_ptr<Driver>> getAllDrivers() const;
    std::vector<std::shared_ptr<Driver>> getNearbyDrivers(const Driver::Location& location, double radiusKm = 10.0) const;
    
    // Ride request handling
    std::string requestFavoriteDriver(const std::string& userId, const std::string& driverId, 
                                    const RideRequest& request, DriverRequestCallback callback);
    std::string requestAnyFavoriteDriver(const std::string& userId, const RideRequest& request, 
                                       DriverRequestCallback callback);
    std::string requestRegularDriver(const std::string& userId, const RideRequest& request, 
                                   DriverRequestCallback callback);
    
    bool cancelRideRequest(const std::string& requestId);
    std::shared_ptr<RideRequest> getRideRequest(const std::string& requestId) const;
    
    // Driver response handling
    bool acceptRideRequest(const std::string& driverId, const std::string& requestId);
    bool rejectRideRequest(const std::string& driverId, const std::string& requestId, const std::string& reason = "");
    
    // Statistics and analytics
    std::vector<std::shared_ptr<Driver>> getMostPopularFavoriteDrivers(int limit = 10) const;
    double getFavoriteDriverAcceptanceRate(const std::string& driverId) const;
    std::unordered_map<std::string, int> getFavoriteDriverStats() const;
    
    // Notifications
    void setNotificationCallback(NotificationCallback callback) { m_notificationCallback = callback; }
    
    // Utility methods
    std::vector<std::shared_ptr<Driver>> sortDriversByPreference(const std::string& userId, 
                                                               const std::vector<std::shared_ptr<Driver>>& drivers) const;
    std::vector<std::shared_ptr<Driver>> filterByAvailability(const std::vector<std::shared_ptr<Driver>>& drivers) const;
    std::vector<std::shared_ptr<Driver>> filterByDistance(const std::vector<std::shared_ptr<Driver>>& drivers, 
                                                         const Driver::Location& userLocation, double maxDistanceKm) const;
    
    // Configuration
    void setMaxFavoriteDrivers(int maxDrivers);
    void setRequestTimeout(int timeoutSeconds);
    void setMaxPickupDistance(double distanceKm);
    
    // Data persistence
    bool saveToFile(const std::string& filename) const;
    bool loadFromFile(const std::string& filename);
    std::string toJson() const;
    bool fromJson(const std::string& json);

private:
    // Internal helper methods
    std::string generateRequestId() const;
    void notifyUser(const std::string& userId, const std::string& message);
    void notifyDriver(const std::string& driverId, const std::string& message);
    void handleRequestTimeout(const std::string& requestId);
    std::shared_ptr<Driver> findBestAlternativeDriver(const RideRequest& request) const;
    void updateDriverStatistics(const std::string& driverId, bool accepted);
    
    // Request prioritization
    int calculateDriverPriority(const std::string& userId, const std::shared_ptr<Driver>& driver) const;
    std::vector<std::shared_ptr<Driver>> prioritizeDrivers(const std::string& userId, 
                                                          const std::vector<std::shared_ptr<Driver>>& drivers) const;
};

#endif // FAVORITE_DRIVER_MANAGER_H
