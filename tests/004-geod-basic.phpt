--TEST--
Geod: Basic geodetic calculations with new factory methods
--SKIPIF--
<?php if (!extension_loaded("proj")) print "skip"; ?>
--FILE--
<?php
// Using global classes: ProjGeod and ProjGeodException

// Test Geod creation from EPSG (WGS84)
echo "Testing Geod creation from EPSG\n";
try {
    $geod = ProjGeod::fromEpsg(4326);
    echo "EPSG:4326 Geod created successfully\n";
    echo "Semi-major axis: " . round($geod->getA(), 1) . "\n";
    echo "Semi-minor axis: " . round($geod->getB(), 1) . "\n";
    echo "Flattening: " . number_format($geod->getF(), 12) . "\n";
    echo "Is sphere: " . ($geod->isSphere() ? "Yes" : "No") . "\n";
    echo "Init string: " . $geod->getInitstring() . "\n";
} catch (ProjGeodException $e) {
    echo "Error: " . $e->getMessage() . "\n";
}

// Test Geod creation from CRS string
echo "\nTesting Geod creation from CRS string\n";
try {
    $geod = ProjGeod::fromCrs("EPSG:4326");
    echo "CRS string Geod created successfully\n";
    echo "Semi-major axis: " . round($geod->getA(), 1) . "\n";
} catch (ProjGeodException $e) {
    echo "Error: " . $e->getMessage() . "\n";
}

// Test Geod creation with explicit parameters
echo "\nTesting Geod creation with explicit parameters\n";
try {
    $geod = ProjGeod::fromParameters(6378137.0, 6356752.314245, "b");
    echo "Explicit Geod created successfully\n";
    echo "Semi-major axis: " . round($geod->getA(), 1) . "\n";
    echo "Semi-minor axis: " . round($geod->getB(), 1) . "\n";
} catch (ProjGeodException $e) {
    echo "Error: " . $e->getMessage() . "\n";
}

// Test forward geodetic computation (single point)
echo "\nTesting forward geodetic computation (single point)\n";
try {
    $geod = ProjGeod::fromEpsg(4326);
    
    // From Boston to point 1000km at 45 degrees - using coordinate tuple
    $result = $geod->fwd([[-71.0, 42.0]], 45.0, 1000000.0);
    echo "Forward result: [" . round($result[0][0], 6) . ", " . round($result[0][1], 6) . "]\n";
    
    if (count($result[0]) > 2) {
        echo "Back azimuth: " . round($result[0][2], 2) . "\n";
    }
} catch (ProjGeodException $e) {
    echo "Error: " . $e->getMessage() . "\n";
}

// Test inverse geodetic computation (single point)
echo "\nTesting inverse geodetic computation (single point)\n";
try {
    $geod = ProjGeod::fromEpsg(4326);
    
    // From Boston to New York - using coordinate tuples
    $result = $geod->inv([[-71.0, 42.0]], [[-74.0, 40.7]]);
    echo "Inverse result - Forward azimuth: " . round($result[0][0], 2) . "\n";
    
    if (count($result[0]) > 1) {
        echo "Back azimuth: " . round($result[0][1], 2) . "\n";
        echo "Distance: " . round($result[0][count($result[0]) - 1], 0) . " meters\n";
    }
} catch (ProjGeodException $e) {
    echo "Error: " . $e->getMessage() . "\n";
}

// Test forward geodetic computation (array)
echo "\nTesting forward geodetic computation (array)\n";
try {
    $geod = ProjGeod::fromEpsg(4326);
    
    $points = [[-71.0, 42.0], [-72.0, 43.0]];
    $azs = [45.0, 90.0];
    $dists = [1000000.0, 500000.0];
    
    $result = $geod->fwd($points, $azs, $dists);
    echo "Array forward: " . count($result) . " points computed\n";
    echo "First result: [" . round($result[0][0], 6) . ", " . round($result[0][1], 6) . "]\n";
} catch (ProjGeodException $e) {
    echo "Error: " . $e->getMessage() . "\n";
}

// Test line length calculation
echo "\nTesting line length calculation\n";
try {
    $geod = ProjGeod::fromEpsg(4326);
    
    // Path from Boston to New York to Philadelphia - using coordinate tuples
    $points = [[-71.0, 42.0], [-74.0, 40.7], [-75.1, 40.0]];
    
    $length = $geod->lineLength($points);
    echo "Total line length: " . round($length / 1000, 1) . " km\n";
} catch (ProjGeodException $e) {
    echo "Error: " . $e->getMessage() . "\n";
}

// Test with different ellipsoid (Clarke 1866)
echo "\nTesting with Clarke 1866 ellipsoid\n";
try {
    $geod = ProjGeod::fromEpsg(4267);  // NAD27 uses Clarke 1866
    echo "Clarke 1866 Geod created successfully\n";
    echo "Semi-major axis: " . round($geod->getA(), 1) . "\n";
    
    $result = $geod->fwd([[-71.0, 42.0]], 45.0, 1000000.0);
    echo "Clarke result: [" . round($result[0][0], 6) . ", " . round($result[0][1], 6) . "]\n";
} catch (ProjGeodException $e) {
    echo "Error: " . $e->getMessage() . "\n";
}

// Test string representation
echo "\nTesting string representation\n";
try {
    $geod = ProjGeod::fromEpsg(4326);
    $str = (string)$geod;
    echo "String representation: " . $str . "\n";
} catch (ProjGeodException $e) {
    echo "Error: " . $e->getMessage() . "\n";
}

// Test accuracy with specific coordinates (Boston-Portland)
echo "\nTesting geodetic accuracy (Boston-Portland)\n";
try {
    $geod = ProjGeod::fromEpsg(4326);
    
    // Test coordinates - Boston and Portland
    $boston_lat = 42.0 + (15.0 / 60.0);   // 42.25
    $boston_lon = -71.0 - (7.0 / 60.0);   // -71.116667
    $portland_lat = 45.0 + (31.0 / 60.0); // 45.516667
    $portland_lon = -123.0 - (41.0 / 60.0); // -123.683333
    
    // Test inverse computation with accuracy validation
    $result = $geod->inv([[$boston_lon, $boston_lat]], [[$portland_lon, $portland_lat]]);
    $az12 = $result[0][0];  // Forward azimuth
    $az21 = $result[0][1];  // Back azimuth 
    $dist = $result[0][2];  // Distance
    
    echo sprintf("Boston-Portland distance: %.1f meters\n", $dist);
    echo sprintf("Forward azimuth: %.3f degrees\n", $az12);
    echo sprintf("Back azimuth: %.3f degrees\n", $az21);
    
    // Validate against expected values (within reasonable tolerance)
    $expected_dist = 4164074.239;
    echo "Distance accuracy: " . (abs($dist - $expected_dist) < 100 ? "good" : "poor") . "\n";
    
    // Test forward/inverse consistency
    $fwd_result = $geod->fwd([[$boston_lon, $boston_lat]], [$az12], [$dist]);
    $end_lon = $fwd_result[0][0];
    $end_lat = $fwd_result[0][1];
    
    $lon_diff = abs($end_lon - $portland_lon);
    $lat_diff = abs($end_lat - $portland_lat);
    echo "Forward/inverse consistency: " . (($lon_diff < 0.0001 && $lat_diff < 0.0001) ? "good" : "poor") . "\n";
    
} catch (ProjGeodException $e) {
    echo "Error: " . $e->getMessage() . "\n";
}

// Test edge case: same point (zero distance)
echo "\nTesting same point calculation\n";
try {
    $geod = ProjGeod::fromEpsg(4326);
    $same_result = $geod->inv([[-71.0, 42.0]], [[-71.0, 42.0]]);
    echo "Same point distance: " . round($same_result[0][2], 1) . " meters\n";
    echo "Same point calculation: " . ($same_result[0][2] < 0.1 ? "correct" : "incorrect") . "\n";
} catch (ProjGeodException $e) {
    echo "Error: " . $e->getMessage() . "\n";
}

// Test parameter validation
echo "\nTesting parameter validation\n";
try {
    $geod = new ProjGeod(-1, 0.5);  // Invalid semi-major axis
    echo "This should have failed!\n";
} catch (ProjGeodException $e) {
    echo "Correctly caught error: " . $e->getMessage() . "\n";
}

try {
    $geod = ProjGeod::fromParameters(6378137.0, -0.5, "f");  // Invalid flattening
    echo "This should have failed!\n";
} catch (ProjGeodException $e) {
    echo "Correctly caught error: " . $e->getMessage() . "\n";
}

echo "\nAll tests completed\n";
?>
--EXPECT--
Testing Geod creation from EPSG
EPSG:4326 Geod created successfully
Semi-major axis: 6378137
Semi-minor axis: 6356752.3
Flattening: 0.003352810665
Is sphere: No
Init string: a=6378137.000000 f=0.0033528107

Testing Geod creation from CRS string
CRS string Geod created successfully
Semi-major axis: 6378137

Testing Geod creation with explicit parameters
Explicit Geod created successfully
Semi-major axis: 6378137
Semi-minor axis: 6356752.3

Testing forward geodetic computation (single point)
Forward result: [-61.520807, 47.996088]
Back azimuth: -128.28

Testing inverse geodetic computation (single point)
Inverse result - Forward azimuth: -118.9
Back azimuth: 59.11
Distance: 289593 meters

Testing forward geodetic computation (array)
Array forward: 2 points computed
First result: [-61.520807, 47.996088]

Testing line length calculation
Total line length: 411.1 km

Testing with Clarke 1866 ellipsoid
Clarke 1866 Geod created successfully
Semi-major axis: 6378206.4
Clarke result: [-61.521095, 47.996147]

Testing string representation
String representation: Geod(a=6378137.000000, f=0.0033528107)

Testing geodetic accuracy (Boston-Portland)
Boston-Portland distance: 4164074.2 meters
Forward azimuth: -66.530 degrees
Back azimuth: 75.654 degrees
Distance accuracy: good
Forward/inverse consistency: good

Testing same point calculation
Same point distance: 0 meters
Same point calculation: correct

Testing parameter validation
Correctly caught error: Semi-major axis must be positive
Correctly caught error: Flattening must be between 0 and 1

All tests completed