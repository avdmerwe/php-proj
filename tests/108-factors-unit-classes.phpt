--TEST--
ProjFactors and ProjUnit: Utility classes functionality
--SKIPIF--
<?php if (!extension_loaded('proj')) die('skip PROJ extension not loaded'); ?>
--FILE--
<?php

// Test 1: ProjFactors functionality via ProjCRS::getFactors()
echo "=== Test ProjFactors via ProjCRS::getFactors() ===\n";
try {
    // Create a projection and get factors
    $crs = ProjCRS::fromString('+proj=merc +ellps=WGS84 +type=crs');
    $factors = $crs->getFactors(0.0, 0.0); // Equator, Greenwich
    
    echo "Factors object created: " . ($factors instanceof ProjFactors ? "true" : "false") . "\n";
    echo "Has meridional_scale property: " . (isset($factors->meridional_scale) ? "true" : "false") . "\n";
    echo "Has parallel_scale property: " . (isset($factors->parallel_scale) ? "true" : "false") . "\n";
    
    // Test getter methods
    echo "getMeridionalScale() works: " . (method_exists($factors, 'getMeridionalScale') ? "true" : "false") . "\n";
    echo "getParallelScale() works: " . (method_exists($factors, 'getParallelScale') ? "true" : "false") . "\n";
    echo "getArealScale() works: " . (method_exists($factors, 'getArealScale') ? "true" : "false") . "\n";
    
    // Test toArray method
    if (method_exists($factors, 'toArray')) {
        $array = $factors->toArray();
        echo "toArray() works: " . (is_array($array) && count($array) > 0 ? "true" : "false") . "\n";
        echo "Array has meridional_scale: " . (isset($array['meridional_scale']) ? "true" : "false") . "\n";
    } else {
        echo "toArray() works: false\n";
    }
    
    // Test string representation
    if (method_exists($factors, '__toString')) {
        $str = (string)$factors;
        echo "toString() works: " . (strlen($str) > 0 && strpos($str, 'ProjFactors') !== false ? "true" : "false") . "\n";
    } else {
        echo "toString() works: false\n";
    }
    
} catch (Exception $e) {
    echo "ProjFactors test failed: " . $e->getMessage() . "\n";
}

// Test 2: ProjUnit class creation and methods
echo "\n=== Test ProjUnit class ===\n";
try {
    // Test constructor with various parameters
    $unit = new ProjUnit("EPSG", "9001", "metre", "linear", 1.0, "m", false);
    
    echo "Unit object created: " . ($unit instanceof ProjUnit ? "true" : "false") . "\n";
    echo "Has auth_name property: " . (isset($unit->auth_name) ? "true" : "false") . "\n";
    echo "Has code property: " . (isset($unit->code) ? "true" : "false") . "\n";
    echo "Has name property: " . (isset($unit->name) ? "true" : "false") . "\n";
    
    // Test getter methods
    echo "getAuthName(): " . $unit->getAuthName() . "\n";
    echo "getCode(): " . $unit->getCode() . "\n";
    echo "getName(): " . $unit->getName() . "\n";
    echo "getCategory(): " . $unit->getCategory() . "\n";
    echo "getConvFactor(): " . $unit->getConvFactor() . "\n";
    echo "getProjShortName(): " . $unit->getProjShortName() . "\n";
    echo "isDeprecated(): " . ($unit->isDeprecated() ? "true" : "false") . "\n";
    
} catch (Exception $e) {
    echo "ProjUnit creation failed: " . $e->getMessage() . "\n";
}

// Test 3: ProjUnit with null values
echo "\n=== Test ProjUnit with null values ===\n";
try {
    $unit_null = new ProjUnit(null, null, "unknown", "linear", 1.0, null, true);
    
    echo "Unit with nulls created: " . ($unit_null instanceof ProjUnit ? "true" : "false") . "\n";
    echo "getAuthName() with null: " . ($unit_null->getAuthName() === null ? "null" : $unit_null->getAuthName()) . "\n";
    echo "getCode() with null: " . ($unit_null->getCode() === null ? "null" : $unit_null->getCode()) . "\n";
    echo "getName() with null: " . $unit_null->getName() . "\n";
    echo "isDeprecated(): " . ($unit_null->isDeprecated() ? "true" : "false") . "\n";
    
} catch (Exception $e) {
    echo "ProjUnit null test failed: " . $e->getMessage() . "\n";
}

// Test 4: ProjUnit array and string representation
echo "\n=== Test ProjUnit array and string methods ===\n";
try {
    $unit = new ProjUnit("EPSG", "9002", "foot", "linear", 0.3048, "ft", false);
    
    // Test toArray method
    if (method_exists($unit, 'toArray')) {
        $array = $unit->toArray();
        echo "toArray() works: " . (is_array($array) ? "true" : "false") . "\n";
        echo "Array has auth_name: " . (isset($array['auth_name']) && $array['auth_name'] === "EPSG" ? "true" : "false") . "\n";
        echo "Array has conv_factor: " . (isset($array['conv_factor']) && abs($array['conv_factor'] - 0.3048) < 0.0001 ? "true" : "false") . "\n";
    } else {
        echo "toArray() works: false\n";
    }
    
    // Test string representation
    if (method_exists($unit, '__toString')) {
        $str = (string)$unit;
        echo "toString() works: " . (strlen($str) > 0 && strpos($str, 'ProjUnit') !== false ? "true" : "false") . "\n";
        echo "String contains code: " . (strpos($str, '9002') !== false ? "true" : "false") . "\n";
    } else {
        echo "toString() works: false\n";
    }
    
} catch (Exception $e) {
    echo "ProjUnit array/string test failed: " . $e->getMessage() . "\n";
}

// Test 5: Verify factors contain reasonable values
echo "\n=== Test factor values are reasonable ===\n";
try {
    $crs = ProjCRS::fromString('+proj=merc +ellps=WGS84 +type=crs');
    $factors = $crs->getFactors(0.0, 45.0); // 45 degrees north
    
    $meridional = $factors->getMeridionalScale();
    $parallel = $factors->getParallelScale();
    $areal = $factors->getArealScale();
    
    echo "Meridional scale reasonable: " . ($meridional > 0.5 && $meridional < 5.0 ? "true" : "false") . "\n";
    echo "Parallel scale reasonable: " . ($parallel > 0.5 && $parallel < 5.0 ? "true" : "false") . "\n";
    echo "Areal scale reasonable: " . ($areal > 0.5 && $areal < 10.0 ? "true" : "false") . "\n";
    
    // Test other factor methods
    echo "Angular distortion available: " . (!is_nan($factors->getAngularDistortion()) ? "true" : "false") . "\n";
    echo "Meridian convergence available: " . (!is_nan($factors->getMeridianConvergence()) ? "true" : "false") . "\n";
    
} catch (Exception $e) {
    echo "Factor values test failed: " . $e->getMessage() . "\n";
}

?>
--EXPECT--
=== Test ProjFactors via ProjCRS::getFactors() ===
Factors object created: true
Has meridional_scale property: true
Has parallel_scale property: true
getMeridionalScale() works: true
getParallelScale() works: true
getArealScale() works: true
toArray() works: true
Array has meridional_scale: true
toString() works: true

=== Test ProjUnit class ===
Unit object created: true
Has auth_name property: true
Has code property: true
Has name property: true
getAuthName(): EPSG
getCode(): 9001
getName(): metre
getCategory(): linear
getConvFactor(): 1
getProjShortName(): m
isDeprecated(): false

=== Test ProjUnit with null values ===
Unit with nulls created: true
getAuthName() with null: null
getCode() with null: null
getName() with null: unknown
isDeprecated(): true

=== Test ProjUnit array and string methods ===
toArray() works: true
Array has auth_name: true
Array has conv_factor: true
toString() works: true
String contains code: true

=== Test factor values are reasonable ===
Meridional scale reasonable: true
Parallel scale reasonable: true
Areal scale reasonable: true
Angular distortion available: true
Meridian convergence available: true