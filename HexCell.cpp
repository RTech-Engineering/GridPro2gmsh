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


#include "HexCell.hpp"
#include "Node.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

HexCell::HexCell(Node *n1, Node *n2, Node *n3, Node *n4, Node *n5, Node *n6, Node *n7, Node *n8)
{ nodes[0] = n1;
  nodes[1] = n2;
  nodes[2] = n3;
  nodes[3] = n4;
  nodes[4] = n5;
  nodes[5] = n6;
  nodes[6] = n7;
  nodes[7] = n8;
}


HexCell::~HexCell()
{
}


#define V1 0
#define V2 1
#define V3 2
#define V4 3
#define V5 4
#define V6 5
#define V7 6
#define V8 7

void HexCell::ComputeIndirect1()
{ 
  int iMin, idMin;
  iMin = 0;
  id = nodes[0]->GetId();
  idMin = id;
  // find smallest global vertex for current hex
  for (int i=1; i<8; i++)
  { id=nodes[i]->GetId();
    if (id < idMin) 
    { iMin = i;
      idMin = id;
    }
  }

  switch (iMin)
  { case 0:
     for (int i=0; i<8; i++)
     { nodesIndirect1[i] = nodes[i];
     }
   break;
   case 1:
     nodesIndirect1[V1] = nodes[V2];
     nodesIndirect1[V2] = nodes[V1];
     nodesIndirect1[V3] = nodes[V5];
     nodesIndirect1[V4] = nodes[V6];
     nodesIndirect1[V5] = nodes[V3];
     nodesIndirect1[V6] = nodes[V4];
     nodesIndirect1[V7] = nodes[V8];
     nodesIndirect1[V8] = nodes[V7];
   break;
   case 2:
     nodesIndirect1[V1] = nodes[V3];
     nodesIndirect1[V2] = nodes[V2];
     nodesIndirect1[V3] = nodes[V6];
     nodesIndirect1[V4] = nodes[V7];
     nodesIndirect1[V5] = nodes[V4];
     nodesIndirect1[V6] = nodes[V1];
     nodesIndirect1[V7] = nodes[V5];
     nodesIndirect1[V8] = nodes[V8];
   break;
   case 3:
     nodesIndirect1[V1] = nodes[V4];
     nodesIndirect1[V2] = nodes[V1];
     nodesIndirect1[V3] = nodes[V2];
     nodesIndirect1[V4] = nodes[V3];
     nodesIndirect1[V5] = nodes[V8];
     nodesIndirect1[V6] = nodes[V5];
     nodesIndirect1[V7] = nodes[V6];
     nodesIndirect1[V8] = nodes[V7];
   break;
   case 4:
     nodesIndirect1[V1] = nodes[V5];
     nodesIndirect1[V2] = nodes[V1];
     nodesIndirect1[V3] = nodes[V4];
     nodesIndirect1[V4] = nodes[V8];
     nodesIndirect1[V5] = nodes[V6];
     nodesIndirect1[V6] = nodes[V2];
     nodesIndirect1[V7] = nodes[V3];
     nodesIndirect1[V8] = nodes[V7];
   break;
   case 5:
     nodesIndirect1[V1] = nodes[V6];
     nodesIndirect1[V2] = nodes[V2];
     nodesIndirect1[V3] = nodes[V1];
     nodesIndirect1[V4] = nodes[V5];
     nodesIndirect1[V5] = nodes[V7];
     nodesIndirect1[V6] = nodes[V3];
     nodesIndirect1[V7] = nodes[V4];
     nodesIndirect1[V8] = nodes[V8];
   break;
   case 6:
     nodesIndirect1[V1] = nodes[V7];
     nodesIndirect1[V2] = nodes[V3];
     nodesIndirect1[V3] = nodes[V2];
     nodesIndirect1[V4] = nodes[V6];
     nodesIndirect1[V5] = nodes[V8];
     nodesIndirect1[V6] = nodes[V4];
     nodesIndirect1[V7] = nodes[V1];
     nodesIndirect1[V8] = nodes[V5];
   break;
   case 7:
     nodesIndirect1[V1] = nodes[V8];
     nodesIndirect1[V2] = nodes[V4];
     nodesIndirect1[V3] = nodes[V3];
     nodesIndirect1[V4] = nodes[V7];
     nodesIndirect1[V5] = nodes[V5];
     nodesIndirect1[V6] = nodes[V1];
     nodesIndirect1[V7] = nodes[V2];
     nodesIndirect1[V8] = nodes[V6];
   break;
  }
}

bool HexCell::Less(int i1, int i2, int i3, int i4)
{ i1--; i2--; i3--, i4--;
  int j1 = nodesIndirect1[i1]->GetId();
  int j2 = nodesIndirect1[i2]->GetId();
  if (j2<j1) 
  { j1 = j2;
  }
  int k1 = nodesIndirect1[i3]->GetId();
  int k2 = nodesIndirect1[i4]->GetId();
  if (k2<k1) 
  { k1 = k2;
  }
  return j1<k1;
}

void HexCell::ComputeDiagonals()
{ inCode = 0;
  nrDiagonals = 0;
  
  bool b;
  // check if the diagonal of face (2,3,7,6) goes is (2-7) or (7-2)
  b = Less(2, 7, 3, 6);
  if (b)
  { inCode += 4;
    nrDiagonals++;
  }
  // check if the diagonal of face (3,7,8,4) goes is (4-7) or (7-4)
  b = Less(4, 7, 3, 8);
  if (b)
  { inCode += 2;
    nrDiagonals++;
  }
  // check if the diagonal of face (5,6,7,8) goes is (5-7) or (7-5)
  b = Less(5, 7, 6, 8);
  if (b)
  { inCode += 1;
    nrDiagonals++;
  }
}


void HexCell::WriteTet(FILE *f, int i1, int i2, int i3, int i4, int &id)
{ i1--; i2--; i3--; i4--;
  fprintf(f, "%d 4 2 0 1 %d %d %d %d\n", id, nodesIndirect2[i1]->GetId()+1, nodesIndirect2[i2]->GetId()+1, nodesIndirect2[i3]->GetId()+1, nodesIndirect2[i4]->GetId()+1);
  id++;
}

void HexCell::Rotate()
{
  for (int i=0; i<8; i++)
  { nodesIndirect2[i] = nodesIndirect1[i];
  }
  if (inCode==1 || inCode==6) 
  { // rotation 120
    nodesIndirect2[4-1] = nodesIndirect1[2-1];
    nodesIndirect2[2-1] = nodesIndirect1[5-1];
    nodesIndirect2[5-1] = nodesIndirect1[4-1];
    nodesIndirect2[6-1] = nodesIndirect1[8-1];
    nodesIndirect2[8-1] = nodesIndirect1[3-1];
    nodesIndirect2[3-1] = nodesIndirect1[6-1];
  }
  if (inCode==2 || inCode==5)
  { // rotation 240
    nodesIndirect2[2-1] = nodesIndirect1[4-1];
    nodesIndirect2[4-1] = nodesIndirect1[5-1];
    nodesIndirect2[5-1] = nodesIndirect1[2-1];
    nodesIndirect2[6-1] = nodesIndirect1[3-1];
    nodesIndirect2[3-1] = nodesIndirect1[8-1];
    nodesIndirect2[8-1] = nodesIndirect1[6-1];
  }
}

int HexCell::WriteAsTet(FILE *f, int &id)
{ 
  ComputeIndirect1();
  // compute number of diagonals and inCode
  ComputeDiagonals();
  // rotation
  Rotate();

  // subdivide and write

  if (nrDiagonals == 0)
  { WriteTet(f, 1, 2, 3, 6, id);
    WriteTet(f, 1, 3, 8, 6, id);
    WriteTet(f, 1, 3, 4, 8, id);
    WriteTet(f, 1, 6, 8, 5, id);
    WriteTet(f, 3, 8, 6, 7, id);
    return 5;
  }
  if (nrDiagonals == 1)
  { WriteTet(f, 1, 6, 8, 5, id);
    WriteTet(f, 1, 2, 8, 6, id);
    WriteTet(f, 2, 7, 8, 6, id);
    WriteTet(f, 1, 8, 3, 4, id);
    WriteTet(f, 1, 8, 2, 3, id);
    WriteTet(f, 2, 8, 7, 3, id);
  }
  if (nrDiagonals == 2)
  { WriteTet(f, 1, 5, 6, 7, id);
    WriteTet(f, 1, 4, 8, 7, id);
    WriteTet(f, 1, 8, 5, 7, id);
    WriteTet(f, 1, 2, 3, 6, id);
    WriteTet(f, 1, 4, 7, 3, id);
    WriteTet(f, 1, 7, 6, 3, id);
  }
  if (nrDiagonals == 3)
  { WriteTet(f, 1, 3, 4, 7, id);
    WriteTet(f, 1, 4, 8, 7, id);
    WriteTet(f, 1, 8, 5, 7, id);
    WriteTet(f, 1, 6, 7, 5, id);
    WriteTet(f, 2, 6, 7, 1, id);
    WriteTet(f, 2, 7, 3, 1, id);
  }
  return 6;
}

int HexCell::CountTets()
{ 
  // compute indirection
  ComputeIndirect1();
  // compute number of diagonals and inCode
  ComputeDiagonals();
  // rotation
  Rotate();

  // subdivide and write
  if (nrDiagonals == 0)
  {
    return 5;
  }
  return 6;
}
