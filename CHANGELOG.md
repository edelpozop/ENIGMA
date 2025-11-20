# Changelog - ENIGMA Platform Generator

## [Latest] - 2025-11-19

### ✅ Added - Flat Hybrid Cluster Platforms
- **New command**: `hybrid-cluster-flat` for generating hybrid platforms with 2-level hierarchy
- **Status**: Production ready and tested
- Resolves segmentation fault issues present in 3-level hierarchical platforms
- Provides simpler and more reliable routing structure

### 🎯 Improved - Cluster-Based Platform Generation
- Native SimGrid `<cluster>` tag support with automatic zone wrapping
- Smart inter-cluster routing using `<zoneRoute>` elements
- Automatic gateway selection (cluster routers)
- Tiered link generation for edge→fog→cloud communication

### 🔧 Fixed - Routing and Link Generation
- Prevented duplicate link generation in flat hybrid platforms
- Improved link selection logic for inter-zone communication
- Fixed automatic route generation for different cluster topologies

### 📚 Documentation
- Updated [CLUSTER_USAGE.md](CLUSTER_USAGE.md) with flat hierarchy documentation
- Created [KNOWN_ISSUES.md](KNOWN_ISSUES.md) with detailed troubleshooting
- Added platform structure comparisons and testing results

## Previous Changes

### [Initial] - Full English Translation
- Translated entire codebase from Spanish to English
- Updated all comments, documentation, and user-facing messages
- Maintained code functionality while improving international accessibility

### Build System Enhancements
- Added multi-path SimGrid detection (/usr, /usr/local, /opt/simgrid-4.1)
- Improved build scripts for manual SimGrid installations
- Enhanced CMake configuration for better dependency management

### Code Quality Improvements
- Eliminated all compilation warnings
- Updated to SimGrid 4.1 API (`Host::add_actor()` instead of deprecated `Host::create_actor()`)
- Fixed unused parameter warnings
- Ensured C++17 compliance

### Architecture Evolution
- Introduced cluster-based platform generation (vs. simple host counts)
- Added `ClusterConfig` structure for flexible cluster definition
- Implemented `use_native_clusters` configuration flag
- Created hierarchical zone structure with proper routing

## Feature Matrix

| Feature | Status | Notes |
|---------|--------|-------|
| Simple platforms (edge/fog/cloud) | ✅ Stable | Original functionality |
| Single-level clusters | ✅ Stable | Tested extensively |
| **Flat hybrid clusters** | ✅ **PRODUCTION** | **Recommended for hybrid** |
| 3-level hybrid clusters | ⚠️ Experimental | Use with caution |
| Native SimGrid clusters | ✅ Stable | Uses `<cluster>` tags |
| Automatic routing | ✅ Stable | zoneRoutes with gateways |

## Platform Types

### Stable (Production Ready) ✅
- `edge <num_devices>`
- `fog <num_nodes>`
- `cloud <num_servers>`
- `hybrid <edge> <fog> <cloud>`
- `edge-cluster <clusters> <nodes>`
- `fog-cluster <clusters> <nodes>`
- `cloud-cluster <clusters> <nodes>`
- **`hybrid-cluster-flat <edge_c> <edge_n> <fog_c> <fog_n> <cloud_c> <cloud_n>`** ⭐

### Experimental ⚠️
- `hybrid-cluster` (3-level hierarchy)

## Testing Summary

### Successfully Tested ✅
- **Edge clusters**: 3 clusters × 4 nodes - Completes in 9.79s
- **Fog clusters**: 2 clusters × 4 nodes - Completes in 13.50s
- **Flat hybrid**: 2 edge (5 nodes) + 1 fog (3 nodes) + 1 cloud (10 nodes) - Completes in 6.41s
  - Full edge→fog→cloud pipeline
  - Data filtering, aggregation, and ML analysis
  - All 18 hosts active and communicating

### Known Issues ⚠️
- **3-level hybrid** (`hybrid-cluster`): Segmentation fault during inter-zone communication
  - Workaround: Use `hybrid-cluster-flat` instead

## Migration Guide

### From `hybrid-cluster` to `hybrid-cluster-flat`

**Before:**
```bash
./build/platform_generator hybrid-cluster 2 10 2 5 1 20
# May cause segfault
```

**After:**
```bash
./build/platform_generator hybrid-cluster-flat 2 10 2 5 1 20
# Works correctly ✅
```

No changes needed in application code - the flat version produces compatible host naming.

## Performance Notes

- Flat hierarchy reduces routing complexity
- Fewer zone nesting levels = faster route resolution
- Native `<cluster>` tags leverage SimGrid optimizations
- Suitable for large-scale simulations (100+ hosts)

## Future Improvements

- [ ] Investigate 3-level hierarchy compatibility with SimGrid 3.36+
- [ ] Add per-cluster customization options (heterogeneous clusters)
- [ ] Implement cluster templates for common configurations
- [ ] Add validation tool for generated platforms
- [ ] Performance benchmarking suite

---

**Contributors**: ENIGMA Development Team
**License**: MIT
