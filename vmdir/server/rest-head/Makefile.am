
noinst_LTLIBRARIES = librest-head.la

librest_head_la_SOURCES = \
    libmain.c

if REST_ENABLED

librest_head_la_SOURCES += \
    accesstoken.c \
    auth.c \
    decode.c \
    encode.c \
    globals.c \
    httperror.c \
    ldapapi.c \
    operation.c \
    param.c \
    resource.c \
    result.c

endif

librest_head_la_CPPFLAGS = \
    -I$(top_srcdir)/include \
    -I$(top_srcdir)/include/public \
    -I$(top_srcdir)/thirdparty/openldap/include \
    -I$(top_srcdir)/thirdparty/openldap/libraries/liblber \
    -I$(top_srcdir)/thirdparty/openldap/libraries/libldap \
    -I$(top_srcdir)/server/include \
    @LW_INCLUDES@ \
    @OPENSSL_INCLUDES@ \
    @JANSSON_INCLUDES@ \
    @COPENAPI_INCLUDES@ \
    @CRESTENGINE_INCLUDES@ \
    @OIDC_INCLUDES@

librest_head_la_LDFLAGS = \
    -static
