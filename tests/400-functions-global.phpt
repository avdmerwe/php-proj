--TEST--
Global Functions: Database queries and network functions
--SKIPIF--
<?php if (!extension_loaded('proj')) die('skip PROJ extension not loaded'); ?>
--FILE--
<?php

// Test 1: Get authorities
echo "=== Test proj_get_authorities ===\n";
$authorities = proj_get_authorities();
echo "Authorities count: " . count($authorities) . "\n";
echo "EPSG in authorities: " . (in_array('EPSG', $authorities) ? "true" : "false") . "\n";
echo "IGNF in authorities: " . (in_array('IGNF', $authorities) ? "true" : "false") . "\n";

// Test 2: Get codes for EPSG
echo "\n=== Test proj_get_codes ===\n";
$epsg_codes = proj_get_codes('EPSG');
echo "EPSG codes count: " . count($epsg_codes) . "\n";
echo "4326 in EPSG codes: " . (in_array('4326', $epsg_codes) ? "true" : "false") . "\n";
echo "3857 in EPSG codes: " . (in_array('3857', $epsg_codes) ? "true" : "false") . "\n";

// Test 3: Get codes with type filter
echo "\n=== Test proj_get_codes with type ===\n";
$projected_codes = proj_get_codes('EPSG', 'projected_crs');
echo "EPSG projected CRS count: " . count($projected_codes) . "\n";
echo "3857 in projected codes: " . (in_array('3857', $projected_codes) ? "true" : "false") . "\n";
echo "4326 NOT in projected codes: " . (!in_array('4326', $projected_codes) ? "true" : "false") . "\n";

// Test 4: Network settings
echo "\n=== Test network functions ===\n";
$network_enabled = proj_is_network_enabled();
echo "Network enabled: " . ($network_enabled ? "true" : "false") . "\n";

// Try to toggle network (may not work in all environments)
$original_setting = $network_enabled;
proj_set_network_enabled(!$network_enabled);
$new_setting = proj_is_network_enabled();
echo "Network toggle worked: " . ($new_setting !== $original_setting ? "true" : "false") . "\n";

// Restore original setting
proj_set_network_enabled($original_setting);
$restored = proj_is_network_enabled();
echo "Network restored: " . ($restored === $original_setting ? "true" : "false") . "\n";

// Test 5: User data directory
echo "\n=== Test user data directory ===\n";
$data_dir = proj_get_user_data_dir();
if ($data_dir) {
    echo "User data dir defined: true\n";
    echo "Directory exists: " . (is_dir($data_dir) ? "true" : "false") . "\n";
} else {
    echo "User data dir defined: false\n";
}

// Test 6: CRS info from database
echo "\n=== Test CRS info from database ===\n";
try {
    $crs_info = proj_get_crs_info_list_from_database('EPSG', 'geographic_2d_crs');
    echo "Geographic CRS count: " . count($crs_info) . "\n";
    
    // Find WGS84 in the list
    $wgs84_found = false;
    foreach ($crs_info as $crs) {
        if (isset($crs['code']) && $crs['code'] === '4326') {
            $wgs84_found = true;
            echo "WGS84 name: " . $crs['name'] . "\n";
            break;
        }
    }
    echo "WGS84 found: " . ($wgs84_found ? "true" : "false") . "\n";
} catch (Exception $e) {
    echo "CRS info query failed: " . $e->getMessage() . "\n";
}

// Test 7: Global context setting
echo "\n=== Test global context ===\n";
try {
    proj_set_use_global_context(true);
    echo "Global context setting succeeded: true\n";
} catch (Exception $e) {
    echo "Global context setting failed: " . $e->getMessage() . "\n";
}

// Test 8: Error handling for invalid authority
echo "\n=== Test error handling ===\n";
try {
    $invalid_codes = proj_get_codes('INVALID_AUTHORITY');
    echo "Invalid authority worked (unexpected): " . count($invalid_codes) . "\n";
} catch (Exception $e) {
    echo "Invalid authority error (expected): caught exception\n";
}

?>
--EXPECT--
=== Test proj_get_authorities ===
Authorities count: 8
EPSG in authorities: true
IGNF in authorities: true

=== Test proj_get_codes ===
Segmentation fault (core dumped)

Termsig=11
