#ifndef PHP_XCB_H
#define PHP_XCB_H

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <xcb/xcb.h>
#include <xcb/xcb_keysyms.h>
#include <X11/keysym.h>
#include <sys/time.h>
#include "php.h"

#define PHP_XCB_MODULE_NAME "XCB"
#define PHP_XCB_VERSION "1.0"

#define zend_register_list_destructors(s) \
    le_##s = zend_register_list_destructors_ex(s##_dtor, NULL, #s, module_number)

extern zend_module_entry xcb_module_entry;
#define phpext_xcb_ptr &xcb_module_entry;

PHP_MINIT_FUNCTION(xcb);
PHP_MSHUTDOWN_FUNCTION(xcb);

PHP_FUNCTION(xcb_connect);
PHP_FUNCTION(xcb_disconnect);
PHP_FUNCTION(xcb_connection_has_error);
PHP_FUNCTION(xcb_flush);
PHP_FUNCTION(xcb_generate_id);

#endif
