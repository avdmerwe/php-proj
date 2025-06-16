#include "../php_proj.h"

/* Class entries */
zend_class_entry *proj_prime_meridian_ce;
extern zend_class_entry *proj_crs_exception_ce;

zend_object_handlers proj_prime_meridian_object_handlers;

/* Method declarations */
static PHP_METHOD(ProjPrimeMeridian, __construct);
static PHP_METHOD(ProjPrimeMeridian, fromAuthority);
static PHP_METHOD(ProjPrimeMeridian, fromEpsg);
static PHP_METHOD(ProjPrimeMeridian, fromJson);
static PHP_METHOD(ProjPrimeMeridian, fromString);
static PHP_METHOD(ProjPrimeMeridian, fromUserInput);
static PHP_METHOD(ProjPrimeMeridian, getName);
static PHP_METHOD(ProjPrimeMeridian, getLongitude);
static PHP_METHOD(ProjPrimeMeridian, getUnitName);
static PHP_METHOD(ProjPrimeMeridian, getUnitConversionFactor);
static PHP_METHOD(ProjPrimeMeridian, isExactSame);
static PHP_METHOD(ProjPrimeMeridian, toJson);
static PHP_METHOD(ProjPrimeMeridian, toWkt);
static PHP_METHOD(ProjPrimeMeridian, __toString);

/* ArgInfo declarations */
ZEND_BEGIN_ARG_INFO_EX(arginfo_prime_meridian_construct, 0, 0, 1)
    ZEND_ARG_INFO(0, prime_meridian_params)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_prime_meridian_fromAuthority, 0, 2, ProjPrimeMeridian, 0)
    ZEND_ARG_TYPE_INFO(0, auth_name, IS_STRING, 0)
    ZEND_ARG_TYPE_INFO(0, code, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_prime_meridian_fromEpsg, 0, 1, ProjPrimeMeridian, 0)
    ZEND_ARG_TYPE_INFO(0, code, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_prime_meridian_fromJson, 0, 1, ProjPrimeMeridian, 0)
    ZEND_ARG_TYPE_INFO(0, json_str, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_prime_meridian_fromString, 0, 1, ProjPrimeMeridian, 0)
    ZEND_ARG_TYPE_INFO(0, string, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_prime_meridian_fromUserInput, 0, 1, ProjPrimeMeridian, 0)
    ZEND_ARG_INFO(0, value)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_prime_meridian_getName, 0, 0, IS_STRING, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_prime_meridian_getLongitude, 0, 0, IS_DOUBLE, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_prime_meridian_getUnitName, 0, 0, IS_STRING, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_prime_meridian_getUnitConversionFactor, 0, 0, IS_DOUBLE, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_prime_meridian_isExactSame, 0, 1, _IS_BOOL, 0)
    ZEND_ARG_OBJ_INFO(0, other, ProjPrimeMeridian, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_prime_meridian_toJson, 0, 0, IS_STRING, 1)
    ZEND_ARG_TYPE_INFO(0, pretty, _IS_BOOL, 0)
ZEND_END_ARG_INFO()


ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_prime_meridian_toWkt, 0, 0, IS_STRING, 1)
    ZEND_ARG_TYPE_INFO(0, version, IS_STRING, 0)
    ZEND_ARG_TYPE_INFO(0, pretty, _IS_BOOL, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_prime_meridian_toString, 0, 0, IS_STRING, 0)
ZEND_END_ARG_INFO()

/* Method table */
static const zend_function_entry proj_prime_meridian_methods[] = {
    PHP_ME(ProjPrimeMeridian, __construct, arginfo_prime_meridian_construct, ZEND_ACC_PUBLIC)
    PHP_ME(ProjPrimeMeridian, fromAuthority, arginfo_prime_meridian_fromAuthority, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC)
    PHP_ME(ProjPrimeMeridian, fromEpsg, arginfo_prime_meridian_fromEpsg, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC)
    PHP_ME(ProjPrimeMeridian, fromJson, arginfo_prime_meridian_fromJson, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC)
    PHP_ME(ProjPrimeMeridian, fromString, arginfo_prime_meridian_fromString, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC)
    PHP_ME(ProjPrimeMeridian, fromUserInput, arginfo_prime_meridian_fromUserInput, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC)
    PHP_ME(ProjPrimeMeridian, getName, arginfo_prime_meridian_getName, ZEND_ACC_PUBLIC)
    PHP_ME(ProjPrimeMeridian, getLongitude, arginfo_prime_meridian_getLongitude, ZEND_ACC_PUBLIC)
    PHP_ME(ProjPrimeMeridian, getUnitName, arginfo_prime_meridian_getUnitName, ZEND_ACC_PUBLIC)
    PHP_ME(ProjPrimeMeridian, getUnitConversionFactor, arginfo_prime_meridian_getUnitConversionFactor, ZEND_ACC_PUBLIC)
    PHP_ME(ProjPrimeMeridian, isExactSame, arginfo_prime_meridian_isExactSame, ZEND_ACC_PUBLIC)
    PHP_ME(ProjPrimeMeridian, toJson, arginfo_prime_meridian_toJson, ZEND_ACC_PUBLIC)
    PHP_ME(ProjPrimeMeridian, toWkt, arginfo_prime_meridian_toWkt, ZEND_ACC_PUBLIC)
    PHP_ME(ProjPrimeMeridian, __toString, arginfo_prime_meridian_toString, ZEND_ACC_PUBLIC)
    PHP_FE_END
};

/* Initialize ProjPrimeMeridian class */
void proj_prime_meridian_init(void)
{
    zend_class_entry ce;
    
    INIT_CLASS_ENTRY(ce, "ProjPrimeMeridian", proj_prime_meridian_methods);
    proj_prime_meridian_ce = zend_register_internal_class(&ce);
    proj_prime_meridian_ce->create_object = proj_prime_meridian_object_create;
    
    memcpy(&proj_prime_meridian_object_handlers, &std_object_handlers, sizeof(zend_object_handlers));
    proj_prime_meridian_object_handlers.free_obj = proj_prime_meridian_object_destroy;
    proj_prime_meridian_object_handlers.offset = XtOffsetOf(proj_prime_meridian_object, std);
}

/* Object creation */
zend_object *proj_prime_meridian_object_create(zend_class_entry *ce)
{
    proj_prime_meridian_object *intern = ecalloc(1, sizeof(proj_prime_meridian_object) + zend_object_properties_size(ce));
    
    zend_object_std_init(&intern->std, ce);
    object_properties_init(&intern->std, ce);
    
    intern->pj = NULL;
    intern->std.handlers = &proj_prime_meridian_object_handlers;
    
    return &intern->std;
}

/* Object destruction */
void proj_prime_meridian_object_destroy(zend_object *object)
{
    proj_prime_meridian_object *intern = PRIME_MERIDIAN_FROM_OBJECT(object);
    
    if (intern->pj) {
        proj_destroy(intern->pj);
        intern->pj = NULL;
    }
    
    zend_object_std_dtor(&intern->std);
}

/* Constructor */
static PHP_METHOD(ProjPrimeMeridian, __construct)
{
    zval *prime_meridian_params;
    char *prime_meridian_string = NULL;
    PJ_CONTEXT *ctx;
    PJ *pj;
    proj_prime_meridian_object *intern;

    ZEND_PARSE_PARAMETERS_START(1, 1)
        Z_PARAM_ZVAL(prime_meridian_params)
    ZEND_PARSE_PARAMETERS_END();

    intern = PRIME_MERIDIAN_FROM_OBJECT(Z_OBJ_P(getThis()));

    if (!proj_parse_crs_input(prime_meridian_params, &prime_meridian_string)) {
        proj_throw_exception(proj_crs_exception_ce, "Invalid prime meridian input");
        return;
    }

    ctx = proj_get_default_context();
    pj = proj_create(ctx, prime_meridian_string);
    
    if (!pj) {
        efree(prime_meridian_string);
        proj_throw_crs_error(ctx);
        return;
    }

    /* Ensure it's a prime meridian */
    if (proj_get_type(pj) != PJ_TYPE_PRIME_MERIDIAN) {
        proj_destroy(pj);
        efree(prime_meridian_string);
        proj_throw_exception(proj_crs_exception_ce, "Input is not a valid prime meridian");
        return;
    }

    intern->pj = pj;
    efree(prime_meridian_string);
}

/* Static method: fromAuthority */
static PHP_METHOD(ProjPrimeMeridian, fromAuthority)
{
    zend_string *auth_name;
    zend_long code;
    char *prime_meridian_string;
    PJ_CONTEXT *ctx;
    PJ *pj;
    
    ZEND_PARSE_PARAMETERS_START(2, 2)
        Z_PARAM_STR(auth_name)
        Z_PARAM_LONG(code)
    ZEND_PARSE_PARAMETERS_END();

    spprintf(&prime_meridian_string, 0, "%s:%ld", ZSTR_VAL(auth_name), code);
    
    ctx = proj_get_default_context();
    pj = proj_create(ctx, prime_meridian_string);
    efree(prime_meridian_string);
    
    if (!pj) {
        proj_throw_crs_error(ctx);
        return;
    }

    if (proj_get_type(pj) != PJ_TYPE_PRIME_MERIDIAN) {
        proj_destroy(pj);
        proj_throw_exception(proj_crs_exception_ce, "%s:%ld is not a valid prime meridian", ZSTR_VAL(auth_name), code);
        return;
    }

    object_init_ex(return_value, proj_prime_meridian_ce);
    proj_prime_meridian_object *intern = PRIME_MERIDIAN_FROM_OBJECT(Z_OBJ_P(return_value));
    intern->pj = pj;
}

/* Static method: fromEpsg */
static PHP_METHOD(ProjPrimeMeridian, fromEpsg)
{
    zend_long epsg_code;
    char *prime_meridian_string;
    PJ_CONTEXT *ctx;
    PJ *pj;
    
    ZEND_PARSE_PARAMETERS_START(1, 1)
        Z_PARAM_LONG(epsg_code)
    ZEND_PARSE_PARAMETERS_END();

    spprintf(&prime_meridian_string, 0, "EPSG:%ld", epsg_code);
    
    ctx = proj_get_default_context();
    pj = proj_create(ctx, prime_meridian_string);
    efree(prime_meridian_string);
    
    if (!pj) {
        proj_throw_crs_error(ctx);
        return;
    }

    if (proj_get_type(pj) != PJ_TYPE_PRIME_MERIDIAN) {
        proj_destroy(pj);
        proj_throw_exception(proj_crs_exception_ce, "EPSG:%ld is not a valid prime meridian", epsg_code);
        return;
    }

    object_init_ex(return_value, proj_prime_meridian_ce);
    proj_prime_meridian_object *intern = PRIME_MERIDIAN_FROM_OBJECT(Z_OBJ_P(return_value));
    intern->pj = pj;
}

/* Get name */
static PHP_METHOD(ProjPrimeMeridian, getName)
{
    proj_prime_meridian_object *intern;
    const char *name;

    ZEND_PARSE_PARAMETERS_NONE();

    intern = PRIME_MERIDIAN_FROM_OBJECT(Z_OBJ_P(getThis()));
    if (!intern->pj) {
        RETURN_NULL();
    }

    name = proj_get_name(intern->pj);
    
    if (!name) {
        RETURN_NULL();
    }
    
    RETURN_STRING(name);
}

/* Get longitude */
static PHP_METHOD(ProjPrimeMeridian, getLongitude)
{
    proj_prime_meridian_object *intern;
    PJ_CONTEXT *ctx;
    double longitude;
    const char *unit_name;
    double unit_conv_factor;

    ZEND_PARSE_PARAMETERS_NONE();

    intern = PRIME_MERIDIAN_FROM_OBJECT(Z_OBJ_P(getThis()));
    if (!intern->pj) {
        RETURN_DOUBLE(0.0);
    }

    ctx = proj_get_default_context();
    
    if (proj_prime_meridian_get_parameters(ctx, intern->pj, &longitude, 
                                          &unit_conv_factor, &unit_name)) {
        RETURN_DOUBLE(longitude);
    }
    
    RETURN_DOUBLE(0.0);
}

/* Get unit name */
static PHP_METHOD(ProjPrimeMeridian, getUnitName)
{
    proj_prime_meridian_object *intern;
    PJ_CONTEXT *ctx;
    double longitude;
    const char *unit_name;
    double unit_conv_factor;

    ZEND_PARSE_PARAMETERS_NONE();

    intern = PRIME_MERIDIAN_FROM_OBJECT(Z_OBJ_P(getThis()));
    if (!intern->pj) {
        RETURN_NULL();
    }

    ctx = proj_get_default_context();
    
    if (proj_prime_meridian_get_parameters(ctx, intern->pj, &longitude, 
                                          &unit_conv_factor, &unit_name)) {
        if (unit_name) {
            RETURN_STRING(unit_name);
        }
    }
    
    RETURN_NULL();
}

/* Get unit conversion factor */
static PHP_METHOD(ProjPrimeMeridian, getUnitConversionFactor)
{
    proj_prime_meridian_object *intern;
    PJ_CONTEXT *ctx;
    double longitude;
    const char *unit_name;
    double unit_conv_factor;

    ZEND_PARSE_PARAMETERS_NONE();

    intern = PRIME_MERIDIAN_FROM_OBJECT(Z_OBJ_P(getThis()));
    if (!intern->pj) {
        RETURN_DOUBLE(0.0);
    }

    ctx = proj_get_default_context();
    
    if (proj_prime_meridian_get_parameters(ctx, intern->pj, &longitude, 
                                          &unit_conv_factor, &unit_name)) {
        RETURN_DOUBLE(unit_conv_factor);
    }
    
    RETURN_DOUBLE(0.0);
}

/* Convert to WKT */
static PHP_METHOD(ProjPrimeMeridian, toWkt)
{
    char *version = "WKT2_2019";
    size_t version_len;
    zend_bool pretty = 0;
    proj_prime_meridian_object *intern;
    PJ_CONTEXT *ctx;
    const char *wkt;
    PJ_WKT_TYPE wkt_type;

    ZEND_PARSE_PARAMETERS_START(0, 2)
        Z_PARAM_OPTIONAL
        Z_PARAM_STRING(version, version_len)
        Z_PARAM_BOOL(pretty)
    ZEND_PARSE_PARAMETERS_END();

    intern = PRIME_MERIDIAN_FROM_OBJECT(Z_OBJ_P(getThis()));
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
static PHP_METHOD(ProjPrimeMeridian, toJson)
{
    zend_bool pretty = 0;
    proj_prime_meridian_object *intern;
    PJ_CONTEXT *ctx;
    const char *json;

    ZEND_PARSE_PARAMETERS_START(0, 1)
        Z_PARAM_OPTIONAL
        Z_PARAM_BOOL(pretty)
    ZEND_PARSE_PARAMETERS_END();

    intern = PRIME_MERIDIAN_FROM_OBJECT(Z_OBJ_P(getThis()));
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
static PHP_METHOD(ProjPrimeMeridian, __toString)
{
    proj_prime_meridian_object *intern;
    const char *name;

    ZEND_PARSE_PARAMETERS_NONE();

    intern = PRIME_MERIDIAN_FROM_OBJECT(Z_OBJ_P(getThis()));
    if (!intern->pj) {
        proj_throw_exception(proj_crs_exception_ce, "Prime meridian object is not initialized");
        return;
    }

    name = proj_get_name(intern->pj);
    
    if (!name) {
        RETURN_STRING("Unknown Prime Meridian");
    }
    
    RETURN_STRING(name);
}

/* Additional factory methods */
static PHP_METHOD(ProjPrimeMeridian, fromString)
{
    zend_string *prime_meridian_string;
    PJ_CONTEXT *ctx;
    PJ *pj;
    
    ZEND_PARSE_PARAMETERS_START(1, 1)
        Z_PARAM_STR(prime_meridian_string)
    ZEND_PARSE_PARAMETERS_END();

    ctx = proj_get_default_context();
    pj = proj_create(ctx, ZSTR_VAL(prime_meridian_string));
    
    if (!pj) {
        proj_throw_crs_error(ctx);
        return;
    }

    if (proj_get_type(pj) != PJ_TYPE_PRIME_MERIDIAN) {
        proj_destroy(pj);
        proj_throw_exception(proj_crs_exception_ce, "String is not a valid prime meridian");
        return;
    }

    object_init_ex(return_value, proj_prime_meridian_ce);
    proj_prime_meridian_object *intern = PRIME_MERIDIAN_FROM_OBJECT(Z_OBJ_P(return_value));
    intern->pj = pj;
}

static PHP_METHOD(ProjPrimeMeridian, fromUserInput)
{
    zval *value;
    char *prime_meridian_string = NULL;
    PJ_CONTEXT *ctx;
    PJ *pj;
    
    ZEND_PARSE_PARAMETERS_START(1, 1)
        Z_PARAM_ZVAL(value)
    ZEND_PARSE_PARAMETERS_END();

    if (!proj_parse_crs_input(value, &prime_meridian_string)) {
        proj_throw_exception(proj_crs_exception_ce, "Invalid prime meridian input");
        return;
    }

    ctx = proj_get_default_context();
    pj = proj_create(ctx, prime_meridian_string);
    
    if (!pj) {
        efree(prime_meridian_string);
        proj_throw_crs_error(ctx);
        return;
    }

    if (proj_get_type(pj) != PJ_TYPE_PRIME_MERIDIAN) {
        proj_destroy(pj);
        efree(prime_meridian_string);
        proj_throw_exception(proj_crs_exception_ce, "Input is not a valid prime meridian");
        return;
    }

    object_init_ex(return_value, proj_prime_meridian_ce);
    proj_prime_meridian_object *intern = PRIME_MERIDIAN_FROM_OBJECT(Z_OBJ_P(return_value));
    intern->pj = pj;
    
    efree(prime_meridian_string);
}

static PHP_METHOD(ProjPrimeMeridian, fromJson)
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

    if (proj_get_type(pj) != PJ_TYPE_PRIME_MERIDIAN) {
        proj_destroy(pj);
        proj_throw_exception(proj_crs_exception_ce, "JSON is not a valid prime meridian");
        return;
    }

    object_init_ex(return_value, proj_prime_meridian_ce);
    proj_prime_meridian_object *intern = PRIME_MERIDIAN_FROM_OBJECT(Z_OBJ_P(return_value));
    intern->pj = pj;
}

static PHP_METHOD(ProjPrimeMeridian, isExactSame)
{
    zval *other;
    proj_prime_meridian_object *intern, *other_intern;

    ZEND_PARSE_PARAMETERS_START(1, 1)
        Z_PARAM_OBJECT_OF_CLASS(other, proj_prime_meridian_ce)
    ZEND_PARSE_PARAMETERS_END();

    intern = PRIME_MERIDIAN_FROM_OBJECT(Z_OBJ_P(getThis()));
    other_intern = PRIME_MERIDIAN_FROM_OBJECT(Z_OBJ_P(other));

    if (!intern->pj || !other_intern->pj) {
        RETURN_FALSE;
    }

    RETURN_BOOL(proj_is_equivalent_to(intern->pj, other_intern->pj, PJ_COMP_EQUIVALENT));
}

