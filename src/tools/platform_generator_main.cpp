#include "platform/PlatformBuilder.hpp"
#include "platform/EdgePlatform.hpp"
#include "platform/FogPlatform.hpp"
#include "platform/CloudPlatform.hpp"
#include <iostream>
#include <string>

using namespace enigma;

void printUsage(const char* progName) {
    std::cout << "Usage: " << progName << " <type> [options]\n\n";
    std::cout << "Platform types:\n";
    std::cout << "  Simple (all hosts interconnected):\n";
    std::cout << "    edge <num_devices>            - Edge platform\n";
    std::cout << "    fog <num_nodes>               - Fog platform\n";
    std::cout << "    cloud <num_servers>           - Cloud platform\n";
    std::cout << "    iot <sensors> <actuators>     - IoT platform\n";
    std::cout << "\n  Cluster-based (organized in clusters):\n";
    std::cout << "    edge-cluster <num_clusters> <nodes_per_cluster>  - Edge clusters\n";
    std::cout << "    fog-cluster <num_clusters> <nodes_per_cluster>   - Fog clusters\n";
    std::cout << "    cloud-cluster <num_clusters> <nodes_per_cluster> - Cloud clusters\n";
    std::cout << "    hybrid-cluster-flat <edge_clusters> <edge_nodes> <fog_clusters> <fog_nodes> <cloud_clusters> <cloud_nodes> [edge_cloud_direct] - Flat hybrid (optional direct Edge-Cloud)\n";
    std::cout << "\nExamples:\n";
    std::cout << "  Simple:\n";
    std::cout << "    " << progName << " edge 10\n";
    std::cout << "\n  Clusters:\n";
    std::cout << "    " << progName << " edge-cluster 3 5       # 3 edge clusters with 5 devices each\n";
    std::cout << "    " << progName << " hybrid-cluster-flat 2 10 2 5 1 20 1  # Flat hybrid with direct Edge-Cloud links\n";
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        printUsage(argv[0]);
        return 1;
    }
    
    std::string type = argv[1];
    
    try {
        // Simple host-based platforms
        if (type == "edge" && argc >= 3) {
            int numDevices = std::stoi(argv[2]);
            std::cout << "Generating Edge platform with " << numDevices << " devices...\n";
            
            auto zone = EdgePlatform::createStarTopology(numDevices);
            PlatformGenerator gen;
            gen.generatePlatform("platforms/edge_platform.xml", zone);
            
        } else if (type == "fog" && argc >= 3) {
            int numNodes = std::stoi(argv[2]);
            std::cout << "Generating Fog platform with " << numNodes << " nodes...\n";
            
            auto zone = FogPlatform::createHierarchicalTopology(numNodes);
            PlatformGenerator gen;
            gen.generatePlatform("platforms/fog_platform.xml", zone);
            
        } else if (type == "cloud" && argc >= 3) {
            int numServers = std::stoi(argv[2]);
            std::cout << "Generating Cloud platform with " << numServers << " servers...\n";
            
            auto zone = CloudPlatform::createCluster(numServers);
            PlatformGenerator gen;
            gen.generatePlatform("platforms/cloud_platform.xml", zone);
            
        } else if (type == "hybrid") {
            std::cerr << "The 'hybrid' hierarchical mode is deprecated. Use 'hybrid-cluster-flat'.\n";
            return 1;
        } else if (type == "iot" && argc >= 4) {
            int sensors = std::stoi(argv[2]);
            int actuators = std::stoi(argv[3]);
            
            std::cout << "Generating IoT platform:\n";
            std::cout << "  - Sensors: " << sensors << "\n";
            std::cout << "  - Actuators: " << actuators << "\n";
            
            auto zone = EdgePlatform::createIoTPlatform(sensors, actuators);
            PlatformGenerator gen;
            gen.generatePlatform("platforms/iot_platform.xml", zone);
        
        // Cluster-based platforms
        } else if (type == "edge-cluster" && argc >= 4) {
            int numClusters = std::stoi(argv[2]);
            int nodesPerCluster = std::stoi(argv[3]);
            
            std::cout << "Generating Edge platform with " << numClusters 
                      << " clusters of " << nodesPerCluster << " nodes each...\n";
            
            std::vector<ClusterConfig> clusters;
            for (int i = 0; i < numClusters; i++) {
                clusters.emplace_back("edge_cluster_" + std::to_string(i), 
                                     nodesPerCluster, "1Gf", 1, "125MBps", "50us");
            }
            
            auto zone = PlatformGenerator::createEdgeWithClusters("edge_platform", clusters);
            PlatformGenerator gen;
            gen.generatePlatform("platforms/edge_platform.xml", zone);
            
        } else if (type == "fog-cluster" && argc >= 4) {
            int numClusters = std::stoi(argv[2]);
            int nodesPerCluster = std::stoi(argv[3]);
            
            std::cout << "Generating Fog platform with " << numClusters 
                      << " clusters of " << nodesPerCluster << " nodes each...\n";
            
            std::vector<ClusterConfig> clusters;
            for (int i = 0; i < numClusters; i++) {
                clusters.emplace_back("fog_cluster_" + std::to_string(i), 
                                     nodesPerCluster, "10Gf", 4, "1GBps", "10us");
            }
            
            auto zone = PlatformGenerator::createFogWithClusters("fog_platform", clusters);
            PlatformGenerator gen;
            gen.generatePlatform("platforms/fog_platform.xml", zone);
            
        } else if (type == "cloud-cluster" && argc >= 4) {
            int numClusters = std::stoi(argv[2]);
            int nodesPerCluster = std::stoi(argv[3]);
            
            std::cout << "Generating Cloud platform with " << numClusters 
                      << " clusters of " << nodesPerCluster << " nodes each...\n";
            
            std::vector<ClusterConfig> clusters;
            for (int i = 0; i < numClusters; i++) {
                clusters.emplace_back("cloud_cluster_" + std::to_string(i), 
                                     nodesPerCluster, "100Gf", 16, "10GBps", "1us");
            }
            
            auto zone = PlatformGenerator::createCloudWithClusters("cloud_platform", clusters);
            PlatformGenerator gen;
            gen.generatePlatform("platforms/cloud_platform.xml", zone);
            
        } else if (type == "hybrid-cluster-flat" && argc >= 8) {
            int edgeClusters = std::stoi(argv[2]);
            int edgeNodes = std::stoi(argv[3]);
            int fogClusters = std::stoi(argv[4]);
            int fogNodes = std::stoi(argv[5]);
            int cloudClusters = std::stoi(argv[6]);
            int cloudNodes = std::stoi(argv[7]);
            bool directEdgeCloud = false;
            if (argc >= 9) {
                directEdgeCloud = (std::stoi(argv[8]) != 0);
            }
            
            std::cout << "Generating flat hybrid platform with clusters:\n";
            std::cout << "  - Edge: " << edgeClusters << " clusters × " << edgeNodes << " nodes\n";
            std::cout << "  - Fog: " << fogClusters << " clusters × " << fogNodes << " nodes\n";
            std::cout << "  - Cloud: " << cloudClusters << " clusters × " << cloudNodes << " nodes\n";
            std::cout << "  - Direct Edge-Cloud links: " << (directEdgeCloud ? "ENABLED" : "DISABLED") << "\n";
            
            std::vector<ClusterConfig> edgeClustersVec, fogClustersVec, cloudClustersVec;
            
            for (int i = 0; i < edgeClusters; i++) {
                edgeClustersVec.emplace_back("edge_cluster_" + std::to_string(i), 
                                            edgeNodes, "1Gf", 1, "125MBps", "50us");
            }
            
            for (int i = 0; i < fogClusters; i++) {
                fogClustersVec.emplace_back("fog_cluster_" + std::to_string(i), 
                                           fogNodes, "10Gf", 4, "1GBps", "10us");
            }
            
            for (int i = 0; i < cloudClusters; i++) {
                cloudClustersVec.emplace_back("cloud_cluster_" + std::to_string(i), 
                                             cloudNodes, "100Gf", 16, "10GBps", "1us");
            }
            
            auto zone = PlatformGenerator::createHybridWithClustersFlat(edgeClustersVec, fogClustersVec, cloudClustersVec, directEdgeCloud);
            PlatformGenerator gen;
            gen.generatePlatform("platforms/hybrid_platform.xml", zone);
            
        } else {
            std::cerr << "Error: Invalid arguments\n\n";
            printUsage(argv[0]);
            return 1;
        }
        
        std::cout << "\nPlatform successfully generated!\n";
        
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}
