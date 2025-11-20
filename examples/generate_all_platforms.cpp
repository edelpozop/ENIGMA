#include "platform/PlatformBuilder.hpp"
#include "platform/EdgePlatform.hpp"
#include "platform/FogPlatform.hpp"
#include "platform/CloudPlatform.hpp"
#include <iostream>

using namespace enigma;

/**
 * Ejemplo 1: Plataforma Edge simple con topología estrella
 */
void example1_simple_edge() {
    std::cout << "\n=== Ejemplo 1: Plataforma Edge Simple ===\n";
    
    PlatformBuilder builder;
    builder.createPlatform("simple_edge")
           .addEdgeLayer(5, "1Gf", "100MBps")
           .build();
    
    std::cout << "✓ Generado: platforms/simple_edge.xml\n";
}

/**
 * Ejemplo 2: Plataforma Fog jerárquica
 */
void example2_fog_hierarchy() {
    std::cout << "\n=== Ejemplo 2: Plataforma Fog Jerárquica ===\n";
    
    auto zone = FogPlatform::createHierarchicalTopology(4, "15Gf");
    
    PlatformGenerator gen;
    gen.generatePlatform("platforms/fog_hierarchy.xml", zone);
    
    std::cout << "✓ Generado: platforms/fog_hierarchy.xml\n";
}

/**
 * Ejemplo 3: Data Center Cloud
 */
void example3_cloud_datacenter() {
    std::cout << "\n=== Ejemplo 3: Data Center Cloud ===\n";
    
    auto zone = CloudPlatform::createDataCenter(3, 8, "200Gf");
    
    PlatformGenerator gen;
    gen.generatePlatform("platforms/cloud_datacenter.xml", zone);
    
    std::cout << "✓ Generado: platforms/cloud_datacenter.xml\n";
}

/**
 * Ejemplo 4: Plataforma Híbrida completa
 */
void example4_full_hybrid() {
    std::cout << "\n=== Ejemplo 4: Plataforma Híbrida Completa ===\n";
    
    PlatformBuilder builder;
    builder.createEdgeFogCloud("full_hybrid")
           .addEdgeLayer(12, "1.5Gf", "150MBps")
           .addFogLayer(6, "12Gf", "1.5GBps")
           .addCloudLayer(4, "150Gf", "15GBps")
           .build();
    
    std::cout << "✓ Generado: platforms/full_hybrid.xml\n";
}

/**
 * Ejemplo 5: Plataforma IoT con sensores y actuadores
 */
void example5_iot_platform() {
    std::cout << "\n=== Ejemplo 5: Plataforma IoT ===\n";
    
    auto zone = EdgePlatform::createIoTPlatform(25, 8, "4Gf");
    
    PlatformGenerator gen;
    gen.generatePlatform("platforms/iot_sensors.xml", zone);
    
    std::cout << "✓ Generado: platforms/iot_sensors.xml\n";
}

/**
 * Ejemplo 6: Plataforma con configuración personalizada
 */
void example6_custom_platform() {
    std::cout << "\n=== Ejemplo 6: Plataforma Personalizada ===\n";
    
    ZoneConfig root("custom_platform", "Full");
    
    // Zona Edge
    ZoneConfig edgeZone("edge_area", "Full");
    edgeZone.hosts.push_back(HostConfig("mobile_device_1", "2Gf", 4));
    edgeZone.hosts.push_back(HostConfig("mobile_device_2", "2Gf", 4));
    edgeZone.hosts.push_back(HostConfig("edge_gateway", "5Gf", 2));
    edgeZone.links.push_back(LinkConfig("5g_link", "1GBps", "5ms"));
    
    // Zona Fog
    ZoneConfig fogZone("fog_area", "Full");
    fogZone.hosts.push_back(HostConfig("fog_server_1", "20Gf", 16));
    fogZone.hosts.push_back(HostConfig("fog_server_2", "20Gf", 16));
    fogZone.links.push_back(LinkConfig("fog_backbone", "10GBps", "2ms"));
    
    // Zona Cloud
    ZoneConfig cloudZone("cloud_area", "Full");
    cloudZone.hosts.push_back(HostConfig("cloud_vm_1", "100Gf", 32));
    cloudZone.hosts.push_back(HostConfig("cloud_vm_2", "100Gf", 32));
    cloudZone.hosts.push_back(HostConfig("cloud_gpu_1", "1000Gf", 128));
    cloudZone.links.push_back(LinkConfig("datacenter_fabric", "100GBps", "100us"));
    
    root.subzones.push_back(edgeZone);
    root.subzones.push_back(fogZone);
    root.subzones.push_back(cloudZone);
    
    // Enlaces entre zonas
    root.links.push_back(LinkConfig("edge_to_fog", "500MBps", "10ms"));
    root.links.push_back(LinkConfig("fog_to_cloud", "5GBps", "50ms"));
    
    PlatformGenerator gen;
    gen.generatePlatform("platforms/custom_platform.xml", root);
    
    std::cout << "✓ Generado: platforms/custom_platform.xml\n";
}

/**
 * Ejemplo 7: Multi-Cloud con diferentes proveedores
 */
void example7_multi_cloud() {
    std::cout << "\n=== Ejemplo 7: Multi-Cloud ===\n";
    
    auto zone = CloudPlatform::createMultiCloud(3, 5, "120Gf");
    
    PlatformGenerator gen;
    gen.generatePlatform("platforms/multi_cloud.xml", zone);
    
    std::cout << "✓ Generado: platforms/multi_cloud.xml\n";
}

/**
 * Ejemplo 8: Edge-Fog con distribución geográfica
 */
void example8_geographic_fog() {
    std::cout << "\n=== Ejemplo 8: Fog Geográfico ===\n";
    
    auto zone = FogPlatform::createGeographicTopology(4, 3, "15Gf");
    
    PlatformGenerator gen;
    gen.generatePlatform("platforms/geographic_fog.xml", zone);
    
    std::cout << "✓ Generado: platforms/geographic_fog.xml\n";
}

int main() {
    std::cout << "╔══════════════════════════════════════════╗\n";
    std::cout << "║  ENIGMA - Platform Generator Examples   ║\n";
    std::cout << "║  Ejemplos de Generación de Plataformas  ║\n";
    std::cout << "╚══════════════════════════════════════════╝\n";
    
    try {
        example1_simple_edge();
        example2_fog_hierarchy();
        example3_cloud_datacenter();
        example4_full_hybrid();
        example5_iot_platform();
        example6_custom_platform();
        example7_multi_cloud();
        example8_geographic_fog();
        
        std::cout << "\n" << std::string(50, '=') << "\n";
        std::cout << "✅ Todos los ejemplos generados exitosamente!\n";
        std::cout << "\nPlataformas generadas en el directorio: platforms/\n";
        std::cout << "\nPara ejecutar aplicaciones:\n";
        std::cout << "  ./build/bin/edge_computing_app platforms/simple_edge.xml\n";
        std::cout << "  ./build/bin/hybrid_cloud_app platforms/full_hybrid.xml\n";
        std::cout << std::string(50, '=') << "\n";
        
    } catch (const std::exception& e) {
        std::cerr << "❌ Error: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}
