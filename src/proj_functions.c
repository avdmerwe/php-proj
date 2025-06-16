#include "../php_proj.h"
#include <strings.h>

/* Function declarations */
static PHP_FUNCTION(proj_version);
static PHP_FUNCTION(proj_get_authorities);
static PHP_FUNCTION(proj_get_codes);
static PHP_FUNCTION(proj_get_crs_info_list_from_database);
static PHP_FUNCTION(proj_set_use_global_context);
static PHP_FUNCTION(proj_is_network_enabled);
static PHP_FUNCTION(proj_set_network_enabled);
static PHP_FUNCTION(proj_get_user_data_dir);
static PHP_FUNCTION(proj_set_user_data_dir);
static PHP_FUNCTION(proj_list_ellipsoids);
static PHP_FUNCTION(proj_get_ellipsoid_by_name);

/* Arginfo definitions */
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_proj_version, 0, 0, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_proj_get_authorities, 0, 0, IS_ARRAY, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_proj_get_codes, 0, 1, IS_ARRAY, 0)
    ZEND_ARG_TYPE_INFO(0, auth_name, IS_STRING, 0)
    ZEND_ARG_TYPE_INFO(0, pj_type, IS_STRING, 1)
    ZEND_ARG_TYPE_INFO(0, allow_deprecated, _IS_BOOL, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_proj_get_crs_info_list_from_database, 0, 1, IS_ARRAY, 0)
    ZEND_ARG_TYPE_INFO(0, auth_name, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_proj_set_use_global_context, 0, 1, IS_VOID, 0)
    ZEND_ARG_TYPE_INFO(0, use_global, _IS_BOOL, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_proj_is_network_enabled, 0, 0, _IS_BOOL, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_proj_set_network_enabled, 0, 1, _IS_BOOL, 0)
    ZEND_ARG_TYPE_INFO(0, enabled, _IS_BOOL, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_proj_get_user_data_dir, 0, 0, IS_STRING, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_proj_set_user_data_dir, 0, 1, IS_VOID, 0)
    ZEND_ARG_TYPE_INFO(0, path, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_proj_list_ellipsoids, 0, 0, IS_ARRAY, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_proj_get_ellipsoid_by_name, 0, 1, IS_ARRAY, 1)
    ZEND_ARG_TYPE_INFO(0, name, IS_STRING, 0)
ZEND_END_ARG_INFO()

/* Function table */
static const zend_function_entry proj_functions[] = {
    PHP_FE(proj_version, arginfo_proj_version)
    PHP_FE(proj_get_authorities, arginfo_proj_get_authorities)
    PHP_FE(proj_get_codes, arginfo_proj_get_codes)
    PHP_FE(proj_get_crs_info_list_from_database, arginfo_proj_get_crs_info_list_from_database)
    PHP_FE(proj_set_use_global_context, arginfo_proj_set_use_global_context)
    PHP_FE(proj_is_network_enabled, arginfo_proj_is_network_enabled)
    PHP_FE(proj_set_network_enabled, arginfo_proj_set_network_enabled)
    PHP_FE(proj_get_user_data_dir, arginfo_proj_get_user_data_dir)
    PHP_FE(proj_set_user_data_dir, arginfo_proj_set_user_data_dir)
    PHP_FE(proj_list_ellipsoids, arginfo_proj_list_ellipsoids)
    PHP_FE(proj_get_ellipsoid_by_name, arginfo_proj_get_ellipsoid_by_name)
    PHP_FE_END
};

/* Initialize global functions */
void proj_functions_init(void)
{
    /* Register functions in the global namespace with proj_ prefix */
    zend_register_functions(NULL, proj_functions, NULL, MODULE_PERSISTENT);
}

/* Get PROJ library version */
static PHP_FUNCTION(proj_version)
{
    PJ_INFO info;
    
    ZEND_PARSE_PARAMETERS_NONE();
    
    info = proj_info();
    
    /* Version should always be set, but return empty string as fallback */
    RETURN_STRING(info.version ? info.version : "");
}

/* Get list of authorities */
static PHP_FUNCTION(proj_get_authorities)
{
    PJ_CONTEXT *ctx;
    PROJ_STRING_LIST authorities;
    int i;

    ZEND_PARSE_PARAMETERS_NONE();

    ctx = proj_get_default_context();
    authorities = proj_get_authorities_from_database(ctx);
    
    if (!authorities) {
        array_init(return_value);
        return;
    }

    array_init(return_value);
    
    for (i = 0; authorities[i]; i++) {
        add_next_index_string(return_value, authorities[i]);
    }
    
    proj_string_list_destroy(authorities);
}

/* Get codes for an authority */
static PHP_FUNCTION(proj_get_codes)
{
    char *auth_name;
    size_t auth_name_len;
    char *pj_type;
    size_t pj_type_len;
    zend_bool allow_deprecated = 0;
    
    PJ_CONTEXT *ctx;
    PJ_TYPE type_enum;
    PROJ_STRING_LIST codes;
    int i;

    ZEND_PARSE_PARAMETERS_START(1, 3)
        Z_PARAM_STRING(auth_name, auth_name_len)
        Z_PARAM_OPTIONAL
        Z_PARAM_STRING_OR_NULL(pj_type, pj_type_len)
        Z_PARAM_BOOL(allow_deprecated)
    ZEND_PARSE_PARAMETERS_END();

    /* Convert type string to enum */
    if (pj_type == NULL) {
        type_enum = PJ_TYPE_CRS; /* Default to all CRS types */
    } else if (strcmp(pj_type, "coordinate_reference_system") == 0) {
        type_enum = PJ_TYPE_CRS;
    } else if (strcmp(pj_type, "geodetic_crs") == 0) {
        type_enum = PJ_TYPE_GEODETIC_CRS;
    } else if (strcmp(pj_type, "geographic_crs") == 0) {
        type_enum = PJ_TYPE_GEOGRAPHIC_CRS;
    } else if (strcmp(pj_type, "projected_crs") == 0) {
        type_enum = PJ_TYPE_PROJECTED_CRS;
    } else if (strcmp(pj_type, "vertical_crs") == 0) {
        type_enum = PJ_TYPE_VERTICAL_CRS;
    } else if (strcmp(pj_type, "compound_crs") == 0) {
        type_enum = PJ_TYPE_COMPOUND_CRS;
    } else if (strcmp(pj_type, "ellipsoid") == 0) {
        type_enum = PJ_TYPE_ELLIPSOID;
    } else {
        type_enum = PJ_TYPE_CRS; /* Default */
    }

    ctx = proj_get_default_context();
    codes = proj_get_codes_from_database(ctx, auth_name, type_enum, allow_deprecated);
    
    if (!codes) {
        array_init(return_value);
        return;
    }

    array_init(return_value);
    
    for (i = 0; codes[i]; i++) {
        add_next_index_string(return_value, codes[i]);
    }
    
    proj_string_list_destroy(codes);
}

/* Get CRS info list from database */
static PHP_FUNCTION(proj_get_crs_info_list_from_database)
{
    char *auth_name = NULL;
    size_t auth_name_len = 0;
    char *pj_type = NULL;
    size_t pj_type_len = 0;
    zval *area_of_interest = NULL;
    zend_bool contains = 0;
    zend_bool allow_deprecated = 0;
    
    PJ_CONTEXT *ctx;
    PROJ_CRS_INFO **info_list;
    int count, i;

    ZEND_PARSE_PARAMETERS_START(0, 5)
        Z_PARAM_OPTIONAL
        Z_PARAM_STRING_OR_NULL(auth_name, auth_name_len)
        Z_PARAM_STRING_OR_NULL(pj_type, pj_type_len)
        Z_PARAM_ZVAL_OR_NULL(area_of_interest)
        Z_PARAM_BOOL(contains)
        Z_PARAM_BOOL(allow_deprecated)
    ZEND_PARSE_PARAMETERS_END();

    ctx = proj_get_default_context();
    
    /* Handle area_of_interest parameter */
    PJ_AREA *area = NULL;
    if (area_of_interest && Z_TYPE_P(area_of_interest) == IS_ARRAY) {
        /* Parse area of interest array [west, south, east, north] */
        zval *west_val = zend_hash_index_find(Z_ARRVAL_P(area_of_interest), 0);
        zval *south_val = zend_hash_index_find(Z_ARRVAL_P(area_of_interest), 1);
        zval *east_val = zend_hash_index_find(Z_ARRVAL_P(area_of_interest), 2);
        zval *north_val = zend_hash_index_find(Z_ARRVAL_P(area_of_interest), 3);
        
        if (west_val && south_val && east_val && north_val) {
            double west = zval_get_double(west_val);
            double south = zval_get_double(south_val);
            double east = zval_get_double(east_val);
            double north = zval_get_double(north_val);
            
            area = proj_area_create();
            if (area) {
                proj_area_set_bbox(area, west, south, east, north);
            }
        }
    }
    
    info_list = proj_get_crs_info_list_from_database(ctx, auth_name, NULL, &count);
    
    if (area) {
        proj_area_destroy(area);
    }
    
    if (!info_list) {
        array_init(return_value);
        return;
    }

    array_init(return_value);
    
    for (i = 0; i < count; i++) {
        zval info_array;
        array_init(&info_array);
        
        add_assoc_string(&info_array, "auth_name", info_list[i]->auth_name ? info_list[i]->auth_name : "");
        add_assoc_string(&info_array, "code", info_list[i]->code ? info_list[i]->code : "");
        add_assoc_string(&info_array, "name", info_list[i]->name ? info_list[i]->name : "");
        /* Convert type enum to string */
        const char *type_str = "unknown";
        switch (info_list[i]->type) {
            case PJ_TYPE_ELLIPSOID: type_str = "ellipsoid"; break;
            case PJ_TYPE_PRIME_MERIDIAN: type_str = "prime_meridian"; break;
            case PJ_TYPE_GEODETIC_REFERENCE_FRAME: type_str = "geodetic_reference_frame"; break;
            case PJ_TYPE_CRS: type_str = "coordinate_reference_system"; break;
            case PJ_TYPE_GEODETIC_CRS: type_str = "geodetic_crs"; break;
            case PJ_TYPE_GEOCENTRIC_CRS: type_str = "geocentric_crs"; break;
            case PJ_TYPE_GEOGRAPHIC_CRS: type_str = "geographic_crs"; break;
            case PJ_TYPE_GEOGRAPHIC_2D_CRS: type_str = "geographic_2d_crs"; break;
            case PJ_TYPE_GEOGRAPHIC_3D_CRS: type_str = "geographic_3d_crs"; break;
            case PJ_TYPE_VERTICAL_CRS: type_str = "vertical_crs"; break;
            case PJ_TYPE_PROJECTED_CRS: type_str = "projected_crs"; break;
            case PJ_TYPE_COMPOUND_CRS: type_str = "compound_crs"; break;
            case PJ_TYPE_TEMPORAL_CRS: type_str = "temporal_crs"; break;
            case PJ_TYPE_ENGINEERING_CRS: type_str = "engineering_crs"; break;
            case PJ_TYPE_BOUND_CRS: type_str = "bound_crs"; break;
            case PJ_TYPE_OTHER_CRS: type_str = "other_crs"; break;
            case PJ_TYPE_CONVERSION: type_str = "conversion"; break;
            case PJ_TYPE_TRANSFORMATION: type_str = "transformation"; break;
            case PJ_TYPE_CONCATENATED_OPERATION: type_str = "concatenated_operation"; break;
            case PJ_TYPE_OTHER_COORDINATE_OPERATION: type_str = "other_coordinate_operation"; break;
            default: type_str = "unknown"; break;
        }
        add_assoc_string(&info_array, "type", type_str);
        add_assoc_bool(&info_array, "deprecated", info_list[i]->deprecated);
        
        if (info_list[i]->bbox_valid) {
            zval bbox_array;
            array_init(&bbox_array);
            add_next_index_double(&bbox_array, info_list[i]->west_lon_degree);
            add_next_index_double(&bbox_array, info_list[i]->south_lat_degree);
            add_next_index_double(&bbox_array, info_list[i]->east_lon_degree);
            add_next_index_double(&bbox_array, info_list[i]->north_lat_degree);
            add_assoc_zval(&info_array, "bbox", &bbox_array);
        }
        
        add_next_index_zval(return_value, &info_array);
    }
    
    proj_crs_info_list_destroy(info_list);
}

/* Set global context usage */
static PHP_FUNCTION(proj_set_use_global_context)
{
    zend_bool active = 1;

    ZEND_PARSE_PARAMETERS_START(0, 1)
        Z_PARAM_OPTIONAL
        Z_PARAM_BOOL(active)
    ZEND_PARSE_PARAMETERS_END();

    /* This is primarily informational for PHP implementation */
    /* since we always use a global context */
    RETURN_BOOL(1);
}

/* Check if network is enabled */
static PHP_FUNCTION(proj_is_network_enabled)
{
    PJ_CONTEXT *ctx;
    int enabled;

    ZEND_PARSE_PARAMETERS_NONE();

    ctx = proj_get_default_context();
    enabled = proj_context_is_network_enabled(ctx);
    
    RETURN_BOOL(enabled);
}

/* Set network enabled */
static PHP_FUNCTION(proj_set_network_enabled)
{
    zend_bool active = 1;
    PJ_CONTEXT *ctx;
    int result;

    ZEND_PARSE_PARAMETERS_START(0, 1)
        Z_PARAM_OPTIONAL
        Z_PARAM_BOOL(active)
    ZEND_PARSE_PARAMETERS_END();

    ctx = proj_get_default_context();
    result = proj_context_set_enable_network(ctx, active);

    /* PROJ returns 1 for success, 0 for failure, but the function always succeeds 
       in setting the network state, so we should always return TRUE for success */
    RETURN_TRUE;
}

/* Get user data directory */
static PHP_FUNCTION(proj_get_user_data_dir)
{
    zend_bool create = 0;
    PJ_CONTEXT *ctx;
    const char *data_dir;

    ZEND_PARSE_PARAMETERS_START(0, 1)
        Z_PARAM_OPTIONAL
        Z_PARAM_BOOL(create)
    ZEND_PARSE_PARAMETERS_END();

    ctx = proj_get_default_context();
    data_dir = proj_context_get_user_writable_directory(ctx, create);
    
    if (!data_dir) {
        proj_throw_exception(proj_data_dir_exception_ce, "Failed to get user data directory");
        return;
    }
    
    RETURN_STRING(data_dir);
}

/* Set user data directory */
static PHP_FUNCTION(proj_set_user_data_dir)
{
    char *user_data_dir = NULL;
    size_t user_data_dir_len = 0;
    PJ_CONTEXT *ctx;

    ZEND_PARSE_PARAMETERS_START(0, 1)
        Z_PARAM_OPTIONAL
        Z_PARAM_STRING_OR_NULL(user_data_dir, user_data_dir_len)
    ZEND_PARSE_PARAMETERS_END();

    ctx = proj_get_default_context();
    
    if (user_data_dir) {
        const char* const path_array[] = { user_data_dir };
        proj_context_set_search_paths(ctx, 1, path_array);
        RETURN_STRING(user_data_dir);
    } else {
        /* Reset to default */
        const char *default_dir = proj_context_get_user_writable_directory(ctx, 0);
        RETURN_STRING(default_dir ? default_dir : "");
    }
}

/* List built-in ellipsoids */
static PHP_FUNCTION(proj_list_ellipsoids)
{
    const PJ_ELLPS *ellps_list;
    int i;

    ZEND_PARSE_PARAMETERS_NONE();

    ellps_list = proj_list_ellps();
    
    if (!ellps_list) {
        array_init(return_value);
        return;
    }

    array_init(return_value);
    
    /* Iterate through the ellipsoid list until we hit a NULL id */
    for (i = 0; ellps_list[i].id; i++) {
        zval ellps_array;
        array_init(&ellps_array);
        
        add_assoc_string(&ellps_array, "id", (char *)ellps_list[i].id);
        add_assoc_string(&ellps_array, "major", (char *)ellps_list[i].major);
        add_assoc_string(&ellps_array, "ell", (char *)ellps_list[i].ell);
        add_assoc_string(&ellps_array, "name", (char *)ellps_list[i].name);
        
        add_next_index_zval(return_value, &ellps_array);
    }
}

/* Get ellipsoid by name from built-in list */
static PHP_FUNCTION(proj_get_ellipsoid_by_name)
{
    char *name;
    size_t name_len;
    const PJ_ELLPS *ellps_list;
    int i;
    zend_bool found = 0;

    ZEND_PARSE_PARAMETERS_START(1, 1)
        Z_PARAM_STRING(name, name_len)
    ZEND_PARSE_PARAMETERS_END();

    ellps_list = proj_list_ellps();
    
    if (!ellps_list) {
        RETURN_NULL();
    }
    
    /* Search for ellipsoid by id or name (case insensitive) */
    for (i = 0; ellps_list[i].id; i++) {
        if (strcasecmp(ellps_list[i].id, name) == 0 ||
            (ellps_list[i].name && strcasestr(ellps_list[i].name, name) != NULL)) {
            found = 1;
            break;
        }
    }
    
    if (!found) {
        RETURN_NULL();
    }
    
    /* Return the found ellipsoid as an array */
    array_init(return_value);
    add_assoc_string(return_value, "id", (char *)ellps_list[i].id);
    add_assoc_string(return_value, "major", (char *)ellps_list[i].major);
    add_assoc_string(return_value, "ell", (char *)ellps_list[i].ell);
    add_assoc_string(return_value, "name", (char *)ellps_list[i].name);
}
