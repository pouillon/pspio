#!/bin/sh
#
# Copyright (C) 2011-2016 Yann Pouillon <notifications@materialsevolution.es>
#
# This file is part of Libpspio.
#
# Libpspio is free software: you can redistribute it and/or modify it under the
# terms of the GNU Lesser General Public License as published by the Free
# Software Foundation, version 3 of the License, or (at your option) any later
# version.
#
# Libpspio is distributed in the hope that it will be useful, but WITHOUT ANY
# WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
# A PARTICULAR PURPOSE.  See the GNU Lesser General Public License for more
# details.
#
# You should have received a copy of the GNU Lesser General Public License
# along with Libpspio.  If not, see <http://www.gnu.org/licenses/> or write to
# the Free Software Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA
# 02110-1301  USA.

#
# IMPORTANT NOTE:
#
#   For maintainer use only!
#
#   PLEASE DO NOT EDIT THIS FILE, AS IT COULD CAUSE A SERIOUS LOSS OF DATA.
#   *** YOU HAVE BEEN WARNED! ***
#

# Check that we are in the right directory
if test ! -s "./configure.ac" -o ! -s "src/pspiof.F90"; then
  echo "wipeout: Cowardly refusing to remove something from here!"
  exit 1
fi

# Make sure the whole directory tree is writable
chmod -R u+w .

# Remove temporary directories and files
echo "Removing temporary directories and files..."
find . -depth -name 'tmp*' -exec rm -rf {} \;
find . -depth -name '*.tmp' -exec rm -rf {} \;
echo "done."

# Remove autotools files
echo "Removing autotools files..."
rm -f core config.log config.status stamp-h1 config.h config.h.in*
rm -rf aclocal.m4 autom4te.cache configure confstat*
(cd config/gnu && rm -f compile config.guess config.sub depcomp install-sh ltmain.sh missing test-driver)
(cd config/m4 && rm -f libtool.m4 ltoptions.m4 ltsugar.m4 ltversion.m4 lt~obsolete.m4)
echo "done."

# Remove Makefiles and machine-generated files
echo "Removing files produced by the configure script..."
rm -f libtool
find . -name Makefile -exec rm {} \;
find . -name Makefile.in -exec rm {} \;
find . -name '*.log' -exec rm {} \;
find . -name '*.trs' -exec rm {} \;
rm -rf libpspio-fortran-*.tar.gz
echo "done."

# Remove object files, libraries and programs
echo "Removing object files, libraries and programs..."
find . -depth -name '.deps' -exec rm -rf {} \;
find . -depth -name '.libs' -exec rm -rf {} \;
find . -name '*.la' -exec rm {} \;
find . -name '*.lo' -exec rm {} \;
find . -name '*.a' -exec rm {} \;
find . -name '*.o' -exec rm {} \;
echo "done."

# Remove test programs
rm -f src/test_fortran src/test_fortran_error
