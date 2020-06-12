#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT(deadbeef-dr-meter, 0.1, mrdakeryas@gmail.com)
AC_CONFIG_AUX_DIR([build-aux])
AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_SRCDIR([dr_meter/src/block_analyser.c])
AC_CONFIG_HEADERS([config.h])

# Checks for programs.
AC_PROG_CC_C99
AM_PROG_AR
AC_PROG_INSTALL

# Use automake to build shared libraries
AM_INIT_AUTOMAKE([-Wall foreign subdir-objects])
LT_INIT([shared disable-static])

# Checks for libraries.
AC_CHECK_LIB([pthread], [pthread_create])
PKG_CHECK_MODULES(GTK2, gtk+-2.0 >= 2.12, FOUND_GTK2=yes, FOUND_GTK2=no)
PKG_CHECK_MODULES(GTK3, gtk+-3.0 >= 3.0, FOUND_GTK3=yes, FOUND_GTK3=no)

# Define HAS_GTK2 and HAS_GTK3 in Makefiles depending on libraries found
AM_CONDITIONAL(HAS_GTK2, test "x$FOUND_GTK2" = "xyes")
AM_CONDITIONAL(HAS_GTK3, test "x$FOUND_GTK3" = "xyes")

# Checks for header files.
AC_CHECK_HEADERS([deadbeef/deadbeef.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_TYPE_INT16_T
AC_TYPE_INT32_T
AC_TYPE_SIZE_T

AC_CONFIG_FILES([Makefile
                 dr_meter/Makefile
                 dr_plugin/Makefile
                 dr_plugin_gui/Makefile])
AC_OUTPUT
