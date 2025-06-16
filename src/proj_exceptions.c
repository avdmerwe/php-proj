#include "../php_proj.h"

/* Exception class entries */
zend_class_entry *proj_exception_ce;
zend_class_entry *proj_crs_exception_ce;
zend_class_entry *proj_transformer_exception_ce;
zend_class_entry *proj_geod_exception_ce;
zend_class_entry *proj_data_dir_exception_ce;
zend_class_entry *proj_pipeline_exception_ce;

/* Initialize exception classes */
void proj_exceptions_init(void)
{
    zend_class_entry ce;

    /* Base ProjException */
    INIT_CLASS_ENTRY(ce, "ProjException", NULL);
    proj_exception_ce = zend_register_internal_class_ex(&ce, zend_ce_exception);

    /* CRSException */
    INIT_CLASS_ENTRY(ce, "ProjCRSException", NULL);
    proj_crs_exception_ce = zend_register_internal_class_ex(&ce, proj_exception_ce);

    /* TransformerException */
    INIT_CLASS_ENTRY(ce, "ProjTransformerException", NULL);
    proj_transformer_exception_ce = zend_register_internal_class_ex(&ce, proj_exception_ce);

    /* GeodException */
    INIT_CLASS_ENTRY(ce, "ProjGeodException", NULL);
    proj_geod_exception_ce = zend_register_internal_class_ex(&ce, proj_exception_ce);

    /* DataDirException */
    INIT_CLASS_ENTRY(ce, "ProjDataDirException", NULL);
    proj_data_dir_exception_ce = zend_register_internal_class_ex(&ce, proj_exception_ce);

    /* PipelineException */
    INIT_CLASS_ENTRY(ce, "ProjPipelineException", NULL);
    proj_pipeline_exception_ce = zend_register_internal_class_ex(&ce, proj_exception_ce);
}

/* Throw a PROJ exception with formatted message */
void proj_throw_exception(zend_class_entry *exception_ce, const char *format, ...)
{
    va_list args;
    char *message;
    size_t len;

    va_start(args, format);
    len = vspprintf(&message, 0, format, args);
    va_end(args);

    zend_throw_exception(exception_ce, message, 0);
    efree(message);
}

/* Throw exception based on PROJ context error */
void proj_throw_proj_error(PJ_CONTEXT *ctx)
{
    int error_code = proj_context_errno(ctx);
    const char *error_msg = proj_errno_string(error_code);
    
    if (error_code == 0) {
        /* No error */
        return;
    }

    /* Map PROJ errors to appropriate exception types */
    zend_class_entry *exception_ce = proj_exception_ce;
    
    switch (error_code) {
        case PROJ_ERR_INVALID_OP:
        case PROJ_ERR_INVALID_OP_WRONG_SYNTAX:
        case PROJ_ERR_INVALID_OP_MISSING_ARG:
        case PROJ_ERR_INVALID_OP_ILLEGAL_ARG_VALUE:
            exception_ce = proj_crs_exception_ce;
            break;
        case PROJ_ERR_COORD_TRANSFM:
        case PROJ_ERR_COORD_TRANSFM_INVALID_COORD:
        case PROJ_ERR_COORD_TRANSFM_OUTSIDE_PROJECTION_DOMAIN:
        case PROJ_ERR_COORD_TRANSFM_NO_OPERATION:
        case PROJ_ERR_COORD_TRANSFM_OUTSIDE_GRID:
        case PROJ_ERR_COORD_TRANSFM_GRID_AT_NODATA:
            exception_ce = proj_transformer_exception_ce;
            break;
        case PROJ_ERR_OTHER_API_MISUSE:
        case PROJ_ERR_OTHER_NO_INVERSE_OP:
        case PROJ_ERR_OTHER_NETWORK_ERROR:
            exception_ce = proj_data_dir_exception_ce;
            break;
        default:
            exception_ce = proj_exception_ce;
            break;
    }

    proj_throw_exception(exception_ce, "PROJ Error [%d]: %s", error_code, error_msg);
}

/* Throw a CRS-specific exception with PROJ error details */
void proj_throw_crs_error(PJ_CONTEXT *ctx)
{
    int error_code = proj_context_errno(ctx);
    const char *error_msg = proj_errno_string(error_code);
    
    if (error_code == 0) {
        /* No error */
        return;
    }

    proj_throw_exception(proj_crs_exception_ce, "PROJ Error [%d]: %s", error_code, error_msg);
}
