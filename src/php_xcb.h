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

#define zend_register_list_destructors(s, dtor) \
    le_##s = zend_register_list_destructors_ex(dtor, NULL, #s, module_number)

#define property_long(name) add_property_long(dest, #name, src->name)
#define property_bool(name) add_property_bool(dest, #name, src->name)
#define property_string(name) add_property_string(dest, #name, src->name)

#define assoc_long(name) add_assoc_long(dest, #name, src->name)
#define assoc_bool(name) add_assoc_bool(dest, #name, src->name)
#define assoc_string(name) add_assoc_string(dest, #name, src->name)

#define STRUCT_TO_OBJECT(type) void type##_to_object(type *src, zval *dest)

extern zend_module_entry xcb_module_entry;
#define phpext_xcb_ptr &xcb_module_entry;

PHP_MINIT_FUNCTION(xcb);
PHP_MSHUTDOWN_FUNCTION(xcb);

PHP_FUNCTION(xcb_connect);
PHP_FUNCTION(xcb_disconnect);
PHP_FUNCTION(xcb_connection_has_error);
PHP_FUNCTION(xcb_flush);
PHP_FUNCTION(xcb_generate_id);
PHP_FUNCTION(xcb_get_setup);
PHP_FUNCTION(xcb_setup_roots_iterator);

#endif
