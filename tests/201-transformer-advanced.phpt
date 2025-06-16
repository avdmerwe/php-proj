--TEST--
Transformer: Advanced transformation features
--SKIPIF--
<?php if (!extension_loaded('proj')) die('skip PROJ extension not loaded'); ?>
--FILE--
<?php

// Test 1: 3D coordinate transformations
echo "=== Test 3D transformations ===\n";
$transformer = ProjTransformer::fromCrs('EPSG:4326', 'EPSG:3857');
$result_3d = $transformer->transform(40.7128, -74.0060, 100.0);
echo "3D result count: " . count($result_3d) . "\n";
echo sprintf("3D elevation preserved: %.1f\n", $result_3d[2]);

// Test 2: 4D coordinate transformations
echo "\n=== Test 4D transformations ===\n";
$result_4d = $transformer->transform(40.7128, -74.0060, 100.0, 2023.5);
echo "4D result count: " . count($result_4d) . "\n";
echo sprintf("Time preserved: %.1f\n", $result_4d[3]);

// Test 3: Batch 3D transformations
echo "\n=== Test batch 3D ===\n";
$coords_3d = [
    [40.7128, -74.0060, 100.0],
    [34.0522, -118.2437, 50.0]
];
$batch_3d = $transformer->transformArray($coords_3d);
echo "Batch 3D count: " . count($batch_3d) . "\n";
echo "First point has elevation: " . (count($batch_3d[0]) === 3 ? "true" : "false") . "\n";

// Test 4: Inverse transformation accuracy
echo "\n=== Test inverse accuracy ===\n";
$forward = $transformer->transform(40.7128, -74.0060);
$inverse = $transformer->transform($forward[0], $forward[1], null, null, false, false, "INVERSE");
$accuracy = abs(40.7128 - $inverse[0]);
echo "Round-trip accurate: " . ($accuracy < 0.0001 ? "true" : "false") . "\n";

// Test 5: Transformer properties
echo "\n=== Test transformer properties ===\n";
echo "Has inverse: " . ($transformer->hasInverse() ? "true" : "false") . "\n";
echo "Description length > 0: " . (strlen($transformer->getDescription()) > 0 ? "true" : "false") . "\n";
$accuracy = $transformer->getAccuracy();
echo "Has accuracy info: " . ($accuracy >= 0 ? "true" : "false") . "\n";

// Test 6: Format conversions
echo "\n=== Test format conversions ===\n";
$wkt = $transformer->toWkt();
$proj4 = $transformer->toProj4();
echo "WKT available: " . (strlen($wkt) > 0 ? "true" : "false") . "\n";
echo "PROJ4 available: " . (strlen($proj4) > 0 ? "true" : "false") . "\n";

// Test 7: Pipeline transformations
echo "\n=== Test pipeline ===\n";
try {
    $pipeline = "+proj=pipeline +step +proj=unitconvert +xy_in=deg +xy_out=rad +step +proj=unitconvert +xy_in=rad +xy_out=deg";
    $pipe_transformer = ProjTransformer::fromPipeline($pipeline);
    $pipe_result = $pipe_transformer->transform(40.7128, -74.0060);
    echo "Pipeline round-trip accurate: " . (abs($pipe_result[0] - 40.7128) < 0.01 ? "true" : "false") . "\n";
} catch (Exception $e) {
    echo "Pipeline failed: " . $e->getMessage() . "\n";
}

// Test 8: Large coordinate handling
echo "\n=== Test edge coordinates ===\n";
try {
    $result = $transformer->transform(89.0, 179.0);
    echo "Large coordinates: " . (is_array($result) && count($result) >= 2 ? "success" : "failed") . "\n";
} catch (Exception $e) {
    echo "Large coordinates failed: " . $e->getMessage() . "\n";
}

// Test 9: Empty array handling
echo "\n=== Test empty array ===\n";
$empty_result = $transformer->transformArray([]);
echo "Empty array result count: " . count($empty_result) . "\n";

// Test 10: Performance test
echo "\n=== Test performance ===\n";
$coords = [];
for ($i = 0; $i < 50; $i++) {
    $coords[] = [40.0 + ($i * 0.01), -74.0];
}
$start = microtime(true);
$results = $transformer->transformArray($coords);
$time = microtime(true) - $start;
echo "50 points transformed: " . (count($results) === 50 ? "true" : "false") . "\n";
echo "Performance reasonable: " . ($time < 0.1 ? "true" : "false") . "\n";

?>
--EXPECT--
=== Test 3D transformations ===
3D result count: 3
3D elevation preserved: 100.0

=== Test 4D transformations ===
4D result count: 4
Time preserved: 2023.5

=== Test batch 3D ===
Batch 3D count: 2
First point has elevation: true

=== Test inverse accuracy ===
Round-trip accurate: true

=== Test transformer properties ===
Has inverse: true
Description length > 0: true
Has accuracy info: true

=== Test format conversions ===
WKT available: true
PROJ4 available: true

=== Test pipeline ===
Pipeline round-trip accurate: true

=== Test edge coordinates ===
Large coordinates: success

=== Test empty array ===
Empty array result count: 0

=== Test performance ===
50 points transformed: true
Performance reasonable: true