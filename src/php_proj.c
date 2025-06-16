#include "../php_proj.h"

/* Module entry point */
zend_module_entry proj_module_entry = {
    STANDARD_MODULE_HEADER,
    PHP_PROJ_EXTNAME,
    NULL, /* functions */
    PHP_MINIT(proj),
    PHP_MSHUTDOWN(proj),
    PHP_RINIT(proj),
    PHP_RSHUTDOWN(proj),
    PHP_MINFO(proj),
    PHP_PROJ_VERSION,
    STANDARD_MODULE_PROPERTIES
};

#ifdef COMPILE_DL_PROJ
ZEND_GET_MODULE(proj)
#endif

#if defined(ZTS) && defined(COMPILE_DL_PROJ)
ZEND_TSRMLS_CACHE_DEFINE()
#endif

/* Global context for thread safety */
static PJ_CONTEXT *global_context = NULL;

/* Module initialization */
PHP_MINIT_FUNCTION(proj)
{
    /* Initialize PROJ global context */
    global_context = proj_context_create();
    if (!global_context) {
        php_error_docref(NULL, E_ERROR, "Failed to create PROJ context");
        return FAILURE;
    }
    
    /* Set PROJ log level to suppress debug messages */
    proj_log_level(global_context, PJ_LOG_NONE);

    /* Initialize exception classes first */
    proj_exceptions_init();
    
    /* Initialize enum classes */
    proj_enums_init();
    
    /* Initialize core classes */
    proj_crs_init();
    proj_transformer_init();
    proj_geod_init();
    proj_area_of_interest_init();
    proj_factors_init();
    proj_unit_init();
    
    /* Initialize new object classes */
    proj_ellipsoid_init();
    proj_prime_meridian_init();
    proj_datum_init();
    proj_axis_init();
    proj_coordinate_system_init();
    proj_coordinate_operation_init();
    proj_area_of_use_init();
    
    /* Initialize global functions */
    proj_functions_init();

    return SUCCESS;
}

/* Module shutdown */
PHP_MSHUTDOWN_FUNCTION(proj)
{
    if (global_context) {
        proj_context_destroy(global_context);
        global_context = NULL;
    }
    
    return SUCCESS;
}

/* Request initialization */
PHP_RINIT_FUNCTION(proj)
{
#if defined(ZTS) && defined(COMPILE_DL_PROJ)
    ZEND_TSRMLS_CACHE_UPDATE();
#endif
    return SUCCESS;
}

/* Request shutdown */
PHP_RSHUTDOWN_FUNCTION(proj)
{
    return SUCCESS;
}

/* Module info */
PHP_MINFO_FUNCTION(proj)
{
    PJ_INFO info = proj_info();
    
    php_info_print_table_start();
    php_info_print_table_row(2, "PROJ Extension", "enabled");
    php_info_print_table_row(2, "Extension Version", PHP_PROJ_VERSION);
    php_info_print_table_row(2, "PROJ Library Version", info.version);
    php_info_print_table_row(2, "PROJ Library Release", info.release);
    php_info_print_table_row(2, "PROJ Search Paths", info.searchpath);
    php_info_print_table_end();

    DISPLAY_INI_ENTRIES();
}

/* Get the default PROJ context */
PJ_CONTEXT *proj_get_default_context(void)
{
    return global_context;
}
