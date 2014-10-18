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
#include "const.c"

zend_class_entry *xcb_iterator_ce;
zend_class_entry *xcb_reply_ce;

zend_function_entry xcb_iterator_functions[] = {
    ZEND_FE_END
};

zend_function_entry xcb_reply_functions[] = {
    ZEND_FE_END
};

ZEND_BEGIN_ARG_INFO(arginfo_xcb_connect, 0)
    ZEND_ARG_PASS_INFO(0)
    ZEND_ARG_PASS_INFO(1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_xcb_screen_next, 1)
    ZEND_ARG_OBJ_INFO(1, iterator, XcbIterator, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_xcb_xinerama_screen_info_next, 1)
    ZEND_ARG_OBJ_INFO(1, iterator, XcbIterator, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_xcb_query_tree_children, 1)
    ZEND_ARG_OBJ_INFO(1, reply, XcbReply, 0)
ZEND_END_ARG_INFO()

zend_function_entry xcb_functions[] = {
    ZEND_FE(xcb_connect, arginfo_xcb_connect)
    ZEND_FE(xcb_disconnect, NULL)
    ZEND_FE(xcb_connection_has_error, NULL)
    ZEND_FE(xcb_get_file_descriptor, NULL)
    ZEND_FE(xcb_flush, NULL)
    ZEND_FE(xcb_generate_id, NULL)
    ZEND_FE(xcb_get_setup, NULL)
    ZEND_FE(xcb_setup_roots_length, NULL)
    ZEND_FE(xcb_setup_roots_iterator, NULL)
    ZEND_FE(xcb_screen_next, arginfo_xcb_screen_next)
    ZEND_FE(xcb_select, NULL)
    ZEND_FE(xcb_poll_for_event, NULL)
    ZEND_FE(xcb_change_window_attributes, NULL)
    ZEND_FE(xcb_grab_key, NULL)
    ZEND_FE(xcb_ungrab_key, NULL)
    ZEND_FE(xcb_grab_button, NULL)
    ZEND_FE(xcb_ungrab_button, NULL)
    ZEND_FE(xcb_key_symbols_alloc, NULL)
    ZEND_FE(xcb_key_symbols_get_keycode, NULL)
    ZEND_FE(xcb_key_symbols_get_keysym, NULL)
    ZEND_FE(xcb_query_extension, NULL)
    ZEND_FE(xcb_list_extensions, NULL)
    ZEND_FE(xcb_xinerama_is_active, NULL)
    ZEND_FE(xcb_xinerama_query_screens, NULL)
    ZEND_FE(xcb_xinerama_query_screens_screen_info_length, NULL)
    ZEND_FE(xcb_xinerama_query_screens_screen_info_iterator, NULL)
    ZEND_FE(xcb_xinerama_screen_info_next, arginfo_xcb_xinerama_screen_info_next)
    ZEND_FE(xcb_xinerama_get_screen_count, NULL)
    ZEND_FE(xcb_xinerama_get_screen_size, NULL)
    ZEND_FE(xcb_xinerama_get_state, NULL)
    ZEND_FE(xcb_map_window, NULL)
    ZEND_FE(xcb_map_subwindows, NULL)
    ZEND_FE(xcb_unmap_window, NULL)
    ZEND_FE(xcb_configure_window, NULL)
    ZEND_FE(xcb_get_input_focus, NULL)
    ZEND_FE(xcb_set_input_focus, NULL)
    ZEND_FE(xcb_create_window, NULL)
    ZEND_FE(xcb_reparent_window, NULL)
    ZEND_FE(xcb_destroy_window, NULL)
    ZEND_FE(xcb_intern_atom, NULL)
    ZEND_FE(xcb_get_atom_name, NULL)
    ZEND_FE(xcb_get_property, NULL)
    ZEND_FE(xcb_change_property, NULL)
    ZEND_FE(xcb_get_geometry, NULL)
    ZEND_FE(xcb_query_tree, NULL)
    ZEND_FE(xcb_query_tree_children, arginfo_xcb_query_tree_children)
    ZEND_FE(xcb_get_window_attributes, NULL)
    ZEND_FE(xcb_change_save_set, NULL)
    ZEND_FE(xcb_send_event, NULL)
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

static xcb_connection_t *c = NULL;
static int le_xcb_setup;
static int le_xcb_screen_iterator;
static int le_xcb_key_symbols;
static int le_xcb_xinerama_query_screens_reply;
static int le_xcb_xinerama_screen_info_iterator;
static int le_xcb_query_tree_reply;

void xcb_generic_dtor(zend_rsrc_list_entry *rsrc TSRMLS_CC)
{
    free(rsrc->ptr);
}

void xcb_key_symbols_dtor(zend_rsrc_list_entry *rsrc TSRMLS_CC)
{
    xcb_key_symbols_free(rsrc->ptr);
}

static int check_connection()
{
    if (c == NULL) {
        zend_error(E_WARNING, "Not connected to the X server");
        return 0;
    }

    return 1;
}

void php_array_to_long_array(zval *src, uint32_t *dest)
{
    int i;

    zend_hash_internal_pointer_reset(HASH_OF(src));

    for (i = 0; ; zend_hash_move_forward(HASH_OF(src)), i++) {
        zval **item;

        if (zend_hash_get_current_data(HASH_OF(src),
                                       (void **) &item) == FAILURE) {
            break;
        }

        convert_to_long(*item);
        dest[i] = Z_LVAL_PP(item);
    }
}

STRUCT_TO_ASSOC(xcb_screen_t)
{
    assoc_long(root);
    assoc_long(default_colormap);
    assoc_long(white_pixel);
    assoc_long(black_pixel);
    assoc_long(current_input_masks);
    renamed_assoc_long(width_in_pixels, width);
    renamed_assoc_long(height_in_pixels, height);
    assoc_long(width_in_millimeters);
    assoc_long(height_in_millimeters);
    assoc_long(min_installed_maps);
    assoc_long(max_installed_maps);
    assoc_long(root_visual);
    assoc_long(backing_stores);
    assoc_bool(save_unders);
    assoc_long(root_depth);
    assoc_long(allowed_depths_len);
}

STRUCT_TO_ASSOC(xcb_generic_event_t)
{
    assoc_long(response_type);
    assoc_long(sequence);
    assoc_long(full_sequence);
}

STRUCT_TO_ASSOC(xcb_xinerama_screen_info_t)
{
    renamed_assoc_long(x_org, x);
    renamed_assoc_long(y_org, y);
    assoc_long(width);
    assoc_long(height);
}

STRUCT_TO_ASSOC(xcb_xinerama_get_screen_count_reply_t)
{
    assoc_long(screen_count);
    assoc_long(window);
}

STRUCT_TO_ASSOC(xcb_xinerama_get_screen_size_reply_t)
{
    assoc_long(width);
    assoc_long(height);
    assoc_long(window);
    assoc_long(screen);
}

STRUCT_TO_ASSOC(xcb_xinerama_get_state_reply_t)
{
    assoc_long(state);
    assoc_long(window);
}

STRUCT_TO_ASSOC(xcb_get_geometry_reply_t)
{
    assoc_long(depth);
    assoc_long(root);
    assoc_long(x);
    assoc_long(y);
    assoc_long(width);
    assoc_long(height);
    assoc_long(border_width);
}

STRUCT_TO_ASSOC(xcb_get_property_reply_t)
{
    assoc_long(format);
    assoc_long(type);
    assoc_long(bytes_after);
    assoc_long(value_len);
}

STRUCT_TO_ASSOC(xcb_get_window_attributes_reply_t)
{
    assoc_long(backing_store);
    assoc_long(visual);
    assoc_long(_class);
    assoc_long(bit_gravity);
    assoc_long(win_gravity);
    assoc_long(backing_planes);
    assoc_long(backing_pixel);
    assoc_bool(save_under);
    assoc_bool(map_is_installed);
    assoc_long(map_state);
    assoc_bool(override_redirect);
    assoc_long(colormap);
    assoc_long(all_event_masks);
    assoc_long(your_event_mask);
    assoc_long(do_not_propagate_mask);
}

STRUCT_TO_ASSOC(xcb_get_input_focus_reply_t)
{
    assoc_long(revert_to);
    assoc_long(focus);
}

EVENT_TO_ASSOC(xcb_generic_error_t)
{
    assoc_long(error_code);
    assoc_long(sequence);
    assoc_long(resource_id);
    assoc_long(minor_code);
    assoc_long(major_code);
    assoc_long(full_sequence);
    EVENT_END
}

EVENT_TO_ASSOC(xcb_key_press_event_t)
{
    assoc_long(detail);
    assoc_long(time);
    assoc_long(root);
    assoc_long(event);
    assoc_long(child);
    assoc_long(root_x);
    assoc_long(root_y);
    assoc_long(event_x);
    assoc_long(event_y);
    assoc_long(state);
    assoc_bool(same_screen);
    EVENT_END
}

EVENT_TO_ASSOC(xcb_key_release_event_t)
{
    assoc_long(detail);
    assoc_long(time);
    assoc_long(root);
    assoc_long(event);
    assoc_long(child);
    assoc_long(root_x);
    assoc_long(root_y);
    assoc_long(event_x);
    assoc_long(event_y);
    assoc_long(state);
    assoc_bool(same_screen);
    EVENT_END
}

EVENT_TO_ASSOC(xcb_button_press_event_t)
{
    assoc_long(detail);
    assoc_long(time);
    assoc_long(root);
    assoc_long(event);
    assoc_long(child);
    assoc_long(root_x);
    assoc_long(root_y);
    assoc_long(event_x);
    assoc_long(event_y);
    assoc_long(state);
    assoc_bool(same_screen);
    EVENT_END
}

EVENT_TO_ASSOC(xcb_button_release_event_t)
{
    assoc_long(detail);
    assoc_long(time);
    assoc_long(root);
    assoc_long(event);
    assoc_long(child);
    assoc_long(root_x);
    assoc_long(root_y);
    assoc_long(event_x);
    assoc_long(event_y);
    assoc_long(state);
    assoc_bool(same_screen);
    EVENT_END
}

EVENT_TO_ASSOC(xcb_motion_notify_event_t)
{
    assoc_long(detail);
    assoc_long(time);
    assoc_long(root);
    assoc_long(event);
    assoc_long(child);
    assoc_long(root_x);
    assoc_long(root_y);
    assoc_long(event_x);
    assoc_long(event_y);
    assoc_long(state);
    assoc_bool(same_screen);
    EVENT_END
}

EVENT_TO_ASSOC(xcb_enter_notify_event_t)
{
    assoc_long(detail);
    assoc_long(time);
    assoc_long(root);
    assoc_long(event);
    assoc_long(child);
    assoc_long(root_x);
    assoc_long(root_y);
    assoc_long(event_x);
    assoc_long(event_y);
    assoc_long(state);
    assoc_long(mode);
    assoc_bool(same_screen_focus);
    EVENT_END
}

EVENT_TO_ASSOC(xcb_leave_notify_event_t)
{
    assoc_long(detail);
    assoc_long(time);
    assoc_long(root);
    assoc_long(event);
    assoc_long(child);
    assoc_long(root_x);
    assoc_long(root_y);
    assoc_long(event_x);
    assoc_long(event_y);
    assoc_long(state);
    assoc_long(mode);
    assoc_bool(same_screen_focus);
    EVENT_END
}

EVENT_TO_ASSOC(xcb_focus_in_event_t)
{
    assoc_long(detail);
    assoc_long(event);
    assoc_long(mode);
    EVENT_END
}

EVENT_TO_ASSOC(xcb_focus_out_event_t)
{
    assoc_long(detail);
    assoc_long(event);
    assoc_long(mode);
    EVENT_END
}

EVENT_TO_ASSOC(xcb_keymap_notify_event_t)
{
    int i;
    zval *keys;

    ALLOC_INIT_ZVAL(keys);
    array_init(keys);

    for (i = 0; i < 31; i++) {
        add_next_index_long(keys, src->keys[i]);
    }

    add_assoc_zval(dest, "keys", keys);
    EVENT_END
}

EVENT_TO_ASSOC(xcb_expose_event_t)
{
    assoc_long(window);
    assoc_long(x);
    assoc_long(y);
    assoc_long(width);
    assoc_long(height);
    assoc_long(count);
    EVENT_END
}

EVENT_TO_ASSOC(xcb_graphics_exposure_event_t)
{
    assoc_long(drawable);
    assoc_long(x);
    assoc_long(y);
    assoc_long(width);
    assoc_long(height);
    assoc_long(minor_opcode);
    assoc_long(count);
    assoc_long(major_opcode);
    EVENT_END
}

EVENT_TO_ASSOC(xcb_no_exposure_event_t)
{
    assoc_long(drawable);
    assoc_long(minor_opcode);
    assoc_long(major_opcode);
    EVENT_END
}

EVENT_TO_ASSOC(xcb_visibility_notify_event_t)
{
    assoc_long(window);
    assoc_long(state);
    EVENT_END
}

EVENT_TO_ASSOC(xcb_create_notify_event_t)
{
    assoc_long(parent);
    assoc_long(window);
    assoc_long(x);
    assoc_long(y);
    assoc_long(width);
    assoc_long(height);
    assoc_long(border_width);
    assoc_long(override_redirect);
    EVENT_END
}

EVENT_TO_ASSOC(xcb_destroy_notify_event_t)
{
    assoc_long(event);
    assoc_long(window);
    EVENT_END
}

EVENT_TO_ASSOC(xcb_unmap_notify_event_t)
{
    assoc_long(event);
    assoc_long(window);
    assoc_long(from_configure);
    EVENT_END
}

EVENT_TO_ASSOC(xcb_map_notify_event_t)
{
    assoc_long(event);
    assoc_long(window);
    assoc_long(override_redirect);
    EVENT_END
}

EVENT_TO_ASSOC(xcb_map_request_event_t)
{
    assoc_long(parent);
    assoc_long(window);
    EVENT_END
}

EVENT_TO_ASSOC(xcb_reparent_notify_event_t)
{
    assoc_long(event);
    assoc_long(window);
    assoc_long(parent);
    assoc_long(x);
    assoc_long(y);
    assoc_long(override_redirect);
    EVENT_END
}

EVENT_TO_ASSOC(xcb_configure_notify_event_t)
{
    assoc_long(event);
    assoc_long(window);
    assoc_long(above_sibling);
    assoc_long(x);
    assoc_long(y);
    assoc_long(width);
    assoc_long(height);
    assoc_long(border_width);
    assoc_long(override_redirect);
    EVENT_END
}

EVENT_TO_ASSOC(xcb_gravity_notify_event_t)
{
    assoc_long(event);
    assoc_long(window);
    assoc_long(x);
    assoc_long(y);
    EVENT_END
}

EVENT_TO_ASSOC(xcb_resize_request_event_t)
{
    assoc_long(window);
    assoc_long(width);
    assoc_long(height);
    EVENT_END
}

EVENT_TO_ASSOC(xcb_configure_request_event_t)
{
    assoc_long(stack_mode);
    assoc_long(parent);
    assoc_long(window);
    assoc_long(sibling);
    assoc_long(x);
    assoc_long(y);
    assoc_long(width);
    assoc_long(height);
    assoc_long(border_width);
    assoc_long(value_mask);
    EVENT_END
}

EVENT_TO_ASSOC(xcb_circulate_notify_event_t)
{
    assoc_long(event);
    assoc_long(window);
    assoc_long(place);
    EVENT_END
}

EVENT_TO_ASSOC(xcb_circulate_request_event_t)
{
    assoc_long(event);
    assoc_long(window);
    assoc_long(place);
    EVENT_END
}

EVENT_TO_ASSOC(xcb_property_notify_event_t)
{
    assoc_long(window);
    assoc_long(atom);
    assoc_long(time);
    assoc_long(state);
    EVENT_END
}

EVENT_TO_ASSOC(xcb_selection_clear_event_t)
{
    assoc_long(time);
    assoc_long(owner);
    assoc_long(selection);
    EVENT_END
}

EVENT_TO_ASSOC(xcb_selection_request_event_t)
{
    assoc_long(time);
    assoc_long(owner);
    assoc_long(requestor);
    assoc_long(selection);
    assoc_long(target);
    assoc_long(property);
    EVENT_END
}

EVENT_TO_ASSOC(xcb_selection_notify_event_t)
{
    assoc_long(time);
    assoc_long(requestor);
    assoc_long(selection);
    assoc_long(target);
    assoc_long(property);
    EVENT_END
}

EVENT_TO_ASSOC(xcb_colormap_notify_event_t)
{
    assoc_long(window);
    assoc_long(colormap);
    assoc_long(_new);
    assoc_long(state);
    EVENT_END
}

EVENT_TO_ASSOC(xcb_mapping_notify_event_t)
{
    assoc_long(request);
    assoc_long(first_keycode);
    assoc_long(count);
    EVENT_END
}

EVENT_TO_ASSOC(xcb_client_message_event_t)
{
    int i;
    zval *data;

    assoc_long(format);
    assoc_long(window);
    assoc_long(type);

    ALLOC_INIT_ZVAL(data);
    array_init(data);

    switch (src->format) {
        case 8:
            for (i = 0; i < 20; i++) {
                add_next_index_long(data, src->data.data8[i]);
            }
            break;
        case 16:
            for (i = 0; i < 10; i++) {
                add_next_index_long(data, src->data.data16[i]);
            }
            break;
        case 32:
            for (i = 0; i < 5; i++) {
                add_next_index_long(data, src->data.data32[i]);
            }
            break;
    }

    add_assoc_zval(dest, "data", data);
    EVENT_END
}

ZEND_MINIT_FUNCTION(xcb)
{
    zend_class_entry tmp_ce;
    INIT_CLASS_ENTRY(tmp_ce, "XcbIterator", xcb_iterator_functions);

    xcb_iterator_ce = zend_register_internal_class(&tmp_ce TSRMLS_CC);
    zend_declare_property_null(xcb_iterator_ce, "iterator", strlen("iterator"),
                               ZEND_ACC_PRIVATE TSRMLS_CC);

    INIT_CLASS_ENTRY(tmp_ce, "XcbReply", xcb_reply_functions);

    xcb_reply_ce = zend_register_internal_class(&tmp_ce TSRMLS_CC);
    zend_declare_property_null(xcb_reply_ce, "reply", strlen("reply"),
                               ZEND_ACC_PRIVATE TSRMLS_CC);

    zend_register_list_destructors(xcb_setup, NULL);
    zend_register_list_destructors(xcb_screen_iterator, xcb_generic_dtor);
    zend_register_list_destructors(xcb_key_symbols, xcb_key_symbols_dtor);
    zend_register_list_destructors(xcb_xinerama_query_screens_reply,
                                   xcb_generic_dtor);
    zend_register_list_destructors(xcb_xinerama_screen_info_iterator,
                                   xcb_generic_dtor);
    zend_register_list_destructors(xcb_query_tree_reply, xcb_generic_dtor);

    import_constants(module_number);

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
    if (!check_connection()) {
        return;
    }

    RETURN_LONG(xcb_connection_has_error(c));
}

ZEND_FUNCTION(xcb_get_file_descriptor)
{
    if (!check_connection()) {
        return;
    }

    RETURN_LONG(xcb_get_file_descriptor(c));
}

ZEND_FUNCTION(xcb_flush)
{
    if (!check_connection()) {
        return;
    }

    RETURN_LONG(xcb_flush(c));
}

ZEND_FUNCTION(xcb_generate_id)
{
    if (!check_connection()) {
        return;
    }

    RETURN_LONG(xcb_generate_id(c));
}

ZEND_FUNCTION(xcb_get_setup)
{
    if (!check_connection()) {
        return;
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
    array_init(data);

    it = malloc(sizeof(xcb_screen_iterator_t));
    *it = xcb_setup_roots_iterator(setup);

    xcb_screen_t_to_assoc(it->data, data);
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
    array_init(data);

    xcb_screen_next(it);
    xcb_screen_t_to_assoc(it->data, data);

    add_property_zval(obj, "data", data);
    add_property_long(obj, "rem", it->rem);
    add_property_long(obj, "index", it->index);
}

ZEND_FUNCTION(xcb_select)
{
    int fd;
    fd_set rfds;
    zval *timeout = NULL;
    struct timeval tv, *tv_ptr = NULL;

    if (!check_connection() || zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
            "l|z", &fd, &timeout) == FAILURE) {
        return;
    }

    if (timeout != NULL) {
        convert_to_long_ex(&timeout);

        tv.tv_sec = Z_LVAL_P(timeout);
        tv.tv_usec = 0;

        tv_ptr = &tv;
    }

    FD_ZERO(&rfds);
    FD_SET(fd, &rfds);

    RETURN_LONG(select(fd + 1, &rfds, NULL, NULL, tv_ptr));
}

ZEND_FUNCTION(xcb_poll_for_event)
{
    xcb_generic_event_t *e;

    if (!check_connection || !(e = xcb_poll_for_event(c))) {
        RETURN_BOOL(0);
    }

    array_init(return_value);

    switch(e->response_type) {
        case XCB_KEY_PRESS:
            xcb_key_press_event_t_to_assoc(e, return_value);
            break;
        case XCB_KEY_RELEASE:
            xcb_key_release_event_t_to_assoc(e, return_value);
            break;
        case XCB_BUTTON_PRESS:
            xcb_button_press_event_t_to_assoc(e, return_value);
            break;
        case XCB_BUTTON_RELEASE:
            xcb_button_release_event_t_to_assoc(e, return_value);
            break;
        case XCB_MOTION_NOTIFY:
            xcb_motion_notify_event_t_to_assoc(e, return_value);
            break;
        case XCB_ENTER_NOTIFY:
            xcb_enter_notify_event_t_to_assoc(e, return_value);
            break;
        case XCB_LEAVE_NOTIFY:
            xcb_leave_notify_event_t_to_assoc(e, return_value);
            break;
        case XCB_FOCUS_IN:
            xcb_focus_in_event_t_to_assoc(e, return_value);
            break;
        case XCB_FOCUS_OUT:
            xcb_focus_out_event_t_to_assoc(e, return_value);
            break;
        case XCB_KEYMAP_NOTIFY:
            xcb_keymap_notify_event_t_to_assoc(e, return_value);
            break;
        case XCB_EXPOSE:
            xcb_expose_event_t_to_assoc(e, return_value);
            break;
        case XCB_GRAPHICS_EXPOSURE:
            xcb_graphics_exposure_event_t_to_assoc(e, return_value);
            break;
        case XCB_NO_EXPOSURE:
            xcb_no_exposure_event_t_to_assoc(e, return_value);
            break;
        case XCB_VISIBILITY_NOTIFY:
            xcb_visibility_notify_event_t_to_assoc(e, return_value);
            break;
        case XCB_CREATE_NOTIFY:
            xcb_create_notify_event_t_to_assoc(e, return_value);
            break;
        case XCB_DESTROY_NOTIFY:
            xcb_destroy_notify_event_t_to_assoc(e, return_value);
            break;
        case XCB_UNMAP_NOTIFY:
            xcb_unmap_notify_event_t_to_assoc(e, return_value);
            break;
        case XCB_MAP_NOTIFY:
            xcb_map_notify_event_t_to_assoc(e, return_value);
            break;
        case XCB_MAP_REQUEST:
            xcb_map_request_event_t_to_assoc(e, return_value);
            break;
        case XCB_REPARENT_NOTIFY:
            xcb_reparent_notify_event_t_to_assoc(e, return_value);
            break;
        case XCB_CONFIGURE_NOTIFY:
            xcb_configure_notify_event_t_to_assoc(e, return_value);
            break;
        case XCB_GRAVITY_NOTIFY:
            xcb_gravity_notify_event_t_to_assoc(e, return_value);
            break;
        case XCB_RESIZE_REQUEST:
            xcb_resize_request_event_t_to_assoc(e, return_value);
            break;
        case XCB_CONFIGURE_REQUEST:
            xcb_configure_request_event_t_to_assoc(e, return_value);
            break;
        case XCB_CIRCULATE_NOTIFY:
            xcb_circulate_notify_event_t_to_assoc(e, return_value);
            break;
        case XCB_CIRCULATE_REQUEST:
            xcb_circulate_request_event_t_to_assoc(e, return_value);
            break;
        case XCB_PROPERTY_NOTIFY:
            xcb_property_notify_event_t_to_assoc(e, return_value);
            break;
        case XCB_SELECTION_CLEAR:
            xcb_selection_clear_event_t_to_assoc(e, return_value);
            break;
        case XCB_SELECTION_REQUEST:
            xcb_selection_request_event_t_to_assoc(e, return_value);
            break;
        case XCB_SELECTION_NOTIFY:
            xcb_selection_notify_event_t_to_assoc(e, return_value);
            break;
        case XCB_COLORMAP_NOTIFY:
            xcb_colormap_notify_event_t_to_assoc(e, return_value);
            break;
        case XCB_MAPPING_NOTIFY:
            xcb_mapping_notify_event_t_to_assoc(e, return_value);
            break;
        case XCB_CLIENT_MESSAGE:
            xcb_client_message_event_t_to_assoc(e, return_value);
            break;
        case XCB_NONE:
            xcb_generic_error_t_to_assoc(e, return_value);
            break;
        default:
            xcb_generic_event_t_to_assoc(e, return_value);
    }

    free(e);
}

ZEND_FUNCTION(xcb_change_window_attributes)
{
    long window, value_mask, n;
    zval *values;
    xcb_void_cookie_t cookie;

    if (!check_connection() || zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
            "lla", &window, &value_mask, &values) == FAILURE) {
        return;
    }

    n = zend_hash_num_elements(HASH_OF(values)) - 1;

    if (n >= 0) {
        uint32_t value_list[n];

        php_array_to_long_array(values, value_list);
        cookie = xcb_change_window_attributes(c, window, value_mask, value_list);

        RETURN_LONG(cookie.sequence);
    }
}

ZEND_FUNCTION(xcb_grab_key)
{
    long owner_events, grab_window, modifiers, key, pointer_mode, keyboard_mode;
    xcb_void_cookie_t cookie;

    if (!check_connection || zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
            "llllll", &owner_events, &grab_window, &modifiers, &key,
            &pointer_mode, &keyboard_mode) == FAILURE) {
        return;
    }

    cookie = xcb_grab_key(c, owner_events, grab_window, modifiers, key,
                          pointer_mode, keyboard_mode);

    RETURN_LONG(cookie.sequence);
}

ZEND_FUNCTION(xcb_ungrab_key)
{
    long key, grab_window, modifiers;

    if (!check_connection || zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
            "lll", &key, &grab_window, &modifiers) == FAILURE) {
        return;
    }

    xcb_ungrab_key(c, key, grab_window, modifiers);
}

ZEND_FUNCTION(xcb_grab_button)
{
    long owner_events, grab_window, event_mask, pointer_mode, keyboard_mode,
         confine_to, cursor, button, modifiers;

    if (!check_connection || zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
            "lllllllll", &owner_events, &grab_window, &event_mask,
            &pointer_mode, &keyboard_mode, &confine_to, &cursor, &button,
            &modifiers) == FAILURE) {
        return;
    }

    xcb_grab_button(c, owner_events, grab_window, event_mask, pointer_mode,
                    keyboard_mode, confine_to, cursor, button, modifiers);
}

ZEND_FUNCTION(xcb_ungrab_button)
{
    long button, grab_window, modifiers;

    if (!check_connection || zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
            "lll", &button, &grab_window, &modifiers) == FAILURE) {
        return;
    }

    xcb_ungrab_button(c, button, grab_window, modifiers);
}

ZEND_FUNCTION(xcb_key_symbols_alloc)
{
    xcb_key_symbols_t *syms = xcb_key_symbols_alloc(c);
    ZEND_REGISTER_RESOURCE(return_value, syms, le_xcb_key_symbols);
}

ZEND_FUNCTION(xcb_key_symbols_get_keycode)
{
    zval *syms_resource;
    xcb_key_symbols_t *syms;
    long keysym;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "rl",
                              &syms_resource, &keysym) == FAILURE) {
        return;
    }

    ZEND_FETCH_RESOURCE(syms, xcb_key_symbols_t*, &syms_resource, -1,
        "xcb_key_symbols_t", le_xcb_key_symbols);

    xcb_keycode_t *key = xcb_key_symbols_get_keycode(syms, keysym);

    if (key) {
        RETVAL_LONG(*key);
        free(key);
    }
}

ZEND_FUNCTION(xcb_key_symbols_get_keysym)
{
    zval *syms_resource;
    xcb_key_symbols_t *syms;
    long keycode, col;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "rll",
                              &syms_resource, &keycode, &col) == FAILURE) {
        return;
    }

    ZEND_FETCH_RESOURCE(syms, xcb_key_symbols_t*, &syms_resource, -1,
        "xcb_key_symbols_t", le_xcb_key_symbols);

    RETURN_LONG(xcb_key_symbols_get_keysym(syms, keycode, col));
}

ZEND_FUNCTION(xcb_query_extension)
{
    char *name;
    long name_len;
    xcb_query_extension_cookie_t cookie;
    xcb_query_extension_reply_t *reply = NULL;

    if (!check_connection() || zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
            "s", &name, &name_len) == FAILURE) {
        return;
    }

    cookie = xcb_query_extension(c, name_len, name);
    reply = xcb_query_extension_reply(c, cookie, NULL);

    array_init(return_value);

    add_assoc_bool(return_value, "present", reply->present);
    add_assoc_long(return_value, "major_opcode", reply->major_opcode);
    add_assoc_long(return_value, "first_event", reply->first_event);
    add_assoc_long(return_value, "first_error", reply->first_error);

    free(reply);
}

ZEND_FUNCTION(xcb_list_extensions)
{
    xcb_list_extensions_cookie_t cookie;
    xcb_list_extensions_reply_t *reply;
    xcb_str_iterator_t it;

    if (!check_connection()) {
        return;
    }

    cookie = xcb_list_extensions(c);
    reply = xcb_list_extensions_reply(c, cookie, NULL);
    it = xcb_list_extensions_names_iterator(reply);

    array_init(return_value);

    for (; it.rem > 0; xcb_str_next(&it))
    {
        char name[it.data->name_len];
        memcpy(name, xcb_str_name(it.data), it.data->name_len);
        name[it.data->name_len] = '\0';
        add_next_index_string(return_value, name, 1);
    }

    free(reply);
}

ZEND_FUNCTION(xcb_xinerama_is_active)
{
    xcb_xinerama_is_active_cookie_t cookie;
    xcb_xinerama_is_active_reply_t *reply = NULL;

    if (!check_connection()) {
        return;
    }

    cookie = xcb_xinerama_is_active(c);
    reply = xcb_xinerama_is_active_reply(c, cookie, NULL);

    if (!reply) {
        php_error(E_WARNING, PHP_XCB_NO_XINERAMA);
        return;
    }

    RETURN_BOOL(reply->state);
}

ZEND_FUNCTION(xcb_xinerama_query_screens)
{
    xcb_xinerama_query_screens_cookie_t cookie;
    xcb_xinerama_query_screens_reply_t *reply = NULL;

    if (!check_connection()) {
        return;
    }

    cookie = xcb_xinerama_query_screens(c);
    reply = xcb_xinerama_query_screens_reply(c, cookie, NULL);

    if (reply) {
        ZEND_REGISTER_RESOURCE(return_value, reply,
                               le_xcb_xinerama_query_screens_reply);
    }
    else {
        php_error(E_WARNING, PHP_XCB_NO_XINERAMA);
    }
}

ZEND_FUNCTION(xcb_xinerama_query_screens_screen_info_length)
{
    zval *reply_rsrc;
    xcb_xinerama_query_screens_reply_t *reply;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "r",
                              &reply_rsrc) == FAILURE) {
        return;
    }

    ZEND_FETCH_RESOURCE(reply, xcb_xinerama_query_screens_reply_t*,
                        &reply_rsrc, -1, "xcb_xinerama_query_screens_reply_t",
                        le_xcb_xinerama_query_screens_reply);

    RETURN_LONG(xcb_xinerama_query_screens_screen_info_length(reply));
}

ZEND_FUNCTION(xcb_xinerama_query_screens_screen_info_iterator)
{
    zval *reply_rsrc, *it_rsrc, *data;
    xcb_xinerama_query_screens_reply_t *reply;
    xcb_xinerama_screen_info_iterator_t *it;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "r",
                              &reply_rsrc) == FAILURE) {
        return;
    }

    ZEND_FETCH_RESOURCE(reply, xcb_xinerama_query_screens_reply_t*,
                        &reply_rsrc, -1, "xcb_xinerama_query_screens_reply_t",
                        le_xcb_xinerama_query_screens_reply);

    ALLOC_INIT_ZVAL(it_rsrc);
    ALLOC_INIT_ZVAL(data);

    object_init_ex(return_value, xcb_iterator_ce);
    array_init(data);

    it = malloc(sizeof(xcb_xinerama_screen_info_iterator_t));
    *it = xcb_xinerama_query_screens_screen_info_iterator(reply);

    xcb_xinerama_screen_info_t_to_assoc(it->data, data);
    ZEND_REGISTER_RESOURCE(it_rsrc, it, le_xcb_xinerama_screen_info_iterator);

    zend_update_property(xcb_iterator_ce, return_value, "iterator",
                         strlen("iterator"), it_rsrc TSRMLS_CC);
    add_property_zval(return_value, "data", data);
    add_property_long(return_value, "rem", it->rem);
    add_property_long(return_value, "index", it->index);
}

ZEND_FUNCTION(xcb_xinerama_screen_info_next)
{
    zval *obj, *rsrc, *data;
    xcb_xinerama_screen_info_iterator_t *it;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
                              "o", &obj) == FAILURE) {
        return;
    }

    rsrc = zend_read_property(xcb_iterator_ce, obj, "iterator",
                            strlen("iterator"), 0 TSRMLS_CC);

    ZEND_FETCH_RESOURCE(it, xcb_xinerama_screen_info_iterator_t*, &rsrc, -1,
                        "xcb_xinerama_screen_info_iterator_t",
                        le_xcb_xinerama_screen_info_iterator);

    ALLOC_INIT_ZVAL(data);
    array_init(data);

    xcb_xinerama_screen_info_next(it);
    xcb_xinerama_screen_info_t_to_assoc(it->data, data);

    add_property_zval(obj, "data", data);
    add_property_long(obj, "rem", it->rem);
    add_property_long(obj, "index", it->index);
}

ZEND_FUNCTION(xcb_xinerama_get_screen_count)
{
    long window;
    xcb_xinerama_get_screen_count_cookie_t cookie;
    xcb_xinerama_get_screen_count_reply_t *reply = NULL;

    if (!check_connection() || zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
            "l", &window) == FAILURE) {
        return;
    }

    cookie = xcb_xinerama_get_screen_count(c, window);
    reply = xcb_xinerama_get_screen_count_reply(c, cookie, NULL);

    if (reply) {
        array_init(return_value);
        xcb_xinerama_get_screen_count_reply_t_to_assoc(reply, return_value);
        free(reply);
    }
}

ZEND_FUNCTION(xcb_xinerama_get_screen_size)
{
    long window, screen;
    xcb_xinerama_get_screen_size_cookie_t cookie;
    xcb_xinerama_get_screen_size_reply_t *reply = NULL;

    if (!check_connection() || zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
            "ll", &window, &screen) == FAILURE) {
        return;
    }

    cookie = xcb_xinerama_get_screen_size(c, window, screen);
    reply = xcb_xinerama_get_screen_size_reply(c, cookie, NULL);

    if (reply) {
        array_init(return_value);
        xcb_xinerama_get_screen_size_reply_t_to_assoc(reply, return_value);
        free(reply);
    }
}

ZEND_FUNCTION(xcb_xinerama_get_state)
{
    long window;
    xcb_xinerama_get_state_cookie_t cookie;
    xcb_xinerama_get_state_reply_t *reply = NULL;

    if (!check_connection() || zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
            "l", &window) == FAILURE) {
        return;
    }

    cookie = xcb_xinerama_get_state(c, window);
    reply = xcb_xinerama_get_state_reply(c, cookie, NULL);

    if (reply) {
        array_init(return_value);
        xcb_xinerama_get_state_reply_t_to_assoc(reply, return_value);
        free(reply);
    }
}

ZEND_FUNCTION(xcb_map_window)
{
    long window;
    xcb_void_cookie_t cookie;

    if (!check_connection() || zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
            "l", &window) == FAILURE) {
        return;
    }

    cookie = xcb_map_window(c, window);
    RETURN_LONG(cookie.sequence);
}

ZEND_FUNCTION(xcb_map_subwindows)
{
    long window;
    xcb_void_cookie_t cookie;

    if (!check_connection() || zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
            "l", &window) == FAILURE) {
        return;
    }

    cookie = xcb_map_subwindows(c, window);
    RETURN_LONG(cookie.sequence);
}

ZEND_FUNCTION(xcb_unmap_window)
{
    long window;
    xcb_void_cookie_t cookie;

    if (!check_connection() || zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
            "l", &window) == FAILURE) {
        return;
    }

    cookie = xcb_unmap_window(c, window);
    RETURN_LONG(cookie.sequence);
}

ZEND_FUNCTION(xcb_configure_window)
{
    long window, value_mask, n;
    zval *values;
    xcb_void_cookie_t cookie;

    if (!check_connection() || zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
            "lla", &window, &value_mask, &values) == FAILURE) {
        return;
    }

    n = zend_hash_num_elements(HASH_OF(values)) - 1;

    if (n >= 0) {
        uint32_t value_list[n];

        php_array_to_long_array(values, value_list);
        cookie = xcb_configure_window(c, window, value_mask, value_list);
        RETURN_LONG(cookie.sequence);
    }
}

ZEND_FUNCTION(xcb_get_input_focus)
{
    xcb_get_input_focus_cookie_t cookie;
    xcb_get_input_focus_reply_t *reply = NULL;

    if (!check_connection()) {
        return;
    }

    cookie = xcb_get_input_focus(c);
    reply = xcb_get_input_focus_reply(c, cookie, NULL);

    if (reply) {
        array_init(return_value);
        xcb_get_input_focus_reply_t_to_assoc(reply, return_value);
        free(reply);
    }
}

ZEND_FUNCTION(xcb_set_input_focus)
{
    long revert_to, window, time;
    xcb_void_cookie_t cookie;

    if (!check_connection() || zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
            "lll", &revert_to, &window, &time) == FAILURE) {
        return;
    }

    cookie = xcb_set_input_focus(c, revert_to, window, time);
    RETURN_LONG(cookie.sequence);
}

ZEND_FUNCTION(xcb_create_window)
{
    long depth, wid, parent, x, y, width, height, border_width, _class,
         visual, value_mask, n;
    zval *values;
    uint32_t *value_list = NULL;
    xcb_void_cookie_t cookie;

    if (!check_connection() || zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
            "llllllllllla", &depth, &wid, &parent, &x, &y, &width, &height,
            &border_width, &_class, &visual, &value_mask, &values) == FAILURE) {
        return;
    }

    n = zend_hash_num_elements(HASH_OF(values)) - 1;

    if (n >= 0) {
        value_list = (uint32_t *)malloc(sizeof(uint32_t) * n);
        php_array_to_long_array(values, value_list);
    }

    cookie = xcb_create_window(c, depth, wid, parent, x, y, width, height,
                      border_width, _class, visual, value_mask, value_list);
    RETVAL_LONG(cookie.sequence);

    free(value_list);
}

ZEND_FUNCTION(xcb_reparent_window)
{
    long window, parent, x, y;
    xcb_void_cookie_t cookie;

    if (!check_connection() || zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
            "llll", &window, &parent, &x, &y) == FAILURE) {
        return;
    }

    cookie = xcb_reparent_window(c, window, parent, x, y);
    RETURN_LONG(cookie.sequence);
}

ZEND_FUNCTION(xcb_destroy_window)
{
    long window;
    xcb_void_cookie_t cookie;

    if (!check_connection() || zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
            "l", &window) == FAILURE) {
        return;
    }

    cookie = xcb_destroy_window(c, window);
    RETURN_LONG(cookie.sequence);
}

ZEND_FUNCTION(xcb_intern_atom)
{
    long only_if_exists, name_len;
    char *name;
    xcb_intern_atom_cookie_t cookie;
    xcb_intern_atom_reply_t *reply = NULL;

    if (!check_connection() || zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
            "ls", &only_if_exists, &name, &name_len) == FAILURE) {
        return;
    }

    cookie = xcb_intern_atom(c, only_if_exists, name_len, name);
    reply = xcb_intern_atom_reply(c, cookie, NULL);

    if (reply) {
        RETVAL_LONG(reply->atom);
        free(reply);
    }
}

ZEND_FUNCTION(xcb_get_atom_name)
{
    long atom;
    xcb_get_atom_name_cookie_t cookie;
    xcb_get_atom_name_reply_t *reply = NULL;

    if (!check_connection() || zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
            "l", &atom) == FAILURE) {
        return;
    }

    cookie = xcb_get_atom_name(c, atom);
    reply = xcb_get_atom_name_reply(c, cookie, NULL);

    if (reply) {
        RETVAL_STRING(xcb_get_atom_name_name(reply), 1);
        free(reply);
    }
}

ZEND_FUNCTION(xcb_get_property)
{
    long _delete, window, property, type, long_offset, long_length;
    xcb_get_property_cookie_t cookie;
    xcb_get_property_reply_t *reply = NULL;

    if (!check_connection() || zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
            "blllll", &_delete, &window, &property, &type, &long_offset,
            &long_length) == FAILURE) {
        return;
    }

    cookie = xcb_get_property(c, _delete, window, property, type, long_offset,
                              long_length);
    reply = xcb_get_property_reply(c, cookie, NULL);

    if (reply) {
        array_init(return_value);
        xcb_get_property_reply_t_to_assoc(reply, return_value);
        free(reply);
    }

}

ZEND_FUNCTION(xcb_change_property)
{
    long mode, window, property, type, format, n;
    zval *zdata;
    xcb_void_cookie_t cookie;

    if (!check_connection() || zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
            "lllllz", &mode, &window, &property, &type, &format,
            &zdata) == FAILURE) {
        return;
    }

    if (Z_TYPE_P(zdata) == IS_STRING) {
        char *s = Z_STRVAL_P(zdata);

        cookie = xcb_change_property(c, mode, window, property, type, 8,
                                     strlen(s), s);
        RETURN_LONG(cookie.sequence);
    }
    else if (Z_TYPE_P(zdata) == IS_LONG) {
        long i = Z_LVAL_P(zdata);

        cookie = xcb_change_property(c, mode, window, property, type, format, 1, &i);
        RETURN_LONG(cookie.sequence);
    }
    else if (Z_TYPE_P(zdata) == IS_ARRAY) {
        n = zend_hash_num_elements(HASH_OF(zdata)) - 1;

        if (n >= 0) {
            if (format == 32) {
                uint32_t value_list[n];

                php_array_to_long_array(zdata, value_list);
                cookie = xcb_change_property(c, mode, window, property, type,
                                             format, n, &value_list);
                RETURN_LONG(cookie.sequence);
            }
            else {
                /* not yet implemented */
            }
        }
    }
}

ZEND_FUNCTION(xcb_get_geometry)
{
    long drawable;
    xcb_get_geometry_cookie_t cookie;
    xcb_get_geometry_reply_t *reply = NULL;

    if (!check_connection() || zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
            "l", &drawable) == FAILURE) {
        return;
    }

    cookie = xcb_get_geometry(c, drawable);
    reply = xcb_get_geometry_reply(c, cookie, NULL);

    if (reply) {
        array_init(return_value);
        xcb_get_geometry_reply_t_to_assoc(reply, return_value);
        free(reply);
    }
}

ZEND_FUNCTION(xcb_query_tree)
{
    long window;
    xcb_query_tree_cookie_t cookie;
    xcb_query_tree_reply_t *reply = NULL;
    zval *reply_rsrc;

    if (!check_connection() || zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
            "l", &window) == FAILURE) {
        return;
    }

    cookie = xcb_query_tree(c, window);
    reply = xcb_query_tree_reply(c, cookie, NULL);

    if (reply) {
        object_init_ex(return_value, xcb_reply_ce);
        ALLOC_INIT_ZVAL(reply_rsrc);
        ZEND_REGISTER_RESOURCE(reply_rsrc, reply, le_xcb_query_tree_reply);

        zend_update_property(xcb_reply_ce, return_value, "reply",
                             strlen("reply"), reply_rsrc TSRMLS_CC);
        add_property_long(return_value, "root", reply->root);
        add_property_long(return_value, "parent", reply->parent);
        add_property_long(return_value, "children_len", reply->children_len);
    }
}

ZEND_FUNCTION(xcb_query_tree_children)
{
    zval *obj, *rsrc;
    xcb_query_tree_reply_t *reply;
    xcb_window_t *children;
    long i;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
                              "o", &obj) == FAILURE) {
        return;
    }

    rsrc = zend_read_property(xcb_reply_ce, obj, "reply",
                            strlen("reply"), 0 TSRMLS_CC);

    ZEND_FETCH_RESOURCE(reply, xcb_query_tree_reply_t*, &rsrc, -1,
                        "xcb_query_tree_reply_t", le_xcb_query_tree_reply);

    array_init(return_value);
    children = xcb_query_tree_children(reply);

    for (i = 0; i < reply->children_len; i++) {
        add_next_index_long(return_value, children[i]);
    }
}

ZEND_FUNCTION(xcb_get_window_attributes)
{
    long window;
    xcb_get_window_attributes_cookie_t cookie;
    xcb_get_window_attributes_reply_t *reply;

    if (!check_connection() || zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
            "l", &window) == FAILURE) {
        return;
    }

    cookie = xcb_get_window_attributes(c, window);
    reply = xcb_get_window_attributes_reply(c, cookie, NULL);

    if (reply) {
        array_init(return_value);
        xcb_get_window_attributes_reply_t_to_assoc(reply, return_value);
        free(reply);
    }
}

ZEND_FUNCTION(xcb_change_save_set)
{
    long mode, window;
    xcb_void_cookie_t cookie;

    if (!check_connection() || zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
            "ll", &mode, &window) == FAILURE) {
        return;
    }

    cookie = xcb_change_save_set(c, mode, window);
    RETURN_LONG(cookie.sequence);
}

ZEND_FUNCTION(xcb_send_event)
{
    long propagate, destination, event_mask, event_length;
    char *event;
    xcb_void_cookie_t cookie;

    if (!check_connection() || zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
            "llls", &propagate, &destination, &event_mask, &event,
            &event_length) == FAILURE) {
        return;
    }

    cookie = xcb_send_event(c, propagate, destination, event_mask, event);
    RETURN_LONG(cookie.sequence);
}
