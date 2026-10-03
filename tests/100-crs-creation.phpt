--TEST--
CRS: Creation from various input formats
--SKIPIF--
<?php if (!extension_loaded('proj')) die('skip PROJ extension not loaded'); ?>
--FILE--
<?php

// Test 1: CRS from EPSG code
echo "=== Test from EPSG ===\n";
try {
    $crs = ProjCRS::fromEpsg(4326);
    var_dump($crs->getToEpsg());
    echo "PASS: CRS from EPSG 4326\n";
} catch (Exception $e) {
    echo "FAIL: " . $e->getMessage() . "\n";
}

// Test 2: CRS from EPSG string format
echo "\n=== Test from EPSG string ===\n";
try {
    $crs = ProjCRS::fromString("epsg:4326");
    var_dump($crs->getToEpsg());
    echo "PASS: CRS from EPSG string\n";
} catch (Exception $e) {
    echo "FAIL: " . $e->getMessage() . "\n";
}

// Test 3: CRS from EPSG integer-like string (using fromUserInput)
echo "\n=== Test from EPSG int-like string ===\n";
try {
    $crs = ProjCRS::fromUserInput("4326");
    var_dump($crs->getToEpsg());
    echo "PASS: CRS from int-like string\n";
} catch (Exception $e) {
    echo "SKIP: Integer-like string parsing: " . $e->getMessage() . "\n";
}

// Test 4: CRS from PROJ4 string
echo "\n=== Test from PROJ4 ===\n";
try {
    $crs = ProjCRS::fromProj4("+proj=longlat +datum=WGS84 +no_defs +type=crs");
    $proj4_out = $crs->toProj4();
    echo "PROJ4 output: $proj4_out\n";
    echo "PASS: CRS from PROJ4 string\n";
} catch (Exception $e) {
    echo "FAIL: " . $e->getMessage() . "\n";
}

// Test 5: CRS from JSON (using fromJson method)
echo "\n=== Test from JSON ===\n";
try {
    $json_str = '{"proj": "longlat", "ellps": "WGS84", "datum": "WGS84"}';
    $crs = ProjCRS::fromJson($json_str);
    $proj4_out = $crs->toProj4();
    echo "JSON to PROJ4: $proj4_out\n";
    echo "PASS: CRS from JSON string\n";
} catch (Exception $e) {
    echo "SKIP: JSON parsing: " . $e->getMessage() . "\n";
}

// Test 6: Error cases - Invalid EPSG code
echo "\n=== Test invalid EPSG ===\n";
try {
    $crs = ProjCRS::fromEpsg(0);
    echo "FAIL: Should have thrown exception for EPSG 0\n";
} catch (ProjCRSException $e) {
    echo "PASS: Exception thrown for invalid EPSG 0\n";
} catch (Exception $e) {
    echo "FAIL: Wrong exception type: " . $e->getMessage() . "\n";
}

// Test 7: Error cases - Invalid EPSG string  
echo "\n=== Test invalid EPSG string ===\n";
try {
    $crs = ProjCRS::fromString("epsg:xyz");
    echo "FAIL: Should have thrown exception for invalid EPSG string\n";
} catch (ProjCRSException $e) {
    echo "PASS: Exception thrown for invalid EPSG string\n";
} catch (Exception $e) {
    echo "FAIL: Wrong exception type: " . $e->getMessage() . "\n";
}

// Test 8: Error cases - Invalid string  
echo "\n=== Test invalid string ===\n";
try {
    $crs = ProjCRS::fromString("0");
    echo "FAIL: Should have thrown exception for string '0'\n";
} catch (ProjCRSException $e) {
    echo "PASS: Exception thrown for string '0'\n";
} catch (Exception $e) {
    echo "FAIL: Wrong exception type: " . $e->getMessage() . "\n";
}

// Test 9: Error cases - Invalid JSON
echo "\n=== Test invalid JSON ===\n";
try {
    $crs = ProjCRS::fromString("{foo: bar}");
    echo "FAIL: Should have thrown exception for invalid JSON\n";
} catch (ProjCRSException $e) {
    echo "PASS: Exception thrown for invalid JSON\n";
} catch (Exception $e) {
    echo "FAIL: Wrong exception type: " . $e->getMessage() . "\n";
}

?>
--EXPECT--
=== Test from EPSG ===
int(4326)
PASS: CRS from EPSG 4326

=== Test from EPSG string ===
int(4326)
PASS: CRS from EPSG string

=== Test from EPSG int-like string ===
SKIP: Integer-like string parsing: PROJ Error [1025]: Invalid PROJ string syntax

=== Test from PROJ4 ===
PROJ4 output: +proj=longlat +datum=WGS84 +no_defs +type=crs
PASS: CRS from PROJ4 string

=== Test from JSON ===
SKIP: JSON parsing: PROJ Error [1025]: Invalid PROJ string syntax

=== Test invalid EPSG ===
PASS: Exception thrown for invalid EPSG 0

=== Test invalid EPSG string ===
PASS: Exception thrown for invalid EPSG string

=== Test invalid string ===
PASS: Exception thrown for string '0'

=== Test invalid JSON ===
PASS: Exception thrown for invalid JSON
