--TEST--
Enums: Constants and enumerations
--SKIPIF--
<?php if (!extension_loaded("proj")) print "skip"; ?>
--FILE--
<?php
// Using global enum classes: ProjWktVersion, ProjVersion, ProjTransformDirection, ProjType

// Test WktVersion constants
echo "Testing WktVersion constants\n";
echo "WKT1_GDAL: " . ProjWktVersion::WKT1_GDAL . "\n";
echo "WKT1_ESRI: " . ProjWktVersion::WKT1_ESRI . "\n";
echo "WKT2_2015: " . ProjWktVersion::WKT2_2015 . "\n";
echo "WKT2_2015_SIMPLIFIED: " . ProjWktVersion::WKT2_2015_SIMPLIFIED . "\n";
echo "WKT2_2019: " . ProjWktVersion::WKT2_2019 . "\n";
echo "WKT2_2019_SIMPLIFIED: " . ProjWktVersion::WKT2_2019_SIMPLIFIED . "\n";

// Test ProjVersion constants
echo "\nTesting ProjVersion constants\n";
echo "PROJ_4: " . ProjVersion::PROJ_4 . "\n";
echo "PROJ_5: " . ProjVersion::PROJ_5 . "\n";

// Test TransformDirection constants
echo "\nTesting TransformDirection constants\n";
echo "FORWARD: " . ProjTransformDirection::FORWARD . "\n";
echo "INVERSE: " . ProjTransformDirection::INVERSE . "\n";
echo "IDENT: " . ProjTransformDirection::IDENT . "\n";

// Test ProjType constants
echo "\nTesting ProjType constants\n";
echo "UNKNOWN: " . ProjType::UNKNOWN . "\n";
echo "ELLIPSOID: " . ProjType::ELLIPSOID . "\n";
echo "PRIME_MERIDIAN: " . ProjType::PRIME_MERIDIAN . "\n";
echo "GEODETIC_REFERENCE_FRAME: " . ProjType::GEODETIC_REFERENCE_FRAME . "\n";
echo "CRS: " . ProjType::CRS . "\n";
echo "GEODETIC_CRS: " . ProjType::GEODETIC_CRS . "\n";
echo "GEOCENTRIC_CRS: " . ProjType::GEOCENTRIC_CRS . "\n";
echo "GEOGRAPHIC_CRS: " . ProjType::GEOGRAPHIC_CRS . "\n";
echo "GEOGRAPHIC_2D_CRS: " . ProjType::GEOGRAPHIC_2D_CRS . "\n";
echo "GEOGRAPHIC_3D_CRS: " . ProjType::GEOGRAPHIC_3D_CRS . "\n";
echo "VERTICAL_CRS: " . ProjType::VERTICAL_CRS . "\n";
echo "PROJECTED_CRS: " . ProjType::PROJECTED_CRS . "\n";
echo "COMPOUND_CRS: " . ProjType::COMPOUND_CRS . "\n";
echo "TEMPORAL_CRS: " . ProjType::TEMPORAL_CRS . "\n";
echo "ENGINEERING_CRS: " . ProjType::ENGINEERING_CRS . "\n";
echo "BOUND_CRS: " . ProjType::BOUND_CRS . "\n";
echo "OTHER_CRS: " . ProjType::OTHER_CRS . "\n";
echo "CONVERSION: " . ProjType::CONVERSION . "\n";
echo "TRANSFORMATION: " . ProjType::TRANSFORMATION . "\n";
echo "CONCATENATED_OPERATION: " . ProjType::CONCATENATED_OPERATION . "\n";
echo "OTHER_COORDINATE_OPERATION: " . ProjType::OTHER_COORDINATE_OPERATION . "\n";

// Test using constants with CRS
echo "\nTesting using constants with CRS\n";
try {
    $crs = ProjCRS::fromEpsg(4326);
    $wkt_default = $crs->toWkt();
    $wkt_2019 = $crs->toWkt(ProjWktVersion::WKT2_2019);
    $wkt_simplified = $crs->toWkt(ProjWktVersion::WKT2_2019_SIMPLIFIED);
    
    echo "Default WKT length: " . strlen($wkt_default) . "\n";
    echo "WKT2_2019 length: " . strlen($wkt_2019) . "\n";
    echo "WKT2_2019_SIMPLIFIED length: " . strlen($wkt_simplified) . "\n";
    echo "Simplified is shorter: " . (strlen($wkt_simplified) < strlen($wkt_2019) ? "Yes" : "No") . "\n";
    
} catch (Exception $e) {
    echo "Error: " . $e->getMessage() . "\n";
}

// Test using constants with ProjCRS
echo "\nTesting using constants with ProjCRS\n";
try {
    $crs = ProjCRS::fromEpsg(4326);
    $proj4_v4 = $crs->toProj4(ProjVersion::PROJ_4);
    $proj4_v5 = $crs->toProj4(ProjVersion::PROJ_5);
    
    echo "PROJ_4 string: " . $proj4_v4 . "\n";
    echo "PROJ_5 string: " . $proj4_v5 . "\n";
    echo "Strings are same: " . ($proj4_v4 === $proj4_v5 ? "Yes" : "No") . "\n";
    
} catch (Exception $e) {
    echo "Error: " . $e->getMessage() . "\n";
}

// Test using constants with ProjTransformer
echo "\nTesting using constants with ProjTransformer\n";
try {
    $transformer = \ProjTransformer::fromCrs('EPSG:4326', 'EPSG:3857');
    
    $forward = $transformer->transform(-74.0, 40.7, null, null, false, false, ProjTransformDirection::FORWARD);
    $inverse = $transformer->transform($forward[0], $forward[1], null, null, false, false, ProjTransformDirection::INVERSE);
    
    echo "Forward result: [" . round($forward[0], 2) . ", " . round($forward[1], 2) . "]\n";
    echo "Inverse result: [" . round($inverse[0], 6) . ", " . round($inverse[1], 6) . "]\n";
    echo "Round trip successful: " . (abs($inverse[0] - (-74.0)) < 0.000001 ? "Yes" : "No") . "\n";
    
} catch (Exception $e) {
    echo "Error: " . $e->getMessage() . "\n";
}

echo "\nAll tests completed\n";
?>
--EXPECT--
Testing WktVersion constants
WKT1_GDAL: WKT1_GDAL
WKT1_ESRI: WKT1_ESRI
WKT2_2015: WKT2_2015
WKT2_2015_SIMPLIFIED: WKT2_2015_SIMPLIFIED
WKT2_2019: WKT2_2019
WKT2_2019_SIMPLIFIED: WKT2_2019_SIMPLIFIED

Testing ProjVersion constants
PROJ_4: PROJ_4
PROJ_5: PROJ_5

Testing TransformDirection constants
FORWARD: FORWARD
INVERSE: INVERSE
IDENT: IDENT

Testing ProjType constants
UNKNOWN: unknown
ELLIPSOID: ellipsoid
PRIME_MERIDIAN: prime_meridian
GEODETIC_REFERENCE_FRAME: geodetic_reference_frame
CRS: coordinate_reference_system
GEODETIC_CRS: geodetic_crs
GEOCENTRIC_CRS: geocentric_crs
GEOGRAPHIC_CRS: geographic_crs
GEOGRAPHIC_2D_CRS: geographic_2d_crs
GEOGRAPHIC_3D_CRS: geographic_3d_crs
VERTICAL_CRS: vertical_crs
PROJECTED_CRS: projected_crs
COMPOUND_CRS: compound_crs
TEMPORAL_CRS: temporal_crs
ENGINEERING_CRS: engineering_crs
BOUND_CRS: bound_crs
OTHER_CRS: other_crs
CONVERSION: conversion
TRANSFORMATION: transformation
CONCATENATED_OPERATION: concatenated_operation
OTHER_COORDINATE_OPERATION: other_coordinate_operation

Testing using constants with CRS
Default WKT length: 1046
WKT2_2019 length: 1046
WKT2_2019_SIMPLIFIED length: 831
Simplified is shorter: Yes

Testing using constants with ProjCRS
PROJ_4 string: +proj=longlat +datum=WGS84 +no_defs +type=crs
PROJ_5 string: +proj=longlat +datum=WGS84 +no_defs +type=crs
Strings are same: Yes

Testing using constants with ProjTransformer
Forward result: [4530703.28, -12515545.21]
Inverse result: [-74, 40.7]
Round trip successful: Yes

All tests completed