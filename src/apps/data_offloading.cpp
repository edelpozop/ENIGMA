#include <simgrid/s4u.hpp>
#include <iostream>
#include <vector>

XBT_LOG_NEW_DEFAULT_CATEGORY(data_offloading, "Data Offloading Application");

namespace sg4 = simgrid::s4u;

/**
 * @brief Data offloading application
 * 
 * Simulates offloading decisions: process locally on Edge,
 * send to Fog, or send to Cloud based on load and resources.
 */

class SmartEdgeDevice {
    std::string fog_name;
    std::string cloud_name;
    double workload;
    
public:
    SmartEdgeDevice(const std::string& fog, const std::string& cloud, double load)
        : fog_name(fog), cloud_name(cloud), workload(load) {}
    
    void operator()() {
        sg4::Host* this_host = sg4::this_actor::get_host();
        XBT_INFO("Edge Device '%s' started (load: %.2f GFlops)",
                 this_host->get_cname(), workload / 1e9);
        
        double device_capacity = this_host->get_speed();
        
        // Offloading decision based on capacity
        if (workload < device_capacity * 0.5) {
            // Local processing - low load
            XBT_INFO("Decision: LOCAL PROCESSING (low load)");
            sg4::this_actor::execute(workload);
            XBT_INFO("Local processing completed in %.2f seconds",
                     sg4::Engine::get_clock());
            
        } else if (workload < device_capacity * 2.0) {
            // Offload to Fog - medium load
            XBT_INFO("Decision: OFFLOAD TO FOG (medium load)");
            
            auto* mbox = sg4::Mailbox::by_name(fog_name);
            auto* task = new double(workload);
            
            XBT_INFO("Sending task to Fog '%s' (%.2f MFlops)...",
                     fog_name.c_str(), workload / 1e6);
            mbox->put(task, workload / 1e3);  // Data size = workload/1000
            
            // Wait for result
            auto* result_mbox = sg4::Mailbox::by_name(
                std::string(this_host->get_cname()) + "_result");
            auto* result = result_mbox->get<std::string>();
            XBT_INFO("Result received from Fog: %s", result->c_str());
            delete result;
            
        } else {
            // Offload to Cloud - high load
            XBT_INFO("Decision: OFFLOAD TO CLOUD (high load)");
            
            auto* mbox = sg4::Mailbox::by_name(cloud_name);
            auto* task = new double(workload);
            
            XBT_INFO("Sending task to Cloud '%s' (%.2f MFlops)...",
                     cloud_name.c_str(), workload / 1e6);
            mbox->put(task, workload / 1e3);
            
            // Wait for result
            auto* result_mbox = sg4::Mailbox::by_name(
                std::string(this_host->get_cname()) + "_result");
            auto* result = result_mbox->get<std::string>();
            XBT_INFO("Result received from Cloud: %s", result->c_str());
            delete result;
        }
        
        XBT_INFO("Task completed successfully");
    }
};

class OffloadingServer {
    std::string type;
    
public:
    OffloadingServer(const std::string& server_type) : type(server_type) {}
    
    void operator()() {
        sg4::Host* this_host = sg4::this_actor::get_host();
        XBT_INFO("[%s] Server '%s' ready to receive tasks",
                 type.c_str(), this_host->get_cname());
        
        auto* mbox = sg4::Mailbox::by_name(this_host->get_cname());
        
        // Server always listening
        while (true) {
            try {
                auto* task = mbox->get<double>(10.0);  // Timeout 10s
                
                XBT_INFO("[%s] Task received: %.2f MFlops", type.c_str(), *task / 1e6);
                
                // Process task
                double start_time = sg4::Engine::get_clock();
                sg4::this_actor::execute(*task);
                double end_time = sg4::Engine::get_clock();
                
                XBT_INFO("[%s] Task processed in %.2f seconds",
                         type.c_str(), end_time - start_time);
                
                delete task;
                
                // Note: In a complete implementation, here we would send the result
                // back to the device that requested it
                
            } catch (const simgrid::TimeoutException&) {
                // Timeout - no more tasks
                XBT_INFO("[%s] No more tasks, finishing", type.c_str());
                break;
            }
        }
        
        XBT_INFO("[%s] Server finished", type.c_str());
    }
};

int main(int argc, char* argv[]) {
    sg4::Engine e(&argc, argv);
    
    if (argc < 2) {
        XBT_CRITICAL("Usage: %s <platform_file.xml>", argv[0]);
        XBT_CRITICAL("Example: %s platforms/hybrid_platform.xml", argv[0]);
        return 1;
    }
    
    e.load_platform(argv[1]);
    
    std::vector<sg4::Host*> hosts = e.get_all_hosts();
    
    if (hosts.size() < 3) {
        XBT_CRITICAL("At least 3 hosts required (Edge, Fog, Cloud)");
        return 1;
    }
    
    XBT_INFO("=== Data Offloading Application ===");
    XBT_INFO("Platform loaded with %zu hosts", hosts.size());
    
    // Classify hosts
    std::vector<sg4::Host*> edge_hosts, fog_hosts, cloud_hosts;
    
    for (auto* host : hosts) {
        std::string name = host->get_cname();
        if (name.find("edge") != std::string::npos) {
            edge_hosts.push_back(host);
        } else if (name.find("fog") != std::string::npos) {
            fog_hosts.push_back(host);
        } else if (name.find("cloud") != std::string::npos) {
            cloud_hosts.push_back(host);
        }
    }
    
    // Default distribution if no specific names
    if (edge_hosts.empty() && fog_hosts.empty() && cloud_hosts.empty()) {
        size_t third = hosts.size() / 3;
        edge_hosts.assign(hosts.begin(), hosts.begin() + third);
        fog_hosts.assign(hosts.begin() + third, hosts.begin() + 2 * third);
        cloud_hosts.assign(hosts.begin() + 2 * third, hosts.end());
    }
    
    if (edge_hosts.empty()) edge_hosts.push_back(hosts[0]);
    if (fog_hosts.empty()) fog_hosts.push_back(hosts[std::min(1UL, hosts.size()-1)]);
    if (cloud_hosts.empty()) cloud_hosts.push_back(hosts.back());
    
    XBT_INFO("Configuration:");
    XBT_INFO("  Edge devices: %zu", edge_hosts.size());
    XBT_INFO("  Fog nodes: %zu", fog_hosts.size());
    XBT_INFO("  Cloud servers: %zu", cloud_hosts.size());
    
    // Create Fog servers
    for (auto* fog : fog_hosts) {
        fog->add_actor("fog_server", OffloadingServer("FOG"));
    }
    
    // Create Cloud servers
    for (auto* cloud : cloud_hosts) {
        cloud->add_actor("cloud_server", OffloadingServer("CLOUD"));
    }
    
    // Create Edge devices with different loads
    double base_workload = 1e9;  // 1 GFlop base
    for (size_t i = 0; i < edge_hosts.size(); i++) {
        double workload = base_workload * (0.5 + i * 0.5);  // Increasing load
        
        std::string fog_name = fog_hosts[i % fog_hosts.size()]->get_cname();
        std::string cloud_name = cloud_hosts[i % cloud_hosts.size()]->get_cname();
        
        edge_hosts[i]->add_actor("smart_device_" + std::to_string(i),
                                 SmartEdgeDevice(fog_name, cloud_name, workload));
    }
    
    // Run simulation
    e.run();
    
    XBT_INFO("=== Simulation completed ===");
    XBT_INFO("Simulated time: %.2f seconds", sg4::Engine::get_clock());
    
    return 0;
}
