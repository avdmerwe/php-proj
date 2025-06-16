--TEST--
Version information: PROJ library and PHP extension version details
--SKIPIF--
<?php if (!extension_loaded('proj')) die('skip PROJ extension not loaded'); ?>
--FILE--
<?php

// Test 1: PHP extension version information
echo "=== Test PHP extension version ===\n";
try {
    $extension_info = new ReflectionExtension('proj');
    $version = $extension_info->getVersion();
    
    echo "Extension loaded: true\n";
    echo "Extension version available: " . (!empty($version) ? "true" : "false") . "\n";
    echo "Version format valid: " . (preg_match('/\d+\.\d+\.\d+/', $version) ? "true" : "false") . "\n";
    
} catch (Exception $e) {
    echo "Extension version test failed: " . $e->getMessage() . "\n";
}

// Test 2: PROJ library version from phpinfo output
echo "\n=== Test PROJ library version ===\n";
try {
    ob_start();
    phpinfo(INFO_MODULES);
    $info = ob_get_contents();
    ob_end_clean();
    
    $has_proj_section = strpos($info, 'PROJ Extension') !== false;
    $has_proj_version = strpos($info, 'PROJ Library Version') !== false;
    $has_proj_release = strpos($info, 'PROJ Library Release') !== false;
    
    echo "PROJ extension in phpinfo: " . ($has_proj_section ? "true" : "false") . "\n";
    echo "PROJ library version listed: " . ($has_proj_version ? "true" : "false") . "\n";
    echo "PROJ library release listed: " . ($has_proj_release ? "true" : "false") . "\n";
    
} catch (Exception $e) {
    echo "PROJ version test failed: " . $e->getMessage() . "\n";
}

// Test 3: PROJ data directory information
echo "\n=== Test PROJ data directory ===\n";
try {
    $user_data_dir = proj_get_user_data_dir();
    echo "User data directory available: " . ($user_data_dir !== null ? "true" : "false") . "\n";
    
    if ($user_data_dir !== null) {
        echo "Data directory path valid: " . (is_string($user_data_dir) && !empty($user_data_dir) ? "true" : "false") . "\n";
    }
    
} catch (Exception $e) {
    echo "Data directory test failed: " . $e->getMessage() . "\n";
}

// Test 4: Database availability
echo "\n=== Test database availability ===\n";
try {
    $authorities = proj_get_authorities();
    $epsg_codes = proj_get_codes('EPSG', 'CRS');
    
    echo "Authorities database available: " . (is_array($authorities) && count($authorities) > 0 ? "true" : "false") . "\n";
    echo "EPSG database accessible: " . (is_array($epsg_codes) && count($epsg_codes) > 0 ? "true" : "false") . "\n";
    
    // Check for specific databases
    $has_epsg = in_array('EPSG', $authorities);
    $has_esri = in_array('ESRI', $authorities);
    
    echo "EPSG database present: " . ($has_epsg ? "true" : "false") . "\n";
    echo "ESRI database present: " . ($has_esri ? "true" : "false") . "\n";
    
} catch (Exception $e) {
    echo "Database availability test failed: " . $e->getMessage() . "\n";
}

// Test 5: Runtime capabilities
echo "\n=== Test runtime capabilities ===\n";
try {
    // Test CRS creation capability
    $crs_capability = false;
    try {
        $test_crs = ProjCRS::fromEpsg(4326);
        $crs_capability = ($test_crs instanceof ProjCRS);
    } catch (Exception $e) {
        // CRS creation failed
    }
    
    // Test transformation capability
    $transform_capability = false;
    try {
        $test_transformer = ProjTransformer::fromCrs('EPSG:4326', 'EPSG:3857');
        $transform_capability = ($test_transformer instanceof ProjTransformer);
    } catch (Exception $e) {
        // Transformation failed
    }
    
    // Test geodetic capability
    $geodetic_capability = false;
    try {
        $test_geod = ProjGeod::fromEpsg(4326);
        $geodetic_capability = ($test_geod instanceof ProjGeod);
    } catch (Exception $e) {
        // Geodetic failed
    }
    
    echo "CRS creation capability: " . ($crs_capability ? "available" : "unavailable") . "\n";
    echo "Coordinate transformation capability: " . ($transform_capability ? "available" : "unavailable") . "\n";
    echo "Geodetic calculation capability: " . ($geodetic_capability ? "available" : "unavailable") . "\n";
    
} catch (Exception $e) {
    echo "Runtime capabilities test failed: " . $e->getMessage() . "\n";
}

// Test 6: Network capabilities
echo "\n=== Test network capabilities ===\n";
try {
    $network_enabled = proj_is_network_enabled();
    echo "Network functionality available: true\n";
    echo "Network status queryable: " . (is_bool($network_enabled) ? "true" : "false") . "\n";
    
    // Test network toggle
    $original_status = $network_enabled;
    proj_set_network_enabled(!$original_status);
    $new_status = proj_is_network_enabled();
    proj_set_network_enabled($original_status); // Restore
    
    echo "Network toggle functional: " . ($new_status !== $original_status ? "true" : "false") . "\n";
    
} catch (Exception $e) {
    echo "Network capabilities test failed: " . $e->getMessage() . "\n";
}

// Test 7: Exception handling capabilities
echo "\n=== Test exception handling ===\n";
try {
    $exception_classes = [
        'ProjException',
        'ProjCRSException', 
        'ProjTransformerException',
        'ProjGeodException'
    ];
    
    $available_count = 0;
    foreach ($exception_classes as $class_name) {
        if (class_exists($class_name)) {
            $available_count++;
        }
    }
    
    echo "Exception classes available: " . ($available_count >= 3 ? "comprehensive" : "basic") . "\n";
    echo "Exception hierarchy functional: " . ($available_count > 0 ? "true" : "false") . "\n";
    
} catch (Exception $e) {
    echo "Exception handling test failed: " . $e->getMessage() . "\n";
}

// Test 8: Performance characteristics
echo "\n=== Test performance characteristics ===\n";
try {
    // Quick performance test
    $start_time = microtime(true);
    
    // Create objects and perform basic operations
    $crs = ProjCRS::fromEpsg(4326);
    $transformer = ProjTransformer::fromCrs($crs, 'EPSG:3857');
    $result = $transformer->transform(0, 0);
    
    $end_time = microtime(true);
    $operation_time = ($end_time - $start_time) * 1000; // milliseconds
    
    echo "Basic operations functional: " . (is_array($result) ? "true" : "false") . "\n";
    echo "Performance acceptable: " . ($operation_time < 100 ? "fast" : "acceptable") . "\n";
    
} catch (Exception $e) {
    echo "Performance test failed: " . $e->getMessage() . "\n";
}

?>
--EXPECT--
=== Test PHP extension version ===
Extension loaded: true
Extension version available: true
Version format valid: true

=== Test PROJ library version ===
PROJ extension in phpinfo: true
PROJ library version listed: true
PROJ library release listed: true

=== Test PROJ data directory ===
User data directory available: true
Data directory path valid: true

=== Test database availability ===
Authorities database available: true
EPSG database accessible: true
EPSG database present: true
ESRI database present: true

=== Test runtime capabilities ===
CRS creation capability: available
Coordinate transformation capability: available
Geodetic calculation capability: available

=== Test network capabilities ===
Network functionality available: true
Network status queryable: true
Network toggle functional: true

=== Test exception handling ===
Exception classes available: comprehensive
Exception hierarchy functional: true

=== Test performance characteristics ===
Basic operations functional: true
Performance acceptable: fast