#include "Driver.h"
#include "FavoriteDriverManager.h"
#include "RideRequest.h"
#include <iostream>
#include <thread>
#include <chrono>

void printSeparator(const std::string& title) {
    std::cout << "\n" << std::string(50, '=') << std::endl;
    std::cout << "  " << title << std::endl;
    std::cout << std::string(50, '=') << std::endl;
}

void demonstrateDriverCreation() {
    printSeparator("Driver Creation and Management");
    
    // Create a driver
    Driver driver("driver_001", "Marcus Johnson", "+1-555-0123");
    
    std::cout << "Created driver: " << driver.getName() << std::endl;
    std::cout << "Driver ID: " << driver.getId() << std::endl;
    std::cout << "Phone: " << driver.getPhoneNumber() << std::endl;
    std::cout << "Initial rating: " << driver.getRating() << std::endl;
    std::cout << "Status: " << driver.getStatusString() << std::endl;
    
    // Set vehicle information
    Driver::Vehicle vehicle("Toyota", "Camry", "Silver", "ABC-123", 2020);
    driver.setVehicle(vehicle);
    std::cout << "\nVehicle: " << driver.getVehicle().year << " " 
              << driver.getVehicle().color << " " 
              << driver.getVehicle().make << " " 
              << driver.getVehicle().model << std::endl;
    std::cout << "License plate: " << driver.getVehicle().plateNumber << std::endl;
    
    // Update driver status and location
    driver.goOnline();
    driver.updateLocation(37.7749, -122.4194); // San Francisco
    std::cout << "\nDriver went online at location: " 
              << driver.getCurrentLocation().latitude << ", " 
              << driver.getCurrentLocation().longitude << std::endl;
    std::cout << "Status: " << driver.getStatusString() << std::endl;
    
    // Simulate some trips and rating updates
    std::cout << "\nSimulating trips and ratings..." << std::endl;
    driver.updateRating(4.8);
    driver.incrementCompletedTrips();
    std::cout << "After trip 1 - Rating: " << driver.getRating() 
              << ", Completed trips: " << driver.getCompletedTrips() << std::endl;
    
    driver.updateRating(5.0);
    driver.incrementCompletedTrips();
    std::cout << "After trip 2 - Rating: " << driver.getRating() 
              << ", Completed trips: " << driver.getCompletedTrips() << std::endl;
    
    driver.updateRating(4.5);
    driver.incrementCompletedTrips();
    std::cout << "After trip 3 - Rating: " << driver.getRating() 
              << ", Completed trips: " << driver.getCompletedTrips() << std::endl;
}

void demonstrateFavoriteDriverSystem() {
    printSeparator("Favorite Driver System");
    
    FavoriteDriverManager manager;
    
    // Set up notification callback
    manager.setNotificationCallback([](const std::string& userId, const std::string& message) {
        std::cout << "[NOTIFICATION for " << userId << "]: " << message << std::endl;
    });
    
    // Create multiple drivers
    auto driver1 = std::make_shared<Driver>("driver_001", "Marcus Johnson", "+1-555-0123");
    driver1->setRating(4.95);
    driver1->setVehicle(Driver::Vehicle("Toyota", "Camry", "Silver", "ABC-123", 2020));
    driver1->goOnline();
    driver1->updateLocation(37.7749, -122.4194);
    
    auto driver2 = std::make_shared<Driver>("driver_002", "Sarah Chen", "+1-555-0124");
    driver2->setRating(4.92);
    driver2->setVehicle(Driver::Vehicle("Honda", "Accord", "Black", "XYZ-789", 2021));
    driver2->goOffline(); // This driver is offline
    driver2->updateLocation(37.7849, -122.4094);
    
    auto driver3 = std::make_shared<Driver>("driver_003", "David Rodriguez", "+1-555-0125");
    driver3->setRating(4.98);
    driver3->setVehicle(Driver::Vehicle("Nissan", "Altima", "White", "DEF-456", 2019));
    driver3->goOnline();
    driver3->updateLocation(37.7649, -122.4294);
    
    // Add drivers to manager
    manager.addDriver(driver1);
    manager.addDriver(driver2);
    manager.addDriver(driver3);
    
    std::cout << "Added 3 drivers to the system" << std::endl;
    
    // User adds favorite drivers
    std::string userId = "user_alex";
    std::cout << "\nUser " << userId << " adding favorite drivers..." << std::endl;
    
    manager.addFavoriteDriver(userId, "driver_001");
    manager.addFavoriteDriver(userId, "driver_002");
    manager.addFavoriteDriver(userId, "driver_003");
    
    // Display favorite drivers
    auto favoriteDrivers = manager.getFavoriteDrivers(userId);
    std::cout << "\nFavorite drivers for " << userId << ":" << std::endl;
    for (const auto& driver : favoriteDrivers) {
        std::cout << "  - " << driver->getName() 
                  << " (Rating: " << driver->getRating() 
                  << ", Status: " << driver->getStatusString() << ")" << std::endl;
    }
    
    // Display available favorite drivers
    auto availableDrivers = manager.getAvailableFavoriteDrivers(userId);
    std::cout << "\nAvailable favorite drivers:" << std::endl;
    for (const auto& driver : availableDrivers) {
        std::cout << "  - " << driver->getName() 
                  << " (Rating: " << driver->getRating() 
                  << ", ETA: " << driver->getEstimatedArrivalTime(Driver::Location(37.7749, -122.4194)) 
                  << " minutes)" << std::endl;
    }
}

void demonstrateRideRequestFlow() {
    printSeparator("Ride Request Flow");
    
    FavoriteDriverManager manager;
    
    // Create and add a driver
    auto driver = std::make_shared<Driver>("driver_001", "Marcus Johnson", "+1-555-0123");
    driver->setRating(4.95);
    driver->goOnline();
    driver->updateLocation(37.7749, -122.4194);
    manager.addDriver(driver);
    
    // User setup
    std::string userId = "user_alex";
    manager.addFavoriteDriver(userId, "driver_001");
    
    std::cout << "User " << userId << " wants to request a ride..." << std::endl;
    
    // Create ride request
    Driver::Location pickup(37.7749, -122.4194);   // Current location
    Driver::Location dropoff(37.7849, -122.4094);  // Destination
    
    RideRequest request(userId, pickup, dropoff, RideRequest::RideType::STANDARD);
    request.setPickupAddress("123 Main Street, San Francisco, CA");
    request.setDropoffAddress("456 Market Street, San Francisco, CA");
    request.setSpecialInstructions("Please call when you arrive");
    
    std::cout << "\nRide request details:" << std::endl;
    std::cout << "  Pickup: " << request.getPickupAddress() << std::endl;
    std::cout << "  Dropoff: " << request.getDropoffAddress() << std::endl;
    st
