#include "../php_proj.h"

/* Class entry */
zend_class_entry *proj_area_of_interest_ce;

/* Object handlers */
zend_object_handlers proj_area_of_interest_object_handlers;

/* Method argument info */
ZEND_BEGIN_ARG_INFO_EX(arginfo_proj_area_of_interest_construct, 0, 0, 4)
    ZEND_ARG_TYPE_INFO(0, west_lon_degree, IS_DOUBLE, 0)
    ZEND_ARG_TYPE_INFO(0, south_lat_degree, IS_DOUBLE, 0)
    ZEND_ARG_TYPE_INFO(0, east_lon_degree, IS_DOUBLE, 0)
    ZEND_ARG_TYPE_INFO(0, north_lat_degree, IS_DOUBLE, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_proj_area_of_interest_contains, 0, 1, _IS_BOOL, 0)
    ZEND_ARG_OBJ_INFO(0, other, ProjAreaOfInterest, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_proj_area_of_interest_intersects, 0, 1, _IS_BOOL, 0)
    ZEND_ARG_OBJ_INFO(0, other, ProjAreaOfInterest, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_proj_area_of_interest_get_coord, 0, 0, IS_DOUBLE, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_proj_area_of_interest_to_string, 0, 0, IS_STRING, 0)
ZEND_END_ARG_INFO()

/* ProjAreaOfInterest constructor */
PHP_METHOD(ProjAreaOfInterest, __construct)
{
    zval *west_zv, *south_zv, *east_zv, *north_zv;
    double west, south, east, north;
    proj_area_of_interest_object *intern;

    ZEND_PARSE_PARAMETERS_START(4, 4)
        Z_PARAM_ZVAL(west_zv)
        Z_PARAM_ZVAL(south_zv)
        Z_PARAM_ZVAL(east_zv)
        Z_PARAM_ZVAL(north_zv)
    ZEND_PARSE_PARAMETERS_END();

    /* Check for NULL values */
    if (Z_TYPE_P(west_zv) == IS_NULL || Z_TYPE_P(south_zv) == IS_NULL ||
        Z_TYPE_P(east_zv) == IS_NULL || Z_TYPE_P(north_zv) == IS_NULL) {
        zend_throw_exception(zend_ce_value_error, "NaN or None values are not allowed.", 0);
        RETURN_THROWS();
    }

    /* Convert to doubles */
    west = zval_get_double(west_zv);
    south = zval_get_double(south_zv);
    east = zval_get_double(east_zv);
    north = zval_get_double(north_zv);

    /* Validate coordinates */
    if (isnan(west) || isnan(south) || isnan(east) || isnan(north)) {
        zend_throw_exception(zend_ce_value_error, "NaN or None values are not allowed.", 0);
        RETURN_THROWS();
    }

    if (west >= east) {
        zend_throw_exception(zend_ce_value_error, "West longitude must be less than east longitude.", 0);
        RETURN_THROWS();
    }

    if (south >= north) {
        zend_throw_exception(zend_ce_value_error, "South latitude must be less than north latitude.", 0);
        RETURN_THROWS();
    }

    intern = AREA_OF_INTEREST_FROM_OBJECT(Z_OBJ_P(getThis()));
    intern->west_lon_degree = west;
    intern->south_lat_degree = south;
    intern->east_lon_degree = east;
    intern->north_lat_degree = north;
    
    /* Update the properties */
    zend_update_property_double(proj_area_of_interest_ce, Z_OBJ_P(getThis()), "west_lon_degree", strlen("west_lon_degree"), west);
    zend_update_property_double(proj_area_of_interest_ce, Z_OBJ_P(getThis()), "south_lat_degree", strlen("south_lat_degree"), south);
    zend_update_property_double(proj_area_of_interest_ce, Z_OBJ_P(getThis()), "east_lon_degree", strlen("east_lon_degree"), east);
    zend_update_property_double(proj_area_of_interest_ce, Z_OBJ_P(getThis()), "north_lat_degree", strlen("north_lat_degree"), north);
}

/* ProjAreaOfInterest contains method */
PHP_METHOD(ProjAreaOfInterest, contains)
{
    zval *other_zv;
    proj_area_of_interest_object *intern, *other;

    ZEND_PARSE_PARAMETERS_START(1, 1)
        Z_PARAM_OBJECT_OF_CLASS(other_zv, proj_area_of_interest_ce)
    ZEND_PARSE_PARAMETERS_END();

    intern = AREA_OF_INTEREST_FROM_OBJECT(Z_OBJ_P(getThis()));
    other = AREA_OF_INTEREST_FROM_OBJECT(Z_OBJ_P(other_zv));

    /* Check if this AOI completely contains the other AOI */
    zend_bool contains = (intern->west_lon_degree <= other->west_lon_degree &&
                         intern->south_lat_degree <= other->south_lat_degree &&
                         intern->east_lon_degree >= other->east_lon_degree &&
                         intern->north_lat_degree >= other->north_lat_degree);

    RETURN_BOOL(contains);
}

/* ProjAreaOfInterest intersects method */
PHP_METHOD(ProjAreaOfInterest, intersects)
{
    zval *other_zv;
    proj_area_of_interest_object *intern, *other;

    ZEND_PARSE_PARAMETERS_START(1, 1)
        Z_PARAM_OBJECT_OF_CLASS(other_zv, proj_area_of_interest_ce)
    ZEND_PARSE_PARAMETERS_END();

    intern = AREA_OF_INTEREST_FROM_OBJECT(Z_OBJ_P(getThis()));
    other = AREA_OF_INTEREST_FROM_OBJECT(Z_OBJ_P(other_zv));

    /* Check if the AOIs intersect */
    zend_bool intersects = !(intern->east_lon_degree <= other->west_lon_degree ||
                            intern->west_lon_degree >= other->east_lon_degree ||
                            intern->north_lat_degree <= other->south_lat_degree ||
                            intern->south_lat_degree >= other->north_lat_degree);

    RETURN_BOOL(intersects);
}

/* Property getter methods */
PHP_METHOD(ProjAreaOfInterest, getWest)
{
    proj_area_of_interest_object *intern;

    ZEND_PARSE_PARAMETERS_NONE();

    intern = AREA_OF_INTEREST_FROM_OBJECT(Z_OBJ_P(getThis()));
    RETURN_DOUBLE(intern->west_lon_degree);
}

PHP_METHOD(ProjAreaOfInterest, getSouth)
{
    proj_area_of_interest_object *intern;

    ZEND_PARSE_PARAMETERS_NONE();

    intern = AREA_OF_INTEREST_FROM_OBJECT(Z_OBJ_P(getThis()));
    RETURN_DOUBLE(intern->south_lat_degree);
}

PHP_METHOD(ProjAreaOfInterest, getEast)
{
    proj_area_of_interest_object *intern;

    ZEND_PARSE_PARAMETERS_NONE();

    intern = AREA_OF_INTEREST_FROM_OBJECT(Z_OBJ_P(getThis()));
    RETURN_DOUBLE(intern->east_lon_degree);
}

PHP_METHOD(ProjAreaOfInterest, getNorth)
{
    proj_area_of_interest_object *intern;

    ZEND_PARSE_PARAMETERS_NONE();

    intern = AREA_OF_INTEREST_FROM_OBJECT(Z_OBJ_P(getThis()));
    RETURN_DOUBLE(intern->north_lat_degree);
}

/* ProjAreaOfInterest __toString method */
PHP_METHOD(ProjAreaOfInterest, __toString)
{
    proj_area_of_interest_object *intern;
    zend_string *str;

    ZEND_PARSE_PARAMETERS_NONE();

    intern = AREA_OF_INTEREST_FROM_OBJECT(Z_OBJ_P(getThis()));
    
    str = zend_strpprintf(0, "AreaOfInterest(west=%.6f, south=%.6f, east=%.6f, north=%.6f)",
                         intern->west_lon_degree, intern->south_lat_degree,
                         intern->east_lon_degree, intern->north_lat_degree);
    
    RETURN_STR(str);
}

/* Method entries for ProjAreaOfInterest */
static const zend_function_entry proj_area_of_interest_methods[] = {
    PHP_ME(ProjAreaOfInterest, __construct, arginfo_proj_area_of_interest_construct, ZEND_ACC_PUBLIC)
    PHP_ME(ProjAreaOfInterest, contains, arginfo_proj_area_of_interest_contains, ZEND_ACC_PUBLIC)
    PHP_ME(ProjAreaOfInterest, intersects, arginfo_proj_area_of_interest_intersects, ZEND_ACC_PUBLIC)
    PHP_ME(ProjAreaOfInterest, getWest, arginfo_proj_area_of_interest_get_coord, ZEND_ACC_PUBLIC)
    PHP_ME(ProjAreaOfInterest, getSouth, arginfo_proj_area_of_interest_get_coord, ZEND_ACC_PUBLIC)
    PHP_ME(ProjAreaOfInterest, getEast, arginfo_proj_area_of_interest_get_coord, ZEND_ACC_PUBLIC)
    PHP_ME(ProjAreaOfInterest, getNorth, arginfo_proj_area_of_interest_get_coord, ZEND_ACC_PUBLIC)
    PHP_ME(ProjAreaOfInterest, __toString, arginfo_proj_area_of_interest_to_string, ZEND_ACC_PUBLIC)
    PHP_FE_END
};

/* Object creation function */
zend_object *proj_area_of_interest_object_create(zend_class_entry *ce)
{
    proj_area_of_interest_object *intern = ecalloc(1, sizeof(proj_area_of_interest_object) + zend_object_properties_size(ce));
    
    zend_object_std_init(&intern->std, ce);
    object_properties_init(&intern->std, ce);
    
    intern->std.handlers = &proj_area_of_interest_object_handlers;
    
    return &intern->std;
}

/* Object destruction function */
void proj_area_of_interest_object_destroy(zend_object *object)
{
    proj_area_of_interest_object *intern = AREA_OF_INTEREST_FROM_OBJECT(object);
    
    zend_object_std_dtor(&intern->std);
}

/* Initialize ProjAreaOfInterest class */
void proj_area_of_interest_init(void)
{
    zend_class_entry ce;

    /* AreaOfInterest class */
    INIT_CLASS_ENTRY(ce, "ProjAreaOfInterest", proj_area_of_interest_methods);
    proj_area_of_interest_ce = zend_register_internal_class(&ce);
    proj_area_of_interest_ce->create_object = proj_area_of_interest_object_create;
    
    memcpy(&proj_area_of_interest_object_handlers, &std_object_handlers, sizeof(zend_object_handlers));
    proj_area_of_interest_object_handlers.free_obj = proj_area_of_interest_object_destroy;
    proj_area_of_interest_object_handlers.offset = XtOffsetOf(proj_area_of_interest_object, std);

    /* Add public properties */
    zend_declare_property_double(proj_area_of_interest_ce, "west_lon_degree", strlen("west_lon_degree"), 0.0, ZEND_ACC_PUBLIC);
    zend_declare_property_double(proj_area_of_interest_ce, "south_lat_degree", strlen("south_lat_degree"), 0.0, ZEND_ACC_PUBLIC);
    zend_declare_property_double(proj_area_of_interest_ce, "east_lon_degree", strlen("east_lon_degree"), 0.0, ZEND_ACC_PUBLIC);
    zend_declare_property_double(proj_area_of_interest_ce, "north_lat_degree", strlen("north_lat_degree"), 0.0, ZEND_ACC_PUBLIC);
}