# Copyright (c) 2013-2014 Martin Donath, voola GmbH <martin.donath@voola.de>

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
# Subdirectories
# -----------------------------------------------------------------------------

SUBDIRS = src

# -----------------------------------------------------------------------------
# Custom build rules
# -----------------------------------------------------------------------------

# Purge development files
purge: clean
	find . -depth -name '.deps*' -exec rm -rf '{}' \;
	find . -depth -name '.libs*' -exec rm -rf '{}' \;
	find . -name 'Makefile' -type f -delete
	find . -name 'Makefile.in' -type f -delete
	find . -name '.dirstamp' -type f -delete
	find . -name '.DS_Store' -type f -delete
	rm -f  aclocal.m4
	rm -rf autom4te.cache
	rm -f  autoscan.log
	rm -f  compile
	rm -f  configure
	rm -f  configure.scan
	rm -f  config.*
	rm -f  depcomp
	rm -f  install-sh
	rm -f  libtool
	rm -f  ltmain.sh
	rm -rf m4
	rm -f  missing
	rm -f  stamp-h1