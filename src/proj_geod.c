#include "../php_proj.h"
#include <geodesic.h>
#include <strings.h>

/* Class entries */
zend_class_entry *proj_geod_ce;
extern zend_class_entry *proj_geod_exception_ce;
#include <math.h>

zend_object_handlers proj_geod_object_handlers;

/* Arginfo declarations for Geod methods */
ZEND_BEGIN_ARG_INFO_EX(arginfo_geod_construct, 0, 0, 2)
    ZEND_ARG_TYPE_INFO(0, a, IS_DOUBLE, 0)
    ZEND_ARG_TYPE_INFO(0, f, IS_DOUBLE, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_geod_fromCrs, 0, 1, ProjGeod, 0)
    ZEND_ARG_INFO(0, crs)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_geod_fromEpsg, 0, 1, ProjGeod, 0)
    ZEND_ARG_TYPE_INFO(0, epsg_code, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_geod_fromEllipsoidName, 0, 1, ProjGeod, 0)
    ZEND_ARG_TYPE_INFO(0, ellipsoid_name, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_geod_fromParameters, 0, 2, ProjGeod, 0)
    ZEND_ARG_TYPE_INFO(0, a, IS_DOUBLE, 0)
    ZEND_ARG_INFO(0, param2)
    ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, param_name, IS_STRING, 0, "\"auto\"")
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_geod_getinitstring, 0, 0, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_geod_issphere, 0, 0, _IS_BOOL, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_geod_geta, 0, 0, IS_DOUBLE, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_geod_getb, 0, 0, IS_DOUBLE, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_geod_getes, 0, 0, IS_DOUBLE, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_geod_getf, 0, 0, IS_DOUBLE, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_geod_fwd, 0, 3, IS_ARRAY, 0)
    ZEND_ARG_INFO(0, points)
    ZEND_ARG_INFO(0, az)
    ZEND_ARG_INFO(0, dist)
    ZEND_ARG_TYPE_INFO(0, radians, _IS_BOOL, 0)
    ZEND_ARG_TYPE_INFO(0, return_back_azimuth, _IS_BOOL, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_geod_inv, 0, 2, IS_ARRAY, 0)
    ZEND_ARG_INFO(0, points1)
    ZEND_ARG_INFO(0, points2)
    ZEND_ARG_TYPE_INFO(0, radians, _IS_BOOL, 0)
    ZEND_ARG_TYPE_INFO(0, return_back_azimuth, _IS_BOOL, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_geod_npts, 0, 3, IS_ARRAY, 0)
    ZEND_ARG_INFO(0, point1)
    ZEND_ARG_INFO(0, point2)
    ZEND_ARG_TYPE_INFO(0, npts, IS_LONG, 0)
    ZEND_ARG_TYPE_INFO(0, radians, _IS_BOOL, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_geod_fwdintermediate, 0, 3, IS_ARRAY, 0)
    ZEND_ARG_INFO(0, point1)
    ZEND_ARG_TYPE_INFO(0, az, IS_DOUBLE, 0)
    ZEND_ARG_ARRAY_INFO(0, distances, 0)
    ZEND_ARG_TYPE_INFO(0, radians, _IS_BOOL, 0)
    ZEND_ARG_TYPE_INFO(0, return_back_azimuth, _IS_BOOL, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_geod_invintermediate, 0, 3, IS_ARRAY, 0)
    ZEND_ARG_INFO(0, point1)
    ZEND_ARG_INFO(0, point2)
    ZEND_ARG_TYPE_INFO(0, npts, IS_LONG, 0)
    ZEND_ARG_TYPE_INFO(0, radians, _IS_BOOL, 0)
    ZEND_ARG_TYPE_INFO(0, return_back_azimuth, _IS_BOOL, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_geod_polygonareaperimeter, 0, 1, IS_ARRAY, 0)
    ZEND_ARG_ARRAY_INFO(0, points, 0)
    ZEND_ARG_TYPE_INFO(0, radians, _IS_BOOL, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_geod_linelength, 0, 1, IS_DOUBLE, 0)
    ZEND_ARG_ARRAY_INFO(0, points, 0)
    ZEND_ARG_TYPE_INFO(0, radians, _IS_BOOL, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_geod_linelengths, 0, 1, IS_ARRAY, 0)
    ZEND_ARG_ARRAY_INFO(0, points, 0)
    ZEND_ARG_TYPE_INFO(0, radians, _IS_BOOL, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_geod_tostring, 0, 0, IS_STRING, 0)
ZEND_END_ARG_INFO()

/* Method declarations */
static PHP_METHOD(Geod, __construct);
static PHP_METHOD(Geod, fromCrs);
static PHP_METHOD(Geod, fromEpsg);
static PHP_METHOD(Geod, fromEllipsoidName);
static PHP_METHOD(Geod, fromParameters);
static PHP_METHOD(Geod, getInitstring);
static PHP_METHOD(Geod, isSphere);
static PHP_METHOD(Geod, getA);
static PHP_METHOD(Geod, getB);
static PHP_METHOD(Geod, getEs);
static PHP_METHOD(Geod, getF);
static PHP_METHOD(Geod, fwd);
static PHP_METHOD(Geod, inv);
static PHP_METHOD(Geod, npts);
static PHP_METHOD(Geod, fwdIntermediate);
static PHP_METHOD(Geod, invIntermediate);
static PHP_METHOD(Geod, polygonAreaPerimeter);
static PHP_METHOD(Geod, lineLength);
static PHP_METHOD(Geod, lineLengths);
static PHP_METHOD(Geod, __toString);

/* Method table */
static const zend_function_entry proj_geod_methods[] = {
    PHP_ME(Geod, __construct, arginfo_geod_construct, ZEND_ACC_PUBLIC)
    PHP_ME(Geod, fromCrs, arginfo_geod_fromCrs, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC)
    PHP_ME(Geod, fromEpsg, arginfo_geod_fromEpsg, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC)
    PHP_ME(Geod, fromEllipsoidName, arginfo_geod_fromEllipsoidName, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC)
    PHP_ME(Geod, fromParameters, arginfo_geod_fromParameters, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC)
    PHP_ME(Geod, getInitstring, arginfo_geod_getinitstring, ZEND_ACC_PUBLIC)
    PHP_ME(Geod, isSphere, arginfo_geod_issphere, ZEND_ACC_PUBLIC)
    PHP_ME(Geod, getA, arginfo_geod_geta, ZEND_ACC_PUBLIC)
    PHP_ME(Geod, getB, arginfo_geod_getb, ZEND_ACC_PUBLIC)
    PHP_ME(Geod, getEs, arginfo_geod_getes, ZEND_ACC_PUBLIC)
    PHP_ME(Geod, getF, arginfo_geod_getf, ZEND_ACC_PUBLIC)
    PHP_ME(Geod, fwd, arginfo_geod_fwd, ZEND_ACC_PUBLIC)
    PHP_ME(Geod, inv, arginfo_geod_inv, ZEND_ACC_PUBLIC)
    PHP_ME(Geod, npts, arginfo_geod_npts, ZEND_ACC_PUBLIC)
    PHP_ME(Geod, fwdIntermediate, arginfo_geod_fwdintermediate, ZEND_ACC_PUBLIC)
    PHP_ME(Geod, invIntermediate, arginfo_geod_invintermediate, ZEND_ACC_PUBLIC)
    PHP_ME(Geod, polygonAreaPerimeter, arginfo_geod_polygonareaperimeter, ZEND_ACC_PUBLIC)
    PHP_ME(Geod, lineLength, arginfo_geod_linelength, ZEND_ACC_PUBLIC)
    PHP_ME(Geod, lineLengths, arginfo_geod_linelengths, ZEND_ACC_PUBLIC)
    PHP_ME(Geod, __toString, arginfo_geod_tostring, ZEND_ACC_PUBLIC)
    PHP_FE_END
};

/* Initialize Geod class */
void proj_geod_init(void)
{
    zend_class_entry ce;
    
    INIT_CLASS_ENTRY(ce, "ProjGeod", proj_geod_methods);
    proj_geod_ce = zend_register_internal_class(&ce);
    proj_geod_ce->create_object = proj_geod_object_create;
    
    memcpy(&proj_geod_object_handlers, &std_object_handlers, sizeof(zend_object_handlers));
    proj_geod_object_handlers.free_obj = proj_geod_object_destroy;
    proj_geod_object_handlers.offset = XtOffsetOf(proj_geod_object, std);
}

/* Object creation */
zend_object *proj_geod_object_create(zend_class_entry *ce)
{
    proj_geod_object *intern = ecalloc(1, sizeof(proj_geod_object) + zend_object_properties_size(ce));
    
    zend_object_std_init(&intern->std, ce);
    object_properties_init(&intern->std, ce);
    
    intern->std.handlers = &proj_geod_object_handlers;
    
    return &intern->std;
}

/* Object destruction */
void proj_geod_object_destroy(zend_object *object)
{
    proj_geod_object *intern = GEOD_FROM_OBJECT(object);
    
    zend_object_std_dtor(&intern->std);
}

/* Constructor - now requires explicit ellipsoid parameters */
static PHP_METHOD(Geod, __construct)
{
    double a, f;
    proj_geod_object *intern;

    ZEND_PARSE_PARAMETERS_START(2, 2)
        Z_PARAM_DOUBLE(a)
        Z_PARAM_DOUBLE(f)
    ZEND_PARSE_PARAMETERS_END();

    if (a <= 0) {
        proj_throw_exception(proj_geod_exception_ce, "Semi-major axis must be positive");
        return;
    }
    
    if (f < 0 || f >= 1) {
        proj_throw_exception(proj_geod_exception_ce, "Flattening must be between 0 and 1");
        return;
    }

    intern = GEOD_FROM_OBJECT(Z_OBJ_P(getThis()));
    intern->a = a;
    intern->f = f;
    
    /* Initialize the PROJ geodesic structure with the ellipsoid parameters */
    geod_init(&intern->geod, intern->a, intern->f);
}

/* Static factory method: fromCrs */
static PHP_METHOD(Geod, fromCrs)
{
    zval *crs_input;
    PJ_CONTEXT *ctx;
    PJ *crs_pj, *ellipsoid;
    double ellipsoid_a, ellipsoid_b, ellipsoid_rf;
    int ellipsoid_is_semi_minor_computed;
    char *crs_string = NULL;
    
    ZEND_PARSE_PARAMETERS_START(1, 1)
        Z_PARAM_ZVAL(crs_input)
    ZEND_PARSE_PARAMETERS_END();

    /* Handle ProjCRS object */
    if (Z_TYPE_P(crs_input) == IS_OBJECT && instanceof_function(Z_OBJCE_P(crs_input), proj_crs_ce)) {
        proj_crs_object *crs_obj = CRS_FROM_OBJECT(Z_OBJ_P(crs_input));
        if (!crs_obj->pj) {
            proj_throw_exception(proj_geod_exception_ce, "Invalid CRS object");
            return;
        }
        crs_pj = proj_clone(proj_get_default_context(), crs_obj->pj);
    } else {
        /* Parse other input types */
        if (!proj_parse_crs_input(crs_input, &crs_string)) {
            proj_throw_exception(proj_geod_exception_ce, "Invalid CRS input");
            return;
        }
        
        ctx = proj_get_default_context();
        crs_pj = proj_create(ctx, crs_string);
        efree(crs_string);
    }
    
    if (!crs_pj) {
        proj_throw_crs_error(proj_get_default_context());
        return;
    }
    
    /* Extract ellipsoid from CRS */
    ctx = proj_get_default_context();
    ellipsoid = proj_get_ellipsoid(ctx, crs_pj);
    if (!ellipsoid) {
        proj_destroy(crs_pj);
        proj_throw_exception(proj_geod_exception_ce, "Failed to get ellipsoid from CRS");
        return;
    }
    
    /* Get ellipsoid parameters */
    int success = proj_ellipsoid_get_parameters(ctx, ellipsoid, &ellipsoid_a, &ellipsoid_b, 
                                               &ellipsoid_is_semi_minor_computed, &ellipsoid_rf);
    if (!success || ellipsoid_a <= 0 || ellipsoid_b <= 0) {
        /* Note: ellipsoid is owned by crs_pj, don't destroy it separately */
        proj_destroy(crs_pj);
        proj_throw_exception(proj_geod_exception_ce, "Failed to get ellipsoid parameters from CRS");
        return;
    }
    
    double f = (ellipsoid_a - ellipsoid_b) / ellipsoid_a;
    
    /* Note: ellipsoid is owned by crs_pj, don't destroy it separately */
    proj_destroy(crs_pj);
    
    /* Create new ProjGeod object */
    object_init_ex(return_value, proj_geod_ce);
    proj_geod_object *intern = GEOD_FROM_OBJECT(Z_OBJ_P(return_value));
    intern->a = ellipsoid_a;
    intern->f = f;
    geod_init(&intern->geod, intern->a, intern->f);
}

/* Static factory method: fromEpsg */
static PHP_METHOD(Geod, fromEpsg)
{
    zend_long epsg_code;
    char *crs_string;
    PJ_CONTEXT *ctx;
    PJ *crs_pj, *ellipsoid;
    double ellipsoid_a, ellipsoid_b, ellipsoid_rf;
    int ellipsoid_is_semi_minor_computed;
    
    ZEND_PARSE_PARAMETERS_START(1, 1)
        Z_PARAM_LONG(epsg_code)
    ZEND_PARSE_PARAMETERS_END();

    if (epsg_code <= 0) {
        proj_throw_exception(proj_geod_exception_ce, "EPSG code must be positive");
        return;
    }
    
    spprintf(&crs_string, 0, "EPSG:%ld", epsg_code);
    
    ctx = proj_get_default_context();
    crs_pj = proj_create(ctx, crs_string);
    efree(crs_string);
    
    if (!crs_pj) {
        proj_throw_crs_error(ctx);
        return;
    }
    
    /* Extract ellipsoid from CRS */
    ellipsoid = proj_get_ellipsoid(ctx, crs_pj);
    if (!ellipsoid) {
        proj_destroy(crs_pj);
        proj_throw_exception(proj_geod_exception_ce, "Failed to get ellipsoid from EPSG:%ld", epsg_code);
        return;
    }
    
    /* Get ellipsoid parameters */
    int success = proj_ellipsoid_get_parameters(ctx, ellipsoid, &ellipsoid_a, &ellipsoid_b, 
                                               &ellipsoid_is_semi_minor_computed, &ellipsoid_rf);
    if (!success || ellipsoid_a <= 0 || ellipsoid_b <= 0) {
        /* Note: ellipsoid is owned by crs_pj, don't destroy it separately */
        proj_destroy(crs_pj);
        proj_throw_exception(proj_geod_exception_ce, "Failed to get ellipsoid parameters from EPSG:%ld", epsg_code);
        return;
    }
    
    double f = (ellipsoid_a - ellipsoid_b) / ellipsoid_a;
    
    /* Note: ellipsoid is owned by crs_pj, don't destroy it separately */
    proj_destroy(crs_pj);
    
    /* Create new ProjGeod object */
    object_init_ex(return_value, proj_geod_ce);
    proj_geod_object *intern = GEOD_FROM_OBJECT(Z_OBJ_P(return_value));
    intern->a = ellipsoid_a;
    intern->f = f;
    geod_init(&intern->geod, intern->a, intern->f);
}

/* Static factory method: fromEllipsoidName */
static PHP_METHOD(Geod, fromEllipsoidName)
{
    char *ellipsoid_name;
    size_t ellipsoid_name_len;
    PJ_CONTEXT *ctx;
    PJ *ellipsoid;
    PJ *crs_pj = NULL;
    double ellipsoid_a, ellipsoid_b, ellipsoid_rf;
    int ellipsoid_is_semi_minor_computed;
    int ellipsoid_owned = 0;  /* Track if we own the ellipsoid object */
    
    ZEND_PARSE_PARAMETERS_START(1, 1)
        Z_PARAM_STRING(ellipsoid_name, ellipsoid_name_len)
    ZEND_PARSE_PARAMETERS_END();

    ctx = proj_get_default_context();
    
    /* Try to create ellipsoid directly from name (for EPSG codes, etc.) */
    ellipsoid = proj_create(ctx, ellipsoid_name);
    
    if (!ellipsoid || proj_get_type(ellipsoid) != PJ_TYPE_ELLIPSOID) {
        if (ellipsoid) proj_destroy(ellipsoid);
        
        /* Try creating with +ellps= format */
        char *proj_string;
        spprintf(&proj_string, 0, "+proj=longlat +ellps=%s", ellipsoid_name);
        crs_pj = proj_create(ctx, proj_string);
        efree(proj_string);
        
        if (crs_pj) {
            /* Extract ellipsoid from the created CRS */
            ellipsoid = proj_get_ellipsoid(ctx, crs_pj);
            /* ellipsoid is owned by crs_pj, not by us */
            ellipsoid_owned = 0;
        }
        
        if (!ellipsoid || proj_get_type(ellipsoid) != PJ_TYPE_ELLIPSOID) {
            if (crs_pj) proj_destroy(crs_pj);
            proj_throw_exception(proj_geod_exception_ce, "Unknown ellipsoid name: %s", ellipsoid_name);
            return;
        }
    } else {
        /* We created the ellipsoid directly, so we own it */
        ellipsoid_owned = 1;
    }
    
    /* Get ellipsoid parameters */
    int success = proj_ellipsoid_get_parameters(ctx, ellipsoid, &ellipsoid_a, &ellipsoid_b, 
                                               &ellipsoid_is_semi_minor_computed, &ellipsoid_rf);
    if (!success || ellipsoid_a <= 0 || ellipsoid_b <= 0) {
        if (ellipsoid_owned) {
            proj_destroy(ellipsoid);
        }
        if (crs_pj) {
            proj_destroy(crs_pj);
        }
        proj_throw_exception(proj_geod_exception_ce, "Failed to get ellipsoid parameters from %s", ellipsoid_name);
        return;
    }
    
    double f = (ellipsoid_a - ellipsoid_b) / ellipsoid_a;
    
    /* Clean up based on ownership */
    if (ellipsoid_owned) {
        proj_destroy(ellipsoid);
    }
    if (crs_pj) {
        proj_destroy(crs_pj);
    }
    
    /* Create new ProjGeod object */
    object_init_ex(return_value, proj_geod_ce);
    proj_geod_object *intern = GEOD_FROM_OBJECT(Z_OBJ_P(return_value));
    intern->a = ellipsoid_a;
    intern->f = f;
    geod_init(&intern->geod, intern->a, intern->f);
}

/* Static factory method: fromParameters */
static PHP_METHOD(Geod, fromParameters)
{
    double a, param2;
    char *param_name = "auto";
    size_t param_name_len;
    double f = 0.0;
    
    ZEND_PARSE_PARAMETERS_START(2, 3)
        Z_PARAM_DOUBLE(a)
        Z_PARAM_DOUBLE(param2)
        Z_PARAM_OPTIONAL
        Z_PARAM_STRING(param_name, param_name_len)
    ZEND_PARSE_PARAMETERS_END();

    if (a <= 0) {
        proj_throw_exception(proj_geod_exception_ce, "Semi-major axis must be positive");
        return;
    }
    
    /* Auto-detect parameter type or use explicit parameter name */
    if (strcasecmp(param_name, "auto") == 0) {
        /* Auto-detect based on value range */
        if (param2 > 0 && param2 < 1) {
            /* Likely flattening */
            f = param2;
        } else if (param2 > 1 && param2 < a) {
            /* Likely semi-minor axis */
            f = (a - param2) / a;
        } else if (param2 > 1) {
            /* Likely inverse flattening */
            f = 1.0 / param2;
        } else {
            proj_throw_exception(proj_geod_exception_ce, "Cannot auto-detect parameter type from value %.6f", param2);
            return;
        }
    } else if (strcasecmp(param_name, "f") == 0 || strcasecmp(param_name, "flattening") == 0) {
        if (param2 < 0 || param2 >= 1) {
            proj_throw_exception(proj_geod_exception_ce, "Flattening must be between 0 and 1");
            return;
        }
        f = param2;
    } else if (strcasecmp(param_name, "b") == 0 || strcasecmp(param_name, "semi_minor") == 0) {
        if (param2 <= 0 || param2 >= a) {
            proj_throw_exception(proj_geod_exception_ce, "Semi-minor axis must be positive and less than semi-major axis");
            return;
        }
        f = (a - param2) / a;
    } else if (strcasecmp(param_name, "rf") == 0 || strcasecmp(param_name, "inverse_flattening") == 0) {
        if (param2 <= 1) {
            proj_throw_exception(proj_geod_exception_ce, "Inverse flattening must be greater than 1");
            return;
        }
        f = 1.0 / param2;
    } else if (strcasecmp(param_name, "es") == 0 || strcasecmp(param_name, "eccentricity_squared") == 0) {
        if (param2 < 0 || param2 >= 1) {
            proj_throw_exception(proj_geod_exception_ce, "Eccentricity squared must be between 0 and 1");
            return;
        }
        /* es = f * (2 - f), solve for f */
        f = 1.0 - sqrt(1.0 - param2);
    } else {
        proj_throw_exception(proj_geod_exception_ce, "Unknown parameter name: %s. Use 'f', 'b', 'rf', 'es', or 'auto'", param_name);
        return;
    }
    
    /* Validate final flattening */
    if (f < 0 || f >= 1) {
        proj_throw_exception(proj_geod_exception_ce, "Computed flattening %.6f is invalid", f);
        return;
    }
    
    /* Create new ProjGeod object */
    object_init_ex(return_value, proj_geod_ce);
    proj_geod_object *intern = GEOD_FROM_OBJECT(Z_OBJ_P(return_value));
    intern->a = a;
    intern->f = f;
    geod_init(&intern->geod, intern->a, intern->f);
}

/* Get initialization string */
static PHP_METHOD(Geod, getInitstring)
{
    proj_geod_object *intern;
    char initstring[256];

    ZEND_PARSE_PARAMETERS_NONE();

    intern = GEOD_FROM_OBJECT(Z_OBJ_P(getThis()));
    
    snprintf(initstring, sizeof(initstring), "a=%.6f f=%.10f", intern->a, intern->f);
    RETURN_STRING(initstring);
}

/* Check if sphere */
static PHP_METHOD(Geod, isSphere)
{
    proj_geod_object *intern;

    ZEND_PARSE_PARAMETERS_NONE();

    intern = GEOD_FROM_OBJECT(Z_OBJ_P(getThis()));
    RETURN_BOOL(intern->f == 0.0);
}

/* Get semi-major axis */
static PHP_METHOD(Geod, getA)
{
    proj_geod_object *intern;

    ZEND_PARSE_PARAMETERS_NONE();

    intern = GEOD_FROM_OBJECT(Z_OBJ_P(getThis()));
    RETURN_DOUBLE(intern->a);
}

/* Get semi-minor axis */
static PHP_METHOD(Geod, getB)
{
    proj_geod_object *intern;

    ZEND_PARSE_PARAMETERS_NONE();

    intern = GEOD_FROM_OBJECT(Z_OBJ_P(getThis()));
    RETURN_DOUBLE(intern->a * (1 - intern->f));
}

/* Get squared eccentricity */
static PHP_METHOD(Geod, getEs)
{
    proj_geod_object *intern;
    double es;

    ZEND_PARSE_PARAMETERS_NONE();

    intern = GEOD_FROM_OBJECT(Z_OBJ_P(getThis()));
    es = intern->f * (2 - intern->f);
    RETURN_DOUBLE(es);
}

/* Get flattening */
static PHP_METHOD(Geod, getF)
{
    proj_geod_object *intern;

    ZEND_PARSE_PARAMETERS_NONE();

    intern = GEOD_FROM_OBJECT(Z_OBJ_P(getThis()));
    RETURN_DOUBLE(intern->f);
}

/* Forward geodetic computation */
static PHP_METHOD(Geod, fwd)
{
    zval *points, *az, *dist;
    zend_bool radians = 0, return_back_azimuth = 1;
    proj_geod_object *intern;
    
    ZEND_PARSE_PARAMETERS_START(3, 5)
        Z_PARAM_ZVAL(points)
        Z_PARAM_ZVAL(az)
        Z_PARAM_ZVAL(dist)
        Z_PARAM_OPTIONAL
        Z_PARAM_BOOL(radians)
        Z_PARAM_BOOL(return_back_azimuth)
    ZEND_PARSE_PARAMETERS_END();

    intern = GEOD_FROM_OBJECT(Z_OBJ_P(getThis()));

    /* Handle single point input - points should be [lon, lat] */
    if (Z_TYPE_P(points) == IS_ARRAY && zend_array_count(Z_ARRVAL_P(points)) == 2) {
        zval *lon_val = zend_hash_index_find(Z_ARRVAL_P(points), 0);
        zval *lat_val = zend_hash_index_find(Z_ARRVAL_P(points), 1);
        
        if (lon_val && lat_val && Z_TYPE_P(az) != IS_ARRAY && Z_TYPE_P(dist) != IS_ARRAY) {
            double lon1, lat1, azi1, s12;
            double lon2, lat2, azi2;
            
            lon1 = zval_get_double(lon_val);
            lat1 = zval_get_double(lat_val);
            azi1 = zval_get_double(az);
            s12 = zval_get_double(dist);
            
            /* Convert to degrees if input is in radians */
            if (radians) {
                lon1 = proj_todeg(lon1);
                lat1 = proj_todeg(lat1);
                azi1 = proj_todeg(azi1);
            }
            
            /* Use PROJ's proper geodesic API for forward computation */
            geod_direct(&intern->geod, lat1, lon1, azi1, s12, &lat2, &lon2, &azi2);
            
            /* Adjust back azimuth to match PROJ command line tool behavior */
            azi2 = azi2 - 180.0;
            if (azi2 < -180.0) {
                azi2 += 360.0;
            }
            
            /* Convert back to radians if requested */
            if (radians) {
                lon2 = proj_torad(lon2);
                lat2 = proj_torad(lat2);
                azi2 = proj_torad(azi2);
            }
            
            /* Return coordinate tuple [lon, lat, back_azimuth?] */
            array_init(return_value);
            add_next_index_double(return_value, lon2);
            add_next_index_double(return_value, lat2);
            
            if (return_back_azimuth) {
                add_next_index_double(return_value, azi2);
            }
            return;
        }
    }

    /* Handle array of coordinate tuples - points should be [[lon1,lat1], [lon2,lat2], ...] */
    if (Z_TYPE_P(points) != IS_ARRAY) {
        proj_throw_exception(proj_geod_exception_ce, "Points must be an array of coordinate tuples");
        return;
    }
    
    size_t count = zend_array_count(Z_ARRVAL_P(points));
    if (count == 0) {
        array_init(return_value);
        return;
    }
    
    /* Parse coordinate tuples into separate arrays */
    double *lon_coords = emalloc(count * sizeof(double));
    double *lat_coords = emalloc(count * sizeof(double));
    
    size_t i = 0;
    zval *entry;
    ZEND_HASH_FOREACH_VAL(Z_ARRVAL_P(points), entry) {
        if (Z_TYPE_P(entry) != IS_ARRAY || zend_array_count(Z_ARRVAL_P(entry)) < 2) {
            efree(lon_coords);
            efree(lat_coords);
            proj_throw_exception(proj_geod_exception_ce, "Each point must be a coordinate tuple [lon, lat]");
            return;
        }
        
        zval *lon_val = zend_hash_index_find(Z_ARRVAL_P(entry), 0);
        zval *lat_val = zend_hash_index_find(Z_ARRVAL_P(entry), 1);
        
        if (!lon_val || !lat_val) {
            efree(lon_coords);
            efree(lat_coords);
            proj_throw_exception(proj_geod_exception_ce, "Invalid coordinate tuple");
            return;
        }
        
        lon_coords[i] = zval_get_double(lon_val);
        lat_coords[i] = zval_get_double(lat_val);
        i++;
    } ZEND_HASH_FOREACH_END();
    
    /* Parse azimuth and distance (can be single values or arrays) */
    double *az_coords = emalloc(count * sizeof(double));
    double *dist_coords = emalloc(count * sizeof(double));
    
    /* Handle azimuth parameter */
    if (Z_TYPE_P(az) == IS_ARRAY) {
        size_t az_count = count;
        double *temp_az = proj_zval_to_array(az, &az_count);
        if (!temp_az || az_count != count) {
            efree(lon_coords); efree(lat_coords); efree(az_coords); efree(dist_coords);
            if (temp_az) efree(temp_az);
            proj_throw_exception(proj_geod_exception_ce, "Azimuth array size mismatch");
            return;
        }
        memcpy(az_coords, temp_az, count * sizeof(double));
        efree(temp_az);
    } else {
        double az_val = zval_get_double(az);
        for (size_t i = 0; i < count; i++) {
            az_coords[i] = az_val;
        }
    }
    
    /* Handle distance parameter */
    if (Z_TYPE_P(dist) == IS_ARRAY) {
        size_t dist_count = count;
        double *temp_dist = proj_zval_to_array(dist, &dist_count);
        if (!temp_dist || dist_count != count) {
            efree(lon_coords); efree(lat_coords); efree(az_coords); efree(dist_coords);
            if (temp_dist) efree(temp_dist);
            proj_throw_exception(proj_geod_exception_ce, "Distance array size mismatch");
            return;
        }
        memcpy(dist_coords, temp_dist, count * sizeof(double));
        efree(temp_dist);
    } else {
        double dist_val = zval_get_double(dist);
        for (size_t i = 0; i < count; i++) {
            dist_coords[i] = dist_val;
        }
    }

    /* Process arrays */
    double *result_lons = emalloc(count * sizeof(double));
    double *result_lats = emalloc(count * sizeof(double));
    double *result_azs = return_back_azimuth ? emalloc(count * sizeof(double)) : NULL;
    
    for (i = 0; i < count; i++) {
        double lon1 = lon_coords[i];
        double lat1 = lat_coords[i];
        double azi1 = az_coords[i];
        double s12 = dist_coords[i];
        double lon2, lat2, azi2;
        
        /* Convert to degrees if input is in radians */
        if (radians) {
            lon1 = proj_todeg(lon1);
            lat1 = proj_todeg(lat1);
            azi1 = proj_todeg(azi1);
        }
        
        /* Use PROJ's proper geodesic API for forward computation */
        geod_direct(&intern->geod, lat1, lon1, azi1, s12, &lat2, &lon2, &azi2);
        
        /* Adjust back azimuth to match PROJ command line tool behavior */
        azi2 = azi2 - 180.0;
        if (azi2 < -180.0) {
            azi2 += 360.0;
        }
        
        /* Convert back to radians if requested */
        if (radians) {
            lon2 = proj_torad(lon2);
            lat2 = proj_torad(lat2);
            azi2 = proj_torad(azi2);
        }
        
        result_lons[i] = lon2;
        result_lats[i] = lat2;
        if (result_azs) result_azs[i] = azi2;
    }

    /* Build result as array of coordinate tuples */
    array_init(return_value);
    
    for (i = 0; i < count; i++) {
        zval point_array;
        array_init(&point_array);
        add_next_index_double(&point_array, result_lons[i]);
        add_next_index_double(&point_array, result_lats[i]);
        
        if (return_back_azimuth && result_azs) {
            add_next_index_double(&point_array, result_azs[i]);
        }
        
        add_next_index_zval(return_value, &point_array);
    }

    efree(lon_coords);
    efree(lat_coords);
    efree(az_coords);
    efree(dist_coords);
    efree(result_lons);
    efree(result_lats);
    if (result_azs) efree(result_azs);
}

/* Inverse geodetic computation */
static PHP_METHOD(Geod, inv)
{
    zval *points1, *points2;
    zend_bool radians = 0, return_back_azimuth = 1;
    proj_geod_object *intern;
    
    ZEND_PARSE_PARAMETERS_START(2, 4)
        Z_PARAM_ZVAL(points1)
        Z_PARAM_ZVAL(points2)
        Z_PARAM_OPTIONAL
        Z_PARAM_BOOL(radians)
        Z_PARAM_BOOL(return_back_azimuth)
    ZEND_PARSE_PARAMETERS_END();

    intern = GEOD_FROM_OBJECT(Z_OBJ_P(getThis()));

    /* Handle single point inputs - points should be [lon, lat] */
    if (Z_TYPE_P(points1) == IS_ARRAY && zend_array_count(Z_ARRVAL_P(points1)) == 2 &&
        Z_TYPE_P(points2) == IS_ARRAY && zend_array_count(Z_ARRVAL_P(points2)) == 2) {
        
        zval *lon1_val = zend_hash_index_find(Z_ARRVAL_P(points1), 0);
        zval *lat1_val = zend_hash_index_find(Z_ARRVAL_P(points1), 1);
        zval *lon2_val = zend_hash_index_find(Z_ARRVAL_P(points2), 0);
        zval *lat2_val = zend_hash_index_find(Z_ARRVAL_P(points2), 1);
        
        if (lon1_val && lat1_val && lon2_val && lat2_val) {
            double lon1, lat1, lon2, lat2;
            double s12, azi1, azi2;
            
            lon1 = zval_get_double(lon1_val);
            lat1 = zval_get_double(lat1_val);
            lon2 = zval_get_double(lon2_val);
            lat2 = zval_get_double(lat2_val);
            
            /* Convert to degrees if input is in radians */
            if (radians) {
                lon1 = proj_todeg(lon1);
                lat1 = proj_todeg(lat1);
                lon2 = proj_todeg(lon2);
                lat2 = proj_todeg(lat2);
            }
            
            /* Use PROJ's proper geodesic API for inverse computation */
            geod_inverse(&intern->geod, lat1, lon1, lat2, lon2, &s12, &azi1, &azi2);
            
            /* Adjust back azimuth to match PROJ command line tool behavior */
            azi2 = azi2 + 180.0;
            if (azi2 >= 180.0) {
                azi2 -= 360.0;
            }
            
            /* Convert back to radians if requested */
            if (radians) {
                azi1 = proj_torad(azi1);
                azi2 = proj_torad(azi2);
            }
            
            array_init(return_value);
            add_next_index_double(return_value, azi1);
            
            if (return_back_azimuth) {
                add_next_index_double(return_value, azi2);
            }
            
            add_next_index_double(return_value, s12);
            return;
        }
    }

    /* Handle array of coordinate tuples */
    if (Z_TYPE_P(points1) != IS_ARRAY || Z_TYPE_P(points2) != IS_ARRAY) {
        proj_throw_exception(proj_geod_exception_ce, "Points must be arrays of coordinate tuples");
        return;
    }
    
    size_t count1 = zend_array_count(Z_ARRVAL_P(points1));
    size_t count2 = zend_array_count(Z_ARRVAL_P(points2));
    
    if (count1 != count2) {
        proj_throw_exception(proj_geod_exception_ce, "Both point arrays must have the same length");
        return;
    }
    
    size_t count = count1;
    if (count == 0) {
        array_init(return_value);
        return;
    }
    
    /* Parse coordinate tuples into separate arrays */
    double *lon1_coords = emalloc(count * sizeof(double));
    double *lat1_coords = emalloc(count * sizeof(double));
    double *lon2_coords = emalloc(count * sizeof(double));
    double *lat2_coords = emalloc(count * sizeof(double));
    
    /* Parse first points array */
    size_t i = 0;
    zval *entry;
    ZEND_HASH_FOREACH_VAL(Z_ARRVAL_P(points1), entry) {
        if (Z_TYPE_P(entry) != IS_ARRAY || zend_array_count(Z_ARRVAL_P(entry)) < 2) {
            efree(lon1_coords); efree(lat1_coords); efree(lon2_coords); efree(lat2_coords);
            proj_throw_exception(proj_geod_exception_ce, "Each point must be a coordinate tuple [lon, lat]");
            return;
        }
        
        zval *lon_val = zend_hash_index_find(Z_ARRVAL_P(entry), 0);
        zval *lat_val = zend_hash_index_find(Z_ARRVAL_P(entry), 1);
        
        if (!lon_val || !lat_val) {
            efree(lon1_coords); efree(lat1_coords); efree(lon2_coords); efree(lat2_coords);
            proj_throw_exception(proj_geod_exception_ce, "Invalid coordinate tuple");
            return;
        }
        
        lon1_coords[i] = zval_get_double(lon_val);
        lat1_coords[i] = zval_get_double(lat_val);
        i++;
    } ZEND_HASH_FOREACH_END();
    
    /* Parse second points array */
    i = 0;
    ZEND_HASH_FOREACH_VAL(Z_ARRVAL_P(points2), entry) {
        if (Z_TYPE_P(entry) != IS_ARRAY || zend_array_count(Z_ARRVAL_P(entry)) < 2) {
            efree(lon1_coords); efree(lat1_coords); efree(lon2_coords); efree(lat2_coords);
            proj_throw_exception(proj_geod_exception_ce, "Each point must be a coordinate tuple [lon, lat]");
            return;
        }
        
        zval *lon_val = zend_hash_index_find(Z_ARRVAL_P(entry), 0);
        zval *lat_val = zend_hash_index_find(Z_ARRVAL_P(entry), 1);
        
        if (!lon_val || !lat_val) {
            efree(lon1_coords); efree(lat1_coords); efree(lon2_coords); efree(lat2_coords);
            proj_throw_exception(proj_geod_exception_ce, "Invalid coordinate tuple");
            return;
        }
        
        lon2_coords[i] = zval_get_double(lon_val);
        lat2_coords[i] = zval_get_double(lat_val);
        i++;
    } ZEND_HASH_FOREACH_END();

    /* Process arrays */
    double *result_azis = emalloc(count * sizeof(double));
    double *result_azis2 = return_back_azimuth ? emalloc(count * sizeof(double)) : NULL;
    double *result_dists = emalloc(count * sizeof(double));
    
    /* Process arrays using PROJ's proper geodesic API */
    for (i = 0; i < count; i++) {
        double lon1 = lon1_coords[i];
        double lat1 = lat1_coords[i];
        double lon2 = lon2_coords[i];
        double lat2 = lat2_coords[i];
        double s12, azi1, azi2;
        
        /* Convert to degrees if input is in radians */
        if (radians) {
            lon1 = proj_todeg(lon1);
            lat1 = proj_todeg(lat1);
            lon2 = proj_todeg(lon2);
            lat2 = proj_todeg(lat2);
        }
        
        /* Use PROJ's proper geodesic API for inverse computation */
        geod_inverse(&intern->geod, lat1, lon1, lat2, lon2, &s12, &azi1, &azi2);
        
        /* Adjust back azimuth to match PROJ command line tool behavior */
        azi2 = azi2 + 180.0;
        if (azi2 >= 180.0) {
            azi2 -= 360.0;
        }
        
        /* Convert back to radians if requested */
        if (radians) {
            azi1 = proj_torad(azi1);
            azi2 = proj_torad(azi2);
        }
        
        result_azis[i] = azi1;
        if (result_azis2) result_azis2[i] = azi2;
        result_dists[i] = s12;
    }

    /* No external objects to cleanup - geodesic structure is part of the object */

    /* Build result as array of result tuples [azi1, azi2?, distance] */
    array_init(return_value);
    
    for (i = 0; i < count; i++) {
        zval result_array;
        array_init(&result_array);
        add_next_index_double(&result_array, result_azis[i]);
        
        if (return_back_azimuth && result_azis2) {
            add_next_index_double(&result_array, result_azis2[i]);
        }
        
        add_next_index_double(&result_array, result_dists[i]);
        add_next_index_zval(return_value, &result_array);
    }

    efree(lon1_coords);
    efree(lat1_coords);
    efree(lon2_coords);
    efree(lat2_coords);
    efree(result_azis);
    if (result_azis2) efree(result_azis2);
    efree(result_dists);
}

/* Line length calculation */
static PHP_METHOD(Geod, lineLength)
{
    zval *points;
    zend_bool radians = 0;
    proj_geod_object *intern;
    size_t count;
    double total_length = 0.0;
    
    ZEND_PARSE_PARAMETERS_START(1, 2)
        Z_PARAM_ARRAY(points)
        Z_PARAM_OPTIONAL
        Z_PARAM_BOOL(radians)
    ZEND_PARSE_PARAMETERS_END();

    intern = GEOD_FROM_OBJECT(Z_OBJ_P(getThis()));

    count = zend_array_count(Z_ARRVAL_P(points));
    if (count < 2) {
        RETURN_DOUBLE(0.0);
    }

    /* Parse coordinate tuples into separate arrays */
    double *lon_coords = emalloc(count * sizeof(double));
    double *lat_coords = emalloc(count * sizeof(double));
    
    size_t i = 0;
    zval *entry;
    ZEND_HASH_FOREACH_VAL(Z_ARRVAL_P(points), entry) {
        if (Z_TYPE_P(entry) != IS_ARRAY || zend_array_count(Z_ARRVAL_P(entry)) < 2) {
            efree(lon_coords);
            efree(lat_coords);
            proj_throw_exception(proj_geod_exception_ce, "Each point must be a coordinate tuple [lon, lat]");
            return;
        }
        
        zval *lon_val = zend_hash_index_find(Z_ARRVAL_P(entry), 0);
        zval *lat_val = zend_hash_index_find(Z_ARRVAL_P(entry), 1);
        
        if (!lon_val || !lat_val) {
            efree(lon_coords);
            efree(lat_coords);
            proj_throw_exception(proj_geod_exception_ce, "Invalid coordinate tuple");
            return;
        }
        
        lon_coords[i] = zval_get_double(lon_val);
        lat_coords[i] = zval_get_double(lat_val);
        i++;
    } ZEND_HASH_FOREACH_END();

    /* Calculate total length between consecutive points */
    for (i = 0; i < count - 1; i++) {
        double lon1 = lon_coords[i];
        double lat1 = lat_coords[i];
        double lon2 = lon_coords[i + 1];
        double lat2 = lat_coords[i + 1];
        double s12, azi1, azi2;
        
        /* Convert to degrees if input is in radians */
        if (radians) {
            lon1 = proj_todeg(lon1);
            lat1 = proj_todeg(lat1);
            lon2 = proj_todeg(lon2);
            lat2 = proj_todeg(lat2);
        }
        
        /* Use PROJ's proper geodesic API for distance calculation */
        geod_inverse(&intern->geod, lat1, lon1, lat2, lon2, &s12, NULL, NULL);
        total_length += s12;
    }

    efree(lon_coords);
    efree(lat_coords);
    
    RETURN_DOUBLE(total_length);
}

/* String representation */
static PHP_METHOD(Geod, __toString)
{
    proj_geod_object *intern;
    char str[256];

    ZEND_PARSE_PARAMETERS_NONE();

    intern = GEOD_FROM_OBJECT(Z_OBJ_P(getThis()));
    
    snprintf(str, sizeof(str), "Geod(a=%.6f, f=%.10f)", intern->a, intern->f);
    RETURN_STRING(str);
}

/* Generate intermediate points between two points */
static PHP_METHOD(Geod, npts) {
    zval *point1, *point2;
    zend_long npts;
    zend_bool radians = 0;
    proj_geod_object *intern;
    
    ZEND_PARSE_PARAMETERS_START(3, 4)
        Z_PARAM_ZVAL(point1)
        Z_PARAM_ZVAL(point2)
        Z_PARAM_LONG(npts)
        Z_PARAM_OPTIONAL
        Z_PARAM_BOOL(radians)
    ZEND_PARSE_PARAMETERS_END();

    intern = GEOD_FROM_OBJECT(Z_OBJ_P(getThis()));

    if (npts < 0) {
        proj_throw_exception(proj_geod_exception_ce, "Number of points must be non-negative");
        return;
    }

    /* Parse coordinate tuples */
    if (Z_TYPE_P(point1) != IS_ARRAY || zend_array_count(Z_ARRVAL_P(point1)) < 2 ||
        Z_TYPE_P(point2) != IS_ARRAY || zend_array_count(Z_ARRVAL_P(point2)) < 2) {
        proj_throw_exception(proj_geod_exception_ce, "Points must be coordinate tuples [lon, lat]");
        return;
    }
    
    zval *lon1_val = zend_hash_index_find(Z_ARRVAL_P(point1), 0);
    zval *lat1_val = zend_hash_index_find(Z_ARRVAL_P(point1), 1);
    zval *lon2_val = zend_hash_index_find(Z_ARRVAL_P(point2), 0);
    zval *lat2_val = zend_hash_index_find(Z_ARRVAL_P(point2), 1);
    
    if (!lon1_val || !lat1_val || !lon2_val || !lat2_val) {
        proj_throw_exception(proj_geod_exception_ce, "Invalid coordinate tuples");
        return;
    }
    
    double lon1 = zval_get_double(lon1_val);
    double lat1 = zval_get_double(lat1_val);
    double lon2 = zval_get_double(lon2_val);
    double lat2 = zval_get_double(lat2_val);

    /* Convert to degrees if input is in radians */
    if (radians) {
        lon1 = proj_todeg(lon1);
        lat1 = proj_todeg(lat1);
        lon2 = proj_todeg(lon2);
        lat2 = proj_todeg(lat2);
    }

    /* Use PROJ geodesic library for intermediate points calculation */
    double s12, azi1, azi2;
    
    /* First get the total distance and forward azimuth using existing geodesic structure */
    geod_inverse(&intern->geod, lat1, lon1, lat2, lon2, &s12, &azi1, &azi2);
    
    array_init(return_value);
    
    /* Generate intermediate points using PROJ geodesic API */
    size_t i;
    for (i = 0; i <= npts + 1; i++) {
        double fraction = (double)i / (double)(npts + 1);
        double distance = fraction * s12;
        double result_lat, result_lon, result_azi;
        
        /* Forward computation to get intermediate point using existing geodesic structure */
        geod_direct(&intern->geod, lat1, lon1, azi1, distance, &result_lat, &result_lon, &result_azi);
        
        /* Convert back to radians if requested */
        if (radians) {
            result_lon = proj_torad(result_lon);
            result_lat = proj_torad(result_lat);
        }
        
        zval point_array;
        array_init(&point_array);
        add_next_index_double(&point_array, result_lon);
        add_next_index_double(&point_array, result_lat);
        add_next_index_zval(return_value, &point_array);
    }
}

/* Forward computation for multiple distances */
static PHP_METHOD(Geod, fwdIntermediate) {
    zval *point1;
    double az;
    zval *distances;
    zend_bool radians = 0, return_back_azimuth = 1;
    proj_geod_object *intern;
    
    ZEND_PARSE_PARAMETERS_START(3, 5)
        Z_PARAM_ZVAL(point1)
        Z_PARAM_DOUBLE(az)
        Z_PARAM_ARRAY(distances)
        Z_PARAM_OPTIONAL
        Z_PARAM_BOOL(radians)
        Z_PARAM_BOOL(return_back_azimuth)
    ZEND_PARSE_PARAMETERS_END();

    intern = GEOD_FROM_OBJECT(Z_OBJ_P(getThis()));

    /* Parse coordinate tuple */
    if (Z_TYPE_P(point1) != IS_ARRAY || zend_array_count(Z_ARRVAL_P(point1)) < 2) {
        proj_throw_exception(proj_geod_exception_ce, "Point must be a coordinate tuple [lon, lat]");
        return;
    }
    
    zval *lon1_val = zend_hash_index_find(Z_ARRVAL_P(point1), 0);
    zval *lat1_val = zend_hash_index_find(Z_ARRVAL_P(point1), 1);
    
    if (!lon1_val || !lat1_val) {
        proj_throw_exception(proj_geod_exception_ce, "Invalid coordinate tuple");
        return;
    }
    
    double lon1 = zval_get_double(lon1_val);
    double lat1 = zval_get_double(lat1_val);

    /* Convert to degrees if input is in radians */
    if (radians) {
        lon1 = proj_todeg(lon1);
        lat1 = proj_todeg(lat1);
        az = proj_todeg(az);
    }

    /* Parse distance array */
    size_t count = zend_array_count(Z_ARRVAL_P(distances));
    double *dist_coords = proj_zval_to_array(distances, &count);
    
    if (!dist_coords) {
        proj_throw_exception(proj_geod_exception_ce, "Failed to parse distances array");
        return;
    }

    array_init(return_value);
    
    /* Generate points for each distance using PROJ geodesic API */
    size_t i;
    for (i = 0; i < count; i++) {
        double distance = dist_coords[i];
        double result_lat, result_lon, result_azi;
        
        /* Forward computation to get point at this distance using existing geodesic structure */
        geod_direct(&intern->geod, lat1, lon1, az, distance, &result_lat, &result_lon, &result_azi);
        
        /* Convert back to radians if requested */
        if (radians) {
            result_lon = proj_torad(result_lon);
            result_lat = proj_torad(result_lat);
            result_azi = proj_torad(result_azi);
        }
        
        zval point_array;
        array_init(&point_array);
        add_next_index_double(&point_array, result_lon);
        add_next_index_double(&point_array, result_lat);
        
        if (return_back_azimuth) {
            add_next_index_double(&point_array, result_azi);
        }
        
        add_next_index_zval(return_value, &point_array);
    }

    efree(dist_coords);
}

/* Inverse computation with intermediate points */
static PHP_METHOD(Geod, invIntermediate) {
    zval *point1, *point2;
    zend_long npts;
    zend_bool radians = 0, return_back_azimuth = 1;
    proj_geod_object *intern;
    
    ZEND_PARSE_PARAMETERS_START(3, 5)
        Z_PARAM_ZVAL(point1)
        Z_PARAM_ZVAL(point2)
        Z_PARAM_LONG(npts)
        Z_PARAM_OPTIONAL
        Z_PARAM_BOOL(radians)
        Z_PARAM_BOOL(return_back_azimuth)
    ZEND_PARSE_PARAMETERS_END();

    intern = GEOD_FROM_OBJECT(Z_OBJ_P(getThis()));

    if (npts < 0) {
        proj_throw_exception(proj_geod_exception_ce, "Number of points must be non-negative");
        return;
    }

    /* Parse coordinate tuples */
    if (Z_TYPE_P(point1) != IS_ARRAY || zend_array_count(Z_ARRVAL_P(point1)) < 2 ||
        Z_TYPE_P(point2) != IS_ARRAY || zend_array_count(Z_ARRVAL_P(point2)) < 2) {
        proj_throw_exception(proj_geod_exception_ce, "Points must be coordinate tuples [lon, lat]");
        return;
    }
    
    zval *lon1_val = zend_hash_index_find(Z_ARRVAL_P(point1), 0);
    zval *lat1_val = zend_hash_index_find(Z_ARRVAL_P(point1), 1);
    zval *lon2_val = zend_hash_index_find(Z_ARRVAL_P(point2), 0);
    zval *lat2_val = zend_hash_index_find(Z_ARRVAL_P(point2), 1);
    
    if (!lon1_val || !lat1_val || !lon2_val || !lat2_val) {
        proj_throw_exception(proj_geod_exception_ce, "Invalid coordinate tuples");
        return;
    }
    
    double lon1 = zval_get_double(lon1_val);
    double lat1 = zval_get_double(lat1_val);
    double lon2 = zval_get_double(lon2_val);
    double lat2 = zval_get_double(lat2_val);

    /* Convert to degrees if input is in radians */
    if (radians) {
        lon1 = proj_todeg(lon1);
        lat1 = proj_todeg(lat1);
        lon2 = proj_todeg(lon2);
        lat2 = proj_todeg(lat2);
    }

    /* Use PROJ geodesic library for intermediate points calculation */
    double s12, azi1, azi2;
    
    /* First get the total distance and forward azimuth using existing geodesic structure */
    geod_inverse(&intern->geod, lat1, lon1, lat2, lon2, &s12, &azi1, &azi2);

    array_init(return_value);
    
    /* Generate intermediate points using PROJ geodesic API */
    size_t i;
    for (i = 1; i <= npts; i++) {
        double fraction = (double)i / (double)(npts + 1);
        double distance = fraction * s12;
        double result_lat, result_lon, result_azi;
        
        /* Forward computation to get intermediate point using existing geodesic structure */
        geod_direct(&intern->geod, lat1, lon1, azi1, distance, &result_lat, &result_lon, &result_azi);
        
        /* Convert back to radians if requested */
        if (radians) {
            result_lon = proj_torad(result_lon);
            result_lat = proj_torad(result_lat);
            result_azi = proj_torad(result_azi);
        }
        
        zval point_array;
        array_init(&point_array);
        add_next_index_double(&point_array, result_lon);
        add_next_index_double(&point_array, result_lat);
        
        if (return_back_azimuth) {
            add_next_index_double(&point_array, result_azi);
        }
        
        add_next_index_zval(return_value, &point_array);
    }
}

/* Calculate polygon area and perimeter */
static PHP_METHOD(Geod, polygonAreaPerimeter) {
    zval *points;
    zend_bool radians = 0;
    proj_geod_object *intern;
    
    ZEND_PARSE_PARAMETERS_START(1, 2)
        Z_PARAM_ARRAY(points)
        Z_PARAM_OPTIONAL
        Z_PARAM_BOOL(radians)
    ZEND_PARSE_PARAMETERS_END();

    intern = GEOD_FROM_OBJECT(Z_OBJ_P(getThis()));

    size_t count = zend_array_count(Z_ARRVAL_P(points));
    if (count < 3) {
        proj_throw_exception(proj_geod_exception_ce, "Polygon must have at least 3 points");
        return;
    }

    /* Parse coordinate tuples into separate arrays */
    double *lon_coords = emalloc(count * sizeof(double));
    double *lat_coords = emalloc(count * sizeof(double));
    
    size_t i = 0;
    zval *entry;
    ZEND_HASH_FOREACH_VAL(Z_ARRVAL_P(points), entry) {
        if (Z_TYPE_P(entry) != IS_ARRAY || zend_array_count(Z_ARRVAL_P(entry)) < 2) {
            efree(lon_coords);
            efree(lat_coords);
            proj_throw_exception(proj_geod_exception_ce, "Each point must be a coordinate tuple [lon, lat]");
            return;
        }
        
        zval *lon_val = zend_hash_index_find(Z_ARRVAL_P(entry), 0);
        zval *lat_val = zend_hash_index_find(Z_ARRVAL_P(entry), 1);
        
        if (!lon_val || !lat_val) {
            efree(lon_coords);
            efree(lat_coords);
            proj_throw_exception(proj_geod_exception_ce, "Invalid coordinate tuple");
            return;
        }
        
        lon_coords[i] = zval_get_double(lon_val);
        lat_coords[i] = zval_get_double(lat_val);
        i++;
    } ZEND_HASH_FOREACH_END();

    /* Use PROJ's proper geodesic polygon API for area and perimeter calculation */
    struct geod_polygon polygon;
    geod_polygon_init(&polygon, 0); /* polyline=0 for polygon */
    
    for (i = 0; i < count; i++) {
        double lon = lon_coords[i];
        double lat = lat_coords[i];
        
        /* Convert to degrees if input is in radians */
        if (radians) {
            lon = proj_todeg(lon);
            lat = proj_todeg(lat);
        }
        
        /* Add vertex to polygon using PROJ geodesic API */
        geod_polygon_addpoint(&intern->geod, &polygon, lat, lon);
    }
    
    /* Compute area and perimeter using PROJ geodesic algorithms */
    double area, perimeter;
    unsigned int polygon_vertices = geod_polygon_compute(&intern->geod, &polygon, 0, 1, &area, &perimeter);
    
    efree(lon_coords);
    efree(lat_coords);

    /* Return array with area and perimeter */
    array_init(return_value);
    add_assoc_double(return_value, "area", area);
    add_assoc_double(return_value, "perimeter", perimeter);
}

/* Individual segment lengths for polylines */
static PHP_METHOD(Geod, lineLengths) {
    zval *points;
    zend_bool radians = 0;
    proj_geod_object *intern;
    
    ZEND_PARSE_PARAMETERS_START(1, 2)
        Z_PARAM_ARRAY(points)
        Z_PARAM_OPTIONAL
        Z_PARAM_BOOL(radians)
    ZEND_PARSE_PARAMETERS_END();

    intern = GEOD_FROM_OBJECT(Z_OBJ_P(getThis()));

    size_t count = zend_array_count(Z_ARRVAL_P(points));
    if (count < 2) {
        /* Return empty array for less than 2 points */
        array_init(return_value);
        return;
    }

    /* Parse coordinate tuples into separate arrays */
    double *lon_coords = emalloc(count * sizeof(double));
    double *lat_coords = emalloc(count * sizeof(double));
    
    size_t i = 0;
    zval *entry;
    ZEND_HASH_FOREACH_VAL(Z_ARRVAL_P(points), entry) {
        if (Z_TYPE_P(entry) != IS_ARRAY || zend_array_count(Z_ARRVAL_P(entry)) < 2) {
            efree(lon_coords);
            efree(lat_coords);
            proj_throw_exception(proj_geod_exception_ce, "Each point must be a coordinate tuple [lon, lat]");
            return;
        }
        
        zval *lon_val = zend_hash_index_find(Z_ARRVAL_P(entry), 0);
        zval *lat_val = zend_hash_index_find(Z_ARRVAL_P(entry), 1);
        
        if (!lon_val || !lat_val) {
            efree(lon_coords);
            efree(lat_coords);
            proj_throw_exception(proj_geod_exception_ce, "Invalid coordinate tuple");
            return;
        }
        
        lon_coords[i] = zval_get_double(lon_val);
        lat_coords[i] = zval_get_double(lat_val);
        i++;
    } ZEND_HASH_FOREACH_END();

    array_init(return_value);

    /* Calculate distance for each segment between consecutive points using PROJ geodesic API */
    for (i = 0; i < count - 1; i++) {
        double lon1 = lon_coords[i];
        double lat1 = lat_coords[i];
        double lon2 = lon_coords[i + 1];
        double lat2 = lat_coords[i + 1];
        
        /* Convert to degrees if input is in radians */
        if (radians) {
            lon1 = proj_todeg(lon1);
            lat1 = proj_todeg(lat1);
            lon2 = proj_todeg(lon2);
            lat2 = proj_todeg(lat2);
        }
        
        /* Use PROJ's proper geodesic API for distance calculation */
        double s12, azi1, azi2;
        geod_inverse(&intern->geod, lat1, lon1, lat2, lon2, &s12, &azi1, &azi2);
        
        add_next_index_double(return_value, s12);
    }
    efree(lon_coords);
    efree(lat_coords);
}
