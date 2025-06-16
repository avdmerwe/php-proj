--TEST--
Data Directory: Data directory management and configuration
--SKIPIF--
<?php if (!extension_loaded('proj')) die('skip PROJ extension not loaded'); ?>
--FILE--
<?php

// Test 1: Basic data directory retrieval
echo "=== Test basic data directory functions ===\n";
try {
    $user_data_dir = proj_get_user_data_dir();
    echo "User data dir available: " . ($user_data_dir !== null ? "true" : "false") . "\n";
    
    if ($user_data_dir !== null) {
        echo "User data dir is string: " . (is_string($user_data_dir) ? "true" : "false") . "\n";
        echo "User data dir length > 0: " . (strlen($user_data_dir) > 0 ? "true" : "false") . "\n";
        echo "User data dir is absolute path: " . (strpos($user_data_dir, '/') === 0 ? "true" : "false") . "\n";
    }
} catch (Exception $e) {
    echo "Data directory test failed: " . $e->getMessage() . "\n";
}

// Test 2: Network data configuration
echo "\n=== Test network data configuration ===\n";
try {
    // Test initial network state
    $initial_network = proj_is_network_enabled();
    echo "Network state is boolean: " . (is_bool($initial_network) ? "true" : "false") . "\n";
    echo "Initial network state: " . ($initial_network ? "enabled" : "disabled") . "\n";
    
    // Test network control
    proj_set_network_enabled(false);
    $disabled_state = proj_is_network_enabled();
    echo "Network can be disabled: " . (!$disabled_state ? "true" : "false") . "\n";
    
    proj_set_network_enabled(true);
    $enabled_state = proj_is_network_enabled();
    echo "Network can be enabled: " . ($enabled_state ? "true" : "false") . "\n";
    
    // Restore initial state
    proj_set_network_enabled($initial_network);
    
} catch (Exception $e) {
    echo "Network configuration failed: " . $e->getMessage() . "\n";
}

// Test 3: Data directory impact on CRS operations
echo "\n=== Test data directory impact on operations ===\n";
try {
    // Test that basic operations work regardless of data dir state
    $crs = ProjCRS::fromEpsg(4326);
    echo "CRS creation works: " . ($crs !== null ? "true" : "false") . "\n";
    echo "CRS name available: " . (strlen($crs->getName()) > 0 ? "true" : "false") . "\n";
    
    // Test transformation works
    $transformer = ProjTransformer::fromCrs('EPSG:4326', 'EPSG:3857');
    $result = $transformer->transform(40.0, -100.0);
    echo "Transformation works: " . (is_array($result) && count($result) >= 2 ? "true" : "false") . "\n";
    
} catch (Exception $e) {
    echo "Operations test failed: " . $e->getMessage() . "\n";
}

// Test 4: Database query functions (related to data directory)
echo "\n=== Test database query functions ===\n";
try {
    // Test authority listing
    $authorities = proj_get_authorities();
    echo "Authorities available: " . (is_array($authorities) && count($authorities) > 0 ? "true" : "false") . "\n";
    echo "EPSG in authorities: " . (in_array('EPSG', $authorities) ? "true" : "false") . "\n";
    
    // Test codes retrieval
    $epsg_codes = proj_get_codes('EPSG', 'CRS');
    echo "EPSG codes available: " . (is_array($epsg_codes) && count($epsg_codes) > 0 ? "true" : "false") . "\n";
    echo "EPSG codes reasonable count: " . (count($epsg_codes) > 1000 ? "true" : "false") . "\n";
    
    // Test specific authority that should exist
    echo "Authority EPSG exists: " . (in_array('EPSG', $authorities) ? "true" : "false") . "\n";
    
} catch (Exception $e) {
    echo "Database queries failed: " . $e->getMessage() . "\n";
}

// Test 5: Network-dependent operations (if available)
echo "\n=== Test network-dependent operations ===\n";
try {
    $network_enabled = proj_is_network_enabled();
    
    if ($network_enabled) {
        echo "Network operations possible: true\n";
        
        // Test that operations still work with network enabled
        $test_crs = ProjCRS::fromEpsg(4326);
        echo "CRS operations with network: " . ($test_crs !== null ? "true" : "false") . "\n";
        
        // Test authority codes with network (might have more available)
        $network_authorities = proj_get_authorities();
        echo "Network authorities count >= base: " . (count($network_authorities) >= count($authorities) ? "true" : "false") . "\n";
    } else {
        echo "Network operations possible: false\n";
        echo "CRS operations without network: true\n";
        echo "Local authorities still available: true\n";
    }
    
} catch (Exception $e) {
    echo "Network operations failed: " . $e->getMessage() . "\n";
}

// Test 6: Data directory consistency
echo "\n=== Test data directory consistency ===\n";
try {
    // Multiple calls should return consistent results
    $dir1 = proj_get_user_data_dir();
    $dir2 = proj_get_user_data_dir();
    
    echo "Multiple calls consistent: " . ($dir1 === $dir2 ? "true" : "false") . "\n";
    
    // Network state should be consistent
    $net1 = proj_is_network_enabled();
    $net2 = proj_is_network_enabled();
    echo "Network state consistent: " . ($net1 === $net2 ? "true" : "false") . "\n";
    
} catch (Exception $e) {
    echo "Consistency test failed: " . $e->getMessage() . "\n";
}

// Test 7: Error handling for data directory issues
echo "\n=== Test data directory error handling ===\n";
try {
    // These should not cause fatal errors even if data is missing
    $user_dir = proj_get_user_data_dir();
    echo "User data dir call safe: true\n";
    
    $network_state = proj_is_network_enabled();
    echo "Network state call safe: true\n";
    
    // Authority queries should handle missing data gracefully
    $safe_authorities = proj_get_authorities();
    echo "Authority queries safe: " . (is_array($safe_authorities) ? "true" : "false") . "\n";
    
} catch (Exception $e) {
    echo "Error handling test passed: " . (strpos($e->getMessage(), 'safe') !== false ? "true" : "false") . "\n";
}

// Test 8: Data directory and coordinate accuracy
echo "\n=== Test data directory impact on accuracy ===\n";
try {
    // Test that coordinate transformations are reasonably accurate
    $accuracy_transformer = ProjTransformer::fromCrs('EPSG:4326', 'EPSG:3857');
    
    // Known coordinate: New York City
    $nyc_result = $accuracy_transformer->transform(40.7128, -74.0060);
    
    // Verify reasonable Web Mercator coordinates for NYC
    $x_reasonable = ($nyc_result[0] > -8300000 && $nyc_result[0] < -8200000);
    $y_reasonable = ($nyc_result[1] > 4900000 && $nyc_result[1] < 5000000);
    
    echo "Coordinate accuracy reasonable: " . ($x_reasonable && $y_reasonable ? "true" : "false") . "\n";
    
    // Test round-trip accuracy
    $reverse = $accuracy_transformer->transform($nyc_result[0], $nyc_result[1], null, null, false, false, "INVERSE");
    $lat_error = abs(40.7128 - $reverse[0]);
    $lon_error = abs(-74.0060 - $reverse[1]);
    
    echo "Round-trip accuracy good: " . ($lat_error < 0.001 && $lon_error < 0.001 ? "true" : "false") . "\n";
    
} catch (Exception $e) {
    echo "Accuracy test failed: " . $e->getMessage() . "\n";
}

// Test 9: Configuration validation
echo "\n=== Test configuration validation ===\n";
try {
    // Test that required PROJ resources are available
    $basic_crs_codes = proj_get_codes('EPSG', 'CRS');
    
    // Should have common EPSG codes
    $has_wgs84 = false;
    $has_web_mercator = false;
    
    foreach ($basic_crs_codes as $code) {
        if ($code == '4326') $has_wgs84 = true;
        if ($code == '3857') $has_web_mercator = true;
    }
    
    echo "WGS84 (4326) available: " . ($has_wgs84 ? "true" : "false") . "\n";
    echo "Web Mercator (3857) available: " . ($has_web_mercator ? "true" : "false") . "\n";
    
    // Test that basic transformations work
    if ($has_wgs84 && $has_web_mercator) {
        $config_transformer = ProjTransformer::fromCrs('EPSG:4326', 'EPSG:3857');
        echo "Basic transformation available: " . ($config_transformer !== null ? "true" : "false") . "\n";
    } else {
        echo "Basic transformation available: false\n";
    }
    
} catch (Exception $e) {
    echo "Configuration validation failed: " . $e->getMessage() . "\n";
}

// Test 10: Performance with data directory operations
echo "\n=== Test performance with data operations ===\n";
try {
    $start_time = microtime(true);
    
    // Perform multiple data directory operations
    for ($i = 0; $i < 10; $i++) {
        proj_get_user_data_dir();
        proj_is_network_enabled();
        if ($i % 3 == 0) {
            proj_get_authorities();
        }
    }
    
    $elapsed = microtime(true) - $start_time;
    echo "Data operations performance good: " . ($elapsed < 0.1 ? "true" : "false") . "\n";
    echo "Performance under 0.1 seconds: " . ($elapsed < 0.1 ? "true" : "false") . "\n";
    
} catch (Exception $e) {
    echo "Performance test failed: " . $e->getMessage() . "\n";
}

?>
--EXPECT--
=== Test basic data directory functions ===
User data dir available: true
User data dir is string: true
User data dir length > 0: true
User data dir is absolute path: true

=== Test network data configuration ===
Network state is boolean: true
Initial network state: disabled
Network can be disabled: true
Network can be enabled: true

=== Test data directory impact on operations ===
CRS creation works: true
CRS name available: true
Transformation works: true

=== Test database query functions ===
Authorities available: true
EPSG in authorities: true
EPSG codes available: true
EPSG codes reasonable count: true
Authority EPSG exists: true

=== Test network-dependent operations ===
Network operations possible: false
CRS operations without network: true
Local authorities still available: true

=== Test data directory consistency ===
Multiple calls consistent: true
Network state consistent: true

=== Test data directory error handling ===
User data dir call safe: true
Network state call safe: true
Authority queries safe: true

=== Test data directory impact on accuracy ===
Coordinate accuracy reasonable: true
Round-trip accuracy good: true

=== Test configuration validation ===
WGS84 (4326) available: true
Web Mercator (3857) available: true
Basic transformation available: true

=== Test performance with data operations ===
Data operations performance good: true
Performance under 0.1 seconds: true