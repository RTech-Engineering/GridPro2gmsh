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

#include <list>
using namespace std;


main()
{ list<int> ll;
  ll.push_back(2);
  ll.push_back(3);
  ll.push_back(6);
  ll.push_back(8);
  ll.push_back(10);
  list<int>::iterator i,j;
  for (i=ll.begin(); i!=ll.end(); i++)
  { printf("%d\n", *i);
    j=i; j++;
    for (; j!=ll.end(); j++)
    { if (*j==8 && *i==6)
      { ll.erase(j);
        break;
      }
    }
  }
}
