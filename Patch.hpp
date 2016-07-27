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


#ifndef PATCH_HPP
#define PATCH_HPP

#include <string>
#include <stdio.h>
#include "Triplet.hpp"
#include "Block.hpp"

using namespace std;

#define IDIR 0
#define JDIR 1
#define KDIR 2
class Block;
class Patch
{ 
  private:
    Block *hostBlock; // number of host block
    int hostFace; // face of host block
    Block *donorBlock; // number of host block
    int donorFace; // face of host block
    string orientation; // mapping of host ijk to donor ijk
    bool isPeriodic;    // true for periodic boundary condition
    short periodicSide;
    int A[3][3];        // transformation matrix
    int BC;
    Triplet lowHost, highHost;   // two i,j,k triplets to define the region of this patch
    Triplet lowDonor, highDonor;  // two i,j,k triplets to define the region of this patch
    int nrFaces;
    bool is2d;
    
    
  public:
    Patch(int setBC, Block *setHostBlock, int setSF1, Block *setDonorBlock, string setOrientation, bool setPeriodic=false, short setPeriodicSide=0, bool is2d=false);
    ~Patch();
    void MakeEdgePointsUnique(); // Makes the points of an interface between two blocks unique
    // if a patch has BC 0 we assume it is an internal BC
    bool IsInternal() {return BC==0;};
    bool IsPeriodic() {return isPeriodic;};
    int GetBC() {return BC;};
    bool Contains(int direction, int i, int j, int k);
    int NrFaces() {return nrFaces;};
    int Count(int BC);
    void Write(FILE *f, int BC, bool isTetMesh);
    void WriteCell(FILE *f, Node *hostNode1, Node *hostNode2, Node *hostNode3, Node *hostNode4, bool isTetMesh);
    void WriteEdge(FILE *f, Node *hostNode1, Node *hostNode2);

};

#endif
