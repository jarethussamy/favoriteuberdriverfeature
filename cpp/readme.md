# C++ Implementation - Uber Favorite Driver Feature

This directory contains a complete C++ implementation of the Uber Favorite Driver feature backend system.

## 🏗️ Architecture

The C++ implementation provides a robust backend system with the following components:

### Core Classes

- **`Driver`** - Represents a driver with all personal info, vehicle details, location, and status
- **`RideRequest`** - Handles ride requests with pickup/dropoff, pricing, and status management  
- **`FavoriteDriverManager`** - Manages user favorites, driver matching, and ride request processing

### Key Features

- **Thread-Safe Operations** - All operations are thread-safe using mutexes
- **Real-Time Location Tracking** - Haversine formula for accurate distance calculations
- **Dynamic Pricing** - Surge pricing based on time and demand
- **Async Request Processing** - Non-blocking ride request handling
- **Comprehensive Validation** - Input validation and error handling
- **JSON Serialization** - Export/import data in JSON format

## 📁 Directory Structure

```
cpp/
├── include/                 # Header files
│   ├── Driver.h            # Driver class definition
│   ├── FavoriteDriverManager.h  # Manager class definition
│   └── RideRequest.h       # Ride request class definition
├── src/                    # Source files
│   ├── Driver.cpp          # Driver implementation
│   ├── FavoriteDriverManager.cpp  # Manager implementation
│   └── RideRequest.cpp     # Ride request implementation
├── tests/                  # Test files
│   └── main.cpp           # Comprehensive test suite
├── examples/               # Example usage
│   └── main.cpp           # Demo application
└── README.md              # This file
```

## 🛠️ Building the Project

### Prerequisites

- **C++17 compatible compiler** (GCC 7+, Clang 5+, MSVC 2017+)
- **CMake 3.12+**
- **Standard C++ libraries**

### Build Instructions

1. **Create build directory:**
   ```bash
   mkdir build && cd build
   ```

2. **Configure with CMake:**
   ```bash
   cmake ..
   ```

3. **Build the project:**
   ```bash
   cmake --build .
   ```

4. **Run tests:**
   ```bash
   ./UberFavoriteDriverTest
   ```

5. **Run examples:**
   ```bash
   ./UberFavoriteDriverExample
   ```

### Build Options

- `BUILD_TESTS=ON/OFF` - Enable/disable test executable (default: ON)
- `BUILD_EXAMPLES=ON/OFF` - Enable/disable example executable (default: ON)

Example with custom options:
```bash
cmake -DBUILD_TESTS=OFF -DBUILD_EXAMPLES=ON ..
```

## 🚀 Usage Examples

### Basic Driver Creation

```cpp
#include "Driver.h"

// Create a driver
Driver driver("driver_001", "Marcus Johnson", "+1234567890");
driver.setRating(4.95);
driver.setVehicle(Driver::Vehicle("Toyota", "Camry", "Silver", "ABC-123", 2020));
driver.goOnline();
driver.updateLocation(37.7749, -122.4194);
```

### Favorite Driver Management

```cpp
#include "FavoriteDriverManager.h"

FavoriteDriverManager manager;

// Add drivers to system
auto driver = std::make_shared<Driver>("driver_001", "Marcus Johnson", "+1234567890");
manager.addDriver(driver);

// User adds favorite driver
std::string userId = "user_123";
manager.addFavoriteDriver(userId, "driver_001");

// Get available favorite drivers
auto availableDrivers = manager.getAvailableFavoriteDrivers(userId);
```

### Ride Request Processing

```cpp
#include "RideRequest.h"

// Create ride request
Driver::Location pickup(37.7749, -122.4194);
Driver::Location dropoff(37.7849, -122.4094);
RideRequest request(userId, pickup, dropoff);

// Request favorite driver
auto callback = [](bool accepted, const std::string& reason) {
    if (accepted) {
        std::cout << "Driver accepted: " << reason << std::endl;
    } else {
        std::cout << "Driver declined: " << reason << std::endl;
    }
};

std::string requestId = manager.requestFavoriteDriver(userId, "driver_001", request, callback);
```

## 🧪 Testing

The test suite (`cpp/tests/main.cpp`) includes comprehensive tests for:

- **Driver functionality** - Creation, status changes, location updates
- **Ride request lifecycle** - Creation, validation, status transitions
- **Favorite driver management** - Adding, removing, filtering
- **Complete request flow** - End-to-end ride request processing
- **Performance testing** - Operations with thousands of drivers
- **Error handling** - Edge cases and invalid inputs

Run tests with:
```bash
./UberFavoriteDriverTest
```

## 📊 Performance Characteristics

- **Driver lookup**: O(1) average case with hash maps
- **Distance calculations**: O(n) for filtering nearby drivers
- **Favorite driver operations**: O(1) for add/remove, O(k) for retrieval where k = favorites count
- **Memory usage**: Approximately 1KB per driver object
- **Thread safety**: Full thread safety with minimal lock contention

## 🔧 Configuration Options

The system supports various configuration parameters:

```cpp
// Maximum favorite drivers per user (default: 10)
manager.setMaxFavoriteDrivers(15);

// Request timeout in seconds (default: 30)
manager.setRequestTimeout(45);

// Maximum pickup distance in km (default: 15.0)
manager.setMaxPickupDistance(20.0);
```

## 📡 Integration Points

### Database Integration
The classes support serialization for database storage:

```cpp
// Serialize driver to JSON
std::string driverJson = driver.toJson();

// Restore from JSON
Driver restoredDriver = Driver::fromJson(driverJson);
```

### API Integration
The manager can be integrated with REST APIs:

```cpp
// Set notification callback for real-time updates
manager.setNotificationCallback([](const std::string& userId, const std::string& message) {
    // Send push notification to user
    sendPushNotification(userId, message);
});
```

### Real-time Updates
The system supports real-time location and status updates:

```cpp
// Update driver location (typically called from GPS tracking)
driver->updateLocation(newLatitude, newLongitude);

// Change driver status
driver->setStatus(Driver::Status::BUSY);
```

## 🔒 Security Considerations

- **Input validation** on all user inputs
- **Rate limiting** for ride requests (configurable)
- **Sanitized JSON output** to prevent injection attacks
- **Thread-safe operations** to prevent race conditions

## 🚀 Production Considerations

### Scalability
- Use connection pooling for database operations
- Implement caching for frequently accessed data
- Consider horizontal scaling with consistent hashing

### Monitoring
- Add logging for all major operations
- Implement metrics collection for performance monitoring
- Set up alerts for error conditions

### Deployment
- Build as static library for integration
- Support for Docker containerization
- Configuration via environment variables

## 📈 Future Enhancements

- **Machine Learning Integration** - Predictive driver matching
- **Advanced Routing** - Integration with mapping services
- **Real-time Analytics** - Performance metrics and insights
- **Multi-language Support** - Internationalization
- **Advanced Pricing Models** - Dynamic surge pricing algorithms

## 🤝 Contributing

1. Follow C++17 standards and best practices
2. Add tests for any new functionality
3. Update documentation for API changes
4. Use RAII and smart pointers for memory management
5. Ensure thread safety for all public methods

## 📄 License

This C++ implementation is part of the Uber Favorite Driver Feature project and follows the same MIT license as the main project.
