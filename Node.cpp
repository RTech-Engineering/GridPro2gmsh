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

#include "Node.hpp"
#include <math.h>


Node::Node(double setX, double setY, double setZ, bool setBoundaryNode)
{ x=setX;
  y=setY;
  z=setZ;
  equivalent=NULL;
  id = -1;
  isBoundaryNode=setBoundaryNode;
}

Node::~Node()
{
}

// set my equivalence to node n2, unless it is already set
void Node::SetEquivalent(Node *n2, bool debug)
{ 
  while (n2->GetEquivalent()) n2=n2->GetEquivalent();

  Node *n1=this;
  while (n1->GetEquivalent()) n1=n1->GetEquivalent();

  if (n1->id > n2->id)
    n1->equivalent = n2;
  else
    n2->equivalent = n1;
}


void Node::PrintEquivalent()
{
 if (equivalent != NULL)
 {  printf("Equivalent:\n");
    Print();
    equivalent->Print();
 }
}

void Node::PrintTecplot(FILE *f)
{ 
 fprintf(f, "%1.13lg %1.13lg %1.13lg\n", x, y, z);
}

void Node::AnalysePeriodic(PeriodicStats *stats, int BC)
{
  printf("ID %d\n", id);
  if (periodicNeighbor.count(BC)!=0)
  { Node *n2= periodicNeighbor.find(BC)->second;
    if (n2)
    { printf("periodic with %d\n", n2->GetRealId());
      stats->AddPoint(this,n2);
    } else printf("Not periodic ?????\n");
  } else
  { printf("Not peridioc for patch %d\n");
  }
}

void Node::PrintDebug(FILE *f)
{
  fprintf(f, "ID %d\n", id);
  fprintf(f, "equiv ID %d\n", GetId());
  fprintf(f, "(%1.13lg %1.13lg %1.13lg)\n", x, y, z);
  // TODO: loop over maps if (periodicNeighbor) fprintf(f, "Periodic node to ID %d\n", periodicNeighbor->GetRealId());
}

void Node::Print(FILE *f)
{ 
 fprintf(f, "%d %1.13lg %1.13lg %1.13lg\n", id+1, x, y, z);
}

double Node::Distance(Node *n2)
{ double dx=x-n2->x;
  double dy=y-n2->y;
  double dz=z-n2->z;
  return sqrt(dx*dx+dy*dy+dz*dz);
}

void Node::SetId(int setId)
{
 id=setId;
 
}

int Node::GetId()
{
 if (equivalent != NULL) return equivalent->GetId();
 else return id;
}

void Node::SetPeriodicNeighbor(int BC, Node *n2)
{ pair<int,Node*> a(BC,n2);
  periodicNeighbor.insert(a);
}

int Node::GetPeriodicId(int BC)
{ if (periodicNeighbor.count(BC)==0) return 0;
  Node *n2= periodicNeighbor.find(BC)->second;
  return n2->GetId();
}

void Node::SetPeriodicSide(int BC, short n)
{ pair<int,short> a(BC,n);
  periodicSide.insert(a);
}

short Node::GetPeriodicSide(int BC) 
{ if (periodicSide.count(BC)==0) return 0;
  return periodicSide.find(BC)->second;
}
