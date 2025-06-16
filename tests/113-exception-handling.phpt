--TEST--
Exception handling: Comprehensive error condition testing
--SKIPIF--
<?php if (!extension_loaded('proj')) die('skip PROJ extension not loaded'); ?>
--FILE--
<?php

// Test 1: Invalid projection parameters
echo "=== Test invalid projection parameters ===\n";
try {
    $invalid_crs = ProjCRS::fromString("+proj=bobbyjoe +type=crs");
    echo "Invalid projection accepted: unexpected\n";
} catch (ProjException $e) {
    echo "Invalid projection rejected: expected (ProjException)\n";
} catch (Exception $e) {
    echo "Invalid projection rejected: expected (Exception)\n";
}

// Test 2: Invalid CRS creation
echo "\n=== Test invalid CRS creation ===\n";
try {
    $invalid_crs = ProjCRS::fromString("+proj=invalid_projection");
    echo "Invalid CRS accepted: unexpected\n";
} catch (ProjCRSException $e) {
    echo "Invalid CRS rejected: expected (ProjCRSException)\n";
} catch (ProjException $e) {
    echo "Invalid CRS rejected: expected (ProjException)\n";
} catch (Exception $e) {
    echo "Invalid CRS rejected: expected (Exception)\n";
}

// Test 3: Invalid EPSG code
echo "\n=== Test invalid EPSG code ===\n";
try {
    $invalid_epsg = ProjCRS::fromEpsg(999999);
    echo "Invalid EPSG accepted: unexpected\n";
} catch (ProjCRSException $e) {
    echo "Invalid EPSG rejected: expected (ProjCRSException)\n";
} catch (ProjException $e) {
    echo "Invalid EPSG rejected: expected (ProjException)\n";
} catch (Exception $e) {
    echo "Invalid EPSG rejected: expected (Exception)\n";
}

// Test 4: Invalid transformer creation
echo "\n=== Test invalid transformer creation ===\n";
try {
    $invalid_transformer = ProjTransformer::fromCrs("invalid_crs", "EPSG:4326");
    echo "Invalid transformer accepted: unexpected\n";
} catch (ProjTransformerException $e) {
    echo "Invalid transformer rejected: expected (ProjTransformerException)\n";
} catch (ProjException $e) {
    echo "Invalid transformer rejected: expected (ProjException)\n";
} catch (Exception $e) {
    echo "Invalid transformer rejected: expected (Exception)\n";
}

// Test 5: Invalid geodetic parameters
echo "\n=== Test invalid geodetic parameters ===\n";
try {
    $invalid_geod = ProjGeod::fromEllipsoidName("invalid_ellipsoid");
    echo "Invalid geodetic accepted: unexpected\n";
} catch (ProjGeodException $e) {
    echo "Invalid geodetic rejected: expected (ProjGeodException)\n";
} catch (ProjException $e) {
    echo "Invalid geodetic rejected: expected (ProjException)\n";
} catch (Exception $e) {
    echo "Invalid geodetic rejected: expected (Exception)\n";
}

// Test 6: Invalid coordinate transformation
echo "\n=== Test invalid coordinate transformation ===\n";
try {
    $transformer = ProjTransformer::fromCrs("EPSG:4326", "EPSG:3857");
    // Try to transform obviously invalid coordinates
    $result = $transformer->transform(999, 999); // Outside valid lat/lon range
    echo "Invalid coordinates processed: " . (is_array($result) ? "success" : "failed") . "\n";
} catch (Exception $e) {
    echo "Invalid coordinates rejected: expected\n";
}

// Test 7: Exception hierarchy verification
echo "\n=== Test exception hierarchy ===\n";
try {
    ProjCRS::fromString("+proj=nonexistent +type=crs");
} catch (ProjException $e) {
    echo "ProjException is base class: true\n";
    echo "Exception message available: " . (!empty($e->getMessage()) ? "true" : "false") . "\n";
    echo "Exception code available: " . (is_numeric($e->getCode()) ? "true" : "false") . "\n";
} catch (Exception $e) {
    echo "Caught generic Exception instead of ProjException\n";
}

// Test 8: Invalid WKT string
echo "\n=== Test invalid WKT string ===\n";
try {
    $invalid_wkt = ProjCRS::fromWkt("INVALID WKT STRING");
    echo "Invalid WKT accepted: unexpected\n";
} catch (ProjCRSException $e) {
    echo "Invalid WKT rejected: expected (ProjCRSException)\n";
} catch (Exception $e) {
    echo "Invalid WKT rejected: expected (Exception)\n";
}

// Test 9: Invalid JSON string
echo "\n=== Test invalid JSON string ===\n";
try {
    $invalid_json = ProjCRS::fromJson("{invalid json}");
    echo "Invalid JSON accepted: unexpected\n";
} catch (ProjCRSException $e) {
    echo "Invalid JSON rejected: expected (ProjCRSException)\n";
} catch (Exception $e) {
    echo "Invalid JSON rejected: expected (Exception)\n";
}

// Test 10: Pipeline creation errors
echo "\n=== Test invalid pipeline ===\n";
try {
    $invalid_pipeline = ProjTransformer::fromPipeline("+step +proj=invalid_step +step +proj=another_invalid");
    echo "Invalid pipeline accepted: unexpected\n";
} catch (ProjTransformerException $e) {
    echo "Invalid pipeline rejected: expected (ProjTransformerException)\n";
} catch (ProjException $e) {
    echo "Invalid pipeline rejected: expected (ProjException)\n";
} catch (Exception $e) {
    echo "Invalid pipeline rejected: expected (Exception)\n";
}

// Test 11: Invalid area of interest
echo "\n=== Test invalid area of interest ===\n";
try {
    // Create invalid AOI (west > east, south > north)
    $invalid_aoi = new ProjAreaOfInterest(180, 90, -180, -90);
    echo "Invalid AOI accepted: " . ($invalid_aoi instanceof ProjAreaOfInterest ? "unexpected" : "failed") . "\n";
} catch (ValueError $e) {
    echo "Invalid AOI rejected: expected\n";
} catch (Exception $e) {
    echo "Invalid AOI rejected: expected\n";
}

// Test 12: Error message quality
echo "\n=== Test error message quality ===\n";
try {
    ProjCRS::fromString("+proj=unknown_projection +datum=WGS84 +type=crs");
} catch (ProjException $e) {
    $message = $e->getMessage();
    echo "Error message not empty: " . (!empty($message) ? "true" : "false") . "\n";
    echo "Error message contains context: " . (strlen($message) > 10 ? "true" : "false") . "\n";
} catch (Exception $e) {
    echo "Generic exception caught\n";
}

?>
--EXPECT--
=== Test invalid projection parameters ===
Invalid projection rejected: expected (ProjException)

=== Test invalid CRS creation ===
Invalid CRS rejected: expected (ProjCRSException)

=== Test invalid EPSG code ===
Invalid EPSG rejected: expected (ProjCRSException)

=== Test invalid transformer creation ===
Invalid transformer rejected: expected (ProjException)

=== Test invalid geodetic parameters ===
Invalid geodetic rejected: expected (ProjGeodException)

=== Test invalid coordinate transformation ===
Invalid coordinates processed: success

=== Test exception hierarchy ===
ProjException is base class: true
Exception message available: true
Exception code available: true

=== Test invalid WKT string ===
Invalid WKT rejected: expected (ProjCRSException)

=== Test invalid JSON string ===
Invalid JSON rejected: expected (ProjCRSException)

=== Test invalid pipeline ===
Invalid pipeline rejected: expected (ProjException)

=== Test invalid area of interest ===
Invalid AOI rejected: expected

=== Test error message quality ===
Error message not empty: true
Error message contains context: true