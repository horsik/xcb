#ifndef PHP_XCB_H
#define PHP_XCB_H

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <xcb/xcb.h>
#include <xcb/xcb_keysyms.h>
#include <sys/time.h>
#include <xcb/xinerama.h>
#include <php5/main/php.h>
#include <php5/Zend/zend_API.h>

#ifdef ZTS
#include "TSRM.h"
#endif

#define PHP_XCB_MODULE_NAME "XCB"
#define PHP_XCB_VERSION "1.0"
#define PHP_XCB_NO_XINERAMA "Xinerama extension not present"

#define zend_register_list_destructors(s, dtor) \
    le_##s = zend_register_list_destructors_ex(dtor, NULL, #s, module_number)

#define assoc_long(name) add_assoc_long(dest, #name, (long) src->name)
#define renamed_assoc_long(oldname, newname) \
    add_assoc_long(dest, #newname, (long) src->oldname)
#define assoc_bool(name) add_assoc_bool(dest, #name, src->name)

#define STRUCT_TO_ASSOC(type) void type##_to_assoc(type *src, zval *dest)

#define EVENT_TO_ASSOC(type)                                    \
void type##_to_assoc(xcb_generic_event_t *event, zval *dest)    \
{                                                               \
    type *src = (type*) event;                                  \
    add_assoc_string(dest, "type", #type, strlen(#type));
#define EVENT_END }

extern zend_module_entry xcb_module_entry;
#define phpext_xcb_ptr &xcb_module_entry;

ZEND_MINIT_FUNCTION(xcb);
ZEND_MSHUTDOWN_FUNCTION(xcb);

ZEND_FUNCTION(xcb_connect);
ZEND_FUNCTION(xcb_disconnect);
ZEND_FUNCTION(xcb_connection_has_error);
ZEND_FUNCTION(xcb_get_file_descriptor);
ZEND_FUNCTION(xcb_flush);
ZEND_FUNCTION(xcb_generate_id);
ZEND_FUNCTION(xcb_get_setup);
ZEND_FUNCTION(xcb_setup_roots_length);
ZEND_FUNCTION(xcb_setup_roots_iterator);
ZEND_FUNCTION(xcb_screen_next);
ZEND_FUNCTION(xcb_select);
ZEND_FUNCTION(xcb_poll_for_event);
ZEND_FUNCTION(xcb_change_window_attributes);
ZEND_FUNCTION(xcb_key_symbols_alloc);
ZEND_FUNCTION(xcb_key_symbols_get_keysym);
ZEND_FUNCTION(xcb_key_symbols_get_keycode);
ZEND_FUNCTION(xcb_query_extension);
ZEND_FUNCTION(xcb_list_extensions);
ZEND_FUNCTION(xcb_xinerama_is_active);
ZEND_FUNCTION(xcb_xinerama_query_screens);
ZEND_FUNCTION(xcb_xinerama_query_screens_screen_info_length);
ZEND_FUNCTION(xcb_xinerama_query_screens_screen_info_iterator);
ZEND_FUNCTION(xcb_xinerama_screen_info_next);
ZEND_FUNCTION(xcb_xinerama_get_screen_count);
ZEND_FUNCTION(xcb_xinerama_get_screen_size);
ZEND_FUNCTION(xcb_xinerama_get_state);
ZEND_FUNCTION(xcb_map_window);
ZEND_FUNCTION(xcb_map_subwindows);
ZEND_FUNCTION(xcb_unmap_window);
ZEND_FUNCTION(xcb_configure_window);
ZEND_FUNCTION(xcb_grab_key);
ZEND_FUNCTION(xcb_ungrab_key);
ZEND_FUNCTION(xcb_grab_button);
ZEND_FUNCTION(xcb_ungrab_button);
ZEND_FUNCTION(xcb_get_input_focus);
ZEND_FUNCTION(xcb_set_input_focus);
ZEND_FUNCTION(xcb_create_window);
ZEND_FUNCTION(xcb_reparent_window);
ZEND_FUNCTION(xcb_destroy_window);
ZEND_FUNCTION(xcb_intern_atom);
ZEND_FUNCTION(xcb_get_atom_name);
ZEND_FUNCTION(xcb_get_property);
ZEND_FUNCTION(xcb_change_property);
ZEND_FUNCTION(xcb_get_geometry);
ZEND_FUNCTION(xcb_query_tree);
ZEND_FUNCTION(xcb_query_tree_children);
ZEND_FUNCTION(xcb_get_window_attributes);
ZEND_FUNCTION(xcb_change_save_set);
ZEND_FUNCTION(xcb_send_event);

#endif
