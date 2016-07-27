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


#ifndef HEXCELL_HPP
#define HEXCELL_HPP

#include "Node.hpp"
#include <vector>
#include <list>

using namespace std;
class HexCell
{ 
  private:
    int id;
    Node *nodes[8]; 
    Node *nodesIndirect1[8]; 
    Node *nodesIndirect2[8]; 
    char inCode;
    short nrDiagonals;
    
  public:
    HexCell(Node *n1, Node *n2, Node *n3, Node *n4, Node *n5, Node *n6, Node *n7, Node *n8);
    ~HexCell();
    int WriteAsTet(FILE *f, int &id);
    int CountTets();
  private:
    bool Less(int i1, int i2, int i3, int i4);
    void WriteTet(FILE *f, int i1, int i2, int i3, int i4, int &id);
    void ComputeIndirect1();
    void ComputeDiagonals();
    void Rotate();
};
#endif

