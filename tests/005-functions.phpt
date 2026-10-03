--TEST--
Functions: Global utility functions
--SKIPIF--
<?php if (!extension_loaded("proj")) print "skip"; ?>
--FILE--
<?php
// Test proj_get_authorities function
echo "Testing get_authorities function\n";
try {
    $authorities = proj_get_authorities();
    echo "Found " . (count($authorities) > 0 ? "> 0" : "0") . " authorities\n";
    echo "Contains EPSG: " . (in_array("EPSG", $authorities) ? "Yes" : "No") . "\n";
    echo "Contains ESRI: " . (in_array("ESRI", $authorities) ? "Yes" : "No") . "\n";
} catch (Exception $e) {
    echo "Error: " . $e->getMessage() . "\n";
}

// Test get_codes function
echo "\nTesting get_codes function\n";
try {
    $codes = proj_get_codes("EPSG", "projected_crs");
    echo "Found " . (count($codes) > 0 ? "> 0" : "0") . " EPSG projected CRS codes\n";
    echo "Contains 3857: " . (in_array("3857", $codes) ? "Yes" : "No") . "\n";
    echo "Contains 32633: " . (in_array("32633", $codes) ? "Yes" : "No") . "\n";
} catch (Exception $e) {
    echo "Error: " . $e->getMessage() . "\n";
}

// Test get_codes with geographic CRS
echo "\nTesting get_codes with geographic CRS\n";
try {
    $codes = proj_get_codes("EPSG", "geographic_crs");
    echo "Found " . (count($codes) > 0 ? "> 0" : "0") . " EPSG geographic CRS codes\n";
    echo "Contains 4326: " . (in_array("4326", $codes) ? "Yes" : "No") . "\n";
    echo "Contains 4269: " . (in_array("4269", $codes) ? "Yes" : "No") . "\n";
} catch (Exception $e) {
    echo "Error: " . $e->getMessage() . "\n";
}

// Test network functions
echo "\nTesting network functions\n";
try {
    $initial_state = proj_is_network_enabled();
    echo "Initial network state: " . ($initial_state ? "Enabled" : "Disabled") . "\n";
    
    $result = proj_set_network_enabled(true);
    echo "Set network enabled: " . ($result ? "Success" : "Failed") . "\n";
    
    $enabled_state = proj_is_network_enabled();
    echo "Network state after enable: " . ($enabled_state ? "Enabled" : "Disabled") . "\n";
    
    $result = proj_set_network_enabled(false);
    echo "Set network disabled: " . ($result ? "Success" : "Failed") . "\n";
    
    $disabled_state = proj_is_network_enabled();
    echo "Network state after disable: " . ($disabled_state ? "Enabled" : "Disabled") . "\n";
    
    // Restore initial state
    proj_set_network_enabled($initial_state);
    
} catch (Exception $e) {
    echo "Error: " . $e->getMessage() . "\n";
}

// Test user data directory functions
echo "\nTesting user data directory functions\n";
try {
    $data_dir = proj_get_user_data_dir();
    echo "User data directory: " . (empty($data_dir) ? "Not set" : "Set") . "\n";
    
    $data_dir_create = proj_get_user_data_dir(true);
    echo "User data directory (create): " . (empty($data_dir_create) ? "Not created" : "Available") . "\n";
    
} catch (Exception $e) {
    echo "Error: " . $e->getMessage() . "\n";
}

// Test CRS info from database
echo "\nTesting CRS info from database\n";
try {
    $crs_info = proj_get_crs_info_list_from_database("EPSG", "projected_crs");
    echo "Found " . (count($crs_info) > 0 ? "> 0" : "0") . " CRS info entries\n";
    
    if (count($crs_info) > 0) {
        $first = $crs_info[0];
        echo "First entry authority: " . $first['auth_name'] . "\n";
        echo "First entry code: " . $first['code'] . "\n";
        echo "First entry name: " . (!empty($first['name']) ? "Has name" : "No name") . "\n";
        echo "First entry deprecated: " . ($first['deprecated'] ? "Yes" : "No") . "\n";
    }
} catch (Exception $e) {
    echo "Error: " . $e->getMessage() . "\n";
}

// Test error handling with invalid authority
echo "\nTesting error handling with invalid authority\n";
try {
    $codes = proj_get_codes("INVALID_AUTH", "projected_crs");
    echo "Invalid authority returned " . count($codes) . " codes\n";
} catch (Exception $e) {
    echo "Error caught for invalid authority\n";
}

// Test error handling with invalid type
echo "\nTesting error handling with invalid type\n";
try {
    $codes = proj_get_codes("EPSG", "invalid_type");
    echo "Invalid type returned " . count($codes) . " codes\n";
} catch (Exception $e) {
    echo "Error caught for invalid type\n";
}

echo "\nAll tests completed\n";
?>
--EXPECT--
Testing get_authorities function
Found > 0 authorities
Contains EPSG: Yes
Contains ESRI: Yes

Testing get_codes function
Found > 0 EPSG projected CRS codes
Contains 3857: Yes
Contains 32633: Yes

Testing get_codes with geographic CRS
Found > 0 EPSG geographic CRS codes
Contains 4326: Yes
Contains 4269: Yes

Testing network functions
Initial network state: Disabled
Set network enabled: Success
Network state after enable: Enabled
Set network disabled: Success
Network state after disable: Disabled

Testing user data directory functions
User data directory: Set
User data directory (create): Available

Testing CRS info from database
Found > 0 CRS info entries
First entry authority: EPSG
First entry code: 2000
First entry name: Has name
First entry deprecated: No

Testing error handling with invalid authority
Invalid authority returned 0 codes

Testing error handling with invalid type
Invalid type returned 6964 codes

All tests completed
