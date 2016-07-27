/*
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
*/

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
void s_skip_line(FILE *fp);
int s_skipblanks(FILE *fp);
int s_search_in_file_ncs(FILE *f, char *s);
int s_search_in_file(FILE *f, char *s);

#ifdef __cplusplus
}
#endif

