/*
  +----------------------------------------------------------------------+
  | PHP Version 5                                                        |
  +----------------------------------------------------------------------+
  | Copyright (c) 1997-2011 The PHP Group                                |
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

PHP_MINIT_FUNCTION(xcb)
{
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
