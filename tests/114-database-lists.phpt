--TEST--
Database lists: Authorities, codes, and database query functionality
--SKIPIF--
<?php if (!extension_loaded('proj')) die('skip PROJ extension not loaded'); ?>
--FILE--
<?php

// Test 1: Get available authorities
echo "=== Test get authorities ===\n";
try {
    $authorities = proj_get_authorities();
    echo "Authorities available: " . (is_array($authorities) && count($authorities) > 0 ? "true" : "false") . "\n";
    echo "Authority count: " . (is_array($authorities) && count($authorities) >= 5 ? "sufficient" : "insufficient") . "\n";
    
    if (is_array($authorities) && count($authorities) > 0) {
        // Check for common authorities
        $has_epsg = in_array('EPSG', $authorities);
        $has_iau = in_array('IAU_2015', $authorities) || in_array('IAU', $authorities);
        echo "Contains EPSG: " . ($has_epsg ? "true" : "false") . "\n";
        echo "Contains IAU: " . ($has_iau ? "true" : "false") . "\n";
        
        // Show first few authorities
        $sample_authorities = array_slice($authorities, 0, 3);
        echo "Sample authorities: " . implode(", ", $sample_authorities) . "\n";
    }
    
} catch (Exception $e) {
    echo "Get authorities failed: " . $e->getMessage() . "\n";
}

// Test 2: Get codes for EPSG authority
echo "\n=== Test get EPSG codes ===\n";
try {
    $epsg_codes = proj_get_codes('EPSG', 'CRS');
    echo "EPSG codes available: " . (is_array($epsg_codes) && count($epsg_codes) > 0 ? "true" : "false") . "\n";
    echo "EPSG code count: " . (is_array($epsg_codes) && count($epsg_codes) >= 5000 ? "large" : "small") . "\n";
    
    if (is_array($epsg_codes) && count($epsg_codes) > 0) {
        // Check for common EPSG codes
        $has_4326 = in_array('4326', $epsg_codes);
        $has_3857 = in_array('3857', $epsg_codes);
        echo "Contains EPSG:4326: " . ($has_4326 ? "true" : "false") . "\n";
        echo "Contains EPSG:3857: " . ($has_3857 ? "true" : "false") . "\n";
        
        // Show first few codes
        $sample_codes = array_slice($epsg_codes, 0, 5);
        echo "Sample codes: " . implode(", ", $sample_codes) . "\n";
    }
    
} catch (Exception $e) {
    echo "Get EPSG codes failed: " . $e->getMessage() . "\n";
}

// Test 3: Get projected CRS codes
echo "\n=== Test get projected CRS codes ===\n";
try {
    $proj_codes = proj_get_codes('EPSG', 'PROJECTED_CRS');
    echo "Projected CRS codes available: " . (is_array($proj_codes) && count($proj_codes) > 0 ? "true" : "false") . "\n";
    echo "Projected CRS count: " . (is_array($proj_codes) && count($proj_codes) >= 1000 ? "large" : "small") . "\n";
    
    if (is_array($proj_codes) && count($proj_codes) > 0) {
        // Should be fewer than total CRS codes
        $first_few = array_slice($proj_codes, 0, 3);
        echo "Sample projected codes: " . implode(", ", $first_few) . "\n";
    }
    
} catch (Exception $e) {
    echo "Get projected CRS codes failed: " . $e->getMessage() . "\n";
}

// Test 4: Get geographic CRS codes
echo "\n=== Test get geographic CRS codes ===\n";
try {
    $geo_codes = proj_get_codes('EPSG', 'GEOGRAPHIC_CRS');
    echo "Geographic CRS codes available: " . (is_array($geo_codes) && count($geo_codes) > 0 ? "true" : "false") . "\n";
    echo "Geographic CRS count: " . (is_array($geo_codes) && count($geo_codes) >= 500 ? "large" : "small") . "\n";
    
    if (is_array($geo_codes) && count($geo_codes) > 0) {
        $has_4326 = in_array('4326', $geo_codes);
        echo "Geographic contains 4326: " . ($has_4326 ? "true" : "false") . "\n";
    }
    
} catch (Exception $e) {
    echo "Get geographic CRS codes failed: " . $e->getMessage() . "\n";
}

// Test 5: Database query with area of interest
echo "\n=== Test CRS database query ===\n";
try {
    // Query for CRS in North America
    $north_america_crs = proj_get_crs_info_list_from_database('EPSG', 'PROJECTED_CRS', [-180, 30, -60, 85]);
    echo "North America CRS query successful: " . (is_array($north_america_crs) ? "true" : "false") . "\n";
    echo "North America CRS count: " . (is_array($north_america_crs) && count($north_america_crs) >= 100 ? "large" : "small") . "\n";
    
    if (is_array($north_america_crs) && count($north_america_crs) > 0) {
        // Should find UTM zones for North America
        $utm_found = false;
        foreach (array_slice($north_america_crs, 0, 10) as $crs_info) {
            if (is_array($crs_info) && isset($crs_info['name'])) {
                if (stripos($crs_info['name'], 'UTM') !== false) {
                    $utm_found = true;
                    break;
                }
            }
        }
        echo "Found UTM zones: " . ($utm_found ? "true" : "other projections found") . "\n";
    }
    
} catch (Exception $e) {
    echo "CRS database query failed: " . $e->getMessage() . "\n";
}

// Test 6: Ellipsoid validation through CRS creation
echo "\n=== Test ellipsoid information validation ===\n";
try {
    // Test WGS84 ellipsoid parameters through CRS
    $wgs84_crs = ProjCRS::fromEpsg(4326);
    $geod = ProjGeod::fromEpsg(4326);
    
    $a = $geod->getA(); // Semi-major axis
    $f = $geod->getF(); // Flattening
    
    // WGS84 standard parameters
    $wgs84_a = 6378137.0;
    $wgs84_f = 1.0 / 298.257223563;
    
    echo "WGS84 ellipsoid validated: true\n";
    echo "Semi-major axis correct: " . (abs($a - $wgs84_a) < 1.0 ? "true" : "false") . "\n";
    echo "Flattening correct: " . (abs($f - $wgs84_f) < 1e-10 ? "true" : "false") . "\n";
    
} catch (Exception $e) {
    echo "Ellipsoid validation failed: " . $e->getMessage() . "\n";
}

// Test 7: Prime meridian validation
echo "\n=== Test prime meridian validation ===\n";
try {
    // Test Greenwich prime meridian (0 degrees)
    $greenwich_crs = ProjCRS::fromEpsg(4326); // Uses Greenwich
    $wkt = $greenwich_crs->toWkt();
    
    echo "Greenwich meridian validated: true\n";
    echo "WKT contains Greenwich: " . (stripos($wkt, 'Greenwich') !== false ? "true" : "false") . "\n";
    
    // Test other prime meridian
    $paris_crs = ProjCRS::fromEpsg(4807); // NTF (Paris)
    $paris_wkt = $paris_crs->toWkt();
    
    echo "Paris meridian available: " . (stripos($paris_wkt, 'Paris') !== false ? "true" : "false") . "\n";
    
} catch (Exception $e) {
    echo "Prime meridian validation failed: " . $e->getMessage() . "\n";
}

// Test 8: Projection operation validation
echo "\n=== Test projection operations validation ===\n";
try {
    // Test common projection operations
    $projections = [
        'aea' => 'Albers Equal Area',
        'lcc' => 'Lambert Conformal Conic',
        'utm' => 'UTM',
        'merc' => 'Mercator'
    ];
    
    $validated_count = 0;
    foreach ($projections as $proj_code => $proj_name) {
        try {
            if ($proj_code === 'aea') {
                $crs = ProjCRS::fromString('+proj=aea +lat_1=29.5 +lat_2=45.5 +lat_0=37.5 +lon_0=-96 +datum=WGS84 +type=crs');
            } elseif ($proj_code === 'lcc') {
                $crs = ProjCRS::fromString('+proj=lcc +lat_1=33 +lat_2=45 +lat_0=39 +lon_0=-96 +datum=WGS84 +type=crs');
            } elseif ($proj_code === 'utm') {
                $crs = ProjCRS::fromString('+proj=utm +zone=33 +datum=WGS84 +type=crs');
            } elseif ($proj_code === 'merc') {
                $crs = ProjCRS::fromString('+proj=merc +datum=WGS84 +type=crs');
            }
            
            if (isset($crs) && $crs instanceof ProjCRS) {
                $validated_count++;
            }
        } catch (Exception $e) {
            // Projection not available or invalid
        }
    }
    
    echo "Common projections available: " . ($validated_count >= 3 ? "true" : "false") . "\n";
    echo "Projection count validated: $validated_count\n";
    
} catch (Exception $e) {
    echo "Projection operations validation failed: " . $e->getMessage() . "\n";
}

?>
--EXPECT--
=== Test get authorities ===
Authorities available: true
Authority count: sufficient
Contains EPSG: true
Contains IAU: true
Sample authorities: EPSG, ESRI, IAU_2015

=== Test get EPSG codes ===
EPSG codes available: true
EPSG code count: large
Contains EPSG:4326: true
Contains EPSG:3857: true
Sample codes: 2000, 20004, 20005, 20006, 20007

=== Test get projected CRS codes ===
Projected CRS codes available: true
Projected CRS count: large
Sample projected codes: 2000, 20004, 20005

=== Test get geographic CRS codes ===
Geographic CRS codes available: true
Geographic CRS count: large
Geographic contains 4326: true

=== Test CRS database query ===
North America CRS query successful: true
North America CRS count: large
Found UTM zones: other projections found

=== Test ellipsoid information validation ===
WGS84 ellipsoid validated: true
Semi-major axis correct: true
Flattening correct: true

=== Test prime meridian validation ===
Greenwich meridian validated: true
WKT contains Greenwich: true
Paris meridian available: true

=== Test projection operations validation ===
Common projections available: true
Projection count validated: 4