#include "../php_proj.h"

/* Class entries */
zend_class_entry *proj_area_of_use_ce;
extern zend_class_entry *proj_crs_exception_ce;

zend_object_handlers proj_area_of_use_object_handlers;

/* Method declarations */
static PHP_METHOD(ProjAreaOfUse, __construct);
static PHP_METHOD(ProjAreaOfUse, getWest);
static PHP_METHOD(ProjAreaOfUse, getSouth);
static PHP_METHOD(ProjAreaOfUse, getEast);
static PHP_METHOD(ProjAreaOfUse, getNorth);
static PHP_METHOD(ProjAreaOfUse, getName);
static PHP_METHOD(ProjAreaOfUse, contains);
static PHP_METHOD(ProjAreaOfUse, intersects);
static PHP_METHOD(ProjAreaOfUse, __toString);

/* ArgInfo declarations */
ZEND_BEGIN_ARG_INFO_EX(arginfo_area_of_use_construct, 0, 0, 5)
    ZEND_ARG_TYPE_INFO(0, west, IS_DOUBLE, 0)
    ZEND_ARG_TYPE_INFO(0, south, IS_DOUBLE, 0)
    ZEND_ARG_TYPE_INFO(0, east, IS_DOUBLE, 0)
    ZEND_ARG_TYPE_INFO(0, north, IS_DOUBLE, 0)
    ZEND_ARG_TYPE_INFO(0, name, IS_STRING, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_area_of_use_getWest, 0, 0, IS_DOUBLE, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_area_of_use_getSouth, 0, 0, IS_DOUBLE, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_area_of_use_getEast, 0, 0, IS_DOUBLE, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_area_of_use_getNorth, 0, 0, IS_DOUBLE, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_area_of_use_getName, 0, 0, IS_STRING, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_area_of_use_contains, 0, 1, _IS_BOOL, 0)
    ZEND_ARG_OBJ_INFO(0, other, ProjAreaOfUse, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_area_of_use_intersects, 0, 1, _IS_BOOL, 0)
    ZEND_ARG_OBJ_INFO(0, other, ProjAreaOfUse, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_area_of_use_toString, 0, 0, IS_STRING, 0)
ZEND_END_ARG_INFO()

/* Method table */
static const zend_function_entry proj_area_of_use_methods[] = {
    PHP_ME(ProjAreaOfUse, __construct, arginfo_area_of_use_construct, ZEND_ACC_PUBLIC)
    PHP_ME(ProjAreaOfUse, getWest, arginfo_area_of_use_getWest, ZEND_ACC_PUBLIC)
    PHP_ME(ProjAreaOfUse, getSouth, arginfo_area_of_use_getSouth, ZEND_ACC_PUBLIC)
    PHP_ME(ProjAreaOfUse, getEast, arginfo_area_of_use_getEast, ZEND_ACC_PUBLIC)
    PHP_ME(ProjAreaOfUse, getNorth, arginfo_area_of_use_getNorth, ZEND_ACC_PUBLIC)
    PHP_ME(ProjAreaOfUse, getName, arginfo_area_of_use_getName, ZEND_ACC_PUBLIC)
    PHP_ME(ProjAreaOfUse, contains, arginfo_area_of_use_contains, ZEND_ACC_PUBLIC)
    PHP_ME(ProjAreaOfUse, intersects, arginfo_area_of_use_intersects, ZEND_ACC_PUBLIC)
    PHP_ME(ProjAreaOfUse, __toString, arginfo_area_of_use_toString, ZEND_ACC_PUBLIC)
    PHP_FE_END
};

/* Initialize ProjAreaOfUse class */
void proj_area_of_use_init(void)
{
    zend_class_entry ce;
    
    INIT_CLASS_ENTRY(ce, "ProjAreaOfUse", proj_area_of_use_methods);
    proj_area_of_use_ce = zend_register_internal_class(&ce);
    proj_area_of_use_ce->create_object = proj_area_of_use_object_create;
    
    memcpy(&proj_area_of_use_object_handlers, &std_object_handlers, sizeof(zend_object_handlers));
    proj_area_of_use_object_handlers.free_obj = proj_area_of_use_object_destroy;
    proj_area_of_use_object_handlers.offset = XtOffsetOf(proj_area_of_use_object, std);
}

/* Object creation */
zend_object *proj_area_of_use_object_create(zend_class_entry *ce)
{
    proj_area_of_use_object *intern = ecalloc(1, sizeof(proj_area_of_use_object) + zend_object_properties_size(ce));
    
    zend_object_std_init(&intern->std, ce);
    object_properties_init(&intern->std, ce);
    
    intern->west = 0.0;
    intern->south = 0.0;
    intern->east = 0.0;
    intern->north = 0.0;
    intern->name = NULL;
    intern->std.handlers = &proj_area_of_use_object_handlers;
    
    return &intern->std;
}

/* Object destruction */
void proj_area_of_use_object_destroy(zend_object *object)
{
    proj_area_of_use_object *intern = AREA_OF_USE_FROM_OBJECT(object);
    
    if (intern->name) {
        efree(intern->name);
        intern->name = NULL;
    }
    
    zend_object_std_dtor(&intern->std);
}

/* Constructor */
static PHP_METHOD(ProjAreaOfUse, __construct)
{
    double west, south, east, north;
    zend_string *name = NULL;
    proj_area_of_use_object *intern;

    ZEND_PARSE_PARAMETERS_START(4, 5)
        Z_PARAM_DOUBLE(west)
        Z_PARAM_DOUBLE(south)
        Z_PARAM_DOUBLE(east)
        Z_PARAM_DOUBLE(north)
        Z_PARAM_OPTIONAL
        Z_PARAM_STR_OR_NULL(name)
    ZEND_PARSE_PARAMETERS_END();

    intern = AREA_OF_USE_FROM_OBJECT(Z_OBJ_P(getThis()));

    /* Validate coordinate bounds */
    if (west < -180.0 || west > 180.0 || east < -180.0 || east > 180.0) {
        proj_throw_exception(proj_crs_exception_ce, "Longitude values must be between -180 and 180 degrees");
        return;
    }
    
    if (south < -90.0 || south > 90.0 || north < -90.0 || north > 90.0) {
        proj_throw_exception(proj_crs_exception_ce, "Latitude values must be between -90 and 90 degrees");
        return;
    }
    
    if (south > north) {
        proj_throw_exception(proj_crs_exception_ce, "South latitude cannot be greater than north latitude");
        return;
    }

    /* Store the area information */
    intern->west = west;
    intern->south = south;
    intern->east = east;
    intern->north = north;
    
    if (name) {
        intern->name = estrdup(ZSTR_VAL(name));
    }
}

/* Get west longitude */
static PHP_METHOD(ProjAreaOfUse, getWest)
{
    proj_area_of_use_object *intern;

    ZEND_PARSE_PARAMETERS_NONE();

    intern = AREA_OF_USE_FROM_OBJECT(Z_OBJ_P(getThis()));
    
    RETURN_DOUBLE(intern->west);
}

/* Get south latitude */
static PHP_METHOD(ProjAreaOfUse, getSouth)
{
    proj_area_of_use_object *intern;

    ZEND_PARSE_PARAMETERS_NONE();

    intern = AREA_OF_USE_FROM_OBJECT(Z_OBJ_P(getThis()));
    
    RETURN_DOUBLE(intern->south);
}

/* Get east longitude */
static PHP_METHOD(ProjAreaOfUse, getEast)
{
    proj_area_of_use_object *intern;

    ZEND_PARSE_PARAMETERS_NONE();

    intern = AREA_OF_USE_FROM_OBJECT(Z_OBJ_P(getThis()));
    
    RETURN_DOUBLE(intern->east);
}

/* Get north latitude */
static PHP_METHOD(ProjAreaOfUse, getNorth)
{
    proj_area_of_use_object *intern;

    ZEND_PARSE_PARAMETERS_NONE();

    intern = AREA_OF_USE_FROM_OBJECT(Z_OBJ_P(getThis()));
    
    RETURN_DOUBLE(intern->north);
}

/* Get name */
static PHP_METHOD(ProjAreaOfUse, getName)
{
    proj_area_of_use_object *intern;

    ZEND_PARSE_PARAMETERS_NONE();

    intern = AREA_OF_USE_FROM_OBJECT(Z_OBJ_P(getThis()));
    
    if (intern->name) {
        RETURN_STRING(intern->name);
    } else {
        RETURN_NULL();
    }
}

/* Check if this area contains another area */
static PHP_METHOD(ProjAreaOfUse, contains)
{
    zval *other;
    proj_area_of_use_object *intern, *other_intern;

    ZEND_PARSE_PARAMETERS_START(1, 1)
        Z_PARAM_OBJECT_OF_CLASS(other, proj_area_of_use_ce)
    ZEND_PARSE_PARAMETERS_END();

    intern = AREA_OF_USE_FROM_OBJECT(Z_OBJ_P(getThis()));
    other_intern = AREA_OF_USE_FROM_OBJECT(Z_OBJ_P(other));

    /* Check if this area completely contains the other area */
    zend_bool contains = (intern->west <= other_intern->west &&
                         intern->south <= other_intern->south &&
                         intern->east >= other_intern->east &&
                         intern->north >= other_intern->north);

    RETURN_BOOL(contains);
}

/* Check if this area intersects with another area */
static PHP_METHOD(ProjAreaOfUse, intersects)
{
    zval *other;
    proj_area_of_use_object *intern, *other_intern;

    ZEND_PARSE_PARAMETERS_START(1, 1)
        Z_PARAM_OBJECT_OF_CLASS(other, proj_area_of_use_ce)
    ZEND_PARSE_PARAMETERS_END();

    intern = AREA_OF_USE_FROM_OBJECT(Z_OBJ_P(getThis()));
    other_intern = AREA_OF_USE_FROM_OBJECT(Z_OBJ_P(other));

    /* Check if the areas intersect (overlap) */
    zend_bool intersects = !(intern->east < other_intern->west ||
                            intern->west > other_intern->east ||
                            intern->north < other_intern->south ||
                            intern->south > other_intern->north);

    RETURN_BOOL(intersects);
}

/* String representation */
static PHP_METHOD(ProjAreaOfUse, __toString)
{
    proj_area_of_use_object *intern;
    char *result;

    ZEND_PARSE_PARAMETERS_NONE();

    intern = AREA_OF_USE_FROM_OBJECT(Z_OBJ_P(getThis()));
    
    if (intern->name) {
        spprintf(&result, 0, "%s (%.4f°W, %.4f°S, %.4f°E, %.4f°N)", 
                 intern->name, intern->west, intern->south, intern->east, intern->north);
    } else {
        spprintf(&result, 0, "Area of Use (%.4f°W, %.4f°S, %.4f°E, %.4f°N)", 
                 intern->west, intern->south, intern->east, intern->north);
    }
    
    RETVAL_STRING(result);
    efree(result);
}