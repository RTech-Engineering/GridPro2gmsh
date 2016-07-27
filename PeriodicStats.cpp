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

#include "PeriodicStats.hpp"
#include <math.h>
#include <stdlib.h>


PeriodicStats::PeriodicStats()
{ count=0;
  dx=dy=dz=0.0;
  l=m=n=0.0;
  dxrot=dyrot=dzrot=0;

}


void PeriodicStats::AddPoint(Node *n1, Node *n2)
{ 
  // obtain two nodes which are supposed to be rotational periodic
  double dxl=fabs(n2->x-n1->x);
  double dyl=fabs(n2->y-n1->y);
  double dzl=fabs(n2->z-n1->z);
  dx+=dxl;
  dy+=dyl;
  dz+=dzl;

  if (n1==n2)
  { n1->x=n2->x=0;
    n1->z=n2->z=0;
  }
  //if (dxl+dyl+dzl<1e-6) return;
  printf("ANGLE %lg , DIST %lg %lg %lg\n", atan2(n1->z,n1->x)*180./M_PI-atan2(n2->z,n2->x)*180./M_PI, fabs(n2->x-n1->x), fabs(n2->y-n1->y), fabs(n2->z-n1->z));

  l+=fabs(atan2(n1->z,n1->y)-atan2(n2->z,n2->y));
  m+=fabs(atan2(n1->z,n1->x)-atan2(n2->z,n2->x));
  n+=fabs(atan2(n1->y,n1->x)-atan2(n2->y,n2->x));
  double theta = (atan2(n1->z,n1->x)>atan2(n2->z,n2->x)?-120*M_PI/180.:120*M_PI/180.);
  double x2=n1->x*cos(theta) - n1->z*sin(theta);
  double z2=n1->x*sin(theta) + n1->z*cos(theta);

  double m=fabs(atan2(n1->z,n1->x)-atan2(z2,x2));

  printf("Points x:%lg %lg, y:%lg,%lg,  z: %lg %lg,  m=%lg\n", n2->x, x2, n1->y, n2->y, n2->z, z2, m*180/M_PI);
  dxrot+=fabs(n2->x- x2);
  dzrot+=fabs(n2->z- z2);

  if (fabs(n2->x-x2)+ fabs(n2->y-n1->y)+ fabs(n2->z-z2)>0.001)
  { printf("Error; point difference too large after rotation\n");
    exit(1);
  }
  count ++;
}


double PeriodicStats::MeanDX()
{ if (count==0) return 0;
  return dx/count;
}

double PeriodicStats::MeanDY()
{ if (count==0) return 0;
  return dy/count;
}

double PeriodicStats::MeanDZ()
{ if (count==0) return 0;
  return dz/count;
}

double PeriodicStats::MeanDXrot()
{ if (count==0) return 0;
  return dxrot/count;
}

double PeriodicStats::MeanDYrot()
{ if (count==0) return 0;
  return dyrot/count;
}

double PeriodicStats::MeanDZrot()
{ if (count==0) return 0;
  return dzrot/count;
}

double PeriodicStats::MeanL()
{ if (count==0) return 0;
  return l/count;
}

double PeriodicStats::MeanM()
{ if (count==0) return 0;
  return m/count;
}

double PeriodicStats::MeanN()
{ if (count==0) return 0;
  return n/count;
}

void PeriodicStats::Print()
{
  printf("dx=%1.16lg\n", MeanDX());
  printf("dy=%1.16lg\n", MeanDY());
  printf("dz=%1.16lg\n", MeanDZ());
  printf("dl=%1.16lg\n", MeanL()*180./M_PI);
  printf("dm=%1.16lg\n", MeanM()*180./M_PI);
  printf("dn=%1.16lg\n", MeanN()*180./M_PI);
  printf("dxrot=%1.16lg\n", MeanDXrot());
  printf("dyrot=%1.16lg\n", MeanDYrot());
  printf("dzrot=%1.16lg\n", MeanDZrot());
}
