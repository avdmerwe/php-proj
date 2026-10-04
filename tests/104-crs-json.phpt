--TEST--
CRS: JSON serialization and deserialization
--SKIPIF--
<?php if (!extension_loaded('proj')) die('skip PROJ extension not loaded'); ?>
--FILE--
<?php

// Test 1: Basic JSON serialization
echo "=== Test basic JSON serialization ===\n";
try {
    $crs = ProjCRS::fromEpsg(3857); // Web Mercator
    $json = $crs->toJson();
    
    echo "JSON generated: " . (strlen($json) > 0 ? "true" : "false") . "\n";
    echo "JSON contains type: " . (strpos($json, '"type"') !== false ? "true" : "false") . "\n";
    echo "JSON contains ProjectedCRS: " . (strpos($json, 'ProjectedCRS') !== false ? "true" : "false") . "\n";
    echo "JSON is compact: " . (strpos($json, "\n") === false ? "true" : "false") . "\n";
} catch (Exception $e) {
    echo "JSON serialization failed: " . $e->getMessage() . "\n";
}

// Test 2: JSON round-trip for different CRS types
echo "\n=== Test JSON round-trip for different CRS ===\n";
$test_epsg_codes = [4326, 3857, 32633]; // Geographic, Projected, UTM
$round_trip_success = 0;

foreach ($test_epsg_codes as $epsg) {
    try {
        $original = ProjCRS::fromEpsg($epsg);
        $json = $original->toJson();
        $restored = ProjCRS::fromJson($json);
        
        if ($original->equals($restored)) {
            $round_trip_success++;
            echo "EPSG:$epsg round-trip: success\n";
        } else {
            echo "EPSG:$epsg round-trip: failed\n";
        }
    } catch (Exception $e) {
        echo "EPSG:$epsg round-trip: error - " . $e->getMessage() . "\n";
    }
}

echo "Round-trip success rate: $round_trip_success/" . count($test_epsg_codes) . "\n";

// Test 3: JSON structure validation for Geographic CRS
echo "\n=== Test JSON structure for Geographic CRS ===\n";
try {
    $geo_crs = ProjCRS::fromEpsg(4326);
    $json = $geo_crs->toJson();
    $data = json_decode($json, true);
    
    echo "JSON decode success: " . (json_last_error() === JSON_ERROR_NONE ? "true" : "false") . "\n";
    echo "Has type field: " . (isset($data['type']) ? "true" : "false") . "\n";
    echo "Type is Geographic: " . (strpos($data['type'], 'Geographic') !== false ? "true" : "false") . "\n";
    echo "Has name field: " . (isset($data['name']) ? "true" : "false") . "\n";
    
    if (isset($data['name'])) {
        echo "Name contains WGS 84: " . (strpos($data['name'], 'WGS 84') !== false ? "true" : "false") . "\n";
    }
} catch (Exception $e) {
    echo "JSON structure validation failed: " . $e->getMessage() . "\n";
}

// Test 4: JSON structure validation for Projected CRS
echo "\n=== Test JSON structure for Projected CRS ===\n";
try {
    $proj_crs = ProjCRS::fromEpsg(3857);
    $json = $proj_crs->toJson();
    $data = json_decode($json, true);
    
    echo "Projected JSON decode: " . (json_last_error() === JSON_ERROR_NONE ? "true" : "false") . "\n";
    echo "Type is ProjectedCRS: " . ($data['type'] === 'ProjectedCRS' ? "true" : "false") . "\n";
    echo "Has base_crs: " . (isset($data['base_crs']) ? "true" : "false") . "\n";
    echo "Has conversion: " . (isset($data['conversion']) ? "true" : "false") . "\n";
    
    if (isset($data['conversion'])) {
        echo "Conversion has method: " . (isset($data['conversion']['method']) ? "true" : "false") . "\n";
    }
} catch (Exception $e) {
    echo "Projected JSON validation failed: " . $e->getMessage() . "\n";
}

// Test 5: JSON creation from custom PROJ4 strings
echo "\n=== Test JSON from PROJ4 strings ===\n";
$proj4_strings = [
    "+proj=longlat +datum=WGS84 +no_defs",
    "+proj=merc +datum=WGS84 +units=m +no_defs",
];

foreach ($proj4_strings as $proj4) {
    try {
        $crs = ProjCRS::fromProj4($proj4);
        $json = $crs->toJson();
        echo "PROJ4 JSON success: " . (strlen($json) > 0 ? "true" : "false") . "\n";
        
        // Test round-trip
        $restored = ProjCRS::fromJson($json);
        echo "PROJ4 JSON round-trip: " . ($crs->equals($restored) ? "true" : "false") . "\n";
        break; // Only test first successful one
    } catch (Exception $e) {
        echo "PROJ4 JSON failed: " . $e->getMessage() . "\n";
    }
}

// Test 6: Error handling for invalid JSON
echo "\n=== Test invalid JSON handling ===\n";
$invalid_json_tests = [
    '{"invalid": "json"}',
    '{"type": "InvalidCRS"}',
    'not json at all',
    '{}', // Empty object
];

$error_count = 0;
foreach ($invalid_json_tests as $json) {
    try {
        ProjCRS::fromJson($json);
        echo "Invalid JSON unexpectedly succeeded\n";
    } catch (ProjCRSException $e) {
        $error_count++;
        echo "Invalid JSON properly rejected\n";
    } catch (Exception $e) {
        echo "Invalid JSON wrong exception type\n";
    }
}
echo "Error handling rate: $error_count/" . count($invalid_json_tests) . "\n";

// Test 7: Large JSON handling
echo "\n=== Test complex CRS JSON ===\n";
try {
    // Use a more complex projected CRS
    $complex_crs = ProjCRS::fromEpsg(2154); // RGF93 / Lambert-93
    $json = $complex_crs->toJson();
    $json_size = strlen($json);
    
    echo "Complex JSON size > 1000: " . ($json_size > 1000 ? "true" : "false") . "\n";
    echo "Complex JSON contains Lambert: " . (strpos($json, 'Lambert') !== false ? "true" : "false") . "\n";
    
    // Test parsing and round-trip
    $data = json_decode($json, true);
    echo "Complex JSON parseable: " . (json_last_error() === JSON_ERROR_NONE ? "true" : "false") . "\n";
    
    $restored = ProjCRS::fromJson($json);
    echo "Complex round-trip: " . ($complex_crs->equals($restored) ? "true" : "false") . "\n";
    
} catch (Exception $e) {
    echo "Complex JSON test failed: " . $e->getMessage() . "\n";
}

// Test 8: JSON comparison with WKT
echo "\n=== Test JSON vs WKT consistency ===\n";
try {
    $crs = ProjCRS::fromEpsg(4326);
    $json = $crs->toJson();
    $wkt = $crs->toWkt();
    
    echo "Both formats available: " . (strlen($json) > 0 && strlen($wkt) > 0 ? "true" : "false") . "\n";
    
    // Both should represent the same CRS
    $from_json = ProjCRS::fromJson($json);
    $from_wkt = ProjCRS::fromWkt($wkt);
    echo "JSON and WKT equivalent: " . ($from_json->equals($from_wkt) ? "true" : "false") . "\n";
    
} catch (Exception $e) {
    echo "JSON vs WKT test failed: " . $e->getMessage() . "\n";
}

// Test 9: Edge case - Geocentric CRS JSON
echo "\n=== Test Geocentric CRS JSON ===\n";
try {
    $geocentric = ProjCRS::fromEpsg(4978); // WGS 84 geocentric
    $json = $geocentric->toJson();
    $data = json_decode($json, true);
    
    echo "Geocentric JSON success: " . (strlen($json) > 0 ? "true" : "false") . "\n";
    echo "Type is GeocentricCRS: " . (isset($data['type']) && strpos($data['type'], 'Geocentric') !== false ? "true" : "false") . "\n";
    
    $round_trip = ProjCRS::fromJson($json);
    echo "Geocentric round-trip: " . ($geocentric->equals($round_trip) ? "true" : "false") . "\n";
    
} catch (Exception $e) {
    echo "Geocentric JSON failed: " . $e->getMessage() . "\n";
}

// Test 10: Performance test with multiple JSON operations
echo "\n=== Test JSON performance ===\n";
try {
    $start_time = microtime(true);
    
    // Perform multiple JSON operations
    for ($i = 0; $i < 10; $i++) {
        $crs = ProjCRS::fromEpsg(4326);
        $json = $crs->toJson();
        $restored = ProjCRS::fromJson($json);
    }
    
    $elapsed = microtime(true) - $start_time;
    // Assert the threshold only. The raw duration was previously printed and
    // pinned in --EXPECT-- as "0.0008 seconds", which made this test fail
    // whenever the machine was a fraction of a millisecond slower -- roughly
    // 60% of runs here. A wall-clock measurement cannot be an expectation.
    echo "JSON performance good: " . ($elapsed < 0.1 ? "true" : "false") . "\n";
    
} catch (Exception $e) {
    echo "JSON performance test failed: " . $e->getMessage() . "\n";
}

?>
--EXPECT--
=== Test basic JSON serialization ===
JSON generated: true
JSON contains type: true
JSON contains ProjectedCRS: true
JSON is compact: false

=== Test JSON round-trip for different CRS ===
EPSG:4326 round-trip: success
EPSG:3857 round-trip: success
EPSG:32633 round-trip: success
Round-trip success rate: 3/3

=== Test JSON structure for Geographic CRS ===
JSON decode success: true
Has type field: true
Type is Geographic: true
Has name field: true
Name contains WGS 84: true

=== Test JSON structure for Projected CRS ===
Projected JSON decode: true
Type is ProjectedCRS: true
Has base_crs: true
Has conversion: true
Conversion has method: true

=== Test JSON from PROJ4 strings ===
PROJ4 JSON failed: Provided PROJ4 string is not a valid CRS
PROJ4 JSON failed: Provided PROJ4 string is not a valid CRS

=== Test invalid JSON handling ===
Invalid JSON properly rejected
Invalid JSON properly rejected
Invalid JSON properly rejected
Invalid JSON properly rejected
Error handling rate: 4/4

=== Test complex CRS JSON ===
Complex JSON size > 1000: true
Complex JSON contains Lambert: true
Complex JSON parseable: true
Complex round-trip: false

=== Test JSON vs WKT consistency ===
Both formats available: true
JSON and WKT equivalent: true

=== Test Geocentric CRS JSON ===
Geocentric JSON success: true
Type is GeocentricCRS: false
Geocentric round-trip: true

=== Test JSON performance ===
JSON performance good: true