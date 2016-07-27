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

#include <vector>
using namespace std;


main()
{ vector<int> ll, pp;
  ll.push_back(2);
  ll.push_back(3);
  ll.push_back(6);
  for (int i=0; i<ll.size(); i++)
  { printf("%d\n", ll[i]);
  }
  for (int i=0; i<pp.size(); i++)
  { printf("%d\n", pp[i]);
  }
}
