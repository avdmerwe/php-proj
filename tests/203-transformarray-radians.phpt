--TEST--
Transformer: transformArray with radians and errcheck parameters
--SKIPIF--
<?php if (!extension_loaded('proj')) die('skip PROJ extension not loaded'); ?>
--FILE--
<?php

// Test 1: transformArray with radians parameter
echo "=== Test transformArray with radians ===\n";
$transformer = ProjTransformer::fromCrs('EPSG:4326', 'EPSG:3857');

// Test that radians parameter is passed through correctly
// For EPSG:4326, the default is degrees, so radians=false should work normally
$coords_degrees = [[40.7128, -74.0060]];
$result1 = $transformer->transformArray($coords_degrees, false, false);
echo "Transform with radians=false: " . (count($result1) === 1 ? "success" : "failed") . "\n";

// With radians=true, we would need to pass radians, but EPSG:4326 expects degrees
// So this is just testing that the parameter is accepted
$result2 = $transformer->transformArray($coords_degrees, true, false);
echo "Transform with radians=true: " . (count($result2) === 1 ? "success" : "failed") . "\n";

// The key test is that the parameter exists and doesn't cause errors
echo "Radians parameter accepted: true\n";

// Test 2: transformArray with errcheck parameter
echo "\n=== Test transformArray with errcheck ===\n";
try {
    // Valid coordinates - should work
    $valid_coords = [[40.7128, -74.0060]];
    $result = $transformer->transformArray($valid_coords, false, true);
    echo "Valid coords with errcheck: success\n";
} catch (Exception $e) {
    echo "Valid coords with errcheck: failed - " . $e->getMessage() . "\n";
}

// Test 3: transformArray with direction parameter
echo "\n=== Test transformArray with direction ===\n";
$forward_result = $transformer->transformArray([[40.7128, -74.0060]], false, false, "FORWARD");
$inverse_result = $transformer->transformArray($forward_result, false, false, "INVERSE");

$orig_lat = $inverse_result[0][0];
$orig_lon = $inverse_result[0][1];
echo sprintf("Round-trip accuracy: lat=%.6f, lon=%.6f\n", $orig_lat, $orig_lon);
echo "Round-trip successful: " . (abs($orig_lat - 40.7128) < 0.0001 && abs($orig_lon - (-74.0060)) < 0.0001 ? "true" : "false") . "\n";

// Test 4: All parameters together
echo "\n=== Test all parameters ===\n";
$rad_coords = [[deg2rad(40.7128), deg2rad(-74.0060)]];
$result = $transformer->transformArray($rad_coords, true, false, "FORWARD");
echo "All parameters work: " . (is_array($result) && count($result) === 1 ? "true" : "false") . "\n";

// Test 5: Backward compatibility (no extra parameters)
echo "\n=== Test backward compatibility ===\n";
$simple_result = $transformer->transformArray([[40.7128, -74.0060]]);
echo "Backward compatible: " . (is_array($simple_result) && count($simple_result) === 1 ? "true" : "false") . "\n";

// Test 6: Parameter order variations
echo "\n=== Test parameter variations ===\n";
// Named parameters
$named_result = $transformer->transformArray(
    coordinates: [[40.7128, -74.0060]], 
    direction: "FORWARD",
    radians: false,
    errcheck: false
);
echo "Named parameters work: " . (is_array($named_result) ? "true" : "false") . "\n";

// Test 7: Consistency with transform() method
echo "\n=== Test consistency with transform() ===\n";
$single_result = $transformer->transform(40.7128, -74.0060, null, null, false, false, "FORWARD");
$array_result = $transformer->transformArray([[40.7128, -74.0060]], false, false, "FORWARD");
$diff_x = abs($single_result[0] - $array_result[0][0]);
$diff_y = abs($single_result[1] - $array_result[0][1]);
echo "Methods consistent: " . ($diff_x < 0.01 && $diff_y < 0.01 ? "true" : "false") . "\n";

?>
--EXPECT--
=== Test transformArray with radians ===
Transform with radians=false: success
Transform with radians=true: success
Radians parameter accepted: true

=== Test transformArray with errcheck ===
Valid coords with errcheck: success

=== Test transformArray with direction ===
Round-trip accuracy: lat=40.712800, lon=-74.006000
Round-trip successful: true

=== Test all parameters ===
All parameters work: true

=== Test backward compatibility ===
Backward compatible: true

=== Test parameter variations ===
Named parameters work: true

=== Test consistency with transform() ===
Methods consistent: true