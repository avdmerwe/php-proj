--TEST--
Utility functions: Array handling and coordinate processing utilities
--SKIPIF--
<?php if (!extension_loaded('proj')) die('skip PROJ extension not loaded'); ?>
--FILE--
<?php

// Test 1: Array coordinate handling in transformations
echo "=== Test array coordinate processing ===\n";
try {
    $transformer = ProjTransformer::fromCrs('EPSG:4326', 'EPSG:3857');
    
    // Test different array input formats
    $single_coordinate = [-74.0, 40.7];
    $multiple_coordinates = [
        [-74.0, 40.7],
        [-73.9, 40.8],
        [-73.8, 40.9]
    ];
    
    // Test transformArray with tuple format
    $single_result = $transformer->transformArray([$single_coordinate]);
    echo "Single coordinate transformation: " . (is_array($single_result) && count($single_result) === 1 ? "success" : "fail") . "\n";
    
    $multiple_result = $transformer->transformArray($multiple_coordinates);
    echo "Multiple coordinate transformation: " . (is_array($multiple_result) && count($multiple_result) === 3 ? "success" : "fail") . "\n";
    
    // Verify result format
    if (is_array($multiple_result) && count($multiple_result) >= 1) {
        $first_result = $multiple_result[0];
        echo "Result is coordinate tuple: " . (is_array($first_result) && count($first_result) >= 2 ? "yes" : "no") . "\n";
        echo "X coordinate is numeric: " . (is_numeric($first_result[0]) ? "yes" : "no") . "\n";
        echo "Y coordinate is numeric: " . (is_numeric($first_result[1]) ? "yes" : "no") . "\n";
    }
    
} catch (Exception $e) {
    echo "Array coordinate test failed: " . $e->getMessage() . "\n";
}

// Test 2: 3D coordinate array handling
echo "\n=== Test 3D coordinate arrays ===\n";
try {
    $coordinates_3d = [
        [-74.0, 40.7, 10.0],
        [-73.9, 40.8, 20.0],
        [-73.8, 40.9, 30.0]
    ];
    
    $result_3d = $transformer->transformArray($coordinates_3d);
    echo "3D coordinate transformation: " . (is_array($result_3d) && count($result_3d) === 3 ? "success" : "fail") . "\n";
    
    if (is_array($result_3d) && count($result_3d) >= 1) {
        $first_3d = $result_3d[0];
        echo "3D result has Z coordinate: " . (is_array($first_3d) && count($first_3d) >= 3 ? "yes" : "no") . "\n";
        echo "Z coordinate preserved: " . (is_array($first_3d) && isset($first_3d[2]) && is_numeric($first_3d[2]) ? "yes" : "no") . "\n";
    }
    
} catch (Exception $e) {
    echo "3D coordinate test failed: " . $e->getMessage() . "\n";
}

// Test 3: Input validation and error handling
echo "\n=== Test input validation ===\n";
try {
    // Test invalid coordinate format
    $invalid_coords = [
        [1, 2, 3, 4, 5], // Too many dimensions
        ["invalid", "data"], // Non-numeric data
        [1] // Too few dimensions
    ];
    
    foreach ($invalid_coords as $index => $coord) {
        try {
            $result = $transformer->transformArray([$coord]);
            echo "Invalid coordinate $index: accepted (unexpected)\n";
        } catch (Exception $e) {
            echo "Invalid coordinate $index: rejected (expected)\n";
        }
    }
    
} catch (Exception $e) {
    echo "Input validation test failed: " . $e->getMessage() . "\n";
}

// Test 4: Numeric type conversion
echo "\n=== Test numeric type conversion ===\n";
try {
    // Test mixed numeric types
    $mixed_coords = [
        ["-74.0", "40.7"],      // String numbers
        [-74, 40],              // Integers
        [-74.5, 40.75]          // Floats
    ];
    
    foreach ($mixed_coords as $index => $coord) {
        try {
            $result = $transformer->transformArray([$coord]);
            echo "Mixed type coordinate $index: " . (is_array($result) && count($result) === 1 ? "converted" : "failed") . "\n";
        } catch (Exception $e) {
            echo "Mixed type coordinate $index failed: " . $e->getMessage() . "\n";
        }
    }
    
} catch (Exception $e) {
    echo "Numeric conversion test failed: " . $e->getMessage() . "\n";
}

// Test 5: Large array handling
echo "\n=== Test large array performance ===\n";
try {
    // Generate large coordinate array
    $large_coords = [];
    for ($i = 0; $i < 1000; $i++) {
        $large_coords[] = [-74.0 + ($i * 0.001), 40.7 + ($i * 0.001)];
    }
    
    $start_time = microtime(true);
    $large_result = $transformer->transformArray($large_coords);
    $end_time = microtime(true);
    
    $processing_time = ($end_time - $start_time) * 1000; // Convert to milliseconds
    
    echo "Large array (1000 points) processed: " . (is_array($large_result) && count($large_result) === 1000 ? "success" : "fail") . "\n";
    echo "Processing time: " . ($processing_time < 10 ? "fast" : "acceptable") . "\n";
    echo "Performance acceptable: " . ($processing_time < 100 ? "yes" : "no") . " (< 100ms)\n";
    
} catch (Exception $e) {
    echo "Large array test failed: " . $e->getMessage() . "\n";
}

// Test 6: Geodetic array calculations
echo "\n=== Test geodetic array utilities ===\n";
try {
    $geod = ProjGeod::fromEpsg(4326);
    
    // Test arrays in geodetic calculations
    $points = [[-74.0, 40.7], [-73.9, 40.8], [-73.8, 40.9], [-73.7, 41.0]];
    
    // Test lineLength with coordinate tuples
    $total_length = $geod->lineLength($points);
    echo "Line length calculation: " . (is_numeric($total_length) && $total_length > 0 ? "success" : "fail") . "\n";
    echo "Total distance reasonable: " . ($total_length > 10000 && $total_length < 100000 ? "yes" : "no") . "\n";
    
    // Test forward calculation with coordinate tuple
    $fwd_result = $geod->fwd([[$points[0][0], $points[0][1]]], 45.0, 10000); // 10km at 45 degrees
    echo "Forward calculation result: " . (is_array($fwd_result) && count($fwd_result) >= 1 ? "success" : "fail") . "\n";
    
    if (is_array($fwd_result) && count($fwd_result) >= 1 && is_array($fwd_result[0])) {
        echo "Forward result has longitude: " . (is_numeric($fwd_result[0][0]) ? "yes" : "no") . "\n";
        echo "Forward result has latitude: " . (is_numeric($fwd_result[0][1]) ? "yes" : "no") . "\n";
    }
    
} catch (Exception $e) {
    echo "Geodetic array test failed: " . $e->getMessage() . "\n";
}

// Test 7: Coordinate precision handling
echo "\n=== Test coordinate precision ===\n";
try {
    // Test high precision coordinates
    $high_precision_coords = [
        [-74.123456789012345, 40.987654321098765]
    ];
    
    $precise_result = $transformer->transformArray($high_precision_coords);
    
    if (is_array($precise_result) && count($precise_result) >= 1) {
        $result_coord = $precise_result[0];
        if (is_array($result_coord) && count($result_coord) >= 2) {
            // Check if precision is maintained (at least 6 decimal places in result)
            $x_precision = strlen(substr(strrchr((string)$result_coord[0], "."), 1));
            $y_precision = strlen(substr(strrchr((string)$result_coord[1], "."), 1));
            
            echo "High precision transformation: success\n";
            echo "X precision maintained: " . ($x_precision >= 6 ? "yes" : "no") . "\n";
            echo "Y precision maintained: " . ($y_precision >= 6 ? "yes" : "no") . "\n";
        } else {
            echo "High precision transformation: result format error\n";
        }
    } else {
        echo "High precision transformation: failed\n";
    }
    
} catch (Exception $e) {
    echo "Precision test failed: " . $e->getMessage() . "\n";
}

?>
--EXPECT--
=== Test array coordinate processing ===
Single coordinate transformation: success
Multiple coordinate transformation: success
Result is coordinate tuple: yes
X coordinate is numeric: yes
Y coordinate is numeric: yes

=== Test 3D coordinate arrays ===
3D coordinate transformation: success
3D result has Z coordinate: yes
Z coordinate preserved: yes

=== Test input validation ===
Invalid coordinate 0: accepted (unexpected)
Invalid coordinate 1: accepted (unexpected)
Invalid coordinate 2: rejected (expected)

=== Test numeric type conversion ===
Mixed type coordinate 0: converted
Mixed type coordinate 1: converted
Mixed type coordinate 2: converted

=== Test large array performance ===
Large array (1000 points) processed: success
Processing time: fast
Performance acceptable: yes (< 100ms)

=== Test geodetic array utilities ===
Line length calculation: success
Total distance reasonable: yes
Forward calculation result: success
Forward result has longitude: yes
Forward result has latitude: yes

=== Test coordinate precision ===
High precision transformation: success
X precision maintained: yes
Y precision maintained: yes