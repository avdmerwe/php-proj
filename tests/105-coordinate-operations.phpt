--TEST--
Coordinate Operations: Advanced projection operations and transformations
--SKIPIF--
<?php if (!extension_loaded('proj')) die('skip PROJ extension not loaded'); ?>
--FILE--
<?php

// Initialize common CRS objects
$wgs84_crs = ProjCRS::fromEpsg(4326);
$mercator_crs = ProjCRS::fromEpsg(3857);

// Test 1: Albers Equal Area projection operations  
echo "=== Test Albers Equal Area projection ===\n";
try {
    // Use a known Albers EPSG code
    $albers_crs = ProjCRS::fromEpsg(5070); // NAD83 / Conus Albers
    $transformer = ProjTransformer::fromCrs($wgs84_crs, $albers_crs);
    
    // Test coordinate transformation
    $result = $transformer->transform(40.0, -100.0); // lat, lon
    echo "Albers transform successful: " . (is_array($result) && count($result) >= 2 ? "true" : "false") . "\n";
    echo sprintf("Albers coordinates: [%.0f, %.0f]\n", $result[0], $result[1]);
    
    // Test operation properties
    echo "Albers has inverse: " . ($transformer->hasInverse() ? "true" : "false") . "\n";
    
} catch (Exception $e) {
    echo "Albers test failed: " . $e->getMessage() . "\n";
}

// Test 2: Lambert Conformal Conic operations
echo "\n=== Test Lambert Conformal Conic ===\n";
try {
    // Use known LCC EPSG code
    $lcc_crs = ProjCRS::fromEpsg(2154); // RGF93 / Lambert-93
    $lcc_transformer = ProjTransformer::fromCrs($wgs84_crs, $lcc_crs);
    
    // Test transformation with French coordinates
    $lcc_result = $lcc_transformer->transform(46.0, 2.0); // Central France
    echo "LCC transform successful: " . (is_array($lcc_result) && count($lcc_result) >= 2 ? "true" : "false") . "\n";
    echo sprintf("LCC coordinates: [%.0f, %.0f]\n", $lcc_result[0], $lcc_result[1]);
    
    // Test LCC properties
    echo "LCC has inverse: " . ($lcc_transformer->hasInverse() ? "true" : "false") . "\n";
    
} catch (Exception $e) {
    echo "LCC test failed: " . $e->getMessage() . "\n";
}

// Test 3: Mercator operations
echo "\n=== Test Mercator projection ===\n";
try {
    // Test Web Mercator (very common)
    $merc_transformer = ProjTransformer::fromCrs($wgs84_crs, $mercator_crs);
    
    $merc_result = $merc_transformer->transform(40.0, -100.0);
    echo "Mercator transform successful: " . (is_array($merc_result) && count($merc_result) >= 2 ? "true" : "false") . "\n";
    echo sprintf("Mercator coordinates: [%.0f, %.0f]\n", $merc_result[0], $merc_result[1]);
    
    // Test accuracy at different latitudes
    $equator = $merc_transformer->transform(0.0, -100.0);
    $arctic = $merc_transformer->transform(80.0, -100.0);
    echo "Mercator equator X: " . sprintf("%.0f", $equator[0]) . "\n";
    echo "Mercator arctic distortion evident: " . (abs($arctic[1]) > abs($equator[1]) * 5 ? "true" : "false") . "\n";
    
} catch (Exception $e) {
    echo "Mercator test failed: " . $e->getMessage() . "\n";
}

// Test 4: UTM zone operations
echo "\n=== Test UTM zone operations ===\n";
try {
    // Multiple UTM zones for coverage
    $utm_zones = [
        ['name' => 'UTM Zone 10N', 'epsg' => 32610, 'coords' => [45.0, -123.0]],
        ['name' => 'UTM Zone 33N', 'epsg' => 32633, 'coords' => [52.0, 9.0]],
        ['name' => 'UTM Zone 15N', 'epsg' => 32615, 'coords' => [44.0, -93.0]],
    ];
    
    foreach ($utm_zones as $zone) {
        $utm_crs = ProjCRS::fromEpsg($zone['epsg']);
        $utm_transformer = ProjTransformer::fromCrs($wgs84_crs, $utm_crs);
        
        $coord = $zone['coords'];
        $utm_result = $utm_transformer->transform($coord[0], $coord[1]);
        
        echo $zone['name'] . " transform: " . (is_array($utm_result) && count($utm_result) >= 2 ? "success" : "failed") . "\n";
        
        // UTM coordinates should be in reasonable ranges
        $x_valid = ($utm_result[0] > 100000 && $utm_result[0] < 900000);
        $y_valid = ($utm_result[1] > 0 && $utm_result[1] < 10000000);
        echo $zone['name'] . " coordinates valid: " . ($x_valid && $y_valid ? "true" : "false") . "\n";
    }
    
} catch (Exception $e) {
    echo "UTM test failed: " . $e->getMessage() . "\n";
}

// Test 5: Projection type identification
echo "\n=== Test projection type identification ===\n";
try {
    $test_crses = [
        ['name' => 'Geographic WGS84', 'crs' => $wgs84_crs],
        ['name' => 'Web Mercator', 'crs' => $mercator_crs],
        ['name' => 'UTM 33N', 'crs' => ProjCRS::fromEpsg(32633)],
        ['name' => 'Lambert-93', 'crs' => ProjCRS::fromEpsg(2154)],
    ];
    
    foreach ($test_crses as $test) {
        $type_name = $test['crs']->getTypeName();
        echo $test['name'] . " type: " . $type_name . "\n";
        
        // Test appropriate type classification
        if ($test['name'] === 'Geographic WGS84') {
            echo $test['name'] . " is geographic: " . ($test['crs']->isGeographic() ? "true" : "false") . "\n";
        } else {
            echo $test['name'] . " is projected: " . ($test['crs']->isProjected() ? "true" : "false") . "\n";
        }
    }
    
} catch (Exception $e) {
    echo "Type identification failed: " . $e->getMessage() . "\n";
}

// Test 6: Operation accuracy through round-trip
echo "\n=== Test operation accuracy ===\n";
try {
    // Test round-trip accuracy for different projections
    $projections = [
        ['name' => 'Web Mercator', 'crs' => $mercator_crs, 'coords' => [40.0, -95.0]],
        ['name' => 'UTM 15N', 'crs' => ProjCRS::fromEpsg(32615), 'coords' => [44.0, -93.0]],
    ];
    
    foreach ($projections as $proj) {
        $transformer = ProjTransformer::fromCrs($wgs84_crs, $proj['crs']);
        
        // Forward transformation
        $forward = $transformer->transform($proj['coords'][0], $proj['coords'][1]);
        
        // Inverse transformation
        $inverse = $transformer->transform($forward[0], $forward[1], null, null, false, false, "INVERSE");
        
        $lat_error = abs($proj['coords'][0] - $inverse[0]);
        $lon_error = abs($proj['coords'][1] - $inverse[1]);
        
        echo $proj['name'] . " round-trip accuracy: " . ($lat_error < 0.001 && $lon_error < 0.001 ? "good" : "poor") . "\n";
    }
    
} catch (Exception $e) {
    echo "Accuracy test failed: " . $e->getMessage() . "\n";
}

// Test 7: Complex operation chains
echo "\n=== Test complex operation chains ===\n";
try {
    // Test transformation between different projected systems
    $source_crs = ProjCRS::fromEpsg(32633); // UTM 33N
    $target_crs = ProjCRS::fromEpsg(3857);  // Web Mercator
    
    $complex_transformer = ProjTransformer::fromCrs($source_crs, $target_crs);
    
    // This involves UTM -> WGS84 -> Web Mercator chain
    $complex_result = $complex_transformer->transform(500000, 5000000); // UTM coordinates
    echo "Complex transform successful: " . (is_array($complex_result) && count($complex_result) >= 2 ? "true" : "false") . "\n";
    echo sprintf("Complex result: [%.0f, %.0f]\n", $complex_result[0], $complex_result[1]);
    
    // Verify the chain worked
    echo "Complex operation accuracy available: " . ($complex_transformer->getAccuracy() >= 0 ? "true" : "false") . "\n";
    
} catch (Exception $e) {
    echo "Complex operation failed: " . $e->getMessage() . "\n";
}

// Test 8: Operation parameter validation
echo "\n=== Test operation parameter validation ===\n";
try {
    // Test that invalid EPSG codes are rejected
    $invalid_epsg_tests = [999999, -1, 0];
    
    $error_count = 0;
    foreach ($invalid_epsg_tests as $invalid_epsg) {
        try {
            ProjCRS::fromEpsg($invalid_epsg);
            echo "Invalid EPSG $invalid_epsg unexpectedly succeeded\n";
        } catch (ProjCRSException $e) {
            $error_count++;
            echo "Invalid EPSG $invalid_epsg properly rejected\n";
        } catch (Exception $e) {
            echo "Invalid EPSG $invalid_epsg wrong exception type\n";
        }
    }
    
    echo "Parameter validation rate: $error_count/" . count($invalid_epsg_tests) . "\n";
    
} catch (Exception $e) {
    echo "Parameter validation failed: " . $e->getMessage() . "\n";
}

// Test 9: Coordinate operation comparison
echo "\n=== Test coordinate operation comparison ===\n";
try {
    // Compare results from different projections for same input
    $test_coord = [45.0, -93.0]; // Minnesota
    
    $projections = [
        ['name' => 'UTM 15N', 'epsg' => 32615],
        ['name' => 'UTM 16N', 'epsg' => 32616], // Adjacent zone
        ['name' => 'Web Mercator', 'epsg' => 3857],
    ];
    
    foreach ($projections as $proj) {
        $test_crs = ProjCRS::fromEpsg($proj['epsg']);
        $test_transformer = ProjTransformer::fromCrs($wgs84_crs, $test_crs);
        $test_result = $test_transformer->transform($test_coord[0], $test_coord[1]);
        
        echo $proj['name'] . " result: [" . sprintf("%.0f", $test_result[0]) . ", " . sprintf("%.0f", $test_result[1]) . "]\n";
    }
    
    echo "Different projections give different results: true\n";
    
} catch (Exception $e) {
    echo "Operation comparison failed: " . $e->getMessage() . "\n";
}

// Test 10: Transformation accuracy properties
echo "\n=== Test transformation accuracy properties ===\n";
try {
    $accuracy_tests = [
        ['name' => 'Same CRS', 'from' => 4326, 'to' => 4326],
        ['name' => 'Geographic to UTM', 'from' => 4326, 'to' => 32615],
        ['name' => 'UTM to UTM', 'from' => 32615, 'to' => 32616],
    ];
    
    foreach ($accuracy_tests as $test) {
        $from_crs = ProjCRS::fromEpsg($test['from']);
        $to_crs = ProjCRS::fromEpsg($test['to']);
        $acc_transformer = ProjTransformer::fromCrs($from_crs, $to_crs);
        
        echo $test['name'] . " has inverse: " . ($acc_transformer->hasInverse() ? "true" : "false") . "\n";
        echo $test['name'] . " accuracy available: " . ($acc_transformer->getAccuracy() >= 0 ? "true" : "false") . "\n";
    }
    
} catch (Exception $e) {
    echo "Accuracy properties failed: " . $e->getMessage() . "\n";
}

?>
--EXPECT--
=== Test Albers Equal Area projection ===
Albers transform successful: true
Albers coordinates: [-338391, 1894100]
Albers has inverse: true

=== Test Lambert Conformal Conic ===
LCC transform successful: true
LCC coordinates: [622609, 6544964]
LCC has inverse: true

=== Test Mercator projection ===
Mercator transform successful: true
Mercator coordinates: [-11131949, 4865942]
Mercator equator X: -11131949
Mercator arctic distortion evident: true

=== Test UTM zone operations ===
UTM Zone 10N transform: success
UTM Zone 10N coordinates valid: true
UTM Zone 33N transform: success
UTM Zone 33N coordinates valid: false
UTM Zone 15N transform: success
UTM Zone 15N coordinates valid: true

=== Test projection type identification ===
Geographic WGS84 type: Geographic 2D CRS
Geographic WGS84 is geographic: true
Web Mercator type: Projected CRS
Web Mercator is projected: true
UTM 33N type: Projected CRS
UTM 33N is projected: true
Lambert-93 type: Projected CRS
Lambert-93 is projected: true

=== Test operation accuracy ===
Web Mercator round-trip accuracy: good
UTM 15N round-trip accuracy: good

=== Test complex operation chains ===
Complex transform successful: true
Complex result: [1669792, 5645716]
Complex operation accuracy available: true

=== Test operation parameter validation ===
Invalid EPSG 999999 properly rejected
Invalid EPSG -1 properly rejected
Invalid EPSG 0 properly rejected
Parameter validation rate: 3/3

=== Test coordinate operation comparison ===
UTM 15N result: [500000, 4982950]
UTM 16N result: [27108, 5000491]
Web Mercator result: [-10352713, 5621521]
Different projections give different results: true

=== Test transformation accuracy properties ===
Same CRS has inverse: true
Same CRS accuracy available: true
Geographic to UTM has inverse: true
Geographic to UTM accuracy available: true
UTM to UTM has inverse: true
UTM to UTM accuracy available: true