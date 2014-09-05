PHP_ARG_ENABLE(xcb, The X protocol C-language Binding (XCB) wrapper,
[ --enable-xcb   Enable XCB support])

if test "$PHP_XCB" = "yes"; then
    AC_SEARCH_LIBS([xcb_connect], [xcb], [
        PHP_ADD_LIBRARY_WITH_PATH(xcb, /usr/lib, XCB_SHARED_LIBADD)
        AC_DEFINE(HAVE_LIBXCB, 1, [ ])
    ], [ AC_MSG_ERROR([xcb library not found]) ])

    PHP_SUBST(XCB_SHARED_LIBADD)
    AC_SEARCH_LIBS([xcb_key_symbols_alloc], [xcb-keysyms], [
        PHP_ADD_LIBRARY_WITH_PATH(xcb-keysyms, /usr/lib, XCB_SHARED_LIBADD)
        AC_DEFINE(HAVE_LIBXCB_KEYSYMS, 1, [ ])
    ], [ AC_MSG_ERROR([xcb-keysyms library not found]) ])

    PHP_ADD_INCLUDE(/usr/include)
    PHP_NEW_EXTENSION(xcb, src/xcb.c, $ext_shared)
    PHP_SUBST(XCB_SHARED_LIBADD)
fi
