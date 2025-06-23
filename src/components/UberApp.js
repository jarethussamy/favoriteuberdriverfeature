import React, { useState, useEffect } from 'react';
import { Heart, Star, MapPin, Clock, User, Car, Phone, MessageCircle, Navigation } from 'lucide-react';

const UberApp = () => {
  const [currentView, setCurrentView] = useState('home');
  const [favoriteDrivers, setFavoriteDrivers] = useState([
    {
      id: 1,
      name: 'Marcus Johnson',
      rating: 4.95,
      completedRides: 1247,
      carModel: 'Toyota Camry',
      carColor: 'Silver',
      plateNumber: 'ABC-123',
      photo: '👨🏿‍💼',
      isOnline: true,
      estimatedArrival: '3 min',
      location: '0.5 miles away'
    },
    {
      id: 2,
      name: 'Sarah Chen',
      rating: 4.92,
      completedRides: 892,
      carModel: 'Honda Accord',
      carColor: 'Black',
      plateNumber: 'XYZ-789',
      photo: '👩🏻‍💼',
      isOnline: false,
      lastSeen: '2 hours ago',
      location: 'Offline'
    },
    {
      id: 3,
      name: 'David Rodriguez',
      rating: 4.98,
      completedRides: 2156,
      carModel: 'Nissan Altima',
      carColor: 'White',
      plateNumber: 'DEF-456',
      photo: '👨🏽‍💼',
      isOnline: true,
      estimatedArrival: '7 min',
      location: '1.2 miles away'
    }
  ]);
  
  const [selectedDriver, setSelectedDriver] = useState(null);
  const [rideRequest, setRideRequest] = useState(null);

  const handleRequestFavoriteDriver = (driver) => {
    if (driver.isOnline) {
      setSelectedDriver(driver);
      setRideRequest({
        driver: driver,
        pickup: '123 Main St, San Francisco',
        destination: '456 Market St, San Francisco',
        estimatedFare: '$12.50',
        estimatedTime: '15 min'
      });
      setCurrentView('booking');
    }
  };

  const confirmRide = () => {
    setCurrentView('waiting');
    // Simulate driver acceptance
    setTimeout(() => {
      setCurrentView('driverEnRoute');
    }, 3000);
  };

  const renderHomeView = () => (
    <div className="space-y-6 animate-fade-in">
      <div className="bg-gradient-to-r from-black to-gray-800 rounded-2xl p-6 text-white">
        <h2 className="text-2xl font-bold mb-2">Good afternoon, Alex!</h2>
        <p className="text-gray-300">Where would you like to go?</p>
      </div>

      <div className="space-y-4">
        <div className="flex items-center justify-between">
          <h3 className="text-xl font-semibold">Your Favorite Drivers</h3>
          <Heart className="w-6 h-6 text-red-500 fill-current" />
        </div>
        
        {favoriteDrivers.map(driver => (
          <div key={driver.id} className="bg-white rounded-xl p-4 shadow-uber border border-gray-100 animate-slide-up">
            <div className="flex items-center justify-between mb-3">
              <div className="flex items-center space-x-3">
                <div className="text-3xl">{driver.photo}</div>
                <div>
                  <h4 className="font-semibold text-lg">{driver.name}</h4>
                  <div className="flex items-center space-x-2 text-sm text-gray-600">
                    <Star className="w-4 h-4 fill-yellow-400 text-yellow-400" />
                    <span>{driver.rating}</span>
                    <span>•</span>
                    <span>{driver.completedRides} rides</span>
                  </div>
                </div>
              </div>
              <div className={`px-3 py-1 rounded-full text-sm font-medium transition-colors ${
                driver.isOnline 
                  ? 'bg-green-100 text-green-800' 
                  : 'bg-gray-100 text-gray-600'
              }`}>
                {driver.isOnline ? 'Online' : 'Offline'}
              </div>
            </div>
            
            <div className="flex items-center justify-between text-sm text-gray-600 mb-3">
              <div className="flex items-center space-x-1">
                <Car className="w-4 h-4" />
                <span>{driver.carModel} • {driver.carColor}</span>
              </div>
              <span>{driver.plateNumber}</span>
            </div>
            
            <div className="flex items-center justify-between">
              <div className="flex items-center space-x-1 text-sm text-gray-600">
                <MapPin className="w-4 h-4" />
                <span>{driver.location}</span>
              </div>
              {driver.isOnline && (
                <button
                  onClick={() => handleRequestFavoriteDriver(driver)}
                  className="bg-black text-white px-4 py-2 rounded-lg font-medium hover:bg-gray-800 transition-colors transform hover:scale-105"
                >
                  Request • {driver.estimatedArrival}
                </button>
              )}
              {!driver.isOnline && (
                <span className="text-sm text-gray-500">Last seen: {driver.lastSeen}</span>
              )}
            </div>
          </div>
        ))}
      </div>

      <button 
        className="w-full bg-gray-100 text-gray-700 py-4 rounded-xl font-medium hover:bg-gray-200 transition-colors"
        onClick={() => setCurrentView('regular')}
      >
        Request Any Available Driver
      </button>
    </div>
  );

  const renderBookingView = () => (
    <div className="space-y-6 animate-fade-in">
      <div className="flex items-center space-x-3 mb-6">
        <button 
          onClick={() => setCurrentView('home')}
          className="text-gray-600 hover:text-gray-800 transition-colors"
        >
          ←
        </button>
        <h2 className="text-xl font-semibold">Confirm Your Ride</h2>
      </div>

      <div className="bg-blue-50 border border-blue-200 rounded-xl p-4 animate-slide-up">
        <div className="flex items-center space-x-2 mb-2">
          <Heart className="w-5 h-5 text-red-500 fill-current" />
          <span className="font-medium text-blue-800">Requesting Favorite Driver</span>
        </div>
        <p className="text-sm text-blue-700">
          {selectedDriver?.name} will be notified of your ride request first
        </p>
      </div>

      <div className="bg-white rounded-xl p-4 shadow-uber animate-slide-up">
        <div className="flex items-center space-x-4 mb-4">
          <div className="text-4xl">{selectedDriver?.photo}</div>
          <div className="flex-1">
            <h3 className="font-semibold text-lg">{selectedDriver?.name}</h3>
            <div className="flex items-center space-x-2 text-sm text-gray-600">
              <Star className="w-4 h-4 fill-yellow-400 text-yellow-400" />
              <span>{selectedDriver?.rating}</span>
              <span>•</span>
              <span>{selectedDriver?.completedRides} rides</span>
            </div>
            <div className="text-sm text-gray-600 mt-1">
              {selectedDriver?.carModel} • {selectedDriver?.carColor} • {selectedDriver?.plateNumber}
            </div>
          </div>
        </div>
      </div>

      <div className="space-y-4 animate-slide-up">
        <div className="flex items-center space-x-3">
          <div className="w-3 h-3 bg-green-500 rounded-full"></div>
          <span className="text-sm text-gray-600">Pickup</span>
        </div>
        <p className="font-medium ml-6">{rideRequest?.pickup}</p>
        
        <div className="flex items-center space-x-3">
          <div className="w-3 h-3 bg-red-500 rounded-full"></div>
          <span className="text-sm text-gray-600">Drop-off</span>
        </div>
        <p className="font-medium ml-6">{rideRequest?.destination}</p>
      </div>

      <div className="bg-gray-50 rounded-xl p-4 animate-slide-up">
        <div className="flex justify-between items-center mb-2">
          <span className="font-medium">Estimated fare</span>
          <span className="font-semibold text-lg">{rideRequest?.estimatedFare}</span>
        </div>
        <div className="flex justify-between items-center">
          <span className="text-sm text-gray-600">Estimated time</span>
          <span className="text-sm text-gray-600">{rideRequest?.estimatedTime}</span>
        </div>
      </div>

      <button
        onClick={confirmRide}
        className="w-full bg-black text-white py-4 rounded-xl font-medium text-lg hover:bg-gray-800 transition-colors transform hover:scale-105"
      >
        Confirm Ride with {selectedDriver?.name}
      </button>
    </div>
  );

  const renderWaitingView = () => (
    <div className="space-y-6 text-center animate-fade-in">
      <div className="bg-yellow-50 border border-yellow-200 rounded-xl p-6">
        <Clock className="w-12 h-12 text-yellow-600 mx-auto mb-4 animate-pulse-slow" />
        <h2 className="text-xl font-semibold mb-2">Requesting {selectedDriver?.name}</h2>
        <p className="text-gray-600">
          We're checking if your favorite driver is available to accept your ride...
        </p>
      </div>
      
      <div className="animate-pulse">
        <div className="bg-gray-200 h-2 rounded-full mb-4"></div>
        <p className="text-sm text-gray-500">This usually takes 10-30 seconds</p>
      </div>
    </div>
  );

  const renderDriverEnRouteView = () => (
    <div className="space-y-6 animate-fade-in">
      <div className="bg-green-50 border border-green-200 rounded-xl p-4 text-center animate-slide-up">
        <h2 className="text-lg font-semibold text-green-800 mb-1">
          {selectedDriver?.name} accepted your ride!
        </h2>
        <p className="text-green-700">Arriving in {selectedDriver?.estimatedArrival}</p>
      </div>

      <div className="bg-white rounded-xl p-4 shadow-uber animate-slide-up">
        <div className="flex items-center justify-between mb-4">
          <div className="flex items-center space-x-3">
            <div className="text-3xl">{selectedDriver?.photo}</div>
            <div>
              <h3 className="font-semibold">{selectedDriver?.name}</h3>
              <div className="flex items-center space-x-1 text-sm text-gray-600">
                <Star className="w-4 h-4 fill-yellow-400 text-yellow-400" />
                <span>{selectedDriver?.rating}</span>
              </div>
            </div>
          </div>
          <div className="flex space-x-3">
            <button className="p-2 bg-gray-100 rounded-full hover:bg-gray-200 transition-colors">
              <Phone className="w-5 h-5" />
            </button>
            <button className="p-2 bg-gray-100 rounded-full hover:bg-gray-200 transition-colors">
              <MessageCircle className="w-5 h-5" />
            </button>
          </div>
        </div>
        
        <div className="text-center mb-4">
          <p className="font-medium">{selectedDriver?.carModel}</p>
          <p className="text-gray-600">{selectedDriver?.carColor} • {selectedDriver?.plateNumber}</p>
        </div>
        
        <div className="bg-gray-50 rounded-lg p-3 text-center">
          <Navigation className="w-6 h-6 mx-auto mb-2 text-blue-600" />
          <p className="font-medium">Driver is on the way</p>
          <p className="text-sm text-gray-600">Track your driver's location</p>
        </div>
      </div>
    </div>
  );

  const renderRegularView = () => (
    <div className="space-y-6 text-center animate-fade-in">
      <div className="flex items-center space-x-3 mb-6">
        <button 
          onClick={() => setCurrentView('home')}
          className="text-gray-600 hover:text-gray-800 transition-colors"
        >
          ←
        </button>
        <h2 className="text-xl font-semibold">Finding Available Drivers</h2>
      </div>
      <div className="bg-gray-50 rounded-xl p-6">
        <Car className="w-12 h-12 text-gray-400 mx-auto mb-4 animate-pulse-slow" />
        <p className="text-gray-600">Looking for the best driver nearby...</p>
      </div>
      <button 
        onClick={() => setCurrentView('home')}
        className="text-blue-600 hover:text-blue-800 transition-colors"
      >
        ← Back to Favorite Drivers
      </button>
    </div>
  );

  return (
    <div className="max-w-md mx-auto bg-gray-50 min-h-screen">
      <div className="bg-white shadow-sm px-6 py-4">
        <h1 className="text-2xl font-bold">Uber</h1>
      </div>
      
      <div className="p-6">
        {currentView === 'home' && renderHomeView()}
        {currentView === 'booking' && renderBookingView()}
        {currentView === 'waiting' && renderWaitingView()}
        {currentView === 'driverEnRoute' && renderDriverEnRouteView()}
        {currentView === 'regular' && renderRegularView()}
      </div>
    </div>
  );
};

export default UberApp;
