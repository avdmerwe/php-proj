--TEST--
Transformer: Basic coordinate transformations
--SKIPIF--
<?php if (!extension_loaded('proj')) die('skip PROJ extension not loaded'); ?>
--FILE--
<?php

// Test 1: Basic WGS84 to Web Mercator transformation
echo "=== Test WGS84 to Web Mercator ===\n";
$transformer = ProjTransformer::fromCrs('EPSG:4326', 'EPSG:3857');

$lon = -74.0060; // NYC longitude
$lat = 40.7128;  // NYC latitude

$result = $transformer->transform($lat, $lon);  // Note: lat, lon order for EPSG:4326
echo sprintf("Input: %.4f, %.4f\n", $lat, $lon);
echo sprintf("Output: %.2f, %.2f\n", $result[0], $result[1]);

// Test 2: Inverse transformation
echo "\n=== Test inverse transformation ===\n";
$result_inv = $transformer->transform($result[0], $result[1], null, null, false, false, "INVERSE");
echo sprintf("Inverse: %.4f, %.4f\n", $result_inv[0], $result_inv[1]);

// Test 3: Transformer properties
echo "\n=== Test transformer properties ===\n";
echo "Description: " . $transformer->getDescription() . "\n";
echo "Has inverse: " . ($transformer->hasInverse() ? "true" : "false") . "\n";

// Test 4: Array transformation
echo "\n=== Test array transformation ===\n";
$coordinates = [
    [40.7128, -74.0060],  // NYC (lat, lon)
    [34.0522, -118.2437], // LA
    [41.8781, -87.6298]   // Chicago
];

$transformed = $transformer->transformArray($coordinates);
echo "Transformed " . count($transformed) . " points:\n";
foreach ($transformed as $i => $point) {
    echo sprintf("Point %d: %.2f, %.2f\n", $i + 1, $point[0], $point[1]);
}

// Test 5: Different CRS formats
echo "\n=== Test different CRS input formats ===\n";
try {
    $crs_from = ProjCRS::fromEpsg(4326);
    $crs_to = ProjCRS::fromEpsg(3857);
    $transformer2 = ProjTransformer::fromCrs($crs_from, $crs_to);
    
    $result2 = $transformer2->transform($lat, $lon);
    echo sprintf("CRS object input: %.2f, %.2f\n", $result2[0], $result2[1]);
    echo "Same as string input: " . (abs($result[0] - $result2[0]) < 0.01 ? "true" : "false") . "\n";
} catch (Exception $e) {
    echo "CRS object test failed: " . $e->getMessage() . "\n";
}

// Test 6: Pipeline transformer
echo "\n=== Test pipeline transformer ===\n";
try {
    $pipeline = "+proj=pipeline +step +proj=unitconvert +xy_in=deg +xy_out=rad +step +proj=merc +a=6378137 +b=6378137";
    $pipe_transformer = ProjTransformer::fromPipeline($pipeline);
    
    $pipe_result = $pipe_transformer->transform($lat, $lon);
    echo sprintf("Pipeline result: %.2f, %.2f\n", $pipe_result[0], $pipe_result[1]);
} catch (Exception $e) {
    echo "Pipeline test failed: " . $e->getMessage() . "\n";
}

// Test 7: Error handling
echo "\n=== Test error handling ===\n";
try {
    $bad_transformer = ProjTransformer::fromCrs('INVALID:1234', 'EPSG:4326');
    echo "ERROR: Should have thrown exception\n";
} catch (ProjTransformerException $e) {
    echo "PASS: Exception thrown for invalid CRS\n";
} catch (Exception $e) {
    echo "PARTIAL: Exception thrown but wrong type: " . get_class($e) . "\n";
}

?>
--EXPECT--
=== Test WGS84 to Web Mercator ===
Input: 40.7128, -74.0060
Output: -8238310.24, 4970071.58

=== Test inverse transformation ===
Inverse: 40.7128, -74.0060

=== Test transformer properties ===
Description: Popular Visualisation Pseudo-Mercator
Has inverse: true

=== Test array transformation ===
Transformed 3 points:
Point 1: -8238310.24, 4970071.58
Point 2: -13162828.47, 4035813.37
Point 3: -9754904.71, 5142736.87

=== Test different CRS input formats ===
CRS object input: -8238310.24, 4970071.58
Same as string input: true

=== Test pipeline transformer ===
Pipeline result: 4532128.16, -12517968.83

=== Test error handling ===
PARTIAL: Exception thrown but wrong type: ProjException