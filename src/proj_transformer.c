#include "../php_proj.h"

/* Class entries */
zend_class_entry *proj_transformer_ce;
zend_class_entry *proj_transformer_group_ce;
extern zend_class_entry *proj_transformer_exception_ce;
extern zend_class_entry *proj_crs_ce;

zend_object_handlers proj_transformer_object_handlers;
zend_object_handlers proj_transformer_group_object_handlers;

/* Arginfo declarations for Transformer methods */
ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_transformer_fromcrs, 0, 2, ProjTransformer, 0)
    ZEND_ARG_INFO(0, crs_from)
    ZEND_ARG_INFO(0, crs_to)
    ZEND_ARG_TYPE_INFO(0, always_xy, _IS_BOOL, 0)
    ZEND_ARG_ARRAY_INFO(0, area_of_interest, 1)
    ZEND_ARG_TYPE_INFO(0, authority, IS_STRING, 1)
    ZEND_ARG_TYPE_INFO(0, accuracy, IS_DOUBLE, 0)
    ZEND_ARG_TYPE_INFO(0, allow_ballpark, _IS_BOOL, 0)
    ZEND_ARG_TYPE_INFO(0, force_over, _IS_BOOL, 0)
    ZEND_ARG_TYPE_INFO(0, only_best, _IS_BOOL, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_transformer_frompipeline, 0, 1, ProjTransformer, 0)
    ZEND_ARG_TYPE_INFO(0, pipeline, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_transformer_getdescription, 0, 0, IS_STRING, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_transformer_hasinverse, 0, 0, _IS_BOOL, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_transformer_getaccuracy, 0, 0, IS_DOUBLE, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_transformer_transform, 0, 2, IS_ARRAY, 0)
    ZEND_ARG_INFO(0, xx)
    ZEND_ARG_INFO(0, yy)
    ZEND_ARG_INFO(0, zz)
    ZEND_ARG_INFO(0, tt)
    ZEND_ARG_TYPE_INFO(0, radians, _IS_BOOL, 0)
    ZEND_ARG_TYPE_INFO(0, errcheck, _IS_BOOL, 0)
    ZEND_ARG_TYPE_INFO(0, direction, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_transformer_transformarray, 0, 1, IS_ARRAY, 0)
    ZEND_ARG_ARRAY_INFO(0, coordinates, 0)
    ZEND_ARG_TYPE_INFO(0, radians, _IS_BOOL, 0)
    ZEND_ARG_TYPE_INFO(0, errcheck, _IS_BOOL, 0)
    ZEND_ARG_TYPE_INFO(0, direction, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_transformer_transformbounds, 0, 4, IS_ARRAY, 0)
    ZEND_ARG_TYPE_INFO(0, left, IS_DOUBLE, 0)
    ZEND_ARG_TYPE_INFO(0, bottom, IS_DOUBLE, 0)
    ZEND_ARG_TYPE_INFO(0, right, IS_DOUBLE, 0)
    ZEND_ARG_TYPE_INFO(0, top, IS_DOUBLE, 0)
    ZEND_ARG_TYPE_INFO(0, densify_pts, IS_LONG, 0)
    ZEND_ARG_TYPE_INFO(0, radians, _IS_BOOL, 0)
    ZEND_ARG_TYPE_INFO(0, errcheck, _IS_BOOL, 0)
    ZEND_ARG_TYPE_INFO(0, direction, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_transformer_towkt, 0, 0, IS_STRING, 0)
    ZEND_ARG_TYPE_INFO(0, version, IS_STRING, 0)
    ZEND_ARG_TYPE_INFO(0, pretty, _IS_BOOL, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_transformer_toproj4, 0, 0, IS_STRING, 0)
    ZEND_ARG_TYPE_INFO(0, version, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_transformer_tostring, 0, 0, IS_STRING, 0)
ZEND_END_ARG_INFO()

/* Arginfo declarations for TransformerGroup methods */
ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_transformergroup_fromcrs, 0, 2, ProjTransformerGroup, 0)
    ZEND_ARG_INFO(0, crs_from)
    ZEND_ARG_INFO(0, crs_to)
    ZEND_ARG_TYPE_INFO(0, always_xy, _IS_BOOL, 0)
    ZEND_ARG_ARRAY_INFO(0, area_of_interest, 1)
    ZEND_ARG_TYPE_INFO(0, authority, IS_STRING, 1)
    ZEND_ARG_TYPE_INFO(0, accuracy, IS_DOUBLE, 0)
    ZEND_ARG_TYPE_INFO(0, allow_ballpark, _IS_BOOL, 0)
    ZEND_ARG_TYPE_INFO(0, force_over, _IS_BOOL, 0)
    ZEND_ARG_TYPE_INFO(0, only_best, _IS_BOOL, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_transformergroup_gettransformers, 0, 0, IS_ARRAY, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_transformergroup_downloadgrids, 0, 0, _IS_BOOL, 0)
    ZEND_ARG_TYPE_INFO(0, verbose, _IS_BOOL, 0)
ZEND_END_ARG_INFO()

/* Method declarations */
static PHP_METHOD(ProjTransformer, fromCrs);
static PHP_METHOD(ProjTransformer, fromPipeline);
static PHP_METHOD(ProjTransformer, getDescription);
static PHP_METHOD(ProjTransformer, hasInverse);
static PHP_METHOD(ProjTransformer, getAccuracy);
static PHP_METHOD(ProjTransformer, transform);
static PHP_METHOD(ProjTransformer, transformArray);
static PHP_METHOD(ProjTransformer, transformBounds);
static PHP_METHOD(ProjTransformer, toWkt);
static PHP_METHOD(ProjTransformer, toProj4);
static PHP_METHOD(ProjTransformer, __toString);

static PHP_METHOD(ProjTransformerGroup, fromCrs);
static PHP_METHOD(ProjTransformerGroup, getTransformers);

/* Method tables */
static const zend_function_entry proj_transformer_methods[] = {
    PHP_ME(ProjTransformer, fromCrs, arginfo_transformer_fromcrs, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC)
    PHP_ME(ProjTransformer, fromPipeline, arginfo_transformer_frompipeline, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC)
    PHP_ME(ProjTransformer, getDescription, arginfo_transformer_getdescription, ZEND_ACC_PUBLIC)
    PHP_ME(ProjTransformer, hasInverse, arginfo_transformer_hasinverse, ZEND_ACC_PUBLIC)
    PHP_ME(ProjTransformer, getAccuracy, arginfo_transformer_getaccuracy, ZEND_ACC_PUBLIC)
    PHP_ME(ProjTransformer, transform, arginfo_transformer_transform, ZEND_ACC_PUBLIC)
    PHP_ME(ProjTransformer, transformArray, arginfo_transformer_transformarray, ZEND_ACC_PUBLIC)
    PHP_ME(ProjTransformer, transformBounds, arginfo_transformer_transformbounds, ZEND_ACC_PUBLIC)
    PHP_ME(ProjTransformer, toWkt, arginfo_transformer_towkt, ZEND_ACC_PUBLIC)
    PHP_ME(ProjTransformer, toProj4, arginfo_transformer_toproj4, ZEND_ACC_PUBLIC)
    PHP_ME(ProjTransformer, __toString, arginfo_transformer_tostring, ZEND_ACC_PUBLIC)
    PHP_FE_END
};

static const zend_function_entry proj_transformer_group_methods[] = {
    PHP_ME(ProjTransformerGroup, fromCrs, arginfo_transformergroup_fromcrs, ZEND_ACC_PUBLIC | ZEND_ACC_STATIC)
    PHP_ME(ProjTransformerGroup, getTransformers, arginfo_transformergroup_gettransformers, ZEND_ACC_PUBLIC)
    PHP_FE_END
};

/* Initialize Transformer classes */
void proj_transformer_init(void)
{
    zend_class_entry ce;
    
    /* Transformer class */
    INIT_CLASS_ENTRY(ce, "ProjTransformer", proj_transformer_methods);
    proj_transformer_ce = zend_register_internal_class(&ce);
    proj_transformer_ce->create_object = proj_transformer_object_create;
    
    memcpy(&proj_transformer_object_handlers, &std_object_handlers, sizeof(zend_object_handlers));
    proj_transformer_object_handlers.free_obj = proj_transformer_object_destroy;
    proj_transformer_object_handlers.offset = XtOffsetOf(proj_transformer_object, std);

    /* TransformerGroup class */
    INIT_CLASS_ENTRY(ce, "ProjTransformerGroup", proj_transformer_group_methods);
    proj_transformer_group_ce = zend_register_internal_class(&ce);
    proj_transformer_group_ce->create_object = proj_transformer_group_object_create;
    
    memcpy(&proj_transformer_group_object_handlers, &std_object_handlers, sizeof(zend_object_handlers));
    proj_transformer_group_object_handlers.free_obj = proj_transformer_group_object_destroy;
    proj_transformer_group_object_handlers.offset = XtOffsetOf(proj_transformer_group_object, std);
}

/* Object creation */
zend_object *proj_transformer_object_create(zend_class_entry *ce)
{
    proj_transformer_object *intern = ecalloc(1, sizeof(proj_transformer_object) + zend_object_properties_size(ce));
    
    zend_object_std_init(&intern->std, ce);
    object_properties_init(&intern->std, ce);
    
    intern->pj = NULL;
    intern->source_crs = NULL;
    intern->target_crs = NULL;
    intern->std.handlers = &proj_transformer_object_handlers;
    
    return &intern->std;
}

/* Group object creation */
zend_object *proj_transformer_group_object_create(zend_class_entry *ce)
{
    proj_transformer_group_object *intern = ecalloc(1, sizeof(proj_transformer_group_object) + zend_object_properties_size(ce));
    
    zend_object_std_init(&intern->std, ce);
    object_properties_init(&intern->std, ce);
    
    intern->transformers = NULL;
    intern->count = 0;
    intern->std.handlers = &proj_transformer_group_object_handlers;
    
    return &intern->std;
}

/* Object destruction */
void proj_transformer_object_destroy(zend_object *object)
{
    proj_transformer_object *intern = TRANSFORMER_FROM_OBJECT(object);
    
    if (intern->pj) {
        proj_destroy(intern->pj);
        intern->pj = NULL;
    }
    
    if (intern->source_crs) {
        /* Decrement reference count */
        OBJ_RELEASE(&intern->source_crs->std);
        intern->source_crs = NULL;
    }
    
    if (intern->target_crs) {
        /* Decrement reference count */
        OBJ_RELEASE(&intern->target_crs->std);
        intern->target_crs = NULL;
    }
    
    zend_object_std_dtor(&intern->std);
}

/* Group object destruction */
void proj_transformer_group_object_destroy(zend_object *object)
{
    proj_transformer_group_object *intern = TRANSFORMER_GROUP_FROM_OBJECT(object);
    size_t i;
    
    if (intern->transformers) {
        for (i = 0; i < intern->count; i++) {
            if (intern->transformers[i]) {
                proj_destroy(intern->transformers[i]);
            }
        }
        efree(intern->transformers);
        intern->transformers = NULL;
    }
    
    zend_object_std_dtor(&intern->std);
}

/* Static method: fromCrs */
static PHP_METHOD(ProjTransformer, fromCrs)
{
    zval *crs_from, *crs_to;
    zend_bool always_xy = 0;
    zval *area_of_interest = NULL;
    char *authority = NULL;
    size_t authority_len = 0;
    double accuracy = -1.0;
    zend_bool allow_ballpark = 1;
    zend_bool force_over = 0;
    zend_bool only_best = 1;
    
    char *from_string = NULL, *to_string = NULL;
    PJ_CONTEXT *ctx;
    PJ *pj;
    proj_transformer_object *intern;

    ZEND_PARSE_PARAMETERS_START(2, 9)
        Z_PARAM_ZVAL(crs_from)
        Z_PARAM_ZVAL(crs_to)
        Z_PARAM_OPTIONAL
        Z_PARAM_BOOL(always_xy)
        Z_PARAM_ZVAL_OR_NULL(area_of_interest)
        Z_PARAM_STRING_OR_NULL(authority, authority_len)
        Z_PARAM_DOUBLE(accuracy)
        Z_PARAM_BOOL(allow_ballpark)
        Z_PARAM_BOOL(force_over)
        Z_PARAM_BOOL(only_best)
    ZEND_PARSE_PARAMETERS_END();

    /* Parse input CRS */
    if (!proj_parse_crs_input(crs_from, &from_string)) {
        proj_throw_exception(proj_transformer_exception_ce, "Invalid source CRS");
        return;
    }
    
    if (!proj_parse_crs_input(crs_to, &to_string)) {
        efree(from_string);
        proj_throw_exception(proj_transformer_exception_ce, "Invalid target CRS");
        return;
    }

    ctx = proj_get_default_context();
    
    /* Create transformation */
    pj = proj_create_crs_to_crs(ctx, from_string, to_string, NULL);
    
    if (!pj) {
        efree(from_string);
        efree(to_string);
        proj_throw_proj_error(ctx);
        return;
    }

    /* Normalize for visualization if always_xy is true */
    if (always_xy) {
        PJ *normalized = proj_normalize_for_visualization(ctx, pj);
        if (normalized) {
            proj_destroy(pj);
            pj = normalized;
        }
    }

    /* Create return object */
    object_init_ex(return_value, proj_transformer_ce);
    intern = TRANSFORMER_FROM_OBJECT(Z_OBJ_P(return_value));
    intern->pj = pj;

    /* Store source and target CRS objects if they were passed as objects */
    if (Z_TYPE_P(crs_from) == IS_OBJECT && instanceof_function(Z_OBJCE_P(crs_from), proj_crs_ce)) {
        intern->source_crs = CRS_FROM_OBJECT(Z_OBJ_P(crs_from));
        GC_ADDREF(&intern->source_crs->std);
    }
    
    if (Z_TYPE_P(crs_to) == IS_OBJECT && instanceof_function(Z_OBJCE_P(crs_to), proj_crs_ce)) {
        intern->target_crs = CRS_FROM_OBJECT(Z_OBJ_P(crs_to));
        GC_ADDREF(&intern->target_crs->std);
    }

    efree(from_string);
    efree(to_string);
}

/* Static method: fromPipeline */
static PHP_METHOD(ProjTransformer, fromPipeline)
{
    char *pipeline;
    size_t pipeline_len;
    PJ_CONTEXT *ctx;
    PJ *pj;
    proj_transformer_object *intern;

    ZEND_PARSE_PARAMETERS_START(1, 1)
        Z_PARAM_STRING(pipeline, pipeline_len)
    ZEND_PARSE_PARAMETERS_END();

    ctx = proj_get_default_context();

    /* A pipeline definition must be headed by "+proj=pipeline". PROJ 8 tolerated
     * a bare "+step ..." body; PROJ 9 instead builds a VALID IDENTITY transform,
     * so proj_create() succeeds and transform() silently returns its input
     * unchanged. Add the missing header rather than return wrong coordinates.
     * Only strings starting with "+step" are rewritten: fromPipeline() is also
     * called with plain proj strings (e.g. "+proj=lcc ... +inv"), for which
     * prepending the header would be invalid. */
    {
        const char *p = pipeline;
        while (*p == ' ' || *p == '\t') {
            p++;
        }
        if (strncmp(p, "+step", 5) == 0) {
            char *headed;
            spprintf(&headed, 0, "+proj=pipeline %s", p);
            pj = proj_create(ctx, headed);
            efree(headed);
        } else {
            pj = proj_create(ctx, pipeline);
        }
    }

    if (!pj) {
        proj_throw_proj_error(ctx);
        return;
    }

    object_init_ex(return_value, proj_transformer_ce);
    intern = TRANSFORMER_FROM_OBJECT(Z_OBJ_P(return_value));
    intern->pj = pj;
}

/* Get description */
static PHP_METHOD(ProjTransformer, getDescription)
{
    proj_transformer_object *intern;
    const char *desc;

    ZEND_PARSE_PARAMETERS_NONE();

    intern = TRANSFORMER_FROM_OBJECT(Z_OBJ_P(getThis()));
    if (!intern->pj) {
        RETURN_NULL();
    }

    desc = proj_get_name(intern->pj);
    if (!desc) {
        proj_throw_exception(proj_transformer_exception_ce, "Failed to get transformer description");
        return;
    }
    
    RETURN_STRING(desc);
}

/* Check if has inverse */
static PHP_METHOD(ProjTransformer, hasInverse)
{
    proj_transformer_object *intern;
    PJ_CONTEXT *ctx;

    ZEND_PARSE_PARAMETERS_NONE();

    intern = TRANSFORMER_FROM_OBJECT(Z_OBJ_P(getThis()));
    if (!intern->pj) {
        RETURN_FALSE;
    }

    ctx = proj_get_default_context();
    RETURN_BOOL(proj_pj_info(intern->pj).has_inverse);
}

/* Get accuracy */
static PHP_METHOD(ProjTransformer, getAccuracy)
{
    proj_transformer_object *intern;
    double accuracy;

    ZEND_PARSE_PARAMETERS_NONE();

    intern = TRANSFORMER_FROM_OBJECT(Z_OBJ_P(getThis()));
    if (!intern->pj) {
        RETURN_NULL();
    }

    accuracy = proj_pj_info(intern->pj).accuracy;
    if (accuracy >= 0) {
        RETURN_DOUBLE(accuracy);
    } else {
        RETURN_NULL();
    }
}

/* Transform coordinates */
static PHP_METHOD(ProjTransformer, transform)
{
    zval *xx, *yy, *zz = NULL, *tt = NULL;
    zend_bool radians = 0, errcheck = 0;
    char *direction = "FORWARD";
    size_t direction_len;
    
    proj_transformer_object *intern;
    PJ_CONTEXT *ctx;
    PJ_DIRECTION dir;
    
    ZEND_PARSE_PARAMETERS_START(2, 7)
        Z_PARAM_ZVAL(xx)
        Z_PARAM_ZVAL(yy)
        Z_PARAM_OPTIONAL
        Z_PARAM_ZVAL_OR_NULL(zz)
        Z_PARAM_ZVAL_OR_NULL(tt)
        Z_PARAM_BOOL(radians)
        Z_PARAM_BOOL(errcheck)
        Z_PARAM_STRING(direction, direction_len)
    ZEND_PARSE_PARAMETERS_END();

    intern = TRANSFORMER_FROM_OBJECT(Z_OBJ_P(getThis()));
    if (!intern->pj) {
        proj_throw_exception(proj_transformer_exception_ce, "No transformation available");
        return;
    }

    ctx = proj_get_default_context();
    dir = proj_direction_to_enum(direction);

    /* Handle scalar coordinates */
    if (Z_TYPE_P(xx) != IS_ARRAY && Z_TYPE_P(yy) != IS_ARRAY) {
        PJ_COORD coord = {{0, 0, 0, 0}};
        PJ_COORD result;
        
        coord.xyzt.x = zval_get_double(xx);
        coord.xyzt.y = zval_get_double(yy);
        
        if (zz && Z_TYPE_P(zz) != IS_NULL) {
            coord.xyzt.z = zval_get_double(zz);
        }
        if (tt && Z_TYPE_P(tt) != IS_NULL) {
            coord.xyzt.t = zval_get_double(tt);
        }

        /* Convert to radians if needed */
        if (!radians && proj_angular_input(intern->pj, dir)) {
            coord.xyzt.x = proj_torad(coord.xyzt.x);
            coord.xyzt.y = proj_torad(coord.xyzt.y);
        }

        result = proj_trans(intern->pj, dir, coord);
        
        if (result.xyzt.x == HUGE_VAL) {
            if (errcheck) {
                proj_throw_proj_error(ctx);
                return;
            }
        }

        /* Convert from radians if needed */
        if (!radians && proj_angular_output(intern->pj, dir)) {
            result.xyzt.x = proj_todeg(result.xyzt.x);
            result.xyzt.y = proj_todeg(result.xyzt.y);
        }

        array_init(return_value);
        add_next_index_double(return_value, result.xyzt.x);
        add_next_index_double(return_value, result.xyzt.y);
        
        if (zz && Z_TYPE_P(zz) != IS_NULL) {
            add_next_index_double(return_value, result.xyzt.z);
        }
        if (tt && Z_TYPE_P(tt) != IS_NULL) {
            add_next_index_double(return_value, result.xyzt.t);
        }
        return;
    }

    /* Handle array coordinates */
    size_t count;
    if (!proj_validate_coordinate_arrays(xx, yy, zz, tt, &count)) {
        proj_throw_exception(proj_transformer_exception_ce, "Invalid coordinate arrays");
        return;
    }

    double *x_coords = proj_zval_to_array(xx, &count);
    double *y_coords = proj_zval_to_array(yy, &count);
    double *z_coords = zz ? proj_zval_to_array(zz, &count) : NULL;
    double *t_coords = tt ? proj_zval_to_array(tt, &count) : NULL;
    
    if (!x_coords || !y_coords) {
        if (x_coords) efree(x_coords);
        if (y_coords) efree(y_coords);
        if (z_coords) efree(z_coords);
        if (t_coords) efree(t_coords);
        proj_throw_exception(proj_transformer_exception_ce, "Failed to parse coordinate arrays");
        return;
    }

    /* Transform arrays */
    size_t i;
    for (i = 0; i < count; i++) {
        PJ_COORD coord = {{x_coords[i], y_coords[i], 
                          z_coords ? z_coords[i] : 0.0,
                          t_coords ? t_coords[i] : 0.0}};
        PJ_COORD result;

        /* Convert to radians if needed */
        if (!radians && proj_angular_input(intern->pj, dir)) {
            coord.xyzt.x = proj_torad(coord.xyzt.x);
            coord.xyzt.y = proj_torad(coord.xyzt.y);
        }

        result = proj_trans(intern->pj, dir, coord);
        
        if (result.xyzt.x == HUGE_VAL) {
            if (errcheck) {
                efree(x_coords);
                efree(y_coords);
                if (z_coords) efree(z_coords);
                if (t_coords) efree(t_coords);
                proj_throw_proj_error(ctx);
                return;
            }
        }

        /* Convert from radians if needed */
        if (!radians && proj_angular_output(intern->pj, dir)) {
            result.xyzt.x = proj_todeg(result.xyzt.x);
            result.xyzt.y = proj_todeg(result.xyzt.y);
        }

        x_coords[i] = result.xyzt.x;
        y_coords[i] = result.xyzt.y;
        if (z_coords) z_coords[i] = result.xyzt.z;
        if (t_coords) t_coords[i] = result.xyzt.t;
    }

    /* Build result arrays */
    array_init(return_value);
    
    zval x_array, y_array;
    array_init(&x_array);
    array_init(&y_array);
    
    for (i = 0; i < count; i++) {
        add_next_index_double(&x_array, x_coords[i]);
        add_next_index_double(&y_array, y_coords[i]);
    }
    
    add_next_index_zval(return_value, &x_array);
    add_next_index_zval(return_value, &y_array);
    
    if (z_coords) {
        zval z_array;
        array_init(&z_array);
        for (i = 0; i < count; i++) {
            add_next_index_double(&z_array, z_coords[i]);
        }
        add_next_index_zval(return_value, &z_array);
    }
    
    if (t_coords) {
        zval t_array;
        array_init(&t_array);
        for (i = 0; i < count; i++) {
            add_next_index_double(&t_array, t_coords[i]);
        }
        add_next_index_zval(return_value, &t_array);
    }

    efree(x_coords);
    efree(y_coords);
    if (z_coords) efree(z_coords);
    if (t_coords) efree(t_coords);
}

/* String representation */
static PHP_METHOD(ProjTransformer, __toString)
{
    proj_transformer_object *intern;
    PJ_CONTEXT *ctx;
    const char *def;

    ZEND_PARSE_PARAMETERS_NONE();

    intern = TRANSFORMER_FROM_OBJECT(Z_OBJ_P(getThis()));
    if (!intern->pj) {
        proj_throw_exception(proj_transformer_exception_ce, "Transformer object is not initialized");
        return;
    }

    ctx = proj_get_default_context();
    def = proj_as_proj_string(ctx, intern->pj, PJ_PROJ_5, NULL);
    
    if (!def) {
        proj_throw_proj_error(ctx);
        return;
    }
    
    RETURN_STRING(def);
}

/* Transform coordinate arrays with tuple-based API */
static PHP_METHOD(ProjTransformer, transformArray) {
    zval *coordinates;
    zend_bool radians = 0, errcheck = 0;
    char *direction = "FORWARD";
    size_t direction_len = 7;
    proj_transformer_object *intern;
    PJ_CONTEXT *ctx;
    PJ_DIRECTION dir;
    
    ZEND_PARSE_PARAMETERS_START(1, 4)
        Z_PARAM_ARRAY(coordinates)
        Z_PARAM_OPTIONAL
        Z_PARAM_BOOL(radians)
        Z_PARAM_BOOL(errcheck)
        Z_PARAM_STRING(direction, direction_len)
    ZEND_PARSE_PARAMETERS_END();

    intern = TRANSFORMER_FROM_OBJECT(Z_OBJ_P(getThis()));
    
    if (!intern->pj) {
        proj_throw_exception(proj_transformer_exception_ce, "Transformer object is not initialized");
        return;
    }
    
    ctx = proj_get_default_context();
    dir = proj_direction_to_enum(direction);
    
    /* Get coordinate count */
    size_t count = zend_array_count(Z_ARRVAL_P(coordinates));
    
    /* Handle empty arrays */
    if (count == 0) {
        array_init(return_value);
        return;
    }

    /* Prepare result array */
    array_init(return_value);
    
    /* Transform each coordinate tuple */
    HashTable *coord_array = Z_ARRVAL_P(coordinates);
    zval *coord_tuple;
    
    ZEND_HASH_FOREACH_VAL(coord_array, coord_tuple) {
        if (Z_TYPE_P(coord_tuple) != IS_ARRAY) {
            proj_throw_exception(proj_transformer_exception_ce, "Each coordinate must be an array");
            return;
        }
        
        HashTable *tuple_array = Z_ARRVAL_P(coord_tuple);
        size_t tuple_size = zend_array_count(tuple_array);
        
        if (tuple_size < 2) {
            proj_throw_exception(proj_transformer_exception_ce, "Each coordinate must have at least X and Y values");
            return;
        }
        
        /* Extract coordinate values */
        zval *x_val = zend_hash_index_find(tuple_array, 0);
        zval *y_val = zend_hash_index_find(tuple_array, 1);
        zval *z_val = tuple_size > 2 ? zend_hash_index_find(tuple_array, 2) : NULL;
        zval *t_val = tuple_size > 3 ? zend_hash_index_find(tuple_array, 3) : NULL;
        
        if (!x_val || !y_val) {
            proj_throw_exception(proj_transformer_exception_ce, "Invalid coordinate format");
            return;
        }
        
        double x = zval_get_double(x_val);
        double y = zval_get_double(y_val);
        double z = z_val ? zval_get_double(z_val) : 0.0;
        double t = t_val ? zval_get_double(t_val) : 0.0;
        
        /* Convert to radians if needed */
        if (!radians && proj_angular_input(intern->pj, dir)) {
            x = proj_torad(x);
            y = proj_torad(y);
        }
        
        /* Transform coordinate */
        PJ_COORD coord_in = proj_coord(x, y, z, t);
        PJ_COORD coord_out = proj_trans(intern->pj, dir, coord_in);
        
        /* Check for errors if requested */
        if (errcheck && coord_out.xyzt.x == HUGE_VAL) {
            proj_throw_proj_error(ctx);
            return;
        }
        
        /* Convert from radians if needed */
        if (!radians && proj_angular_output(intern->pj, dir)) {
            coord_out.xyzt.x = proj_todeg(coord_out.xyzt.x);
            coord_out.xyzt.y = proj_todeg(coord_out.xyzt.y);
        }
        
        /* Create result coordinate array */
        zval result_tuple;
        array_init(&result_tuple);
        add_next_index_double(&result_tuple, coord_out.xyzt.x);
        add_next_index_double(&result_tuple, coord_out.xyzt.y);
        
        if (z_val) {
            add_next_index_double(&result_tuple, coord_out.xyzt.z);
        }
        if (t_val) {
            add_next_index_double(&result_tuple, coord_out.xyzt.t);
        }
        
        /* Add result to output array */
        add_next_index_zval(return_value, &result_tuple);
    } ZEND_HASH_FOREACH_END();
}

/* Method: transformBounds */
static PHP_METHOD(ProjTransformer, transformBounds)
{
    proj_transformer_object *intern;
    PJ_CONTEXT *ctx;
    double min_x, min_y, max_x, max_y;
    zend_long densify_pts = 21;
    zend_bool radians = 0, errcheck = 0;
    char *direction = "FORWARD";
    size_t direction_len;
    PJ_DIRECTION dir;
    
    ZEND_PARSE_PARAMETERS_START(4, 8)
        Z_PARAM_DOUBLE(min_x)
        Z_PARAM_DOUBLE(min_y)
        Z_PARAM_DOUBLE(max_x)
        Z_PARAM_DOUBLE(max_y)
        Z_PARAM_OPTIONAL
        Z_PARAM_LONG(densify_pts)
        Z_PARAM_BOOL(radians)
        Z_PARAM_BOOL(errcheck)
        Z_PARAM_STRING(direction, direction_len)
    ZEND_PARSE_PARAMETERS_END();

    intern = TRANSFORMER_FROM_OBJECT(Z_OBJ_P(ZEND_THIS));
    if (!intern->pj) {
        proj_throw_exception(proj_transformer_exception_ce, "Transformer object is not initialized");
        return;
    }

    ctx = proj_get_default_context();
    dir = proj_direction_to_enum(direction);
    
    /* Convert input coordinates to radians if needed */
    if (!radians && proj_angular_input(intern->pj, dir)) {
        min_x = proj_torad(min_x);
        min_y = proj_torad(min_y);
        max_x = proj_torad(max_x);
        max_y = proj_torad(max_y);
    }
    
    double out_min_x, out_min_y, out_max_x, out_max_y;
    
    int success = proj_trans_bounds(ctx, intern->pj, dir, min_x, min_y, max_x, max_y,
                                   &out_min_x, &out_min_y, &out_max_x, &out_max_y, densify_pts);
    
    if (!success) {
        if (errcheck) {
            proj_throw_proj_error(ctx);
            return;
        } else {
            proj_throw_exception(proj_transformer_exception_ce, "Failed to transform bounds");
            RETURN_NULL();
        }
    }
    
    /* Convert output coordinates from radians if needed */
    if (!radians && proj_angular_output(intern->pj, dir)) {
        out_min_x = proj_todeg(out_min_x);
        out_min_y = proj_todeg(out_min_y);
        out_max_x = proj_todeg(out_max_x);
        out_max_y = proj_todeg(out_max_y);
    }
    
    array_init(return_value);
    add_next_index_double(return_value, out_min_x);
    add_next_index_double(return_value, out_min_y);
    add_next_index_double(return_value, out_max_x);
    add_next_index_double(return_value, out_max_y);
}

/* Method: toWkt */
static PHP_METHOD(ProjTransformer, toWkt)
{
    proj_transformer_object *intern;
    PJ_CONTEXT *ctx;
    const char *wkt;
    
    if (zend_parse_parameters_none() == FAILURE) {
        return;
    }

    intern = TRANSFORMER_FROM_OBJECT(Z_OBJ_P(ZEND_THIS));
    if (!intern->pj) {
        proj_throw_exception(proj_transformer_exception_ce, "Transformer object is not initialized");
        return;
    }

    ctx = proj_get_default_context();
    wkt = proj_as_wkt(ctx, intern->pj, PJ_WKT2_2019, NULL);
    
    if (wkt) {
        RETURN_STRING(wkt);
    } else {
        proj_throw_exception(proj_transformer_exception_ce, "Failed to convert transformer to WKT");
        RETURN_NULL();
    }
}

/* Method: toProj4 */
static PHP_METHOD(ProjTransformer, toProj4)
{
    proj_transformer_object *intern;
    PJ_CONTEXT *ctx;
    const char *proj4;
    
    if (zend_parse_parameters_none() == FAILURE) {
        return;
    }

    intern = TRANSFORMER_FROM_OBJECT(Z_OBJ_P(ZEND_THIS));
    if (!intern->pj) {
        proj_throw_exception(proj_transformer_exception_ce, "Transformer object is not initialized");
        return;
    }

    ctx = proj_get_default_context();
    proj4 = proj_as_proj_string(ctx, intern->pj, PJ_PROJ_5, NULL);
    
    if (proj4) {
        RETURN_STRING(proj4);
    } else {
        proj_throw_exception(proj_transformer_exception_ce, "Failed to convert transformer to PROJ4");
        RETURN_NULL();
    }
}

/* Method: fromCrs */
static PHP_METHOD(ProjTransformerGroup, fromCrs) {
    zval *crs_from, *crs_to;
    zend_bool always_xy = 0, allow_ballpark = 1, force_over = 0, only_best = 1;
    zval *area_of_interest = NULL;
    char *authority = NULL;
    size_t authority_len = 0;
    double accuracy = -1.0;
    
    ZEND_PARSE_PARAMETERS_START(2, 9)
        Z_PARAM_ZVAL(crs_from)
        Z_PARAM_ZVAL(crs_to)
        Z_PARAM_OPTIONAL
        Z_PARAM_BOOL(always_xy)
        Z_PARAM_ARRAY_OR_NULL(area_of_interest)
        Z_PARAM_STRING_OR_NULL(authority, authority_len)
        Z_PARAM_DOUBLE(accuracy)
        Z_PARAM_BOOL(allow_ballpark)
        Z_PARAM_BOOL(force_over)
        Z_PARAM_BOOL(only_best)
    ZEND_PARSE_PARAMETERS_END();

    char *from_string = NULL, *to_string = NULL;
    PJ_CONTEXT *ctx = proj_get_default_context();
    
    /* Parse CRS inputs */
    if (!proj_parse_crs_input(crs_from, &from_string) || !proj_parse_crs_input(crs_to, &to_string)) {
        if (from_string) efree(from_string);
        if (to_string) efree(to_string);
        proj_throw_exception(proj_transformer_exception_ce, "Invalid CRS input");
        return;
    }

    /* Create source and target CRS objects */
    PJ *source_pj = proj_create(ctx, from_string);
    PJ *target_pj = proj_create(ctx, to_string);
    
    if (!source_pj || !target_pj) {
        if (source_pj) proj_destroy(source_pj);
        if (target_pj) proj_destroy(target_pj);
        efree(from_string);
        efree(to_string);
        proj_throw_exception(proj_transformer_exception_ce, "Failed to create CRS objects");
        return;
    }

    /* Create transformation operation group using PROJ */
    PJ_OPERATION_FACTORY_CONTEXT *op_ctx = proj_create_operation_factory_context(ctx, NULL);
    
    /* Parse area of interest from array if provided [west_lon, south_lat, east_lon, north_lat] */
    if (area_of_interest) {
        HashTable *arr = Z_ARRVAL_P(area_of_interest);
        if (zend_array_count(arr) >= 4) {
            zval *west_val = zend_hash_index_find(arr, 0);
            zval *south_val = zend_hash_index_find(arr, 1);
            zval *east_val = zend_hash_index_find(arr, 2);
            zval *north_val = zend_hash_index_find(arr, 3);
            
            if (west_val && south_val && east_val && north_val) {
                double west_lon = zval_get_double(west_val);
                double south_lat = zval_get_double(south_val);
                double east_lon = zval_get_double(east_val);
                double north_lat = zval_get_double(north_val);
                
                proj_operation_factory_context_set_area_of_interest(ctx, op_ctx, 
                    west_lon, south_lat, east_lon, north_lat);
            }
        }
    }
    
    PJ_OBJ_LIST *op_list = proj_create_operations(ctx, source_pj, target_pj, op_ctx);
    
    if (!op_list) {
        proj_operation_factory_context_destroy(op_ctx);
        proj_destroy(source_pj);
        proj_destroy(target_pj);
        efree(from_string);
        efree(to_string);
        proj_throw_exception(proj_transformer_exception_ce, "Failed to create transformation operations");
        return;
    }

    int op_count = proj_list_get_count(op_list);
    if (op_count == 0) {
        proj_list_destroy(op_list);
        proj_operation_factory_context_destroy(op_ctx);
        proj_destroy(source_pj);
        proj_destroy(target_pj);
        efree(from_string);
        efree(to_string);
        proj_throw_exception(proj_transformer_exception_ce, "No transformation operations found");
        return;
    }

    /* Create transformer group object */
    object_init_ex(return_value, proj_transformer_group_ce);
    proj_transformer_group_object *intern = TRANSFORMER_GROUP_FROM_OBJECT(Z_OBJ_P(return_value));
    
    /* Allocate array for transformers */
    intern->transformers = emalloc(op_count * sizeof(PJ*));
    intern->count = op_count;
    
    /* Store each operation as a transformer */
    int i;
    for (i = 0; i < op_count; i++) {
        PJ *op = proj_list_get(ctx, op_list, i);
        if (op) {
            intern->transformers[i] = op;
        } else {
            intern->transformers[i] = NULL;
        }
    }

    proj_list_destroy(op_list);
    proj_operation_factory_context_destroy(op_ctx);
    proj_destroy(source_pj);
    proj_destroy(target_pj);
    efree(from_string);
    efree(to_string);
}

/* Get array of individual ProjTransformer objects */
static PHP_METHOD(ProjTransformerGroup, getTransformers) {
    proj_transformer_group_object *intern;
    PJ_CONTEXT *ctx;
    
    ZEND_PARSE_PARAMETERS_NONE();

    intern = TRANSFORMER_GROUP_FROM_OBJECT(Z_OBJ_P(getThis()));
    
    if (!intern->transformers || intern->count == 0) {
        array_init(return_value);
        return;
    }

    ctx = proj_get_default_context();
    array_init(return_value);
    
    size_t i;
    for (i = 0; i < intern->count; i++) {
        if (intern->transformers[i]) {
            /* Create a new ProjTransformer object for each transformer */
            zval transformer_obj;
            object_init_ex(&transformer_obj, proj_transformer_ce);
            proj_transformer_object *transformer_intern = TRANSFORMER_FROM_OBJECT(Z_OBJ_P(&transformer_obj));
            
            /* Clone the PJ object for the new transformer */
            transformer_intern->pj = proj_clone(ctx, intern->transformers[i]);
            
            if (transformer_intern->pj) {
                add_next_index_zval(return_value, &transformer_obj);
            } else {
                zval_ptr_dtor(&transformer_obj);
            }
        }
    }
}
