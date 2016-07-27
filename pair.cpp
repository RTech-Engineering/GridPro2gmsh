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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <utility>
#include <map>
#include <string>
using namespace std;

main()
{

  pair<int, string> a(1, "test 1");
  pair<int, string> b(2, "test 2");
  pair<int, string> c(1, "test 3");

  map<int, string> myMap;

  myMap.insert(a);
  myMap.insert(b);
  myMap.insert(c);


  printf("Count 1: %d\n", myMap.count(1));
  printf("Count 2: %d\n", myMap.count(2));
  printf("Count 3: %d\n", myMap.count(3));

  map<int, string>::iterator it = myMap.find(1);
  printf("Object with int=1: %s\n", (*it).second.c_str());
}
