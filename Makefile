# Copyright (C) 2016  R.Tech
#
# This file is part of GridPro2Gmsh.
#
# GridPro2Gmsh is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# GridPro2Gmsh is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with GridPro2Gmsh.  If not, see <http://www.gnu.org/licenses/>.
#
# Author : Martin Spel (R.Tech)


CFLAGS = -g
LDFLAGS = -g
LINKOPTS =
LOCAL_LIBRARIES =

all:: gridpro2gmsh

.cpp.o:
	$(CXX) -c $(CFLAGS) $<

util.o:	util.c util.h
	$(CC) -c $(CFLAGS) util.c

gridpro2gmsh.o:	gridpro2gmsh.cpp util.h
	$(CXX) -c $(CFLAGS) gridpro2gmsh.cpp

OBJECTS = HexCell.o  Patch.o Node.o Block.o gridpro2gmsh.o  util.o Triplet.o Face.o RTechTimer.o PeriodicStats.o


gridpro2gmsh:: $(OBJECTS) $(DEPS)
	rm -f $@
	$(CXX) -o $@ $(LINKOPTS) $(OBJECTS) \
          $(DEPS) $(LOCAL_LIBRARIES) $(LDFLAGS) $(SYSLIBS) $(SYSLAST_LIBRARIES)

clean::
	rm -f *.o *.a gridpro2gmsh


tar::
	tar cvfz GridPro2gmsh.v1_00_00.tgz Makefile *.h *.hpp *.cpp *.c LICENSE
