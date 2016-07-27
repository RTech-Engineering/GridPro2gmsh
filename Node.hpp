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


#ifndef NODE_HPP
#define NODE_HPP

#include <string>
#include <map>
#include <stdio.h>
using namespace std;
class PeriodicStats;

class Node 
{ friend class Face;
  friend class PeriodicStats;
  private:
    double x, y, z;
    int id;
    Node *equivalent;
    map<int, Node*> periodicNeighbor;
    bool isBoundaryNode;
    map <int, short> periodicSide;  // which side of the periodic surface: -1, 0 (matching) or 1
  public:
    Node(double x=0, double y=0, double z=0, bool setBoundaryNode=false);
    ~Node();
    void SetEquivalent(Node *n2, bool debug=false);
    void PrintEquivalent();
    void PrintDebug(FILE *f = stdout);
    void AnalysePeriodic(PeriodicStats *stats, int BC);
    Node* GetEquivalent() {return equivalent;};
    void Print(FILE *f = stdout);
    void PrintTecplot(FILE *f = stdout);
    double Distance(Node *n2);
    void SetId(int setId);
    int GetId();
    int GetRealId() {return id;};
    void SetPeriodicNeighbor(int BC, Node *n2);
    int GetPeriodicId(int BC);
    void SetPeriodicSide(int BC, short n);
    short GetPeriodicSide(int BC);
};

#include "PeriodicStats.hpp"
#endif

