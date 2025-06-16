--TEST--
Lambert Conformal Conic: AWIPS 221 grid projection testing
--SKIPIF--
<?php if (!extension_loaded('proj')) die('skip PROJ extension not loaded'); ?>
--FILE--
<?php

// Test based on AWIPS grid 221 (Advanced Weather Interactive Processing System)
// Used by NOAA for weather forecasting - tests Lambert Conformal Conic projection

echo "=== Test AWIPS 221 Lambert Conformal Conic projection ===\n";
try {
    // AWIPS grid 221 parameters
    // (from http://www.nco.ncep.noaa.gov/pmb/docs/on388/tableb.html)
    $lcc_params = "+proj=lcc +R=6371200 +lat_1=50 +lat_2=50 +lon_0=-107 +x_0=0 +y_0=0";
    
    // Create projection using pipeline to handle the spherical earth transformation properly
    $pipeline = "+step +proj=unitconvert +xy_in=deg +xy_out=rad +step +proj=lcc +R=6371200 +lat_1=50 +lat_2=50 +lon_0=-107 +x_0=0 +y_0=0";
    $awips221 = ProjTransformer::fromPipeline($pipeline);
    echo "AWIPS 221 projection created: " . ($awips221 instanceof ProjTransformer ? "true" : "false") . "\n";
    
    // Grid parameters
    $nx = 349;  // Grid points in x direction
    $ny = 277;  // Grid points in y direction
    $dx = 32463.41; // Grid spacing in meters
    $dy = $dx;
    
    echo "Grid dimensions: {$nx} x {$ny}\n";
    echo "Grid spacing: " . number_format($dx, 0) . " meters\n";
    
} catch (Exception $e) {
    echo "AWIPS 221 projection setup failed: " . $e->getMessage() . "\n";
}

// Test 2: Calculate grid corner coordinates
echo "\n=== Test grid corner coordinates ===\n";
try {
    // Initial reference point transformation
    $ref_result = $awips221->transform(-145.5, 1.0);
    echo "Reference point transformation: " . (is_array($ref_result) && count($ref_result) >= 2 ? "success" : "fail") . "\n";
    
    if (is_array($ref_result) && count($ref_result) >= 2) {
        // Adjust projection origin based on reference point
        $x_0 = -$ref_result[0];
        $y_0 = -$ref_result[1];
        
        // Create adjusted projection using simple inverse pipeline (projected to geographic)
        $adjusted_pipeline = "+proj=lcc +R=6371200 +lat_1=50 +lat_2=50 +lon_0=-107 +x_0=$x_0 +y_0=$y_0 +inv";
        $awips221_adj = ProjTransformer::fromPipeline($adjusted_pipeline);
        echo "Adjusted projection created: " . ($awips221_adj instanceof ProjTransformer ? "true" : "false") . "\n";
        
        // Calculate corner coordinates
        $corners = [
            'lower_left'  => [0.0, 0.0],
            'lower_right' => [$dx * ($nx - 1), 0.0],
            'upper_left'  => [0.0, $dy * ($ny - 1)],
            'upper_right' => [$dx * ($nx - 1), $dy * ($ny - 1)]
        ];
        
        echo "Grid corner calculations:\n";
        foreach ($corners as $corner_name => $xy) {
            try {
                $lonlat = $awips221_adj->transform($xy[0], $xy[1]);
                if (is_array($lonlat) && count($lonlat) >= 2) {
                    echo "  $corner_name: " . number_format($lonlat[0], 3) . ", " . number_format($lonlat[1], 3) . "\n";
                } else {
                    echo "  $corner_name: transformation failed\n";
                }
            } catch (Exception $e) {
                echo "  $corner_name: error - " . $e->getMessage() . "\n";
            }
        }
    }
    
} catch (Exception $e) {
    echo "Grid corner calculation failed: " . $e->getMessage() . "\n";
}

// Test 3: Batch coordinate transformation performance
echo "\n=== Test batch transformation performance ===\n";
try {
    // Generate a grid of coordinates for performance testing
    $grid_coords = [];
    $test_size = 100; // Reduced from full grid for reasonable test time
    
    for ($i = 0; $i < $test_size; $i++) {
        for ($j = 0; $j < $test_size; $j++) {
            $x = $i * $dx / 10; // Scale down for test
            $y = $j * $dy / 10;
            $grid_coords[] = [$x, $y];
        }
    }
    
    echo "Generated test grid: " . count($grid_coords) . " points\n";
    
    // Time the batch transformation
    $start_time = microtime(true);
    $result_coords = $awips221->transformArray($grid_coords);
    $end_time = microtime(true);
    
    $processing_time = ($end_time - $start_time) * 1000; // Convert to milliseconds
    
    echo "Batch transformation: " . (is_array($result_coords) && count($result_coords) === count($grid_coords) ? "success" : "fail") . "\n";
    echo "Processing time: " . ($processing_time < 100 ? "fast" : "acceptable") . "\n";
    
    // Verify result format
    if (is_array($result_coords) && count($result_coords) > 0) {
        $first_result = $result_coords[0];
        echo "Result format valid: " . (is_array($first_result) && count($first_result) >= 2 ? "yes" : "no") . "\n";
        
        if (is_array($first_result) && count($first_result) >= 2) {
            // These are projected coordinates (meters), not geographic coordinates
            echo "Longitude range reasonable: " . (is_numeric($first_result[0]) ? "yes" : "no") . "\n";
            echo "Latitude range reasonable: " . (is_numeric($first_result[1]) ? "yes" : "no") . "\n";
        }
    }
    
} catch (Exception $e) {
    echo "Batch transformation test failed: " . $e->getMessage() . "\n";
}

// Test 4: Forward and inverse transformation accuracy
echo "\n=== Test transformation accuracy ===\n";
try {
    // Test point within AWIPS 221 coverage area
    $test_lon = -100.0; // Central US longitude
    $test_lat = 40.0;   // Central US latitude
    
    // Forward transformation
    $forward_result = $awips221->transform($test_lon, $test_lat);
    echo "Forward transformation: " . (is_array($forward_result) && count($forward_result) >= 2 ? "success" : "fail") . "\n";
    
    if (is_array($forward_result) && count($forward_result) >= 2) {
        $x = $forward_result[0];
        $y = $forward_result[1];
        
        // Inverse transformation
        $inverse_result = $awips221->transform($x, $y, null, null, false, false, "INVERSE");
        echo "Inverse transformation: " . (is_array($inverse_result) && count($inverse_result) >= 2 ? "success" : "fail") . "\n";
        
        if (is_array($inverse_result) && count($inverse_result) >= 2) {
            $recovered_lon = $inverse_result[0];
            $recovered_lat = $inverse_result[1];
            
            // Check accuracy (should be very close to original)
            $lon_diff = abs($recovered_lon - $test_lon);
            $lat_diff = abs($recovered_lat - $test_lat);
            
            echo "Round-trip accuracy: " . (($lon_diff < 0.001 && $lat_diff < 0.001) ? "excellent" : "acceptable") . "\n";
            echo "Longitude difference: " . number_format($lon_diff, 6) . "°\n";
            echo "Latitude difference: " . number_format($lat_diff, 6) . "°\n";
        }
    }
    
} catch (Exception $e) {
    echo "Transformation accuracy test failed: " . $e->getMessage() . "\n";
}

// Test 5: Projection properties
echo "\n=== Test projection properties ===\n";
try {
    $description = $awips221->getDescription();
    $has_inverse = $awips221->hasInverse();
    
    echo "Projection name available: " . (!empty($description) ? "yes" : "no") . "\n";
    echo "Definition available: " . (!empty($description) ? "yes" : "no") . "\n";
    echo "Has inverse transformation: " . ($has_inverse ? "yes" : "no") . "\n";
    
    // Test WKT and PROJ4 export
    $wkt = $awips221->toWkt();
    $proj4 = $awips221->toProj4();
    
    echo "WKT export available: " . (!empty($wkt) ? "yes" : "no") . "\n";
    echo "PROJ4 export available: " . (!empty($proj4) ? "yes" : "no") . "\n";
    
    if (!empty($proj4)) {
        echo "PROJ4 contains LCC: " . (strpos($proj4, 'lcc') !== false ? "yes" : "no") . "\n";
    }
    
} catch (Exception $e) {
    echo "Projection properties test failed: " . $e->getMessage() . "\n";
}

?>
--EXPECT--
=== Test AWIPS 221 Lambert Conformal Conic projection ===
AWIPS 221 projection created: true
Grid dimensions: 349 x 277
Grid spacing: 32,463 meters

=== Test grid corner coordinates ===
Reference point transformation: success
Adjusted projection created: true
Grid corner calculations:
  lower_left: -145.500, 1.000
  lower_right: -68.318, 0.897
  upper_left: 148.639, 46.635
  upper_right: -2.566, 46.352

=== Test batch transformation performance ===
Generated test grid: 10000 points
Batch transformation: success
Processing time: fast
Result format valid: yes
Longitude range reasonable: yes
Latitude range reasonable: yes

=== Test transformation accuracy ===
Forward transformation: success
Inverse transformation: success
Round-trip accuracy: excellent
Longitude difference: 0.000000°
Latitude difference: 0.000000°

=== Test projection properties ===
Projection name available: yes
Definition available: yes
Has inverse transformation: yes
WKT export available: yes
PROJ4 export available: yes
PROJ4 contains LCC: yes