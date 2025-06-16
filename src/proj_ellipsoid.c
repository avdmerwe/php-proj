#include "../php_proj.h"

/* Class entries */
zend_class_entry *proj_ellipsoid_ce;
extern zend_class_entry *proj_crs_exception_ce;

zend_object_handlers proj_ellipsoid_object_handlers;

/* Method declarations */
static PHP_METHOD(ProjEllipsoid, __construct);
static PHP_METHOD(ProjEllipsoid, fromAuthority);
static PHP_METHOD(ProjEllipsoid, fromEpsg);
static PHP_METHOD(ProjEllipsoid, fromJson);
static PHP_METHOD(ProjEllipsoid, fromString);
static PHP_METHOD(ProjEllipsoid, fromUserInput);
static PHP_METHOD(ProjEllipsoid, getName);
static PHP_METHOD(ProjEllipsoid, getSemiMajorMetre);
static PHP_METHOD(ProjEllipsoid, getSemiMinorMetre);
static PHP_METHOD(ProjEllipsoid, getInverseFlattening);
static PHP_METHOD(ProjEllipsoid, isSemiMinorComputed);
static PHP_METHOD(ProjEllipsoid, getRemarks);
static PHP_METHOD(ProjEllipsoid, getScope);
static PHP_METHOD(ProjEllipsoid, isExactSame);
static PHP_METHOD(ProjEllipsoid, toJson);
static PHP_METHOD(ProjEllipsoid, toWkt);
static PHP_METHOD(ProjEllipsoid, __toString);

/* ArgInfo declarations */
ZEND_BEGIN_ARG_INFO_EX(arginfo_ellipsoid_construct, 0, 0, 1)
    ZEND_ARG_INFO(0, ellipsoid_params)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_ellipsoid_fromAuthority, 0, 2, ProjEllipsoid, 0)
    ZEND_ARG_TYPE_INFO(0, auth_name, IS_STRING, 0)
    ZEND_ARG_TYPE_INFO(0, code, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_ellipsoid_fromEpsg, 0, 1, ProjEllipsoid, 0)
    ZEND_ARG_TYPE_INFO(0, code, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_ellipsoid_fromJson, 0, 1, ProjEllipsoid, 0)
    ZEND_ARG_TYPE_INFO(0, json_str, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_ellipsoid_fromString, 0, 1, ProjEllipsoid, 0)
    ZEND_ARG_TYPE_INFO(0, string, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_ellipsoid_fromUserInput, 0, 1, ProjEllipsoid, 0)
    ZEND_ARG_INFO(0, value)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_ellipsoid_getName, 0, 0, IS_STRING, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_ellipsoid_getSemiMajorMetre, 0, 0, IS_DOUBLE, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_ellipsoid_getSemiMinorMetre, 0, 0, IS_DOUBLE, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_ellipsoid_getInverseFlattening, 0, 0, IS_DOUBLE, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_ellipsoid_isSemiMinorComputed, 0, 0, _IS_BOOL, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_ellipsoid_getRemarks, 0, 0, IS_STRING, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_ellipsoid_getScope, 0, 0, IS_STRING, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_ellipsoid_isExactSame, 0, 1, _IS_BOOL, 0)
    ZEND_ARG_OBJ_INFO(0, other, ProjEllipsoid, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_ellipsoid_toJson, 0, 0, IS_STRING, 1)
    ZEND_ARG_TYPE_INFO(0, pretty, _IS_BOOL, 0)
ZEND_END_ARG_INFO()


ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_ellipsoid_toWkt, 0, 0, IS_STRING, 1)
    ZEND_ARG_TYPE_INFO(0, version, IS_STRING, 0)
    ZEND_ARG_TYPE_INFO(0, pretty, _IS_BOOL, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_ellipsoid_toString, 0, 0, IS_STRING, 0)
ZEND_END_ARG_INFO()

/* Method table */
static const zend_function_entry proj_ellipsoid_methods[] = {
    PHP_ME(ProjEllipsoid, __construct, arginfo_ellipsoid_construct, ZEND_ACC_PUBLIC)
    PHP_ME(ProjEllipsoid, fromAuthority, arginfo_ellipsoid_fromAuthority, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC)
    PHP_ME(ProjEllipsoid, fromEpsg, arginfo_ellipsoid_fromEpsg, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC)
    PHP_ME(ProjEllipsoid, fromJson, arginfo_ellipsoid_fromJson, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC)
    PHP_ME(ProjEllipsoid, fromString, arginfo_ellipsoid_fromString, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC)
    PHP_ME(ProjEllipsoid, fromUserInput, arginfo_ellipsoid_fromUserInput, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC)
    PHP_ME(ProjEllipsoid, getName, arginfo_ellipsoid_getName, ZEND_ACC_PUBLIC)
    PHP_ME(ProjEllipsoid, getSemiMajorMetre, arginfo_ellipsoid_getSemiMajorMetre, ZEND_ACC_PUBLIC)
    PHP_ME(ProjEllipsoid, getSemiMinorMetre, arginfo_ellipsoid_getSemiMinorMetre, ZEND_ACC_PUBLIC)
    PHP_ME(ProjEllipsoid, getInverseFlattening, arginfo_ellipsoid_getInverseFlattening, ZEND_ACC_PUBLIC)
    PHP_ME(ProjEllipsoid, isSemiMinorComputed, arginfo_ellipsoid_isSemiMinorComputed, ZEND_ACC_PUBLIC)
    PHP_ME(ProjEllipsoid, getRemarks, arginfo_ellipsoid_getRemarks, ZEND_ACC_PUBLIC)
    PHP_ME(ProjEllipsoid, getScope, arginfo_ellipsoid_getScope, ZEND_ACC_PUBLIC)
    PHP_ME(ProjEllipsoid, isExactSame, arginfo_ellipsoid_isExactSame, ZEND_ACC_PUBLIC)
    PHP_ME(ProjEllipsoid, toJson, arginfo_ellipsoid_toJson, ZEND_ACC_PUBLIC)
    PHP_ME(ProjEllipsoid, toWkt, arginfo_ellipsoid_toWkt, ZEND_ACC_PUBLIC)
    PHP_ME(ProjEllipsoid, __toString, arginfo_ellipsoid_toString, ZEND_ACC_PUBLIC)
    PHP_FE_END
};

/* Initialize ProjEllipsoid class */
void proj_ellipsoid_init(void)
{
    zend_class_entry ce;
    
    INIT_CLASS_ENTRY(ce, "ProjEllipsoid", proj_ellipsoid_methods);
    proj_ellipsoid_ce = zend_register_internal_class(&ce);
    proj_ellipsoid_ce->create_object = proj_ellipsoid_object_create;
    
    memcpy(&proj_ellipsoid_object_handlers, &std_object_handlers, sizeof(zend_object_handlers));
    proj_ellipsoid_object_handlers.free_obj = proj_ellipsoid_object_destroy;
    proj_ellipsoid_object_handlers.offset = XtOffsetOf(proj_ellipsoid_object, std);
}

/* Object creation */
zend_object *proj_ellipsoid_object_create(zend_class_entry *ce)
{
    proj_ellipsoid_object *intern = ecalloc(1, sizeof(proj_ellipsoid_object) + zend_object_properties_size(ce));
    
    zend_object_std_init(&intern->std, ce);
    object_properties_init(&intern->std, ce);
    
    intern->pj = NULL;
    intern->std.handlers = &proj_ellipsoid_object_handlers;
    
    return &intern->std;
}

/* Object destruction */
void proj_ellipsoid_object_destroy(zend_object *object)
{
    proj_ellipsoid_object *intern = ELLIPSOID_FROM_OBJECT(object);
    
    if (intern->pj) {
        proj_destroy(intern->pj);
        intern->pj = NULL;
    }
    
    zend_object_std_dtor(&intern->std);
}

/* Constructor */
static PHP_METHOD(ProjEllipsoid, __construct)
{
    zval *ellipsoid_params;
    char *ellipsoid_string = NULL;
    PJ_CONTEXT *ctx;
    PJ *pj;
    proj_ellipsoid_object *intern;

    ZEND_PARSE_PARAMETERS_START(1, 1)
        Z_PARAM_ZVAL(ellipsoid_params)
    ZEND_PARSE_PARAMETERS_END();

    intern = ELLIPSOID_FROM_OBJECT(Z_OBJ_P(getThis()));

    if (!proj_parse_crs_input(ellipsoid_params, &ellipsoid_string)) {
        proj_throw_exception(proj_crs_exception_ce, "Invalid ellipsoid input");
        return;
    }

    ctx = proj_get_default_context();
    pj = proj_create(ctx, ellipsoid_string);
    
    if (!pj) {
        efree(ellipsoid_string);
        proj_throw_crs_error(ctx);
        return;
    }

    /* Ensure it's an ellipsoid */
    if (proj_get_type(pj) != PJ_TYPE_ELLIPSOID) {
        proj_destroy(pj);
        efree(ellipsoid_string);
        proj_throw_exception(proj_crs_exception_ce, "Input is not a valid ellipsoid");
        return;
    }

    intern->pj = pj;
    efree(ellipsoid_string);
}

/* Static method: fromAuthority */
static PHP_METHOD(ProjEllipsoid, fromAuthority)
{
    zend_string *auth_name;
    zend_long code;
    char *ellipsoid_string;
    PJ_CONTEXT *ctx;
    PJ *pj;
    
    ZEND_PARSE_PARAMETERS_START(2, 2)
        Z_PARAM_STR(auth_name)
        Z_PARAM_LONG(code)
    ZEND_PARSE_PARAMETERS_END();

    spprintf(&ellipsoid_string, 0, "%s:%ld", ZSTR_VAL(auth_name), code);
    
    ctx = proj_get_default_context();
    pj = proj_create(ctx, ellipsoid_string);
    efree(ellipsoid_string);
    
    if (!pj) {
        proj_throw_crs_error(ctx);
        return;
    }

    if (proj_get_type(pj) != PJ_TYPE_ELLIPSOID) {
        proj_destroy(pj);
        proj_throw_exception(proj_crs_exception_ce, "%s:%ld is not a valid ellipsoid", ZSTR_VAL(auth_name), code);
        return;
    }

    object_init_ex(return_value, proj_ellipsoid_ce);
    proj_ellipsoid_object *intern = ELLIPSOID_FROM_OBJECT(Z_OBJ_P(return_value));
    intern->pj = pj;
}

/* Static method: fromEpsg */
static PHP_METHOD(ProjEllipsoid, fromEpsg)
{
    zend_long epsg_code;
    char *ellipsoid_string;
    PJ_CONTEXT *ctx;
    PJ *pj;
    
    ZEND_PARSE_PARAMETERS_START(1, 1)
        Z_PARAM_LONG(epsg_code)
    ZEND_PARSE_PARAMETERS_END();

    spprintf(&ellipsoid_string, 0, "EPSG:%ld", epsg_code);
    
    ctx = proj_get_default_context();
    pj = proj_create(ctx, ellipsoid_string);
    efree(ellipsoid_string);
    
    if (!pj) {
        proj_throw_crs_error(ctx);
        return;
    }

    if (proj_get_type(pj) != PJ_TYPE_ELLIPSOID) {
        proj_destroy(pj);
        proj_throw_exception(proj_crs_exception_ce, "EPSG:%ld is not a valid ellipsoid", epsg_code);
        return;
    }

    object_init_ex(return_value, proj_ellipsoid_ce);
    proj_ellipsoid_object *intern = ELLIPSOID_FROM_OBJECT(Z_OBJ_P(return_value));
    intern->pj = pj;
}

/* Get name */
static PHP_METHOD(ProjEllipsoid, getName)
{
    proj_ellipsoid_object *intern;
    const char *name;

    ZEND_PARSE_PARAMETERS_NONE();

    intern = ELLIPSOID_FROM_OBJECT(Z_OBJ_P(getThis()));
    if (!intern->pj) {
        RETURN_NULL();
    }

    name = proj_get_name(intern->pj);
    
    if (!name) {
        RETURN_NULL();
    }
    
    RETURN_STRING(name);
}

/* Get semi major axis in metres */
static PHP_METHOD(ProjEllipsoid, getSemiMajorMetre)
{
    proj_ellipsoid_object *intern;
    PJ_CONTEXT *ctx;
    double semi_major_axis, semi_minor_axis, inv_flattening;
    int is_semi_minor_computed;

    ZEND_PARSE_PARAMETERS_NONE();

    intern = ELLIPSOID_FROM_OBJECT(Z_OBJ_P(getThis()));
    if (!intern->pj) {
        RETURN_DOUBLE(0.0);
    }

    ctx = proj_get_default_context();
    
    if (proj_ellipsoid_get_parameters(ctx, intern->pj, &semi_major_axis, &semi_minor_axis, 
                                     &is_semi_minor_computed, &inv_flattening)) {
        RETURN_DOUBLE(semi_major_axis);
    }
    
    RETURN_DOUBLE(0.0);
}

/* Get semi minor axis in metres */
static PHP_METHOD(ProjEllipsoid, getSemiMinorMetre)
{
    proj_ellipsoid_object *intern;
    PJ_CONTEXT *ctx;
    double semi_major_axis, semi_minor_axis, inv_flattening;
    int is_semi_minor_computed;

    ZEND_PARSE_PARAMETERS_NONE();

    intern = ELLIPSOID_FROM_OBJECT(Z_OBJ_P(getThis()));
    if (!intern->pj) {
        RETURN_DOUBLE(0.0);
    }

    ctx = proj_get_default_context();
    
    if (proj_ellipsoid_get_parameters(ctx, intern->pj, &semi_major_axis, &semi_minor_axis, 
                                     &is_semi_minor_computed, &inv_flattening)) {
        RETURN_DOUBLE(semi_minor_axis);
    }
    
    RETURN_DOUBLE(0.0);
}

/* Get inverse flattening */
static PHP_METHOD(ProjEllipsoid, getInverseFlattening)
{
    proj_ellipsoid_object *intern;
    PJ_CONTEXT *ctx;
    double semi_major_axis, semi_minor_axis, inv_flattening;
    int is_semi_minor_computed;

    ZEND_PARSE_PARAMETERS_NONE();

    intern = ELLIPSOID_FROM_OBJECT(Z_OBJ_P(getThis()));
    if (!intern->pj) {
        RETURN_DOUBLE(0.0);
    }

    ctx = proj_get_default_context();
    
    if (proj_ellipsoid_get_parameters(ctx, intern->pj, &semi_major_axis, &semi_minor_axis, 
                                     &is_semi_minor_computed, &inv_flattening)) {
        RETURN_DOUBLE(inv_flattening);
    }
    
    RETURN_DOUBLE(0.0);
}

/* Check if semi minor is computed */
static PHP_METHOD(ProjEllipsoid, isSemiMinorComputed)
{
    proj_ellipsoid_object *intern;
    PJ_CONTEXT *ctx;
    double semi_major_axis, semi_minor_axis, inv_flattening;
    int is_semi_minor_computed;

    ZEND_PARSE_PARAMETERS_NONE();

    intern = ELLIPSOID_FROM_OBJECT(Z_OBJ_P(getThis()));
    if (!intern->pj) {
        RETURN_FALSE;
    }

    ctx = proj_get_default_context();
    
    if (proj_ellipsoid_get_parameters(ctx, intern->pj, &semi_major_axis, &semi_minor_axis, 
                                     &is_semi_minor_computed, &inv_flattening)) {
        RETURN_BOOL(is_semi_minor_computed);
    }
    
    RETURN_FALSE;
}

/* Get remarks */
static PHP_METHOD(ProjEllipsoid, getRemarks)
{
    proj_ellipsoid_object *intern;
    
    ZEND_PARSE_PARAMETERS_NONE();

    intern = ELLIPSOID_FROM_OBJECT(Z_OBJ_P(getThis()));
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
static PHP_METHOD(ProjEllipsoid, getScope)
{
    proj_ellipsoid_object *intern;
    
    ZEND_PARSE_PARAMETERS_NONE();

    intern = ELLIPSOID_FROM_OBJECT(Z_OBJ_P(getThis()));
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
static PHP_METHOD(ProjEllipsoid, toWkt)
{
    char *version = "WKT2_2019";
    size_t version_len;
    zend_bool pretty = 0;
    proj_ellipsoid_object *intern;
    PJ_CONTEXT *ctx;
    const char *wkt;
    PJ_WKT_TYPE wkt_type;

    ZEND_PARSE_PARAMETERS_START(0, 2)
        Z_PARAM_OPTIONAL
        Z_PARAM_STRING(version, version_len)
        Z_PARAM_BOOL(pretty)
    ZEND_PARSE_PARAMETERS_END();

    intern = ELLIPSOID_FROM_OBJECT(Z_OBJ_P(getThis()));
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
static PHP_METHOD(ProjEllipsoid, toJson)
{
    zend_bool pretty = 0;
    proj_ellipsoid_object *intern;
    PJ_CONTEXT *ctx;
    const char *json;

    ZEND_PARSE_PARAMETERS_START(0, 1)
        Z_PARAM_OPTIONAL
        Z_PARAM_BOOL(pretty)
    ZEND_PARSE_PARAMETERS_END();

    intern = ELLIPSOID_FROM_OBJECT(Z_OBJ_P(getThis()));
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
static PHP_METHOD(ProjEllipsoid, __toString)
{
    proj_ellipsoid_object *intern;
    const char *name;

    ZEND_PARSE_PARAMETERS_NONE();

    intern = ELLIPSOID_FROM_OBJECT(Z_OBJ_P(getThis()));
    if (!intern->pj) {
        proj_throw_exception(proj_crs_exception_ce, "Ellipsoid object is not initialized");
        return;
    }

    name = proj_get_name(intern->pj);
    
    if (!name) {
        RETURN_STRING("Unknown Ellipsoid");
    }
    
    RETURN_STRING(name);
}

/* Static methods for other factory methods can be implemented similarly */
static PHP_METHOD(ProjEllipsoid, fromString)
{
    zend_string *ellipsoid_string;
    PJ_CONTEXT *ctx;
    PJ *pj;
    
    ZEND_PARSE_PARAMETERS_START(1, 1)
        Z_PARAM_STR(ellipsoid_string)
    ZEND_PARSE_PARAMETERS_END();

    ctx = proj_get_default_context();
    pj = proj_create(ctx, ZSTR_VAL(ellipsoid_string));
    
    if (!pj) {
        proj_throw_crs_error(ctx);
        return;
    }

    if (proj_get_type(pj) != PJ_TYPE_ELLIPSOID) {
        proj_destroy(pj);
        proj_throw_exception(proj_crs_exception_ce, "String is not a valid ellipsoid");
        return;
    }

    object_init_ex(return_value, proj_ellipsoid_ce);
    proj_ellipsoid_object *intern = ELLIPSOID_FROM_OBJECT(Z_OBJ_P(return_value));
    intern->pj = pj;
}

static PHP_METHOD(ProjEllipsoid, fromUserInput)
{
    zval *value;
    char *ellipsoid_string = NULL;
    PJ_CONTEXT *ctx;
    PJ *pj;
    
    ZEND_PARSE_PARAMETERS_START(1, 1)
        Z_PARAM_ZVAL(value)
    ZEND_PARSE_PARAMETERS_END();

    if (!proj_parse_crs_input(value, &ellipsoid_string)) {
        proj_throw_exception(proj_crs_exception_ce, "Invalid ellipsoid input");
        return;
    }

    ctx = proj_get_default_context();
    pj = proj_create(ctx, ellipsoid_string);
    
    if (!pj) {
        efree(ellipsoid_string);
        proj_throw_crs_error(ctx);
        return;
    }

    if (proj_get_type(pj) != PJ_TYPE_ELLIPSOID) {
        proj_destroy(pj);
        efree(ellipsoid_string);
        proj_throw_exception(proj_crs_exception_ce, "Input is not a valid ellipsoid");
        return;
    }

    object_init_ex(return_value, proj_ellipsoid_ce);
    proj_ellipsoid_object *intern = ELLIPSOID_FROM_OBJECT(Z_OBJ_P(return_value));
    intern->pj = pj;
    
    efree(ellipsoid_string);
}

static PHP_METHOD(ProjEllipsoid, fromJson)
{
    zend_string *json_string;
    PJ_CONTEXT *ctx;
    PJ *pj;
    
    ZEND_PARSE_PARAMETERS_START(1, 1)
        Z_PARAM_STR(json_string)
    ZEND_PARSE_PARAMETERS_END();

    ctx = proj_get_default_context();
    pj = proj_create(ctx, ZSTR_VAL(json_string));
    
    if (!pj) {
        proj_throw_crs_error(ctx);
        return;
    }

    if (proj_get_type(pj) != PJ_TYPE_ELLIPSOID) {
        proj_destroy(pj);
        proj_throw_exception(proj_crs_exception_ce, "JSON is not a valid ellipsoid");
        return;
    }

    object_init_ex(return_value, proj_ellipsoid_ce);
    proj_ellipsoid_object *intern = ELLIPSOID_FROM_OBJECT(Z_OBJ_P(return_value));
    intern->pj = pj;
}

static PHP_METHOD(ProjEllipsoid, isExactSame)
{
    zval *other;
    proj_ellipsoid_object *intern, *other_intern;

    ZEND_PARSE_PARAMETERS_START(1, 1)
        Z_PARAM_OBJECT_OF_CLASS(other, proj_ellipsoid_ce)
    ZEND_PARSE_PARAMETERS_END();

    intern = ELLIPSOID_FROM_OBJECT(Z_OBJ_P(getThis()));
    other_intern = ELLIPSOID_FROM_OBJECT(Z_OBJ_P(other));

    if (!intern->pj || !other_intern->pj) {
        RETURN_FALSE;
    }

    RETURN_BOOL(proj_is_equivalent_to(intern->pj, other_intern->pj, PJ_COMP_EQUIVALENT));
}

