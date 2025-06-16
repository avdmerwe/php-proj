--TEST--
proj_version() - Get PROJ library version
--FILE--
<?php
// Test proj_version() function
$version = proj_version();

// Check that version is a string
var_dump(is_string($version));

// Check that version is not empty
var_dump(strlen($version) > 0);

// Version should contain dots (e.g., "6.3.1", "7.2.1", "8.0.0")
var_dump(strpos($version, '.') !== false);

// Print version for debugging (pattern will vary by system)
echo "PROJ library version: " . $version . "\n";

// Version should start with a digit (0-9)
var_dump($version[0] >= '0' && $version[0] <= '9');

// Test that calling multiple times returns the same value
$version2 = proj_version();
var_dump($version === $version2);

?>
--EXPECTF--
bool(true)
bool(true)
bool(true)
PROJ library version: %s
bool(true)
bool(true)