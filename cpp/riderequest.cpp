#include "RideRequest.h"
#include <random>
#include <sstream>
#include <iomanip>
#include <cmath>

// Default constructor
RideRequest::RideRequest()
    : m_requestId(generateRequestId()), m_userId(""), m_assignedDriverId(""),
      m_pickupLocation(), m_dropoffLocation(), m_pickupAddress(""), m_dropoffAddress(""),
      m_status(Status::PENDING), m_rideType(RideType::STANDARD), m_paymentInfo(),
      m_requestTime(std::chrono::system_clock::now()), m_acceptedTime(), m_completedTime(),
      m_specialInstructions(""), m_isFavoriteDriverRequest(false), m_rejectionReason(""),
      m_estimatedDurationMinutes(0), m_estimatedDistanceKm(0.0) {
}

// Parameterized constructor
RideRequest::RideRequest(const std::string& userId, const Driver::Location& pickup,
                        const Driver::Location& dropoff, RideType type)
    : m_requestId(generateRequestId()), m_userId(userId), m_assignedDriverId(""),
      m_pickupLocation(pickup), m_dropoffLocation(dropoff), m_pickupAddress(""), m_dropoffAddress(""),
      m_status(Status::PENDING), m_rideType(type), m_paymentInfo(),
      m_requestTime(std::chrono::system_clock::now()), m_acceptedTime(), m_completedTime(),
      m_specialInstructions(""), m_isFavoriteDriverRequest(false), m_rejectionReason(""),
      m_estimatedDurationMinutes(0), m_estimatedDistanceKm(0.0) {
    
    calculateEstimates();
}

// Copy constructor
RideRequest::RideRequest(const RideRequest& other)
    : m_requestId(other.m_requestId), m_userId(other.m_userId), m_assignedDriverId(other.m_assignedDriverId),
      m_pickupLocation(other.m_pickupLocation), m_dropoffLocation(other.m_dropoffLocation),
      m_pickupAddress(other.m_pickupAddress), m_dropoffAddress(other.m_dropoffAddress),
      m_status(other.m_status), m_rideType(other.m_rideType), m_paymentInfo(other.m_paymentInfo),
      m_requestTime(other.m_requestTime), m_acceptedTime(other.m_acceptedTime), m_completedTime(other.m_completedTime),
      m_specialInstructions(other.m_specialInstructions), m_isFavoriteDriverRequest(other.m_isFavoriteDriverRequest),
      m_rejectionReason(other.m_rejectionReason), m_estimatedDurationMinutes(other.m_estimatedDurationMinutes),
      m_estimatedDistanceKm(other.m_estimatedDistanceKm) {
}

// Copy assignment operator
RideRequest& RideRequest::operator=(const RideRequest& other) {
    if (this != &other) {
        m_requestId = other.m_requestId;
        m_userId = other.m_userId;
        m_assignedDriverId = other.m_assignedDriverId;
        m_pickupLocation = other.m_pickupLocation;
        m_dropoffLocation = other.m_dropoffLocation;
        m_pickupAddress = other.m_pickupAddress;
        m_dropoffAddress = other.m_dropoffAddress;
        m_status = other.m_status;
        m_rideType = other.m_rideType;
        m_paymentInfo = other.m_paymentInfo;
        m_requestTime = other.m_requestTime;
        m_acceptedTime = other.m_acceptedTime;
        m_completedTime = other.m_completedTime;
        m_specialInstructions = other.m_specialInstructions;
        m_isFavoriteDriverRequest = other.m_isFavoriteDriverRequest;
        m_rejectionReason = other.m_rejectionReason;
        m_estimatedDurationMinutes = other.m_estimatedDurationMinutes;
        m_estimatedDistanceKm = other.m_estimatedDistanceKm;
    }
    return *this;
}

// Move constructor
RideRequest::RideRequest(RideRequest&& other) noexcept
    : m_requestId(std::move(other.m_requestId)), m_userId(std::move(other.m_userId)),
      m_assignedDriverId(std::move(other.m_assignedDriverId)), m_pickupLocation(std::move(other.m_pickupLocation)),
      m_dropoffLocation(std::move(other.m_dropoffLocation)), m_pickupAddress(std::move(other.m_pickupAddress)),
      m_dropoffAddress(std::move(other.m_dropoffAddress)), m_status(other.m_status), m_rideType(other.m_rideType),
      m_paymentInfo(std::move(other.m_paymentInfo)), m_requestTime(other.m_requestTime),
      m_acceptedTime(other.m_acceptedTime), m_completedTime(other.m_completedTime),
      m_specialInstructions(std::move(other.m_specialInstructions)), m_isFavoriteDriverRequest(other.m_isFavoriteDriverRequest),
      m_rejectionReason(std::move(other.m_rejectionReason)), m_estimatedDurationMinutes(other.m_estimatedDurationMinutes),
      m_estimatedDistanceKm(other.m_estimatedDistanceKm) {
}

// Move assignment operator
RideRequest& RideRequest::operator=(RideRequest&& other) noexcept {
    if (this != &other) {
        m_requestId = std::move(other.m_requestId);
        m_userId = std::move(other.m_userId);
        m_assignedDriverId = std::move(other.m_assignedDriverId);
        m_pickupLocation = std::move(other.m_pickupLocation);
        m_dropoffLocation = std::move(other.m_dropoffLocation);
        m_pickupAddress = std::move(other.m_pickupAddress);
        m_dropoffAddress = std::move(other.m_dropoffAddress);
        m_status = other.m_status;
        m_rideType = other.m_rideType;
        m_paymentInfo = std::move(other.m_paymentInfo);
        m_requestTime = other.m_requestTime;
        m_acceptedTime = other.m_acceptedTime;
        m_completedTime = other.m_completedTime;
        m_specialInstructions = std::move(other.m_specialInstructions);
        m_isFavoriteDriverRequest = other.m_isFavoriteDriverRequest;
        m_rejectionReason = std::move(other.m_rejectionReason);
        m_estimatedDurationMinutes = other.m_estimatedDurationMinutes;
        m_estimatedDistanceKm = other.m_estimatedDistanceKm;
    }
    return *this;
}

// Status management
void RideRequest::setStatus(Status status) {
    m_status = status;
    updateTimestamp(status);
}

void RideRequest::assignDriver(const std::string& driverId) {
    m_assignedDriverId = driverId;
    m_status = Status::DRIVER_NOTIFIED;
    updateTimestamp(m_status);
}

void RideRequest::acceptRequest() {
    m_status = Status::ACCEPTED;
    updateTimestamp(m_status);
}

void RideRequest::rejectRequest(const std::string& reason) {
    m_status = Status::REJECTED;
    m_rejectionReason = reason;
    updateTimestamp(m_status);
}

void RideRequest::cancelRequest() {
    m_status = Status::CANCELLED;
    updateTimestamp(m_status);
}

void RideRequest::completeRequest() {
    m_status = Status::COMPLETED;
    updateTimestamp(m_status);
}

void RideRequest::markInProgress() {
    m_status = Status::IN_PROGRESS;
    updateTimestamp(m_status);
}

// Utility methods
std::string RideRequest::getStatusString() const {
    switch (m_status) {
        case Status::PENDING: return "Pending";
        case Status::DRIVER_NOTIFIED: return "Driver Notified";
        case Status::ACCEPTED: return "Accepted";
        case Status::REJECTED: return "Rejected";
        case Status::CANCELLED: return "Cancelled";
        case Status::IN_PROGRESS: return "In Progress";
        case Status::COMPLETED: return "Completed";
        case Status::FAILED: return "Failed";
        default: return "Unknown";
    }
}

std::string RideRequest::getRideTypeString() const {
    switch (m_rideType) {
        case RideType::STANDARD: return "Standard";
        case RideType::PREMIUM: return "Premium";
        case RideType::SHARED: return "Shared";
        case RideType::XL: return "XL";
        default: return "Unknown";
    }
}

bool RideRequest::isActive() const {
    return m_status == Status::PENDING || m_status == Status::DRIVER_NOTIFIED ||
           m_status == Status::ACCEPTED || m_status == Status::IN_PROGRESS;
}

bool RideRequest::isPending() const {
    return m_status == Status::PENDING || m_status == Status::DRIVER_NOTIFIED;
}

bool RideRequest::isCompleted() const {
    return m_status == Status::COMPLETED;
}

bool RideRequest::isCancelled() const {
    return m_status == Status::CANCELLED;
}

double RideRequest::calculateDistance() const {
    // Haversine formula for calculating distance between two points on Earth
    const double R = 6371.0; // Earth's radius in kilometers
    
    double lat1 = m_pickupLocation.latitude * M_PI / 180.0;
    double lat2 = m_dropoffLocation.latitude * M_PI / 180.0;
    double deltaLat = (m_dropoffLocation.latitude - m_pickupLocation.latitude) * M_PI / 180.0;
    double deltaLng = (m_dropoffLocation.longitude - m_pickupLocation.longitude) * M_PI / 180.0;
    
    double a = sin(deltaLat / 2) * sin(deltaLat / 2) +
               cos(lat1) * cos(lat2) * sin(deltaLng / 2) * sin(deltaLng / 2);
    double c = 2 * atan2(sqrt(a), sqrt(1 - a));
    
    return R * c;
}

std::chrono::minutes RideRequest::getWaitTime() const {
    if (m_status == Status::PENDING || m_status == Status::DRIVER_NOTIFIED) {
        auto now = std::chrono::system_clock::now();
        return std::chrono::duration_cast<std::chrono::minutes>(now - m_requestTime);
    } else if (m_acceptedTime != std::chrono::system_clock::time_point{}) {
        return std::chrono::duration_cast<std::chrono::minutes>(m_acceptedTime - m_requestTime);
    }
    return std::chrono::minutes(0);
}

std::chrono::minutes RideRequest::getTripDuration() const {
    if (m_status == Status::COMPLETED && 
        m_acceptedTime != std::chrono::system_clock::time_point{} &&
        m_completedTime != std::chrono::system_clock::time_point{}) {
        return std::chrono::duration_cast<std::chrono::minutes>(m_completedTime - m_acceptedTime);
    }
    return std::chrono::minutes(0);
}

std::chrono::minutes RideRequest::getTotalTime() const {
    if (m_status == Status::COMPLETED && 
        m_completedTime != std::chrono::system_clock::time_point{}) {
        return std::chrono::duration_cast<std::chrono::minutes>(m_completedTime - m_requestTime);
    }
    auto now = std::chrono::system_clock::now();
    return std::chrono::duration_cast<std::chrono::minutes>(now - m_requestTime);
}

// Business logic
bool RideRequest::canBeAssigned() const {
    return m_status == Status::PENDING;
}

bool RideRequest::canBeCancelled() const {
    return m_status == Status::PENDING || m_status == Status::DRIVER_NOTIFIED || m_status == Status::ACCEPTED;
}

bool RideRequest::hasDriver() const {
    return !m_assignedDriverId.empty();
}

double RideRequest::getMultiplier() const {
    // Simple surge pricing logic
    auto now = std::chrono::system_clock::now();
    auto timeT = std::chrono::system_clock::to_time_t(now);
    auto localTime = *std::localtime(&timeT);
    
    // Higher multiplier during peak hours
    if ((localTime.tm_hour >= 7 && localTime.tm_hour <= 9) ||
        (localTime.tm_hour >= 17 && localTime.tm_hour <= 19)) {
        return 1.5; // 1.5x during rush hours
    }
    
    return 1.0; // Normal pricing
}

void RideRequest::calculateEstimates() {
    m_estimatedDistanceKm = calculate
