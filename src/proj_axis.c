#include "../php_proj.h"

/* Class entries */
zend_class_entry *proj_axis_ce;
extern zend_class_entry *proj_crs_exception_ce;

zend_object_handlers proj_axis_object_handlers;

/* Method declarations */
static PHP_METHOD(ProjAxis, __construct);
static PHP_METHOD(ProjAxis, getName);
static PHP_METHOD(ProjAxis, getAbbrev);
static PHP_METHOD(ProjAxis, getDirection);
static PHP_METHOD(ProjAxis, getUnitName);
static PHP_METHOD(ProjAxis, getUnitAuthCode);
static PHP_METHOD(ProjAxis, getUnitCode);
static PHP_METHOD(ProjAxis, getUnitConversionFactor);
static PHP_METHOD(ProjAxis, __toString);

/* ArgInfo declarations */
ZEND_BEGIN_ARG_INFO_EX(arginfo_axis_construct, 0, 0, 7)
    ZEND_ARG_TYPE_INFO(0, name, IS_STRING, 0)
    ZEND_ARG_TYPE_INFO(0, abbrev, IS_STRING, 0)
    ZEND_ARG_TYPE_INFO(0, direction, IS_STRING, 0)
    ZEND_ARG_TYPE_INFO(0, unit_name, IS_STRING, 0)
    ZEND_ARG_TYPE_INFO(0, unit_auth_code, IS_STRING, 1)
    ZEND_ARG_TYPE_INFO(0, unit_code, IS_STRING, 1)
    ZEND_ARG_TYPE_INFO(0, unit_conversion_factor, IS_DOUBLE, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_axis_getName, 0, 0, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_axis_getAbbrev, 0, 0, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_axis_getDirection, 0, 0, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_axis_getUnitName, 0, 0, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_axis_getUnitAuthCode, 0, 0, IS_STRING, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_axis_getUnitCode, 0, 0, IS_STRING, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_axis_getUnitConversionFactor, 0, 0, IS_DOUBLE, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_axis_toString, 0, 0, IS_STRING, 0)
ZEND_END_ARG_INFO()

/* Method table */
static const zend_function_entry proj_axis_methods[] = {
    PHP_ME(ProjAxis, __construct, arginfo_axis_construct, ZEND_ACC_PUBLIC)
    PHP_ME(ProjAxis, getName, arginfo_axis_getName, ZEND_ACC_PUBLIC)
    PHP_ME(ProjAxis, getAbbrev, arginfo_axis_getAbbrev, ZEND_ACC_PUBLIC)
    PHP_ME(ProjAxis, getDirection, arginfo_axis_getDirection, ZEND_ACC_PUBLIC)
    PHP_ME(ProjAxis, getUnitName, arginfo_axis_getUnitName, ZEND_ACC_PUBLIC)
    PHP_ME(ProjAxis, getUnitAuthCode, arginfo_axis_getUnitAuthCode, ZEND_ACC_PUBLIC)
    PHP_ME(ProjAxis, getUnitCode, arginfo_axis_getUnitCode, ZEND_ACC_PUBLIC)
    PHP_ME(ProjAxis, getUnitConversionFactor, arginfo_axis_getUnitConversionFactor, ZEND_ACC_PUBLIC)
    PHP_ME(ProjAxis, __toString, arginfo_axis_toString, ZEND_ACC_PUBLIC)
    PHP_FE_END
};

/* Initialize ProjAxis class */
void proj_axis_init(void)
{
    zend_class_entry ce;
    
    INIT_CLASS_ENTRY(ce, "ProjAxis", proj_axis_methods);
    proj_axis_ce = zend_register_internal_class(&ce);
    proj_axis_ce->create_object = proj_axis_object_create;
    
    memcpy(&proj_axis_object_handlers, &std_object_handlers, sizeof(zend_object_handlers));
    proj_axis_object_handlers.free_obj = proj_axis_object_destroy;
    proj_axis_object_handlers.offset = XtOffsetOf(proj_axis_object, std);
}

/* Object creation */
zend_object *proj_axis_object_create(zend_class_entry *ce)
{
    proj_axis_object *intern = ecalloc(1, sizeof(proj_axis_object) + zend_object_properties_size(ce));
    
    zend_object_std_init(&intern->std, ce);
    object_properties_init(&intern->std, ce);
    
    intern->name = NULL;
    intern->abbrev = NULL;
    intern->direction = NULL;
    intern->unit_name = NULL;
    intern->unit_auth_code = NULL;
    intern->unit_code = NULL;
    intern->unit_conversion_factor = 0.0;
    intern->std.handlers = &proj_axis_object_handlers;
    
    return &intern->std;
}

/* Object destruction */
void proj_axis_object_destroy(zend_object *object)
{
    proj_axis_object *intern = AXIS_FROM_OBJECT(object);
    
    if (intern->name) {
        efree(intern->name);
        intern->name = NULL;
    }
    if (intern->abbrev) {
        efree(intern->abbrev);
        intern->abbrev = NULL;
    }
    if (intern->direction) {
        efree(intern->direction);
        intern->direction = NULL;
    }
    if (intern->unit_name) {
        efree(intern->unit_name);
        intern->unit_name = NULL;
    }
    if (intern->unit_auth_code) {
        efree(intern->unit_auth_code);
        intern->unit_auth_code = NULL;
    }
    if (intern->unit_code) {
        efree(intern->unit_code);
        intern->unit_code = NULL;
    }
    
    zend_object_std_dtor(&intern->std);
}

/* Constructor */
static PHP_METHOD(ProjAxis, __construct)
{
    zend_string *name, *abbrev, *direction, *unit_name;
    zend_string *unit_auth_code = NULL, *unit_code = NULL;
    double unit_conversion_factor;
    proj_axis_object *intern;

    ZEND_PARSE_PARAMETERS_START(5, 7)
        Z_PARAM_STR(name)
        Z_PARAM_STR(abbrev)
        Z_PARAM_STR(direction)
        Z_PARAM_STR(unit_name)
        Z_PARAM_OPTIONAL
        Z_PARAM_STR_OR_NULL(unit_auth_code)
        Z_PARAM_STR_OR_NULL(unit_code)
        Z_PARAM_DOUBLE(unit_conversion_factor)
    ZEND_PARSE_PARAMETERS_END();

    intern = AXIS_FROM_OBJECT(Z_OBJ_P(getThis()));

    /* Store the axis information */
    intern->name = estrdup(ZSTR_VAL(name));
    intern->abbrev = estrdup(ZSTR_VAL(abbrev));
    intern->direction = estrdup(ZSTR_VAL(direction));
    intern->unit_name = estrdup(ZSTR_VAL(unit_name));
    
    if (unit_auth_code) {
        intern->unit_auth_code = estrdup(ZSTR_VAL(unit_auth_code));
    }
    if (unit_code) {
        intern->unit_code = estrdup(ZSTR_VAL(unit_code));
    }
    
    intern->unit_conversion_factor = unit_conversion_factor;
}

/* Get name */
static PHP_METHOD(ProjAxis, getName)
{
    proj_axis_object *intern;

    ZEND_PARSE_PARAMETERS_NONE();

    intern = AXIS_FROM_OBJECT(Z_OBJ_P(getThis()));
    
    if (intern->name) {
        RETURN_STRING(intern->name);
    } else {
        RETURN_STRING("");
    }
}

/* Get abbreviation */
static PHP_METHOD(ProjAxis, getAbbrev)
{
    proj_axis_object *intern;

    ZEND_PARSE_PARAMETERS_NONE();

    intern = AXIS_FROM_OBJECT(Z_OBJ_P(getThis()));
    
    if (intern->abbrev) {
        RETURN_STRING(intern->abbrev);
    } else {
        RETURN_STRING("");
    }
}

/* Get direction */
static PHP_METHOD(ProjAxis, getDirection)
{
    proj_axis_object *intern;

    ZEND_PARSE_PARAMETERS_NONE();

    intern = AXIS_FROM_OBJECT(Z_OBJ_P(getThis()));
    
    if (intern->direction) {
        RETURN_STRING(intern->direction);
    } else {
        RETURN_STRING("");
    }
}

/* Get unit name */
static PHP_METHOD(ProjAxis, getUnitName)
{
    proj_axis_object *intern;

    ZEND_PARSE_PARAMETERS_NONE();

    intern = AXIS_FROM_OBJECT(Z_OBJ_P(getThis()));
    
    if (intern->unit_name) {
        RETURN_STRING(intern->unit_name);
    } else {
        RETURN_STRING("");
    }
}

/* Get unit auth code */
static PHP_METHOD(ProjAxis, getUnitAuthCode)
{
    proj_axis_object *intern;

    ZEND_PARSE_PARAMETERS_NONE();

    intern = AXIS_FROM_OBJECT(Z_OBJ_P(getThis()));
    
    if (intern->unit_auth_code) {
        RETURN_STRING(intern->unit_auth_code);
    } else {
        RETURN_NULL();
    }
}

/* Get unit code */
static PHP_METHOD(ProjAxis, getUnitCode)
{
    proj_axis_object *intern;

    ZEND_PARSE_PARAMETERS_NONE();

    intern = AXIS_FROM_OBJECT(Z_OBJ_P(getThis()));
    
    if (intern->unit_code) {
        RETURN_STRING(intern->unit_code);
    } else {
        RETURN_NULL();
    }
}

/* Get unit conversion factor */
static PHP_METHOD(ProjAxis, getUnitConversionFactor)
{
    proj_axis_object *intern;

    ZEND_PARSE_PARAMETERS_NONE();

    intern = AXIS_FROM_OBJECT(Z_OBJ_P(getThis()));
    
    RETURN_DOUBLE(intern->unit_conversion_factor);
}

/* String representation */
static PHP_METHOD(ProjAxis, __toString)
{
    proj_axis_object *intern;
    char *result;

    ZEND_PARSE_PARAMETERS_NONE();

    intern = AXIS_FROM_OBJECT(Z_OBJ_P(getThis()));
    
    spprintf(&result, 0, "%s (%s) [%s]", 
             intern->name ? intern->name : "Unknown",
             intern->abbrev ? intern->abbrev : "",
             intern->direction ? intern->direction : "");
    
    RETVAL_STRING(result);
    efree(result);
}