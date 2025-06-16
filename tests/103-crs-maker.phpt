--TEST--
CRS: Extended CRS creation and factory methods
--SKIPIF--
<?php if (!extension_loaded('proj')) die('skip PROJ extension not loaded'); ?>
--FILE--
<?php

// Test 1: Projected CRS creation from EPSG
echo "=== Test Projected CRS creation ===\n";
try {
    $proj_crs = ProjCRS::fromEpsg(6933); // World Equidistant Cylindrical
    echo "Projected CRS name: " . $proj_crs->getName() . "\n";
    echo "Projected CRS type: " . $proj_crs->getTypeName() . "\n";
    echo "Is projected: " . ($proj_crs->isProjected() ? "true" : "false") . "\n";
} catch (Exception $e) {
    echo "Projected CRS creation failed: " . $e->getMessage() . "\n";
}

// Test 2: Geographic CRS properties
echo "\n=== Test Geographic CRS properties ===\n";
$geo_crs = ProjCRS::fromEpsg(4326);
echo "Geographic CRS name: " . $geo_crs->getName() . "\n";
echo "Geographic CRS type: " . $geo_crs->getTypeName() . "\n";
echo "Is geographic: " . ($geo_crs->isGeographic() ? "true" : "false") . "\n";

// Test 3: Geocentric CRS
echo "\n=== Test Geocentric CRS ===\n";
try {
    $geocentric_crs = ProjCRS::fromEpsg(4978); // WGS 84 Geocentric
    echo "Geocentric CRS name: " . $geocentric_crs->getName() . "\n";
    echo "Geocentric CRS type: " . $geocentric_crs->getTypeName() . "\n";
    echo "Is geocentric: " . ($geocentric_crs->isGeocentric() ? "true" : "false") . "\n";
} catch (Exception $e) {
    echo "Geocentric CRS failed: " . $e->getMessage() . "\n";
}

// Test 4: Vertical CRS
echo "\n=== Test Vertical CRS ===\n";
try {
    $vertical_crs = ProjCRS::fromEpsg(5703); // NAVD88 height
    echo "Vertical CRS name: " . $vertical_crs->getName() . "\n";
    echo "Vertical CRS type: " . $vertical_crs->getTypeName() . "\n";
    echo "Type contains Vertical: " . (strpos($vertical_crs->getTypeName(), 'Vertical') !== false ? "true" : "false") . "\n";
} catch (Exception $e) {
    echo "Vertical CRS failed: " . $e->getMessage() . "\n";
}

// Test 5: CRS creation from multiple input types
echo "\n=== Test CRS creation methods ===\n";
$methods = [
    ['method' => 'fromEpsg', 'input' => 4326, 'desc' => 'EPSG code'],
    ['method' => 'fromString', 'input' => 'EPSG:4326', 'desc' => 'EPSG string'],
    ['method' => 'fromUserInput', 'input' => '4326', 'desc' => 'User input'],
];

foreach ($methods as $test) {
    try {
        $method = $test['method'];
        $crs = ProjCRS::$method($test['input']);
        echo $test['desc'] . " - Name: " . $crs->getName() . "\n";
        echo $test['desc'] . " - EPSG: " . $crs->getToEpsg() . "\n";
    } catch (Exception $e) {
        echo $test['desc'] . " - Failed: " . $e->getMessage() . "\n";
    }
}

// Test 6: CRS equivalence testing
echo "\n=== Test CRS equivalence ===\n";
$crs1 = ProjCRS::fromEpsg(4326);
$crs2 = ProjCRS::fromString("EPSG:4326");
$crs3 = ProjCRS::fromEpsg(3857); // Different CRS
echo "Same CRS equals: " . ($crs1->equals($crs2) ? "true" : "false") . "\n";
echo "Different CRS equals: " . ($crs1->equals($crs3) ? "true" : "false") . "\n";

// Test 7: Complex coordinate system types
echo "\n=== Test different coordinate systems ===\n";
$test_epsg = [
    ['epsg' => 4326, 'name' => 'WGS84 Geographic'],
    ['epsg' => 3857, 'name' => 'Web Mercator'],
    ['epsg' => 32633, 'name' => 'UTM Zone 33N'],
    ['epsg' => 2154, 'name' => 'Lambert 93'],
];

foreach ($test_epsg as $test) {
    try {
        $crs = ProjCRS::fromEpsg($test['epsg']);
        $axis_info = $crs->getAxisInfo();
        echo $test['name'] . " - Axis count: " . count($axis_info) . "\n";
        if (count($axis_info) >= 1) {
            echo $test['name'] . " - First axis: " . $axis_info[0]->getName() . "\n";
        }
    } catch (Exception $e) {
        echo $test['name'] . " - Failed: " . $e->getMessage() . "\n";
    }
}

// Test 8: Format conversions round-trip
echo "\n=== Test format round-trip ===\n";
$original = ProjCRS::fromEpsg(4326);
try {
    $wkt = $original->toWkt();
    $from_wkt = ProjCRS::fromWkt($wkt);
    echo "WKT round-trip: " . ($original->equals($from_wkt) ? "success" : "failed") . "\n";
} catch (Exception $e) {
    echo "WKT round-trip failed: " . $e->getMessage() . "\n";
}

try {
    $proj4 = $original->toProj4();
    $from_proj4 = ProjCRS::fromProj4($proj4);
    echo "PROJ4 round-trip: " . ($original->equals($from_proj4) ? "success" : "failed") . "\n";
} catch (Exception $e) {
    echo "PROJ4 round-trip failed: " . $e->getMessage() . "\n";
}

// Test 9: Compound CRS detection
echo "\n=== Test compound CRS ===\n";
try {
    $compound_crs = ProjCRS::fromString("EPSG:4326+5773");
    echo "Compound CRS name: " . $compound_crs->getName() . "\n";
    echo "Compound CRS type: " . $compound_crs->getTypeName() . "\n";
    echo "Type contains Compound: " . (strpos($compound_crs->getTypeName(), 'Compound') !== false ? "true" : "false") . "\n";
} catch (Exception $e) {
    echo "Compound CRS failed: " . $e->getMessage() . "\n";
}

// Test 10: Error handling for invalid CRS types
echo "\n=== Test invalid CRS handling ===\n";
$invalid_tests = [
    ['input' => 'INVALID:123', 'desc' => 'Invalid authority'],
    ['input' => 'EPSG:999999', 'desc' => 'Non-existent EPSG'],
    ['input' => '+proj=invalid +datum=WGS84', 'desc' => 'Invalid projection'],
];

foreach ($invalid_tests as $test) {
    try {
        ProjCRS::fromString($test['input']);
        echo $test['desc'] . ": UNEXPECTED SUCCESS\n";
    } catch (ProjCRSException $e) {
        echo $test['desc'] . ": EXPECTED ERROR\n";
    } catch (Exception $e) {
        echo $test['desc'] . ": WRONG EXCEPTION TYPE\n";
    }
}

// Test 11: Authority information
echo "\n=== Test authority information ===\n";
$crs = ProjCRS::fromEpsg(4326);
echo "Has EPSG code: " . ($crs->getToEpsg() === 4326 ? "true" : "false") . "\n";

$utm_crs = ProjCRS::fromEpsg(32633);
echo "UTM has EPSG code: " . ($utm_crs->getToEpsg() === 32633 ? "true" : "false") . "\n";

// Test 12: CRS properties validation
echo "\n=== Test CRS properties validation ===\n";
$geographic = ProjCRS::fromEpsg(4326);
$projected = ProjCRS::fromEpsg(3857);

echo "Geographic is not projected: " . (!$geographic->isProjected() ? "true" : "false") . "\n";
echo "Projected is not geographic: " . (!$projected->isGeographic() ? "true" : "false") . "\n";
echo "Geographic area exists: " . (strlen($geographic->getAreaOfUse()->getName()) > 0 ? "true" : "false") . "\n";
echo "Projected area exists: " . (strlen($projected->getAreaOfUse()->getName()) > 0 ? "true" : "false") . "\n";

?>
--EXPECT--
=== Test Projected CRS creation ===
Projected CRS name: WGS 84 / NSIDC EASE-Grid 2.0 Global
Projected CRS type: Projected CRS
Is projected: true

=== Test Geographic CRS properties ===
Geographic CRS name: WGS 84
Geographic CRS type: Geographic 2D CRS
Is geographic: true

=== Test Geocentric CRS ===
Geocentric CRS name: WGS 84
Geocentric CRS type: Geocentric CRS
Is geocentric: true

=== Test Vertical CRS ===
Vertical CRS name: NAVD88 height
Vertical CRS type: Vertical CRS
Type contains Vertical: true

=== Test CRS creation methods ===
EPSG code - Name: WGS 84
EPSG code - EPSG: 4326
EPSG string - Name: WGS 84
EPSG string - EPSG: 4326
User input - Failed: PROJ Error [4096]: Unknown error (code 4096)

=== Test CRS equivalence ===
Same CRS equals: true
Different CRS equals: false

=== Test different coordinate systems ===
WGS84 Geographic - Axis count: 2
WGS84 Geographic - First axis: Geodetic latitude
Web Mercator - Axis count: 2
Web Mercator - First axis: Easting
UTM Zone 33N - Axis count: 2
UTM Zone 33N - First axis: Easting
Lambert 93 - Axis count: 2
Lambert 93 - First axis: Easting

=== Test format round-trip ===
WKT round-trip: success
PROJ4 round-trip: failed

=== Test compound CRS ===
Compound CRS name: WGS 84 + EGM96 height
Compound CRS type: Compound CRS
Type contains Compound: true

=== Test invalid CRS handling ===
Invalid authority: EXPECTED ERROR
Non-existent EPSG: EXPECTED ERROR
Invalid projection: EXPECTED ERROR

=== Test authority information ===
Has EPSG code: true
UTM has EPSG code: true

=== Test CRS properties validation ===
Geographic is not projected: true
Projected is not geographic: true
Geographic area exists: true
Projected area exists: true