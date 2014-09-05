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

zend_function_entry xcb_functions[] = {
    PHP_FE(xcb_connect, NULL)
    PHP_FE(xcb_disconnect, NULL)
    PHP_FE(xcb_connection_has_error, NULL)
    PHP_FE(xcb_flush, NULL)
    PHP_FE(xcb_generate_id, NULL)
    PHP_FE(xcb_get_setup, NULL)
    PHP_FE(xcb_setup_roots_iterator, NULL)
    {NULL, NULL, NULL}
};

zend_module_entry xcb_module_entry = {
    STANDARD_MODULE_HEADER,
    PHP_XCB_MODULE_NAME,
    xcb_functions,
    PHP_MINIT(xcb),
    PHP_MSHUTDOWN(xcb),
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

PHP_MINIT_FUNCTION(xcb)
{
    zend_register_list_destructors(xcb_setup, NULL);
    return SUCCESS;
}

PHP_MSHUTDOWN_FUNCTION(xcb)
{
    if (c != NULL) {
        xcb_disconnect(c);
    }
    return SUCCESS;
}

PHP_FUNCTION(xcb_connect)
{
    char *displayname = NULL;
    int l;
    int *screenp = NULL;

    if (c != NULL) {
        zend_error(E_NOTICE, "Already connected to the X server");
        return;
    }

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
                              "|sl", &displayname, &l, &screenp) == FAILURE) {
        return;
    }

    c = xcb_connect(displayname, screenp);
}

PHP_FUNCTION(xcb_disconnect)
{
    xcb_disconnect(c);
    c = NULL;
}

PHP_FUNCTION(xcb_connection_has_error)
{
    RETURN_LONG(xcb_connection_has_error(c));
}

PHP_FUNCTION(xcb_flush)
{
    RETURN_LONG(xcb_flush(c));
}

PHP_FUNCTION(xcb_generate_id)
{
    RETURN_LONG(xcb_generate_id(c));
}

PHP_FUNCTION(xcb_get_setup)
{
    const xcb_setup_t *setup = xcb_get_setup(c);
    ZEND_REGISTER_RESOURCE(return_value, (xcb_setup_t *) setup, le_xcb_setup);
}

PHP_FUNCTION(xcb_setup_roots_iterator)
{
    zval *rsrc, *data;
    const xcb_setup_t *setup;
    xcb_screen_iterator_t iterator;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
                              "r", &rsrc) == FAILURE) {
        return;
    }

    ZEND_FETCH_RESOURCE(setup, xcb_setup_t*, &rsrc, -1, "xcb_setup_t",
                        le_xcb_setup);

    object_init(return_value);
    ALLOC_INIT_ZVAL(data);
    object_init(data);

    iterator = xcb_setup_roots_iterator(setup);
    xcb_screen_t_to_object(iterator.data, data);

    add_property_zval(return_value, "data", data);
    add_property_long(return_value, "rem", iterator.rem);
    add_property_long(return_value, "index", iterator.index);
}
