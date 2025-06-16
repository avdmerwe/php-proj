#include "../php_proj.h"

/* Class entry */
zend_class_entry *proj_unit_ce;

/* Object handlers */
zend_object_handlers proj_unit_object_handlers;

/* Method argument info */
ZEND_BEGIN_ARG_INFO_EX(arginfo_proj_unit_construct, 0, 0, 6)
    ZEND_ARG_TYPE_INFO(0, auth_name, IS_STRING, 1)
    ZEND_ARG_TYPE_INFO(0, code, IS_STRING, 1)
    ZEND_ARG_TYPE_INFO(0, name, IS_STRING, 1)
    ZEND_ARG_TYPE_INFO(0, category, IS_STRING, 1)
    ZEND_ARG_TYPE_INFO(0, conv_factor, IS_DOUBLE, 0)
    ZEND_ARG_TYPE_INFO(0, proj_short_name, IS_STRING, 1)
    ZEND_ARG_TYPE_INFO(0, deprecated, _IS_BOOL, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_proj_unit_get_string, 0, 0, IS_STRING, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_proj_unit_get_double, 0, 0, IS_DOUBLE, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_proj_unit_get_bool, 0, 0, _IS_BOOL, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_proj_unit_to_array, 0, 0, IS_ARRAY, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_proj_unit_to_string, 0, 0, IS_STRING, 0)
ZEND_END_ARG_INFO()

/* ProjUnit constructor */
PHP_METHOD(ProjUnit, __construct)
{
    char *auth_name = NULL, *code = NULL, *name = NULL, *category = NULL, *proj_short_name = NULL;
    size_t auth_name_len = 0, code_len = 0, name_len = 0, category_len = 0, proj_short_name_len = 0;
    double conv_factor = 1.0;
    zend_bool deprecated = 0;
    proj_unit_object *intern;

    ZEND_PARSE_PARAMETERS_START(6, 7)
        Z_PARAM_STRING_OR_NULL(auth_name, auth_name_len)
        Z_PARAM_STRING_OR_NULL(code, code_len)
        Z_PARAM_STRING_OR_NULL(name, name_len)
        Z_PARAM_STRING_OR_NULL(category, category_len)
        Z_PARAM_DOUBLE(conv_factor)
        Z_PARAM_STRING_OR_NULL(proj_short_name, proj_short_name_len)
        Z_PARAM_OPTIONAL
        Z_PARAM_BOOL(deprecated)
    ZEND_PARSE_PARAMETERS_END();

    intern = UNIT_FROM_OBJECT(Z_OBJ_P(getThis()));
    
    /* Copy strings */
    intern->auth_name = auth_name ? estrdup(auth_name) : NULL;
    intern->code = code ? estrdup(code) : NULL;
    intern->name = name ? estrdup(name) : NULL;
    intern->category = category ? estrdup(category) : NULL;
    intern->proj_short_name = proj_short_name ? estrdup(proj_short_name) : NULL;
    intern->conv_factor = conv_factor;
    intern->deprecated = deprecated;
    
    /* Update the properties */
    if (auth_name) {
        zend_update_property_string(proj_unit_ce, Z_OBJ_P(getThis()), "auth_name", strlen("auth_name"), auth_name);
    } else {
        zend_update_property_null(proj_unit_ce, Z_OBJ_P(getThis()), "auth_name", strlen("auth_name"));
    }
    
    if (code) {
        zend_update_property_string(proj_unit_ce, Z_OBJ_P(getThis()), "code", strlen("code"), code);
    } else {
        zend_update_property_null(proj_unit_ce, Z_OBJ_P(getThis()), "code", strlen("code"));
    }
    
    if (name) {
        zend_update_property_string(proj_unit_ce, Z_OBJ_P(getThis()), "name", strlen("name"), name);
    } else {
        zend_update_property_null(proj_unit_ce, Z_OBJ_P(getThis()), "name", strlen("name"));
    }
    
    if (category) {
        zend_update_property_string(proj_unit_ce, Z_OBJ_P(getThis()), "category", strlen("category"), category);
    } else {
        zend_update_property_null(proj_unit_ce, Z_OBJ_P(getThis()), "category", strlen("category"));
    }
    
    if (proj_short_name) {
        zend_update_property_string(proj_unit_ce, Z_OBJ_P(getThis()), "proj_short_name", strlen("proj_short_name"), proj_short_name);
    } else {
        zend_update_property_null(proj_unit_ce, Z_OBJ_P(getThis()), "proj_short_name", strlen("proj_short_name"));
    }
    
    zend_update_property_double(proj_unit_ce, Z_OBJ_P(getThis()), "conv_factor", strlen("conv_factor"), conv_factor);
    zend_update_property_bool(proj_unit_ce, Z_OBJ_P(getThis()), "deprecated", strlen("deprecated"), deprecated);
}

/* Property getter methods */
PHP_METHOD(ProjUnit, getAuthName)
{
    proj_unit_object *intern;

    ZEND_PARSE_PARAMETERS_NONE();

    intern = UNIT_FROM_OBJECT(Z_OBJ_P(getThis()));
    
    if (intern->auth_name) {
        RETURN_STRING(intern->auth_name);
    } else {
        RETURN_NULL();
    }
}

PHP_METHOD(ProjUnit, getCode)
{
    proj_unit_object *intern;

    ZEND_PARSE_PARAMETERS_NONE();

    intern = UNIT_FROM_OBJECT(Z_OBJ_P(getThis()));
    
    if (intern->code) {
        RETURN_STRING(intern->code);
    } else {
        RETURN_NULL();
    }
}

PHP_METHOD(ProjUnit, getName)
{
    proj_unit_object *intern;

    ZEND_PARSE_PARAMETERS_NONE();

    intern = UNIT_FROM_OBJECT(Z_OBJ_P(getThis()));
    
    if (intern->name) {
        RETURN_STRING(intern->name);
    } else {
        RETURN_NULL();
    }
}

PHP_METHOD(ProjUnit, getCategory)
{
    proj_unit_object *intern;

    ZEND_PARSE_PARAMETERS_NONE();

    intern = UNIT_FROM_OBJECT(Z_OBJ_P(getThis()));
    
    if (intern->category) {
        RETURN_STRING(intern->category);
    } else {
        RETURN_NULL();
    }
}

PHP_METHOD(ProjUnit, getProjShortName)
{
    proj_unit_object *intern;

    ZEND_PARSE_PARAMETERS_NONE();

    intern = UNIT_FROM_OBJECT(Z_OBJ_P(getThis()));
    
    if (intern->proj_short_name) {
        RETURN_STRING(intern->proj_short_name);
    } else {
        RETURN_NULL();
    }
}

PHP_METHOD(ProjUnit, getConvFactor)
{
    proj_unit_object *intern;

    ZEND_PARSE_PARAMETERS_NONE();

    intern = UNIT_FROM_OBJECT(Z_OBJ_P(getThis()));
    RETURN_DOUBLE(intern->conv_factor);
}

PHP_METHOD(ProjUnit, isDeprecated)
{
    proj_unit_object *intern;

    ZEND_PARSE_PARAMETERS_NONE();

    intern = UNIT_FROM_OBJECT(Z_OBJ_P(getThis()));
    RETURN_BOOL(intern->deprecated);
}

/* Convert unit to associative array */
PHP_METHOD(ProjUnit, toArray)
{
    proj_unit_object *intern;

    ZEND_PARSE_PARAMETERS_NONE();

    intern = UNIT_FROM_OBJECT(Z_OBJ_P(getThis()));

    array_init(return_value);
    
    if (intern->auth_name) {
        add_assoc_string(return_value, "auth_name", intern->auth_name);
    } else {
        add_assoc_null(return_value, "auth_name");
    }
    
    if (intern->code) {
        add_assoc_string(return_value, "code", intern->code);
    } else {
        add_assoc_null(return_value, "code");
    }
    
    if (intern->name) {
        add_assoc_string(return_value, "name", intern->name);
    } else {
        add_assoc_null(return_value, "name");
    }
    
    if (intern->category) {
        add_assoc_string(return_value, "category", intern->category);
    } else {
        add_assoc_null(return_value, "category");
    }
    
    if (intern->proj_short_name) {
        add_assoc_string(return_value, "proj_short_name", intern->proj_short_name);
    } else {
        add_assoc_null(return_value, "proj_short_name");
    }
    
    add_assoc_double(return_value, "conv_factor", intern->conv_factor);
    add_assoc_bool(return_value, "deprecated", intern->deprecated);
}

/* String representation */
PHP_METHOD(ProjUnit, __toString)
{
    proj_unit_object *intern;
    zend_string *result;

    ZEND_PARSE_PARAMETERS_NONE();

    intern = UNIT_FROM_OBJECT(Z_OBJ_P(getThis()));

    result = zend_strpprintf(0, 
        "ProjUnit(auth_name=%s, code=%s, name=%s, category=%s, conv_factor=%.6f, proj_short_name=%s, deprecated=%s)",
        intern->auth_name ? intern->auth_name : "null",
        intern->code ? intern->code : "null", 
        intern->name ? intern->name : "null",
        intern->category ? intern->category : "null",
        intern->conv_factor,
        intern->proj_short_name ? intern->proj_short_name : "null",
        intern->deprecated ? "true" : "false"
    );

    RETURN_STR(result);
}

/* Method entries for ProjUnit */
static const zend_function_entry proj_unit_methods[] = {
    PHP_ME(ProjUnit, __construct, arginfo_proj_unit_construct, ZEND_ACC_PUBLIC)
    PHP_ME(ProjUnit, getAuthName, arginfo_proj_unit_get_string, ZEND_ACC_PUBLIC)
    PHP_ME(ProjUnit, getCode, arginfo_proj_unit_get_string, ZEND_ACC_PUBLIC)
    PHP_ME(ProjUnit, getName, arginfo_proj_unit_get_string, ZEND_ACC_PUBLIC)
    PHP_ME(ProjUnit, getCategory, arginfo_proj_unit_get_string, ZEND_ACC_PUBLIC)
    PHP_ME(ProjUnit, getProjShortName, arginfo_proj_unit_get_string, ZEND_ACC_PUBLIC)
    PHP_ME(ProjUnit, getConvFactor, arginfo_proj_unit_get_double, ZEND_ACC_PUBLIC)
    PHP_ME(ProjUnit, isDeprecated, arginfo_proj_unit_get_bool, ZEND_ACC_PUBLIC)
    PHP_ME(ProjUnit, toArray, arginfo_proj_unit_to_array, ZEND_ACC_PUBLIC)
    PHP_ME(ProjUnit, __toString, arginfo_proj_unit_to_string, ZEND_ACC_PUBLIC)
    PHP_FE_END
};

/* Object creation function */
zend_object *proj_unit_object_create(zend_class_entry *ce)
{
    proj_unit_object *intern = ecalloc(1, sizeof(proj_unit_object) + zend_object_properties_size(ce));
    
    zend_object_std_init(&intern->std, ce);
    object_properties_init(&intern->std, ce);
    
    intern->std.handlers = &proj_unit_object_handlers;
    
    return &intern->std;
}

/* Object destruction function */
void proj_unit_object_destroy(zend_object *object)
{
    proj_unit_object *intern = UNIT_FROM_OBJECT(object);
    
    if (intern->auth_name) {
        efree(intern->auth_name);
    }
    if (intern->code) {
        efree(intern->code);
    }
    if (intern->name) {
        efree(intern->name);
    }
    if (intern->category) {
        efree(intern->category);
    }
    if (intern->proj_short_name) {
        efree(intern->proj_short_name);
    }
    
    zend_object_std_dtor(&intern->std);
}

/* Initialize ProjUnit class */
void proj_unit_init(void)
{
    zend_class_entry ce;

    /* Unit class */
    INIT_CLASS_ENTRY(ce, "ProjUnit", proj_unit_methods);
    proj_unit_ce = zend_register_internal_class(&ce);
    proj_unit_ce->create_object = proj_unit_object_create;
    
    memcpy(&proj_unit_object_handlers, &std_object_handlers, sizeof(zend_object_handlers));
    proj_unit_object_handlers.free_obj = proj_unit_object_destroy;
    proj_unit_object_handlers.offset = XtOffsetOf(proj_unit_object, std);

    /* Add public properties */
    zend_declare_property_null(proj_unit_ce, "auth_name", strlen("auth_name"), ZEND_ACC_PUBLIC);
    zend_declare_property_null(proj_unit_ce, "code", strlen("code"), ZEND_ACC_PUBLIC);
    zend_declare_property_null(proj_unit_ce, "name", strlen("name"), ZEND_ACC_PUBLIC);
    zend_declare_property_null(proj_unit_ce, "category", strlen("category"), ZEND_ACC_PUBLIC);
    zend_declare_property_null(proj_unit_ce, "proj_short_name", strlen("proj_short_name"), ZEND_ACC_PUBLIC);
    zend_declare_property_double(proj_unit_ce, "conv_factor", strlen("conv_factor"), 1.0, ZEND_ACC_PUBLIC);
    zend_declare_property_bool(proj_unit_ce, "deprecated", strlen("deprecated"), 0, ZEND_ACC_PUBLIC);
}