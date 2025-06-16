#include "../php_proj.h"

/* Class entry */
zend_class_entry *proj_factors_ce;

/* Object handlers */
zend_object_handlers proj_factors_object_handlers;

/* Method argument info */
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_proj_factors_get_property, 0, 0, IS_DOUBLE, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_proj_factors_to_array, 0, 0, IS_ARRAY, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_proj_factors_to_string, 0, 0, IS_STRING, 0)
ZEND_END_ARG_INFO()

/* Property getter methods */
PHP_METHOD(ProjFactors, getMeridionalScale)
{
    proj_factors_object *intern;

    ZEND_PARSE_PARAMETERS_NONE();

    intern = FACTORS_FROM_OBJECT(Z_OBJ_P(getThis()));
    RETURN_DOUBLE(intern->meridional_scale);
}

PHP_METHOD(ProjFactors, getParallelScale)
{
    proj_factors_object *intern;

    ZEND_PARSE_PARAMETERS_NONE();

    intern = FACTORS_FROM_OBJECT(Z_OBJ_P(getThis()));
    RETURN_DOUBLE(intern->parallel_scale);
}

PHP_METHOD(ProjFactors, getArealScale)
{
    proj_factors_object *intern;

    ZEND_PARSE_PARAMETERS_NONE();

    intern = FACTORS_FROM_OBJECT(Z_OBJ_P(getThis()));
    RETURN_DOUBLE(intern->areal_scale);
}

PHP_METHOD(ProjFactors, getAngularDistortion)
{
    proj_factors_object *intern;

    ZEND_PARSE_PARAMETERS_NONE();

    intern = FACTORS_FROM_OBJECT(Z_OBJ_P(getThis()));
    RETURN_DOUBLE(intern->angular_distortion);
}

PHP_METHOD(ProjFactors, getMeridianParallelAngle)
{
    proj_factors_object *intern;

    ZEND_PARSE_PARAMETERS_NONE();

    intern = FACTORS_FROM_OBJECT(Z_OBJ_P(getThis()));
    RETURN_DOUBLE(intern->meridian_parallel_angle);
}

PHP_METHOD(ProjFactors, getMeridianConvergence)
{
    proj_factors_object *intern;

    ZEND_PARSE_PARAMETERS_NONE();

    intern = FACTORS_FROM_OBJECT(Z_OBJ_P(getThis()));
    RETURN_DOUBLE(intern->meridian_convergence);
}

PHP_METHOD(ProjFactors, getTissotSemimajor)
{
    proj_factors_object *intern;

    ZEND_PARSE_PARAMETERS_NONE();

    intern = FACTORS_FROM_OBJECT(Z_OBJ_P(getThis()));
    RETURN_DOUBLE(intern->tissot_semimajor);
}

PHP_METHOD(ProjFactors, getTissotSemiminor)
{
    proj_factors_object *intern;

    ZEND_PARSE_PARAMETERS_NONE();

    intern = FACTORS_FROM_OBJECT(Z_OBJ_P(getThis()));
    RETURN_DOUBLE(intern->tissot_semiminor);
}

PHP_METHOD(ProjFactors, getDxDlam)
{
    proj_factors_object *intern;

    ZEND_PARSE_PARAMETERS_NONE();

    intern = FACTORS_FROM_OBJECT(Z_OBJ_P(getThis()));
    RETURN_DOUBLE(intern->dx_dlam);
}

PHP_METHOD(ProjFactors, getDxDphi)
{
    proj_factors_object *intern;

    ZEND_PARSE_PARAMETERS_NONE();

    intern = FACTORS_FROM_OBJECT(Z_OBJ_P(getThis()));
    RETURN_DOUBLE(intern->dx_dphi);
}

PHP_METHOD(ProjFactors, getDyDlam)
{
    proj_factors_object *intern;

    ZEND_PARSE_PARAMETERS_NONE();

    intern = FACTORS_FROM_OBJECT(Z_OBJ_P(getThis()));
    RETURN_DOUBLE(intern->dy_dlam);
}

PHP_METHOD(ProjFactors, getDyDphi)
{
    proj_factors_object *intern;

    ZEND_PARSE_PARAMETERS_NONE();

    intern = FACTORS_FROM_OBJECT(Z_OBJ_P(getThis()));
    RETURN_DOUBLE(intern->dy_dphi);
}

/* Convert factors to associative array */
PHP_METHOD(ProjFactors, toArray)
{
    proj_factors_object *intern;

    ZEND_PARSE_PARAMETERS_NONE();

    intern = FACTORS_FROM_OBJECT(Z_OBJ_P(getThis()));

    array_init(return_value);
    add_assoc_double(return_value, "meridional_scale", intern->meridional_scale);
    add_assoc_double(return_value, "parallel_scale", intern->parallel_scale);
    add_assoc_double(return_value, "areal_scale", intern->areal_scale);
    add_assoc_double(return_value, "angular_distortion", intern->angular_distortion);
    add_assoc_double(return_value, "meridian_parallel_angle", intern->meridian_parallel_angle);
    add_assoc_double(return_value, "meridian_convergence", intern->meridian_convergence);
    add_assoc_double(return_value, "tissot_semimajor", intern->tissot_semimajor);
    add_assoc_double(return_value, "tissot_semiminor", intern->tissot_semiminor);
    add_assoc_double(return_value, "dx_dlam", intern->dx_dlam);
    add_assoc_double(return_value, "dx_dphi", intern->dx_dphi);
    add_assoc_double(return_value, "dy_dlam", intern->dy_dlam);
    add_assoc_double(return_value, "dy_dphi", intern->dy_dphi);
}

/* String representation */
PHP_METHOD(ProjFactors, __toString)
{
    proj_factors_object *intern;
    zend_string *result;

    ZEND_PARSE_PARAMETERS_NONE();

    intern = FACTORS_FROM_OBJECT(Z_OBJ_P(getThis()));

    result = zend_strpprintf(0, 
        "ProjFactors(meridional_scale=%.6f, parallel_scale=%.6f, areal_scale=%.6f, "
        "angular_distortion=%.6f, meridian_parallel_angle=%.6f, meridian_convergence=%.6f, "
        "tissot_semimajor=%.6f, tissot_semiminor=%.6f, "
        "dx_dlam=%.6f, dx_dphi=%.6f, dy_dlam=%.6f, dy_dphi=%.6f)",
        intern->meridional_scale, intern->parallel_scale, intern->areal_scale,
        intern->angular_distortion, intern->meridian_parallel_angle, intern->meridian_convergence,
        intern->tissot_semimajor, intern->tissot_semiminor,
        intern->dx_dlam, intern->dx_dphi, intern->dy_dlam, intern->dy_dphi
    );

    RETURN_STR(result);
}

/* Method entries for ProjFactors */
static const zend_function_entry proj_factors_methods[] = {
    PHP_ME(ProjFactors, getMeridionalScale, arginfo_proj_factors_get_property, ZEND_ACC_PUBLIC)
    PHP_ME(ProjFactors, getParallelScale, arginfo_proj_factors_get_property, ZEND_ACC_PUBLIC)
    PHP_ME(ProjFactors, getArealScale, arginfo_proj_factors_get_property, ZEND_ACC_PUBLIC)
    PHP_ME(ProjFactors, getAngularDistortion, arginfo_proj_factors_get_property, ZEND_ACC_PUBLIC)
    PHP_ME(ProjFactors, getMeridianParallelAngle, arginfo_proj_factors_get_property, ZEND_ACC_PUBLIC)
    PHP_ME(ProjFactors, getMeridianConvergence, arginfo_proj_factors_get_property, ZEND_ACC_PUBLIC)
    PHP_ME(ProjFactors, getTissotSemimajor, arginfo_proj_factors_get_property, ZEND_ACC_PUBLIC)
    PHP_ME(ProjFactors, getTissotSemiminor, arginfo_proj_factors_get_property, ZEND_ACC_PUBLIC)
    PHP_ME(ProjFactors, getDxDlam, arginfo_proj_factors_get_property, ZEND_ACC_PUBLIC)
    PHP_ME(ProjFactors, getDxDphi, arginfo_proj_factors_get_property, ZEND_ACC_PUBLIC)
    PHP_ME(ProjFactors, getDyDlam, arginfo_proj_factors_get_property, ZEND_ACC_PUBLIC)
    PHP_ME(ProjFactors, getDyDphi, arginfo_proj_factors_get_property, ZEND_ACC_PUBLIC)
    PHP_ME(ProjFactors, toArray, arginfo_proj_factors_to_array, ZEND_ACC_PUBLIC)
    PHP_ME(ProjFactors, __toString, arginfo_proj_factors_to_string, ZEND_ACC_PUBLIC)
    PHP_FE_END
};

/* Object creation function */
zend_object *proj_factors_object_create(zend_class_entry *ce)
{
    proj_factors_object *intern = ecalloc(1, sizeof(proj_factors_object) + zend_object_properties_size(ce));
    
    zend_object_std_init(&intern->std, ce);
    object_properties_init(&intern->std, ce);
    
    intern->std.handlers = &proj_factors_object_handlers;
    
    return &intern->std;
}

/* Object destruction function */
void proj_factors_object_destroy(zend_object *object)
{
    proj_factors_object *intern = FACTORS_FROM_OBJECT(object);
    
    zend_object_std_dtor(&intern->std);
}

/* Initialize ProjFactors class */
void proj_factors_init(void)
{
    zend_class_entry ce;

    /* Factors class */
    INIT_CLASS_ENTRY(ce, "ProjFactors", proj_factors_methods);
    proj_factors_ce = zend_register_internal_class(&ce);
    proj_factors_ce->create_object = proj_factors_object_create;
    
    memcpy(&proj_factors_object_handlers, &std_object_handlers, sizeof(zend_object_handlers));
    proj_factors_object_handlers.free_obj = proj_factors_object_destroy;
    proj_factors_object_handlers.offset = XtOffsetOf(proj_factors_object, std);

    /* Add public properties for direct access */
    zend_declare_property_double(proj_factors_ce, "meridional_scale", strlen("meridional_scale"), 0.0, ZEND_ACC_PUBLIC);
    zend_declare_property_double(proj_factors_ce, "parallel_scale", strlen("parallel_scale"), 0.0, ZEND_ACC_PUBLIC);
    zend_declare_property_double(proj_factors_ce, "areal_scale", strlen("areal_scale"), 0.0, ZEND_ACC_PUBLIC);
    zend_declare_property_double(proj_factors_ce, "angular_distortion", strlen("angular_distortion"), 0.0, ZEND_ACC_PUBLIC);
    zend_declare_property_double(proj_factors_ce, "meridian_parallel_angle", strlen("meridian_parallel_angle"), 0.0, ZEND_ACC_PUBLIC);
    zend_declare_property_double(proj_factors_ce, "meridian_convergence", strlen("meridian_convergence"), 0.0, ZEND_ACC_PUBLIC);
    zend_declare_property_double(proj_factors_ce, "tissot_semimajor", strlen("tissot_semimajor"), 0.0, ZEND_ACC_PUBLIC);
    zend_declare_property_double(proj_factors_ce, "tissot_semiminor", strlen("tissot_semiminor"), 0.0, ZEND_ACC_PUBLIC);
    zend_declare_property_double(proj_factors_ce, "dx_dlam", strlen("dx_dlam"), 0.0, ZEND_ACC_PUBLIC);
    zend_declare_property_double(proj_factors_ce, "dx_dphi", strlen("dx_dphi"), 0.0, ZEND_ACC_PUBLIC);
    zend_declare_property_double(proj_factors_ce, "dy_dlam", strlen("dy_dlam"), 0.0, ZEND_ACC_PUBLIC);
    zend_declare_property_double(proj_factors_ce, "dy_dphi", strlen("dy_dphi"), 0.0, ZEND_ACC_PUBLIC);
}