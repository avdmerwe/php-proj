#include "../php_proj.h"

/* Enum class entries */
zend_class_entry *proj_wkt_version_ce;
zend_class_entry *proj_version_ce;
zend_class_entry *proj_transform_direction_ce;
zend_class_entry *proj_pj_type_ce;

/* Initialize enum classes */
void proj_enums_init(void)
{
    zend_class_entry ce;

    /* WktVersion enum */
    INIT_CLASS_ENTRY(ce, "ProjWktVersion", NULL);
    proj_wkt_version_ce = zend_register_internal_class(&ce);
    
    zend_declare_class_constant_string(proj_wkt_version_ce, "WKT1_GDAL", sizeof("WKT1_GDAL") - 1, "WKT1_GDAL");
    zend_declare_class_constant_string(proj_wkt_version_ce, "WKT1_ESRI", sizeof("WKT1_ESRI") - 1, "WKT1_ESRI");
    zend_declare_class_constant_string(proj_wkt_version_ce, "WKT2_2015", sizeof("WKT2_2015") - 1, "WKT2_2015");
    zend_declare_class_constant_string(proj_wkt_version_ce, "WKT2_2015_SIMPLIFIED", sizeof("WKT2_2015_SIMPLIFIED") - 1, "WKT2_2015_SIMPLIFIED");
    zend_declare_class_constant_string(proj_wkt_version_ce, "WKT2_2019", sizeof("WKT2_2019") - 1, "WKT2_2019");
    zend_declare_class_constant_string(proj_wkt_version_ce, "WKT2_2019_SIMPLIFIED", sizeof("WKT2_2019_SIMPLIFIED") - 1, "WKT2_2019_SIMPLIFIED");

    /* ProjVersion enum */
    INIT_CLASS_ENTRY(ce, "ProjVersion", NULL);
    proj_version_ce = zend_register_internal_class(&ce);
    
    zend_declare_class_constant_string(proj_version_ce, "PROJ_4", sizeof("PROJ_4") - 1, "PROJ_4");
    zend_declare_class_constant_string(proj_version_ce, "PROJ_5", sizeof("PROJ_5") - 1, "PROJ_5");

    /* TransformDirection enum */
    INIT_CLASS_ENTRY(ce, "ProjTransformDirection", NULL);
    proj_transform_direction_ce = zend_register_internal_class(&ce);
    
    zend_declare_class_constant_string(proj_transform_direction_ce, "FORWARD", sizeof("FORWARD") - 1, "FORWARD");
    zend_declare_class_constant_string(proj_transform_direction_ce, "INVERSE", sizeof("INVERSE") - 1, "INVERSE");
    zend_declare_class_constant_string(proj_transform_direction_ce, "IDENT", sizeof("IDENT") - 1, "IDENT");

    /* ProjType enum */
    INIT_CLASS_ENTRY(ce, "ProjType", NULL);
    proj_pj_type_ce = zend_register_internal_class(&ce);
    
    zend_declare_class_constant_string(proj_pj_type_ce, "UNKNOWN", sizeof("UNKNOWN") - 1, "unknown");
    zend_declare_class_constant_string(proj_pj_type_ce, "ELLIPSOID", sizeof("ELLIPSOID") - 1, "ellipsoid");
    zend_declare_class_constant_string(proj_pj_type_ce, "PRIME_MERIDIAN", sizeof("PRIME_MERIDIAN") - 1, "prime_meridian");
    zend_declare_class_constant_string(proj_pj_type_ce, "GEODETIC_REFERENCE_FRAME", sizeof("GEODETIC_REFERENCE_FRAME") - 1, "geodetic_reference_frame");
    zend_declare_class_constant_string(proj_pj_type_ce, "DYNAMIC_GEODETIC_REFERENCE_FRAME", sizeof("DYNAMIC_GEODETIC_REFERENCE_FRAME") - 1, "dynamic_geodetic_reference_frame");
    zend_declare_class_constant_string(proj_pj_type_ce, "VERTICAL_REFERENCE_FRAME", sizeof("VERTICAL_REFERENCE_FRAME") - 1, "vertical_reference_frame");
    zend_declare_class_constant_string(proj_pj_type_ce, "DYNAMIC_VERTICAL_REFERENCE_FRAME", sizeof("DYNAMIC_VERTICAL_REFERENCE_FRAME") - 1, "dynamic_vertical_reference_frame");
    zend_declare_class_constant_string(proj_pj_type_ce, "DATUM_ENSEMBLE", sizeof("DATUM_ENSEMBLE") - 1, "datum_ensemble");
    zend_declare_class_constant_string(proj_pj_type_ce, "CRS", sizeof("CRS") - 1, "coordinate_reference_system");
    zend_declare_class_constant_string(proj_pj_type_ce, "GEODETIC_CRS", sizeof("GEODETIC_CRS") - 1, "geodetic_crs");
    zend_declare_class_constant_string(proj_pj_type_ce, "GEOCENTRIC_CRS", sizeof("GEOCENTRIC_CRS") - 1, "geocentric_crs");
    zend_declare_class_constant_string(proj_pj_type_ce, "GEOGRAPHIC_CRS", sizeof("GEOGRAPHIC_CRS") - 1, "geographic_crs");
    zend_declare_class_constant_string(proj_pj_type_ce, "GEOGRAPHIC_2D_CRS", sizeof("GEOGRAPHIC_2D_CRS") - 1, "geographic_2d_crs");
    zend_declare_class_constant_string(proj_pj_type_ce, "GEOGRAPHIC_3D_CRS", sizeof("GEOGRAPHIC_3D_CRS") - 1, "geographic_3d_crs");
    zend_declare_class_constant_string(proj_pj_type_ce, "VERTICAL_CRS", sizeof("VERTICAL_CRS") - 1, "vertical_crs");
    zend_declare_class_constant_string(proj_pj_type_ce, "PROJECTED_CRS", sizeof("PROJECTED_CRS") - 1, "projected_crs");
    zend_declare_class_constant_string(proj_pj_type_ce, "COMPOUND_CRS", sizeof("COMPOUND_CRS") - 1, "compound_crs");
    zend_declare_class_constant_string(proj_pj_type_ce, "TEMPORAL_CRS", sizeof("TEMPORAL_CRS") - 1, "temporal_crs");
    zend_declare_class_constant_string(proj_pj_type_ce, "ENGINEERING_CRS", sizeof("ENGINEERING_CRS") - 1, "engineering_crs");
    zend_declare_class_constant_string(proj_pj_type_ce, "BOUND_CRS", sizeof("BOUND_CRS") - 1, "bound_crs");
    zend_declare_class_constant_string(proj_pj_type_ce, "OTHER_CRS", sizeof("OTHER_CRS") - 1, "other_crs");
    zend_declare_class_constant_string(proj_pj_type_ce, "CONVERSION", sizeof("CONVERSION") - 1, "conversion");
    zend_declare_class_constant_string(proj_pj_type_ce, "TRANSFORMATION", sizeof("TRANSFORMATION") - 1, "transformation");
    zend_declare_class_constant_string(proj_pj_type_ce, "CONCATENATED_OPERATION", sizeof("CONCATENATED_OPERATION") - 1, "concatenated_operation");
    zend_declare_class_constant_string(proj_pj_type_ce, "OTHER_COORDINATE_OPERATION", sizeof("OTHER_COORDINATE_OPERATION") - 1, "other_coordinate_operation");
}
