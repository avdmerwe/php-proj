--TEST--
Network functionality: Enable/disable network operations
--SKIPIF--
<?php if (!extension_loaded('proj')) die('skip PROJ extension not loaded'); ?>
--FILE--
<?php

// Test 1: Basic network status functions
echo "=== Test basic network status ===\n";
try {
    // Check initial network status
    $initial_status = proj_is_network_enabled();
    echo "Network initially enabled: " . ($initial_status ? "true" : "false") . "\n";
    
    // Test that function returns boolean
    echo "proj_is_network_enabled returns bool: " . (is_bool($initial_status) ? "true" : "false") . "\n";
    
} catch (Exception $e) {
    echo "Network status test failed: " . $e->getMessage() . "\n";
}

// Test 2: Enable network
echo "\n=== Test enable network ===\n";
try {
    // Enable network
    proj_set_network_enabled(true);
    $enabled_status = proj_is_network_enabled();
    echo "Network enabled successfully: " . ($enabled_status ? "true" : "false") . "\n";
    
} catch (Exception $e) {
    echo "Enable network test failed: " . $e->getMessage() . "\n";
}

// Test 3: Disable network
echo "\n=== Test disable network ===\n";
try {
    // Disable network
    proj_set_network_enabled(false);
    $disabled_status = proj_is_network_enabled();
    echo "Network disabled successfully: " . ($disabled_status ? "false" : "true") . "\n";
    
} catch (Exception $e) {
    echo "Disable network test failed: " . $e->getMessage() . "\n";
}

// Test 4: Toggle network multiple times
echo "\n=== Test network toggle ===\n";
try {
    // Enable, check, disable, check
    proj_set_network_enabled(true);
    $status1 = proj_is_network_enabled();
    
    proj_set_network_enabled(false);
    $status2 = proj_is_network_enabled();
    
    proj_set_network_enabled(true);
    $status3 = proj_is_network_enabled();
    
    echo "Toggle pattern (true->false->true): " . 
         ($status1 ? "true" : "false") . " -> " .
         ($status2 ? "true" : "false") . " -> " .
         ($status3 ? "true" : "false") . "\n";
    
    // Verify expected pattern
    echo "Toggle pattern correct: " . 
         (($status1 && !$status2 && $status3) ? "true" : "false") . "\n";
    
} catch (Exception $e) {
    echo "Network toggle test failed: " . $e->getMessage() . "\n";
}

// Test 5: Network functionality with CRS operations
echo "\n=== Test network impact on CRS operations ===\n";
try {
    // Disable network first
    proj_set_network_enabled(false);
    
    // Try basic CRS operations (should work without network)
    $crs = ProjCRS::fromEpsg(4326);
    echo "CRS creation works without network: " . ($crs instanceof ProjCRS ? "true" : "false") . "\n";
    
    $transformer = ProjTransformer::fromCrs($crs, 'EPSG:3857');
    echo "Transformer creation works without network: " . ($transformer instanceof ProjTransformer ? "true" : "false") . "\n";
    
    // Simple transformation should work
    $result = $transformer->transform(-74.0, 40.7);
    echo "Transformation works without network: " . (is_array($result) && count($result) >= 2 ? "true" : "false") . "\n";
    
    // Re-enable network
    proj_set_network_enabled(true);
    echo "Network re-enabled: " . (proj_is_network_enabled() ? "true" : "false") . "\n";
    
} catch (Exception $e) {
    echo "Network CRS operations test failed: " . $e->getMessage() . "\n";
}

// Test 6: Check data directory still works
echo "\n=== Test data directory access ===\n";
try {
    $user_data_dir = proj_get_user_data_dir();
    echo "User data dir accessible: " . ($user_data_dir !== null ? "true" : "false") . "\n";
    
    // Test authorities still work
    $authorities = proj_get_authorities();
    echo "Authorities available: " . (is_array($authorities) && count($authorities) > 0 ? "true" : "false") . "\n";
    
} catch (Exception $e) {
    echo "Data directory test failed: " . $e->getMessage() . "\n";
}

?>
--EXPECT--
=== Test basic network status ===
Network initially enabled: false
proj_is_network_enabled returns bool: true

=== Test enable network ===
Network enabled successfully: true

=== Test disable network ===
Network disabled successfully: true

=== Test network toggle ===
Toggle pattern (true->false->true): true -> false -> true
Toggle pattern correct: true

=== Test network impact on CRS operations ===
CRS creation works without network: true
Transformer creation works without network: true
Transformation works without network: true
Network re-enabled: true

=== Test data directory access ===
User data dir accessible: true
Authorities available: true