AC_INIT([stringc], [1.0], [bug-report@address])
AC_CONFIG_MACRO_DIR([m4])
AM_INIT_AUTOMAKE([foreign -Wall -Werror subdir-objects])
AC_CANONICAL_HOST

AC_ENABLE_SHARED
AC_ENABLE_STATIC


AC_PROG_CC
AM_PROG_CC_C_O

# CC_CHECK_CFLAGS_APPEND([-std=c11])
#CC_CHECK_CFLAGS_APPEND([-Wall])
#CC_CHECK_CFLAGS_APPEND([-Wextra])
#CC_CHECK_CFLAGS_APPEND([-Wno-unused-parameter])
#CC_CHECK_CFLAGS_APPEND([-pedantic])
#CC_CHECK_CFLAGS_APPEND([-g])


# fix last automake error ~1.13.4
m4_pattern_allow([AM_PROG_AR])
AM_PROG_AR

AC_PROG_LIBTOOL
LT_INIT

AC_CONFIG_FILES([Makefile])
AC_OUTPUT
