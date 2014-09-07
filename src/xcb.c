/*
  +----------------------------------------------------------------------+
  | PHP Version 5                                                        |
  +----------------------------------------------------------------------+
  | Copyright (c) 1997-2014 The PHP Group                                |
  +----------------------------------------------------------------------+
  | This source file is subject to version 3.01 of the PHP license,      |
  | that is bundled with this package in the file LICENSE, and is        |
  | available through the world-wide-web at the following url:           |
  | http://www.php.net/license/3_01.txt                                  |
  | If you did not receive a copy of the PHP license and are unable to   |
  | obtain it through the world-wide-web, please send a note to          |
  | license@php.net so we can mail you a copy immediately.               |
  +----------------------------------------------------------------------+
  | Author:                                                              |
  +----------------------------------------------------------------------+
*/

#include "php_xcb.h"

zend_class_entry *xcb_iterator_ce;

zend_function_entry xcb_iterator_functions[] = {
    ZEND_FE_END
};

ZEND_BEGIN_ARG_INFO(arginfo_xcb_connect, 0)
    ZEND_ARG_PASS_INFO(0)
    ZEND_ARG_PASS_INFO(1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_xcb_screen_next, 1)
    ZEND_ARG_OBJ_INFO(1, iterator, XcbIterator, 0)
ZEND_END_ARG_INFO()

zend_function_entry xcb_functions[] = {
    ZEND_FE(xcb_connect, arginfo_xcb_connect)
    ZEND_FE(xcb_disconnect, NULL)
    ZEND_FE(xcb_connection_has_error, NULL)
    ZEND_FE(xcb_flush, NULL)
    ZEND_FE(xcb_generate_id, NULL)
    ZEND_FE(xcb_get_setup, NULL)
    ZEND_FE(xcb_setup_roots_length, NULL)
    ZEND_FE(xcb_setup_roots_iterator, NULL)
    ZEND_FE(xcb_screen_next, arginfo_xcb_screen_next)
    ZEND_FE_END
};

zend_module_entry xcb_module_entry = {
    STANDARD_MODULE_HEADER,
    PHP_XCB_MODULE_NAME,
    xcb_functions,
    ZEND_MINIT(xcb),
    ZEND_MSHUTDOWN(xcb),
    NULL,           // PHP_RINIT
    NULL,           // PHP_RSHUTDOWN
    NULL,           // PHP_MINFO
    PHP_XCB_VERSION,
    STANDARD_MODULE_PROPERTIES
};

#if COMPILE_DL_XCB
ZEND_GET_MODULE(xcb)
#endif

xcb_connection_t *c = NULL;
static int le_xcb_setup;
static int le_xcb_screen_iterator;

void xcb_generic_dtor(zend_rsrc_list_entry *rsrc TSRMLS_CC)
{
    free(rsrc->ptr);
}

STRUCT_TO_OBJECT(xcb_screen_t)
{
    property_long(root);
    property_long(default_colormap);
    property_long(white_pixel);
    property_long(black_pixel);
    property_long(current_input_masks);
    property_long(width_in_pixels);
    property_long(height_in_pixels);
    property_long(width_in_millimeters);
    property_long(height_in_millimeters);
    property_long(min_installed_maps);
    property_long(max_installed_maps);
    property_long(root_visual);
    property_long(backing_stores);
    property_long(save_unders);
    property_long(root_depth);
    property_long(allowed_depths_len);
}

ZEND_MINIT_FUNCTION(xcb)
{
    zend_class_entry tmp_ce;
    INIT_CLASS_ENTRY(tmp_ce, "XcbIterator", xcb_iterator_functions);

    xcb_iterator_ce = zend_register_internal_class(&tmp_ce TSRMLS_CC);
    zend_declare_property_null(xcb_iterator_ce, "iterator", strlen("iterator"),
                               ZEND_ACC_PRIVATE TSRMLS_CC);

    zend_register_list_destructors(xcb_setup, NULL);
    zend_register_list_destructors(xcb_screen_iterator, xcb_generic_dtor);

    return SUCCESS;
}

ZEND_MSHUTDOWN_FUNCTION(xcb)
{
    if (c != NULL) {
        xcb_disconnect(c);
    }
    return SUCCESS;
}

ZEND_FUNCTION(xcb_connect)
{
    const char *displayname = NULL;
    long l;
    zval *ref = NULL;
    int screenp;

    if (c != NULL) {
        zend_error(E_NOTICE, "Already connected to the X server");
        return;
    }

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
                              "|sz", &displayname, &l, &ref) == FAILURE) {
        return;
    }

    c = xcb_connect(displayname, &screenp);
    if (ref != NULL) {
        ZVAL_LONG(ref, screenp);
    }
}

ZEND_FUNCTION(xcb_disconnect)
{
    xcb_disconnect(c);
    c = NULL;
}

ZEND_FUNCTION(xcb_connection_has_error)
{
    if (c == NULL) {
        RETURN_BOOL(0);
    }

    RETURN_LONG(xcb_connection_has_error(c));
}

ZEND_FUNCTION(xcb_flush)
{
    if (c == NULL) {
        RETURN_BOOL(0);
    }

    RETURN_LONG(xcb_flush(c));
}

ZEND_FUNCTION(xcb_generate_id)
{
    if (c == NULL) {
        RETURN_BOOL(0);
        return;
    }

    RETURN_LONG(xcb_generate_id(c));
}

ZEND_FUNCTION(xcb_get_setup)
{
    if (c == NULL) {
        RETURN_BOOL(0);
    }

    const xcb_setup_t *setup = xcb_get_setup(c);
    ZEND_REGISTER_RESOURCE(return_value, (xcb_setup_t *) setup, le_xcb_setup);
}

ZEND_FUNCTION(xcb_setup_roots_length)
{
    zval *rsrc;
    const xcb_setup_t *setup;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
                              "r", &rsrc) == FAILURE) {
        return;
    }

    ZEND_FETCH_RESOURCE(setup, xcb_setup_t*, &rsrc, -1, "xcb_setup_t",
                        le_xcb_setup);

    RETURN_LONG(xcb_setup_roots_length(setup));
}

ZEND_FUNCTION(xcb_setup_roots_iterator)
{
    zval *setup_rsrc, *it_rsrc, *data;
    const xcb_setup_t *setup;
    xcb_screen_iterator_t *it;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
                              "r", &setup_rsrc) == FAILURE) {
        return;
    }

    ZEND_FETCH_RESOURCE(setup, xcb_setup_t*, &setup_rsrc, -1, "xcb_setup_t",
                        le_xcb_setup);

    ALLOC_INIT_ZVAL(it_rsrc);
    ALLOC_INIT_ZVAL(data);

    object_init_ex(return_value, xcb_iterator_ce);
    object_init(data);

    it = malloc(sizeof(xcb_screen_iterator_t));
    *it = xcb_setup_roots_iterator(setup);

    xcb_screen_t_to_object(it->data, data);
    ZEND_REGISTER_RESOURCE(it_rsrc, it, le_xcb_screen_iterator);

    zend_update_property(xcb_iterator_ce, return_value, "iterator",
                         strlen("iterator"), it_rsrc TSRMLS_CC);
    add_property_zval(return_value, "data", data);
    add_property_long(return_value, "rem", it->rem);
    add_property_long(return_value, "index", it->index);
}

ZEND_FUNCTION(xcb_screen_next)
{
    zval *obj, *rsrc, *data;
    xcb_screen_iterator_t *it;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
                              "o", &obj) == FAILURE) {
        return;
    }

    rsrc = zend_read_property(xcb_iterator_ce, obj, "iterator",
                            strlen("iterator"), 0 TSRMLS_CC);

    ZEND_FETCH_RESOURCE(it, xcb_screen_iterator_t*, &rsrc, -1,
                        "xcb_screen_iterator_t", le_xcb_screen_iterator);

    ALLOC_INIT_ZVAL(data);
    object_init(data);

    xcb_screen_next(it);
    xcb_screen_t_to_object(it->data, data);

    add_property_zval(obj, "data", data);
    add_property_long(obj, "rem", it->rem);
    add_property_long(obj, "index", it->index);
}
