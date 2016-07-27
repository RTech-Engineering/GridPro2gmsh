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


#ifndef FACE_HPP
#define FACE_HPP
#include "Node.hpp"
#include <string>
#include <math.h>
#include <stdio.h>

class Face
{ friend class Node;
  private:
    Node *n0, *n1, *n2, *n3;
    int idFace;
    int owner, neighbor;
    int BC;
    int internalBC;
    int BCorg; // only for debug
    int sumId;
    class Face* matchingFace;

    // for periodic BCs
    bool isPeriodic;
    int sumPeriodicId;
    int idPeriodicFace;
    short periodicSide;
  public:
    
    Face(Node *n0, Node *n1, Node *n2, Node *n3, int setOwner, int setNeighbor, int setBC);
    ~Face();
    void Print(FILE *f = stdout);
    void PrintDebug(FILE *f = stdout);
    void AnalysePeriodic(PeriodicStats *stats, int BC);
    void WriteTecplot(FILE *f = stdout);
    void SetIdFace(int setIdFace);
    int GetIdFace() {return idFace;};
    int GetOwner();
    int GetNeighbor();
    void SetOwner(int setOwner);
    void SetNeighbor(int setNeighbor);
    bool MergeNeeded(Face *other);
    int SumId();
    void ComputeSumPeriodicId();
    bool PeriodicMatch(Face *x);
    int SumPeriodicId();
    int GetBCorg() {return BCorg;};
    int GetBC() {return BC;};

    void SetPeriodic() { isPeriodic=true;};
    bool IsInternal() { return BC==-1;};
    bool IsPeriodic() { return isPeriodic;};
    void SetMatchingFace(Face *face) { matchingFace=face;};
    void SetPeriodicFaceId(int x) { idPeriodicFace=x;};
    int GetPeriodicFaceId() { return idPeriodicFace;};
    int GetPeriodicSide() { return periodicSide;};
};
#endif
