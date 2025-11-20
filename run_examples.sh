#!/bin/bash

# Script to run complete examples

echo "======================================"
echo "  ENIGMA - Run Examples"
echo "======================================"
echo ""

if [ ! -d "build/bin" ]; then
    echo "❌ Error: Project is not compiled"
    echo "   Run first: ./build.sh"
    exit 1
fi

# Generate platforms if they don't exist
echo "📋 Generating example platforms..."
echo ""

# Edge Platform
if [ ! -f "platforms/edge_platform.xml" ]; then
    echo "Generating Edge platform..."
    ./build/bin/platform_generator edge 5
fi

# Fog Platform
if [ ! -f "platforms/fog_platform.xml" ]; then
    echo "Generating Fog platform..."
    ./build/bin/platform_generator fog 3
fi

# Cloud Platform
if [ ! -f "platforms/cloud_platform.xml" ]; then
    echo "Generating Cloud platform..."
    ./build/bin/platform_generator cloud 4
fi

# Hybrid Platform
if [ ! -f "platforms/hybrid_platform.xml" ]; then
    echo "Generating Hybrid platform..."
    ./build/bin/platform_generator hybrid 8 4 2
fi

# IoT Platform
if [ ! -f "platforms/iot_platform.xml" ]; then
    echo "Generating IoT platform..."
    ./build/bin/platform_generator iot 15 5
fi

echo ""
echo "✅ Platforms generated!"
echo ""

# Examples menu
while true; do
    echo "======================================"
    echo "Select an example to run:"
    echo "======================================"
    echo "1) Edge Computing"
    echo "2) Fog Analytics"
    echo "3) Hybrid Cloud (Edge-Fog-Cloud)"
    echo "4) Data Offloading"
    echo "5) Exit"
    echo ""
    read -p "Option [1-5]: " option
    
    case $option in
        1)
            echo ""
            echo "🚀 Running: Edge Computing Application"
            echo "=========================================="
            ./build/bin/edge_computing_app platforms/edge_platform.xml
            echo ""
            read -p "Press Enter to continue..."
            clear
            ;;
        2)
            echo ""
            echo "🚀 Running: Fog Analytics Application"
            echo "=========================================="
            ./build/bin/fog_analytics_app platforms/fog_platform.xml
            echo ""
            read -p "Press Enter to continue..."
            clear
            ;;
        3)
            echo ""
            echo "🚀 Running: Hybrid Cloud Application"
            echo "=========================================="
            ./build/bin/hybrid_cloud_app platforms/hybrid_platform.xml
            echo ""
            read -p "Press Enter to continue..."
            clear
            ;;
        4)
            echo ""
            echo "🚀 Running: Data Offloading Application"
            echo "=========================================="
            ./build/bin/data_offloading_app platforms/hybrid_platform.xml
            echo ""
            read -p "Press Enter to continue..."
            clear
            ;;
        5)
            echo "👋 Goodbye!"
            exit 0
            ;;
        *)
            echo "❌ Invalid option"
            sleep 1
            clear
            ;;
    esac
done

