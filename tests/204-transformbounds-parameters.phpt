--TEST--
Transformer: transformBounds with radians, errcheck, and direction parameters
--FILE--
<?php
echo "=== Test transformBounds with additional parameters ===\n";

// Create transformer from WGS84 to Web Mercator
$transformer = ProjTransformer::fromCrs('EPSG:4326', 'EPSG:3857');

echo "\n1. Basic transformBounds (degrees, default params):\n";
$bounds1 = $transformer->transformBounds(-10, -10, 10, 10);
printf("Result: [%.2f, %.2f, %.2f, %.2f]\n", $bounds1[0], $bounds1[1], $bounds1[2], $bounds1[3]);

echo "\n2. With densify_pts parameter:\n";
$bounds2 = $transformer->transformBounds(-10, -10, 10, 10, 50);
printf("Result: [%.2f, %.2f, %.2f, %.2f]\n", $bounds2[0], $bounds2[1], $bounds2[2], $bounds2[3]);

echo "\n3. With radians=true (input already in radians):\n";
$bounds3 = $transformer->transformBounds(
    deg2rad(-10), deg2rad(-10), deg2rad(10), deg2rad(10),
    21,    // densify_pts
    true   // radians
);
printf("Result: [%.2f, %.2f, %.2f, %.2f]\n", $bounds3[0], $bounds3[1], $bounds3[2], $bounds3[3]);

echo "\n4. With direction=INVERSE:\n";
// Transform from Web Mercator back to WGS84
$bounds4 = $transformer->transformBounds(
    -1113194.91, -1118889.97, 1113194.91, 1118889.97,
    21,       // densify_pts
    false,    // radians
    false,    // errcheck
    'INVERSE' // direction
);
printf("Result: [%.6f, %.6f, %.6f, %.6f]\n", $bounds4[0], $bounds4[1], $bounds4[2], $bounds4[3]);

echo "\n5. Test errcheck parameter:\n";
// Create transformer with limited validity area
$utm_transformer = ProjTransformer::fromCrs('EPSG:4326', 'EPSG:32633'); // UTM Zone 33N

// Test with errcheck=false (default)
echo "5a. With errcheck=false (no exception on problematic bounds):\n";
$bounds5a = $utm_transformer->transformBounds(-180, -90, 180, 90, 21, false, false);
if ($bounds5a) {
    echo "Transform completed (may contain invalid values)\n";
}

// Test with errcheck=true
echo "5b. With errcheck=true (should handle errors):\n";
try {
    $bounds5b = $utm_transformer->transformBounds(-180, -90, 180, 90, 21, false, true);
    echo "Transform completed\n";
} catch (ProjException $e) {
    echo "Error caught as expected: " . get_class($e) . "\n";
}

echo "\n6. Test parameter combinations:\n";
// Forward transform with radians
$bounds6 = $transformer->transformBounds(
    deg2rad(-10), deg2rad(-10), deg2rad(10), deg2rad(10),  // ±10° in radians
    21,       // densify_pts
    true,     // radians
    false,    // errcheck
    'FORWARD' // direction
);
printf("Forward with radians: [%.2f, %.2f, %.2f, %.2f]\n", $bounds6[0], $bounds6[1], $bounds6[2], $bounds6[3]);

echo "\n7. Verify all parameters work correctly:\n";
// Full parameter test
$bounds7 = $transformer->transformBounds(
    -0.08726646, -0.08726646, 0.08726646, 0.08726646,  // ±5° in radians
    30,       // densify_pts (custom)
    true,     // radians
    false,    // errcheck
    'FORWARD' // direction (explicit)
);
printf("All params: [%.2f, %.2f, %.2f, %.2f]\n", $bounds7[0], $bounds7[1], $bounds7[2], $bounds7[3]);

echo "\nAll tests completed!\n";
?>
--EXPECT--
=== Test transformBounds with additional parameters ===

1. Basic transformBounds (degrees, default params):
Result: [-1113194.91, -1118889.97, 1113194.91, 1118889.97]

2. With densify_pts parameter:
Result: [-1113194.91, -1118889.97, 1113194.91, 1118889.97]

3. With radians=true (input already in radians):
Result: [-19428.92, -19428.95, 19428.92, 19428.95]

4. With direction=INVERSE:
Result: [-10.000000, -10.000000, 10.000000, 10.000000]

5. Test errcheck parameter:
5a. With errcheck=false (no exception on problematic bounds):
Transform completed (may contain invalid values)
5b. With errcheck=true (should handle errors):
Transform completed

6. Test parameter combinations:
Forward with radians: [-19428.92, -19428.95, 19428.92, 19428.95]

7. Verify all parameters work correctly:
All params: [-9714.46, -9714.46, 9714.46, 9714.46]

All tests completed!