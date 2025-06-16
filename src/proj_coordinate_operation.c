#include "../php_proj.h"

/* Class entries */
zend_class_entry *proj_coordinate_operation_ce;
extern zend_class_entry *proj_crs_exception_ce;

zend_object_handlers proj_coordinate_operation_object_handlers;

/* Method declarations */
static PHP_METHOD(ProjCoordinateOperation, __construct);
static PHP_METHOD(ProjCoordinateOperation, fromAuthority);
static PHP_METHOD(ProjCoordinateOperation, fromEpsg);
static PHP_METHOD(ProjCoordinateOperation, fromJson);
static PHP_METHOD(ProjCoordinateOperation, fromString);
static PHP_METHOD(ProjCoordinateOperation, fromUserInput);
static PHP_METHOD(ProjCoordinateOperation, getName);
static PHP_METHOD(ProjCoordinateOperation, getMethodName);
static PHP_METHOD(ProjCoordinateOperation, getMethodAuthName);
static PHP_METHOD(ProjCoordinateOperation, getMethodCode);
static PHP_METHOD(ProjCoordinateOperation, getAccuracy);
static PHP_METHOD(ProjCoordinateOperation, isInstantiable);
static PHP_METHOD(ProjCoordinateOperation, getRemarks);
static PHP_METHOD(ProjCoordinateOperation, getScope);
static PHP_METHOD(ProjCoordinateOperation, isExactSame);
static PHP_METHOD(ProjCoordinateOperation, toJson);
static PHP_METHOD(ProjCoordinateOperation, toWkt);
static PHP_METHOD(ProjCoordinateOperation, __toString);

/* ArgInfo declarations */
ZEND_BEGIN_ARG_INFO_EX(arginfo_coordinate_operation_construct, 0, 0, 1)
    ZEND_ARG_INFO(0, operation_params)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_coordinate_operation_fromAuthority, 0, 2, ProjCoordinateOperation, 0)
    ZEND_ARG_TYPE_INFO(0, auth_name, IS_STRING, 0)
    ZEND_ARG_TYPE_INFO(0, code, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_coordinate_operation_fromEpsg, 0, 1, ProjCoordinateOperation, 0)
    ZEND_ARG_TYPE_INFO(0, code, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_coordinate_operation_fromJson, 0, 1, ProjCoordinateOperation, 0)
    ZEND_ARG_TYPE_INFO(0, json_str, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_coordinate_operation_fromString, 0, 1, ProjCoordinateOperation, 0)
    ZEND_ARG_TYPE_INFO(0, string, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_coordinate_operation_fromUserInput, 0, 1, ProjCoordinateOperation, 0)
    ZEND_ARG_INFO(0, value)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_coordinate_operation_getName, 0, 0, IS_STRING, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_coordinate_operation_getMethodName, 0, 0, IS_STRING, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_coordinate_operation_getMethodAuthName, 0, 0, IS_STRING, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_coordinate_operation_getMethodCode, 0, 0, IS_STRING, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_coordinate_operation_getAccuracy, 0, 0, IS_DOUBLE, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_coordinate_operation_isInstantiable, 0, 0, _IS_BOOL, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_coordinate_operation_getRemarks, 0, 0, IS_STRING, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_coordinate_operation_getScope, 0, 0, IS_STRING, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_coordinate_operation_isExactSame, 0, 1, _IS_BOOL, 0)
    ZEND_ARG_OBJ_INFO(0, other, ProjCoordinateOperation, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_coordinate_operation_toJson, 0, 0, IS_STRING, 1)
    ZEND_ARG_TYPE_INFO(0, pretty, _IS_BOOL, 0)
ZEND_END_ARG_INFO()


ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_coordinate_operation_toWkt, 0, 0, IS_STRING, 1)
    ZEND_ARG_TYPE_INFO(0, version, IS_STRING, 0)
    ZEND_ARG_TYPE_INFO(0, pretty, _IS_BOOL, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_coordinate_operation_toString, 0, 0, IS_STRING, 0)
ZEND_END_ARG_INFO()

/* Method table */
static const zend_function_entry proj_coordinate_operation_methods[] = {
    PHP_ME(ProjCoordinateOperation, __construct, arginfo_coordinate_operation_construct, ZEND_ACC_PUBLIC)
    PHP_ME(ProjCoordinateOperation, fromAuthority, arginfo_coordinate_operation_fromAuthority, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC)
    PHP_ME(ProjCoordinateOperation, fromEpsg, arginfo_coordinate_operation_fromEpsg, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC)
    PHP_ME(ProjCoordinateOperation, fromJson, arginfo_coordinate_operation_fromJson, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC)
    PHP_ME(ProjCoordinateOperation, fromString, arginfo_coordinate_operation_fromString, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC)
    PHP_ME(ProjCoordinateOperation, fromUserInput, arginfo_coordinate_operation_fromUserInput, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC)
    PHP_ME(ProjCoordinateOperation, getName, arginfo_coordinate_operation_getName, ZEND_ACC_PUBLIC)
    PHP_ME(ProjCoordinateOperation, getMethodName, arginfo_coordinate_operation_getMethodName, ZEND_ACC_PUBLIC)
    PHP_ME(ProjCoordinateOperation, getMethodAuthName, arginfo_coordinate_operation_getMethodAuthName, ZEND_ACC_PUBLIC)
    PHP_ME(ProjCoordinateOperation, getMethodCode, arginfo_coordinate_operation_getMethodCode, ZEND_ACC_PUBLIC)
    PHP_ME(ProjCoordinateOperation, getAccuracy, arginfo_coordinate_operation_getAccuracy, ZEND_ACC_PUBLIC)
    PHP_ME(ProjCoordinateOperation, isInstantiable, arginfo_coordinate_operation_isInstantiable, ZEND_ACC_PUBLIC)
    PHP_ME(ProjCoordinateOperation, getRemarks, arginfo_coordinate_operation_getRemarks, ZEND_ACC_PUBLIC)
    PHP_ME(ProjCoordinateOperation, getScope, arginfo_coordinate_operation_getScope, ZEND_ACC_PUBLIC)
    PHP_ME(ProjCoordinateOperation, isExactSame, arginfo_coordinate_operation_isExactSame, ZEND_ACC_PUBLIC)
    PHP_ME(ProjCoordinateOperation, toJson, arginfo_coordinate_operation_toJson, ZEND_ACC_PUBLIC)
    PHP_ME(ProjCoordinateOperation, toWkt, arginfo_coordinate_operation_toWkt, ZEND_ACC_PUBLIC)
    PHP_ME(ProjCoordinateOperation, __toString, arginfo_coordinate_operation_toString, ZEND_ACC_PUBLIC)
    PHP_FE_END
};

/* Initialize ProjCoordinateOperation class */
void proj_coordinate_operation_init(void)
{
    zend_class_entry ce;
    
    INIT_CLASS_ENTRY(ce, "ProjCoordinateOperation", proj_coordinate_operation_methods);
    proj_coordinate_operation_ce = zend_register_internal_class(&ce);
    proj_coordinate_operation_ce->create_object = proj_coordinate_operation_object_create;
    
    memcpy(&proj_coordinate_operation_object_handlers, &std_object_handlers, sizeof(zend_object_handlers));
    proj_coordinate_operation_object_handlers.free_obj = proj_coordinate_operation_object_destroy;
    proj_coordinate_operation_object_handlers.offset = XtOffsetOf(proj_coordinate_operation_object, std);
}

/* Object creation */
zend_object *proj_coordinate_operation_object_create(zend_class_entry *ce)
{
    proj_coordinate_operation_object *intern = ecalloc(1, sizeof(proj_coordinate_operation_object) + zend_object_properties_size(ce));
    
    zend_object_std_init(&intern->std, ce);
    object_properties_init(&intern->std, ce);
    
    intern->pj = NULL;
    intern->std.handlers = &proj_coordinate_operation_object_handlers;
    
    return &intern->std;
}

/* Object destruction */
void proj_coordinate_operation_object_destroy(zend_object *object)
{
    proj_coordinate_operation_object *intern = COORDINATE_OPERATION_FROM_OBJECT(object);
    
    if (intern->pj) {
        proj_destroy(intern->pj);
        intern->pj = NULL;
    }
    
    zend_object_std_dtor(&intern->std);
}

/* Constructor */
static PHP_METHOD(ProjCoordinateOperation, __construct)
{
    zval *operation_params;
    char *operation_string = NULL;
    PJ_CONTEXT *ctx;
    PJ *pj;
    proj_coordinate_operation_object *intern;

    ZEND_PARSE_PARAMETERS_START(1, 1)
        Z_PARAM_ZVAL(operation_params)
    ZEND_PARSE_PARAMETERS_END();

    intern = COORDINATE_OPERATION_FROM_OBJECT(Z_OBJ_P(getThis()));

    if (!proj_parse_crs_input(operation_params, &operation_string)) {
        proj_throw_exception(proj_crs_exception_ce, "Invalid coordinate operation input");
        return;
    }

    ctx = proj_get_default_context();
    pj = proj_create(ctx, operation_string);
    
    if (!pj) {
        efree(operation_string);
        proj_throw_crs_error(ctx);
        return;
    }

    /* Validate it's a coordinate operation */
    PJ_TYPE pj_type = proj_get_type(pj);
    if (pj_type != PJ_TYPE_CONVERSION && 
        pj_type != PJ_TYPE_TRANSFORMATION &&
        pj_type != PJ_TYPE_CONCATENATED_OPERATION &&
        pj_type != PJ_TYPE_OTHER_COORDINATE_OPERATION) {
        proj_destroy(pj);
        efree(operation_string);
        proj_throw_exception(proj_crs_exception_ce, "Input is not a valid coordinate operation");
        return;
    }

    intern->pj = pj;
    efree(operation_string);
}

/* Static method: fromAuthority */
static PHP_METHOD(ProjCoordinateOperation, fromAuthority)
{
    zend_string *auth_name;
    zend_long code;
    char *operation_string;
    PJ_CONTEXT *ctx;
    PJ *pj;
    
    ZEND_PARSE_PARAMETERS_START(2, 2)
        Z_PARAM_STR(auth_name)
        Z_PARAM_LONG(code)
    ZEND_PARSE_PARAMETERS_END();

    spprintf(&operation_string, 0, "%s:%ld", ZSTR_VAL(auth_name), code);
    
    ctx = proj_get_default_context();
    pj = proj_create(ctx, operation_string);
    efree(operation_string);
    
    if (!pj) {
        proj_throw_crs_error(ctx);
        return;
    }

    PJ_TYPE pj_type = proj_get_type(pj);
    if (pj_type != PJ_TYPE_CONVERSION && 
        pj_type != PJ_TYPE_TRANSFORMATION &&
        pj_type != PJ_TYPE_CONCATENATED_OPERATION &&
        pj_type != PJ_TYPE_OTHER_COORDINATE_OPERATION) {
        proj_destroy(pj);
        proj_throw_exception(proj_crs_exception_ce, "%s:%ld is not a valid coordinate operation", ZSTR_VAL(auth_name), code);
        return;
    }

    object_init_ex(return_value, proj_coordinate_operation_ce);
    proj_coordinate_operation_object *intern = COORDINATE_OPERATION_FROM_OBJECT(Z_OBJ_P(return_value));
    intern->pj = pj;
}

/* Static method: fromEpsg */
static PHP_METHOD(ProjCoordinateOperation, fromEpsg)
{
    zend_long epsg_code;
    char *operation_string;
    PJ_CONTEXT *ctx;
    PJ *pj;
    
    ZEND_PARSE_PARAMETERS_START(1, 1)
        Z_PARAM_LONG(epsg_code)
    ZEND_PARSE_PARAMETERS_END();

    spprintf(&operation_string, 0, "EPSG:%ld", epsg_code);
    
    ctx = proj_get_default_context();
    pj = proj_create(ctx, operation_string);
    efree(operation_string);
    
    if (!pj) {
        proj_throw_crs_error(ctx);
        return;
    }

    PJ_TYPE pj_type = proj_get_type(pj);
    if (pj_type != PJ_TYPE_CONVERSION && 
        pj_type != PJ_TYPE_TRANSFORMATION &&
        pj_type != PJ_TYPE_CONCATENATED_OPERATION &&
        pj_type != PJ_TYPE_OTHER_COORDINATE_OPERATION) {
        proj_destroy(pj);
        proj_throw_exception(proj_crs_exception_ce, "EPSG:%ld is not a valid coordinate operation", epsg_code);
        return;
    }

    object_init_ex(return_value, proj_coordinate_operation_ce);
    proj_coordinate_operation_object *intern = COORDINATE_OPERATION_FROM_OBJECT(Z_OBJ_P(return_value));
    intern->pj = pj;
}

/* Get name */
static PHP_METHOD(ProjCoordinateOperation, getName)
{
    proj_coordinate_operation_object *intern;
    const char *name;

    ZEND_PARSE_PARAMETERS_NONE();

    intern = COORDINATE_OPERATION_FROM_OBJECT(Z_OBJ_P(getThis()));
    if (!intern->pj) {
        RETURN_NULL();
    }

    name = proj_get_name(intern->pj);
    
    if (!name) {
        RETURN_NULL();
    }
    
    RETURN_STRING(name);
}

/* Get method name */
static PHP_METHOD(ProjCoordinateOperation, getMethodName)
{
    proj_coordinate_operation_object *intern;
    PJ_CONTEXT *ctx;
    const char *method_name;

    ZEND_PARSE_PARAMETERS_NONE();

    intern = COORDINATE_OPERATION_FROM_OBJECT(Z_OBJ_P(getThis()));
    if (!intern->pj) {
        RETURN_NULL();
    }

    ctx = proj_get_default_context();
    if (proj_coordoperation_get_method_info(ctx, intern->pj, &method_name, NULL, NULL)) {
        if (method_name) {
            RETURN_STRING(method_name);
        }
    }
    
    RETURN_NULL();
}

/* Get method authority name */
static PHP_METHOD(ProjCoordinateOperation, getMethodAuthName)
{
    proj_coordinate_operation_object *intern;
    PJ_CONTEXT *ctx;
    const char *method_auth_name;

    ZEND_PARSE_PARAMETERS_NONE();

    intern = COORDINATE_OPERATION_FROM_OBJECT(Z_OBJ_P(getThis()));
    if (!intern->pj) {
        RETURN_NULL();
    }

    ctx = proj_get_default_context();
    if (proj_coordoperation_get_method_info(ctx, intern->pj, NULL, &method_auth_name, NULL)) {
        if (method_auth_name) {
            RETURN_STRING(method_auth_name);
        }
    }
    
    RETURN_NULL();
}

/* Get method code */
static PHP_METHOD(ProjCoordinateOperation, getMethodCode)
{
    proj_coordinate_operation_object *intern;
    PJ_CONTEXT *ctx;
    const char *method_code;

    ZEND_PARSE_PARAMETERS_NONE();

    intern = COORDINATE_OPERATION_FROM_OBJECT(Z_OBJ_P(getThis()));
    if (!intern->pj) {
        RETURN_NULL();
    }

    ctx = proj_get_default_context();
    if (proj_coordoperation_get_method_info(ctx, intern->pj, NULL, NULL, &method_code)) {
        if (method_code) {
            RETURN_STRING(method_code);
        }
    }
    
    RETURN_NULL();
}

/* Get accuracy */
static PHP_METHOD(ProjCoordinateOperation, getAccuracy)
{
    proj_coordinate_operation_object *intern;
    PJ_CONTEXT *ctx;
    double accuracy;

    ZEND_PARSE_PARAMETERS_NONE();

    intern = COORDINATE_OPERATION_FROM_OBJECT(Z_OBJ_P(getThis()));
    if (!intern->pj) {
        RETURN_NULL();
    }

    ctx = proj_get_default_context();
    accuracy = proj_coordoperation_get_accuracy(ctx, intern->pj);
    if (accuracy >= 0) {
        RETURN_DOUBLE(accuracy);
    }
    
    RETURN_NULL();
}

/* Check if instantiable */
static PHP_METHOD(ProjCoordinateOperation, isInstantiable)
{
    proj_coordinate_operation_object *intern;
    PJ_CONTEXT *ctx;

    ZEND_PARSE_PARAMETERS_NONE();

    intern = COORDINATE_OPERATION_FROM_OBJECT(Z_OBJ_P(getThis()));
    if (!intern->pj) {
        RETURN_FALSE;
    }

    ctx = proj_get_default_context();
    RETURN_BOOL(proj_coordoperation_is_instantiable(ctx, intern->pj));
}

/* Get remarks */
static PHP_METHOD(ProjCoordinateOperation, getRemarks)
{
    proj_coordinate_operation_object *intern;
    
    ZEND_PARSE_PARAMETERS_NONE();

    intern = COORDINATE_OPERATION_FROM_OBJECT(Z_OBJ_P(getThis()));
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
static PHP_METHOD(ProjCoordinateOperation, getScope)
{
    proj_coordinate_operation_object *intern;
    
    ZEND_PARSE_PARAMETERS_NONE();

    intern = COORDINATE_OPERATION_FROM_OBJECT(Z_OBJ_P(getThis()));
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
static PHP_METHOD(ProjCoordinateOperation, toWkt)
{
    char *version = "WKT2_2019";
    size_t version_len;
    zend_bool pretty = 0;
    proj_coordinate_operation_object *intern;
    PJ_CONTEXT *ctx;
    const char *wkt;
    PJ_WKT_TYPE wkt_type;

    ZEND_PARSE_PARAMETERS_START(0, 2)
        Z_PARAM_OPTIONAL
        Z_PARAM_STRING(version, version_len)
        Z_PARAM_BOOL(pretty)
    ZEND_PARSE_PARAMETERS_END();

    intern = COORDINATE_OPERATION_FROM_OBJECT(Z_OBJ_P(getThis()));
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
static PHP_METHOD(ProjCoordinateOperation, toJson)
{
    zend_bool pretty = 0;
    proj_coordinate_operation_object *intern;
    PJ_CONTEXT *ctx;
    const char *json;

    ZEND_PARSE_PARAMETERS_START(0, 1)
        Z_PARAM_OPTIONAL
        Z_PARAM_BOOL(pretty)
    ZEND_PARSE_PARAMETERS_END();

    intern = COORDINATE_OPERATION_FROM_OBJECT(Z_OBJ_P(getThis()));
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
static PHP_METHOD(ProjCoordinateOperation, __toString)
{
    proj_coordinate_operation_object *intern;
    const char *name;

    ZEND_PARSE_PARAMETERS_NONE();

    intern = COORDINATE_OPERATION_FROM_OBJECT(Z_OBJ_P(getThis()));
    if (!intern->pj) {
        proj_throw_exception(proj_crs_exception_ce, "Coordinate operation object is not initialized");
        return;
    }

    name = proj_get_name(intern->pj);
    
    if (!name) {
        RETURN_STRING("Unknown Coordinate Operation");
    }
    
    RETURN_STRING(name);
}

/* Additional factory methods */
static PHP_METHOD(ProjCoordinateOperation, fromString)
{
    zend_string *operation_string;
    PJ_CONTEXT *ctx;
    PJ *pj;
    
    ZEND_PARSE_PARAMETERS_START(1, 1)
        Z_PARAM_STR(operation_string)
    ZEND_PARSE_PARAMETERS_END();

    ctx = proj_get_default_context();
    pj = proj_create(ctx, ZSTR_VAL(operation_string));
    
    if (!pj) {
        proj_throw_crs_error(ctx);
        return;
    }

    PJ_TYPE pj_type = proj_get_type(pj);
    if (pj_type != PJ_TYPE_CONVERSION && 
        pj_type != PJ_TYPE_TRANSFORMATION &&
        pj_type != PJ_TYPE_CONCATENATED_OPERATION &&
        pj_type != PJ_TYPE_OTHER_COORDINATE_OPERATION) {
        proj_destroy(pj);
        proj_throw_exception(proj_crs_exception_ce, "String is not a valid coordinate operation");
        return;
    }

    object_init_ex(return_value, proj_coordinate_operation_ce);
    proj_coordinate_operation_object *intern = COORDINATE_OPERATION_FROM_OBJECT(Z_OBJ_P(return_value));
    intern->pj = pj;
}

static PHP_METHOD(ProjCoordinateOperation, fromUserInput)
{
    zval *value;
    char *operation_string = NULL;
    PJ_CONTEXT *ctx;
    PJ *pj;
    
    ZEND_PARSE_PARAMETERS_START(1, 1)
        Z_PARAM_ZVAL(value)
    ZEND_PARSE_PARAMETERS_END();

    if (!proj_parse_crs_input(value, &operation_string)) {
        proj_throw_exception(proj_crs_exception_ce, "Invalid coordinate operation input");
        return;
    }

    ctx = proj_get_default_context();
    pj = proj_create(ctx, operation_string);
    
    if (!pj) {
        efree(operation_string);
        proj_throw_crs_error(ctx);
        return;
    }

    PJ_TYPE pj_type = proj_get_type(pj);
    if (pj_type != PJ_TYPE_CONVERSION && 
        pj_type != PJ_TYPE_TRANSFORMATION &&
        pj_type != PJ_TYPE_CONCATENATED_OPERATION &&
        pj_type != PJ_TYPE_OTHER_COORDINATE_OPERATION) {
        proj_destroy(pj);
        efree(operation_string);
        proj_throw_exception(proj_crs_exception_ce, "Input is not a valid coordinate operation");
        return;
    }

    object_init_ex(return_value, proj_coordinate_operation_ce);
    proj_coordinate_operation_object *intern = COORDINATE_OPERATION_FROM_OBJECT(Z_OBJ_P(return_value));
    intern->pj = pj;
    
    efree(operation_string);
}

static PHP_METHOD(ProjCoordinateOperation, fromJson)
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
    if (pj_type != PJ_TYPE_CONVERSION && 
        pj_type != PJ_TYPE_TRANSFORMATION &&
        pj_type != PJ_TYPE_CONCATENATED_OPERATION &&
        pj_type != PJ_TYPE_OTHER_COORDINATE_OPERATION) {
        proj_destroy(pj);
        proj_throw_exception(proj_crs_exception_ce, "JSON is not a valid coordinate operation");
        return;
    }

    object_init_ex(return_value, proj_coordinate_operation_ce);
    proj_coordinate_operation_object *intern = COORDINATE_OPERATION_FROM_OBJECT(Z_OBJ_P(return_value));
    intern->pj = pj;
}

static PHP_METHOD(ProjCoordinateOperation, isExactSame)
{
    zval *other;
    proj_coordinate_operation_object *intern, *other_intern;

    ZEND_PARSE_PARAMETERS_START(1, 1)
        Z_PARAM_OBJECT_OF_CLASS(other, proj_coordinate_operation_ce)
    ZEND_PARSE_PARAMETERS_END();

    intern = COORDINATE_OPERATION_FROM_OBJECT(Z_OBJ_P(getThis()));
    other_intern = COORDINATE_OPERATION_FROM_OBJECT(Z_OBJ_P(other));

    if (!intern->pj || !other_intern->pj) {
        RETURN_FALSE;
    }

    RETURN_BOOL(proj_is_equivalent_to(intern->pj, other_intern->pj, PJ_COMP_EQUIVALENT));
}

