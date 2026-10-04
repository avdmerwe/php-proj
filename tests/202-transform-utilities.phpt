--TEST--
Transform: Advanced transformation utilities and coordinate processing
--SKIPIF--
<?php if (!extension_loaded('proj')) die('skip PROJ extension not loaded'); ?>
--FILE--
<?php

// Test 1: Simple coordinate transformation test
echo "=== Test basic coordinate transformation ===\n";
try {
    // Simple WGS84 to Web Mercator transformation
    $transformer = ProjTransformer::fromCrs('EPSG:4326', 'EPSG:3857');
    $result = $transformer->transform(1.0, -145.5); // lat, lon
    echo "Transform successful: " . (is_array($result) && count($result) >= 2 ? "true" : "false") . "\n";
    echo sprintf("Transform result: [%.3f, %.3f]\n", $result[0], $result[1]);
} catch (Exception $e) {
    echo "Transformation failed: " . $e->getMessage() . "\n";
}

// Test 2: UTM coordinate transformations (NAD83 to NAD27)
echo "\n=== Test UTM NAD83 to NAD27 ===\n";
try {
    $nad83_utm = ProjCRS::fromEpsg(26915); // UTM Zone 15N NAD83
    $nad27_utm = ProjCRS::fromEpsg(26715); // UTM Zone 15N NAD27
    
    $transformer = ProjTransformer::fromCrs($nad83_utm, $nad27_utm);
    
    // Jefferson City, MO coordinates (transform from UTM to lat/lon for verification)
    $utm_to_latlon = ProjTransformer::fromCrs("EPSG:26915", "EPSG:4326");
    $latlon = $utm_to_latlon->transform(569704.566, 4269024.671);
    // Round to 5 decimal places to tolerate small precision differences between environments
    echo sprintf("Jefferson City lat/lon: [%.5f, %.5f]\n", round($latlon[0], 5), round($latlon[1], 5));
    
    // Transform to NAD27
    $result = $transformer->transform(569704.566, 4269024.671);
    echo "NAD83 to NAD27 successful: " . (is_array($result) && count($result) >= 2 ? "true" : "false") . "\n";
    // PROJ late-binds the NAD83 -> NAD27 operation per point, so the result
    // depends on whether the optional NADCON grids are present in the PROJ user
    // directory ($HOME/.local/share/proj). Measured both ways on PROJ 9.4.0:
    // with the grids  569722.342, 4268814.028  (NADCON, 0.15 m class)
    // without them    569720.460, 4268813.880  (ballpark geographic offset)
    // dpkg-buildpackage builds with a sanitised HOME, so the grids are not
    // visible there. Assert agreement with the grid-based reference to within
    // 5 m, which covers both paths and still catches a real break.
    $ref = [569722.342, 4268814.028];
    echo "NAD27 coordinates within 5 m of reference: "
        . ((abs($result[0] - $ref[0]) < 5.0 && abs($result[1] - $ref[1]) < 5.0) ? "true" : "false") . "\n";
    
} catch (Exception $e) {
    echo "UTM transformation failed: " . $e->getMessage() . "\n";
}

// Test 3: Batch coordinate transformations with multiple points
echo "\n=== Test batch coordinate processing ===\n";
try {
    $transformer = ProjTransformer::fromCrs('EPSG:26915', 'EPSG:26715');
    
    // Multiple Missouri cities (Columbia, KC, St. Louis)
    $coordinates = [
        [567703.344, 4298200.739], // Columbia
        [351730.944, 4353698.725], // Kansas City
        [728553.093, 4292319.005]  // St. Louis
    ];
    
    $results = $transformer->transformArray($coordinates);
    echo "Batch transform count: " . count($results) . "\n";
    echo "All results valid: " . (count($results) === 3 && 
                                   is_array($results[0]) && count($results[0]) >= 2 && 
                                   is_array($results[1]) && count($results[1]) >= 2 && 
                                   is_array($results[2]) && count($results[2]) >= 2 ? "true" : "false") . "\n";
    
    // Verify first result
    // Same grid dependence as the NAD83 -> NAD27 case above. Measured both ways:
    // with the grids  567721.149, 4297989.112
    // without them    567719.249, 4297989.776
    $columbia_ref = [567721.149, 4297989.112];
    echo "Columbia NAD27 within 5 m of reference: "
        . ((abs($results[0][0] - $columbia_ref[0]) < 5.0 && abs($results[0][1] - $columbia_ref[1]) < 5.0) ? "true" : "false") . "\n";
    
} catch (Exception $e) {
    echo "Batch transformation failed: " . $e->getMessage() . "\n";
}

// Test 4: Round-trip transformation accuracy
echo "\n=== Test round-trip accuracy ===\n";
try {
    $transformer = ProjTransformer::fromCrs('EPSG:4326', 'EPSG:3857');
    
    $original_lat = 38.5667;
    $original_lon = -92.2;
    
    // Forward transformation
    $projected = $transformer->transform($original_lat, $original_lon);
    
    // Inverse transformation
    $back = $transformer->transform($projected[0], $projected[1], null, null, false, false, "INVERSE");
    
    $lat_error = abs($original_lat - $back[0]);
    $lon_error = abs($original_lon - $back[1]);
    
    echo "Round-trip lat accuracy: " . ($lat_error < 0.0001 ? "good" : "poor") . "\n";
    echo "Round-trip lon accuracy: " . ($lon_error < 0.0001 ? "good" : "poor") . "\n";
    echo sprintf("Lat error: %.8f\n", $lat_error);
    echo sprintf("Lon error: %.8f\n", $lon_error);
    
} catch (Exception $e) {
    echo "Round-trip test failed: " . $e->getMessage() . "\n";
}

// Test 5: Large array performance test
echo "\n=== Test large array performance ===\n";
try {
    $transformer = ProjTransformer::fromCrs('EPSG:4326', 'EPSG:3857');
    
    // Generate test grid (20x20 = 400 points)
    $coordinates = [];
    for ($lat = 35; $lat <= 45; $lat += 0.5) {
        for ($lon = -100; $lon <= -90; $lon += 0.5) {
            $coordinates[] = [$lat, $lon];
        }
    }
    
    $start_time = microtime(true);
    $results = $transformer->transformArray($coordinates);
    $elapsed = microtime(true) - $start_time;
    
    echo "Large array count: " . count($coordinates) . "\n";
    echo "Transform results: " . count($results) . "\n";
    // Assert the threshold only -- a wall-clock measurement cannot be a stable
    // expectation. The raw duration was pinned as "0.0001 seconds", which held
    // only because this loop is fast; it is the same latent failure that made
    // 104-crs-json flaky.
    echo "Performance good: " . ($elapsed < 0.1 ? "true" : "false") . "\n";
    
} catch (Exception $e) {
    echo "Large array test failed: " . $e->getMessage() . "\n";
}

// Test 6: Coordinate bounds transformation
echo "\n=== Test bounds transformation ===\n";
try {
    $transformer = ProjTransformer::fromCrs('EPSG:4326', 'EPSG:3857');
    
    // Missouri state bounds (approximate)
    $bounds = $transformer->transformBounds(36.0, -95.8, 40.6, -89.1);
    
    echo "Bounds transformation successful: " . (is_array($bounds) && count($bounds) === 4 ? "true" : "false") . "\n";
    echo sprintf("Transformed bounds: [%.0f, %.0f, %.0f, %.0f]\n", 
                 $bounds[0], $bounds[1], $bounds[2], $bounds[3]);
    
} catch (Exception $e) {
    echo "Bounds transformation failed: " . $e->getMessage() . "\n";
}

// Test 7: 3D coordinate transformations
echo "\n=== Test 3D coordinate transformations ===\n";
try {
    $transformer = ProjTransformer::fromCrs('EPSG:4326', 'EPSG:3857');
    
    // Test with elevation
    $result_3d = $transformer->transform(39.0, -94.5, 300.0);
    echo "3D transform successful: " . (is_array($result_3d) && count($result_3d) === 3 ? "true" : "false") . "\n";
    echo sprintf("3D elevation preserved: %.1f\n", $result_3d[2]);
    
    // Test 3D array
    $coords_3d = [
        [39.0, -94.5, 300.0],
        [38.5, -92.2, 250.0]
    ];
    
    $results_3d = $transformer->transformArray($coords_3d);
    echo "3D array successful: " . (count($results_3d) === 2 && count($results_3d[0]) === 3 ? "true" : "false") . "\n";
    
} catch (Exception $e) {
    echo "3D transformation failed: " . $e->getMessage() . "\n";
}

// Test 8: Pipeline transformations
echo "\n=== Test pipeline transformations ===\n";
try {
    // Simple unit conversion pipeline
    $pipeline = "+proj=pipeline +step +proj=unitconvert +xy_in=deg +xy_out=rad +step +proj=unitconvert +xy_in=rad +xy_out=deg";
    $pipe_transformer = ProjTransformer::fromPipeline($pipeline);
    
    $test_coords = [39.0, -94.5];
    $pipe_result = $pipe_transformer->transform($test_coords[0], $test_coords[1]);
    
    $accuracy = abs($test_coords[0] - $pipe_result[0]) + abs($test_coords[1] - $pipe_result[1]);
    echo "Pipeline successful: " . ($accuracy < 0.01 ? "true" : "false") . "\n";
    echo sprintf("Pipeline accuracy: %.6f\n", $accuracy);
    
} catch (Exception $e) {
    echo "Pipeline transformation failed: " . $e->getMessage() . "\n";
}

// Test 9: Error handling for invalid coordinates
echo "\n=== Test invalid coordinate handling ===\n";
try {
    $transformer = ProjTransformer::fromCrs('EPSG:4326', 'EPSG:3857');
    
    // Test with invalid latitude (>90)
    $result = $transformer->transform(95.0, -94.5);
    echo "Invalid latitude handling: " . (is_array($result) ? "unexpected success" : "proper error") . "\n";
    
} catch (Exception $e) {
    echo "Invalid coordinate error: " . $e->getMessage() . "\n";
}

// Test 10: Transformer information and properties
echo "\n=== Test transformer properties ===\n";
try {
    $transformer = ProjTransformer::fromCrs('EPSG:4326', 'EPSG:3857');
    
    echo "Has description: " . (strlen($transformer->getDescription()) > 0 ? "true" : "false") . "\n";
    echo "Has inverse: " . ($transformer->hasInverse() ? "true" : "false") . "\n";
    echo "Has accuracy: " . ($transformer->getAccuracy() >= 0 ? "true" : "false") . "\n";
    
    $wkt = $transformer->toWkt();
    $proj4 = $transformer->toProj4();
    echo "WKT export: " . (strlen($wkt) > 0 ? "available" : "unavailable") . "\n";
    echo "PROJ4 export: " . (strlen($proj4) > 0 ? "available" : "unavailable") . "\n";
    
} catch (Exception $e) {
    echo "Transformer properties failed: " . $e->getMessage() . "\n";
}

?>
--EXPECT--
=== Test basic coordinate transformation ===
Transform successful: true
Transform result: [-16196985.910, 111325.143]

=== Test UTM NAD83 to NAD27 ===
Jefferson City lat/lon: [38.56694, -92.19988]
NAD83 to NAD27 successful: true
NAD27 coordinates within 5 m of reference: true

=== Test batch coordinate processing ===
Batch transform count: 3
All results valid: true
Columbia NAD27 within 5 m of reference: true

=== Test round-trip accuracy ===
Round-trip lat accuracy: good
Round-trip lon accuracy: good
Lat error: 0.00000000
Lon error: 0.00000000

=== Test large array performance ===
Large array count: 441
Transform results: 441
Performance good: true

=== Test bounds transformation ===
Bounds transformation successful: true
Transformed bounds: [-10664407, 4300621, -9918567, 4953520]

=== Test 3D coordinate transformations ===
3D transform successful: true
3D elevation preserved: 300.0
3D array successful: true

=== Test pipeline transformations ===
Pipeline successful: true
Pipeline accuracy: 0.000000

=== Test invalid coordinate handling ===
Invalid latitude handling: unexpected success

=== Test transformer properties ===
Has description: true
Has inverse: true
Has accuracy: true
WKT export: available
PROJ4 export: available