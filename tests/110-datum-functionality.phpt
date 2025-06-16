--TEST--
Datum functionality: Datum transformations and coordinate system handling
--SKIPIF--
<?php if (!extension_loaded('proj')) die('skip PROJ extension not loaded'); ?>
--FILE--
<?php

// Test 1: Basic datum transformation - WGS84 to UTM
echo "=== Test WGS84 to UTM datum transformation ===\n";
try {
    // Test case from Trieste, Molo Sartorio (test_datum_shift.py)
    $wgs84_lat = 45.647188611;  // Degrees
    $wgs84_lon = 13.759554722;  // Degrees  
    $wgs84_z = 52.8;           // Ellipsoidal height in meters
    
    // Expected UTM coordinates
    $expected_utm_x = 403340.9672367854;
    $expected_utm_y = 5055597.175553089;
    
    // Create CRS objects
    $wgs84_crs = ProjCRS::fromEpsg(4326);  // WGS84 geographic
    $utm33_crs = ProjCRS::fromEpsg(32633); // UTM zone 33N (WGS84)
    
    echo "WGS84 CRS created: " . ($wgs84_crs instanceof ProjCRS ? "true" : "false") . "\n";
    echo "UTM33 CRS created: " . ($utm33_crs instanceof ProjCRS ? "true" : "false") . "\n";
    
    // Create transformer
    $transformer = ProjTransformer::fromCrs($wgs84_crs, $utm33_crs);
    echo "Transformer created: " . ($transformer instanceof ProjTransformer ? "true" : "false") . "\n";
    
    // Transform coordinates (note: EPSG:4326 uses lat/lon order)
    $result = $transformer->transform($wgs84_lat, $wgs84_lon, $wgs84_z);
    echo "Transformation successful: " . (is_array($result) && count($result) >= 2 ? "true" : "false") . "\n";
    
    if (is_array($result) && count($result) >= 2) {
        $diff_x = abs($result[0] - $expected_utm_x);
        $diff_y = abs($result[1] - $expected_utm_y);
        echo "X coordinate accuracy: " . ($diff_x < 1.0 ? "good" : "poor") . " (diff: " . number_format($diff_x, 2) . ")\n";
        echo "Y coordinate accuracy: " . ($diff_y < 1.0 ? "good" : "poor") . " (diff: " . number_format($diff_y, 2) . ")\n";
    }
    
} catch (Exception $e) {
    echo "WGS84 to UTM test failed: " . $e->getMessage() . "\n";
}

// Test 2: Inverse transformation - UTM to WGS84
echo "\n=== Test UTM to WGS84 inverse transformation ===\n";
try {
    $utm_x = 403340.97;
    $utm_y = 5055597.17;
    $utm_z = 52.8;
    
    // Create inverse transformer
    $inverse_transformer = ProjTransformer::fromCrs($utm33_crs, $wgs84_crs);
    
    // Transform back
    $back_result = $inverse_transformer->transform($utm_x, $utm_y, $utm_z);
    echo "Inverse transformation successful: " . (is_array($back_result) && count($back_result) >= 2 ? "true" : "false") . "\n";
    
    if (is_array($back_result) && count($back_result) >= 2) {
        $diff_lat = abs($back_result[0] - $wgs84_lat);
        $diff_lon = abs($back_result[1] - $wgs84_lon);
        echo "Latitude accuracy: " . ($diff_lat < 0.001 ? "good" : "poor") . " (diff: " . number_format($diff_lat, 6) . ")\n";
        echo "Longitude accuracy: " . ($diff_lon < 0.001 ? "good" : "poor") . " (diff: " . number_format($diff_lon, 6) . ")\n";
    }
    
} catch (Exception $e) {
    echo "UTM to WGS84 test failed: " . $e->getMessage() . "\n";
}

// Test 3: Different datum transformation - NAD27 to WGS84
echo "\n=== Test NAD27 to WGS84 datum transformation ===\n";
try {
    // Test coordinates from test_datum.py
    $nad27_lon = -111.5;
    $nad27_lat = 45.25919444444;
    
    // Create NAD27 UTM Zone 10 CRS
    $nad27_utm_crs = ProjCRS::fromEpsg(26710); // NAD27 UTM Zone 10N
    $wgs84_utm_crs = ProjCRS::fromEpsg(32610);  // WGS84 UTM Zone 10N
    
    echo "NAD27 UTM CRS created: " . ($nad27_utm_crs instanceof ProjCRS ? "true" : "false") . "\n";
    echo "WGS84 UTM CRS created: " . ($wgs84_utm_crs instanceof ProjCRS ? "true" : "false") . "\n";
    
    // First transform NAD27 geographic to NAD27 UTM
    $nad27_geo_crs = ProjCRS::fromEpsg(4267); // NAD27 geographic
    $geo_to_utm_transformer = ProjTransformer::fromCrs($nad27_geo_crs, $nad27_utm_crs);
    $nad27_utm_result = $geo_to_utm_transformer->transform($nad27_lat, $nad27_lon);
    
    echo "NAD27 geo to UTM successful: " . (is_array($nad27_utm_result) && count($nad27_utm_result) >= 2 ? "true" : "false") . "\n";
    
    // Then transform NAD27 UTM to WGS84 UTM (this involves datum shift)
    if (is_array($nad27_utm_result) && count($nad27_utm_result) >= 2) {
        $datum_transformer = ProjTransformer::fromCrs($nad27_utm_crs, $wgs84_utm_crs);
        $wgs84_utm_result = $datum_transformer->transform($nad27_utm_result[0], $nad27_utm_result[1]);
        
        echo "NAD27 to WGS84 datum shift successful: " . (is_array($wgs84_utm_result) && count($wgs84_utm_result) >= 2 ? "true" : "false") . "\n";
        
        if (is_array($wgs84_utm_result) && count($wgs84_utm_result) >= 2) {
            // Coordinates should be different due to datum shift
            $x_diff = abs($wgs84_utm_result[0] - $nad27_utm_result[0]);
            $y_diff = abs($wgs84_utm_result[1] - $nad27_utm_result[1]);
            echo "Datum shift detected: " . (($x_diff > 1.0 || $y_diff > 1.0) ? "true" : "false") . " (X diff: " . number_format($x_diff, 2) . ", Y diff: " . number_format($y_diff, 2) . ")\n";
        }
    }
    
} catch (Exception $e) {
    echo "NAD27 to WGS84 test failed: " . $e->getMessage() . "\n";
}

// Test 4: CRS datum information extraction
echo "\n=== Test CRS datum information ===\n";
try {
    // Test various CRS with different datums
    $test_cases = [
        ['EPSG:4326', 'WGS84'],
        ['EPSG:4267', 'NAD27'],
        ['EPSG:4269', 'NAD83'],
        ['EPSG:4230', 'European Datum 1950']
    ];
    
    foreach ($test_cases as [$epsg_code, $expected_datum]) {
        $crs = ProjCRS::fromString($epsg_code);
        $crs_name = $crs->getName();
        
        echo "CRS $epsg_code: " . $crs_name . "\n";
        echo "  Type: " . $crs->getTypeName() . "\n";
        echo "  Is Geographic: " . ($crs->isGeographic() ? "true" : "false") . "\n";
        
        // Check if datum name appears in CRS name or WKT
        $wkt = $crs->toWkt();
        $contains_datum = (stripos($wkt, $expected_datum) !== false || stripos($crs_name, $expected_datum) !== false);
        echo "  Contains expected datum info: " . ($contains_datum ? "true" : "false") . "\n";
    }
    
} catch (Exception $e) {
    echo "CRS datum info test failed: " . $e->getMessage() . "\n";
}

// Test 5: Ellipsoid information from geodetic calculations
echo "\n=== Test ellipsoid parameters ===\n";
try {
    // Create geodetic calculator for different ellipsoids
    $wgs84_geod = ProjGeod::fromEpsg(4326);
    echo "WGS84 geodetic calculator created: " . ($wgs84_geod instanceof ProjGeod ? "true" : "false") . "\n";
    
    // Get ellipsoid parameters
    $a = $wgs84_geod->getA(); // Semi-major axis
    $f = $wgs84_geod->getF(); // Flattening
    $b = $wgs84_geod->getB(); // Semi-minor axis
    
    echo "Semi-major axis (a): " . number_format($a, 1) . " meters\n";
    echo "Flattening (f): " . number_format($f, 10) . "\n";
    echo "Semi-minor axis (b): " . number_format($b, 1) . " meters\n";
    
    // Verify WGS84 parameters are reasonable
    $expected_a = 6378137.0;  // WGS84 semi-major axis
    $expected_f = 1/298.257223563;  // WGS84 flattening
    
    $a_correct = abs($a - $expected_a) < 1.0;
    $f_correct = abs($f - $expected_f) < 1e-9;
    
    echo "WGS84 parameters correct: " . (($a_correct && $f_correct) ? "true" : "false") . "\n";
    
} catch (Exception $e) {
    echo "Ellipsoid parameters test failed: " . $e->getMessage() . "\n";
}

?>
--EXPECT--
=== Test WGS84 to UTM datum transformation ===
WGS84 CRS created: true
UTM33 CRS created: true
Transformer created: true
Transformation successful: true
X coordinate accuracy: good (diff: 0.00)
Y coordinate accuracy: good (diff: 0.00)

=== Test UTM to WGS84 inverse transformation ===
Inverse transformation successful: true
Latitude accuracy: good (diff: 0.000000)
Longitude accuracy: good (diff: 0.000000)

=== Test NAD27 to WGS84 datum transformation ===
NAD27 UTM CRS created: true
WGS84 UTM CRS created: true
NAD27 geo to UTM successful: true
NAD27 to WGS84 datum shift successful: true
Datum shift detected: true (X diff: 88.30, Y diff: 198.46)

=== Test CRS datum information ===
CRS EPSG:4326: WGS 84
  Type: Geographic 2D CRS
  Is Geographic: true
  Contains expected datum info: false
CRS EPSG:4267: NAD27
  Type: Geographic 2D CRS
  Is Geographic: true
  Contains expected datum info: true
CRS EPSG:4269: NAD83
  Type: Geographic 2D CRS
  Is Geographic: true
  Contains expected datum info: true
CRS EPSG:4230: ED50
  Type: Geographic 2D CRS
  Is Geographic: true
  Contains expected datum info: true

=== Test ellipsoid parameters ===
WGS84 geodetic calculator created: true
Semi-major axis (a): 6,378,137.0 meters
Flattening (f): 0.0033528107
Semi-minor axis (b): 6,356,752.3 meters
WGS84 parameters correct: true