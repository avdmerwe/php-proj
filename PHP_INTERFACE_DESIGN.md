# PHP Interface Design for PROJ Bindings

This document defines the PHP interface that provides comprehensive PROJ library integration following PHP extension conventions.

## Core Design Principles

1. **PHP Extension Standards**: Follow standard PHP extension naming conventions
2. **Global Namespace**: Classes in global namespace with `Proj` prefix, functions with `proj_` prefix
3. **Type Safety**: Use PHP type declarations and return types
4. **Memory Safety**: Proper resource management and cleanup
5. **Performance**: Efficient array handling and batch operations
6. **Error Handling**: Comprehensive exception hierarchy

## API Structure

Following standard PHP extension conventions:

### Global Functions
```php
<?php
declare(strict_types=1);

/**
 * Get list of available authorities from PROJ database
 * @return string[] Array of authority names (e.g., ['EPSG', 'IGNF', 'ESRI'])
 */
function proj_get_authorities(): array {}

/**
 * Get codes for a specific authority and type
 * @param string $auth_name Authority name (e.g., 'EPSG')
 * @param string|null $pj_type Optional PROJ object type filter (e.g., 'projected_crs')
 * @param bool $allow_deprecated Whether to include deprecated codes (default: false)
 * @return string[] Array of codes
 */
function proj_get_codes(string $auth_name, ?string $pj_type = null, bool $allow_deprecated = false): array {}

/**
 * Query CRS definitions from database with optional filtering
 * @param string|null $auth_name Authority name or null for all
 * @param string|null $pj_type Optional PROJ object type filter
 * @param array<float>|ProjAreaOfInterest|null $area_of_interest Geographic filter as [west, south, east, north] or ProjAreaOfInterest object
 * @param bool $contains Whether to require full containment in area (default: false)
 * @param bool $allow_deprecated Whether to include deprecated CRS (default: false)
 * @return array<int, array{auth_name: string, code: string, name: string, type: string, area_name: string|null, projection_method_name: string|null}>
 */
function proj_get_crs_info_list_from_database(?string $auth_name = null, ?string $pj_type = null, array|ProjAreaOfInterest|null $area_of_interest = null, bool $contains = false, bool $allow_deprecated = false): array {}

/**
 * Check if network access is enabled for grid downloads
 * @return bool True if network is enabled
 */
function proj_is_network_enabled(): bool {}

/**
 * Enable or disable network access for grid downloads
 * @param bool $enabled Whether to enable network access
 * @return void
 */
function proj_set_network_enabled(bool $enabled = true): void {}

/**
 * Get the user data directory path
 * @param bool $create Whether to create the directory if it doesn't exist
 * @return string|null Directory path or null if not set
 */
function proj_get_user_data_dir(bool $create = false): ?string {}

/**
 * Set the user data directory path
 * @param string|null $path Directory path or null to unset
 * @return void
 * @throws ProjDataDirException If path is invalid
 */
function proj_set_user_data_dir(?string $path = null): void {}
```

### Core Classes (Global Namespace)
```php
<?php
declare(strict_types=1);

// Main classes
class ProjCRS {}           // Coordinate Reference System creation, properties, conversions
class ProjTransformer {}   // Coordinate transformations with full pipeline support
class ProjGeod {}          // Geodetic calculations (distance, bearing, area computations)

// Utility classes
class ProjFactors {}       // Scale and angular distortion factors
class ProjAreaOfInterest {} // Geographic area specifications  
class ProjUnit {}          // Unit conversion and information
class ProjTransformerGroup {} // Advanced transformation group operations

// Object Model classes (NEW)
class ProjCoordinateSystem {} // Coordinate system objects with axis information
class ProjAxis {}          // Individual coordinate axis properties
class ProjDatum {}         // Geodetic reference frame objects
class ProjEllipsoid {}     // Ellipsoid parameter objects
class ProjPrimeMeridian {} // Prime meridian objects
class ProjCoordinateOperation {} // Coordinate transformation operations
class ProjAreaOfUse {}     // Area of use boundaries

// Enumeration classes
class ProjWktVersion {}    // WKT format versions
class ProjVersion {}       // PROJ string versions
class ProjTransformDirection {} // Transform directions
class ProjType {}          // PROJ object types

// Exception classes
class ProjException extends Exception {}
class ProjCRSException extends ProjException {}
class ProjTransformerException extends ProjException {}
class ProjGeodException extends ProjException {}
class ProjDataDirException extends ProjException {}
class ProjPipelineException extends ProjException {}
```

## Implemented Class Interfaces

### 1. ProjCRS Class

```php
<?php
declare(strict_types=1);

/**
 * Coordinate Reference System (CRS) class for PROJ integration
 */
class ProjCRS
{
    /**
     * Create CRS from PROJ parameters
     * @param string|int|array<string, mixed>|ProjCRS $crs CRS input (PROJ string, EPSG code, parameter array, or ProjCRS object)
     * @throws ProjCRSException If CRS creation fails
     */
    public function __construct(string|int|array|ProjCRS $crs) {}
    
    /**
     * Create CRS from EPSG code
     * @param int $epsg_code EPSG code
     * @return self
     * @throws ProjCRSException If EPSG code is invalid
     */
    public static function fromEpsg(int $epsg_code): self {}
    
    /**
     * Create CRS from any supported string format
     * @param string $crs_string CRS string (WKT, PROJ4, URN, etc.)
     * @return self
     * @throws ProjCRSException If string format is invalid
     */
    public static function fromString(string $crs_string): self {}
    
    /**
     * Create CRS from flexible user input
     * @param mixed $value User input (string, int, array, ProjCRS object)
     * @return self
     * @throws ProjCRSException If input cannot be parsed
     */
    public static function fromUserInput(mixed $value): self {}
    
    /**
     * Create CRS from WKT string
     * @param string $wkt WKT string
     * @return self
     * @throws ProjCRSException If WKT is invalid
     */
    public static function fromWkt(string $wkt): self {}
    
    /**
     * Create CRS from PROJ4 string
     * @param string $proj4 PROJ4 string
     * @return self
     * @throws ProjCRSException If PROJ4 string is invalid
     */
    public static function fromProj4(string $proj4): self {}
    
    /**
     * Create CRS from JSON string
     * @param string $json JSON string
     * @return self
     * @throws ProjCRSException If JSON is invalid
     */
    public static function fromJson(string $json): self {}
    
    /**
     * Create CRS from authority and code
     * @param string $authority Authority name (e.g., 'EPSG')
     * @param string $code Authority code
     * @return self
     * @throws ProjCRSException If authority/code combination is invalid
     */
    public static function fromAuthority(string $authority, int $code): self {}
    
    /**
     * Get CRS name
     * @return string|null CRS name or null if not available
     */
    public function getName(): ?string {}
    
    /**
     * Get CRS type name
     * @return string|null Type description (e.g., "Geographic 2D CRS") or null
     */
    public function getTypeName(): ?string {}
    
    /**
     * Get axis information
     * @return ProjAxis[]|null Array of ProjAxis objects or null if not available
     */
    public function getAxisInfo(): ?array {}
    
    /**
     * Get area of use information
     * @return ProjAreaOfUse|null Area of use object with bounds and name, or null if not defined
     */
    public function getAreaOfUse(): ?ProjAreaOfUse {}
    
    /**
     * Get EPSG code if available
     * @return int|null EPSG code or null if not an EPSG CRS
     */
    public function getToEpsg(): ?int {}
    
    /**
     * Get coordinate system object
     * @return ProjCoordinateSystem|null Coordinate system object or null if not available
     */
    public function getCoordinateSystem(): ?ProjCoordinateSystem {}
    
    /**
     * Get datum object
     * @return ProjDatum|null Datum object or null if not available
     */
    public function getDatum(): ?ProjDatum {}
    
    /**
     * Get ellipsoid object
     * @return ProjEllipsoid|null Ellipsoid object or null if not available
     */
    public function getEllipsoid(): ?ProjEllipsoid {}
    
    /**
     * Get prime meridian object
     * @return ProjPrimeMeridian|null Prime meridian object or null if not available
     */
    public function getPrimeMeridian(): ?ProjPrimeMeridian {}
    
    /**
     * Get coordinate operation object (for projected CRS)
     * @return ProjCoordinateOperation|null Coordinate operation object or null if not a projected CRS
     */
    public function getCoordinateOperation(): ?ProjCoordinateOperation {}
    
    /**
     * Check if CRS is geographic
     * @return bool True if geographic CRS
     */
    public function isGeographic(): bool {}
    
    /**
     * Check if CRS is projected
     * @return bool True if projected CRS
     */
    public function isProjected(): bool {}
    
    /**
     * Check if CRS is geocentric
     * @return bool True if geocentric CRS
     */
    public function isGeocentric(): bool {}
    
    /**
     * Check if CRS is compound
     * @return bool True if compound CRS
     */
    public function isCompound(): bool {}
    
    /**
     * Check if CRS is engineering
     * @return bool True if engineering CRS
     */
    public function isEngineering(): bool {}
    
    /**
     * Check if CRS is vertical
     * @return bool True if vertical CRS
     */
    public function isVertical(): bool {}
    
    /**
     * Check if CRS is bound
     * @return bool True if bound CRS
     */
    public function isBound(): bool {}
    
    /**
     * Check if CRS is derived
     * @return bool True if derived CRS
     */
    public function isDerived(): bool {}
    
    /**
     * Get geodetic CRS
     * @return ProjCRS|null Geodetic CRS object or null if not available
     */
    public function getGeodeticCrs(): ?ProjCRS {}
    
    /**
     * Get source CRS (for bound CRS)
     * @return ProjCRS|null Source CRS object or null if not a bound CRS
     */
    public function getSourceCrs(): ?ProjCRS {}
    
    /**
     * Get target CRS (for bound CRS)
     * @return ProjCRS|null Target CRS object or null if not a bound CRS
     */
    public function getTargetCrs(): ?ProjCRS {}
    
    /**
     * Get sub-CRS list (for compound CRS)
     * @return ProjCRS[] Array of sub-CRS objects
     */
    public function getSubCrsList(): array {}
    
    /**
     * Get CRS remarks
     * @return string|null Remarks or null if not available
     */
    public function getRemarks(): ?string {}
    
    /**
     * Get CRS scope
     * @return string|null Scope or null if not available
     */
    public function getScope(): ?string {}
    
    /**
     * Get authority information
     * @return array|null Authority information as [auth_name, code] or null if not available
     */
    public function getToAuthority(): ?array {}
    
    /**
     * Convert to WKT string
     * @param string $version WKT version (use ProjWktVersion constants, default: WKT2_2019)
     * @param bool $pretty Whether to format with newlines and indentation (default: false)
     * @return string|null WKT representation or null on failure
     * @throws ProjCRSException If conversion fails
     */
    public function toWkt(string $version = ProjWktVersion::WKT2_2019, bool $pretty = false): ?string {}
    
    /**
     * Convert to PROJ4 string
     * @param string $version PROJ version (use ProjVersion constants, default: PROJ_5)
     * @return string|null PROJ4 representation or null on failure
     * @throws ProjCRSException If conversion fails
     */
    public function toProj4(string $version = ProjVersion::PROJ_5): ?string {}
    
    /**
     * Convert to JSON string
     * @param bool $pretty Whether to format with indentation (default: false)
     * @return string JSON representation
     * @throws ProjCRSException If conversion fails
     */
    public function toJson(bool $pretty = false): string {}
    
    /**
     * Compare with another CRS
     * @param ProjCRS $other CRS to compare with
     * @param bool $ignore_axis_order Whether to ignore axis order differences (default: false)
     * @return bool True if CRSs are equivalent
     */
    public function equals(ProjCRS $other, bool $ignore_axis_order = false): bool {}
    
    /**
     * Check if exactly the same as another CRS
     * @param ProjCRS $other CRS to compare with
     * @return bool True if CRSs are exactly the same
     */
    public function isExactSame(ProjCRS $other): bool {}
    
    /**
     * Calculate projection factors at a point
     * @param float $longitude Longitude in degrees
     * @param float $latitude Latitude in degrees
     * @return ProjFactors|null Projection factors object or null if not available
     * @throws ProjCRSException If calculation fails
     */
    public function getFactors(float $longitude, float $latitude): ?ProjFactors {}
    
    /**
     * Get string representation
     * @return string Default string representation
     */
    public function __toString(): string {}
}
```

### 2. ProjTransformer Class

```php
<?php
declare(strict_types=1);

/**
 * Coordinate transformer for converting between coordinate reference systems
 */
class ProjTransformer
{
    /**
     * Create transformer between two CRSs
     * @param string|int|ProjCRS $crs_from Source CRS
     * @param string|int|ProjCRS $crs_to Target CRS
     * @param bool $always_xy Force XY axis order regardless of CRS definition (default: false)
     * @param array<float>|null $area_of_interest Geographic area as [west, south, east, north] for operation selection
     * @param string|null $authority Authority for operation selection (e.g., 'EPSG')
     * @param float $accuracy Minimum accuracy in meters (default: 0.0)
     * @param bool $allow_ballpark Whether to allow ballpark transformations (default: false)
     * @param bool $force_over Force longitude wraparound (default: false)
     * @param bool $only_best Return only the best transformation (default: false)
     * @return self
     * @throws ProjTransformerException If transformer creation fails
     */
    public static function fromCrs(
        string|int|ProjCRS $crs_from, 
        string|int|ProjCRS $crs_to, 
        bool $always_xy = false,
        ?array $area_of_interest = null,
        ?string $authority = null,
        float $accuracy = 0.0,
        bool $allow_ballpark = false,
        bool $force_over = false,
        bool $only_best = false
    ): self {}
    
    /**
     * Create transformer from PROJ pipeline string
     * @param string $pipeline PROJ pipeline definition
     * @return self
     * @throws ProjPipelineException If pipeline is invalid
     */
    public static function fromPipeline(string $pipeline): self {}
    
    /**
     * Get transformation description
     * @return string|null Human-readable description or null
     */
    public function getDescription(): ?string {}
    
    /**
     * Check if inverse transformation is available
     * @return bool True if inverse is available
     */
    public function hasInverse(): bool {}
    
    /**
     * Get transformation accuracy
     * @return float|null Accuracy in meters or null if unknown
     */
    public function getAccuracy(): ?float {}
    
    /**
     * Transform coordinates
     * @param float|array<float> $xx X coordinate(s) or longitude(s)
     * @param float|array<float> $yy Y coordinate(s) or latitude(s)
     * @param float|array<float>|null $zz Z coordinate(s) (height/elevation)
     * @param float|array<float>|null $tt Time coordinate(s)
     * @param bool $radians Whether coordinates are in radians (default: false)
     * @param bool $errcheck Whether to check for errors (default: false)
     * @param string $direction Transform direction (use ProjTransformDirection constants, default: FORWARD)
     * @return array{0: float|array<float>, 1: float|array<float>, 2?: float|array<float>, 3?: float|array<float>} Transformed coordinates
     * @throws ProjTransformerException If transformation fails
     */
    public function transform(
        float|array $xx, 
        float|array $yy, 
        float|array|null $zz = null, 
        float|array|null $tt = null, 
        bool $radians = false, 
        bool $errcheck = false, 
        string $direction = ProjTransformDirection::FORWARD
    ): array {}
    
    /**
     * Transform array of coordinate tuples
     * @param array<int, array{0: float, 1: float, 2?: float, 3?: float}> $coordinates Array of coordinate tuples
     * @param bool $radians Whether coordinates are in radians (default: false)
     * @param bool $errcheck Whether to check for errors (default: false)
     * @param string $direction Transform direction (use ProjTransformDirection constants, default: FORWARD)
     * @return array<int, array{0: float, 1: float, 2?: float, 3?: float}> Transformed coordinates
     * @throws ProjTransformerException If transformation fails
     */
    public function transformArray(array $coordinates, bool $radians = false, bool $errcheck = false, string $direction = ProjTransformDirection::FORWARD): array {}
    
    /**
     * Convert to WKT string
     * @param string $version WKT version (use ProjWktVersion constants, default: WKT2_2019)
     * @param bool $pretty Whether to format with newlines and indentation (default: false)
     * @return string WKT representation
     * @throws ProjTransformerException If conversion fails
     */
    public function toWkt(string $version = ProjWktVersion::WKT2_2019, bool $pretty = false): string {}
    
    /**
     * Convert to PROJ4 string
     * @param string $version PROJ version (use ProjVersion constants, default: PROJ_5)
     * @return string PROJ4 representation
     * @throws ProjTransformerException If conversion fails
     */
    public function toProj4(string $version = ProjVersion::PROJ_5): string {}
    
    /**
     * Transform bounding box
     * @param float $left Left/minimum X coordinate
     * @param float $bottom Bottom/minimum Y coordinate
     * @param float $right Right/maximum X coordinate
     * @param float $top Top/maximum Y coordinate
     * @param int $densify_pts Number of points to add along edges (default: 21)
     * @param bool $radians Whether coordinates are in radians (default: false)
     * @param bool $errcheck Whether to check for errors (default: false)
     * @param string $direction Transform direction (use ProjTransformDirection constants, default: FORWARD)
     * @return array{left: float, bottom: float, right: float, top: float} Transformed bounds
     * @throws ProjTransformerException If transformation fails
     */
    public function transformBounds(
        float $left, 
        float $bottom, 
        float $right, 
        float $top, 
        int $densify_pts = 21,
        bool $radians = false,
        bool $errcheck = false,
        string $direction = ProjTransformDirection::FORWARD
    ): array {}
    
    /**
     * Get string representation
     * @return string Default string representation
     */
    public function __toString(): string {}
}
```

### 3. ProjGeod Class

```php
<?php
declare(strict_types=1);

/**
 * Geodetic calculator for distance, bearing, and area computations
 */
class ProjGeod
{
    /**
     * Create geodetic calculator with explicit ellipsoid parameters
     * @param float $a Semi-major axis in meters (must be > 0)
     * @param float $f Flattening (must be between 0 and 1)
     * @throws ProjGeodException If parameters are invalid
     */
    public function __construct(float $a, float $f) {}
    
    /**
     * Create geodetic calculator from coordinate reference system
     * @param mixed $crs CRS object, EPSG code, or CRS string
     * @return ProjGeod
     * @throws ProjGeodException If CRS is invalid or has no ellipsoid
     */
    public static function fromCrs(mixed $crs): ProjGeod {}
    
    /**
     * Create geodetic calculator from EPSG code
     * @param int $epsg_code EPSG code (must be > 0)
     * @return ProjGeod
     * @throws ProjGeodException If EPSG code is invalid or has no ellipsoid
     */
    public static function fromEpsg(int $epsg_code): ProjGeod {}
    
    /**
     * Create geodetic calculator from ellipsoid name
     * @param string $ellipsoid_name Ellipsoid name (uses PROJ database)
     * @return ProjGeod
     * @throws ProjGeodException If ellipsoid name is unknown
     */
    public static function fromEllipsoidName(string $ellipsoid_name): ProjGeod {}
    
    /**
     * Create geodetic calculator from ellipsoid parameters
     * @param float $a Semi-major axis in meters (must be > 0)
     * @param float $param2 Second parameter (type determined by $param_name)
     * @param string $param_name Parameter type: "auto", "f", "flattening", "b", "semi_minor", "rf", "inverse_flattening", "es", "eccentricity_squared"
     * @return ProjGeod
     * @throws ProjGeodException If parameters are invalid
     */
    public static function fromParameters(float $a, float $param2, string $param_name = "auto"): ProjGeod {}
    
    /**
     * Get initialization string
     * @return string Initialization parameters
     */
    public function getInitstring(): string {}
    
    /**
     * Check if using spherical calculations
     * @return bool True if sphere, false if ellipsoid
     */
    public function isSphere(): bool {}
    
    /**
     * Get semi-major axis
     * @return float Semi-major axis in meters
     */
    public function getA(): float {}
    
    /**
     * Get semi-minor axis
     * @return float Semi-minor axis in meters
     */
    public function getB(): float {}
    
    /**
     * Get flattening
     * @return float Flattening value
     */
    public function getF(): float {}
    
    /**
     * Get eccentricity squared
     * @return float Eccentricity squared
     */
    public function getEs(): float {}
    
    /**
     * Forward geodetic computation
     * @param mixed $points Point coordinates as [lon, lat] tuple or array of tuples
     * @param mixed $az Azimuth(s) in degrees
     * @param mixed $dist Distance(s) in meters
     * @param bool $radians Whether angles are in radians (default: false)
     * @param bool $return_back_azimuth Whether to return back azimuth (default: false)
     * @return array Array of result tuples [lon, lat, back_azimuth?]
     * @throws ProjGeodException If computation fails
     */
    public function fwd(
        mixed $points, 
        mixed $az, 
        mixed $dist,
        bool $radians = false,
        bool $return_back_azimuth = false
    ): array {}
    
    /**
     * Inverse geodetic computation
     * @param mixed $points1 Start point coordinates as [lon, lat] tuple or array of tuples
     * @param mixed $points2 End point coordinates as [lon, lat] tuple or array of tuples
     * @param bool $radians Whether angles are in radians (default: false)
     * @param bool $return_back_azimuth Whether to return back azimuth (default: false)
     * @return array Array of result tuples [forward_azimuth, back_azimuth?, distance]
     * @throws ProjGeodException If computation fails
     */
    public function inv(
        mixed $points1, 
        mixed $points2,
        bool $radians = false,
        bool $return_back_azimuth = false
    ): array {}
    
    /**
     * Calculate total line length
     * @param array $points Array of coordinate tuples [[lon, lat], ...]
     * @param bool $radians Whether angles are in radians (default: false)
     * @return float Total length in meters
     * @throws ProjGeodException If calculation fails
     */
    public function lineLength(array $points, bool $radians = false): float {}
    
    /**
     * Calculate individual segment lengths
     * @param array $points Array of coordinate tuples [[lon, lat], ...]
     * @param bool $radians Whether angles are in radians (default: false)
     * @return array Segment lengths in meters
     * @throws ProjGeodException If calculation fails
     */
    public function lineLengths(array $points, bool $radians = false): array {}
    
    /**
     * Generate intermediate points between two locations
     * @param mixed $point1 Start point as [lon, lat] tuple
     * @param mixed $point2 End point as [lon, lat] tuple
     * @param int $npts Number of intermediate points
     * @param bool $radians Whether angles are in radians (default: false)
     * @return array Array of coordinate tuples including endpoints
     * @throws ProjGeodException If generation fails
     */
    public function npts(
        mixed $point1, 
        mixed $point2, 
        int $npts,
        bool $radians = false
    ): array {}
    
    /**
     * Forward computation for multiple distances
     * @param mixed $point1 Start point as [lon, lat] tuple
     * @param float $az Azimuth in degrees
     * @param array $distances Array of distances in meters
     * @param bool $radians Whether angles are in radians (default: false)
     * @param bool $return_back_azimuth Whether to return back azimuth (default: false)
     * @return array Array of coordinate tuples
     * @throws ProjGeodException If computation fails
     */
    public function fwdIntermediate(
        mixed $point1, 
        float $az, 
        array $distances,
        bool $radians = false,
        bool $return_back_azimuth = false
    ): array {}
    
    /**
     * Inverse computation with intermediate points
     * @param mixed $point1 Start point as [lon, lat] tuple
     * @param mixed $point2 End point as [lon, lat] tuple
     * @param int $npts Number of intermediate points
     * @param bool $radians Whether angles are in radians (default: false)
     * @param bool $return_back_azimuth Whether to return back azimuth (default: false)
     * @return array Array of intermediate coordinate tuples
     * @throws ProjGeodException If computation fails
     */
    public function invIntermediate(
        mixed $point1, 
        mixed $point2, 
        int $npts,
        bool $radians = false,
        bool $return_back_azimuth = false
    ): array {}
    
    /**
     * Calculate polygon area and perimeter
     * @param array $points Array of coordinate tuples [[lon, lat], ...]
     * @param bool $radians Whether angles are in radians (default: false)
     * @return array Associative array with 'area' and 'perimeter' keys
     * @throws ProjGeodException If calculation fails
     */
    public function polygonAreaPerimeter(array $points, bool $radians = false): array {}
    
    /**
     * Get string representation
     * @return string Default string representation
     */
    public function __toString(): string {}
}
```

### 4. Enumeration Classes

```php
<?php
declare(strict_types=1);

/**
 * WKT format version constants
 */
class ProjWktVersion
{
    public const WKT1_GDAL = "WKT1_GDAL";
    public const WKT1_ESRI = "WKT1_ESRI";
    public const WKT2_2015 = "WKT2_2015";
    public const WKT2_2015_SIMPLIFIED = "WKT2_2015_SIMPLIFIED";
    public const WKT2_2019 = "WKT2_2019";
    public const WKT2_2019_SIMPLIFIED = "WKT2_2019_SIMPLIFIED";
}

/**
 * PROJ string version constants
 */
class ProjVersion
{
    public const PROJ_4 = "PROJ_4";
    public const PROJ_5 = "PROJ_5";
}

/**
 * Transform direction constants
 */
class ProjTransformDirection
{
    public const FORWARD = "FORWARD";
    public const INVERSE = "INVERSE";
    public const IDENT = "IDENT";
}

/**
 * PROJ object type constants
 */
class ProjType
{
    public const UNKNOWN = "unknown";
    public const ELLIPSOID = "ellipsoid";
    public const PRIME_MERIDIAN = "prime_meridian";
    public const GEODETIC_REFERENCE_FRAME = "geodetic_reference_frame";
    public const CRS = "coordinate_reference_system";
    public const GEODETIC_CRS = "geodetic_crs";
    public const GEOCENTRIC_CRS = "geocentric_crs";
    public const GEOGRAPHIC_CRS = "geographic_crs";
    public const GEOGRAPHIC_2D_CRS = "geographic_2d_crs";
    public const GEOGRAPHIC_3D_CRS = "geographic_3d_crs";
    public const VERTICAL_CRS = "vertical_crs";
    public const PROJECTED_CRS = "projected_crs";
    public const COMPOUND_CRS = "compound_crs";
    public const TEMPORAL_CRS = "temporal_crs";
    public const ENGINEERING_CRS = "engineering_crs";
    public const BOUND_CRS = "bound_crs";
    public const OTHER_CRS = "other_crs";
    public const CONVERSION = "conversion";
    public const TRANSFORMATION = "transformation";
    public const CONCATENATED_OPERATION = "concatenated_operation";
    public const OTHER_COORDINATE_OPERATION = "other_coordinate_operation";
}
```

### 5. ProjAreaOfInterest Class
```php
<?php
declare(strict_types=1);

/**
 * Geographic area specification for spatial filtering
 */
class ProjAreaOfInterest {
    /**
     * Create geographic area
     * @param float $west_lon_degree Western longitude in degrees
     * @param float $south_lat_degree Southern latitude in degrees
     * @param float $east_lon_degree Eastern longitude in degrees
     * @param float $north_lat_degree Northern latitude in degrees
     * @throws ProjException If coordinates are invalid
     */
    public function __construct(float $west_lon_degree, float $south_lat_degree, 
                               float $east_lon_degree, float $north_lat_degree) {}
    
    /**
     * Check if this area contains another area
     * @param ProjAreaOfInterest $other Area to test
     * @return bool True if this area fully contains the other
     */
    public function contains(ProjAreaOfInterest $other): bool {}
    
    /**
     * Check if this area intersects with another area
     * @param ProjAreaOfInterest $other Area to test
     * @return bool True if areas overlap
     */
    public function intersects(ProjAreaOfInterest $other): bool {}
    
    /**
     * Get western longitude
     * @return float Western longitude in degrees
     */
    public function getWest(): float {}
    
    /**
     * Get southern latitude
     * @return float Southern latitude in degrees
     */
    public function getSouth(): float {}
    
    /**
     * Get eastern longitude
     * @return float Eastern longitude in degrees
     */
    public function getEast(): float {}
    
    /**
     * Get northern latitude
     * @return float Northern latitude in degrees
     */
    public function getNorth(): float {}
    
    /**
     * @var float Western longitude in degrees
     */
    public float $west_lon_degree;
    
    /**
     * @var float Southern latitude in degrees
     */
    public float $south_lat_degree;
    
    /**
     * @var float Eastern longitude in degrees
     */
    public float $east_lon_degree;
    
    /**
     * @var float Northern latitude in degrees
     */
    public float $north_lat_degree;
}
```

### 6. ProjFactors Class
```php
<?php
declare(strict_types=1);

/**
 * Projection scale and distortion factors
 */
class ProjFactors {
    /**
     * Get meridional scale factor
     * @return float Scale factor along meridian
     */
    public function getMeridionalScale(): float {}
    
    /**
     * Get parallel scale factor
     * @return float Scale factor along parallel
     */
    public function getParallelScale(): float {}
    
    /**
     * Get areal scale factor
     * @return float Area scale factor
     */
    public function getArealScale(): float {}
    
    /**
     * Get angular distortion
     * @return float Angular distortion in radians
     */
    public function getAngularDistortion(): float {}
    
    /**
     * Get meridian-parallel angle
     * @return float Angle between meridian and parallel in radians
     */
    public function getMeridianParallelAngle(): float {}
    
    /**
     * Get meridian convergence
     * @return float Convergence angle in radians
     */
    public function getMeridianConvergence(): float {}
    
    /**
     * Get Tissot indicatrix semi-major axis
     * @return float Semi-major axis length
     */
    public function getTissotSemimajor(): float {}
    
    /**
     * Get Tissot indicatrix semi-minor axis
     * @return float Semi-minor axis length
     */
    public function getTissotSemiminor(): float {}
    
    /**
     * Get dx/dlambda differential
     * @return float Partial derivative
     */
    public function getDxDlam(): float {}
    
    /**
     * Get dx/dphi differential
     * @return float Partial derivative
     */
    public function getDxDphi(): float {}
    
    /**
     * Get dy/dlambda differential
     * @return float Partial derivative
     */
    public function getDyDlam(): float {}
    
    /**
     * Get dy/dphi differential
     * @return float Partial derivative
     */
    public function getDyDphi(): float {}
    
    /**
     * Convert to associative array
     * @return array<string, float> All factor values
     */
    public function toArray(): array {}
    
    /**
     * Get string representation
     * @return string Human-readable factor summary
     */
    public function __toString(): string {}
}
```

### 7. ProjUnit Class
```php
<?php
declare(strict_types=1);

/**
 * Unit definition and conversion information
 */
class ProjUnit {
    /**
     * Create unit information object
     * @param string|null $auth_name Authority name (e.g., 'EPSG')
     * @param string|null $code Authority code
     * @param string|null $name Unit name
     * @param string|null $category Unit category (e.g., 'linear', 'angular')
     * @param float $conv_factor Conversion factor to base unit
     * @param string|null $proj_short_name PROJ short name
     * @param bool $deprecated Whether unit is deprecated
     */
    public function __construct(?string $auth_name, ?string $code, ?string $name,
                               ?string $category, float $conv_factor, 
                               ?string $proj_short_name, bool $deprecated = false) {}
    
    /**
     * Get authority name
     * @return string|null Authority name or null
     */
    public function getAuthName(): ?string {}
    
    /**
     * Get authority code
     * @return string|null Authority code or null
     */
    public function getCode(): ?string {}
    
    /**
     * Get unit name
     * @return string|null Unit name or null
     */
    public function getName(): ?string {}
    
    /**
     * Get unit category
     * @return string|null Category or null
     */
    public function getCategory(): ?string {}
    
    /**
     * Get PROJ short name
     * @return string|null Short name or null
     */
    public function getProjShortName(): ?string {}
    
    /**
     * Get conversion factor
     * @return float Conversion factor to base unit
     */
    public function getConvFactor(): float {}
    
    /**
     * Check if deprecated
     * @return bool True if deprecated
     */
    public function isDeprecated(): bool {}
    
    /**
     * Convert to associative array
     * @return array<string, mixed> Unit data
     */
    public function toArray(): array {}
    
    /**
     * Get string representation
     * @return string Human-readable unit description
     */
    public function __toString(): string {}
}
```

### 8. ProjTransformerGroup Class
```php
<?php
declare(strict_types=1);

/**
 * Group of alternative coordinate transformations
 */
class ProjTransformerGroup {
    /**
     * Create transformation group between CRSs
     * @param string|int|ProjCRS $crs_from Source CRS
     * @param string|int|ProjCRS $crs_to Target CRS
     * @param bool $always_xy Force XY axis order (default: false)
     * @param array<float>|null $area_of_interest Geographic area as [west, south, east, north]
     * @param string|null $authority Authority for operation selection
     * @param float $accuracy Minimum accuracy in meters (default: 0.0)
     * @param bool $allow_ballpark Whether to allow ballpark transformations (default: false)
     * @param bool $force_over Force longitude wraparound (default: false)
     * @param bool $only_best Return only the best transformation (default: false)
     * @return self
     * @throws ProjTransformerException If group creation fails
     */
    public static function fromCrs(
        string|int|ProjCRS $crs_from, 
        string|int|ProjCRS $crs_to,
        bool $always_xy = false,
        ?array $area_of_interest = null,
        ?string $authority = null,
        float $accuracy = 0.0,
        bool $allow_ballpark = false,
        bool $force_over = false,
        bool $only_best = false
    ): self {}
    
    /**
     * Get all available transformers
     * @return ProjTransformer[] Array of transformer objects
     */
    public function getTransformers(): array {}
    
    /**
     * Download required transformation grids
     * @param bool $verbose Whether to output progress information (default: false)
     * @return bool True if all grids downloaded successfully
     */
    public function downloadGrids(bool $verbose = false): bool {}
}
```

## Usage Examples

### Basic CRS Operations
```php
<?php
declare(strict_types=1);

// Create CRS using various methods
$crs1 = ProjCRS::fromEpsg(4326);
$crs2 = ProjCRS::fromWkt($wkt_string);
$crs3 = ProjCRS::fromProj4('+proj=longlat +datum=WGS84 +no_defs +type=crs');
$crs4 = ProjCRS::fromAuthority('EPSG', 4326);

// Get CRS information
echo $crs1->getName();           // "WGS 84"
echo $crs1->getTypeName();       // "Geographic 2D CRS"
print_r($crs1->getAxisInfo());   // Array of axis information
print_r($crs1->getAreaOfUse());  // Geographic bounds
echo $crs1->getToEpsg();         // 4326
```

### Coordinate Transformations
```php
<?php
declare(strict_types=1);

// Create transformer
$transformer = ProjTransformer::fromCrs('EPSG:4326', 'EPSG:3857');

// Transform single point (forward)
[$x, $y] = $transformer->transform(-74.0, 40.7);

// Transform single point (inverse)
[$lon, $lat] = $transformer->transform($x, $y, direction: ProjTransformDirection::INVERSE);

// Transform multiple points with tuple-based API
$coordinates = [[-74.0, 40.7], [-73.9, 40.8]];
$result = $transformer->transformArray($coordinates);

// Transform bounding box
$bounds = $transformer->transformBounds(-180.0, -85.0, 180.0, 85.0);

// Get transformer information
echo $transformer->getDescription();
echo $transformer->toWkt();
echo $transformer->toProj4();
```

### Global Functions
```php
<?php
declare(strict_types=1);

// Get available authorities
$authorities = proj_get_authorities();

// Get codes for an authority
$codes = proj_get_codes('EPSG', 'projected_crs');

// Network settings
proj_set_network_enabled(true);
$enabled = proj_is_network_enabled();

// Database queries with area of interest filtering
$area_of_interest = [-180.0, -90.0, 180.0, 90.0]; // [west, south, east, north]
$crs_list = proj_get_crs_info_list_from_database('EPSG', 'projected_crs', $area_of_interest);
```

### Advanced Geodetic Calculations
```php
<?php
declare(strict_types=1);

// Create geodetic calculator (NEW factory methods)
$geod = ProjGeod::fromEpsg(4326);                                    // WGS84 from EPSG
$geod = ProjGeod::fromCrs("EPSG:4269");                             // NAD83 from CRS string
$geod = ProjGeod::fromParameters(6378137.0, 6356752.314245, "b");   // Explicit parameters
$geod = ProjGeod::fromParameters(6378137.0, 0.0033528107);          // Auto-detect flattening

// Forward geodetic computation
$result = $geod->fwd([[-74.0, 40.7]], 45.0, 10000.0); // 10km at 45° bearing
[$lon2, $lat2, $back_azimuth] = $result[0];

// Inverse geodetic computation  
$result = $geod->inv([[-74.0, 40.7]], [[-73.9, 40.8]]);
[$forward_az, $back_az, $distance] = $result[0];

// Generate intermediate points between two locations
$points = $geod->npts([-74.0, 40.7], [-73.9, 40.8], 5); // 5 points between + endpoints

// Calculate polygon area and perimeter
$polygon_points = [[-74.0, 40.7], [-73.9, 40.8], [-73.8, 40.7], [-74.0, 40.7]];
$result = $geod->polygonAreaPerimeter($polygon_points);
echo "Area: {$result['area']} sq meters, Perimeter: {$result['perimeter']} meters";

// Individual segment lengths
$segment_lengths = $geod->lineLengths($polygon_points);
```

### Transformer Groups and Advanced Operations
```php
<?php
declare(strict_types=1);

// Create transformation group with multiple operations
$group = ProjTransformerGroup::fromCrs('EPSG:4326', 'EPSG:3857');

// Get all available transformations
$transformers = $group->getTransformers();
echo "Found " . count($transformers) . " transformation operations";

// Advanced projection operations
$proj = new Proj('+proj=utm +zone=33 +datum=WGS84');

// Extract CRS from projection
$crs = $proj->getCrs();

// Transform bounding box
$bounds = $proj->transformBounds(-180.0, -85.0, 180.0, 85.0, 21); // 21 densification points

// Convert to lat/long
$latlong_proj = $proj->toLatlong();

// Calculate distortion factors at a point
$factors = $proj->getFactors(0.0, 45.0); // Returns associative array
echo "Scale factor: " . $factors['meridional_scale'];

// Batch transformation for large datasets with tuple-based API
$coordinates = [];
for ($lon = -180; $lon <= 180; $lon++) {
    $coordinates[] = [(float)$lon, 0.0]; // Points along equator
}
$result = $transformer->transformArray($coordinates);

// 3D and 4D coordinate tuples also supported
$coordinates_3d = [
    [2.3522, 48.8566, 100.0],        // Paris with elevation
    [-74.0060, 40.7128, 50.0]        // NYC with elevation
];
$result_3d = $transformer->transformArray($coordinates_3d);

$coordinates_4d = [
    [2.3522, 48.8566, 100.0, 2023.0], // With time coordinate
    [-74.0060, 40.7128, 50.0, 2024.0]
];
$result_4d = $transformer->transformArray($coordinates_4d);
```

### Error Handling
```php
<?php
declare(strict_types=1);

try {
    $crs = ProjCRS::fromEpsg(4326);
    $transformer = ProjTransformer::fromCrs($crs, 'EPSG:3857');
    [$x, $y] = $transformer->transform(-74.0, 40.7);
} catch (ProjCRSException $e) {
    echo "CRS Error: " . $e->getMessage();
} catch (ProjTransformerException $e) {
    echo "Transformer Error: " . $e->getMessage();
} catch (ProjException $e) {
    echo "General PROJ Error: " . $e->getMessage();
}
```

### Modern API Migration Examples
```php
<?php
declare(strict_types=1);

// Legacy Proj class functionality replaced with modern API:

// Instead of: new Proj('+proj=merc +ellps=WGS84')
// Use modern CRS and Transformer approach:
$mercator_crs = ProjCRS::fromProj4('+proj=merc +ellps=WGS84');
$wgs84_crs = ProjCRS::fromEpsg(4326);
$transformer = ProjTransformer::fromCrs($wgs84_crs, $mercator_crs);

// Transform coordinates
[$x, $y] = $transformer->transform(0.0, 0.0);

// Transform coordinates (inverse)
[$lon, $lat] = $transformer->transform($x, $y, direction: 'INVERSE');

// Transform array of coordinates
$coords = [[0.0, 0.0], [0.1, 0.1]];
$result = $transformer->transformArray($coords, radians: true);

// Get CRS information (instead of projection info)
echo $mercator_crs->getName();
echo $mercator_crs->toProj4();
echo $transformer->hasInverse() ? "Has inverse" : "No inverse";
```

## Object Model Classes (NEW)

### 1. ProjCoordinateSystem Class

```php
<?php
declare(strict_types=1);

/**
 * Coordinate system class representing axis definitions and properties
 */
class ProjCoordinateSystem
{
    /**
     * Create coordinate system from constructor parameters
     * @param string|array<string, mixed> $coordinate_system_params Coordinate system parameters
     * @throws ProjCRSException If creation fails
     */
    public function __construct(string|array $coordinate_system_params) {}
    
    /**
     * Create coordinate system from string
     * @param string $cs_string Coordinate system string
     * @return self
     * @throws ProjCRSException If parsing fails
     */
    public static function fromString(string $cs_string): self {}
    
    /**
     * Create coordinate system from JSON
     * @param string $json JSON representation
     * @return self
     * @throws ProjCRSException If parsing fails
     */
    public static function fromJson(string $json): self {}
    
    /**
     * Create coordinate system from user input
     * @param string|array<string, mixed> $input Flexible input format
     * @return self
     * @throws ProjCRSException If parsing fails
     */
    public static function fromUserInput(string|array $input): self {}
    
    /**
     * Get coordinate system name
     * @return string|null Name or null if not available
     */
    public function getName(): ?string {}
    
    /**
     * Get axis list as array of ProjAxis objects
     * @return ProjAxis[] Array of axis objects
     */
    public function getAxisList(): array {}
    
    /**
     * Get coordinate system remarks
     * @return string|null Remarks or null if not available
     */
    public function getRemarks(): ?string {}
    
    /**
     * Get coordinate system scope
     * @return string|null Scope or null if not available
     */
    public function getScope(): ?string {}
    
    /**
     * Convert to WKT representation
     * @param string $version WKT version (default: WKT2_2019)
     * @param bool $pretty Pretty formatting (default: false)
     * @return string|null WKT string or null on failure
     */
    public function toWkt(string $version = ProjWktVersion::WKT2_2019, bool $pretty = false): ?string {}
    
    /**
     * Convert to JSON string
     * @param bool $pretty Pretty formatting (default: false)
     * @return string|null JSON string or null on failure
     */
    public function toJson(bool $pretty = false): ?string {}
    
    /**
     * Check if exactly the same as another coordinate system
     * @param ProjCoordinateSystem $other Other coordinate system
     * @return bool True if exactly the same
     */
    public function isExactSame(ProjCoordinateSystem $other): bool {}
    
    /**
     * Get string representation
     * @return string String representation
     */
    public function __toString(): string {}
}
```

### 2. ProjAxis Class

```php
<?php
declare(strict_types=1);

/**
 * Coordinate axis class representing individual axis properties
 */
class ProjAxis
{
    /**
     * Create axis with complete properties
     * @param string $name Axis name (e.g., "Latitude", "Longitude")
     * @param string $abbrev Axis abbreviation (e.g., "Lat", "Lon")
     * @param string $direction Axis direction (e.g., "north", "east")
     * @param string $unit_name Unit name (e.g., "degree")
     * @param string|null $unit_auth_code Unit authority code (nullable)
     * @param string|null $unit_code Unit code (nullable)
     * @param float $unit_conversion_factor Conversion factor to SI base unit
     * @throws ProjCRSException If parameters are invalid
     */
    public function __construct(string $name, string $abbrev, string $direction, string $unit_name, ?string $unit_auth_code, ?string $unit_code, float $unit_conversion_factor) {}
    
    /**
     * Get axis name
     * @return string Axis name
     */
    public function getName(): string {}
    
    /**
     * Get axis abbreviation
     * @return string Axis abbreviation
     */
    public function getAbbrev(): string {}
    
    /**
     * Get axis direction
     * @return string Axis direction
     */
    public function getDirection(): string {}
    
    /**
     * Get unit name
     * @return string Unit name
     */
    public function getUnitName(): string {}
    
    /**
     * Get unit authority code
     * @return string|null Unit authority code or null
     */
    public function getUnitAuthCode(): ?string {}
    
    /**
     * Get unit code
     * @return string|null Unit code or null
     */
    public function getUnitCode(): ?string {}
    
    /**
     * Get unit conversion factor
     * @return float Conversion factor to SI base unit
     */
    public function getUnitConversionFactor(): float {}
    
    /**
     * Get string representation
     * @return string String representation showing name, abbreviation, and direction
     */
    public function __toString(): string {}
}
```

### 3. ProjDatum Class

```php
<?php
declare(strict_types=1);

/**
 * Datum class representing geodetic reference frames
 */
class ProjDatum
{
    /**
     * Create datum from constructor parameters
     * @param string|array<string, mixed> $datum_params Datum parameters
     * @throws ProjCRSException If creation fails
     */
    public function __construct(string|array $datum_params) {}
    
    /**
     * Create datum from authority and code
     * @param string $auth_name Authority name (e.g., "EPSG")
     * @param string $code Authority code
     * @return self
     * @throws ProjCRSException If creation fails
     */
    public static function fromAuthority(string $auth_name, int $code): self {}
    
    /**
     * Create datum from EPSG code
     * @param int $epsg_code EPSG code
     * @return self
     * @throws ProjCRSException If creation fails
     */
    public static function fromEpsg(int $epsg_code): self {}
    
    /**
     * Create datum from string representation
     * @param string $datum_string Datum string
     * @return self
     * @throws ProjCRSException If parsing fails
     */
    public static function fromString(string $datum_string): self {}
    
    /**
     * Create datum from JSON representation
     * @param string $json JSON string
     * @return self
     * @throws ProjCRSException If parsing fails
     */
    public static function fromJson(string $json): self {}
    
    /**
     * Create datum from flexible user input
     * @param string|int|array<string, mixed> $input Flexible input format
     * @return self
     * @throws ProjCRSException If parsing fails
     */
    public static function fromUserInput(string|int|array $input): self {}
    
    /**
     * Get datum name
     * @return string|null Datum name or null if not available
     */
    public function getName(): ?string {}
    
    /**
     * Get datum type name
     * @return string|null Type description or null if not available
     */
    public function getTypeName(): ?string {}
    
    /**
     * Get associated ellipsoid
     * @return ProjEllipsoid|null Ellipsoid object or null if not available
     */
    public function getEllipsoid(): ?ProjEllipsoid {}
    
    /**
     * Get associated prime meridian
     * @return ProjPrimeMeridian|null Prime meridian object or null if not available
     */
    public function getPrimeMeridian(): ?ProjPrimeMeridian {}
    
    /**
     * Get datum remarks
     * @return string|null Remarks or null if not available
     */
    public function getRemarks(): ?string {}
    
    /**
     * Get datum scope
     * @return string|null Scope or null if not available
     */
    public function getScope(): ?string {}
    
    /**
     * Convert to WKT representation
     * @param string $version WKT version (default: WKT2_2019)
     * @param bool $pretty Pretty formatting (default: false)
     * @return string|null WKT string or null on failure
     */
    public function toWkt(string $version = ProjWktVersion::WKT2_2019, bool $pretty = false): ?string {}
    
    /**
     * Convert to JSON string
     * @param bool $pretty Pretty formatting (default: false)
     * @return string|null JSON string or null on failure
     */
    public function toJson(bool $pretty = false): ?string {}
    
    /**
     * Check if exactly the same as another datum
     * @param ProjDatum $other Other datum
     * @return bool True if exactly the same
     */
    public function isExactSame(ProjDatum $other): bool {}
    
    /**
     * Get string representation
     * @return string String representation
     */
    public function __toString(): string {}
}
```

### 4. ProjEllipsoid Class

```php
<?php
declare(strict_types=1);

/**
 * Ellipsoid class representing ellipsoid parameters and geometry
 */
class ProjEllipsoid
{
    /**
     * Create ellipsoid from constructor parameters
     * @param string|array<string, mixed> $ellipsoid_params Ellipsoid parameters
     * @throws ProjCRSException If creation fails
     */
    public function __construct(string|array $ellipsoid_params) {}
    
    /**
     * Create ellipsoid from authority and code
     * @param string $auth_name Authority name (e.g., "EPSG")
     * @param string $code Authority code
     * @return self
     * @throws ProjCRSException If creation fails
     */
    public static function fromAuthority(string $auth_name, int $code): self {}
    
    /**
     * Create ellipsoid from EPSG code
     * @param int $epsg_code EPSG code
     * @return self
     * @throws ProjCRSException If creation fails
     */
    public static function fromEpsg(int $epsg_code): self {}
    
    /**
     * Create ellipsoid from string representation
     * @param string $ellipsoid_string Ellipsoid string
     * @return self
     * @throws ProjCRSException If parsing fails
     */
    public static function fromString(string $ellipsoid_string): self {}
    
    /**
     * Create ellipsoid from JSON representation
     * @param string $json JSON string
     * @return self
     * @throws ProjCRSException If parsing fails
     */
    public static function fromJson(string $json): self {}
    
    /**
     * Create ellipsoid from flexible user input
     * @param string|int|array<string, mixed> $input Flexible input format
     * @return self
     * @throws ProjCRSException If parsing fails
     */
    public static function fromUserInput(string|int|array $input): self {}
    
    /**
     * Get ellipsoid name
     * @return string|null Ellipsoid name or null if not available
     */
    public function getName(): ?string {}
    
    /**
     * Get semi-major axis length in metres
     * @return float|null Semi-major axis or null if not available
     */
    public function getSemiMajorMetre(): ?float {}
    
    /**
     * Get semi-minor axis length in metres
     * @return float|null Semi-minor axis or null if not available
     */
    public function getSemiMinorMetre(): ?float {}
    
    /**
     * Get inverse flattening value
     * @return float|null Inverse flattening or null if not available
     */
    public function getInverseFlattening(): ?float {}
    
    /**
     * Check if semi-minor axis is computed from flattening
     * @return bool True if semi-minor axis is computed
     */
    public function isSemiMinorComputed(): bool {}
    
    /**
     * Convert to WKT representation
     * @param string $version WKT version (default: WKT2_2019)
     * @param bool $pretty Pretty formatting (default: false)
     * @return string|null WKT string or null on failure
     */
    public function toWkt(string $version = ProjWktVersion::WKT2_2019, bool $pretty = false): ?string {}
    
    /**
     * Convert to JSON string
     * @param bool $pretty Pretty formatting (default: false)
     * @return string|null JSON string or null on failure
     */
    public function toJson(bool $pretty = false): ?string {}
    
    /**
     * Check if exactly the same as another ellipsoid
     * @param ProjEllipsoid $other Other ellipsoid
     * @return bool True if exactly the same
     */
    public function isExactSame(ProjEllipsoid $other): bool {}
    
    /**
     * Get string representation
     * @return string String representation
     */
    public function __toString(): string {}
}
```

### 5. ProjPrimeMeridian Class

```php
<?php
declare(strict_types=1);

/**
 * Prime meridian class representing prime meridian definition
 */
class ProjPrimeMeridian
{
    /**
     * Create prime meridian from constructor parameters
     * @param string|array<string, mixed> $prime_meridian_params Prime meridian parameters
     * @throws ProjCRSException If creation fails
     */
    public function __construct(string|array $prime_meridian_params) {}
    
    /**
     * Create prime meridian from authority and code
     * @param string $auth_name Authority name (e.g., "EPSG")
     * @param string $code Authority code
     * @return self
     * @throws ProjCRSException If creation fails
     */
    public static function fromAuthority(string $auth_name, int $code): self {}
    
    /**
     * Create prime meridian from EPSG code
     * @param int $epsg_code EPSG code
     * @return self
     * @throws ProjCRSException If creation fails
     */
    public static function fromEpsg(int $epsg_code): self {}
    
    /**
     * Create prime meridian from string representation
     * @param string $pm_string Prime meridian string
     * @return self
     * @throws ProjCRSException If parsing fails
     */
    public static function fromString(string $pm_string): self {}
    
    /**
     * Create prime meridian from JSON representation
     * @param string $json JSON string
     * @return self
     * @throws ProjCRSException If parsing fails
     */
    public static function fromJson(string $json): self {}
    
    /**
     * Create prime meridian from flexible user input
     * @param string|int|array<string, mixed> $input Flexible input format
     * @return self
     * @throws ProjCRSException If parsing fails
     */
    public static function fromUserInput(string|int|array $input): self {}
    
    /**
     * Get prime meridian name
     * @return string|null Prime meridian name or null if not available
     */
    public function getName(): ?string {}
    
    /**
     * Get longitude offset from Greenwich
     * @return float|null Longitude offset or null if not available
     */
    public function getLongitude(): ?float {}
    
    /**
     * Get unit name for longitude measurement
     * @return string|null Unit name or null if not available
     */
    public function getUnitName(): ?string {}
    
    /**
     * Get unit conversion factor
     * @return float|null Conversion factor or null if not available
     */
    public function getUnitConversionFactor(): ?float {}
    
    /**
     * Convert to WKT representation
     * @param string $version WKT version (default: WKT2_2019)
     * @param bool $pretty Pretty formatting (default: false)
     * @return string|null WKT string or null on failure
     */
    public function toWkt(string $version = ProjWktVersion::WKT2_2019, bool $pretty = false): ?string {}
    
    /**
     * Convert to JSON string
     * @param bool $pretty Pretty formatting (default: false)
     * @return string|null JSON string or null on failure
     */
    public function toJson(bool $pretty = false): ?string {}
    
    /**
     * Check if exactly the same as another prime meridian
     * @param ProjPrimeMeridian $other Other prime meridian
     * @return bool True if exactly the same
     */
    public function isExactSame(ProjPrimeMeridian $other): bool {}
    
    /**
     * Get string representation
     * @return string String representation
     */
    public function __toString(): string {}
}
```

### 6. ProjCoordinateOperation Class

```php
<?php
declare(strict_types=1);

/**
 * Coordinate operation class representing transformation operations
 */
class ProjCoordinateOperation
{
    /**
     * Create coordinate operation from constructor parameters
     * @param string|array<string, mixed> $operation_params Operation parameters
     * @throws ProjCRSException If creation fails
     */
    public function __construct(string|array $operation_params) {}
    
    /**
     * Create coordinate operation from authority and code
     * @param string $auth_name Authority name (e.g., "EPSG")
     * @param string $code Authority code
     * @return self
     * @throws ProjCRSException If creation fails
     */
    public static function fromAuthority(string $auth_name, int $code): self {}
    
    /**
     * Create coordinate operation from EPSG code
     * @param int $epsg_code EPSG code
     * @return self
     * @throws ProjCRSException If creation fails
     */
    public static function fromEpsg(int $epsg_code): self {}
    
    /**
     * Create coordinate operation from string representation
     * @param string $operation_string Operation string
     * @return self
     * @throws ProjCRSException If parsing fails
     */
    public static function fromString(string $operation_string): self {}
    
    /**
     * Create coordinate operation from JSON representation
     * @param string $json JSON string
     * @return self
     * @throws ProjCRSException If parsing fails
     */
    public static function fromJson(string $json): self {}
    
    /**
     * Create coordinate operation from flexible user input
     * @param string|int|array<string, mixed> $input Flexible input format
     * @return self
     * @throws ProjCRSException If parsing fails
     */
    public static function fromUserInput(string|int|array $input): self {}
    
    /**
     * Get operation name
     * @return string|null Operation name or null if not available
     */
    public function getName(): ?string {}
    
    /**
     * Get transformation method name
     * @return string|null Method name or null if not available
     */
    public function getMethodName(): ?string {}
    
    /**
     * Get method authority name
     * @return string|null Method authority name or null if not available
     */
    public function getMethodAuthName(): ?string {}
    
    /**
     * Get method code
     * @return string|null Method code or null if not available
     */
    public function getMethodCode(): ?string {}
    
    /**
     * Get transformation accuracy in metres
     * @return float|null Accuracy or null if not available
     */
    public function getAccuracy(): ?float {}
    
    /**
     * Check if operation can be instantiated
     * @return bool True if operation is instantiable
     */
    public function isInstantiable(): bool {}
    
    /**
     * Get operation remarks
     * @return string|null Remarks or null if not available
     */
    public function getRemarks(): ?string {}
    
    /**
     * Get operation scope
     * @return string|null Scope or null if not available
     */
    public function getScope(): ?string {}
    
    /**
     * Convert to WKT representation
     * @param string $version WKT version (default: WKT2_2019)
     * @param bool $pretty Pretty formatting (default: false)
     * @return string|null WKT string or null on failure
     */
    public function toWkt(string $version = ProjWktVersion::WKT2_2019, bool $pretty = false): ?string {}
    
    /**
     * Convert to JSON string
     * @param bool $pretty Pretty formatting (default: false)
     * @return string|null JSON string or null on failure
     */
    public function toJson(bool $pretty = false): ?string {}
    
    /**
     * Check if exactly the same as another operation
     * @param ProjCoordinateOperation $other Other operation
     * @return bool True if exactly the same
     */
    public function isExactSame(ProjCoordinateOperation $other): bool {}
    
    /**
     * Get string representation
     * @return string String representation
     */
    public function __toString(): string {}
}
```

### 7. ProjAreaOfUse Class

```php
<?php
declare(strict_types=1);

/**
 * Area of use class representing geographic boundaries
 */
class ProjAreaOfUse
{
    /**
     * Create area of use with geographic bounds
     * @param float $west Western longitude boundary
     * @param float $south Southern latitude boundary
     * @param float $east Eastern longitude boundary
     * @param float $north Northern latitude boundary
     * @param string|null $name Optional area name
     * @throws ProjCRSException If coordinate bounds are invalid
     */
    public function __construct(float $west, float $south, float $east, float $north, ?string $name = null) {}
    
    /**
     * Get western longitude boundary
     * @return float Western longitude
     */
    public function getWest(): float {}
    
    /**
     * Get southern latitude boundary
     * @return float Southern latitude
     */
    public function getSouth(): float {}
    
    /**
     * Get eastern longitude boundary
     * @return float Eastern longitude
     */
    public function getEast(): float {}
    
    /**
     * Get northern latitude boundary
     * @return float Northern latitude
     */
    public function getNorth(): float {}
    
    /**
     * Get area name
     * @return string|null Area name or null if not set
     */
    public function getName(): ?string {}
    
    /**
     * Check if this area contains another area
     * @param ProjAreaOfUse $other Other area to test
     * @return bool True if this area completely contains the other area
     */
    public function contains(ProjAreaOfUse $other): bool {}
    
    /**
     * Check if this area intersects with another area
     * @param ProjAreaOfUse $other Other area to test
     * @return bool True if areas intersect (overlap)
     */
    public function intersects(ProjAreaOfUse $other): bool {}
    
    /**
     * Get string representation
     * @return string String representation with coordinates and name
     */
    public function __toString(): string {}
}
```
