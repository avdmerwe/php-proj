--TEST--
CRS: Advanced CRS operations and edge cases
--SKIPIF--
<?php if (!extension_loaded('proj')) die('skip PROJ extension not loaded'); ?>
--FILE--
<?php

// Test 1: WKT format variations
echo "=== Test WKT format variations ===\n";
$crs = ProjCRS::fromEpsg(4326);
$wkt_default = $crs->toWkt();
$wkt_2019 = $crs->toWkt(ProjWktVersion::WKT2_2019);
echo "WKT default length > 0: " . (strlen($wkt_default) > 0 ? "true" : "false") . "\n";
echo "WKT 2019 length > 0: " . (strlen($wkt_2019) > 0 ? "true" : "false") . "\n";
echo "WKT contains GEOGCRS: " . (strpos($wkt_default, 'GEOGCRS') !== false ? "true" : "false") . "\n";

// Test 2: PROJ version format variations
echo "\n=== Test PROJ4 version formats ===\n";
$proj4_v4 = $crs->toProj4(ProjVersion::PROJ_4);
$proj4_v5 = $crs->toProj4(ProjVersion::PROJ_5);
echo "PROJ4 v4: " . $proj4_v4 . "\n";
echo "PROJ4 v5: " . $proj4_v5 . "\n";
echo "Versions same: " . ($proj4_v4 === $proj4_v5 ? "true" : "false") . "\n";

// Test 3: Complex CRS from authority
echo "\n=== Test complex CRS from authority ===\n";
try {
    $utm_crs = ProjCRS::fromAuthority('EPSG', 32633); // UTM Zone 33N
    echo "UTM Name: " . $utm_crs->getName() . "\n";
    echo "UTM Type: " . $utm_crs->getTypeName() . "\n";
    echo "UTM EPSG: " . $utm_crs->getToEpsg() . "\n";
} catch (Exception $e) {
    echo "UTM from authority failed: " . $e->getMessage() . "\n";
}

// Test 4: CRS comparison with different creation methods
echo "\n=== Test CRS equivalence ===\n";
$crs1 = ProjCRS::fromEpsg(4326);
$crs2 = ProjCRS::fromString("EPSG:4326");
$crs3 = ProjCRS::fromWkt($crs1->toWkt());
echo "EPSG vs String: " . ($crs1->equals($crs2) ? "true" : "false") . "\n";
echo "EPSG vs WKT: " . ($crs1->equals($crs3) ? "true" : "false") . "\n";

// Test 5: Different projection types
echo "\n=== Test different projection types ===\n";
$projections = [
    ['name' => 'Web Mercator', 'epsg' => 3857],
    ['name' => 'UTM 32N', 'epsg' => 32632],
    ['name' => 'Lambert Conformal Conic', 'epsg' => 2154], // RGF93 / Lambert-93
];

foreach ($projections as $proj_info) {
    try {
        $crs = ProjCRS::fromEpsg($proj_info['epsg']);
        echo $proj_info['name'] . " - Type: " . $crs->getTypeName() . "\n";
        echo $proj_info['name'] . " - Is Projected: " . ($crs->isProjected() ? "true" : "false") . "\n";
    } catch (Exception $e) {
        echo $proj_info['name'] . " - Failed: " . $e->getMessage() . "\n";
    }
}

// Test 6: Axis information detailed analysis
echo "\n=== Test detailed axis analysis ===\n";
$projected_crs = ProjCRS::fromEpsg(32632); // UTM 32N
$axis_info = $projected_crs->getAxisInfo();
echo "Projected CRS axis count: " . count($axis_info) . "\n";
if (count($axis_info) >= 2) {
    echo "First axis: " . $axis_info[0]->getName() . " (" . $axis_info[0]->getDirection() . ")\n";
    echo "Second axis: " . $axis_info[1]->getName() . " (" . $axis_info[1]->getDirection() . ")\n";
    echo "First axis unit: " . $axis_info[0]->getUnitName() . "\n";
}

// Test 7: Area of use for different CRS types
echo "\n=== Test area of use variations ===\n";
$global_crs = ProjCRS::fromEpsg(4326); // Global
$regional_crs = ProjCRS::fromEpsg(32632); // UTM 32N - Regional

$global_area = $global_crs->getAreaOfUse();
$regional_area = $regional_crs->getAreaOfUse();

echo "Global area name: " . $global_area->getName() . "\n";
echo "Regional area name: " . $regional_area->getName() . "\n";
echo "Global bounds wider than regional: " . (
    ($global_area->getEast() - $global_area->getWest()) > 
    ($regional_area->getEast() - $regional_area->getWest()) 
    ? "true" : "false") . "\n";

// Test 8: JSON serialization round-trip
echo "\n=== Test JSON round-trip ===\n";
try {
    $original_crs = ProjCRS::fromEpsg(3857);
    $json_str = $original_crs->toJson();
    $restored_crs = ProjCRS::fromJson($json_str);
    echo "JSON round-trip successful: " . ($original_crs->equals($restored_crs) ? "true" : "false") . "\n";
    echo "JSON contains type field: " . (strpos($json_str, '"type"') !== false ? "true" : "false") . "\n";
} catch (Exception $e) {
    echo "JSON round-trip failed: " . $e->getMessage() . "\n";
}

// Test 9: Complex PROJ4 string creation
echo "\n=== Test complex PROJ4 string ===\n";
try {
    $complex_proj4 = "+proj=lcc +lat_1=49 +lat_2=44 +lat_0=46.5 +lon_0=3 +x_0=700000 +y_0=6600000 +ellps=GRS80 +towgs84=0,0,0,0,0,0,0 +units=m +no_defs";
    $crs_from_proj4 = ProjCRS::fromProj4($complex_proj4);
    echo "Complex PROJ4 name: " . $crs_from_proj4->getName() . "\n";
    echo "Complex PROJ4 type: " . $crs_from_proj4->getTypeName() . "\n";
    echo "Complex PROJ4 projected: " . ($crs_from_proj4->isProjected() ? "true" : "false") . "\n";
} catch (Exception $e) {
    echo "Complex PROJ4 failed: " . $e->getMessage() . "\n";
}

// Test 10: Error handling for invalid inputs
echo "\n=== Test comprehensive error handling ===\n";
$invalid_tests = [
    ['method' => 'fromEpsg', 'input' => -1, 'desc' => 'negative EPSG'],
    ['method' => 'fromEpsg', 'input' => 999999, 'desc' => 'non-existent EPSG'],
    ['method' => 'fromString', 'input' => 'INVALID:123', 'desc' => 'invalid authority'],
    ['method' => 'fromWkt', 'input' => 'INVALID WKT', 'desc' => 'malformed WKT'],
];

foreach ($invalid_tests as $test) {
    try {
        $method = $test['method'];
        ProjCRS::$method($test['input']);
        echo $test['desc'] . ": UNEXPECTED SUCCESS\n";
    } catch (ProjCRSException $e) {
        echo $test['desc'] . ": EXPECTED ERROR\n";
    } catch (Exception $e) {
        echo $test['desc'] . ": WRONG EXCEPTION TYPE\n";
    }
}

?>
--EXPECT--
=== Test WKT format variations ===
WKT default length > 0: true
WKT 2019 length > 0: true
WKT contains GEOGCRS: true

=== Test PROJ4 version formats ===
PROJ4 v4: +proj=longlat +datum=WGS84 +no_defs +type=crs
PROJ4 v5: +proj=longlat +datum=WGS84 +no_defs +type=crs
Versions same: true

=== Test complex CRS from authority ===
UTM Name: WGS 84 / UTM zone 33N
UTM Type: Projected CRS
UTM EPSG: 32633

=== Test CRS equivalence ===
EPSG vs String: true
EPSG vs WKT: true

=== Test different projection types ===
Web Mercator - Type: Projected CRS
Web Mercator - Is Projected: true
UTM 32N - Type: Projected CRS
UTM 32N - Is Projected: true
Lambert Conformal Conic - Type: Projected CRS
Lambert Conformal Conic - Is Projected: true

=== Test detailed axis analysis ===
Projected CRS axis count: 2
First axis: Easting (east)
Second axis: Northing (north)
First axis unit: metre

=== Test area of use variations ===
Global area name: World.
Regional area name: Between 6°E and 12°E, northern hemisphere between equator and 84°N, onshore and offshore. Algeria. Austria. Cameroon. Denmark. Equatorial Guinea. France. Gabon. Germany. Italy. Libya. Liechtenstein. Monaco. Netherlands. Niger. Nigeria. Norway. Sao Tome and Principe. Svalbard. Sweden. Switzerland. Tunisia. Vatican City State.
Global bounds wider than regional: true

=== Test JSON round-trip ===
JSON round-trip successful: true
JSON contains type field: true

=== Test complex PROJ4 string ===
Complex PROJ4 failed: Provided PROJ4 string is not a valid CRS

=== Test comprehensive error handling ===
negative EPSG: EXPECTED ERROR
non-existent EPSG: EXPECTED ERROR
invalid authority: EXPECTED ERROR
malformed WKT: EXPECTED ERROR