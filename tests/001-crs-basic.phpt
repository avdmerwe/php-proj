--TEST--
CRS: Basic CRS creation and properties
--SKIPIF--
<?php if (!extension_loaded("proj")) print "skip"; ?>
--FILE--
<?php
// No namespace imports needed - using global namespace

// Test basic CRS creation from EPSG code
echo "Testing CRS creation from EPSG code\n";
try {
    $crs = ProjCRS::fromEpsg(4326);
    echo "CRS created successfully\n";
    echo "Name: " . $crs->getName() . "\n";
    echo "Type: " . $crs->getTypeName() . "\n";
    echo "Is Geographic: " . ($crs->isGeographic() ? "Yes" : "No") . "\n";
    echo "Is Projected: " . ($crs->isProjected() ? "Yes" : "No") . "\n";
} catch (ProjCRSException $e) {
    echo "Error: " . $e->getMessage() . "\n";
}

// Test CRS creation from string
echo "\nTesting CRS creation from string\n";
try {
    $crs = ProjCRS::fromString("EPSG:3857");
    echo "CRS created successfully\n";
    echo "Name: " . $crs->getName() . "\n";
    echo "Is Projected: " . ($crs->isProjected() ? "Yes" : "No") . "\n";
} catch (ProjCRSException $e) {
    echo "Error: " . $e->getMessage() . "\n";
}

// Test CRS creation from user input
echo "\nTesting CRS creation from user input (integer)\n";
try {
    $crs = ProjCRS::fromUserInput(4326);
    echo "CRS created successfully\n";
    echo "Name: " . $crs->getName() . "\n";
} catch (ProjCRSException $e) {
    echo "Error: " . $e->getMessage() . "\n";
}

// Test invalid CRS
echo "\nTesting invalid CRS\n";
try {
    $crs = ProjCRS::fromEpsg(999999);
    echo "Should not reach here\n";
} catch (ProjCRSException $e) {
    echo "Expected error caught: Invalid EPSG code\n";
}

// Test CRS conversion methods
echo "\nTesting CRS conversion methods\n";
try {
    $crs = ProjCRS::fromEpsg(4326);
    
    $wkt = $crs->toWkt();
    echo "WKT length: " . strlen($wkt) . "\n";
    
    $proj4 = $crs->toProj4();
    echo "PROJ4: " . $proj4 . "\n";
    
    $json = $crs->toJson();
    echo "JSON length: " . strlen($json) . "\n";
    
} catch (ProjCRSException $e) {
    echo "Error: " . $e->getMessage() . "\n";
}

// Test CRS equality
echo "\nTesting CRS equality\n";
try {
    $crs1 = ProjCRS::fromEpsg(4326);
    $crs2 = ProjCRS::fromEpsg(4326);
    $crs3 = ProjCRS::fromEpsg(3857);
    
    echo "CRS1 equals CRS2: " . ($crs1->equals($crs2) ? "Yes" : "No") . "\n";
    echo "CRS1 equals CRS3: " . ($crs1->equals($crs3) ? "Yes" : "No") . "\n";
    
} catch (ProjCRSException $e) {
    echo "Error: " . $e->getMessage() . "\n";
}

echo "\nAll tests completed\n";
?>
--EXPECT--
Testing CRS creation from EPSG code
CRS created successfully
Name: WGS 84
Type: Geographic 2D CRS
Is Geographic: Yes
Is Projected: No

Testing CRS creation from string
CRS created successfully
Name: WGS 84 / Pseudo-Mercator
Is Projected: Yes

Testing CRS creation from user input (integer)
CRS created successfully
Name: WGS 84

Testing invalid CRS
Expected error caught: Invalid EPSG code

Testing CRS conversion methods
WKT length: 1046
PROJ4: +proj=longlat +datum=WGS84 +no_defs +type=crs
JSON length: 2096

Testing CRS equality
CRS1 equals CRS2: Yes
CRS1 equals CRS3: No

All tests completed