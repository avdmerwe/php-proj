#include "../php_proj.h"

/* Class entries */
zend_class_entry *proj_crs_ce;
extern zend_class_entry *proj_crs_exception_ce;
extern zend_class_entry *proj_coordinate_system_ce;
extern zend_class_entry *proj_datum_ce;
extern zend_class_entry *proj_ellipsoid_ce;
extern zend_class_entry *proj_prime_meridian_ce;
extern zend_class_entry *proj_area_of_use_ce;
extern zend_class_entry *proj_coordinate_operation_ce;
extern zend_class_entry *proj_axis_ce;
extern zend_class_entry *proj_factors_ce;

zend_object_handlers proj_crs_object_handlers;

/* Method declarations */
static PHP_METHOD(ProjCRS, __construct);
static PHP_METHOD(ProjCRS, fromEpsg);
static PHP_METHOD(ProjCRS, fromString);
static PHP_METHOD(ProjCRS, fromUserInput);
static PHP_METHOD(ProjCRS, fromWkt);
static PHP_METHOD(ProjCRS, fromProj4);
static PHP_METHOD(ProjCRS, fromJson);
static PHP_METHOD(ProjCRS, fromAuthority);
static PHP_METHOD(ProjCRS, getName);
static PHP_METHOD(ProjCRS, getTypeName);
static PHP_METHOD(ProjCRS, getAxisInfo);
static PHP_METHOD(ProjCRS, getAreaOfUse);
static PHP_METHOD(ProjCRS, getToEpsg);
static PHP_METHOD(ProjCRS, getCoordinateSystem);
static PHP_METHOD(ProjCRS, getCoordinateOperation);
static PHP_METHOD(ProjCRS, getDatum);
static PHP_METHOD(ProjCRS, getEllipsoid);
static PHP_METHOD(ProjCRS, getPrimeMeridian);
static PHP_METHOD(ProjCRS, getGeodeticCrs);
static PHP_METHOD(ProjCRS, getSourceCrs);
static PHP_METHOD(ProjCRS, getTargetCrs);
static PHP_METHOD(ProjCRS, getSubCrsList);
static PHP_METHOD(ProjCRS, getRemarks);
static PHP_METHOD(ProjCRS, getScope);
static PHP_METHOD(ProjCRS, getToAuthority);
static PHP_METHOD(ProjCRS, isGeographic);
static PHP_METHOD(ProjCRS, isProjected);
static PHP_METHOD(ProjCRS, isGeocentric);
static PHP_METHOD(ProjCRS, isCompound);
static PHP_METHOD(ProjCRS, isEngineering);
static PHP_METHOD(ProjCRS, isVertical);
static PHP_METHOD(ProjCRS, isBound);
static PHP_METHOD(ProjCRS, isDerived);
static PHP_METHOD(ProjCRS, toWkt);
static PHP_METHOD(ProjCRS, toProj4);
static PHP_METHOD(ProjCRS, toJson);
static PHP_METHOD(ProjCRS, equals);
static PHP_METHOD(ProjCRS, isExactSame);
static PHP_METHOD(ProjCRS, getFactors);
static PHP_METHOD(ProjCRS, __toString);

/* ArgInfo declarations */
ZEND_BEGIN_ARG_INFO_EX(arginfo_crs_construct, 0, 0, 1)
    ZEND_ARG_INFO(0, crs)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_crs_fromEpsg, 0, 1, ProjCRS, 0)
    ZEND_ARG_TYPE_INFO(0, epsg_code, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_crs_fromString, 0, 1, ProjCRS, 0)
    ZEND_ARG_TYPE_INFO(0, crs_string, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_crs_fromUserInput, 0, 1, ProjCRS, 0)
    ZEND_ARG_INFO(0, value)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_crs_fromWkt, 0, 1, ProjCRS, 0)
    ZEND_ARG_TYPE_INFO(0, wkt, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_crs_fromProj4, 0, 1, ProjCRS, 0)
    ZEND_ARG_TYPE_INFO(0, proj4, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_crs_fromJson, 0, 1, ProjCRS, 0)
    ZEND_ARG_TYPE_INFO(0, json, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_crs_fromAuthority, 0, 2, ProjCRS, 0)
    ZEND_ARG_TYPE_INFO(0, authority, IS_STRING, 0)
    ZEND_ARG_TYPE_INFO(0, code, IS_LONG, 0)
ZEND_END_ARG_INFO()


ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_crs_getName, 0, 0, IS_STRING, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_crs_getTypeName, 0, 0, IS_STRING, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_crs_getAxisInfo, 0, 0, IS_ARRAY, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_crs_getAreaOfUse, 0, 0, ProjAreaOfUse, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_crs_getToEpsg, 0, 0, IS_LONG, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_crs_getCoordinateSystem, 0, 0, ProjCoordinateSystem, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_crs_getCoordinateOperation, 0, 0, ProjCoordinateOperation, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_crs_getDatum, 0, 0, ProjDatum, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_crs_getEllipsoid, 0, 0, ProjEllipsoid, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_crs_getPrimeMeridian, 0, 0, ProjPrimeMeridian, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_crs_getGeodeticCrs, 0, 0, ProjCRS, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_crs_getSourceCrs, 0, 0, ProjCRS, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_crs_getTargetCrs, 0, 0, ProjCRS, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_crs_getSubCrsList, 0, 0, IS_ARRAY, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_crs_getRemarks, 0, 0, IS_STRING, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_crs_getScope, 0, 0, IS_STRING, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_crs_getToAuthority, 0, 0, IS_ARRAY, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_crs_isGeographic, 0, 0, _IS_BOOL, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_crs_isProjected, 0, 0, _IS_BOOL, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_crs_isGeocentric, 0, 0, _IS_BOOL, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_crs_isCompound, 0, 0, _IS_BOOL, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_crs_isEngineering, 0, 0, _IS_BOOL, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_crs_isVertical, 0, 0, _IS_BOOL, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_crs_isBound, 0, 0, _IS_BOOL, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_crs_isDerived, 0, 0, _IS_BOOL, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_crs_toWkt, 0, 0, IS_STRING, 1)
    ZEND_ARG_TYPE_INFO(0, version, IS_STRING, 0)
    ZEND_ARG_TYPE_INFO(0, pretty, _IS_BOOL, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_crs_toProj4, 0, 0, IS_STRING, 1)
    ZEND_ARG_TYPE_INFO(0, version, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_crs_toJson, 0, 0, IS_STRING, 1)
    ZEND_ARG_TYPE_INFO(0, pretty, _IS_BOOL, 0)
ZEND_END_ARG_INFO()



ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_crs_equals, 0, 1, _IS_BOOL, 0)
    ZEND_ARG_OBJ_INFO(0, other, ProjCRS, 0)
    ZEND_ARG_TYPE_INFO(0, ignore_axis_order, _IS_BOOL, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_crs_isExactSame, 0, 1, _IS_BOOL, 0)
    ZEND_ARG_OBJ_INFO(0, other, ProjCRS, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_crs_getFactors, 0, 2, ProjFactors, 1)
    ZEND_ARG_TYPE_INFO(0, longitude, IS_DOUBLE, 0)
    ZEND_ARG_TYPE_INFO(0, latitude, IS_DOUBLE, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_crs_toString, 0, 0, IS_STRING, 0)
ZEND_END_ARG_INFO()

/* Method table */
static const zend_function_entry proj_crs_methods[] = {
    PHP_ME(ProjCRS, __construct, arginfo_crs_construct, ZEND_ACC_PUBLIC)
    PHP_ME(ProjCRS, fromEpsg, arginfo_crs_fromEpsg, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC)
    PHP_ME(ProjCRS, fromString, arginfo_crs_fromString, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC)
    PHP_ME(ProjCRS, fromUserInput, arginfo_crs_fromUserInput, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC)
    PHP_ME(ProjCRS, fromWkt, arginfo_crs_fromWkt, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC)
    PHP_ME(ProjCRS, fromProj4, arginfo_crs_fromProj4, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC)
    PHP_ME(ProjCRS, fromJson, arginfo_crs_fromJson, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC)
    PHP_ME(ProjCRS, fromAuthority, arginfo_crs_fromAuthority, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC)
    PHP_ME(ProjCRS, getName, arginfo_crs_getName, ZEND_ACC_PUBLIC)
    PHP_ME(ProjCRS, getTypeName, arginfo_crs_getTypeName, ZEND_ACC_PUBLIC)
    PHP_ME(ProjCRS, getAxisInfo, arginfo_crs_getAxisInfo, ZEND_ACC_PUBLIC)
    PHP_ME(ProjCRS, getAreaOfUse, arginfo_crs_getAreaOfUse, ZEND_ACC_PUBLIC)
    PHP_ME(ProjCRS, getToEpsg, arginfo_crs_getToEpsg, ZEND_ACC_PUBLIC)
    PHP_ME(ProjCRS, getCoordinateSystem, arginfo_crs_getCoordinateSystem, ZEND_ACC_PUBLIC)
    PHP_ME(ProjCRS, getCoordinateOperation, arginfo_crs_getCoordinateOperation, ZEND_ACC_PUBLIC)
    PHP_ME(ProjCRS, getDatum, arginfo_crs_getDatum, ZEND_ACC_PUBLIC)
    PHP_ME(ProjCRS, getEllipsoid, arginfo_crs_getEllipsoid, ZEND_ACC_PUBLIC)
    PHP_ME(ProjCRS, getPrimeMeridian, arginfo_crs_getPrimeMeridian, ZEND_ACC_PUBLIC)
    PHP_ME(ProjCRS, getGeodeticCrs, arginfo_crs_getGeodeticCrs, ZEND_ACC_PUBLIC)
    PHP_ME(ProjCRS, getSourceCrs, arginfo_crs_getSourceCrs, ZEND_ACC_PUBLIC)
    PHP_ME(ProjCRS, getTargetCrs, arginfo_crs_getTargetCrs, ZEND_ACC_PUBLIC)
    PHP_ME(ProjCRS, getSubCrsList, arginfo_crs_getSubCrsList, ZEND_ACC_PUBLIC)
    PHP_ME(ProjCRS, getRemarks, arginfo_crs_getRemarks, ZEND_ACC_PUBLIC)
    PHP_ME(ProjCRS, getScope, arginfo_crs_getScope, ZEND_ACC_PUBLIC)
    PHP_ME(ProjCRS, getToAuthority, arginfo_crs_getToAuthority, ZEND_ACC_PUBLIC)
    PHP_ME(ProjCRS, isGeographic, arginfo_crs_isGeographic, ZEND_ACC_PUBLIC)
    PHP_ME(ProjCRS, isProjected, arginfo_crs_isProjected, ZEND_ACC_PUBLIC)
    PHP_ME(ProjCRS, isGeocentric, arginfo_crs_isGeocentric, ZEND_ACC_PUBLIC)
    PHP_ME(ProjCRS, isCompound, arginfo_crs_isCompound, ZEND_ACC_PUBLIC)
    PHP_ME(ProjCRS, isEngineering, arginfo_crs_isEngineering, ZEND_ACC_PUBLIC)
    PHP_ME(ProjCRS, isVertical, arginfo_crs_isVertical, ZEND_ACC_PUBLIC)
    PHP_ME(ProjCRS, isBound, arginfo_crs_isBound, ZEND_ACC_PUBLIC)
    PHP_ME(ProjCRS, isDerived, arginfo_crs_isDerived, ZEND_ACC_PUBLIC)
    PHP_ME(ProjCRS, toWkt, arginfo_crs_toWkt, ZEND_ACC_PUBLIC)
    PHP_ME(ProjCRS, toProj4, arginfo_crs_toProj4, ZEND_ACC_PUBLIC)
    PHP_ME(ProjCRS, toJson, arginfo_crs_toJson, ZEND_ACC_PUBLIC)
    PHP_ME(ProjCRS, equals, arginfo_crs_equals, ZEND_ACC_PUBLIC)
    PHP_ME(ProjCRS, isExactSame, arginfo_crs_isExactSame, ZEND_ACC_PUBLIC)
    PHP_ME(ProjCRS, getFactors, arginfo_crs_getFactors, ZEND_ACC_PUBLIC)
    PHP_ME(ProjCRS, __toString, arginfo_crs_toString, ZEND_ACC_PUBLIC)
    PHP_FE_END
};

/* Initialize CRS class */
void proj_crs_init(void)
{
    zend_class_entry ce;
    
    INIT_CLASS_ENTRY(ce, "ProjCRS", proj_crs_methods);
    proj_crs_ce = zend_register_internal_class(&ce);
    proj_crs_ce->create_object = proj_crs_object_create;
    
    memcpy(&proj_crs_object_handlers, &std_object_handlers, sizeof(zend_object_handlers));
    proj_crs_object_handlers.free_obj = proj_crs_object_destroy;
    proj_crs_object_handlers.offset = XtOffsetOf(proj_crs_object, std);
}

/* Object creation */
zend_object *proj_crs_object_create(zend_class_entry *ce)
{
    proj_crs_object *intern = ecalloc(1, sizeof(proj_crs_object) + zend_object_properties_size(ce));
    
    zend_object_std_init(&intern->std, ce);
    object_properties_init(&intern->std, ce);
    
    intern->pj = NULL;
    intern->std.handlers = &proj_crs_object_handlers;
    
    return &intern->std;
}

/* Object destruction */
void proj_crs_object_destroy(zend_object *object)
{
    proj_crs_object *intern = CRS_FROM_OBJECT(object);
    
    if (intern->pj) {
        proj_destroy(intern->pj);
        intern->pj = NULL;
    }
    
    zend_object_std_dtor(&intern->std);
}

/* Constructor */
static PHP_METHOD(ProjCRS, __construct)
{
    zval *projparams;
    char *crs_string = NULL;
    PJ_CONTEXT *ctx;
    PJ *pj;
    proj_crs_object *intern;

    ZEND_PARSE_PARAMETERS_START(1, 1)
        Z_PARAM_ZVAL(projparams)
    ZEND_PARSE_PARAMETERS_END();

    intern = CRS_FROM_OBJECT(Z_OBJ_P(getThis()));

    if (!proj_parse_crs_input(projparams, &crs_string)) {
        proj_throw_exception(proj_crs_exception_ce, "Invalid CRS input");
        return;
    }

    ctx = proj_get_default_context();
    pj = proj_create(ctx, crs_string);
    
    if (!pj) {
        efree(crs_string);
        proj_throw_crs_error(ctx);
        return;
    }

    /* Ensure it's a CRS */
    if (!proj_is_crs(pj)) {
        proj_destroy(pj);
        efree(crs_string);
        proj_throw_exception(proj_crs_exception_ce, "Input is not a valid CRS");
        return;
    }

    intern->pj = pj;
    efree(crs_string);
}

/* Static method: fromEpsg */
static PHP_METHOD(ProjCRS, fromEpsg)
{
    zend_long epsg_code;
    char *crs_string;
    PJ_CONTEXT *ctx;
    PJ *pj;
    
    ZEND_PARSE_PARAMETERS_START(1, 1)
        Z_PARAM_LONG(epsg_code)
    ZEND_PARSE_PARAMETERS_END();

    spprintf(&crs_string, 0, "EPSG:%ld", epsg_code);
    
    ctx = proj_get_default_context();
    pj = proj_create(ctx, crs_string);
    efree(crs_string);
    
    if (!pj) {
        proj_throw_crs_error(ctx);
        return;
    }

    if (!proj_is_crs(pj)) {
        proj_destroy(pj);
        proj_throw_exception(proj_crs_exception_ce, "EPSG:%ld is not a valid CRS", epsg_code);
        return;
    }

    object_init_ex(return_value, proj_crs_ce);
    proj_crs_object *intern = CRS_FROM_OBJECT(Z_OBJ_P(return_value));
    intern->pj = pj;
}

/* Static method: fromString */
static PHP_METHOD(ProjCRS, fromString)
{
    char *crs_string;
    size_t crs_string_len;
    PJ_CONTEXT *ctx;
    PJ *pj;
    
    ZEND_PARSE_PARAMETERS_START(1, 1)
        Z_PARAM_STRING(crs_string, crs_string_len)
    ZEND_PARSE_PARAMETERS_END();

    ctx = proj_get_default_context();
    pj = proj_create(ctx, crs_string);
    
    if (!pj) {
        proj_throw_crs_error(ctx);
        return;
    }

    if (!proj_is_crs(pj)) {
        proj_destroy(pj);
        proj_throw_exception(proj_crs_exception_ce, "String is not a valid CRS");
        return;
    }

    object_init_ex(return_value, proj_crs_ce);
    proj_crs_object *intern = CRS_FROM_OBJECT(Z_OBJ_P(return_value));
    intern->pj = pj;
}

/* Static method: fromUserInput */
static PHP_METHOD(ProjCRS, fromUserInput)
{
    zval *value;
    char *crs_string = NULL;
    PJ_CONTEXT *ctx;
    PJ *pj;
    
    ZEND_PARSE_PARAMETERS_START(1, 1)
        Z_PARAM_ZVAL(value)
    ZEND_PARSE_PARAMETERS_END();

    if (!proj_parse_crs_input(value, &crs_string)) {
        proj_throw_exception(proj_crs_exception_ce, "Invalid CRS input");
        return;
    }

    ctx = proj_get_default_context();
    pj = proj_create(ctx, crs_string);
    
    if (!pj) {
        efree(crs_string);
        proj_throw_crs_error(ctx);
        return;
    }

    if (!proj_is_crs(pj)) {
        proj_destroy(pj);
        efree(crs_string);
        proj_throw_exception(proj_crs_exception_ce, "Input is not a valid CRS");
        return;
    }

    object_init_ex(return_value, proj_crs_ce);
    proj_crs_object *intern = CRS_FROM_OBJECT(Z_OBJ_P(return_value));
    intern->pj = pj;
    
    efree(crs_string);
}

/* Get name */
static PHP_METHOD(ProjCRS, getName)
{
    proj_crs_object *intern;
    PJ_CONTEXT *ctx;
    const char *name;

    ZEND_PARSE_PARAMETERS_NONE();

    intern = CRS_FROM_OBJECT(Z_OBJ_P(getThis()));
    if (!intern->pj) {
        RETURN_NULL();
    }

    ctx = proj_get_default_context();
    name = proj_get_name(intern->pj);
    
    if (!name) {
        proj_throw_exception(proj_crs_exception_ce, "Failed to get CRS name");
        return;
    }
    
    RETURN_STRING(name);
}

/* Get type name */
static PHP_METHOD(ProjCRS, getTypeName)
{
    proj_crs_object *intern;
    PJ_TYPE type;

    ZEND_PARSE_PARAMETERS_NONE();

    intern = CRS_FROM_OBJECT(Z_OBJ_P(getThis()));
    if (!intern->pj) {
        RETURN_NULL();
    }

    type = proj_get_type(intern->pj);
    
    switch (type) {
        case PJ_TYPE_GEOGRAPHIC_2D_CRS:
            RETURN_STRING("Geographic 2D CRS");
        case PJ_TYPE_GEOGRAPHIC_3D_CRS:
            RETURN_STRING("Geographic 3D CRS");
        case PJ_TYPE_GEOCENTRIC_CRS:
            RETURN_STRING("Geocentric CRS");
        case PJ_TYPE_PROJECTED_CRS:
            RETURN_STRING("Projected CRS");
        case PJ_TYPE_VERTICAL_CRS:
            RETURN_STRING("Vertical CRS");
        case PJ_TYPE_COMPOUND_CRS:
            RETURN_STRING("Compound CRS");
        case PJ_TYPE_BOUND_CRS:
            RETURN_STRING("Bound CRS");
        default:
            RETURN_STRING("Unknown CRS");
    }
}

/* Check if geographic */
static PHP_METHOD(ProjCRS, isGeographic)
{
    proj_crs_object *intern;
    PJ_TYPE type;

    ZEND_PARSE_PARAMETERS_NONE();

    intern = CRS_FROM_OBJECT(Z_OBJ_P(getThis()));
    if (!intern->pj) {
        RETURN_FALSE;
    }

    type = proj_get_type(intern->pj);
    RETURN_BOOL(type == PJ_TYPE_GEOGRAPHIC_2D_CRS || type == PJ_TYPE_GEOGRAPHIC_3D_CRS);
}

/* Check if projected */
static PHP_METHOD(ProjCRS, isProjected)
{
    proj_crs_object *intern;
    PJ_TYPE type;

    ZEND_PARSE_PARAMETERS_NONE();

    intern = CRS_FROM_OBJECT(Z_OBJ_P(getThis()));
    if (!intern->pj) {
        RETURN_FALSE;
    }

    type = proj_get_type(intern->pj);
    RETURN_BOOL(type == PJ_TYPE_PROJECTED_CRS);
}

/* Check if geocentric */
static PHP_METHOD(ProjCRS, isGeocentric)
{
    proj_crs_object *intern;
    PJ_TYPE type;

    ZEND_PARSE_PARAMETERS_NONE();

    intern = CRS_FROM_OBJECT(Z_OBJ_P(getThis()));
    if (!intern->pj) {
        RETURN_FALSE;
    }

    type = proj_get_type(intern->pj);
    RETURN_BOOL(type == PJ_TYPE_GEOCENTRIC_CRS);
}

/* Convert to WKT */
static PHP_METHOD(ProjCRS, toWkt)
{
    char *version = "WKT2_2019";
    size_t version_len;
    zend_bool pretty = 0;
    proj_crs_object *intern;
    PJ_CONTEXT *ctx;
    const char *wkt;
    PJ_WKT_TYPE wkt_type;

    ZEND_PARSE_PARAMETERS_START(0, 2)
        Z_PARAM_OPTIONAL
        Z_PARAM_STRING(version, version_len)
        Z_PARAM_BOOL(pretty)
    ZEND_PARSE_PARAMETERS_END();

    intern = CRS_FROM_OBJECT(Z_OBJ_P(getThis()));
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

/* Convert to PROJ4 */
static PHP_METHOD(ProjCRS, toProj4)
{
    char *version = "PROJ_5";
    size_t version_len;
    proj_crs_object *intern;
    PJ_CONTEXT *ctx;
    const char *proj4;

    ZEND_PARSE_PARAMETERS_START(0, 1)
        Z_PARAM_OPTIONAL
        Z_PARAM_STRING(version, version_len)
    ZEND_PARSE_PARAMETERS_END();

    intern = CRS_FROM_OBJECT(Z_OBJ_P(getThis()));
    if (!intern->pj) {
        RETURN_NULL();
    }

    ctx = proj_get_default_context();
    proj4 = proj_as_proj_string(ctx, intern->pj, PJ_PROJ_4, NULL);
    
    if (proj4) {
        RETURN_STRING(proj4);
    } else {
        proj_throw_crs_error(ctx);
        RETURN_NULL();
    }
}

/* Convert to JSON */
static PHP_METHOD(ProjCRS, toJson)
{
    zend_bool pretty = 0;
    proj_crs_object *intern;
    PJ_CONTEXT *ctx;
    const char *json;

    ZEND_PARSE_PARAMETERS_START(0, 1)
        Z_PARAM_OPTIONAL
        Z_PARAM_BOOL(pretty)
    ZEND_PARSE_PARAMETERS_END();

    intern = CRS_FROM_OBJECT(Z_OBJ_P(getThis()));
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

/* Equals comparison */
static PHP_METHOD(ProjCRS, equals)
{
    zval *other;
    zend_bool ignore_axis_order = 0;
    proj_crs_object *intern, *other_intern;

    ZEND_PARSE_PARAMETERS_START(1, 2)
        Z_PARAM_OBJECT_OF_CLASS(other, proj_crs_ce)
        Z_PARAM_OPTIONAL
        Z_PARAM_BOOL(ignore_axis_order)
    ZEND_PARSE_PARAMETERS_END();

    intern = CRS_FROM_OBJECT(Z_OBJ_P(getThis()));
    other_intern = CRS_FROM_OBJECT(Z_OBJ_P(other));

    if (!intern->pj || !other_intern->pj) {
        RETURN_FALSE;
    }

    /* Use PROJ's comparison function */
    RETURN_BOOL(proj_is_equivalent_to(intern->pj, other_intern->pj, PJ_COMP_STRICT));
}

/* String representation */
static PHP_METHOD(ProjCRS, __toString)
{
    proj_crs_object *intern;
    PJ_CONTEXT *ctx;
    const char *wkt;

    ZEND_PARSE_PARAMETERS_NONE();

    intern = CRS_FROM_OBJECT(Z_OBJ_P(getThis()));
    if (!intern->pj) {
        proj_throw_exception(proj_crs_exception_ce, "CRS object is not initialized");
        return;
    }

    ctx = proj_get_default_context();
    wkt = proj_as_wkt(ctx, intern->pj, PJ_WKT2_2019_SIMPLIFIED, NULL);
    
    if (!wkt) {
        proj_throw_crs_error(ctx);
        return;
    }
    
    RETURN_STRING(wkt);
}

/* Static method: fromWkt */
static PHP_METHOD(ProjCRS, fromWkt)
{
    zend_string *wkt_string;
    PJ_CONTEXT *ctx;
    PJ *pj;
    
    ZEND_PARSE_PARAMETERS_START(1, 1)
        Z_PARAM_STR(wkt_string)
    ZEND_PARSE_PARAMETERS_END();

    ctx = proj_get_default_context();
    pj = proj_create(ctx, ZSTR_VAL(wkt_string));
    
    if (!pj) {
        proj_throw_crs_error(ctx);
        return;
    }

    if (!proj_is_crs(pj)) {
        proj_destroy(pj);
        proj_throw_exception(proj_crs_exception_ce, "Provided WKT is not a valid CRS");
        return;
    }

    object_init_ex(return_value, proj_crs_ce);
    proj_crs_object *intern = CRS_FROM_OBJECT(Z_OBJ_P(return_value));
    intern->pj = pj;
}

/* Static method: fromProj4 */
static PHP_METHOD(ProjCRS, fromProj4)
{
    zend_string *proj4_string;
    PJ_CONTEXT *ctx;
    PJ *pj;
    
    ZEND_PARSE_PARAMETERS_START(1, 1)
        Z_PARAM_STR(proj4_string)
    ZEND_PARSE_PARAMETERS_END();

    ctx = proj_get_default_context();
    pj = proj_create(ctx, ZSTR_VAL(proj4_string));
    
    if (!pj) {
        proj_throw_crs_error(ctx);
        return;
    }

    if (!proj_is_crs(pj)) {
        proj_destroy(pj);
        proj_throw_exception(proj_crs_exception_ce, "Provided PROJ4 string is not a valid CRS");
        return;
    }

    object_init_ex(return_value, proj_crs_ce);
    proj_crs_object *intern = CRS_FROM_OBJECT(Z_OBJ_P(return_value));
    intern->pj = pj;
}

/* Static method: fromJson */
static PHP_METHOD(ProjCRS, fromJson)
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

    if (!proj_is_crs(pj)) {
        proj_destroy(pj);
        proj_throw_exception(proj_crs_exception_ce, "Provided JSON is not a valid CRS");
        return;
    }

    object_init_ex(return_value, proj_crs_ce);
    proj_crs_object *intern = CRS_FROM_OBJECT(Z_OBJ_P(return_value));
    intern->pj = pj;
}

/* Static method: fromAuthority */
static PHP_METHOD(ProjCRS, fromAuthority)
{
    zend_string *auth_name;
    zend_long code;
    char *crs_string;
    PJ_CONTEXT *ctx;
    PJ *pj;
    
    ZEND_PARSE_PARAMETERS_START(2, 2)
        Z_PARAM_STR(auth_name)
        Z_PARAM_LONG(code)
    ZEND_PARSE_PARAMETERS_END();

    spprintf(&crs_string, 0, "%s:%ld", ZSTR_VAL(auth_name), code);
    
    ctx = proj_get_default_context();
    pj = proj_create(ctx, crs_string);
    efree(crs_string);
    
    if (!pj) {
        proj_throw_crs_error(ctx);
        return;
    }

    if (!proj_is_crs(pj)) {
        proj_destroy(pj);
        proj_throw_exception(proj_crs_exception_ce, "%s:%ld is not a valid CRS", ZSTR_VAL(auth_name), code);
        return;
    }

    object_init_ex(return_value, proj_crs_ce);
    proj_crs_object *intern = CRS_FROM_OBJECT(Z_OBJ_P(return_value));
    intern->pj = pj;
}

/* Method: getAxisInfo */
static PHP_METHOD(ProjCRS, getAxisInfo)
{
    proj_crs_object *intern;
    PJ_CONTEXT *ctx;
    
    if (zend_parse_parameters_none() == FAILURE) {
        return;
    }

    intern = CRS_FROM_OBJECT(Z_OBJ_P(ZEND_THIS));
    if (!intern->pj) {
        proj_throw_exception(proj_crs_exception_ce, "CRS object is not initialized");
        return;
    }

    ctx = proj_get_default_context();
    
    /* Get coordinate system from CRS */
    PJ *cs = proj_crs_get_coordinate_system(ctx, intern->pj);
    if (!cs) {
        RETURN_NULL();
    }

    int axis_count = proj_cs_get_axis_count(ctx, cs);
    if (axis_count <= 0) {
        proj_destroy(cs);
        RETURN_NULL();
    }

    array_init(return_value);
    
    for (int i = 0; i < axis_count; i++) {
        const char *name, *abbrev, *direction, *unit_name;
        double unit_conv_factor;
        
        if (proj_cs_get_axis_info(ctx, cs, i, &name, &abbrev, &direction, 
                                 &unit_conv_factor, &unit_name, NULL, NULL)) {
            /* Create ProjAxis object */
            zval axis_obj;
            object_init_ex(&axis_obj, proj_axis_ce);
            proj_axis_object *axis_intern = AXIS_FROM_OBJECT(Z_OBJ(axis_obj));
            
            /* Set axis properties */
            axis_intern->name = estrdup(name ? name : "");
            axis_intern->abbrev = estrdup(abbrev ? abbrev : "");
            axis_intern->direction = estrdup(direction ? direction : "");
            axis_intern->unit_name = estrdup(unit_name ? unit_name : "");
            axis_intern->unit_auth_code = NULL;
            axis_intern->unit_code = NULL;
            axis_intern->unit_conversion_factor = unit_conv_factor;
            
            add_next_index_zval(return_value, &axis_obj);
        }
    }
    
    proj_destroy(cs);
}

/* Method: getAreaOfUse */
static PHP_METHOD(ProjCRS, getAreaOfUse)
{
    proj_crs_object *intern;
    PJ_CONTEXT *ctx;
    
    if (zend_parse_parameters_none() == FAILURE) {
        return;
    }

    intern = CRS_FROM_OBJECT(Z_OBJ_P(ZEND_THIS));
    if (!intern->pj) {
        proj_throw_exception(proj_crs_exception_ce, "CRS object is not initialized");
        return;
    }

    ctx = proj_get_default_context();
    
    double west_lon, south_lat, east_lon, north_lat;
    const char *area_name;
    
    if (proj_get_area_of_use(ctx, intern->pj, &west_lon, &south_lat, &east_lon, 
                            &north_lat, &area_name)) {
        
        /* Create ProjAreaOfUse object */
        object_init_ex(return_value, proj_area_of_use_ce);
        proj_area_of_use_object *area_intern = AREA_OF_USE_FROM_OBJECT(Z_OBJ_P(return_value));
        
        /* Set the area properties */
        area_intern->west = west_lon;
        area_intern->south = south_lat;
        area_intern->east = east_lon;
        area_intern->north = north_lat;
        
        if (area_name) {
            area_intern->name = estrdup(area_name);
        }
    } else {
        RETURN_NULL();
    }
}

/* Method: getToEpsg */
static PHP_METHOD(ProjCRS, getToEpsg)
{
    proj_crs_object *intern;
    PJ_CONTEXT *ctx;
    
    if (zend_parse_parameters_none() == FAILURE) {
        return;
    }

    intern = CRS_FROM_OBJECT(Z_OBJ_P(ZEND_THIS));
    if (!intern->pj) {
        proj_throw_exception(proj_crs_exception_ce, "CRS object is not initialized");
        return;
    }

    ctx = proj_get_default_context();
    
    const char *auth_name = NULL;
    const char *code = NULL;
    
    if (proj_get_id_auth_name(intern->pj, 0) && proj_get_id_code(intern->pj, 0)) {
        auth_name = proj_get_id_auth_name(intern->pj, 0);
        code = proj_get_id_code(intern->pj, 0);
        
        if (auth_name && code && strcmp(auth_name, "EPSG") == 0) {
            RETURN_LONG(atol(code));
        }
    }
    
    RETURN_NULL();
}

/* Method: getCoordinateSystem */
static PHP_METHOD(ProjCRS, getCoordinateSystem)
{
    proj_crs_object *intern;
    PJ_CONTEXT *ctx;
    
    ZEND_PARSE_PARAMETERS_NONE();

    intern = CRS_FROM_OBJECT(Z_OBJ_P(getThis()));
    if (!intern->pj) {
        RETURN_NULL();
    }

    ctx = proj_get_default_context();
    
    PJ *cs = proj_crs_get_coordinate_system(ctx, intern->pj);
    if (!cs) {
        RETURN_NULL();
    }

    /* Create ProjCoordinateSystem object */
    object_init_ex(return_value, proj_coordinate_system_ce);
    proj_coordinate_system_object *cs_intern = COORDINATE_SYSTEM_FROM_OBJECT(Z_OBJ_P(return_value));
    cs_intern->pj = cs;
}

/* Method: getCoordinateOperation */
static PHP_METHOD(ProjCRS, getCoordinateOperation)
{
    proj_crs_object *intern;
    PJ_CONTEXT *ctx;
    
    ZEND_PARSE_PARAMETERS_NONE();

    intern = CRS_FROM_OBJECT(Z_OBJ_P(getThis()));
    if (!intern->pj) {
        RETURN_NULL();
    }

    ctx = proj_get_default_context();
    PJ_TYPE type = proj_get_type(intern->pj);
    
    /* Only projected CRS have coordinate operations */
    if (type != PJ_TYPE_PROJECTED_CRS) {
        RETURN_NULL();
    }
    
    PJ *conversion = proj_crs_get_coordoperation(ctx, intern->pj);
    if (!conversion) {
        RETURN_NULL();
    }

    /* Create ProjCoordinateOperation object */
    object_init_ex(return_value, proj_coordinate_operation_ce);
    proj_coordinate_operation_object *coord_op_intern = COORDINATE_OPERATION_FROM_OBJECT(Z_OBJ_P(return_value));
    coord_op_intern->pj = conversion;
}

/* Method: getDatum */
static PHP_METHOD(ProjCRS, getDatum)
{
    proj_crs_object *intern;
    PJ_CONTEXT *ctx;
    
    ZEND_PARSE_PARAMETERS_NONE();

    intern = CRS_FROM_OBJECT(Z_OBJ_P(getThis()));
    if (!intern->pj) {
        RETURN_NULL();
    }

    ctx = proj_get_default_context();
    
    PJ *datum = proj_crs_get_datum(ctx, intern->pj);
    if (!datum) {
        /* Try to get datum from geodetic CRS for projected CRS */
        PJ *geodetic_crs = proj_crs_get_geodetic_crs(ctx, intern->pj);
        if (geodetic_crs) {
            datum = proj_crs_get_datum(ctx, geodetic_crs);
            proj_destroy(geodetic_crs);
        }
        
        if (!datum) {
            RETURN_NULL();
        }
    }

    /* Create ProjDatum object */
    object_init_ex(return_value, proj_datum_ce);
    proj_datum_object *datum_intern = DATUM_FROM_OBJECT(Z_OBJ_P(return_value));
    datum_intern->pj = datum;
}

/* Method: getEllipsoid */
static PHP_METHOD(ProjCRS, getEllipsoid)
{
    proj_crs_object *intern;
    PJ_CONTEXT *ctx;
    
    ZEND_PARSE_PARAMETERS_NONE();

    intern = CRS_FROM_OBJECT(Z_OBJ_P(getThis()));
    if (!intern->pj) {
        RETURN_NULL();
    }

    ctx = proj_get_default_context();
    
    PJ *ellipsoid = proj_get_ellipsoid(ctx, intern->pj);
    if (!ellipsoid) {
        RETURN_NULL();
    }

    /* Create ProjEllipsoid object */
    object_init_ex(return_value, proj_ellipsoid_ce);
    proj_ellipsoid_object *ellipsoid_intern = ELLIPSOID_FROM_OBJECT(Z_OBJ_P(return_value));
    ellipsoid_intern->pj = ellipsoid;
}

/* Method: getPrimeMeridian */
static PHP_METHOD(ProjCRS, getPrimeMeridian)
{
    proj_crs_object *intern;
    PJ_CONTEXT *ctx;
    
    ZEND_PARSE_PARAMETERS_NONE();

    intern = CRS_FROM_OBJECT(Z_OBJ_P(getThis()));
    if (!intern->pj) {
        RETURN_NULL();
    }

    ctx = proj_get_default_context();
    
    PJ *prime_meridian = proj_get_prime_meridian(ctx, intern->pj);
    if (!prime_meridian) {
        RETURN_NULL();
    }

    /* Create ProjPrimeMeridian object */
    object_init_ex(return_value, proj_prime_meridian_ce);
    proj_prime_meridian_object *pm_intern = PRIME_MERIDIAN_FROM_OBJECT(Z_OBJ_P(return_value));
    pm_intern->pj = prime_meridian;
}

/* Method: getGeodeticCrs */
static PHP_METHOD(ProjCRS, getGeodeticCrs)
{
    proj_crs_object *intern;
    PJ_CONTEXT *ctx;
    
    ZEND_PARSE_PARAMETERS_NONE();

    intern = CRS_FROM_OBJECT(Z_OBJ_P(getThis()));
    if (!intern->pj) {
        RETURN_NULL();
    }

    ctx = proj_get_default_context();
    
    PJ *geodetic_crs = proj_crs_get_geodetic_crs(ctx, intern->pj);
    if (!geodetic_crs) {
        RETURN_NULL();
    }

    object_init_ex(return_value, proj_crs_ce);
    proj_crs_object *new_intern = CRS_FROM_OBJECT(Z_OBJ_P(return_value));
    new_intern->pj = geodetic_crs;
}

/* Method: getSourceCrs */
static PHP_METHOD(ProjCRS, getSourceCrs)
{
    proj_crs_object *intern;
    PJ_CONTEXT *ctx;
    
    ZEND_PARSE_PARAMETERS_NONE();

    intern = CRS_FROM_OBJECT(Z_OBJ_P(getThis()));
    if (!intern->pj) {
        RETURN_NULL();
    }

    ctx = proj_get_default_context();
    PJ_TYPE type = proj_get_type(intern->pj);
    
    /* Only bound CRS have source CRS */
    if (type != PJ_TYPE_BOUND_CRS) {
        RETURN_NULL();
    }
    
    PJ *source_crs = proj_get_source_crs(ctx, intern->pj);
    if (!source_crs) {
        RETURN_NULL();
    }

    object_init_ex(return_value, proj_crs_ce);
    proj_crs_object *new_intern = CRS_FROM_OBJECT(Z_OBJ_P(return_value));
    new_intern->pj = source_crs;
}

/* Method: getTargetCrs */
static PHP_METHOD(ProjCRS, getTargetCrs)
{
    proj_crs_object *intern;
    PJ_CONTEXT *ctx;
    
    ZEND_PARSE_PARAMETERS_NONE();

    intern = CRS_FROM_OBJECT(Z_OBJ_P(getThis()));
    if (!intern->pj) {
        RETURN_NULL();
    }

    ctx = proj_get_default_context();
    PJ_TYPE type = proj_get_type(intern->pj);
    
    /* Only bound CRS have target CRS */
    if (type != PJ_TYPE_BOUND_CRS) {
        RETURN_NULL();
    }
    
    PJ *target_crs = proj_get_target_crs(ctx, intern->pj);
    if (!target_crs) {
        RETURN_NULL();
    }

    object_init_ex(return_value, proj_crs_ce);
    proj_crs_object *new_intern = CRS_FROM_OBJECT(Z_OBJ_P(return_value));
    new_intern->pj = target_crs;
}

/* Method: getSubCrsList */
static PHP_METHOD(ProjCRS, getSubCrsList)
{
    proj_crs_object *intern;
    PJ_CONTEXT *ctx;
    
    ZEND_PARSE_PARAMETERS_NONE();

    intern = CRS_FROM_OBJECT(Z_OBJ_P(getThis()));
    if (!intern->pj) {
        array_init(return_value);
        return;
    }

    ctx = proj_get_default_context();
    PJ_TYPE type = proj_get_type(intern->pj);
    
    /* Only compound CRS have sub-CRS list */
    if (type != PJ_TYPE_COMPOUND_CRS) {
        array_init(return_value);
        return;
    }
    
    array_init(return_value);
    
    /* Compound CRS typically have 2 components (horizontal + vertical)
     * but we'll check up to a reasonable limit to be safe */
    for (int i = 0; i < 10; i++) {
        PJ *sub_crs = proj_crs_get_sub_crs(ctx, intern->pj, i);
        if (!sub_crs) {
            /* No more sub-CRS components */
            break;
        }
        
        zval sub_crs_obj;
        object_init_ex(&sub_crs_obj, proj_crs_ce);
        proj_crs_object *sub_intern = CRS_FROM_OBJECT(Z_OBJ(sub_crs_obj));
        sub_intern->pj = sub_crs;
        
        add_next_index_zval(return_value, &sub_crs_obj);
    }
}

/* Method: getRemarks */
static PHP_METHOD(ProjCRS, getRemarks)
{
    proj_crs_object *intern;
    
    ZEND_PARSE_PARAMETERS_NONE();

    intern = CRS_FROM_OBJECT(Z_OBJ_P(getThis()));
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

/* Method: getScope */
static PHP_METHOD(ProjCRS, getScope)
{
    proj_crs_object *intern;
    
    ZEND_PARSE_PARAMETERS_NONE();

    intern = CRS_FROM_OBJECT(Z_OBJ_P(getThis()));
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

/* Method: getToAuthority */
static PHP_METHOD(ProjCRS, getToAuthority)
{
    proj_crs_object *intern;
    
    ZEND_PARSE_PARAMETERS_NONE();

    intern = CRS_FROM_OBJECT(Z_OBJ_P(getThis()));
    if (!intern->pj) {
        RETURN_NULL();
    }

    const char *auth_name = proj_get_id_auth_name(intern->pj, 0);
    const char *code = proj_get_id_code(intern->pj, 0);
    
    if (auth_name && code) {
        array_init(return_value);
        add_next_index_string(return_value, auth_name);
        add_next_index_string(return_value, code);
    } else {
        RETURN_NULL();
    }
}

/* Method: isCompound */
static PHP_METHOD(ProjCRS, isCompound)
{
    proj_crs_object *intern;
    PJ_TYPE type;

    ZEND_PARSE_PARAMETERS_NONE();

    intern = CRS_FROM_OBJECT(Z_OBJ_P(getThis()));
    if (!intern->pj) {
        RETURN_FALSE;
    }

    type = proj_get_type(intern->pj);
    RETURN_BOOL(type == PJ_TYPE_COMPOUND_CRS);
}

/* Method: isEngineering */
static PHP_METHOD(ProjCRS, isEngineering)
{
    proj_crs_object *intern;
    PJ_TYPE type;

    ZEND_PARSE_PARAMETERS_NONE();

    intern = CRS_FROM_OBJECT(Z_OBJ_P(getThis()));
    if (!intern->pj) {
        RETURN_FALSE;
    }

    type = proj_get_type(intern->pj);
    RETURN_BOOL(type == PJ_TYPE_ENGINEERING_CRS);
}

/* Method: isVertical */
static PHP_METHOD(ProjCRS, isVertical)
{
    proj_crs_object *intern;
    PJ_TYPE type;

    ZEND_PARSE_PARAMETERS_NONE();

    intern = CRS_FROM_OBJECT(Z_OBJ_P(getThis()));
    if (!intern->pj) {
        RETURN_FALSE;
    }

    type = proj_get_type(intern->pj);
    RETURN_BOOL(type == PJ_TYPE_VERTICAL_CRS);
}

/* Method: isBound */
static PHP_METHOD(ProjCRS, isBound)
{
    proj_crs_object *intern;
    PJ_TYPE type;

    ZEND_PARSE_PARAMETERS_NONE();

    intern = CRS_FROM_OBJECT(Z_OBJ_P(getThis()));
    if (!intern->pj) {
        RETURN_FALSE;
    }

    type = proj_get_type(intern->pj);
    RETURN_BOOL(type == PJ_TYPE_BOUND_CRS);
}

/* Method: isDerived */
static PHP_METHOD(ProjCRS, isDerived)
{
    proj_crs_object *intern;
    PJ_TYPE type;

    ZEND_PARSE_PARAMETERS_NONE();

    intern = CRS_FROM_OBJECT(Z_OBJ_P(getThis()));
    if (!intern->pj) {
        RETURN_FALSE;
    }

    PJ_CONTEXT *ctx = proj_get_default_context();
    RETURN_BOOL(proj_crs_is_derived(ctx, intern->pj));
}

/* Method: isExactSame */
static PHP_METHOD(ProjCRS, isExactSame)
{
    zval *other;
    proj_crs_object *intern, *other_intern;

    ZEND_PARSE_PARAMETERS_START(1, 1)
        Z_PARAM_OBJECT_OF_CLASS(other, proj_crs_ce)
    ZEND_PARSE_PARAMETERS_END();

    intern = CRS_FROM_OBJECT(Z_OBJ_P(getThis()));
    other_intern = CRS_FROM_OBJECT(Z_OBJ_P(other));

    if (!intern->pj || !other_intern->pj) {
        RETURN_FALSE;
    }

    /* Use PROJ's exact comparison function */
    RETURN_BOOL(proj_is_equivalent_to(intern->pj, other_intern->pj, PJ_COMP_EQUIVALENT));
}

/* Method: getFactors */
static PHP_METHOD(ProjCRS, getFactors)
{
    double longitude, latitude;
    proj_crs_object *intern;
    PJ_CONTEXT *ctx;
    PJ_COORD coord;
    PJ_FACTORS factors;

    ZEND_PARSE_PARAMETERS_START(2, 2)
        Z_PARAM_DOUBLE(longitude)
        Z_PARAM_DOUBLE(latitude)
    ZEND_PARSE_PARAMETERS_END();

    intern = CRS_FROM_OBJECT(Z_OBJ_P(getThis()));
    if (!intern->pj) {
        RETURN_NULL();
    }

    ctx = proj_get_default_context();
    
    /* Convert degrees to radians for PROJ */
    coord = proj_coord(proj_torad(longitude), proj_torad(latitude), 0, 0);
    
    /* Get projection factors */
    factors = proj_factors(intern->pj, coord);
    
    /* Check if the calculation was successful */
    if (factors.meridional_scale == HUGE_VAL) {
        RETURN_NULL();
    }

    /* Create ProjFactors object */
    object_init_ex(return_value, proj_factors_ce);
    proj_factors_object *factors_intern = FACTORS_FROM_OBJECT(Z_OBJ_P(return_value));
    
    /* Set factor values */
    factors_intern->meridional_scale = factors.meridional_scale;
    factors_intern->parallel_scale = factors.parallel_scale;
    factors_intern->areal_scale = factors.areal_scale;
    factors_intern->angular_distortion = factors.angular_distortion;
    factors_intern->meridian_parallel_angle = factors.meridian_parallel_angle;
    factors_intern->meridian_convergence = factors.meridian_convergence;
    factors_intern->tissot_semimajor = factors.tissot_semimajor;
    factors_intern->tissot_semiminor = factors.tissot_semiminor;
    factors_intern->dx_dlam = factors.dx_dlam;
    factors_intern->dx_dphi = factors.dx_dphi;
    factors_intern->dy_dlam = factors.dy_dlam;
    factors_intern->dy_dphi = factors.dy_dphi;
}


