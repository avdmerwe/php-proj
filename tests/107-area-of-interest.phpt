--TEST--
Area of Interest: Geographic area specifications and bounding box operations
--SKIPIF--
<?php if (!extension_loaded('proj')) die('skip PROJ extension not loaded'); ?>
--FILE--
<?php

// Test 1: ProjAreaOfInterest basic creation and properties
echo "=== Test ProjAreaOfInterest creation ===\n";
try {
    // Test basic constructor with valid coordinates
    $aoi = new ProjAreaOfInterest(1.0, 1.0, 4.0, 4.0);
    echo "AOI creation successful: true\n";
    
    // Test accessing properties
    echo "AOI west: " . $aoi->west_lon_degree . "\n";
    echo "AOI south: " . $aoi->south_lat_degree . "\n";
    echo "AOI east: " . $aoi->east_lon_degree . "\n";
    echo "AOI north: " . $aoi->north_lat_degree . "\n";
    
} catch (Exception $e) {
    echo "AOI creation failed: " . $e->getMessage() . "\n";
}

// Test 2: Area of Interest constructor validation
echo "\n=== Test AOI constructor validation ===\n";
try {
    // Test valid coordinate orders
    $valid_aoi = new ProjAreaOfInterest(-180.0, -90.0, 180.0, 90.0);
    echo "Global AOI creation: true\n";
    
    $small_aoi = new ProjAreaOfInterest(0.5, 0.5, 1.5, 1.5);
    echo "Small AOI creation: true\n";
    
} catch (Exception $e) {
    echo "Valid AOI validation failed: " . $e->getMessage() . "\n";
}

// Test 3: Invalid coordinate validation
echo "\n=== Test invalid coordinate validation ===\n";
$invalid_tests = [
    'null west' => [null, 1.0, 4.0, 4.0],
    'null south' => [1.0, null, 4.0, 4.0], 
    'null east' => [1.0, 1.0, null, 4.0],
    'null north' => [1.0, 1.0, 4.0, null],
    'west > east' => [4.0, 1.0, 1.0, 4.0],
    'south > north' => [1.0, 4.0, 4.0, 1.0],
];

$error_count = 0;
foreach ($invalid_tests as $test_name => $coords) {
    try {
        $invalid_aoi = new ProjAreaOfInterest($coords[0], $coords[1], $coords[2], $coords[3]);
        echo "Invalid $test_name unexpectedly succeeded\n";
    } catch (ValueError $e) {
        $error_count++;
        echo "Invalid $test_name properly rejected\n";
    } catch (Exception $e) {
        $error_count++;
        echo "Invalid $test_name properly rejected\n";
    }
}
echo "Validation error rate: $error_count/" . count($invalid_tests) . "\n";

// Test 4: BBox compatibility alias
echo "\n=== Test BBox alias functionality ===\n";
try {
    // Test that BBox class exists and behaves like AreaOfInterest
    if (class_exists('BBox')) {
        $bbox = new BBox(2.0, 2.0, 5.0, 5.0);
        echo "BBox creation successful: true\n";
        echo "BBox is instanceof ProjAreaOfInterest: " . ($bbox instanceof ProjAreaOfInterest ? "true" : "false") . "\n";
    } else {
        echo "BBox class available: false\n";
        echo "BBox alias not implemented: expected\n";
    }
    
} catch (Exception $e) {
    echo "BBox test failed: " . $e->getMessage() . "\n";
}

// Test 5: Area containment testing
echo "\n=== Test area containment operations ===\n";
try {
    $outer_aoi = new ProjAreaOfInterest(1.0, 1.0, 4.0, 4.0);
    $inner_aoi = new ProjAreaOfInterest(2.0, 2.0, 3.0, 3.0);
    $overlap_aoi = new ProjAreaOfInterest(2.0, 2.0, 5.0, 5.0);
    $separate_aoi = new ProjAreaOfInterest(10.0, 10.0, 20.0, 20.0);
    
    // Test containment method
    if (method_exists($outer_aoi, 'contains')) {
        echo "Contains method available: true\n";
        echo "Outer contains inner: " . ($outer_aoi->contains($inner_aoi) ? "true" : "false") . "\n";
        echo "Outer contains overlap: " . ($outer_aoi->contains($overlap_aoi) ? "true" : "false") . "\n";
        echo "Outer contains separate: " . ($outer_aoi->contains($separate_aoi) ? "true" : "false") . "\n";
    } else {
        echo "Contains method available: false\n";
        echo "Contains functionality not implemented: expected\n";
    }
    
} catch (Exception $e) {
    echo "Containment test failed: " . $e->getMessage() . "\n";
}

// Test 6: Area intersection testing
echo "\n=== Test area intersection operations ===\n";
try {
    $aoi1 = new ProjAreaOfInterest(1.0, 1.0, 4.0, 4.0);
    $aoi2 = new ProjAreaOfInterest(2.0, 2.0, 5.0, 5.0);
    $aoi3 = new ProjAreaOfInterest(10.0, 10.0, 20.0, 20.0);
    
    // Test intersection method
    if (method_exists($aoi1, 'intersects')) {
        echo "Intersects method available: true\n";
        echo "AOI1 intersects AOI2: " . ($aoi1->intersects($aoi2) ? "true" : "false") . "\n";
        echo "AOI1 intersects AOI3: " . ($aoi1->intersects($aoi3) ? "true" : "false") . "\n";
    } else {
        echo "Intersects method available: false\n";
        echo "Intersects functionality not implemented: expected\n";
    }
    
} catch (Exception $e) {
    echo "Intersection test failed: " . $e->getMessage() . "\n";
}

// Test 7: Area of Interest with coordinate transformations
echo "\n=== Test AOI with transformations ===\n";
try {
    // Test using AOI with database queries
    $test_aoi = new ProjAreaOfInterest(-10.0, 40.0, 10.0, 60.0); // Europe roughly
    
    // Test if AOI can be used with database functions
    if (function_exists('proj_get_crs_info_list_from_database')) {
        $crs_list = proj_get_crs_info_list_from_database('EPSG', 'PROJECTED_CRS', $test_aoi);
        echo "AOI with database query: " . (is_array($crs_list) ? "success" : "failed") . "\n";
        echo "CRS count with AOI filter: " . (is_array($crs_list) ? count($crs_list) : 0) . "\n";
    } else {
        echo "AOI with database query: not available\n";
        echo "Database AOI filtering not implemented: expected\n";
    }
    
} catch (Exception $e) {
    echo "AOI transformation test failed: " . $e->getMessage() . "\n";
}

// Test 8: Area of Interest coordinate validation ranges
echo "\n=== Test AOI coordinate range validation ===\n";
try {
    // Test extreme but valid coordinates
    $global_aoi = new ProjAreaOfInterest(-180.0, -90.0, 180.0, 90.0);
    echo "Global range AOI: true\n";
    
    $small_range_aoi = new ProjAreaOfInterest(0.0001, 0.0001, 0.0002, 0.0002);
    echo "Very small AOI: true\n";
    
    // Test coordinates that exceed normal geographic bounds
    $invalid_coords = [
        'longitude too west' => [-181.0, 0.0, 0.0, 1.0],
        'longitude too east' => [0.0, 0.0, 181.0, 1.0],
        'latitude too south' => [0.0, -91.0, 1.0, 0.0],
        'latitude too north' => [0.0, 0.0, 1.0, 91.0],
    ];
    
    $boundary_error_count = 0;
    foreach ($invalid_coords as $test_name => $coords) {
        try {
            $boundary_aoi = new ProjAreaOfInterest($coords[0], $coords[1], $coords[2], $coords[3]);
            echo "Boundary test $test_name: allowed\n";
        } catch (Exception $e) {
            $boundary_error_count++;
            echo "Boundary test $test_name: rejected\n";
        }
    }
    
    echo "Boundary validation errors: $boundary_error_count/" . count($invalid_coords) . "\n";
    
} catch (Exception $e) {
    echo "Range validation failed: " . $e->getMessage() . "\n";
}

// Test 9: Area calculation and properties
echo "\n=== Test AOI area calculations ===\n";
try {
    $aoi = new ProjAreaOfInterest(0.0, 0.0, 1.0, 1.0);
    
    // Test calculated properties if available
    if (method_exists($aoi, 'getArea')) {
        $area = $aoi->getArea();
        echo "Area calculation available: true\n";
        echo "Area value reasonable: " . ($area > 0 ? "true" : "false") . "\n";
    } else {
        echo "Area calculation available: false\n";
    }
    
    if (method_exists($aoi, 'getWidth')) {
        $width = $aoi->getWidth();
        echo "Width calculation: " . $width . "\n";
    } else {
        echo "Width calculation available: false\n";
    }
    
    if (method_exists($aoi, 'getHeight')) {
        $height = $aoi->getHeight();
        echo "Height calculation: " . $height . "\n";
    } else {
        echo "Height calculation available: false\n";
    }
    
} catch (Exception $e) {
    echo "Area calculation failed: " . $e->getMessage() . "\n";
}

// Test 10: AOI serialization and string representation
echo "\n=== Test AOI serialization ===\n";
try {
    $aoi = new ProjAreaOfInterest(1.5, 2.5, 3.5, 4.5);
    
    // Test string representation
    if (method_exists($aoi, '__toString')) {
        $str_repr = (string)$aoi;
        echo "String representation available: true\n";
        echo "String contains coordinates: " . (strpos($str_repr, '1.5') !== false ? "true" : "false") . "\n";
    } else {
        echo "String representation available: false\n";
    }
    
    // Test serialization
    $serialized = serialize($aoi);
    echo "Serialization successful: " . (strlen($serialized) > 0 ? "true" : "false") . "\n";
    
    $unserialized = unserialize($serialized);
    echo "Unserialization successful: " . ($unserialized instanceof ProjAreaOfInterest ? "true" : "false") . "\n";
    
    if ($unserialized instanceof ProjAreaOfInterest) {
        echo "Coordinates preserved: " . ($unserialized->west_lon_degree == 1.5 ? "true" : "false") . "\n";
    }
    
} catch (Exception $e) {
    echo "Serialization test failed: " . $e->getMessage() . "\n";
}

?>
--EXPECT--
=== Test ProjAreaOfInterest creation ===
AOI creation successful: true
AOI west: 1
AOI south: 1
AOI east: 4
AOI north: 4

=== Test AOI constructor validation ===
Global AOI creation: true
Small AOI creation: true

=== Test invalid coordinate validation ===
Invalid null west properly rejected
Invalid null south properly rejected
Invalid null east properly rejected
Invalid null north properly rejected
Invalid west > east properly rejected
Invalid south > north properly rejected
Validation error rate: 6/6

=== Test BBox alias functionality ===
BBox class available: false
BBox alias not implemented: expected

=== Test area containment operations ===
Contains method available: true
Outer contains inner: true
Outer contains overlap: false
Outer contains separate: false

=== Test area intersection operations ===
Intersects method available: true
AOI1 intersects AOI2: true
AOI1 intersects AOI3: false

=== Test AOI with transformations ===
AOI with database query: success
CRS count with AOI filter: 7497

=== Test AOI coordinate range validation ===
Global range AOI: true
Very small AOI: true
Boundary test longitude too west: allowed
Boundary test longitude too east: allowed
Boundary test latitude too south: allowed
Boundary test latitude too north: allowed
Boundary validation errors: 0/4

=== Test AOI area calculations ===
Area calculation available: false
Width calculation available: false
Height calculation available: false

=== Test AOI serialization ===
String representation available: true
String contains coordinates: true
Serialization successful: true
Unserialization successful: true
Coordinates preserved: true
