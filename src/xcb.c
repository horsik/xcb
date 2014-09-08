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
    ZEND_FE(xcb_wait_for_event, NULL)
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
    property_bool(save_unders);
    property_long(root_depth);
    property_long(allowed_depths_len);
}

STRUCT_TO_OBJECT(xcb_generic_event_t)
{
    property_long(response_type);
    property_long(sequence);
    property_long(full_sequence);
}

EVENT_TO_OBJECT(xcb_generic_error_t)
{
    property_long(error_code);
    property_long(resource_id);
    property_long(minor_code);
    property_long(major_code);
    property_long(full_sequence);
    EVENT_END
}

EVENT_TO_OBJECT(xcb_key_press_event_t)
{
    property_long(detail);
    property_long(time);
    property_long(root);
    property_long(event);
    property_long(child);
    property_long(root_x);
    property_long(root_y);
    property_long(event_x);
    property_long(event_y);
    property_long(state);
    property_bool(same_screen);
    EVENT_END
}

EVENT_TO_OBJECT(xcb_key_release_event_t)
{
    property_long(detail);
    property_long(time);
    property_long(root);
    property_long(event);
    property_long(child);
    property_long(root_x);
    property_long(root_y);
    property_long(event_x);
    property_long(event_y);
    property_long(state);
    property_bool(same_screen);
    EVENT_END
}

EVENT_TO_OBJECT(xcb_button_press_event_t)
{
    property_long(detail);
    property_long(time);
    property_long(root);
    property_long(event);
    property_long(child);
    property_long(root_x);
    property_long(root_y);
    property_long(event_x);
    property_long(event_y);
    property_long(state);
    property_bool(same_screen);
    EVENT_END
}

EVENT_TO_OBJECT(xcb_button_release_event_t)
{
    property_long(detail);
    property_long(time);
    property_long(root);
    property_long(event);
    property_long(child);
    property_long(root_x);
    property_long(root_y);
    property_long(event_x);
    property_long(event_y);
    property_long(state);
    property_bool(same_screen);
    EVENT_END
}

EVENT_TO_OBJECT(xcb_motion_notify_event_t)
{
    property_long(detail);
    property_long(time);
    property_long(root);
    property_long(event);
    property_long(child);
    property_long(root_x);
    property_long(root_y);
    property_long(event_x);
    property_long(event_y);
    property_long(state);
    property_bool(same_screen);
    EVENT_END
}

EVENT_TO_OBJECT(xcb_enter_notify_event_t)
{
    property_long(detail);
    property_long(time);
    property_long(root);
    property_long(event);
    property_long(child);
    property_long(root_x);
    property_long(root_y);
    property_long(event_x);
    property_long(event_y);
    property_long(state);
    property_long(mode);
    property_bool(same_screen_focus);
    EVENT_END
}

EVENT_TO_OBJECT(xcb_leave_notify_event_t)
{
    property_long(detail);
    property_long(time);
    property_long(root);
    property_long(event);
    property_long(child);
    property_long(root_x);
    property_long(root_y);
    property_long(event_x);
    property_long(event_y);
    property_long(state);
    property_long(mode);
    property_bool(same_screen_focus);
    EVENT_END
}

EVENT_TO_OBJECT(xcb_focus_in_event_t)
{
    property_long(detail);
    property_long(event);
    property_long(mode);
    EVENT_END
}

EVENT_TO_OBJECT(xcb_focus_out_event_t)
{
    property_long(detail);
    property_long(event);
    property_long(mode);
    EVENT_END
}

EVENT_TO_OBJECT(xcb_keymap_notify_event_t)
{
    int i;
    zval *keys;

    ALLOC_INIT_ZVAL(keys);
    array_init(keys);

    for (i = 0; i < 31; i++) {
        add_next_index_long(keys, src->keys[i]);
    }

    add_property_zval(dest, "keys", keys);
    EVENT_END
}

EVENT_TO_OBJECT(xcb_expose_event_t)
{
    property_long(window);
    property_long(x);
    property_long(y);
    property_long(width);
    property_long(height);
    property_long(count);
    EVENT_END
}

EVENT_TO_OBJECT(xcb_graphics_exposure_event_t)
{
    property_long(drawable);
    property_long(x);
    property_long(y);
    property_long(width);
    property_long(height);
    property_long(minor_opcode);
    property_long(count);
    property_long(major_opcode);
    EVENT_END
}

EVENT_TO_OBJECT(xcb_no_exposure_event_t)
{
    property_long(drawable);
    property_long(minor_opcode);
    property_long(major_opcode);
    EVENT_END
}

EVENT_TO_OBJECT(xcb_visibility_notify_event_t)
{
    property_long(window);
    property_long(state);
    EVENT_END
}

EVENT_TO_OBJECT(xcb_create_notify_event_t)
{
    property_long(parent);
    property_long(window);
    property_long(x);
    property_long(y);
    property_long(width);
    property_long(height);
    property_long(border_width);
    property_long(override_redirect);
    EVENT_END
}

EVENT_TO_OBJECT(xcb_destroy_notify_event_t)
{
    property_long(event);
    property_long(window);
    EVENT_END
}

EVENT_TO_OBJECT(xcb_unmap_notify_event_t)
{
    property_long(event);
    property_long(window);
    property_long(from_configure);
    EVENT_END
}

EVENT_TO_OBJECT(xcb_map_notify_event_t)
{
    property_long(event);
    property_long(window);
    property_long(override_redirect);
    EVENT_END
}

EVENT_TO_OBJECT(xcb_map_request_event_t)
{
    property_long(parent);
    property_long(window);
    EVENT_END
}

EVENT_TO_OBJECT(xcb_reparent_notify_event_t)
{
    property_long(event);
    property_long(window);
    property_long(parent);
    property_long(x);
    property_long(y);
    property_long(override_redirect);
    EVENT_END
}

EVENT_TO_OBJECT(xcb_configure_notify_event_t)
{
    property_long(event);
    property_long(window);
    property_long(above_sibling);
    property_long(x);
    property_long(y);
    property_long(width);
    property_long(height);
    property_long(border_width);
    property_long(override_redirect);
    EVENT_END
}

EVENT_TO_OBJECT(xcb_gravity_notify_event_t)
{
    property_long(event);
    property_long(window);
    property_long(x);
    property_long(y);
    EVENT_END
}

EVENT_TO_OBJECT(xcb_resize_request_event_t)
{
    property_long(window);
    property_long(width);
    property_long(height);
    EVENT_END
}

EVENT_TO_OBJECT(xcb_configure_request_event_t)
{
    property_long(stack_mode);
    property_long(parent);
    property_long(window);
    property_long(sibling);
    property_long(x);
    property_long(y);
    property_long(width);
    property_long(height);
    property_long(border_width);
    property_long(value_mask);
    EVENT_END
}

EVENT_TO_OBJECT(xcb_circulate_notify_event_t)
{
    property_long(event);
    property_long(window);
    property_long(place);
    EVENT_END
}

EVENT_TO_OBJECT(xcb_circulate_request_event_t)
{
    property_long(event);
    property_long(window);
    property_long(place);
    EVENT_END
}

EVENT_TO_OBJECT(xcb_property_notify_event_t)
{
    property_long(window);
    property_long(atom);
    property_long(time);
    property_long(state);
    EVENT_END
}

EVENT_TO_OBJECT(xcb_selection_clear_event_t)
{
    property_long(time);
    property_long(owner);
    property_long(selection);
    EVENT_END
}

EVENT_TO_OBJECT(xcb_selection_request_event_t)
{
    property_long(time);
    property_long(owner);
    property_long(requestor);
    property_long(selection);
    property_long(target);
    property_long(property);
    EVENT_END
}

EVENT_TO_OBJECT(xcb_selection_notify_event_t)
{
    property_long(time);
    property_long(requestor);
    property_long(selection);
    property_long(target);
    property_long(property);
    EVENT_END
}

EVENT_TO_OBJECT(xcb_colormap_notify_event_t)
{
    property_long(window);
    property_long(colormap);
    property_long(_new);
    property_long(state);
    EVENT_END
}

EVENT_TO_OBJECT(xcb_mapping_notify_event_t)
{
    property_long(request);
    property_long(first_keycode);
    property_long(count);
    EVENT_END
}

EVENT_TO_OBJECT(xcb_client_message_event_t)
{
    int i;
    zval *data;

    property_long(format);
    property_long(window);
    property_long(type);

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

    add_property_zval(dest, "data", data);
    EVENT_END
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

ZEND_FUNCTION(xcb_wait_for_event)
{
    int fd;
    fd_set rfds;
    long timeout = 0;
    struct timeval tv = {0, 0};
    xcb_generic_event_t *e;

    if (c == NULL || zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "|l",
                              &timeout) == FAILURE) {
        RETURN_BOOL(0);
    }

    if (timeout > 0) {
        fd = xcb_get_file_descriptor(c);
        FD_ZERO(&rfds);
        FD_SET(fd, &rfds);

        tv.tv_sec = timeout;
        select(fd + 1, &rfds, NULL, NULL, &tv);

        if (!(e = xcb_poll_for_queued_event(c))) {
            RETURN_BOOL(0);
        }
    }
    else {
        if (!(e = xcb_wait_for_event(c))) {
            zend_error(E_WARNING, "Cannot read event: broken pipe");
        }
    }

    object_init(return_value);

    switch(e->response_type) {
        case XCB_KEY_PRESS:
            xcb_key_press_event_t_to_object(e, return_value);
            break;
        case XCB_KEY_RELEASE:
            xcb_key_release_event_t_to_object(e, return_value);
            break;
        case XCB_BUTTON_PRESS:
            xcb_button_press_event_t_to_object(e, return_value);
            break;
        case XCB_BUTTON_RELEASE:
            xcb_button_release_event_t_to_object(e, return_value);
            break;
        case XCB_MOTION_NOTIFY:
            xcb_motion_notify_event_t_to_object(e, return_value);
            break;
        case XCB_ENTER_NOTIFY:
            xcb_enter_notify_event_t_to_object(e, return_value);
            break;
        case XCB_LEAVE_NOTIFY:
            xcb_leave_notify_event_t_to_object(e, return_value);
            break;
        case XCB_FOCUS_IN:
            xcb_focus_in_event_t_to_object(e, return_value);
            break;
        case XCB_FOCUS_OUT:
            xcb_focus_out_event_t_to_object(e, return_value);
            break;
        case XCB_KEYMAP_NOTIFY:
            xcb_keymap_notify_event_t_to_object(e, return_value);
            break;
        case XCB_EXPOSE:
            xcb_expose_event_t_to_object(e, return_value);
            break;
        case XCB_GRAPHICS_EXPOSURE:
            xcb_graphics_exposure_event_t_to_object(e, return_value);
            break;
        case XCB_NO_EXPOSURE:
            xcb_no_exposure_event_t_to_object(e, return_value);
            break;
        case XCB_VISIBILITY_NOTIFY:
            xcb_visibility_notify_event_t_to_object(e, return_value);
            break;
        case XCB_CREATE_NOTIFY:
            xcb_create_notify_event_t_to_object(e, return_value);
            break;
        case XCB_DESTROY_NOTIFY:
            xcb_destroy_notify_event_t_to_object(e, return_value);
            break;
        case XCB_UNMAP_NOTIFY:
            xcb_unmap_notify_event_t_to_object(e, return_value);
            break;
        case XCB_MAP_NOTIFY:
            xcb_map_notify_event_t_to_object(e, return_value);
            break;
        case XCB_MAP_REQUEST:
            xcb_map_request_event_t_to_object(e, return_value);
            break;
        case XCB_REPARENT_NOTIFY:
            xcb_reparent_notify_event_t_to_object(e, return_value);
            break;
        case XCB_CONFIGURE_NOTIFY:
            xcb_configure_notify_event_t_to_object(e, return_value);
            break;
        case XCB_GRAVITY_NOTIFY:
            xcb_gravity_notify_event_t_to_object(e, return_value);
            break;
        case XCB_RESIZE_REQUEST:
            xcb_resize_request_event_t_to_object(e, return_value);
            break;
        case XCB_CONFIGURE_REQUEST:
            xcb_configure_request_event_t_to_object(e, return_value);
            break;
        case XCB_CIRCULATE_NOTIFY:
            xcb_circulate_notify_event_t_to_object(e, return_value);
            break;
        case XCB_CIRCULATE_REQUEST:
            xcb_circulate_request_event_t_to_object(e, return_value);
            break;
        case XCB_PROPERTY_NOTIFY:
            xcb_property_notify_event_t_to_object(e, return_value);
            break;
        case XCB_SELECTION_CLEAR:
            xcb_selection_clear_event_t_to_object(e, return_value);
            break;
        case XCB_SELECTION_REQUEST:
            xcb_selection_request_event_t_to_object(e, return_value);
            break;
        case XCB_SELECTION_NOTIFY:
            xcb_selection_notify_event_t_to_object(e, return_value);
            break;
        case XCB_COLORMAP_NOTIFY:
            xcb_colormap_notify_event_t_to_object(e, return_value);
            break;
        case XCB_MAPPING_NOTIFY:
            xcb_mapping_notify_event_t_to_object(e, return_value);
            break;
        case XCB_CLIENT_MESSAGE:
            xcb_client_message_event_t_to_object(e, return_value);
            break;
        case XCB_NONE:
            xcb_generic_error_t_to_object(e, return_value);
            break;
        default:
            xcb_generic_event_t_to_object(e, return_value);
    }

    free(e);
}
