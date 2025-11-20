# ENIGMA - gENeric Iot edGe siMulAtor

A C++ project that enables creation of XML platforms for Edge, Fog, and Cloud infrastructures, with applications using SimGrid 4.1 for simulation.

## Requirements

- C++17 or higher
- CMake 3.10 or higher
- SimGrid 4.1 or higher
- Compatible compiler (GCC 7+, Clang 5+, MSVC 2017+)

## SimGrid Installation

### Ubuntu/Debian
```bash
sudo apt-get update
sudo apt-get install simgrid libsimgrid-dev
```

### From Source
```bash
git clone https://github.com/simgrid/simgrid.git
cd simgrid
mkdir build && cd build
cmake -DCMAKE_INSTALL_PREFIX=/opt/simgrid-4.1 ..
make -j$(nproc)
sudo make install
```

## Build

```bash
mkdir build
cd build
cmake ..
make
```

## Project Structure

```
ENIGMA/
├── include/                 # Public headers
│   ├── platform/           # Platform generators (Edge, Fog, Cloud)
│   ├── comms/              # Communication modules
│   │   └── mqtt/           # MQTT headers (Broker, Publisher, Subscriber)
│   └── utils/              # Utility headers (XMLWriter)
├── src/                     # Implementation files
│   ├── platform/           # Platform generator implementations
│   ├── comms/              # Communication module implementations
│   │   └── mqtt/           # MQTT implementation (Broker, Publisher, Subscriber)
│   ├── tools/              # Command-line tools (platform_generator)
│   └── utils/              # Utility implementations (XMLWriter)
├── tests/                   # Test/Example applications
│   ├── edge_computing.cpp  # Basic edge computing
│   ├── fog_analytics.cpp   # Fog analytics
│   ├── hybrid_cloud.cpp    # Multi-tier hybrid
│   ├── data_offloading.cpp # Smart offloading decisions
│   ├── mqtt_edge_app.cpp   # MQTT pub/sub example
├── build/                   # Build artifacts (generated)
├── CMakeLists.txt           # CMake configuration
├── build.sh                 # Build script
├── run_examples.sh          # Interactive examples runner
└── README.md                
```

## Usage

### 1. Generate a Platform

```cpp
#include "platform/PlatformBuilder.hpp"

int main() {
    PlatformBuilder builder;
    
    // Create an Edge-Fog-Cloud platform
    builder.createEdgeFogCloud("my_platform.xml")
           .addEdgeLayer(10, "1Gf", "125MBps")
           .addFogLayer(5, "10Gf", "1GBps")
           .addCloudLayer(3, "100Gf", "10GBps")
           .build();
    
    return 0;
}
```

### 2. Run an Application

```cpp
#include <simgrid/s4u.hpp>

int main(int argc, char* argv[]) {
    simgrid::s4u::Engine e(&argc, argv);
    e.load_platform("platforms/my_platform.xml");
    e.load_deployment("deployments/my_deployment.xml");
    e.run();
    
    return 0;
}
```

### 3. Use MQTT (Optional)

```cpp
#include "comms/mqtt/MQTT.hpp"

void sensor_actor() {
    MQTTPublisher pub("mqtt_broker");
    pub.publish("sensors/temp", "25.5°C", 100);
}

void gateway_actor() {
    MQTTSubscriber sub("mqtt_broker");
    sub.subscribe("sensors/temp");
    auto msg = sub.receive();
}
```

## Examples

The `tests/` directory contains complete example applications:

- **edge_computing**: Edge application with distributed processing
- **fog_analytics**: Analytics system with Fog nodes
- **hybrid_cloud**: Hybrid Edge-Fog-Cloud architecture
- **data_offloading**: Smart offloading with request/response cycle
- **mqtt_edge_app**: MQTT publish/subscribe pattern for IoT/Edge ⭐ NEW

## 📁 Detailed Project Structure

```
ENIGMA/
├── include/                 # Public API headers
│   ├── platform/           # Platform generation
│   │   ├── PlatformGenerator.hpp
│   │   ├── PlatformBuilder.hpp
│   │   ├── EdgePlatform.hpp
│   │   ├── FogPlatform.hpp
│   │   └── CloudPlatform.hpp
│   ├── comms/              # Communication protocols
│   │   └── mqtt/           # MQTT module
│   │       ├── MQTT.hpp           # Convenience header
│   │       ├── MQTTBroker.hpp     # Broker component
│   │       ├── MQTTPublisher.hpp  # Publisher client
│   │       └── MQTTSubscriber.hpp # Subscriber client
│   └── utils/              # Utilities
│       └── XMLWriter.hpp
│
├── src/                     # Implementation files
│   ├── platform/           # Platform implementations
│   │   ├── PlatformGenerator.cpp
│   │   ├── PlatformBuilder.cpp
│   │   ├── EdgePlatform.cpp
│   │   ├── FogPlatform.cpp
│   │   └── CloudPlatform.cpp
│   ├── comms/              # Communication implementations
│   │   └── mqtt/           # MQTT implementation
│   │       ├── MQTTBroker.cpp
│   │       ├── MQTTPublisher.cpp
│   │       └── MQTTSubscriber.cpp
│   ├── tools/              # CLI tools
│   │   └── platform_generator_main.cpp
│   └── utils/              # Utility implementations
│       └── XMLWriter.cpp
│
├── tests/                   # Test applications (5 apps)
│   ├── edge_computing.cpp  # Edge-only processing
│   ├── fog_analytics.cpp   # Fog layer analytics
│   ├── hybrid_cloud.cpp    # Multi-tier application
│   ├── data_offloading.cpp # Smart offloading with responses
│   ├── mqtt_edge_app.cpp   # MQTT pub/sub IoT example
│
├── platforms/               # Generated XML platforms
├── deployments/             # Deployment configurations
├── build/                   # Build output directory
│
├── CMakeLists.txt           # Main build configuration
├── build.sh                 # Quick build script
├── run_examples.sh          # Interactive runner
├── verify.sh                # Verification script
│
├── QUICKSTART.md            # Quick start guide
├── CLUSTER_USAGE.md         # Cluster generation guide
└── README.md                
```
