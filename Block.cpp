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


#include "Block.hpp"
#include "HexCell.hpp"
#include "Node.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Block::Block(int setI, int setJ, int setK, int setId)
{ // printf("Hello I'm the constructor of Block called with %d %d %d\n", setI, setJ, setK);
  I=setI;
  J=setJ;
  K=setK;
  id=setId;
}


Block::~Block()
{ printf("Hello I'm the destructor of Block\n");
}


void Block::ReadGridPro(FILE *f, int meshType, bool axisym, double scaling)
// nodes are placed consecutively in vector; since the K is running fastest, the indexing in the
// vector for a point i, j, k is (with i,j,k starting from 0,0,0):
// i*J*K+j*K+k
{ bool onBoundary;
  double x, y, z;
  Node *node;
  Node *node1;
  int angle=5;
  double rayon;
  // the boolean is2d identifies a two-dimensional case. we need to transform it to 3d so we create an extra slice if it is true and at the end of the routine we set K to 2.
  int meshTypeX = meshType;
  int meshTypeY = meshType;
  int meshTypeZ = meshType;

  bool is2D = (K==1);
  bool is1D = (K==1 & J==1);
  if ((I-1)%meshTypeX!=0)
    {
     printf("The mesh is not divisible by %d along direction i, I=%d, J=%d, K=%d\n", meshTypeX, I, J, K);
     if ((I-1)%(meshTypeX-1)!=0)
       { printf("The mesh is not divisible by %d along direction i, I=%d, J=%d, K=%d\n", (meshTypeX-1), I, J, K);
         if ((I-1)%(meshTypeX-2)!=0)
           {printf("The mesh is not divisible by %d along direction i, I=%d, J=%d, K=%d\n", (meshTypeX-2), I, J, K);
	    meshTypeX = meshTypeX-3;
	    printf("The mesh cannot be coarsened along i direction,I=%d, J=%d, K=%d\n", I, J, K );
    }
	 else
	   {meshTypeX = meshTypeX-2;
	    printf("The mesh has been coarsened by %d along direction i\n", meshTypeX);
           }
       }
     else
       { meshTypeX=meshTypeX-1;
         printf("The mesh has been coarsened by %d along direction i\n", meshTypeX);
       }
    }
  if ((J-1)%meshTypeY!=0)
    {
     printf("The mesh is not divisible by %d along direction j, I=%d, J=%d, K=%d\n", meshTypeY, I, J, K);
     if ((J-1)%(meshTypeY-1)!=0)
       { printf("The mesh is not divisible by %d along direction j, I=%d, J=%d, K=%d\n", (meshTypeY-1), I, J, K);
         if ((J-1)%(meshTypeY-2)!=0)
           {printf("The mesh is not divisible by %d along direction j, I=%d, J=%d, K=%d\n", (meshTypeY-2), I, J, K);
	    meshTypeY = meshTypeY-3;
	    printf("The mesh cannot be coarsened along j direction,I=%d, J=%d, K=%d\n", I, J, K );
           }
	 else
	   {meshTypeY = meshTypeY-2;
	    printf("The mesh has been coarsened by %d along direction j\n", meshTypeY);
           }
       }
     else
       { meshTypeY=meshTypeY-1;
         printf("The mesh has been coarsened by %d along direction j\n", meshTypeY);
       }
    }
  if ((K-1)%meshTypeZ!=0)
    {
     printf("The mesh is not divisible by %d along direction k, I=%d, J=%d, K=%d\n", meshTypeZ, I, J, K);
     if ((K-1)%(meshTypeZ-1)!=0)
       { printf("The mesh is not divisible by %d along direction k, I=%d, J=%d, K=%d\n", (meshTypeZ-1), I, J, K);
         if ((K-1)%(meshTypeZ-2)!=0)
           {printf("The mesh is not divisible by %d along direction k, I=%d, J=%d, K=%d\n", (meshTypeZ-2), I, J, K);
	    meshTypeZ = meshTypeZ-3;
	    printf("The mesh cannot be coarsened along k direction,I=%d, J=%d, K=%d\n", I, J, K );
           }
	 else
	   {meshTypeZ = meshTypeZ-2;
	    printf("The mesh has been coarsened by %d along direction k\n", meshTypeZ);
           }
       }
     else
       { meshTypeZ=meshTypeZ-1;
         printf("The mesh has been coarsened by %d along direction k\n", meshTypeZ);
    }
    }
  for (int i=0; i<I; i++)
  { for (int j=0; j<J; j++)
    { for (int k=0; k<K; k++)
      {
	fscanf(f, "%lg%lg%lg", &x, &y, &z);
        x=x*scaling;
  	y=y*scaling;
  	z=z*scaling;
        if ((k%meshTypeZ==0) && (j%meshTypeY==0) && (i%meshTypeX==0))
	{
          onBoundary = (I>1 && (i==0 || i==I-1)) ||
                       (J>1 && (j==0 || j==J-1)) ||
                       (K>1 && (k==0 || k==K-1));
          if (onBoundary) 
          {
           // create an edge node
           node=new Node(x, y, z, true);
          }
          else 
          {
           // create a regular node (internal)
            node=new Node(x, y, z, false);
          }
          nodes.push_back(node);
        }
      }
    }
  }
  
  I=(I-1)/meshTypeX + 1;
  J=(J-1)/meshTypeY + 1;
  K=(K-1)/meshTypeZ + 1;
}


Node* Block::GetNode(int i, int j, int k)
{ 
 return nodes[i*J*K+j*K+k];
}


Node* Block::GetNode(Triplet *t)
{ 
 return nodes[t->i*J*K+t->j*K+t->k];
}

void Block::PrintEquivalent()
{
 for (int n=0; n<nodes.size(); n++)
  {
   nodes[n]->PrintEquivalent();
  }
}


void Block::CountCells(int &count, bool tetMesh)
{ 
  cellOffset = count;
  if (tetMesh) 
  { count += CountCellsTet();

  } else
  {
    if (GetDimension() == 2) 
    { count+=(I-1)*(J-1);
    } else 
    { count+=(I-1)*(J-1)*(K-1);
    }
  }
}


void Block::CountPoints(int &count)
{
 for (int n=0; n<nodes.size(); n++)
  {
   if (nodes[n]->GetEquivalent() ==NULL) 
   {
    nodes[n]->SetId(count);
    count++;
   }
  } 
}


void Block::WritePoints(FILE *f)
{
 for (int n=0; n<nodes.size(); n++)
  {
   if (nodes[n]->GetEquivalent() ==NULL) 
   {
    nodes[n]->Print(f);
   } 
  }
}

int Block::MakeCellId(int i, int j, int k)
{
 if (GetDimension() == 2)
 { 
  k = 1;
  if ((i<0) || (i==I-1)) return -1;
  if ((j<0) || (j==J-1)) return -1;
  return i*(J-1)+j+cellOffset;
 } else
 {
 if ((i<0) || (i==I-1)) return -1;
 if ((j<0) || (j==J-1)) return -1;
 if ((k<0) || (k==K-1)) return -1;
 return i*(J-1)*(K-1)+j*(K-1)+k+cellOffset;
 }
}


int Block::FindPatch(int direction, int i, int j, int k)
{ 
  switch (direction)
  { case IDIR:
      for (int n=0; n<externalPatches.size(); n++)
      { 
        if ((externalPatches[n]->Contains(IDIR,i,j,k)) && (externalPatches[n]->Contains(IDIR,i,j+1,k+1)))
          return externalPatches[n]->GetBC();
      }
    break;
    case JDIR:
      for (int n=0; n<externalPatches.size(); n++)
      { 
        if ((externalPatches[n]->Contains(JDIR,i,j,k)) && (externalPatches[n]->Contains(JDIR,i+1,j,k+1)))
          return externalPatches[n]->GetBC();
      }
    break;
    case KDIR:
      for (int n=0; n<externalPatches.size(); n++)
      { 
        if ((externalPatches[n]->Contains(KDIR,i,j,k)) && (externalPatches[n]->Contains(KDIR,i+1,j+1,k)))
          return externalPatches[n]->GetBC();
      }
    break;
    default: {printf("what are you doing here?\n"); exit(1);}
  }
  return -1;
}


void Block::CreateFaces(list <Face*> &faces)
{
 int ownerId;
 int neighborId;
 int i;
 int j;
 int k;
 Node *n0, *n1, *n2, *n3;
 int id0, id1, id2, id3;
 Face *face;
 int BC;


 // All faces in i-direction
 for (k=0; k<K-1; k++)
 {
   for (j=0; j<J-1; j++)
   {
    for(i=0; i<I; i++)
    {  
        ownerId    = MakeCellId(i-1, j, k);
        neighborId = MakeCellId(i  , j, k);
        n0=GetNode(i,j,k);
        n1=GetNode(i,j+1,k);
        n2=GetNode(i,j+1,k+1);
        n3=GetNode(i,j,k+1);
        id0=n0->GetId();
        id1=n1->GetId();
        id2=n2->GetId();
        id3=n3->GetId();
        // find if all nodes are within any external patch
        // return the corresponding BC or -1 if none
        BC = FindPatch(IDIR, i, j, k);  
        if (i==0) face=new Face(n0, n3, n2, n1, ownerId, neighborId, BC);
        else face=new Face(n0, n1, n2, n3, ownerId, neighborId, BC);
        faces.push_back(face);
    }
   }
 }
 // All faces in j-direction
 for (k=0; k<K-1; k++)
 {
  for(i=0; i<I-1; i++)
   {
    for (j=0; j<J; j++)
     {  
        ownerId    = MakeCellId(i, j-1, k);
        neighborId = MakeCellId(i, j  , k);
        n0=GetNode(i,j,k);
        n1=GetNode(i,j,k+1);
        n2=GetNode(i+1,j,k+1);
        n3=GetNode(i+1,j,k);
        id0=n0->GetId();
        id1=n1->GetId();
        id2=n2->GetId();
        id3=n3->GetId();
        BC = FindPatch(JDIR, i, j, k);
        if (j==0) face=new Face(n0, n3, n2, n1, ownerId, neighborId, BC);
        else face=new Face(n0, n1, n2, n3, ownerId, neighborId, BC);
        faces.push_back(face);
     } 
   }
 }

 // All faces in k-direction
 for(i=0; i<I-1; i++)
 {
  for (j=0; j<J-1; j++)
   {
    for (k=0; k<K; k++)
     {   
        ownerId    = MakeCellId(i, j, k-1);
        neighborId = MakeCellId(i, j, k);
        n0=GetNode(i,j,k);
        n1=GetNode(i+1,j,k);
        n2=GetNode(i+1,j+1,k);
        n3=GetNode(i,j+1,k);
        id0=n0->GetId();
        id1=n1->GetId();
        id2=n2->GetId();
        id3=n3->GetId();
        BC = FindPatch(KDIR, i, j, k);
        if (k==0) face=new Face(n0, n3, n2, n1, ownerId, neighborId, BC);
        else face=new Face(n0, n1, n2, n3, ownerId, neighborId, BC);
        faces.push_back(face);
     }
   }
 } 
}

void Block::AddExternalPatch(Patch *patch)
{
 externalPatches.push_back(patch);
}


void Block::AddBoundaryZone(int side, int nBZ)
{ boundaryCellSide[nBZ].push_back(side);
}


void Block::PrintBoundaryCellList(FILE *f, int nBZ)
{ 
  vector<int>::iterator iBound;
  // boundaryCellSide contains a list of sides (0: IMIN, 1 IMAX etc) that have a boundaryCell condition
  
  for (iBound=boundaryCellSide[nBZ].begin(); iBound!=boundaryCellSide[nBZ].end(); iBound++)
  { switch (*iBound)
    { case IMIN:
        for (int j=0; j<J-1; j++)
          { for (int k=0; k<K-1; k++)
              fprintf(f, "%d\n", MakeCellId(0,j,k));
          }
      break;
      case IMAX:
        for (int j=0; j<J-1; j++)
          { for (int k=0; k<K-1; k++)
              fprintf(f, "%d\n", MakeCellId(I-2,j,k));
          }
      break;     
      case JMIN:
        for (int i=0; i<I-1; i++)
          { for (int k=0; k<K-1; k++)
              fprintf(f, "%d\n", MakeCellId(i,0,k));
          }
      break;
      case JMAX:
        for (int i=0; i<I-1; i++)
          { for (int k=0; k<K-1; k++)
              fprintf(f, "%d\n", MakeCellId(i,J-2,k));
          }
      break;
      case KMIN:
        for (int i=0; i<I-1; i++)
          { for (int j=0; j<J-1; j++)
              fprintf(f, "%d\n", MakeCellId(i,j,0));
          }
      break;
      case KMAX:
        for (int i=0; i<I-1; i++)
          { for (int j=0; j<J-1; j++)
              fprintf(f, "%d\n", MakeCellId(i,j,K-2));
          }
      break;
    }
  }
}

void Block::PrintCellList(FILE *f)
{ 
  for (int i=0; i<I-1; i++)
  { for (int j=0; j<J-1; j++)
    { for (int k=0; k<K-1; k++)
        fprintf(f, "%d\n", MakeCellId(i,j,k));
    }
  }
}

int Block::NrCells()
{ return (I-1)*(J-1)*(K-1);
}

int Block::NrBoundaryZoneCells(int nBZ)
{
  vector<int>::iterator iBound;
  // boundaryCellSide contains a list of sides (0: IMIN, 1 IMAX etc) that have a boundaryCell condition
  
  for (iBound=boundaryCellSide[nBZ].begin(); iBound!=boundaryCellSide[nBZ].end(); iBound++)
  { switch (*iBound)
    { case IMIN:
        return (J-1)*(K-1);
      break;
      case IMAX:
        return (J-1)*(K-1);
      break;     
      case JMIN:
        return (I-1)*(K-1);
      break;
      case JMAX:
        return (I-1)*(K-1);
      break;
      case KMIN:
        return (I-1)*(J-1);
      break;
      case KMAX:
        return (I-1)*(J-1);
      break;
    }
  }
  return 0;
}

void Block::CreateBoundaryZoneVector(int numberBZ)
{ boundaryCellSide.resize(numberBZ);
}

int Block::GetDimension()
{
  return (K==1) ? 2 : 3;
}


void Block::WriteCells(FILE *f, bool tetMesh, int &id)
{
  if (tetMesh) 
  { WriteCellsTet(f, id);
  } else
  { if (GetDimension() == 2)
     { WriteCellsQuad(f);
     } else
     { WriteCellsHex(f);
     }
  }
}

void Block::WriteCellsHex(FILE *f)
{
  int id0, id1, id2, id3, id4, id5, id6, id7;
  int ownerId;
  Node *n0, *n1, *n2, *n3, *n4, *n5, *n6, *n7;
  for (int k=0; k<K-1; k++)
  {
    for (int j=0; j<J-1; j++)
    {
      for(int i=0; i<I-1; i++)
      {  
        ownerId    = MakeCellId(i, j, k);

        n0=GetNode(i  ,j,  k);
        n1=GetNode(i+1,j  ,k);
        n2=GetNode(i+1,j+1,k);
        n3=GetNode(i  ,j+1,k);

        n4=GetNode(i  ,j,  k+1);
        n5=GetNode(i+1,j  ,k+1);
        n6=GetNode(i+1,j+1,k+1);
        n7=GetNode(i  ,j+1,k+1);

        id0=n0->GetId() + 1;
        id1=n1->GetId() + 1;
        id2=n2->GetId() + 1;
        id3=n3->GetId() + 1;
        id4=n4->GetId() + 1;
        id5=n5->GetId() + 1;
        id6=n6->GetId() + 1;
        id7=n7->GetId() + 1;
        fprintf(f, "%d 5 2 0 1  %d %d %d %d  %d %d %d %d\n", ownerId, id0, id1, id2, id3, id4, id5, id6, id7);
      }
    }
  }
}


void Block::WriteCellsTet(FILE *f, int &id)
{
  int id0, id1, id2, id3, id4, id5, id6, id7;
  int ownerId;
  Node *n0, *n1, *n2, *n3, *n4, *n5, *n6, *n7;
  HexCell *h;
  for (int k=0; k<K-1; k++)
  {
    for (int j=0; j<J-1; j++)
    {
      for(int i=0; i<I-1; i++)
      {  
        ownerId    = MakeCellId(i, j, k);
        n0=GetNode(i  ,j,  k);
        n1=GetNode(i+1,j  ,k);
        n2=GetNode(i+1,j+1,k);
        n3=GetNode(i  ,j+1,k);
        n4=GetNode(i  ,j,  k+1);
        n5=GetNode(i+1,j  ,k+1);
        n6=GetNode(i+1,j+1,k+1);
        n7=GetNode(i  ,j+1,k+1);
        h = new HexCell(n0, n1, n2, n3, n4, n5, n6, n7);
        h->WriteAsTet(f, id);
        //fprintf(f, "12 %d %d %d %d  %d %d %d %d %d\n", id0, id1, id2, id3, id4, id5, id6, id7, ownerId);
      }
    }
  }
}


void Block::WriteCellsQuad(FILE *f)
{
  int id0, id1, id2, id3;
  int ownerId;
  Node *n0, *n1, *n2, *n3;
 for (int k=0; k<=K-1; k++)
 {
    for (int j=0; j<J-1; j++)
    {
      for(int i=0; i<I-1; i++)
      {
        ownerId    = MakeCellId(i, j, k);
        n0=GetNode(i  ,j,   k);
        n1=GetNode(i+1,j,   k);
        n2=GetNode(i+1,j+1, k);
        n3=GetNode(i  ,j+1, k);
        //n4=GetNode(i  ,j,  k+1);
        //n5=GetNode(i+1,j  ,k+1);
        //n6=GetNode(i+1,j+1,k+1);
        //n7=GetNode(i  ,j+1,k+1);
        id0=n0->GetId();
        id1=n1->GetId();
        id2=n2->GetId();
        id3=n3->GetId();
        //id4=n4->GetId();
        //id5=n5->GetId();
        //id6=n6->GetId();
        //id7=n7->GetId();
        fprintf(f, "9 %d %d %d %d %d\n", id0, id1, id2, id3, ownerId);
      }
    }
 }
}


int Block::CountCellsTet()
{
  int id0, id1, id2, id3, id4, id5, id6, id7;
  int ownerId;
  Node *n0, *n1, *n2, *n3, *n4, *n5, *n6, *n7;
  HexCell *h;
  int count = 0;
  for (int k=0; k<K-1; k++)
  {
    for (int j=0; j<J-1; j++)
    {
      for(int i=0; i<I-1; i++)
      {  
        ownerId    = MakeCellId(i, j, k);
        n0=GetNode(i  ,j,  k);
        n1=GetNode(i+1,j  ,k);
        n2=GetNode(i+1,j+1,k);
        n3=GetNode(i  ,j+1,k);
        n4=GetNode(i  ,j,  k+1);
        n5=GetNode(i+1,j  ,k+1);
        n6=GetNode(i+1,j+1,k+1);
        n7=GetNode(i  ,j+1,k+1);
        h = new HexCell(n0, n1, n2, n3, n4, n5, n6, n7);
        count += h->CountTets();
      }
    }
  }
  return count;
}
