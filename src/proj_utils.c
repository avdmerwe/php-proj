#include "../php_proj.h"

extern zend_class_entry *proj_crs_ce;

/* Convert C array to PHP array */
zval *proj_array_to_zval(double *array, size_t count)
{
    zval *result;
    size_t i;

    result = emalloc(sizeof(zval));
    ZVAL_UNDEF(result);
    array_init_size(result, count);

    for (i = 0; i < count; i++) {
        add_next_index_double(result, array[i]);
    }

    return result;
}

/* Convert PHP array to C array */
double *proj_zval_to_array(zval *zv, size_t *count)
{
    HashTable *ht;
    zval *entry;
    double *result;
    size_t i = 0;

    if (Z_TYPE_P(zv) != IS_ARRAY) {
        *count = 0;
        return NULL;
    }

    ht = Z_ARRVAL_P(zv);
    *count = zend_hash_num_elements(ht);
    
    if (*count == 0) {
        return NULL;
    }

    result = emalloc(*count * sizeof(double));

    ZEND_HASH_FOREACH_VAL(ht, entry) {
        if (i >= *count) break;
        
        switch (Z_TYPE_P(entry)) {
            case IS_LONG:
                result[i] = (double)Z_LVAL_P(entry);
                break;
            case IS_DOUBLE:
                result[i] = Z_DVAL_P(entry);
                break;
            case IS_STRING:
                result[i] = strtod(Z_STRVAL_P(entry), NULL);
                break;
            default:
                efree(result);
                *count = 0;
                return NULL;
        }
        i++;
    } ZEND_HASH_FOREACH_END();

    return result;
}

/* Parse CRS input (string, int, or CRS object) */
zend_bool proj_parse_crs_input(zval *input, char **crs_string)
{
    switch (Z_TYPE_P(input)) {
        case IS_STRING:
            *crs_string = estrdup(Z_STRVAL_P(input));
            return 1;
            
        case IS_LONG: {
            /* EPSG code */
            spprintf(crs_string, 0, "EPSG:%ld", Z_LVAL_P(input));
            return 1;
        }
        
        case IS_OBJECT: {
            /* CRS object */
            if (instanceof_function(Z_OBJCE_P(input), proj_crs_ce)) {
                proj_crs_object *crs_obj = CRS_FROM_OBJECT(Z_OBJ_P(input));
                if (crs_obj->pj) {
                    PJ_CONTEXT *ctx = proj_get_default_context();
                    const char *wkt = proj_as_wkt(ctx, crs_obj->pj, PJ_WKT2_2019, NULL);
                    if (wkt) {
                        *crs_string = estrdup(wkt);
                        return 1;
                    }
                }
            }
            break;
        }
    }
    
    *crs_string = NULL;
    return 0;
}

/* Validate coordinate arrays and get count */
zend_bool proj_validate_coordinate_arrays(zval *x, zval *y, zval *z, zval *t, size_t *count)
{
    size_t x_count = 0, y_count = 0, z_count = 0, t_count = 0;

    /* X coordinates are required */
    if (!x || Z_TYPE_P(x) != IS_ARRAY) {
        return 0;
    }
    x_count = zend_hash_num_elements(Z_ARRVAL_P(x));
    *count = x_count;

    /* Y coordinates are required */
    if (!y || Z_TYPE_P(y) != IS_ARRAY) {
        return 0;
    }
    y_count = zend_hash_num_elements(Z_ARRVAL_P(y));
    if (y_count != x_count) {
        return 0;
    }

    /* Z coordinates are optional but must match count if provided */
    if (z && Z_TYPE_P(z) == IS_ARRAY) {
        z_count = zend_hash_num_elements(Z_ARRVAL_P(z));
        if (z_count != x_count) {
            return 0;
        }
    }

    /* T coordinates are optional but must match count if provided */
    if (t && Z_TYPE_P(t) == IS_ARRAY) {
        t_count = zend_hash_num_elements(Z_ARRVAL_P(t));
        if (t_count != x_count) {
            return 0;
        }
    }

    return 1;
}

/* Convert WKT version string to PROJ enum */
PJ_WKT_TYPE proj_wkt_version_to_enum(const char *version)
{
    if (strcmp(version, "WKT1_GDAL") == 0) {
        return PJ_WKT1_GDAL;
    } else if (strcmp(version, "WKT1_ESRI") == 0) {
        return PJ_WKT1_ESRI;
    } else if (strcmp(version, "WKT2_2015") == 0) {
        return PJ_WKT2_2015;
    } else if (strcmp(version, "WKT2_2015_SIMPLIFIED") == 0) {
        return PJ_WKT2_2015_SIMPLIFIED;
    } else if (strcmp(version, "WKT2_2019") == 0) {
        return PJ_WKT2_2019;
    } else if (strcmp(version, "WKT2_2019_SIMPLIFIED") == 0) {
        return PJ_WKT2_2019_SIMPLIFIED;
    }
    
    /* Default to WKT2_2019 */
    return PJ_WKT2_2019;
}

/* Convert transform direction string to PROJ enum */
PJ_DIRECTION proj_direction_to_enum(const char *direction)
{
    if (strcmp(direction, "FORWARD") == 0) {
        return PJ_FWD;
    } else if (strcmp(direction, "INVERSE") == 0) {
        return PJ_INV;
    } else if (strcmp(direction, "IDENT") == 0) {
        return PJ_IDENT;
    }
    
    /* Default to forward */
    return PJ_FWD;
}
