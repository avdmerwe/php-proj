#include "../php_proj.h"

/* Class entries */
zend_class_entry *proj_coordinate_system_ce;
extern zend_class_entry *proj_crs_exception_ce;
extern zend_class_entry *proj_axis_ce;

zend_object_handlers proj_coordinate_system_object_handlers;

/* Method declarations */
static PHP_METHOD(ProjCoordinateSystem, __construct);
static PHP_METHOD(ProjCoordinateSystem, fromJson);
static PHP_METHOD(ProjCoordinateSystem, fromString);
static PHP_METHOD(ProjCoordinateSystem, fromUserInput);
static PHP_METHOD(ProjCoordinateSystem, getName);
static PHP_METHOD(ProjCoordinateSystem, getAxisList);
static PHP_METHOD(ProjCoordinateSystem, getRemarks);
static PHP_METHOD(ProjCoordinateSystem, getScope);
static PHP_METHOD(ProjCoordinateSystem, isExactSame);
static PHP_METHOD(ProjCoordinateSystem, toJson);
static PHP_METHOD(ProjCoordinateSystem, toWkt);
static PHP_METHOD(ProjCoordinateSystem, __toString);

/* ArgInfo declarations */
ZEND_BEGIN_ARG_INFO_EX(arginfo_coordinate_system_construct, 0, 0, 1)
    ZEND_ARG_INFO(0, coordinate_system_params)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_coordinate_system_fromJson, 0, 1, ProjCoordinateSystem, 0)
    ZEND_ARG_TYPE_INFO(0, json_str, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_coordinate_system_fromString, 0, 1, ProjCoordinateSystem, 0)
    ZEND_ARG_TYPE_INFO(0, string, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_coordinate_system_fromUserInput, 0, 1, ProjCoordinateSystem, 0)
    ZEND_ARG_INFO(0, value)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_coordinate_system_getName, 0, 0, IS_STRING, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_coordinate_system_getAxisList, 0, 0, IS_ARRAY, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_coordinate_system_getRemarks, 0, 0, IS_STRING, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_coordinate_system_getScope, 0, 0, IS_STRING, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_coordinate_system_isExactSame, 0, 1, _IS_BOOL, 0)
    ZEND_ARG_OBJ_INFO(0, other, ProjCoordinateSystem, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_coordinate_system_toJson, 0, 0, IS_STRING, 1)
    ZEND_ARG_TYPE_INFO(0, pretty, _IS_BOOL, 0)
ZEND_END_ARG_INFO()


ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_coordinate_system_toWkt, 0, 0, IS_STRING, 1)
    ZEND_ARG_TYPE_INFO(0, version, IS_STRING, 0)
    ZEND_ARG_TYPE_INFO(0, pretty, _IS_BOOL, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_coordinate_system_toString, 0, 0, IS_STRING, 0)
ZEND_END_ARG_INFO()

/* Method table */
static const zend_function_entry proj_coordinate_system_methods[] = {
    PHP_ME(ProjCoordinateSystem, __construct, arginfo_coordinate_system_construct, ZEND_ACC_PUBLIC)
    PHP_ME(ProjCoordinateSystem, fromJson, arginfo_coordinate_system_fromJson, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC)
    PHP_ME(ProjCoordinateSystem, fromString, arginfo_coordinate_system_fromString, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC)
    PHP_ME(ProjCoordinateSystem, fromUserInput, arginfo_coordinate_system_fromUserInput, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC)
    PHP_ME(ProjCoordinateSystem, getName, arginfo_coordinate_system_getName, ZEND_ACC_PUBLIC)
    PHP_ME(ProjCoordinateSystem, getAxisList, arginfo_coordinate_system_getAxisList, ZEND_ACC_PUBLIC)
    PHP_ME(ProjCoordinateSystem, getRemarks, arginfo_coordinate_system_getRemarks, ZEND_ACC_PUBLIC)
    PHP_ME(ProjCoordinateSystem, getScope, arginfo_coordinate_system_getScope, ZEND_ACC_PUBLIC)
    PHP_ME(ProjCoordinateSystem, isExactSame, arginfo_coordinate_system_isExactSame, ZEND_ACC_PUBLIC)
    PHP_ME(ProjCoordinateSystem, toJson, arginfo_coordinate_system_toJson, ZEND_ACC_PUBLIC)
    PHP_ME(ProjCoordinateSystem, toWkt, arginfo_coordinate_system_toWkt, ZEND_ACC_PUBLIC)
    PHP_ME(ProjCoordinateSystem, __toString, arginfo_coordinate_system_toString, ZEND_ACC_PUBLIC)
    PHP_FE_END
};

/* Initialize ProjCoordinateSystem class */
void proj_coordinate_system_init(void)
{
    zend_class_entry ce;
    
    INIT_CLASS_ENTRY(ce, "ProjCoordinateSystem", proj_coordinate_system_methods);
    proj_coordinate_system_ce = zend_register_internal_class(&ce);
    proj_coordinate_system_ce->create_object = proj_coordinate_system_object_create;
    
    memcpy(&proj_coordinate_system_object_handlers, &std_object_handlers, sizeof(zend_object_handlers));
    proj_coordinate_system_object_handlers.free_obj = proj_coordinate_system_object_destroy;
    proj_coordinate_system_object_handlers.offset = XtOffsetOf(proj_coordinate_system_object, std);
}

/* Object creation */
zend_object *proj_coordinate_system_object_create(zend_class_entry *ce)
{
    proj_coordinate_system_object *intern = ecalloc(1, sizeof(proj_coordinate_system_object) + zend_object_properties_size(ce));
    
    zend_object_std_init(&intern->std, ce);
    object_properties_init(&intern->std, ce);
    
    intern->pj = NULL;
    intern->std.handlers = &proj_coordinate_system_object_handlers;
    
    return &intern->std;
}

/* Object destruction */
void proj_coordinate_system_object_destroy(zend_object *object)
{
    proj_coordinate_system_object *intern = COORDINATE_SYSTEM_FROM_OBJECT(object);
    
    if (intern->pj) {
        proj_destroy(intern->pj);
        intern->pj = NULL;
    }
    
    zend_object_std_dtor(&intern->std);
}

/* Constructor */
static PHP_METHOD(ProjCoordinateSystem, __construct)
{
    zval *coordinate_system_params;
    char *cs_string = NULL;
    PJ_CONTEXT *ctx;
    PJ *pj;
    proj_coordinate_system_object *intern;

    ZEND_PARSE_PARAMETERS_START(1, 1)
        Z_PARAM_ZVAL(coordinate_system_params)
    ZEND_PARSE_PARAMETERS_END();

    intern = COORDINATE_SYSTEM_FROM_OBJECT(Z_OBJ_P(getThis()));

    if (!proj_parse_crs_input(coordinate_system_params, &cs_string)) {
        proj_throw_exception(proj_crs_exception_ce, "Invalid coordinate system input");
        return;
    }

    ctx = proj_get_default_context();
    pj = proj_create(ctx, cs_string);
    
    if (!pj) {
        efree(cs_string);
        proj_throw_crs_error(ctx);
        return;
    }

    /* For coordinate systems, we need to extract it from a CRS */
    PJ *cs = NULL;
    if (proj_is_crs(pj)) {
        cs = proj_crs_get_coordinate_system(ctx, pj);
        proj_destroy(pj);
        if (!cs) {
            efree(cs_string);
            proj_throw_exception(proj_crs_exception_ce, "Could not extract coordinate system from CRS");
            return;
        }
        pj = cs;
    } else {
        /* Assume it's already a coordinate system */
        // Note: PROJ doesn't have a direct way to validate if something is a coordinate system
        // We'll accept it and let subsequent operations validate
    }

    intern->pj = pj;
    efree(cs_string);
}

/* Static method: fromString */
static PHP_METHOD(ProjCoordinateSystem, fromString)
{
    zend_string *cs_string;
    PJ_CONTEXT *ctx;
    PJ *pj, *cs = NULL;
    
    ZEND_PARSE_PARAMETERS_START(1, 1)
        Z_PARAM_STR(cs_string)
    ZEND_PARSE_PARAMETERS_END();

    ctx = proj_get_default_context();
    pj = proj_create(ctx, ZSTR_VAL(cs_string));
    
    if (!pj) {
        proj_throw_crs_error(ctx);
        return;
    }

    /* Extract coordinate system if it's a CRS */
    if (proj_is_crs(pj)) {
        cs = proj_crs_get_coordinate_system(ctx, pj);
        proj_destroy(pj);
        if (!cs) {
            proj_throw_exception(proj_crs_exception_ce, "Could not extract coordinate system from CRS");
            return;
        }
        pj = cs;
    }

    object_init_ex(return_value, proj_coordinate_system_ce);
    proj_coordinate_system_object *intern = COORDINATE_SYSTEM_FROM_OBJECT(Z_OBJ_P(return_value));
    intern->pj = pj;
}

/* Get name */
static PHP_METHOD(ProjCoordinateSystem, getName)
{
    proj_coordinate_system_object *intern;
    const char *name;

    ZEND_PARSE_PARAMETERS_NONE();

    intern = COORDINATE_SYSTEM_FROM_OBJECT(Z_OBJ_P(getThis()));
    if (!intern->pj) {
        RETURN_NULL();
    }

    name = proj_get_name(intern->pj);
    
    if (!name) {
        RETURN_NULL();
    }
    
    RETURN_STRING(name);
}

/* Get axis list (returns array of ProjAxis objects) */
static PHP_METHOD(ProjCoordinateSystem, getAxisList)
{
    proj_coordinate_system_object *intern;
    PJ_CONTEXT *ctx;

    ZEND_PARSE_PARAMETERS_NONE();

    intern = COORDINATE_SYSTEM_FROM_OBJECT(Z_OBJ_P(getThis()));
    if (!intern->pj) {
        array_init(return_value);
        return;
    }

    ctx = proj_get_default_context();
    
    int axis_count = proj_cs_get_axis_count(ctx, intern->pj);
    if (axis_count <= 0) {
        array_init(return_value);
        return;
    }

    array_init(return_value);
    
    for (int i = 0; i < axis_count; i++) {
        const char *name, *abbrev, *direction, *unit_name;
        double unit_conv_factor;
        const char *unit_auth_name = NULL, *unit_code = NULL;
        
        if (proj_cs_get_axis_info(ctx, intern->pj, i, &name, &abbrev, &direction, 
                                 &unit_conv_factor, &unit_name, &unit_auth_name, &unit_code)) {
            
            /* Create ProjAxis object */
            zval axis_obj;
            object_init_ex(&axis_obj, proj_axis_ce);
            proj_axis_object *axis_intern = AXIS_FROM_OBJECT(Z_OBJ(axis_obj));
            
            /* Set axis properties directly */
            axis_intern->name = estrdup(name ? name : "");
            axis_intern->abbrev = estrdup(abbrev ? abbrev : "");
            axis_intern->direction = estrdup(direction ? direction : "");
            axis_intern->unit_name = estrdup(unit_name ? unit_name : "");
            axis_intern->unit_auth_code = unit_auth_name ? estrdup(unit_auth_name) : NULL;
            axis_intern->unit_code = unit_code ? estrdup(unit_code) : NULL;
            axis_intern->unit_conversion_factor = unit_conv_factor;
            
            add_next_index_zval(return_value, &axis_obj);
        }
    }
}

/* Get remarks */
static PHP_METHOD(ProjCoordinateSystem, getRemarks)
{
    proj_coordinate_system_object *intern;
    
    ZEND_PARSE_PARAMETERS_NONE();

    intern = COORDINATE_SYSTEM_FROM_OBJECT(Z_OBJ_P(getThis()));
    if (!intern->pj) {
        RETURN_NULL();
    }

    const char *remarks = proj_get_remarks(intern->pj);
    if (remarks && strlen(remarks) > 0) {
        RETURN_STRING(remarks);
    } else {
        RETURN_NULL();
    }
}

/* Get scope */
static PHP_METHOD(ProjCoordinateSystem, getScope)
{
    proj_coordinate_system_object *intern;
    
    ZEND_PARSE_PARAMETERS_NONE();

    intern = COORDINATE_SYSTEM_FROM_OBJECT(Z_OBJ_P(getThis()));
    if (!intern->pj) {
        RETURN_NULL();
    }

    const char *scope = proj_get_scope(intern->pj);
    if (scope && strlen(scope) > 0) {
        RETURN_STRING(scope);
    } else {
        RETURN_NULL();
    }
}

/* Convert to WKT */
static PHP_METHOD(ProjCoordinateSystem, toWkt)
{
    char *version = "WKT2_2019";
    size_t version_len;
    zend_bool pretty = 0;
    proj_coordinate_system_object *intern;
    PJ_CONTEXT *ctx;
    const char *wkt;
    PJ_WKT_TYPE wkt_type;

    ZEND_PARSE_PARAMETERS_START(0, 2)
        Z_PARAM_OPTIONAL
        Z_PARAM_STRING(version, version_len)
        Z_PARAM_BOOL(pretty)
    ZEND_PARSE_PARAMETERS_END();

    intern = COORDINATE_SYSTEM_FROM_OBJECT(Z_OBJ_P(getThis()));
    if (!intern->pj) {
        RETURN_NULL();
    }

    wkt_type = proj_wkt_version_to_enum(version);
    ctx = proj_get_default_context();
    wkt = proj_as_wkt(ctx, intern->pj, wkt_type, NULL);
    
    if (wkt) {
        RETURN_STRING(wkt);
    } else {
        proj_throw_crs_error(ctx);
        RETURN_NULL();
    }
}

/* Convert to JSON */
static PHP_METHOD(ProjCoordinateSystem, toJson)
{
    zend_bool pretty = 0;
    proj_coordinate_system_object *intern;
    PJ_CONTEXT *ctx;
    const char *json;

    ZEND_PARSE_PARAMETERS_START(0, 1)
        Z_PARAM_OPTIONAL
        Z_PARAM_BOOL(pretty)
    ZEND_PARSE_PARAMETERS_END();

    intern = COORDINATE_SYSTEM_FROM_OBJECT(Z_OBJ_P(getThis()));
    if (!intern->pj) {
        RETURN_NULL();
    }

    ctx = proj_get_default_context();
    json = proj_as_projjson(ctx, intern->pj, NULL);
    
    if (json) {
        RETURN_STRING(json);
    } else {
        proj_throw_crs_error(ctx);
        RETURN_NULL();
    }
}

/* String representation */
static PHP_METHOD(ProjCoordinateSystem, __toString)
{
    proj_coordinate_system_object *intern;
    const char *name;

    ZEND_PARSE_PARAMETERS_NONE();

    intern = COORDINATE_SYSTEM_FROM_OBJECT(Z_OBJ_P(getThis()));
    if (!intern->pj) {
        proj_throw_exception(proj_crs_exception_ce, "Coordinate system object is not initialized");
        return;
    }

    name = proj_get_name(intern->pj);
    
    if (!name) {
        RETURN_STRING("Unknown Coordinate System");
    }
    
    RETURN_STRING(name);
}

/* Additional factory methods */
static PHP_METHOD(ProjCoordinateSystem, fromUserInput)
{
    zval *value;
    char *cs_string = NULL;
    PJ_CONTEXT *ctx;
    PJ *pj, *cs = NULL;
    
    ZEND_PARSE_PARAMETERS_START(1, 1)
        Z_PARAM_ZVAL(value)
    ZEND_PARSE_PARAMETERS_END();

    if (!proj_parse_crs_input(value, &cs_string)) {
        proj_throw_exception(proj_crs_exception_ce, "Invalid coordinate system input");
        return;
    }

    ctx = proj_get_default_context();
    pj = proj_create(ctx, cs_string);
    
    if (!pj) {
        efree(cs_string);
        proj_throw_crs_error(ctx);
        return;
    }

    if (proj_is_crs(pj)) {
        cs = proj_crs_get_coordinate_system(ctx, pj);
        proj_destroy(pj);
        if (!cs) {
            efree(cs_string);
            proj_throw_exception(proj_crs_exception_ce, "Could not extract coordinate system from CRS");
            return;
        }
        pj = cs;
    }

    object_init_ex(return_value, proj_coordinate_system_ce);
    proj_coordinate_system_object *intern = COORDINATE_SYSTEM_FROM_OBJECT(Z_OBJ_P(return_value));
    intern->pj = pj;
    
    efree(cs_string);
}

static PHP_METHOD(ProjCoordinateSystem, fromJson)
{
    zend_string *json_string;
    PJ_CONTEXT *ctx;
    PJ *pj, *cs = NULL;
    
    ZEND_PARSE_PARAMETERS_START(1, 1)
        Z_PARAM_STR(json_string)
    ZEND_PARSE_PARAMETERS_END();

    ctx = proj_get_default_context();
    pj = proj_create(ctx, ZSTR_VAL(json_string));
    
    if (!pj) {
        proj_throw_crs_error(ctx);
        return;
    }

    if (proj_is_crs(pj)) {
        cs = proj_crs_get_coordinate_system(ctx, pj);
        proj_destroy(pj);
        if (!cs) {
            proj_throw_exception(proj_crs_exception_ce, "Could not extract coordinate system from JSON");
            return;
        }
        pj = cs;
    }

    object_init_ex(return_value, proj_coordinate_system_ce);
    proj_coordinate_system_object *intern = COORDINATE_SYSTEM_FROM_OBJECT(Z_OBJ_P(return_value));
    intern->pj = pj;
}

static PHP_METHOD(ProjCoordinateSystem, isExactSame)
{
    zval *other;
    proj_coordinate_system_object *intern, *other_intern;

    ZEND_PARSE_PARAMETERS_START(1, 1)
        Z_PARAM_OBJECT_OF_CLASS(other, proj_coordinate_system_ce)
    ZEND_PARSE_PARAMETERS_END();

    intern = COORDINATE_SYSTEM_FROM_OBJECT(Z_OBJ_P(getThis()));
    other_intern = COORDINATE_SYSTEM_FROM_OBJECT(Z_OBJ_P(other));

    if (!intern->pj || !other_intern->pj) {
        RETURN_FALSE;
    }

    RETURN_BOOL(proj_is_equivalent_to(intern->pj, other_intern->pj, PJ_COMP_EQUIVALENT));
}

