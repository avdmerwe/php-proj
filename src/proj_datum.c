#include "../php_proj.h"

/* Class entries */
zend_class_entry *proj_datum_ce;
extern zend_class_entry *proj_crs_exception_ce;
extern zend_class_entry *proj_ellipsoid_ce;
extern zend_class_entry *proj_prime_meridian_ce;

zend_object_handlers proj_datum_object_handlers;

/* Method declarations */
static PHP_METHOD(ProjDatum, __construct);
static PHP_METHOD(ProjDatum, fromAuthority);
static PHP_METHOD(ProjDatum, fromEpsg);
static PHP_METHOD(ProjDatum, fromJson);
static PHP_METHOD(ProjDatum, fromString);
static PHP_METHOD(ProjDatum, fromUserInput);
static PHP_METHOD(ProjDatum, getName);
static PHP_METHOD(ProjDatum, getTypeName);
static PHP_METHOD(ProjDatum, getEllipsoid);
static PHP_METHOD(ProjDatum, getPrimeMeridian);
static PHP_METHOD(ProjDatum, getRemarks);
static PHP_METHOD(ProjDatum, getScope);
static PHP_METHOD(ProjDatum, isExactSame);
static PHP_METHOD(ProjDatum, toJson);
static PHP_METHOD(ProjDatum, toWkt);
static PHP_METHOD(ProjDatum, __toString);

/* ArgInfo declarations */
ZEND_BEGIN_ARG_INFO_EX(arginfo_datum_construct, 0, 0, 1)
    ZEND_ARG_INFO(0, datum_params)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_datum_fromAuthority, 0, 2, ProjDatum, 0)
    ZEND_ARG_TYPE_INFO(0, auth_name, IS_STRING, 0)
    ZEND_ARG_TYPE_INFO(0, code, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_datum_fromEpsg, 0, 1, ProjDatum, 0)
    ZEND_ARG_TYPE_INFO(0, code, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_datum_fromJson, 0, 1, ProjDatum, 0)
    ZEND_ARG_TYPE_INFO(0, json_str, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_datum_fromString, 0, 1, ProjDatum, 0)
    ZEND_ARG_TYPE_INFO(0, string, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_datum_fromUserInput, 0, 1, ProjDatum, 0)
    ZEND_ARG_INFO(0, value)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_datum_getName, 0, 0, IS_STRING, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_datum_getTypeName, 0, 0, IS_STRING, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_datum_getEllipsoid, 0, 0, ProjEllipsoid, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_datum_getPrimeMeridian, 0, 0, ProjPrimeMeridian, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_datum_getRemarks, 0, 0, IS_STRING, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_datum_getScope, 0, 0, IS_STRING, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_datum_isExactSame, 0, 1, _IS_BOOL, 0)
    ZEND_ARG_OBJ_INFO(0, other, ProjDatum, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_datum_toJson, 0, 0, IS_STRING, 1)
    ZEND_ARG_TYPE_INFO(0, pretty, _IS_BOOL, 0)
ZEND_END_ARG_INFO()


ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_datum_toWkt, 0, 0, IS_STRING, 1)
    ZEND_ARG_TYPE_INFO(0, version, IS_STRING, 0)
    ZEND_ARG_TYPE_INFO(0, pretty, _IS_BOOL, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_datum_toString, 0, 0, IS_STRING, 0)
ZEND_END_ARG_INFO()

/* Method table */
static const zend_function_entry proj_datum_methods[] = {
    PHP_ME(ProjDatum, __construct, arginfo_datum_construct, ZEND_ACC_PUBLIC)
    PHP_ME(ProjDatum, fromAuthority, arginfo_datum_fromAuthority, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC)
    PHP_ME(ProjDatum, fromEpsg, arginfo_datum_fromEpsg, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC)
    PHP_ME(ProjDatum, fromJson, arginfo_datum_fromJson, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC)
    PHP_ME(ProjDatum, fromString, arginfo_datum_fromString, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC)
    PHP_ME(ProjDatum, fromUserInput, arginfo_datum_fromUserInput, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC)
    PHP_ME(ProjDatum, getName, arginfo_datum_getName, ZEND_ACC_PUBLIC)
    PHP_ME(ProjDatum, getTypeName, arginfo_datum_getTypeName, ZEND_ACC_PUBLIC)
    PHP_ME(ProjDatum, getEllipsoid, arginfo_datum_getEllipsoid, ZEND_ACC_PUBLIC)
    PHP_ME(ProjDatum, getPrimeMeridian, arginfo_datum_getPrimeMeridian, ZEND_ACC_PUBLIC)
    PHP_ME(ProjDatum, getRemarks, arginfo_datum_getRemarks, ZEND_ACC_PUBLIC)
    PHP_ME(ProjDatum, getScope, arginfo_datum_getScope, ZEND_ACC_PUBLIC)
    PHP_ME(ProjDatum, isExactSame, arginfo_datum_isExactSame, ZEND_ACC_PUBLIC)
    PHP_ME(ProjDatum, toJson, arginfo_datum_toJson, ZEND_ACC_PUBLIC)
    PHP_ME(ProjDatum, toWkt, arginfo_datum_toWkt, ZEND_ACC_PUBLIC)
    PHP_ME(ProjDatum, __toString, arginfo_datum_toString, ZEND_ACC_PUBLIC)
    PHP_FE_END
};

/* Initialize ProjDatum class */
void proj_datum_init(void)
{
    zend_class_entry ce;
    
    INIT_CLASS_ENTRY(ce, "ProjDatum", proj_datum_methods);
    proj_datum_ce = zend_register_internal_class(&ce);
    proj_datum_ce->create_object = proj_datum_object_create;
    
    memcpy(&proj_datum_object_handlers, &std_object_handlers, sizeof(zend_object_handlers));
    proj_datum_object_handlers.free_obj = proj_datum_object_destroy;
    proj_datum_object_handlers.offset = XtOffsetOf(proj_datum_object, std);
}

/* Object creation */
zend_object *proj_datum_object_create(zend_class_entry *ce)
{
    proj_datum_object *intern = ecalloc(1, sizeof(proj_datum_object) + zend_object_properties_size(ce));
    
    zend_object_std_init(&intern->std, ce);
    object_properties_init(&intern->std, ce);
    
    intern->pj = NULL;
    intern->std.handlers = &proj_datum_object_handlers;
    
    return &intern->std;
}

/* Object destruction */
void proj_datum_object_destroy(zend_object *object)
{
    proj_datum_object *intern = DATUM_FROM_OBJECT(object);
    
    if (intern->pj) {
        proj_destroy(intern->pj);
        intern->pj = NULL;
    }
    
    zend_object_std_dtor(&intern->std);
}

/* Constructor */
static PHP_METHOD(ProjDatum, __construct)
{
    zval *datum_params;
    char *datum_string = NULL;
    PJ_CONTEXT *ctx;
    PJ *pj;
    proj_datum_object *intern;

    ZEND_PARSE_PARAMETERS_START(1, 1)
        Z_PARAM_ZVAL(datum_params)
    ZEND_PARSE_PARAMETERS_END();

    intern = DATUM_FROM_OBJECT(Z_OBJ_P(getThis()));

    if (!proj_parse_crs_input(datum_params, &datum_string)) {
        proj_throw_exception(proj_crs_exception_ce, "Invalid datum input");
        return;
    }

    ctx = proj_get_default_context();
    pj = proj_create(ctx, datum_string);
    
    if (!pj) {
        efree(datum_string);
        proj_throw_crs_error(ctx);
        return;
    }

    /* Ensure it's a datum (geodetic reference frame) */
    PJ_TYPE pj_type = proj_get_type(pj);
    if (pj_type != PJ_TYPE_GEODETIC_REFERENCE_FRAME && 
        pj_type != PJ_TYPE_VERTICAL_REFERENCE_FRAME &&
        pj_type != PJ_TYPE_DYNAMIC_GEODETIC_REFERENCE_FRAME &&
        pj_type != PJ_TYPE_DYNAMIC_VERTICAL_REFERENCE_FRAME) {
        proj_destroy(pj);
        efree(datum_string);
        proj_throw_exception(proj_crs_exception_ce, "Input is not a valid datum");
        return;
    }

    intern->pj = pj;
    efree(datum_string);
}

/* Static method: fromAuthority */
static PHP_METHOD(ProjDatum, fromAuthority)
{
    zend_string *auth_name;
    zend_long code;
    char *datum_string;
    PJ_CONTEXT *ctx;
    PJ *pj;
    
    ZEND_PARSE_PARAMETERS_START(2, 2)
        Z_PARAM_STR(auth_name)
        Z_PARAM_LONG(code)
    ZEND_PARSE_PARAMETERS_END();

    spprintf(&datum_string, 0, "%s:%ld", ZSTR_VAL(auth_name), code);
    
    ctx = proj_get_default_context();
    pj = proj_create(ctx, datum_string);
    efree(datum_string);
    
    if (!pj) {
        proj_throw_crs_error(ctx);
        return;
    }

    PJ_TYPE pj_type = proj_get_type(pj);
    if (pj_type != PJ_TYPE_GEODETIC_REFERENCE_FRAME && 
        pj_type != PJ_TYPE_VERTICAL_REFERENCE_FRAME &&
        pj_type != PJ_TYPE_DYNAMIC_GEODETIC_REFERENCE_FRAME &&
        pj_type != PJ_TYPE_DYNAMIC_VERTICAL_REFERENCE_FRAME) {
        proj_destroy(pj);
        proj_throw_exception(proj_crs_exception_ce, "%s:%ld is not a valid datum", ZSTR_VAL(auth_name), code);
        return;
    }

    object_init_ex(return_value, proj_datum_ce);
    proj_datum_object *intern = DATUM_FROM_OBJECT(Z_OBJ_P(return_value));
    intern->pj = pj;
}

/* Static method: fromEpsg */
static PHP_METHOD(ProjDatum, fromEpsg)
{
    zend_long epsg_code;
    char *datum_string;
    PJ_CONTEXT *ctx;
    PJ *pj;
    
    ZEND_PARSE_PARAMETERS_START(1, 1)
        Z_PARAM_LONG(epsg_code)
    ZEND_PARSE_PARAMETERS_END();

    spprintf(&datum_string, 0, "EPSG:%ld", epsg_code);
    
    ctx = proj_get_default_context();
    pj = proj_create(ctx, datum_string);
    efree(datum_string);
    
    if (!pj) {
        proj_throw_crs_error(ctx);
        return;
    }

    PJ_TYPE pj_type = proj_get_type(pj);
    if (pj_type != PJ_TYPE_GEODETIC_REFERENCE_FRAME && 
        pj_type != PJ_TYPE_VERTICAL_REFERENCE_FRAME &&
        pj_type != PJ_TYPE_DYNAMIC_GEODETIC_REFERENCE_FRAME &&
        pj_type != PJ_TYPE_DYNAMIC_VERTICAL_REFERENCE_FRAME) {
        proj_destroy(pj);
        proj_throw_exception(proj_crs_exception_ce, "EPSG:%ld is not a valid datum", epsg_code);
        return;
    }

    object_init_ex(return_value, proj_datum_ce);
    proj_datum_object *intern = DATUM_FROM_OBJECT(Z_OBJ_P(return_value));
    intern->pj = pj;
}

/* Get name */
static PHP_METHOD(ProjDatum, getName)
{
    proj_datum_object *intern;
    const char *name;

    ZEND_PARSE_PARAMETERS_NONE();

    intern = DATUM_FROM_OBJECT(Z_OBJ_P(getThis()));
    if (!intern->pj) {
        RETURN_NULL();
    }

    name = proj_get_name(intern->pj);
    
    if (!name) {
        RETURN_NULL();
    }
    
    RETURN_STRING(name);
}

/* Get type name */
static PHP_METHOD(ProjDatum, getTypeName)
{
    proj_datum_object *intern;
    PJ_TYPE datum_type;

    ZEND_PARSE_PARAMETERS_NONE();

    intern = DATUM_FROM_OBJECT(Z_OBJ_P(getThis()));
    if (!intern->pj) {
        RETURN_NULL();
    }

    datum_type = proj_get_type(intern->pj);
    
    switch (datum_type) {
        case PJ_TYPE_GEODETIC_REFERENCE_FRAME:
            RETURN_STRING("Geodetic Reference Frame");
        case PJ_TYPE_VERTICAL_REFERENCE_FRAME:
            RETURN_STRING("Vertical Reference Frame");
        case PJ_TYPE_DYNAMIC_GEODETIC_REFERENCE_FRAME:
            RETURN_STRING("Dynamic Geodetic Reference Frame");
        case PJ_TYPE_DYNAMIC_VERTICAL_REFERENCE_FRAME:
            RETURN_STRING("Dynamic Vertical Reference Frame");
        default:
            RETURN_STRING("Unknown Datum");
    }
}

/* Get ellipsoid */
static PHP_METHOD(ProjDatum, getEllipsoid)
{
    proj_datum_object *intern;
    PJ_CONTEXT *ctx;
    PJ *ellipsoid;

    ZEND_PARSE_PARAMETERS_NONE();

    intern = DATUM_FROM_OBJECT(Z_OBJ_P(getThis()));
    if (!intern->pj) {
        RETURN_NULL();
    }

    ctx = proj_get_default_context();
    ellipsoid = proj_get_ellipsoid(ctx, intern->pj);
    
    if (!ellipsoid) {
        RETURN_NULL();
    }

    object_init_ex(return_value, proj_ellipsoid_ce);
    proj_ellipsoid_object *ellipsoid_intern = ELLIPSOID_FROM_OBJECT(Z_OBJ_P(return_value));
    ellipsoid_intern->pj = ellipsoid;
}

/* Get prime meridian */
static PHP_METHOD(ProjDatum, getPrimeMeridian)
{
    proj_datum_object *intern;
    PJ_CONTEXT *ctx;
    PJ *prime_meridian;

    ZEND_PARSE_PARAMETERS_NONE();

    intern = DATUM_FROM_OBJECT(Z_OBJ_P(getThis()));
    if (!intern->pj) {
        RETURN_NULL();
    }

    ctx = proj_get_default_context();
    prime_meridian = proj_get_prime_meridian(ctx, intern->pj);
    
    if (!prime_meridian) {
        RETURN_NULL();
    }

    object_init_ex(return_value, proj_prime_meridian_ce);
    proj_prime_meridian_object *pm_intern = PRIME_MERIDIAN_FROM_OBJECT(Z_OBJ_P(return_value));
    pm_intern->pj = prime_meridian;
}

/* Get remarks */
static PHP_METHOD(ProjDatum, getRemarks)
{
    proj_datum_object *intern;
    
    ZEND_PARSE_PARAMETERS_NONE();

    intern = DATUM_FROM_OBJECT(Z_OBJ_P(getThis()));
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
static PHP_METHOD(ProjDatum, getScope)
{
    proj_datum_object *intern;
    
    ZEND_PARSE_PARAMETERS_NONE();

    intern = DATUM_FROM_OBJECT(Z_OBJ_P(getThis()));
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
static PHP_METHOD(ProjDatum, toWkt)
{
    char *version = "WKT2_2019";
    size_t version_len;
    zend_bool pretty = 0;
    proj_datum_object *intern;
    PJ_CONTEXT *ctx;
    const char *wkt;
    PJ_WKT_TYPE wkt_type;

    ZEND_PARSE_PARAMETERS_START(0, 2)
        Z_PARAM_OPTIONAL
        Z_PARAM_STRING(version, version_len)
        Z_PARAM_BOOL(pretty)
    ZEND_PARSE_PARAMETERS_END();

    intern = DATUM_FROM_OBJECT(Z_OBJ_P(getThis()));
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
static PHP_METHOD(ProjDatum, toJson)
{
    zend_bool pretty = 0;
    proj_datum_object *intern;
    PJ_CONTEXT *ctx;
    const char *json;

    ZEND_PARSE_PARAMETERS_START(0, 1)
        Z_PARAM_OPTIONAL
        Z_PARAM_BOOL(pretty)
    ZEND_PARSE_PARAMETERS_END();

    intern = DATUM_FROM_OBJECT(Z_OBJ_P(getThis()));
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
static PHP_METHOD(ProjDatum, __toString)
{
    proj_datum_object *intern;
    const char *name;

    ZEND_PARSE_PARAMETERS_NONE();

    intern = DATUM_FROM_OBJECT(Z_OBJ_P(getThis()));
    if (!intern->pj) {
        proj_throw_exception(proj_crs_exception_ce, "Datum object is not initialized");
        return;
    }

    name = proj_get_name(intern->pj);
    
    if (!name) {
        RETURN_STRING("Unknown Datum");
    }
    
    RETURN_STRING(name);
}

/* Additional factory methods */
static PHP_METHOD(ProjDatum, fromString)
{
    zend_string *datum_string;
    PJ_CONTEXT *ctx;
    PJ *pj;
    
    ZEND_PARSE_PARAMETERS_START(1, 1)
        Z_PARAM_STR(datum_string)
    ZEND_PARSE_PARAMETERS_END();

    ctx = proj_get_default_context();
    pj = proj_create(ctx, ZSTR_VAL(datum_string));
    
    if (!pj) {
        proj_throw_crs_error(ctx);
        return;
    }

    PJ_TYPE pj_type = proj_get_type(pj);
    if (pj_type != PJ_TYPE_GEODETIC_REFERENCE_FRAME && 
        pj_type != PJ_TYPE_VERTICAL_REFERENCE_FRAME &&
        pj_type != PJ_TYPE_DYNAMIC_GEODETIC_REFERENCE_FRAME &&
        pj_type != PJ_TYPE_DYNAMIC_VERTICAL_REFERENCE_FRAME) {
        proj_destroy(pj);
        proj_throw_exception(proj_crs_exception_ce, "String is not a valid datum");
        return;
    }

    object_init_ex(return_value, proj_datum_ce);
    proj_datum_object *intern = DATUM_FROM_OBJECT(Z_OBJ_P(return_value));
    intern->pj = pj;
}

static PHP_METHOD(ProjDatum, fromUserInput)
{
    zval *value;
    char *datum_string = NULL;
    PJ_CONTEXT *ctx;
    PJ *pj;
    
    ZEND_PARSE_PARAMETERS_START(1, 1)
        Z_PARAM_ZVAL(value)
    ZEND_PARSE_PARAMETERS_END();

    if (!proj_parse_crs_input(value, &datum_string)) {
        proj_throw_exception(proj_crs_exception_ce, "Invalid datum input");
        return;
    }

    ctx = proj_get_default_context();
    pj = proj_create(ctx, datum_string);
    
    if (!pj) {
        efree(datum_string);
        proj_throw_crs_error(ctx);
        return;
    }

    PJ_TYPE pj_type = proj_get_type(pj);
    if (pj_type != PJ_TYPE_GEODETIC_REFERENCE_FRAME && 
        pj_type != PJ_TYPE_VERTICAL_REFERENCE_FRAME &&
        pj_type != PJ_TYPE_DYNAMIC_GEODETIC_REFERENCE_FRAME &&
        pj_type != PJ_TYPE_DYNAMIC_VERTICAL_REFERENCE_FRAME) {
        proj_destroy(pj);
        efree(datum_string);
        proj_throw_exception(proj_crs_exception_ce, "Input is not a valid datum");
        return;
    }

    object_init_ex(return_value, proj_datum_ce);
    proj_datum_object *intern = DATUM_FROM_OBJECT(Z_OBJ_P(return_value));
    intern->pj = pj;
    
    efree(datum_string);
}

static PHP_METHOD(ProjDatum, fromJson)
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

    PJ_TYPE pj_type = proj_get_type(pj);
    if (pj_type != PJ_TYPE_GEODETIC_REFERENCE_FRAME && 
        pj_type != PJ_TYPE_VERTICAL_REFERENCE_FRAME &&
        pj_type != PJ_TYPE_DYNAMIC_GEODETIC_REFERENCE_FRAME &&
        pj_type != PJ_TYPE_DYNAMIC_VERTICAL_REFERENCE_FRAME) {
        proj_destroy(pj);
        proj_throw_exception(proj_crs_exception_ce, "JSON is not a valid datum");
        return;
    }

    object_init_ex(return_value, proj_datum_ce);
    proj_datum_object *intern = DATUM_FROM_OBJECT(Z_OBJ_P(return_value));
    intern->pj = pj;
}

static PHP_METHOD(ProjDatum, isExactSame)
{
    zval *other;
    proj_datum_object *intern, *other_intern;

    ZEND_PARSE_PARAMETERS_START(1, 1)
        Z_PARAM_OBJECT_OF_CLASS(other, proj_datum_ce)
    ZEND_PARSE_PARAMETERS_END();

    intern = DATUM_FROM_OBJECT(Z_OBJ_P(getThis()));
    other_intern = DATUM_FROM_OBJECT(Z_OBJ_P(other));

    if (!intern->pj || !other_intern->pj) {
        RETURN_FALSE;
    }

    RETURN_BOOL(proj_is_equivalent_to(intern->pj, other_intern->pj, PJ_COMP_EQUIVALENT));
}

