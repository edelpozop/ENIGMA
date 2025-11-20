# ENIGMA - Edge-Fog-Cloud Platform Generator for SimGrid

A C++ project that enables creation of XML platforms for Edge, Fog, and Cloud infrastructures, with applications using SimGrid 4.1 for simulation.

## Features

- 🏗️ **XML Platform Generator**: Creates SimGrid platforms with Edge, Fog, and Cloud topologies
- 🔧 **Builder Pattern**: Fluent API to configure hosts, links, zones, and clusters
- 🚀 **Example Applications**: Distributed application examples using SimGrid
- 📊 **Multiple Topologies**: Support for Edge, Fog, Cloud, and hybrid architectures
- ⚡ **SimGrid 4.1**: Complete integration with SimGrid for distributed systems simulation
- ✅ **Native Cluster Support**: Production-ready single-level cluster platforms (edge-cluster, fog-cluster, cloud-cluster)
- ⚠️ **Multi-Zone Hybrid**: Experimental multi-zone cluster platforms (under investigation - see [KNOWN_ISSUES.md](KNOWN_ISSUES.md))

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
cmake -DCMAKE_INSTALL_PREFIX=/usr/local ..
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
├── src/
│   ├── platform/          # Platform generators
│   ├── builder/           # Builder pattern for platforms
│   ├── apps/              # Example applications
│   └── utils/             # Utilities
├── include/               # Public headers
├── platforms/             # Generated XML platforms
├── deployments/           # Deployment files
├── examples/              # Usage examples
└── CMakeLists.txt
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

## Examples

- **edge_computing**: Edge application with distributed processing
- **fog_analytics**: Analytics system with Fog node
- **hybrid_cloud**: Hybrid Edge-Fog-Cloud architecture
- **data_offloading**: Data offloading example

## 📁 Project Structure

```
ENIGMA/
├── include/          # Public headers
│   ├── platform/    # Edge, Fog, Cloud generators
│   └── utils/       # XMLWriter
├── src/             # Implementations
│   ├── apps/       # 4 example applications
│   ├── platform/   # Generator implementations
│   ├── tools/      # CLI platform_generator
│   └── utils/      # XMLWriter implementation
├── examples/        # Examples and templates
├── platforms/       # Generated XML platforms
├── build.sh         # Build script
└── run_examples.sh  # Interactive script
```
