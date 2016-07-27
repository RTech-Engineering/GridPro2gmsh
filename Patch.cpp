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


#include "gridpro2gmsh.hpp"
#include "Patch.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define sgn(x) (x>2?-1:1)
#define del(x,y) ((x%3==y)?1:0)

Patch::Patch(int setBC, Block *setHostBlock, int setSF1, Block *setDonorBlock, string setOrientation, bool setPeriodic, short setPeriodicSide, bool is2d)
// notes:
// 1) sides are numbered from 0 to 5 and mean:
//    0: imin, 1: imax, 2: jmin, 3: jmax, 4: kmin, 5: kmax
// 2) direction are numbered from 0 to 5 and mean:
//    0: i-dir, 1: j-dir, 2: k-dir, 3: reversed i-dir, 4: reversed j-dir, etc

{ BC=setBC;
  hostBlock=setHostBlock;
  donorBlock=setDonorBlock;
  hostFace=setSF1;//[0 .. 5: imin, imax, jmin, jmax, kmin, kmax]
  orientation=setOrientation;
  if (orientation=="000")
  { printf("Invalid orientation %s\n", orientation.c_str());
    exit(1);
  }
  isPeriodic=setPeriodic;
  periodicSide=setPeriodicSide;
  this->is2d = is2d;


  // my side is hostFace, by dividing by 2 we get the (positive) direction 
  // 0 means idir, 1 means jdir, 2 means kdir
  int dir=hostFace/2; //[0 .. 2: idir, jdir, kdir]

  // We need to find the limits of the Host surface
  int Imax=hostBlock->I - 1;
  int Jmax=hostBlock->J - 1;
  int Kmax=hostBlock->K - 1;

  if (is2d) 
  { Kmax = 0;
  }
  printf("is2d=%d\n", is2d);

  switch (hostFace)
  {
   case 0:
          lowHost.Set(0,0,0);
          highHost.Set(0,Jmax,Kmax);
          nrFaces = (Jmax)*(Kmax);
   break;
   case 1:
          lowHost.Set(Imax,0,0);
          highHost.Set(Imax,Jmax,Kmax);
          nrFaces = (Jmax)*(Kmax);
   break;
   case 2:
          lowHost.Set(0,0,0);
          highHost.Set(Imax,0,Kmax);
          nrFaces = (Imax)*(Kmax);
   break;
   case 3:
          lowHost.Set(0,Jmax,0);
          highHost.Set(Imax,Jmax,Kmax);
          nrFaces = (Imax)*(Kmax);
   break;
   case 4:
          lowHost.Set(0,0,0);
          highHost.Set(Imax,Jmax,0);
          nrFaces = (Imax)*(Jmax);
   break;
   case 5:
          lowHost.Set(0,0,Kmax);
          highHost.Set(Imax,Jmax,Kmax);
          nrFaces = (Imax)*(Jmax);
   break;
  }
  //printf("The lowHost is (%d,%d,%d) and highHost is (%d,%d,%d)\n", lowHost.i,lowHost.j,lowHost.k, highHost.i,highHost.j,highHost.k); 



  // the following is only for internal boundaries or periodic BCs
  if (IsInternal() || IsPeriodic())
  { //printf("This patch is internal \n");
    // other (positive direction) is determined by examining the orientation:
    // convert char to integer
    // the string in the blk.tmp.conn file has value from 1-6 for the orientation, in character form
    // the orientationValue we want to be in  the range 0-5, so we extract from the ASCII 
    // code the ASCII code of '1' (eg '4'-'1'=3)
    // (note that I think in the merged file format the values go from 0-5 for the direction, to check later!
    int orientationVal=orientation[dir]-'1'; // [0 .. 5: i,j,k, -i,-j,-k ]
    //printf("orientationVal= %d \n", orientationVal);

    int otherDir=(orientationVal%3); // [0 .. 2: i,j,k]
    //printf("otherDir= %d \n", otherDir);



    // if orientationVal<=2 then the index goes in the same direction and therefore the 
    // plus face of my block connects to a minus face in the neighbor and vice versa
    if (orientationVal<=2)
    { if (hostFace == 0 || hostFace==2 || hostFace ==4) donorFace=2*otherDir+1;
      else (donorFace=2*otherDir);
    } 
    else // the indices are opposite:
    { if (hostFace == 0 || hostFace==2 || hostFace ==4) donorFace=2*otherDir;
      else (donorFace=2*otherDir+1);
    }


    //printf("I think the other side is: %d\n", donorFace);
  
    // now we fill the 3x3 tranformation matrix A.
    // A is used to compute the donor block local indices (id, jd, kd) as a function of the host block indices (i,j,k) as in:
    // id = lowDonor.i + a[0][0]*(i-lowHost.i) + a[0][1]*(j-lowHost.j) + a[0][2]*(k-lowHost.k)
    // jd = lowDonor.j + a[1][0]*(i-lowHost.i) + a[1][1]*(j-lowHost.j) + a[1][2]*(k-lowHost.k)
    // kd = lowDonor.k + a[2][0]*(i-lowHost.i) + a[2][1]*(j-lowHost.j) + a[2][2]*(k-lowHost.k)

    int a=orientation[0]-'1'; // range [0..5] meaning i,j,k,-i,-j,-k
    int b=orientation[1]-'1'; // range [0..5] meaning i,j,k,-i,-j,-k
    int c=orientation[2]-'1'; // range [0..5] meaning i,j,k,-i,-j,-k

    A[0][0]=sgn(a)*del(a,0); A[0][1]=sgn(b)*del(b,0); A[0][2]=sgn(c)*del(c,0);
    A[1][0]=sgn(a)*del(a,1); A[1][1]=sgn(b)*del(b,1); A[1][2]=sgn(c)*del(c,1);
    A[2][0]=sgn(a)*del(a,2); A[2][1]=sgn(b)*del(b,2); A[2][2]=sgn(c)*del(c,2);

    // if the i-direction is reversed then the sum of the columns is negative, and the patch is in opposite direction
    if (donorFace==0 || donorFace==1)
      lowDonor.i = (donorFace==0) ? 0 : donorBlock->I-1;
    else
      lowDonor.i = (A[0][0]+A[0][1]+A[0][2]<0)?donorBlock->I-1:0;

    if (donorFace==2 || donorFace==3)
      lowDonor.j = (donorFace==2) ? 0 : donorBlock->J-1;
    else
      lowDonor.j = (A[1][0]+A[1][1]+A[1][2]<0)?donorBlock->J-1:0;

    if (donorFace==4 || donorFace==5)
      lowDonor.k = (donorFace==4) ? 0 : donorBlock->K-1;
    else
      lowDonor.k = (A[2][0]+A[2][1]+A[2][2]<0)?donorBlock->K-1:0;


    // knowing all of this we can compute highDonor:
    highDonor.i = lowDonor.i +  A[0][0]*(highHost.i-lowHost.i) + A[0][1]*(highHost.j-lowHost.j) + A[0][2]*(highHost.k-lowHost.k);
    highDonor.j = lowDonor.j +  A[1][0]*(highHost.i-lowHost.i) + A[1][1]*(highHost.j-lowHost.j) + A[1][2]*(highHost.k-lowHost.k);
    highDonor.k = lowDonor.k +  A[2][0]*(highHost.i-lowHost.i) + A[2][1]*(highHost.j-lowHost.j) + A[2][2]*(highHost.k-lowHost.k);
  


    Node *n1, *n2;
    n1=hostBlock->GetNode(&lowHost);
    n2=donorBlock->GetNode(&lowDonor);
#ifdef COMPARE_MED
    printf("The lowDonor is (%d,%d,%d) and highDonor is (%d,%d,%d)\n", lowDonor.i,lowDonor.j,lowDonor.k, highDonor.i,highDonor.j,highDonor.k); 
    printf("Host block %d, face %d\n", hostBlock->Id(), hostFace);
    printf("Donor block %d, face %d\n", donorBlock->Id(), donorFace);
    printf("orientation string: %s  transferred to (%d %d %d)\n", orientation.c_str(), a, b, c);
    printf("A: %2d %2d %2d\n", A[0][0], A[0][1], A[0][2]);
    printf("B: %2d %2d %2d\n", A[1][0], A[1][1], A[1][2]);
    printf("C: %2d %2d %2d\n", A[2][0], A[2][1], A[2][2]);
    n1->Print();
    n2->Print();
#endif


#define DEBUG
#ifdef DEBUG
    // printf("Distance low: %1.13lg\n", n1->Distance(n2));
    if (n1->Distance(n2)>1e-6 && !isPeriodic )
    { printf("Something is not correct for the lower corner:\n");
      printf("Host block %d, face %d\n", hostBlock->Id(), hostFace);
      printf("Donor block %d, face %d\n", donorBlock->Id(), donorFace);
      printf("orientation string: %s  transferred to (%d %d %d)\n", orientation.c_str(), a, b, c);
      printf("A: %2d %2d %2d\n", A[0][0], A[0][1], A[0][2]);
      printf("B: %2d %2d %2d\n", A[1][0], A[1][1], A[1][2]);
      printf("C: %2d %2d %2d\n", A[2][0], A[2][1], A[2][2]);
      n1->Print();
      n2->Print();
      printf("orientationVal= %d \n", orientationVal);
      printf("dir=%d, otherDir= %d \n", dir, otherDir);
    
      exit(1);
    }

    n1=hostBlock->GetNode(&highHost);
    n2=donorBlock->GetNode(&highDonor);
    printf("Distance high: %1.13lg\n", n1->Distance(n2));
    if (n1->Distance(n2)>1e-6 && !isPeriodic)
    { printf("Something is not correct for the upper corner:\n");
      printf("Host block %d, face %d\n", hostBlock->Id(), hostFace);
      printf("Donor block %d, face %d\n", donorBlock->Id(), donorFace);
      printf("orientation string: %s  transferred to (%d %d %d)\n", orientation.c_str(), a, b, c);
      printf("A: %2d %2d %2d\n", A[0][0], A[0][1], A[0][2]);
      printf("B: %2d %2d %2d\n", A[1][0], A[1][1], A[1][2]);
      printf("C: %2d %2d %2d\n", A[2][0], A[2][1], A[2][2]);
      n1->Print();
      n2->Print();
      exit(1);
    }
    //printf("Constructor My address is %x\n", this);
#endif
    if (IsPeriodic())
    { Node *hostNode;
      Node *donorNode;
      int idonor;
      int jdonor;
      int kdonor;
      for (int k=lowHost.k; k<=highHost.k;k++)
      { for (int j=lowHost.j; j<=highHost.j;j++)
        { for (int i=lowHost.i; i<=highHost.i;i++)
          { hostNode=hostBlock->GetNode(i,j,k);
            idonor = lowDonor.i + A[0][0]*(i-lowHost.i) + A[0][1]*(j-lowHost.j) + A[0][2]*(k-lowHost.k);
            jdonor = lowDonor.j + A[1][0]*(i-lowHost.i) + A[1][1]*(j-lowHost.j) + A[1][2]*(k-lowHost.k);
            kdonor = lowDonor.k + A[2][0]*(i-lowHost.i) + A[2][1]*(j-lowHost.j) + A[2][2]*(k-lowHost.k);
            donorNode=donorBlock->GetNode(idonor,jdonor,kdonor);
            hostNode->SetPeriodicNeighbor(BC, donorNode);
            donorNode->SetPeriodicNeighbor(BC, hostNode);
            hostNode->SetPeriodicSide(BC, periodicSide);
            donorNode->SetPeriodicSide(BC, -periodicSide);
          }
        }
      }
    }
  }
}


void Patch::MakeEdgePointsUnique()
{
  Node *hostNode;
  Node *donorNode;
  int idonor;
  int jdonor;
  int kdonor;

  if (!IsInternal()) return;

  // we only want to set the equivalent in the donor block
  for (int k=lowHost.k; k<=highHost.k;k++)
  { for (int j=lowHost.j; j<=highHost.j;j++)
    { for (int i=lowHost.i; i<=highHost.i;i++)
      { hostNode=hostBlock->GetNode(i,j,k);
        idonor = lowDonor.i + A[0][0]*(i-lowHost.i) + A[0][1]*(j-lowHost.j) + A[0][2]*(k-lowHost.k);
        jdonor = lowDonor.j + A[1][0]*(i-lowHost.i) + A[1][1]*(j-lowHost.j) + A[1][2]*(k-lowHost.k);
        kdonor = lowDonor.k + A[2][0]*(i-lowHost.i) + A[2][1]*(j-lowHost.j) + A[2][2]*(k-lowHost.k);
        donorNode=donorBlock->GetNode(idonor,jdonor,kdonor);

        bool debug=false;
        if (hostNode->GetId()<donorNode->GetId()) donorNode->SetEquivalent(hostNode, debug);
      }
    }
  }
}


bool Patch::Contains(int direction, int i, int j, int k)
{
 if  (direction!=hostFace/2) return false;
 switch(direction)
   { 
     case 0:
       if (i!=lowHost.i) return false;
       return ((j>=lowHost.j && j<=highHost.j) && (k>=lowHost.k && k<=highHost.k));
     break;
 
     case 1:
       if (j!=lowHost.j) return false;
       return ((i>=lowHost.i && i<=highHost.i) && (k>=lowHost.k && k<=highHost.k));
     break;
     case 2:
       if (k!=lowHost.k) return false;       
       return ((i>=lowHost.i && i<=highHost.i) && (j>=lowHost.j && j<=highHost.j));
     break;
     default:
       printf("Unknown direction in Patch::Contains\n");
       return false;
   }
}


int Patch::Count(int x)
{ return (x==BC) ? nrFaces : 0;
}

void Patch::WriteCell(FILE *f, Node *hostNode1, Node *hostNode2, Node *hostNode3, Node *hostNode4, bool isTet)
{
  //   4 ---- 3
  //   |      |
  //   |      |
  //   1------2
  if (isTet) 
  {
    int i1 = hostNode1->GetId();
    int i2 = hostNode3->GetId();
    if (i2<i1)
    { i1 = i2;
    }
    int j1 = hostNode2->GetId();
    int j2 = hostNode4->GetId();
    if (j2<j1)
    { j1 = j2;
    }
    if (i1<j1)
    { // split diagonal 1-3
      fprintf(f, "4 %d %d %d\n", hostNode1->GetId(), hostNode2->GetId(), hostNode3->GetId());
      fprintf(f, "4 %d %d %d\n", hostNode1->GetId(), hostNode3->GetId(), hostNode4->GetId());
    } else
    {
      fprintf(f, "4 %d %d %d\n", hostNode1->GetId(), hostNode2->GetId(), hostNode4->GetId());
      fprintf(f, "4 %d %d %d\n", hostNode2->GetId(), hostNode3->GetId(), hostNode4->GetId());
    }
  } else
  {
    fprintf(f, "9 %d  ", hostNode1->GetId());
    fprintf(f, "%d  ", hostNode2->GetId());
    fprintf(f, "%d  ", hostNode3->GetId());
    fprintf(f, "%d\n", hostNode4->GetId());
  }
}

void Patch::WriteEdge(FILE *f, Node *hostNode1, Node *hostNode2)
{
  //   1------2
  fprintf(f, "3 %d  ", hostNode1->GetId());
  fprintf(f, "%d\n", hostNode2->GetId());
}


void Patch::Write(FILE *f, int xBC, bool isTetMesh)
{
  Node *hostNode1, *hostNode2, *hostNode3, *hostNode4;
  if (xBC != BC) 
  { return;
  }
  int dir=hostFace/2; //[0 .. 2: idir, jdir, kdir]

  if (is2d) 
  { 
    switch (dir)
    {
     case 0:
      for (int k=lowHost.k; k<highHost.k; k++)
        { for (int j=lowHost.j; j<highHost.j; j++)
          { for (int i=lowHost.i; i<=highHost.i; i++)
            {
              hostNode1=hostBlock->GetNode(i,j,k);
              hostNode2=hostBlock->GetNode(i,j+1,k);
              WriteEdge(f, hostNode1, hostNode2);
              hostNode1->PrintDebug();
              hostNode2->PrintDebug();
            }
          } 
        }
     break;
     case 1:
       for (int k=lowHost.k; k<highHost.k; k++)
        { for (int j=lowHost.j; j<=highHost.j; j++)
          { for (int i=lowHost.i; i<highHost.i; i++)
            {
              hostNode1=hostBlock->GetNode(i,j,k);
              hostNode2=hostBlock->GetNode(i+1,j,k);
              WriteEdge(f, hostNode1, hostNode2);
              hostNode1->PrintDebug();
              hostNode2->PrintDebug();
            }
          } 
        }
     break;
    }
  } else
  {
    switch (dir)
    {
     case 0:
       for (int k=lowHost.k; k<highHost.k;k++)
       { for (int j=lowHost.j; j<highHost.j;j++)
         { for (int i=lowHost.i; i<=highHost.i;i++)
           {
             hostNode1=hostBlock->GetNode(i,j,k);
             hostNode2=hostBlock->GetNode(i,j+1,k);
             hostNode3=hostBlock->GetNode(i,j+1,k+1);
             hostNode4=hostBlock->GetNode(i,j,k+1);
             WriteCell(f, hostNode1, hostNode2, hostNode3, hostNode4, isTetMesh);
             hostNode1->PrintDebug();
             hostNode2->PrintDebug();
             hostNode3->PrintDebug();
             hostNode4->PrintDebug();
           }
         }
       }
     break;
     case 1:
       for (int k=lowHost.k; k<highHost.k;k++)
       { for (int j=lowHost.j; j<=highHost.j;j++)
         { for (int i=lowHost.i; i<highHost.i;i++)
           { hostNode1=hostBlock->GetNode(i,j,k);
             hostNode2=hostBlock->GetNode(i+1,j,k);
             hostNode3=hostBlock->GetNode(i+1,j,k+1);
             hostNode4=hostBlock->GetNode(i,j,k+1);
             WriteCell(f, hostNode1, hostNode2, hostNode3, hostNode4, isTetMesh);
             hostNode1->PrintDebug();
             hostNode2->PrintDebug();
             hostNode3->PrintDebug();
             hostNode4->PrintDebug();
           }
         }
       }
     break;
     case 2:
       for (int k=lowHost.k; k<=highHost.k;k++)
       { for (int j=lowHost.j; j<highHost.j;j++)
         { for (int i=lowHost.i; i<highHost.i;i++)
           { hostNode1=hostBlock->GetNode(i,j,k);
             hostNode2=hostBlock->GetNode(i,j+1,k);
             hostNode3=hostBlock->GetNode(i+1,j+1,k);
             hostNode4=hostBlock->GetNode(i+1,j,k);
             WriteCell(f, hostNode1, hostNode2, hostNode3, hostNode4, isTetMesh);
             hostNode1->PrintDebug();
             hostNode2->PrintDebug();
             hostNode3->PrintDebug();
             hostNode4->PrintDebug();
           }
         }
     }
     break;
    }
  }
}
