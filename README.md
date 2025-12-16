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

## Platform Generator Tool

The `platform_generator` CLI tool allows you to quickly generate hybrid Edge-Fog-Cloud platforms:

### Basic Usage

```bash
./build/platform_generator hybrid-cluster <eC> <eN> <fC> <fN> <cC> <cN>
```

Where:
- `eC`: Number of Edge clusters
- `eN`: Nodes per Edge cluster
- `fC`: Number of Fog clusters
- `fN`: Nodes per Fog cluster
- `cC`: Number of Cloud clusters
- `cN`: Nodes per Cloud cluster

**Example:**
```bash
# Generate platform with 2 Edge clusters (5 nodes each), 1 Fog cluster (3 nodes), 1 Cloud cluster (10 nodes)
./build/platform_generator hybrid-cluster 2 5 1 3 1 10
```

### Advanced Options

#### Direct Edge-Cloud Connectivity

Add `1` as the 7th parameter to enable direct Edge-to-Cloud links (bypassing Fog):

```bash
./build/platform_generator hybrid-cluster 2 5 1 3 1 10 1
```

This creates 2GBps/30ms links between Edge and Cloud clusters in addition to the normal Edge→Fog→Cloud topology.

#### Custom Output Filename

Specify a custom filename for the generated platform:

```bash
./build/platform_generator hybrid-cluster 2 5 1 3 1 10 1 my_custom_platform.xml
```

The tool automatically prepends `platforms/` if you don't specify a path.

#### Generate Template Application

Use the `--generate-app` flag to automatically create a C++ template application:

```bash
./build/platform_generator hybrid-cluster 2 5 1 3 1 10 1 my_platform.xml --generate-app
```

This generates:
- **Platform XML**: `platforms/my_platform.xml`
- **Template App**: `tests/my_platform_app.cpp`

The template includes:
- `EdgeDevice` class (if edge clusters > 0)
- `FogNode` class (if fog clusters > 0)
- `CloudServer` class (if cloud clusters > 0)
- Actor deployment logic with host classification
- TODO sections for implementing your custom logic

**To use the generated template:**

1. **Edit the template** and implement your logic in the TODO sections
2. **Add to CMakeLists.txt**:
   ```cmake
   add_executable(my_platform_app tests/my_platform_app.cpp)
   target_link_libraries(my_platform_app enigma_platform ${SimGrid_LIBRARY})
   ```
3. **Rebuild and run**:
   ```bash
   cd build && make my_platform_app
   ./my_platform_app ../platforms/my_platform.xml
   ```

### Complete Example

```bash
# Generate a complete Edge-Fog-Cloud platform with direct Edge-Cloud links and template app
./build/platform_generator hybrid-cluster 3 10 2 5 1 20 1 iot_platform.xml --generate-app

# Output:
#   - platforms/iot_platform.xml
#   - tests/iot_platform_app.cpp
```

## Examples

The `tests/` directory contains complete example applications:

- **edge_computing**: Edge application with distributed processing
- **fog_analytics**: Analytics system with Fog nodes
- **hybrid_cloud**: Hybrid Edge-Fog-Cloud architecture
- **data_offloading**: Smart offloading with request/response cycle
- **mqtt_edge_app**: MQTT publish/subscribe pattern for IoT/Edge

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


## Publications

### 2023

<summary>:newspaper: A scalable simulator for cloud, fog and edge computing platforms with mobility support.</summary>
 
  * Authors: Elías Del-Pozo-Puñal, Félix García-Carballeira, Diego Camarmas-Alonso
  * Journal Paper: Future Generation Computer Systems
  * Link: [:link: Open publication](https://www.sciencedirect.com/science/article/pii/S0167739X23000511)
  * Cite:
  ```bash
@article{DELPOZOPUNAL2023117,
title = {A scalable simulator for cloud, fog and edge computing platforms with mobility support},
journal = {Future Generation Computer Systems},
volume = {144},
pages = {117-130},
year = {2023},
issn = {0167-739X},
doi = {https://doi.org/10.1016/j.future.2023.02.010},
url = {https://www.sciencedirect.com/science/article/pii/S0167739X23000511},
author = {Elías Del-Pozo-Puñal and Félix García-Carballeira and Diego Camarmas-Alonso},
keywords = {Internet of things, Simulation, Fog and cloud computing, Mobility, Scalability},
abstract = {In recent years, the devices that make up the Internet of Things (IoT) paradigm have been increasing in number and complexity as different layers of network computing, such as Cloud, Edge and Fog Computing, have emerged. Thanks to these latest paradigms and their different types of communications, it has been possible to reduce and distribute the computational load on the network. However, to build these infrastructures and reduce the cost, it is necessary to use a simulation platform to model these environments and analyze their behavior (power consumption, CPU usage, bandwidth, etc.) beforehand. An essential aspect of simulators is scalability, the ability to add new components and simulate large infrastructures without compromising the performance of the simulator. Many existing simulators, as will be discussed in this article, cannot scale adequately as new elements are added to the system. Furthermore, in these environments, it is useful to simulate mobile devices and to know the location of these devices for the development and analysis of new algorithms. To solve these problems, this article presents and describes the ENIGMA simulator. ENIGMA is a scalable simulator of Edge, Fog and Cloud computing infrastructures, which allows to efficiently simulate a large number of devices and elements and to analyze different characteristics (CPU usage, power consumption, network bandwidth, application execution time, etc.). Its objective is to analyze new infrastructures and algorithms. ENIGMA also includes support for including mobile devices and an API to integrate their visualization in graphical maps. The article presents an evaluation to compare the scalability of ENIGMA with other state-of-the-art simulators and different use cases that illustrate its operation.}
}
  ```
</details>