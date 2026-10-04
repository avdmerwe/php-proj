#ifndef PHP_PROJ_H
#define PHP_PROJ_H

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "ext/standard/info.h"
#include "ext/standard/php_string.h"
#include "ext/json/php_json.h"
#include "zend_exceptions.h"
#include "zend_interfaces.h"

#include <proj.h>
#include <geodesic.h>

#define PHP_PROJ_VERSION "2.0.3"
#define PHP_PROJ_EXTNAME "proj"

/* Module entry */
extern zend_module_entry proj_module_entry;
#define phpext_proj_ptr &proj_module_entry

/* Class entries */
extern zend_class_entry *proj_crs_ce;
extern zend_class_entry *proj_transformer_ce;
extern zend_class_entry *proj_transformer_group_ce;
extern zend_class_entry *proj_ce;
extern zend_class_entry *proj_geod_ce;
extern zend_class_entry *proj_factors_ce;
extern zend_class_entry *proj_area_of_interest_ce;
extern zend_class_entry *proj_unit_ce;

/* New object classes */
extern zend_class_entry *proj_ellipsoid_ce;
extern zend_class_entry *proj_prime_meridian_ce;
extern zend_class_entry *proj_datum_ce;
extern zend_class_entry *proj_axis_ce;
extern zend_class_entry *proj_coordinate_system_ce;
extern zend_class_entry *proj_coordinate_operation_ce;
extern zend_class_entry *proj_area_of_use_ce;

/* Exception class entries */
extern zend_class_entry *proj_exception_ce;
extern zend_class_entry *proj_crs_exception_ce;
extern zend_class_entry *proj_transformer_exception_ce;
extern zend_class_entry *proj_geod_exception_ce;
extern zend_class_entry *proj_data_dir_exception_ce;
extern zend_class_entry *proj_pipeline_exception_ce;

/* Enum class entries */
extern zend_class_entry *proj_wkt_version_ce;
extern zend_class_entry *proj_version_ce;
extern zend_class_entry *proj_transform_direction_ce;
extern zend_class_entry *proj_pj_type_ce;

/* Object structures */
typedef struct {
    PJ *pj;
    zend_object std;
} proj_crs_object;

typedef struct {
    PJ *pj;
    proj_crs_object *source_crs;
    proj_crs_object *target_crs;
    zend_object std;
} proj_transformer_object;

typedef struct {
    PJ **transformers;
    size_t count;
    zend_object std;
} proj_transformer_group_object;

typedef struct {
    PJ *pj;
    zend_object std;
} proj_object;

typedef struct {
    struct geod_geodesic geod; /* PROJ geodesic structure */
    double a;                  /* semi-major axis */
    double f;                  /* flattening */
    zend_object std;
} proj_geod_object;

typedef struct {
    double meridional_scale;
    double parallel_scale;
    double areal_scale;
    double angular_distortion;
    double meridian_parallel_angle;
    double meridian_convergence;
    double tissot_semimajor;
    double tissot_semiminor;
    double dx_dlam;
    double dx_dphi;
    double dy_dlam;
    double dy_dphi;
    zend_object std;
} proj_factors_object;

typedef struct {
    double west_lon_degree;
    double south_lat_degree;
    double east_lon_degree;
    double north_lat_degree;
    zend_object std;
} proj_area_of_interest_object;

typedef struct {
    char *auth_name;
    char *code;
    char *name;
    char *category;
    double conv_factor;
    char *proj_short_name;
    zend_bool deprecated;
    zend_object std;
} proj_unit_object;

/* New object structures */
typedef struct {
    PJ *pj;
    zend_object std;
} proj_ellipsoid_object;

typedef struct {
    PJ *pj;
    zend_object std;
} proj_prime_meridian_object;

typedef struct {
    PJ *pj;
    zend_object std;
} proj_datum_object;

typedef struct {
    char *name;
    char *abbrev;
    char *direction;
    char *unit_name;
    char *unit_auth_code;
    char *unit_code;
    double unit_conversion_factor;
    zend_object std;
} proj_axis_object;

typedef struct {
    PJ *pj;
    zend_object std;
} proj_coordinate_system_object;

typedef struct {
    PJ *pj;
    zend_object std;
} proj_coordinate_operation_object;

typedef struct {
    double west;
    double south;
    double east;
    double north;
    char *name;
    zend_object std;
} proj_area_of_use_object;

/* Object handlers */
extern zend_object_handlers proj_crs_object_handlers;
extern zend_object_handlers proj_transformer_object_handlers;
extern zend_object_handlers proj_transformer_group_object_handlers;
extern zend_object_handlers proj_object_handlers;
extern zend_object_handlers proj_geod_object_handlers;
extern zend_object_handlers proj_factors_object_handlers;
extern zend_object_handlers proj_area_of_interest_object_handlers;
extern zend_object_handlers proj_unit_object_handlers;

/* New object handlers */
extern zend_object_handlers proj_ellipsoid_object_handlers;
extern zend_object_handlers proj_prime_meridian_object_handlers;
extern zend_object_handlers proj_datum_object_handlers;
extern zend_object_handlers proj_axis_object_handlers;
extern zend_object_handlers proj_coordinate_system_object_handlers;
extern zend_object_handlers proj_coordinate_operation_object_handlers;
extern zend_object_handlers proj_area_of_use_object_handlers;

/* Object creation functions */
zend_object *proj_crs_object_create(zend_class_entry *ce);
zend_object *proj_transformer_object_create(zend_class_entry *ce);
zend_object *proj_transformer_group_object_create(zend_class_entry *ce);
zend_object *proj_object_create(zend_class_entry *ce);
zend_object *proj_geod_object_create(zend_class_entry *ce);
zend_object *proj_factors_object_create(zend_class_entry *ce);
zend_object *proj_area_of_interest_object_create(zend_class_entry *ce);
zend_object *proj_unit_object_create(zend_class_entry *ce);

/* New object creation functions */
zend_object *proj_ellipsoid_object_create(zend_class_entry *ce);
zend_object *proj_prime_meridian_object_create(zend_class_entry *ce);
zend_object *proj_datum_object_create(zend_class_entry *ce);
zend_object *proj_axis_object_create(zend_class_entry *ce);
zend_object *proj_coordinate_system_object_create(zend_class_entry *ce);
zend_object *proj_coordinate_operation_object_create(zend_class_entry *ce);
zend_object *proj_area_of_use_object_create(zend_class_entry *ce);

/* Object destruction functions */
void proj_crs_object_destroy(zend_object *object);
void proj_transformer_object_destroy(zend_object *object);
void proj_transformer_group_object_destroy(zend_object *object);
void proj_object_destroy(zend_object *object);
void proj_geod_object_destroy(zend_object *object);
void proj_factors_object_destroy(zend_object *object);
void proj_area_of_interest_object_destroy(zend_object *object);
void proj_unit_object_destroy(zend_object *object);

/* New object destruction functions */
void proj_ellipsoid_object_destroy(zend_object *object);
void proj_prime_meridian_object_destroy(zend_object *object);
void proj_datum_object_destroy(zend_object *object);
void proj_axis_object_destroy(zend_object *object);
void proj_coordinate_system_object_destroy(zend_object *object);
void proj_coordinate_operation_object_destroy(zend_object *object);
void proj_area_of_use_object_destroy(zend_object *object);

/* Helper macros */
#define CRS_FROM_OBJECT(obj) ((proj_crs_object*)((char*)(obj) - XtOffsetOf(proj_crs_object, std)))
#define TRANSFORMER_FROM_OBJECT(obj) ((proj_transformer_object*)((char*)(obj) - XtOffsetOf(proj_transformer_object, std)))
#define TRANSFORMER_GROUP_FROM_OBJECT(obj) ((proj_transformer_group_object*)((char*)(obj) - XtOffsetOf(proj_transformer_group_object, std)))
#define PROJ_FROM_OBJECT(obj) ((proj_object*)((char*)(obj) - XtOffsetOf(proj_object, std)))
#define GEOD_FROM_OBJECT(obj) ((proj_geod_object*)((char*)(obj) - XtOffsetOf(proj_geod_object, std)))
#define FACTORS_FROM_OBJECT(obj) ((proj_factors_object*)((char*)(obj) - XtOffsetOf(proj_factors_object, std)))
#define AREA_OF_INTEREST_FROM_OBJECT(obj) ((proj_area_of_interest_object*)((char*)(obj) - XtOffsetOf(proj_area_of_interest_object, std)))
#define UNIT_FROM_OBJECT(obj) ((proj_unit_object*)((char*)(obj) - XtOffsetOf(proj_unit_object, std)))

/* New helper macros */
#define ELLIPSOID_FROM_OBJECT(obj) ((proj_ellipsoid_object*)((char*)(obj) - XtOffsetOf(proj_ellipsoid_object, std)))
#define PRIME_MERIDIAN_FROM_OBJECT(obj) ((proj_prime_meridian_object*)((char*)(obj) - XtOffsetOf(proj_prime_meridian_object, std)))
#define DATUM_FROM_OBJECT(obj) ((proj_datum_object*)((char*)(obj) - XtOffsetOf(proj_datum_object, std)))
#define AXIS_FROM_OBJECT(obj) ((proj_axis_object*)((char*)(obj) - XtOffsetOf(proj_axis_object, std)))
#define COORDINATE_SYSTEM_FROM_OBJECT(obj) ((proj_coordinate_system_object*)((char*)(obj) - XtOffsetOf(proj_coordinate_system_object, std)))
#define COORDINATE_OPERATION_FROM_OBJECT(obj) ((proj_coordinate_operation_object*)((char*)(obj) - XtOffsetOf(proj_coordinate_operation_object, std)))
#define AREA_OF_USE_FROM_OBJECT(obj) ((proj_area_of_use_object*)((char*)(obj) - XtOffsetOf(proj_area_of_use_object, std)))

/* Utility functions */
PJ_CONTEXT *proj_get_default_context(void);
void proj_throw_exception(zend_class_entry *exception_ce, const char *format, ...);
void proj_throw_proj_error(PJ_CONTEXT *ctx);
void proj_throw_crs_error(PJ_CONTEXT *ctx);
zval *proj_array_to_zval(double *array, size_t count);
double *proj_zval_to_array(zval *zv, size_t *count);
zend_bool proj_parse_crs_input(zval *input, char **crs_string);
zend_bool proj_validate_coordinate_arrays(zval *x, zval *y, zval *z, zval *t, size_t *count);
PJ_WKT_TYPE proj_wkt_version_to_enum(const char *version);
PJ_DIRECTION proj_direction_to_enum(const char *direction);

/* Constants */
#define PROJ_TRANSFORM_FORWARD 1
#define PROJ_TRANSFORM_INVERSE -1
#define PROJ_TRANSFORM_IDENT 0

/* Thread safety */
#if defined(ZTS) && defined(COMPILE_DL_PROJ)
ZEND_TSRMLS_CACHE_EXTERN()
#endif

/* Module lifecycle functions */
PHP_MINIT_FUNCTION(proj);
PHP_MSHUTDOWN_FUNCTION(proj);
PHP_RINIT_FUNCTION(proj);
PHP_RSHUTDOWN_FUNCTION(proj);
PHP_MINFO_FUNCTION(proj);

/* Initialize functions for each class */
void proj_crs_init(void);
void proj_transformer_init(void);
void proj_geod_init(void);
void proj_area_of_interest_init(void);
void proj_factors_init(void);
void proj_unit_init(void);
void proj_exceptions_init(void);
void proj_enums_init(void);
void proj_functions_init(void);

/* New initialize functions */
void proj_ellipsoid_init(void);
void proj_prime_meridian_init(void);
void proj_datum_init(void);
void proj_axis_init(void);
void proj_coordinate_system_init(void);
void proj_coordinate_operation_init(void);
void proj_area_of_use_init(void);

#endif /* PHP_PROJ_H */