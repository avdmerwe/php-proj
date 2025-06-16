--TEST--
CRS: Properties and information methods
--SKIPIF--
<?php if (!extension_loaded('proj')) die('skip PROJ extension not loaded'); ?>
--FILE--
<?php

// Test 1: Basic CRS properties
echo "=== Test basic CRS properties ===\n";
$crs = ProjCRS::fromEpsg(4326);

echo "Name: " . $crs->getName() . "\n";
echo "Type: " . $crs->getTypeName() . "\n";

// Test 2: Type checking methods
echo "\n=== Test type checking ===\n";
echo "Is Geographic: " . ($crs->isGeographic() ? "true" : "false") . "\n";
echo "Is Projected: " . ($crs->isProjected() ? "true" : "false") . "\n";
echo "Is Geocentric: " . ($crs->isGeocentric() ? "true" : "false") . "\n";

// Test 3: Axis information
echo "\n=== Test axis information ===\n";
$axis_info = $crs->getAxisInfo();
echo "Axis count: " . count($axis_info) . "\n";
if (count($axis_info) > 0) {
    echo "First axis: " . $axis_info[0]->getName() . " (" . $axis_info[0]->getDirection() . ")\n";
    echo "Second axis: " . $axis_info[1]->getName() . " (" . $axis_info[1]->getDirection() . ")\n";
}

// Test 4: Area of use
echo "\n=== Test area of use ===\n";
$area = $crs->getAreaOfUse();
if ($area) {
    echo "Area name: " . $area->getName() . "\n";
    echo "Bounds: [" . $area->getWest() . ", " . $area->getSouth() . ", " . $area->getEast() . ", " . $area->getNorth() . "]\n";
} else {
    echo "No area of use information\n";
}

// Test 5: Format conversions
echo "\n=== Test format conversions ===\n";
echo "PROJ4: " . $crs->toProj4() . "\n";

$wkt = $crs->toWkt();
echo "WKT starts with GEOGCRS: " . (strpos($wkt, 'GEOGCRS') === 0 ? "true" : "false") . "\n";

$json = $crs->toJson();
echo "JSON contains 'type': " . (strpos($json, '"type"') !== false ? "true" : "false") . "\n";

// Test 6: String representation
echo "\n=== Test string representation ===\n";
$str = (string)$crs;
echo "String length > 0: " . (strlen($str) > 0 ? "true" : "false") . "\n";

// Test 7: Projected CRS (UTM)
echo "\n=== Test projected CRS (UTM 32N) ===\n";
$utm = ProjCRS::fromEpsg(32632);
echo "UTM Name: " . $utm->getName() . "\n";
echo "UTM Type: " . $utm->getTypeName() . "\n";
echo "UTM Is Geographic: " . ($utm->isGeographic() ? "true" : "false") . "\n";
echo "UTM Is Projected: " . ($utm->isProjected() ? "true" : "false") . "\n";

// Test 8: CRS comparison
echo "\n=== Test CRS comparison ===\n";
$crs2 = ProjCRS::fromEpsg(4326);
$crs3 = ProjCRS::fromEpsg(4269); // NAD83
echo "Same CRS equals: " . ($crs->equals($crs2) ? "true" : "false") . "\n";
echo "Different CRS equals: " . ($crs->equals($crs3) ? "true" : "false") . "\n";

?>
--EXPECT--
=== Test basic CRS properties ===
Name: WGS 84
Type: Geographic 2D CRS

=== Test type checking ===
Is Geographic: true
Is Projected: false
Is Geocentric: false

=== Test axis information ===
Axis count: 2
First axis: Geodetic latitude (north)
Second axis: Geodetic longitude (east)

=== Test area of use ===
Area name: World.
Bounds: [-180, -90, 180, 90]

=== Test format conversions ===
PROJ4: +proj=longlat +datum=WGS84 +no_defs +type=crs
WKT starts with GEOGCRS: true
JSON contains 'type': true

=== Test string representation ===
String length > 0: true

=== Test projected CRS (UTM 32N) ===
UTM Name: WGS 84 / UTM zone 32N
UTM Type: Projected CRS
UTM Is Geographic: false
UTM Is Projected: true

=== Test CRS comparison ===
Same CRS equals: true
Different CRS equals: false