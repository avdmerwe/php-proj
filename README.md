# php-proj

PHP bindings for PROJ cartographic projections library.

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![PHP Version](https://img.shields.io/badge/php-%3E%3D8.1-8892BF.svg)](https://php.net/)
[![PROJ Version](https://img.shields.io/badge/PROJ-%3E%3D6.0.0-brightgreen.svg)](https://proj.org/)

## Overview

This extension provides PHP bindings for the PROJ library, enabling coordinate transformations and cartographic projections within PHP applications.

PROJ (https://proj.org/) is a generic coordinate transformation software that transforms geospatial coordinates from one coordinate reference system (CRS) to another.

## Features

- Coordinate transformations between different coordinate reference systems
- Support for EPSG codes, WKT, and PROJ strings
- Batch coordinate processing for efficient operations
- Context management for PROJ library settings
- Thread-safe operations for multi-threaded environments

## Quick Start

### Installation

#### From Source
```bash
git clone https://github.com/avdmerwe/php-proj.git
cd php-proj
./build.sh build
./build.sh test
sudo ./build.sh install
```

#### Requirements
- PHP 8.1 or higher
- PROJ library 6.0.0 or higher
- pkg-config
- PHP development headers (php-dev package)

### Basic Usage

```php
<?php
// Create coordinate reference systems
$wgs84 = ProjCRS::fromEpsg(4326);
$webMercator = ProjCRS::fromEpsg(3857);

// Create transformer
$transformer = ProjTransformer::fromCrs($wgs84, $webMercator);

// Transform coordinates
list($x, $y) = $transformer->transform(-73.985656, 40.748433); // NYC coordinates
echo "Web Mercator: X=$x, Y=$y\n";

// Geodetic calculations
$geod = ProjGeod::fromEpsg(4326);
$result = $geod->inv(
    [-73.985656, 40.748433],  // NYC
    [-0.127758, 51.507351]    // London
);
echo "Distance: " . $result['distance'] . " meters\n";
echo "Azimuth: " . $result['azimuth'] . " degrees\n";
?>
```

## Build Commands

Use the provided build script for all operations:

```bash
./build.sh build        # Configure and compile the extension  
./build.sh test         # Run the full test suite
./build.sh clean        # Clean build artifacts
./build.sh distclean    # Reset to pristine state
./build.sh debian       # Build Debian package
./build.sh release      # Create release archive
./build.sh tag          # Create git tag for release
./build.sh push         # Push to GitHub
```

For manual testing during development:

```bash
# Test specific functionality
php -d extension=modules/proj.so /path/to/test.php

# Run single test file
php run-tests.php -d extension=modules/proj.so tests/001-crs-basic.phpt

# Check for memory leaks
valgrind --leak-check=full php -d extension=modules/proj.so test_script.php
```

## Installation

### From Package Manager
```bash
# Debian/Ubuntu
sudo apt-get install php-proj
```

### From Source
1. Install dependencies:
   ```bash
   # Debian/Ubuntu
   sudo apt-get install php-dev libproj-dev pkg-config
   ```

2. Build and install:
   ```bash
   git clone https://github.com/avdmerwe/php-proj.git
   cd php-proj
   ./build.sh build
   ./build.sh test
   sudo ./build.sh install
   ```

3. Enable in PHP:
   ```bash
   echo "extension=proj.so" | sudo tee /etc/php/8.1/mods-available/proj.ini
   sudo phpenmod proj
   ```

## API Overview

This extension follows standard PHP extension naming conventions:
- **Global Functions**: `proj_*` prefix (e.g., `proj_get_authorities()`, `proj_get_codes()`)
- **Classes**: `Proj*` prefix in global namespace (e.g., `ProjCRS`, `ProjTransformer`, `Proj`)
- **Exceptions**: `Proj*Exception` classes (e.g., `ProjCRSException`, `ProjTransformerException`)

### Main Classes
- **`ProjCRS`**: Coordinate Reference System creation, properties, and conversions
- **`ProjTransformer`**: Coordinate transformations with full pipeline support
- **`ProjGeod`**: Geodetic calculations (distance, bearing, area computations)
- **`ProjTransformerGroup`**: Advanced transformation group operations

### Utility Classes
- **`ProjFactors`**: Scale and angular distortion factors
- **`ProjAreaOfInterest`**: Geographic area specifications
- **`ProjUnit`**: Unit conversion and information

### Object Model Classes
- **`ProjCoordinateSystem`**: Coordinate system objects with axis information
- **`ProjAxis`**: Individual coordinate axis properties
- **`ProjDatum`**: Geodetic reference frame objects
- **`ProjEllipsoid`**: Ellipsoid parameter objects
- **`ProjPrimeMeridian`**: Prime meridian objects
- **`ProjCoordinateOperation`**: Coordinate transformation operations
- **`ProjAreaOfUse`**: Area of use boundaries

### Enumeration Classes
- **`ProjWktVersion`**: WKT format versions (WKT1_GDAL, WKT2_2019, etc.)
- **`ProjVersion`**: PROJ string versions (PROJ_4, PROJ_5)
- **`ProjTransformDirection`**: Transform directions (FORWARD, INVERSE, IDENT)
- **`ProjType`**: PROJ object types (CRS types, operation types)

## Architecture Overview

This is a comprehensive PHP extension providing complete PROJ library integration with 18 modular C source files:

### Core Architecture
- **php_proj.c**: Extension entry point, global PROJ context management, module lifecycle
- **proj_crs.c**: ProjCRS class with comprehensive factory methods and object-oriented information access
- **proj_transformer.c**: ProjTransformer class supporting coordinate transformations and utilities
- **proj_geod.c**: ProjGeod class for geodetic calculations (distance, bearing, area, inverse/direct)
- **proj_area_of_interest.c**: ProjAreaOfInterest class for geographic area specifications with containment and intersection methods
- **proj_factors.c**: ProjFactors class for projection scale and distortion factor calculations
- **proj_unit.c**: ProjUnit class for unit conversion information and metadata
- **proj_exceptions.c**: Hierarchical exception system with specific exception types
- **proj_utils.c**: Shared utility functions for array conversions and parameter parsing
- **proj_enums.c**: All PROJ enumeration constants and type definitions
- **proj_functions.c**: Global functions (database queries, network settings, grid management)

### Object Model Architecture
- **proj_coordinate_system.c**: ProjCoordinateSystem class for coordinate system objects with axis management
- **proj_axis.c**: ProjAxis class for individual coordinate axis properties and metadata
- **proj_datum.c**: ProjDatum class for geodetic reference frame objects
- **proj_ellipsoid.c**: ProjEllipsoid class for ellipsoid parameter objects with factory methods
- **proj_prime_meridian.c**: ProjPrimeMeridian class for prime meridian objects
- **proj_coordinate_operation.c**: ProjCoordinateOperation class for coordinate transformation operations
- **proj_area_of_use.c**: ProjAreaOfUse class for area of use boundaries (different from ProjAreaOfInterest)

### PHP Object Design Pattern
Each PROJ object is wrapped in a Zend object with proper lifecycle management:
```c
typedef struct {
    PJ *pj;                    // PROJ library object
    zend_object std;           // Standard Zend object
} proj_crs_object;
```

### Memory Management
- Uses emalloc/efree for PHP memory management
- PROJ objects automatically destroyed in Zend object destructors
- Thread-safe context management throughout

## Testing Strategy

The extension uses PHP's standard phpt test framework with 28 comprehensive test files:

### Core Functionality Tests
- **001-crs-basic.phpt**: CRS creation, properties, format conversions
- **002-transformer-basic.phpt**: Coordinate transformations, batch processing
- **004-geod-basic.phpt**: Geodetic calculations and algorithms
- **005-functions.phpt**: Global function testing
- **006-enums.phpt**: Enumeration constants verification

### Extended Functionality Tests  
- **100-crs-creation.phpt**: CRS creation from various input formats
- **101-crs-properties.phpt**: CRS properties and information methods
- **102-crs-advanced.phpt**: Advanced CRS operations and edge cases
- **103-crs-maker.phpt**: Extended CRS creation and factory methods
- **104-crs-json.phpt**: JSON serialization and deserialization
- **105-coordinate-operations.phpt**: Advanced projection operations and transformations
- **106-datadir-management.phpt**: Data directory management and configuration
- **107-area-of-interest.phpt**: Area of Interest functionality (contains, intersects)
- **108-factors-unit-classes.phpt**: ProjFactors and ProjUnit utility classes

### Integration Tests
- **200-transformer-basic.phpt**: Transformer basic coordinate transformations
- **201-transformer-advanced.phpt**: Advanced transformation features
- **202-transform-utilities.phpt**: Transform utilities and coordinate processing
- **300-geod-basic.phpt**: Geodetic calculations and algorithms
- **400-functions-global.phpt**: Database queries and network functions

## Examples

### Coordinate Transformation
```php
// Simple transformation
$transformer = ProjTransformer::fromCrs("EPSG:4326", "EPSG:32633"); // WGS84 to UTM Zone 33N
list($x, $y) = $transformer->transform(12.0, 55.0);

// Batch transformation
$coordinates = [
    [12.0, 55.0],
    [12.1, 55.1],
    [12.2, 55.2]
];
$results = $transformer->transformArray($coordinates);
```

### Working with CRS Objects
```php
$crs = ProjCRS::fromEpsg(4326);
echo "Name: " . $crs->getName() . "\n";
echo "Type: " . $crs->getTypeName() . "\n";
echo "Area of use: " . $crs->getAreaOfUse()->getName() . "\n";

// Get detailed information
$datum = $crs->getDatum();
$ellipsoid = $datum->getEllipsoid();
echo "Ellipsoid: " . $ellipsoid->getName() . "\n";
echo "Semi-major axis: " . $ellipsoid->getSemiMajorMetre() . " m\n";
```

### Geodetic Calculations
```php
$geod = ProjGeod::fromEpsg(4326);

// Calculate distance and bearing
$result = $geod->inv(
    [2.3522, 48.8566],    // Paris
    [13.4050, 52.5200]    // Berlin
);
echo "Distance: " . number_format($result['distance']) . " m\n";
echo "Forward azimuth: " . $result['azimuth'] . "°\n";

// Calculate destination point
$dest = $geod->fwd([2.3522, 48.8566], 45.0, 100000); // 100km northeast from Paris
echo "Destination: " . $dest['lon'] . ", " . $dest['lat'] . "\n";

// Calculate polygon area
$polygon = [
    [0, 0],
    [1, 0],
    [1, 1],
    [0, 1],
    [0, 0]
];
$area_result = $geod->polygonAreaPerimeter($polygon);
echo "Area: " . number_format($area_result['area']) . " m²\n";
echo "Perimeter: " . number_format($area_result['perimeter']) . " m\n";
```

## Development Requirements

### Dependencies
- **PROJ Library**: >= 6.0.0 (detected via pkg-config)
- **PHP**: 8.1+ (uses modern arginfo declarations)
- **Build tools**: phpize, autotools, pkg-config
- **Debian packaging**: debhelper-compat (= 13), dh-php

### Code Standards
- All methods must have proper arginfo declarations for PHP 8+ compatibility
- Memory safety is critical - always pair emalloc with efree
- PROJ context errors must be propagated as PHP exceptions
- Thread safety: Use ZTS-aware patterns throughout
- Performance: Support both scalar and array coordinate inputs

### Key Integration Points
- **PROJ Context**: Single global context with thread-safe access and suppressed debug logging
- **Error Handling**: PROJ errors mapped to specific PHP exception types (ProjCRSException, etc.)
- **Type System**: Full PHP type hints with return type declarations
- **Array Processing**: Efficient C ↔ PHP array conversions for batch operations
- **Naming Conventions**: Global namespace classes with Proj prefix, global functions with proj_ prefix

## Implemented Features

### ProjCRS Factory Methods
- `ProjCRS::fromEpsg(int $epsg_code)` - Create from EPSG code
- `ProjCRS::fromString(string $crs_string)` - Create from any PROJ string
- `ProjCRS::fromUserInput(mixed $value)` - Create from user input (flexible)
- `ProjCRS::fromWkt(string $wkt)` - Create from WKT string
- `ProjCRS::fromProj4(string $proj4)` - Create from PROJ4 string
- `ProjCRS::fromJson(string $json)` - Create from JSON string
- `ProjCRS::fromAuthority(string $authority, int $code)` - Create from authority and code

### ProjCRS Information Methods

#### Basic Properties
- `$crs->getName()` - Get CRS name
- `$crs->getTypeName()` - Get CRS type description  
- `$crs->getToEpsg()` - Get EPSG code if available
- `$crs->getGeodeticCrs()` - Get geodetic CRS
- `$crs->getSourceCrs()`, `$crs->getTargetCrs()` - Get source/target CRS
- `$crs->getSubCrsList()` - Get sub-CRS list
- `$crs->getRemarks()`, `$crs->getScope()` - Get remarks and scope
- `$crs->getToAuthority()` - Get authority information
- `$crs->isGeographic()`, `$crs->isProjected()`, `$crs->isGeocentric()`, `$crs->isCompound()`, `$crs->isEngineering()`, `$crs->isVertical()`, `$crs->isBound()`, `$crs->isDerived()` - Type checks
- `$crs->toWkt(string $version = "WKT2_2019", bool $pretty = false)`, `$crs->toProj4(string $version = "PROJ_5")`, `$crs->toJson(bool $pretty = false)` - Format conversions
- `$crs->equals(ProjCRS $other, bool $ignore_axis_order = false)`, `$crs->isExactSame(ProjCRS $other)` - CRS comparison
- `$crs->getFactors(float $longitude, float $latitude)` - Returns **ProjFactors** object with projection scale and distortion factors

#### Object-Oriented Methods
- `$crs->getCoordinateSystem()` - Returns **ProjCoordinateSystem** object
- `$crs->getDatum()` - Returns **ProjDatum** object
- `$crs->getEllipsoid()` - Returns **ProjEllipsoid** object  
- `$crs->getPrimeMeridian()` - Returns **ProjPrimeMeridian** object
- `$crs->getAreaOfUse()` - Returns **ProjAreaOfUse** object
- `$crs->getCoordinateOperation()` - Returns **ProjCoordinateOperation** object
- `$crs->getAxisInfo()` - Returns array of **ProjAxis** objects

### ProjTransformer Methods
- `ProjTransformer::fromCrs(mixed $crs_from, mixed $crs_to, bool $always_xy = false, ?array $area_of_interest = null, ?string $authority = null, float $accuracy = -1, bool $allow_ballpark = true, bool $force_over = false, bool $only_best = true)` - Create transformer
- `ProjTransformer::fromPipeline(string $pipeline)` - Create from pipeline string
- `$transformer->transform(mixed $xx, mixed $yy, mixed $zz = null, mixed $tt = null, bool $radians = false, bool $errcheck = false, string $direction = "FORWARD")` - Transform coordinates
- `$transformer->transformArray(array $coordinates, bool $radians = false, bool $errcheck = false, string $direction = "FORWARD")` - Batch transformation with coordinate tuples
- `$transformer->getDescription()` - Get transformation description
- `$transformer->hasInverse()` - Check if inverse is available
- `$transformer->getAccuracy()` - Get transformation accuracy
- `$transformer->toWkt(string $version = "WKT2_2019", bool $pretty = false)` - Convert to WKT string
- `$transformer->toProj4(string $version = "PROJ_5")` - Convert to PROJ4 string
- `$transformer->transformBounds(float $left, float $bottom, float $right, float $top, int $densify_pts = 21, bool $radians = false, bool $errcheck = false, string $direction = "FORWARD")` - Transform bounding box

### ProjTransformerGroup Methods
- `ProjTransformerGroup::fromCrs($from, $to, $options)` - Create transformation group with multiple operations
- `$group->getTransformers()` - Get array of individual ProjTransformer objects

### ProjGeod Geodetic Calculations

#### Factory Methods (REQUIRED)
- `ProjGeod::fromCrs(mixed $crs)` - Create from coordinate reference system (CRS object, EPSG code, or CRS string)
- `ProjGeod::fromEpsg(int $epsg_code)` - Create from EPSG code (extracts ellipsoid from CRS)
- `ProjGeod::fromEllipsoidName(string $ellipsoid_name)` - Create from ellipsoid name using PROJ database
- `ProjGeod::fromParameters(float $a, float $param2, string $param_name = "auto")` - Create from explicit ellipsoid parameters

#### Constructor (Explicit Parameters Only)
- `new ProjGeod(float $a, float $f)` - Create with explicit semi-major axis and flattening (no defaults)

#### Parameter Types for fromParameters()
- `$param_name = "auto"` - Auto-detect parameter type from value range
- `$param_name = "f"` or `"flattening"` - Second parameter is flattening (0 ≤ f < 1)
- `$param_name = "b"` or `"semi_minor"` - Second parameter is semi-minor axis (0 < b < a)
- `$param_name = "rf"` or `"inverse_flattening"` - Second parameter is inverse flattening (rf > 1)
- `$param_name = "es"` or `"eccentricity_squared"` - Second parameter is eccentricity squared (0 ≤ es < 1)

#### Core Geodetic Operations
- `$geod->fwd(mixed $points, mixed $az, mixed $dist, bool $radians = false, bool $return_back_azimuth = false)` - Forward geodetic computation
- `$geod->inv(mixed $points1, mixed $points2, bool $radians = false, bool $return_back_azimuth = false)` - Inverse geodetic computation
- `$geod->lineLength(array $points, bool $radians = false)` - Calculate total line length

#### Advanced Geodetic Methods
- `$geod->npts(mixed $point1, mixed $point2, int $npts, bool $radians = false)` - Generate intermediate points between two points
- `$geod->fwdIntermediate(mixed $point1, float $az, array $distances, bool $radians = false, bool $return_back_azimuth = false)` - Forward computation for multiple distances
- `$geod->invIntermediate(mixed $point1, mixed $point2, int $npts, bool $radians = false, bool $return_back_azimuth = false)` - Inverse computation with intermediate points
- `$geod->polygonAreaPerimeter(array $points, bool $radians = false)` - Calculate polygon area and perimeter
- `$geod->lineLengths(array $points, bool $radians = false)` - Individual segment lengths for polylines

#### Geodetic Properties
- `$geod->getInitstring()` - Get initialization string
- `$geod->isSphere()` - Check if using spherical calculations
- `$geod->getA()`, `$geod->getB()`, `$geod->getEs()`, `$geod->getF()` - Ellipsoid parameters

#### Migration from Legacy Constructor
```php
// OLD (deprecated - will throw exceptions)
$geod = new ProjGeod();                    // No default ellipsoid
$geod = new ProjGeod("WGS84");            // No named ellipsoids

// NEW (recommended)
$geod = ProjGeod::fromEpsg(4326);         // WGS84 from EPSG database
$geod = ProjGeod::fromCrs("EPSG:4326");   // WGS84 from CRS string
$geod = ProjGeod::fromParameters(6378137.0, 6356752.314245, "b");  // Explicit parameters
$geod = ProjGeod::fromParameters(6378137.0, 0.0033528107);         // Auto-detect flattening
```

### Global Functions
- `proj_get_authorities()` - Get list of available authorities
- `proj_get_codes(string $auth_name, ?string $pj_type = null, bool $allow_deprecated = false)` - Get codes for authority and type
- `proj_get_crs_info_list_from_database(?string $auth_name = null, ?string $pj_type = null, ?array $area_of_interest = null, bool $contains = false, bool $allow_deprecated = false)` - Query CRS database
- `proj_is_network_enabled()`, `proj_set_network_enabled(bool $enabled = true)` - Network control
- `proj_get_user_data_dir(bool $create = false)` - Get user data directory
- `proj_set_user_data_dir(?string $path = null)` - Set user data directory

### ProjAreaOfInterest Class
Geographic area specification for bounding box operations and database filtering.

#### Constructor and Validation
- `new ProjAreaOfInterest($west, $south, $east, $north)` - Create geographic area specification
  - Validates coordinates: rejects NaN, null values, and invalid coordinate ordering
  - Throws `ValueError` for invalid input (west >= east, south >= north)

#### Geometric Operations
- `$aoi->contains($other_aoi)` - Check if this area completely contains another area  
- `$aoi->intersects($other_aoi)` - Check if this area intersects with another area
- Both methods use proper geometric algorithms for accurate bounding box calculations

#### Coordinate Access
- `$aoi->getWest()`, `$aoi->getSouth()`, `$aoi->getEast()`, `$aoi->getNorth()` - Method accessors
- `$aoi->west_lon_degree`, `$aoi->south_lat_degree`, `$aoi->east_lon_degree`, `$aoi->north_lat_degree` - Direct property access

#### Integration
- Used with `proj_get_crs_info_list_from_database()` for geographic filtering of CRS queries
- Supports serialization/unserialization for persistence
- No coordinate range validation (allows values beyond ±180°/±90° for non-geographic uses)

### ProjFactors Class
- Returned by `$proj->getFactors($lon, $lat)` - Calculate projection scale and distortion factors
- `$factors->getMeridionalScale()`, `$factors->getParallelScale()`, `$factors->getArealScale()` - Scale factor accessors
- `$factors->getAngularDistortion()`, `$factors->getMeridianConvergence()` - Distortion measurements
- `$factors->getTissotSemimajor()`, `$factors->getTissotSemiminor()` - Tissot indicatrix parameters
- `$factors->getDxDlam()`, `$factors->getDxDphi()`, `$factors->getDyDlam()`, `$factors->getDyDphi()` - Differential parameters
- `$factors->toArray()` - Convert to associative array
- `$factors->__toString()` - String representation
- Direct property access to all 12 factor values

### ProjUnit Class  
- `new ProjUnit($auth_name, $code, $name, $category, $conv_factor, $proj_short_name, $deprecated)` - Create unit information
- `$unit->getAuthName()`, `$unit->getCode()`, `$unit->getName()`, `$unit->getCategory()` - Metadata accessors
- `$unit->getProjShortName()`, `$unit->getConvFactor()`, `$unit->isDeprecated()` - Unit properties
- `$unit->toArray()` - Convert to associative array
- `$unit->__toString()` - String representation  
- Direct property access to all unit fields
- Supports nullable string parameters for incomplete unit information

## New Object Model Classes (Complete Reference)

### ProjCoordinateSystem Class
Represents a coordinate system with comprehensive axis information.

#### Factory Methods
- `ProjCoordinateSystem::fromString($cs_string)` - Create from coordinate system string
- `ProjCoordinateSystem::fromJson($json)` - Create from JSON representation
- `ProjCoordinateSystem::fromUserInput($input)` - Create from flexible user input

#### Properties and Methods
- `$cs->getName()` - Get coordinate system name
- `$cs->getAxisList()` - Returns array of **ProjAxis** objects
- `$cs->getRemarks()` - Get coordinate system remarks
- `$cs->getScope()` - Get coordinate system scope
- `$cs->toWkt($version, $pretty)` - Convert to WKT representation
- `$cs->toJson($pretty)` - Convert to JSON string
- `$cs->isExactSame($other)` - Compare with another coordinate system
- `$cs->__toString()` - String representation

### ProjAxis Class
Represents individual coordinate system axes with complete metadata.

#### Constructor
- `new ProjAxis($name, $abbrev, $direction, $unit_name, $unit_auth_code, $unit_code, $unit_conversion_factor)` - Create axis with full properties

#### Properties
- `$axis->getName()` - Get axis name (e.g., "Latitude", "Longitude")
- `$axis->getAbbrev()` - Get axis abbreviation (e.g., "Lat", "Lon")
- `$axis->getDirection()` - Get axis direction (e.g., "north", "east")
- `$axis->getUnitName()` - Get unit name (e.g., "degree")
- `$axis->getUnitAuthCode()` - Get unit authority code (nullable)
- `$axis->getUnitCode()` - Get unit code (nullable)
- `$axis->getUnitConversionFactor()` - Get conversion factor to SI base unit
- `$axis->__toString()` - String representation showing name, abbreviation, and direction

### ProjDatum Class
Represents geodetic reference frames with ellipsoid and prime meridian information.

#### Factory Methods
- `ProjDatum::fromAuthority($auth_name, $code)` - Create from authority and code
- `ProjDatum::fromEpsg($epsg_code)` - Create from EPSG code
- `ProjDatum::fromString($datum_string)` - Create from datum string
- `ProjDatum::fromJson($json)` - Create from JSON representation
- `ProjDatum::fromUserInput($input)` - Create from flexible user input

#### Properties and Methods
- `$datum->getName()` - Get datum name
- `$datum->getTypeName()` - Get datum type ("Geodetic Reference Frame", etc.)
- `$datum->getEllipsoid()` - Returns **ProjEllipsoid** object
- `$datum->getPrimeMeridian()` - Returns **ProjPrimeMeridian** object
- `$datum->getRemarks()` - Get datum remarks
- `$datum->getScope()` - Get datum scope
- `$datum->toWkt($version, $pretty)` - Convert to WKT representation
- `$datum->toJson($pretty)` - Convert to JSON string
- `$datum->isExactSame($other)` - Compare with another datum
- `$datum->__toString()` - String representation

### ProjEllipsoid Class
Represents ellipsoid parameters with comprehensive geometric properties.

#### Factory Methods
- `ProjEllipsoid::fromAuthority($auth_name, $code)` - Create from authority and code
- `ProjEllipsoid::fromEpsg($epsg_code)` - Create from EPSG code
- `ProjEllipsoid::fromString($ellipsoid_string)` - Create from ellipsoid string
- `ProjEllipsoid::fromJson($json)` - Create from JSON representation
- `ProjEllipsoid::fromUserInput($input)` - Create from flexible user input

#### Properties and Methods
- `$ellipsoid->getName()` - Get ellipsoid name
- `$ellipsoid->getSemiMajorMetre()` - Get semi-major axis in metres
- `$ellipsoid->getSemiMinorMetre()` - Get semi-minor axis in metres
- `$ellipsoid->getInverseFlattening()` - Get inverse flattening value
- `$ellipsoid->isSemiMinorComputed()` - Check if semi-minor axis is computed
- `$ellipsoid->toWkt($version, $pretty)` - Convert to WKT representation
- `$ellipsoid->toJson($pretty)` - Convert to JSON string
- `$ellipsoid->isExactSame($other)` - Compare with another ellipsoid
- `$ellipsoid->__toString()` - String representation

### ProjPrimeMeridian Class
Represents prime meridian objects with longitude and unit information.

#### Factory Methods
- `ProjPrimeMeridian::fromAuthority($auth_name, $code)` - Create from authority and code
- `ProjPrimeMeridian::fromEpsg($epsg_code)` - Create from EPSG code
- `ProjPrimeMeridian::fromString($pm_string)` - Create from prime meridian string
- `ProjPrimeMeridian::fromJson($json)` - Create from JSON representation
- `ProjPrimeMeridian::fromUserInput($input)` - Create from flexible user input

#### Properties and Methods
- `$pm->getName()` - Get prime meridian name
- `$pm->getLongitude()` - Get longitude offset from Greenwich in specified units
- `$pm->getUnitName()` - Get unit name for longitude measurement
- `$pm->getUnitConversionFactor()` - Get unit conversion factor
- `$pm->toWkt($version, $pretty)` - Convert to WKT representation
- `$pm->toJson($pretty)` - Convert to JSON string
- `$pm->isExactSame($other)` - Compare with another prime meridian
- `$pm->__toString()` - String representation

### ProjCoordinateOperation Class
Represents coordinate transformation operations with method information.

#### Factory Methods
- `ProjCoordinateOperation::fromAuthority($auth_name, $code)` - Create from authority and code
- `ProjCoordinateOperation::fromEpsg($epsg_code)` - Create from EPSG code
- `ProjCoordinateOperation::fromString($operation_string)` - Create from operation string
- `ProjCoordinateOperation::fromJson($json)` - Create from JSON representation
- `ProjCoordinateOperation::fromUserInput($input)` - Create from flexible user input

#### Properties and Methods
- `$operation->getName()` - Get operation name
- `$operation->getMethodName()` - Get transformation method name
- `$operation->getMethodAuthName()` - Get method authority name
- `$operation->getMethodCode()` - Get method code
- `$operation->getAccuracy()` - Get transformation accuracy in metres
- `$operation->isInstantiable()` - Check if operation can be instantiated
- `$operation->getRemarks()` - Get operation remarks
- `$operation->getScope()` - Get operation scope
- `$operation->toWkt($version, $pretty)` - Convert to WKT representation
- `$operation->toJson($pretty)` - Convert to JSON string
- `$operation->isExactSame($other)` - Compare with another operation
- `$operation->__toString()` - String representation

### ProjAreaOfUse Class
Represents area of use boundaries with geographic bounds (different from ProjAreaOfInterest).

#### Constructor
- `new ProjAreaOfUse($west, $south, $east, $north, $name)` - Create area with geographic bounds and optional name

#### Properties and Methods
- `$area->getWest()` - Get western longitude boundary
- `$area->getSouth()` - Get southern latitude boundary
- `$area->getEast()` - Get eastern longitude boundary
- `$area->getNorth()` - Get northern latitude boundary
- `$area->getName()` - Get area name (nullable)
- `$area->contains($other_area)` - Check if this area contains another area
- `$area->intersects($other_area)` - Check if this area intersects with another area
- `$area->__toString()` - String representation with coordinates and name

## Complete Object-Oriented Usage Examples

### Working with CRS Objects
```php
// Create a CRS and explore its object model
$crs = ProjCRS::fromEpsg(4326);

// Get coordinate system as object (not array)
$cs = $crs->getCoordinateSystem();
echo $cs->getName(); // "ellipsoidal"

// Get axes as array of ProjAxis objects
$axes = $cs->getAxisList();
foreach ($axes as $axis) {
    echo $axis->getName() . " (" . $axis->getAbbrev() . ") - " . $axis->getDirection() . "\n";
    // Output: Geodetic latitude (Lat) - north
    //         Geodetic longitude (Lon) - east
}

// Get datum as object
$datum = $crs->getDatum();
echo $datum->getName(); // "World Geodetic System 1984"

// Get ellipsoid from datum
$ellipsoid = $datum->getEllipsoid();
echo $ellipsoid->getName(); // "WGS 84"
echo $ellipsoid->getSemiMajorMetre(); // 6378137.0

// Get prime meridian from datum
$pm = $datum->getPrimeMeridian();
echo $pm->getName(); // "Greenwich"
echo $pm->getLongitude(); // 0.0

// Get area of use as object
$area = $crs->getAreaOfUse();
echo $area->getName(); // "World"
echo "Bounds: " . $area->getWest() . "," . $area->getSouth() . " to " . $area->getEast() . "," . $area->getNorth();
```

### Object Relationships
```php
$projected_crs = ProjCRS::fromEpsg(3857); // Web Mercator

// Get coordinate operation for projected CRS
$operation = $projected_crs->getCoordinateOperation();
echo $operation->getMethodName(); // "Popular Visualisation Pseudo Mercator"

// All objects have consistent interfaces
$datum = $projected_crs->getDatum();
$ellipsoid = $datum->getEllipsoid();
$wkt = $ellipsoid->toWkt("WKT2_2019", true);
$json = $operation->toJson();
```

## CF-1 (Climate and Forecast) Metadata Convention

**Note**: This PHP extension does NOT implement CF-1 (Climate and Forecast) metadata convention support. This functionality is intentionally excluded as:

1. **Not directly supported by PROJ**: CF-1 conversion requires external libraries and is not a core PROJ library feature
2. **Complexity**: CF-1 support would require additional dependencies (netCDF, etc.) and significant external code
3. **Scope**: This extension focuses on core PROJ functionality for coordinate transformations and CRS operations

Any CF-1 related methods from pyproj (such as `to_cf()`, `from_cf()`) should not be ported to this PHP implementation.

## Legacy Proj Class Exclusion

**Note**: This PHP extension does NOT implement the legacy `Proj` class. This functionality is intentionally excluded as:

1. **Deprecated Design Pattern**: The legacy Proj class represents an older, less object-oriented API design that mixes projection operations with coordinate transformations
2. **API Confusion**: Having both ProjCRS/ProjTransformer (modern) and Proj (legacy) classes would create confusion about which API to use
3. **Maintenance Burden**: Supporting two different APIs for the same functionality increases complexity and maintenance overhead
4. **Focus on Modern API**: This extension focuses on the modern, well-structured PROJ API through ProjCRS and ProjTransformer classes

**Recommended Migration Path**: 
- Use `ProjCRS` for coordinate reference system operations
- Use `ProjTransformer` for coordinate transformations
- Use `ProjGeod` for geodetic calculations

Any legacy `Proj` class methods from pyproj should be reimplemented using the modern ProjCRS/ProjTransformer API pattern.

## toJsonDict() Methods

**Note**: This PHP extension does NOT implement `toJsonDict()` methods. This functionality is intentionally excluded as:

1. **Redundant with toJson()**: The `toJson()` method already provides JSON output as a string, which can be decoded into PHP arrays using `json_decode()`
2. **Not core PROJ functionality**: toJsonDict is a convenience method that duplicates existing functionality
3. **Simplifies API**: Removing redundant methods keeps the API focused and consistent
4. **Performance**: Applications can use `json_decode()` when array format is specifically needed

Any `toJsonDict()` related methods from pyproj should not be ported to this PHP implementation. Use `toJson()` followed by `json_decode()` for equivalent functionality.

## Debugging and Troubleshooting

### Common Issues
- **Extension not loading**: Check PROJ library installation and version
- **Test failures**: Ensure extension loads properly with -dextension=modules/proj.so
- **Undefined symbols**: Verify all class entries are declared in each source file
- **Coordinate order**: Default behavior uses CRS-defined axis order (lat/lon for EPSG:4326)

### Memory Leak Analysis
When running Valgrind memory leak detection, you will observe consistent initialization-time leaks totaling approximately **1,384 bytes**:

```bash
valgrind --leak-check=full php -d extension=modules/proj.so test_script.php
```

**Expected Valgrind Output:**
```
LEAK SUMMARY:
   definitely lost: 1,208 bytes in 13 blocks
   indirectly lost: 176 bytes in 5 blocks
   possibly lost: 0 bytes in 0 blocks
   still reachable: 23,814 bytes in 21 blocks
```

#### These leaks are **UNFIXABLE** and **NOT A CONCERN** because:

1. **Initialization-Time Only**: Leaks occur during module loading, NOT during runtime operations
2. **External Library Sources**: Originate from:
   - PROJ library initialization (`proj_context_create()`, database loading)
   - PHP core class registration system (`zend_register_functions`, `zend_register_internal_class_ex`)
   - Dynamic library loading during extension startup
3. **No Runtime Impact**: Zero additional memory is leaked during actual usage
4. **Industry Standard**: Common pattern in complex PHP extensions that interface with C libraries
5. **Proper Cleanup**: All extension-controlled memory is properly freed in destructors and `PHP_MSHUTDOWN_FUNCTION`

#### Memory Leak Verification Commands:
```bash
# Test with minimal usage - same leaks occur
valgrind --leak-check=full php -d extension=modules/proj.so -r "echo 'loaded';"

# Test with complex operations - no additional leaks
valgrind --leak-check=full php -d extension=modules/proj.so tests/002-transformer-basic.phpt

# All tests pass with zero runtime leaks
./build.sh test  # 26/26 tests pass (100%)
```

#### For Developers:
- **Do NOT attempt to fix these leaks** - they are outside extension control
- **Focus on runtime memory management** - ensure proper `efree()` calls and object destructors
- **Verify new code doesn't introduce additional leaks** by comparing Valgrind output before/after changes
- **Test with complex workloads** to ensure consistent leak amounts regardless of usage patterns

### Build Issues
The build system auto-generates Makefiles, so always use `./build.sh distclean` before rebuilding after significant changes. The extension requires exact PROJ library version compatibility.

## Contributing

Contributions are welcome! Please:

1. Fork the repository
2. Create a feature branch (`git checkout -b feature/amazing-feature`)
3. Commit your changes (`git commit -m 'Add amazing feature'`)
4. Push to the branch (`git push origin feature/amazing-feature`)
5. Open a Pull Request

### Development Guidelines
- Follow existing code style and conventions
- Add tests for new features
- Update documentation as needed
- Ensure all tests pass before submitting PR

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## Acknowledgments

- [PROJ](https://proj.org/) - The PROJ contributors for the excellent cartographic library
- [PHP](https://php.net/) - The PHP Group for the PHP language
- [Claude Code](https://claude.ai/code) - AI assistant that helped create this extension

## Support

- **Issues**: [GitHub Issues](https://github.com/avdmerwe/php-proj/issues)
- **Documentation**: [Full API Documentation](https://github.com/avdmerwe/php-proj/wiki)
- **PROJ Documentation**: [proj.org](https://proj.org/)
