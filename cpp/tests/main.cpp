#include "Driver.h"
#include "FavoriteDriverManager.h"
#include "RideRequest.h"
#include <iostream>
#include <cassert>
#include <thread>
#include <chrono>

void testDriverBasicFunctionality() {
    std::cout << "Testing Driver basic functionality..." << std::endl;
    
    // Test driver creation
    Driver driver("test_001", "John Doe", "+1234567890");
    assert(driver.getId() == "test_001");
    assert(driver.getName() == "John Doe");
    assert(driver.getPhoneNumber() == "+1234567890");
    assert(driver.getRating() == 5.0); // Default rating
    assert(driver.getCompletedTrips() == 0);
    assert(!driver.isOnline());
    
    // Test driver status changes
    driver.goOnline();
    assert(driver.isOnline());
    assert(driver.isAvailable());
    
    driver.goOffline();
    assert(!driver.isOnline());
    assert(!driver.isAvailable());
    
    // Test location updates
    driver.updateLocation(37.7749, -122.4194);
    auto location = driver.getCurrentLocation();
    assert(location.latitude == 37.7749);
    assert(location.longitude == -122.4194);
    
    // Test vehicle information
    Driver::Vehicle vehicle("Toyota", "Camry", "Silver", "ABC-123", 2020);
    driver.setVehicle(vehicle);
    auto driverVehicle = driver.getVehicle();
    assert(driverVehicle.make == "Toyota");
    assert(driverVehicle.model == "Camry");
    assert(driverVehicle.color == "Silver");
    assert(driverVehicle.plateNumber == "ABC-123");
    assert(driverVehicle.year == 2020);
    
    // Test rating updates
    driver.updateRating(4.5);
    driver.incrementCompletedTrips();
    assert(driver.getCompletedTrips() == 1);
    
    driver.updateRating(5.0);
    driver.incrementCompletedTrips();
    assert(driver.getCompletedTrips() == 2);
    assert(driver.getRating() == 4.75); // Average of 4.5 and 5.0
    
    std::cout << "✓ Driver basic functionality tests passed" << std::endl;
}

void testRideRequestFunctionality() {
    std::cout << "Testing RideRequest functionality..." << std::endl;
    
    // Test ride request creation
    Driver::Location pickup(37.7749, -122.4194);
    Driver::Location dropoff(37.7849, -122.4094);
    RideRequest request("user_001", pickup, dropoff, RideRequest::RideType::STANDARD);
    
    assert(request.getUserId() == "user_001");
    assert(!request.getRequestId().empty());
    assert(request.getStatus() == RideRequest::Status::PENDING);
    assert(request.getRideType() == RideRequest::RideType::STANDARD);
    assert(request.isPending());
    assert(!request.isCompleted());
    assert(!request.isCancelled());
    
    // Test address setting
    request.setPickupAddress("123 Main St, San Francisco");
    request.setDropoffAddress("456 Market St, San Francisco");
    assert(request.getPickupAddress() == "123 Main St, San Francisco");
    assert(request.getDropoffAddress() == "456 Market St, San Francisco");
    
    // Test driver assignment
    request.assignDriver("driver_001");
    assert(request.getAssignedDriverId() == "driver_001");
    assert(request.hasDriver());
    assert(request.getStatus() == RideRequest::Status::DRIVER_NOTIFIED);
    
    // Test request acceptance
    request.acceptRequest();
    assert(request.getStatus() == RideRequest::Status::ACCEPTED);
    
    // Test request completion
    request.completeRequest();
    assert(request.getStatus() == RideRequest::Status::COMPLETED);
    assert(request.isCompleted());
    
    // Test distance calculation
    double distance = request.calculateDistance();
    assert(distance > 0.0);
    
    // Test validation
    assert(request.isValid());
    auto errors = request.validate();
    assert(errors.empty());
    
    std::cout << "✓ RideRequest functionality tests passed" << std::endl;
}

void testFavoriteDriverManager() {
    std::cout << "Testing FavoriteDriverManager functionality..." << std::endl;
    
    FavoriteDriverManager manager;
    
    // Create test drivers
    auto driver1 = std::make_shared<Driver>("driver_001", "Alice Smith", "+1111111111");
    driver1->setRating(4.8);
    driver1->goOnline();
    driver1->updateLocation(37.7749, -122.4194);
    
    auto driver2 = std::make_shared<Driver>("driver_002", "Bob Johnson", "+2222222222");
    driver2->setRating(4.9);
    driver2->goOnline();
    driver2->updateLocation(37.7849, -122.4094);
    
    auto driver3 = std::make_shared<Driver>("driver_003", "Carol Davis", "+3333333333");
    driver3->setRating(4.7);
    driver3->goOffline();
    driver3->updateLocation(37.7649, -122.4294);
    
    // Add drivers to manager
    assert(manager.addDriver(driver1));
    assert(manager.addDriver(driver2));
    assert(manager.addDriver(driver3));
    
    // Test favorite driver management
    std::string userId = "user_001";
    
    // Add favorite drivers
    assert(manager.addFavoriteDriver(userId, "driver_001"));
    assert(manager.addFavoriteDriver(userId, "driver_002"));
    assert(manager.addFavoriteDriver(userId, "driver_003"));
    
    // Test favorite driver retrieval
    auto favoriteDrivers = manager.getFavoriteDrivers(userId);
    assert(favoriteDrivers.size() == 3);
    
    // Test available favorite drivers (only online drivers)
    auto availableDrivers = manager.getAvailableFavoriteDrivers(userId);
    assert(availableDrivers.size() == 2); // driver1 and driver2 are online
    
    // Test is favorite driver
    assert(manager.isFavoriteDriver(userId, "driver_001"));
    assert(manager.isFavoriteDriver(userId, "driver_002"));
    assert(!manager.isFavoriteDriver(userId, "driver_999")); // Non-existent driver
    
    // Test favorite driver count
    assert(manager.getFavoriteDriverCount(userId) == 3);
    
    // Test removing favorite driver
    assert(manager.removeFavoriteDriver(userId, "driver_003"));
    assert(manager.getFavoriteDriverCount(userId) == 2);
    assert(!manager.isFavoriteDriver(userId, "driver_003"));
    
    // Test nearby drivers
    Driver::Location userLocation(37.7749, -122.4194);
    auto nearbyDrivers = manager.getNearbyDrivers(userLocation, 5.0); // 5km radius
    assert(!nearbyDrivers.empty());
    
    std::cout << "✓ FavoriteDriverManager functionality tests passed" << std::endl;
}

void testRideRequestFlow() {
    std::cout << "Testing complete ride request flow..." << std::endl;
    
    FavoriteDriverManager manager;
    
    // Setup test data
    auto driver = std::make_shared<Driver>("driver_001", "Test Driver", "+1234567890");
    driver->setRating(4.9);
    driver->goOnline();
    driver->updateLocation(37.7749, -122.4194);
    
    manager.addDriver(driver);
    
    std::string userId = "user_001";
    manager.addFavoriteDriver(userId, "driver_001");
    
    // Create ride request
    Driver::Location pickup(37.7749, -122.4194);
    Driver::Location dropoff(37.7849, -122.4094);
    RideRequest request(userId, pickup, dropoff);
    request.setPickupAddress("123 Main St");
    request.setDropoffAddress("456 Market St");
    
    // Test ride request with callback
    bool callbackCalled = false;
    std::string callbackMessage;
    
    auto callback = [&callbackCalled, &callbackMessage](bool accepted, const std::string& reason) {
        callbackCalled = true;
        callbackMessage = reason;
    };
    
    // Request favorite driver
    std::string requestId = manager.requestFavoriteDriver(userId, "driver_001", request, callback);
    assert(!requestId.empty());
    
    // Wait for async callback
    std::this_thread::sleep_for(std::chrono::seconds(3));
    
    // Check callback was called
    assert(callbackCalled);
    
    // Get the ride request
    auto rideRequest = manager.getRideRequest(requestId);
    assert(rideRequest != nullptr);
    
    std::cout << "✓ Complete ride request flow tests passed" << std::endl;
}

void testPerformance() {
    std::cout << "Testing performance with multiple drivers..." << std::endl;
    
    FavoriteDriverManager manager;
    
    // Create many drivers
    const int numDrivers = 1000;
    for (int i = 0; i < numDrivers; ++i) {
        auto driver = std::make_shared<Driver>(
            "driver_" + std::to_string(i),
            "Driver " + std::to_string(i),
            "+100000000" + std::to_string(i)
        );
        driver->setRating(4.0 + (i % 10) / 10.0); // Ratings between 4.0 and 4.9
        if (i % 2 == 0) driver->goOnline(); // Half online
        
        // Random locations around San Francisco
        double lat = 37.7749 + (rand() % 100 - 50) / 1000.0;
        double lng = -122.4194 + (rand() % 100 - 50) / 1000.0;
        driver->updateLocation(lat, lng);
        
        manager.addDriver(driver);
    }
    
    // Add many favorite drivers for a user
    std::string userId = "performance_user";
    for (int i = 0; i < 10; ++i) {
        manager.addFavoriteDriver(userId, "driver_" + std::to_string(i));
    }
    
    auto start = std::chrono::high_resolution_clock::now();
    
    // Test operations
    auto favoriteDrivers = manager.getFavoriteDrivers(userId);
    auto availableDrivers = manager.getAvailableFavoriteDrivers(userId);
    auto nearbyDrivers = manager.getNearbyDrivers(Driver::Location(37.7749, -122.4194), 10.0);
    
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    
    std::cout << "✓ Performance test completed in " << duration.count() << "ms" << std::endl;
    std::cout << "  - Found " << favoriteDrivers.size() << " favorite drivers" << std::endl;
    std::cout << "  - Found " << availableDrivers.size() << " available favorite drivers" << std::endl;
    std::cout << "  - Found " << nearbyDrivers.size() << " nearby drivers" << std::endl;
}

int main() {
    std::cout << "Running Uber Favorite Driver Feature Tests..." << std::endl;
    std::cout << "==============================================" << std::endl;
    
    try {
        testDriverBasicFunctionality();
        testRideRequestFunctionality();
        testFavoriteDriverManager();
        testRideRequestFlow();
        testPerformance();
        
        std::cout << std::endl;
        std::cout << "✅ All tests passed successfully!" << std::endl;
        std::cout << "==============================================" << std::endl;
        
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "❌ Test failed with exception: " << e.what() << std::endl;
        return 1;
    } catch (...) {
        std::cerr << "❌ Test failed with unknown exception" << std::endl;
        return 1;
    }
}
