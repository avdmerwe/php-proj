--TEST--
Transformer: Basic coordinate transformations
--SKIPIF--
<?php if (!extension_loaded("proj")) print "skip"; ?>
--FILE--
<?php
// Updated to use global namespace classes
// CRS is now ProjCRS in global namespace

// Test basic transformation from geographic to projected
echo "Testing basic coordinate transformation\n";
try {
    $transformer = ProjTransformer::fromCrs('EPSG:4326', 'EPSG:3857');
    echo "Transformer created successfully\n";
    echo "Description: " . $transformer->getDescription() . "\n";
    echo "Has inverse: " . ($transformer->hasInverse() ? "Yes" : "No") . "\n";
    
    // Transform a single point (longitude, latitude)
    $result = $transformer->transform(-74.0, 40.7);
    echo "Transformed point: [" . round($result[0], 2) . ", " . round($result[1], 2) . "]\n";
    
} catch (ProjTransformerException $e) {
    echo "Error: " . $e->getMessage() . "\n";
}

// Test transformation with CRS objects
echo "\nTesting transformation with CRS objects\n";
try {
    $src_crs = ProjCRS::fromEpsg(4326);
    $dst_crs = ProjCRS::fromEpsg(3857);
    $transformer = ProjTransformer::fromCrs($src_crs, $dst_crs);
    
    $result = $transformer->transform(-74.0, 40.7);
    echo "Transformed with CRS objects: [" . round($result[0], 2) . ", " . round($result[1], 2) . "]\n";
    
} catch (ProjTransformerException $e) {
    echo "Error: " . $e->getMessage() . "\n";
}

// Test array transformation
echo "\nTesting array coordinate transformation\n";
try {
    $transformer = ProjTransformer::fromCrs('EPSG:4326', 'EPSG:3857');
    
    $lons = [-74.0, -73.9, -73.8];
    $lats = [40.7, 40.8, 40.9];
    
    $result = $transformer->transform($lons, $lats);
    echo "Transformed " . count($result[0]) . " points\n";
    echo "First point: [" . round($result[0][0], 2) . ", " . round($result[1][0], 2) . "]\n";
    
} catch (ProjTransformerException $e) {
    echo "Error: " . $e->getMessage() . "\n";
}

// Test inverse transformation
echo "\nTesting inverse transformation\n";
try {
    $transformer = ProjTransformer::fromCrs('EPSG:4326', 'EPSG:3857');
    
    // Forward transformation
    $forward = $transformer->transform(-74.0, 40.7);
    echo "Forward: [" . round($forward[0], 2) . ", " . round($forward[1], 2) . "]\n";
    
    // Inverse transformation
    $inverse = $transformer->transform($forward[0], $forward[1], null, null, false, false, 'INVERSE');
    echo "Inverse: [" . round($inverse[0], 6) . ", " . round($inverse[1], 6) . "]\n";
    
} catch (ProjTransformerException $e) {
    echo "Error: " . $e->getMessage() . "\n";
}

// Test transformation from pipeline
echo "\nTesting transformation from pipeline\n";
try {
    $transformer = ProjTransformer::fromPipeline('+proj=pipeline +step +proj=longlat +ellps=WGS84 +step +proj=webmerc +ellps=WGS84');
    echo "Pipeline transformer created\n";
    
    $result = $transformer->transform(-74.0, 40.7);
    echo "Pipeline result: [" . round($result[0], 2) . ", " . round($result[1], 2) . "]\n";
    
} catch (ProjTransformerException $e) {
    echo "Error: " . $e->getMessage() . "\n";
}

// Test transformation with 3D coordinates
echo "\nTesting 3D coordinate transformation\n";
try {
    $transformer = ProjTransformer::fromCrs('EPSG:4326', 'EPSG:3857');
    
    $result = $transformer->transform(-74.0, 40.7, 100.0);
    echo "3D result: [" . round($result[0], 2) . ", " . round($result[1], 2) . ", " . round($result[2], 2) . "]\n";
    
} catch (ProjTransformerException $e) {
    echo "Error: " . $e->getMessage() . "\n";
}

// Test always_xy parameter
echo "\nTesting always_xy parameter\n";
try {
    $transformer = ProjTransformer::fromCrs('EPSG:4326', 'EPSG:3857', true);
    echo "Always XY transformer created\n";
    
    $result = $transformer->transform(-74.0, 40.7);
    echo "Always XY result: [" . round($result[0], 2) . ", " . round($result[1], 2) . "]\n";
    
} catch (ProjTransformerException $e) {
    echo "Error: " . $e->getMessage() . "\n";
}

echo "\nAll tests completed\n";
?>
--EXPECT--
Testing basic coordinate transformation
Transformer created successfully
Description: Popular Visualisation Pseudo-Mercator
Has inverse: Yes
Transformed point: [4530703.28, -12515545.21]

Testing transformation with CRS objects
Transformed with CRS objects: [4530703.28, -12515545.21]

Testing array coordinate transformation
Transformed 3 points
First point: [4530703.28, -12515545.21]

Testing inverse transformation
Forward: [4530703.28, -12515545.21]
Inverse: [-74, 40.7]

Testing transformation from pipeline
Pipeline transformer created
Pipeline result: [-8237642.32, 4968191.93]

Testing 3D coordinate transformation
3D result: [4530703.28, -12515545.21, 100]

Testing always_xy parameter
Always XY transformer created
Always XY result: [-8237642.32, 4968191.93]

All tests completed