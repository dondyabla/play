# Copyright (c) 2013 Martin Donath, voola GmbH <martin.donath@voola.de>

# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to
# deal in the Software without restriction, including without limitation the
# rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
# sell copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:

# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.

# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
# FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
# IN THE SOFTWARE.

# -----------------------------------------------------------------------------
# Prologue
# -----------------------------------------------------------------------------

AC_PREREQ(2.61)

AC_INIT(play, 0.1)
AM_INIT_AUTOMAKE([subdir-objects foreign])

AC_PROG_CC
AC_PROG_CPP

AC_CONFIG_SRCDIR([src/play.c])
AC_CONFIG_HEADER([config.h])
AC_CONFIG_MACRO_DIR([m4])

# -----------------------------------------------------------------------------
# Dependencies
# -----------------------------------------------------------------------------

# Checks for programs
AC_PROG_INSTALL

# -----------------------------------------------------------------------------
# Arguments
# -----------------------------------------------------------------------------

# Disable optimized compilation
AC_ARG_ENABLE([optimized],
	[AS_HELP_STRING([--disable-optimized],
		[no optimizations @<:@default=enabled@:>@])],
	[OPTIMIZATIONS="-g"],
	[OPTIMIZATIONS="-O3 -DNDEBUG" \
	 OPTIMIZATIONS+=" -fno-exceptions" \
	 OPTIMIZATIONS+=" -fno-unwind-tables" \
	 OPTIMIZATIONS+=" -fno-asynchronous-unwind-tables" \
	 OPTIMIZATIONS+=" -fno-stack-protector" \
	 OPTIMIZATIONS+=" -fomit-frame-pointer"])
AC_SUBST([OPTIMIZATIONS])

# -----------------------------------------------------------------------------
# Header files and functions
# -----------------------------------------------------------------------------

# Check for header files
AC_HEADER_STDC
AC_CHECK_HEADERS([ \
	errno.h \
	getopt.h \
	netinet/in.h \
	stdint.h \
	stdio.h \
	stdlib.h \
	string.h \
	sys/socket.h])

# Check for typedefs, structures, and compiler characteristics
AC_TYPE_INT32_T
AC_TYPE_SIZE_T
AC_TYPE_UINT8_T

# Check for library functions and config files
AC_FUNC_FORK
AC_CHECK_FUNCS([socket strerror])
AC_CONFIG_FILES([ \
	Makefile \
	src/Makefile])

# -----------------------------------------------------------------------------
# Compiler and linker flags
# -----------------------------------------------------------------------------

# Compiler and linker flags
CFLAGS="-std=c99"
CPPFLAGS="-g -Wall"

AC_OUTPUT