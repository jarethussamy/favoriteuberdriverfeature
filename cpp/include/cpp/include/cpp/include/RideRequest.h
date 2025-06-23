#ifndef RIDE_REQUEST_H
#define RIDE_REQUEST_H

#include "Driver.h"
#include <string>
#include <chrono>
#include <memory>

/**
 * @brief Represents a ride request in the Uber system
 * 
 * This class encapsulates all ride request information including
 * pickup/dropoff locations, user details, driver assignment, and request status.
 */
class RideRequest {
public:
    enum class Status {
        PENDING,           // Request created, looking for driver
        DRIVER_NOTIFIED,   // Favorite driver has been notified
        ACCEPTED,          // Driver accepted the request
        REJECTED,          // Driver rejected the request
        CANCELLED,         // User cancelled the request
        IN_PROGRESS,       // Driver is en route or trip is active
        COMPLETED,         // Trip completed successfully
        FAILED            // Request failed (timeout, no drivers, etc.)
    };

    enum class RideType {
        STANDARD,
        PREMIUM,
        SHARED,
        XL
    };

    struct PaymentInfo {
        double estimatedFare;
        double actualFare;
        std::string paymentMethod;
        bool isPaid;
        
        PaymentInfo(double estFare = 0.0, const std::string& method = "card")
            : estimatedFare(estFare), actualFare(0.0), paymentMethod(method), isPaid(false) {}
    };

private:
    std::string m_requestId;
    std::string m_userId;
    std::string m_assignedDriverId;
    Driver::Location m_pickupLocation;
    Driver::Location m_dropoffLocation;
    std::string m_pickupAddress;
    std::string m_dropoffAddress;
    Status m_status;
    RideType m_rideType;
    PaymentInfo m_paymentInfo;
    std::chrono::system_clock::time_point m_requestTime;
    std::chrono::system_clock::time_point m_acceptedTime;
    std::chrono::system_clock::time_point m_completedTime;
    std::string m_specialInstructions;
    bool m_isFavoriteDriverRequest;
    std::string m_rejectionReason;
    int m_estimatedDurationMinutes;
    double m_estimatedDistanceKm;

public:
    // Constructors
    RideRequest();
    RideRequest(const std::string& userId, const Driver::Location& pickup, 
                const Driver::Location& dropoff, RideType type = RideType::STANDARD);
    
    // Destructor
    virtual ~RideRequest() = default;
    
    // Copy and move constructors/operators
    RideRequest(const RideRequest& other);
    RideRequest& operator=(const RideRequest& other);
    RideRequest(RideRequest&& other) noexcept;
    RideRequest& operator=(RideRequest&& other) noexcept;

    // Getters
    const std::string& getRequestId() const { return m_requestId; }
    const std::string& getUserId() const { return m_userId; }
    const std::string& getAssignedDriverId() const { return m_assignedDriverId; }
    const Driver::Location& getPickupLocation() const { return m_pickupLocation; }
    const Driver::Location& getDropoffLocation() const { return m_dropoffLocation; }
    const std::string& getPickupAddress() const { return m_pickupAddress; }
    const std::string& getDropoffAddress() const { return m_dropoffAddress; }
    Status getStatus() const { return m_status; }
    RideType getRideType() const { return m_rideType; }
    const PaymentInfo& getPaymentInfo() const { return m_paymentInfo; }
    std::chrono::system_clock::time_point getRequestTime() const { return m_requestTime; }
    std::chrono::system_clock::time_point getAcceptedTime() const { return m_acceptedTime; }
    std::chrono::system_clock::time_point getCompletedTime() const { return m_completedTime; }
    const std::string& getSpecialInstructions() const { return m_specialInstructions; }
    bool isFavoriteDriverRequest() const { return m_isFavoriteDriverRequest; }
    const std::string& getRejectionReason() const { return m_rejectionReason; }
    int getEstimatedDurationMinutes() const { return m_estimatedDurationMinutes; }
    double getEstimatedDistanceKm() const { return m_estimatedDistanceKm; }

    // Setters
    void setUserId(const std::string& userId) { m_userId = userId; }
    void setPickupLocation(const Driver::Location& location) { m_pickupLocation = location; }
    void setDropoffLocation(const Driver::Location& location) { m_dropoffLocation = location; }
    void setPickupAddress(const std::string& address) { m_pickupAddress = address; }
    void setDropoffAddress(const std::string& address) { m_dropoffAddress = address; }
    void setRideType(RideType type) { m_rideType = type; }
    void setSpecialInstructions(const std::string& instructions) { m_specialInstructions = instructions; }
    void setFavoriteDriverRequest(bool isFavorite) { m_isFavoriteDriverRequest = isFavorite; }
    void setEstimatedDuration(int minutes) { m_estimatedDurationMinutes = minutes; }
    void setEstimatedDistance(double km) { m_estimatedDistanceKm = km; }

    // Status management
    void setStatus(Status status);
    void assignDriver(const std::string& driverId);
    void acceptRequest();
    void rejectRequest(const std::string& reason = "");
    void cancelRequest();
    void completeRequest();
    void markInProgress();

    // Payment management
    void setEstimatedFare(double fare) { m_paymentInfo.estimatedFare = fare; }
    void setActualFare(double fare) { m_paymentInfo.actualFare = fare; }
    void setPaymentMethod(const std::string& method) { m_paymentInfo.paymentMethod = method; }
    void markAsPaid() { m_paymentInfo.isPaid = true; }

    // Utility methods
    std::string getStatusString() const;
    std::string getRideTypeString() const;
    bool isActive() const;
    bool isPending() const;
    bool isCompleted() const;
    bool isCancelled() const;
    double calculateDistance() const;
    std::chrono::minutes getWaitTime() const;
    std::chrono::minutes getTripDuration() const;
    std::chrono::minutes getTotalTime() const;

    // Business logic
    bool canBeAssigned() const;
    bool canBeCancelled() const;
    bool hasDriver() const;
    double getMultiplier() const; // Surge pricing multiplier
    void calculateEstimates();
    
    // Validation
    bool isValid() const;
    std::vector<std::string> validate() const;

    // Serialization
    std::string toJson() const;
    static RideRequest fromJson(const std::string& json);
    
    // Comparison operators
    bool operator==(const RideRequest& other) const;
    bool operator!=(const RideRequest& other) const;

private:
    // Helper methods
    std::string generateRequestId() const;
    void updateTimestamp(Status status);
    double calculateBaseFare() const;
    double calculateSurgeFare() const;
};

#endif // RIDE_REQUEST_H
