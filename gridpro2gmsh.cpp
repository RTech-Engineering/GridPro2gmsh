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

#include "util.h"
#include <vector>
#include <list>
#include <string>
#include <map>
#include <utility>
#include <algorithm>
#include "ctype.h"



#include "Block.hpp"
#include "Patch.hpp"
#include "RTechTimer.hpp"


#define MAXLABEL 10000

using namespace std;


int GetLine(char *line, FILE *fp)
{
  while( fgets(line,255,fp),line[0]=='#');
  return feof(fp);
}


static bool compareFaceSumId(Face *f1, Face *f2)
{ return (f1->SumId()<f2->SumId());
}

static bool compareFaceBC(Face *f1, Face *f2)
{ bool sameBC = (f1->GetBC()==f2->GetBC());
  bool sameOwner = (f1->GetOwner()==f2->GetOwner());
  if (f1->IsPeriodic() && f2->IsPeriodic())
    return (f1->GetBC()<f2->GetBC()) || (sameBC && f1->GetPeriodicFaceId()<f2->GetPeriodicFaceId()) ;

  return (f1->GetBC()<f2->GetBC()) || (sameBC && (f1->GetOwner()<f2->GetOwner())) ||
           (sameBC && sameOwner && (f1->GetNeighbor()<f2->GetNeighbor()));  
}

main(int argc, char *argv[])
{ FILE *f;
  int meshType=1;
  bool tetMesh = false;
  bool mediumMesh = false;
  bool coarseMesh = false;
  bool axisym=false;
  RTechTimer timer;
  double scaling=1;
  list <int> cyclicPatches;
  list <int> solidPatches;
  vector <int> boundaryZonePatches;
	
  if (argc<3) 
  { printf("Usage: %s <grid input file> <grid output file>\n", argv[0]);
        printf("options (* default):\n");
        printf("--tet          creates a tet mesh\n");
        printf("--medium       creates a medium mesh\n");
        printf("--coarse       creates a coarse mesh\n");
        printf("--axisym       creates an axisymetric mesh from a 2D file\n");
        printf("--scale        creates a scaled mesh with scale equal to the folowing argument\n");
        printf("\n");
        return 1;
    exit(1);
  }  
  else //(argc>=2)
  {
   for (int readArg=3; readArg<argc; readArg++)
    {
      if (!strcmp("--help",argv[readArg]))
      {
        printf("options (* default):\n");
        printf("--tet          creates a tet mesh\n");
        printf("--medium       creates a medium mesh\n");
        printf("--coarse       creates a coarse mesh\n");
        printf("--axisym       creates an axisymetric mesh from a 2D file\n");
        printf("--scale        creates a scaled mesh with scale equal to the folowing argument\n");
        printf("\n");
        return 1;
      }
      else if (!strcmp("--tet",argv[readArg]))
      { printf("you're trying to create a tet mesh\n");
	tetMesh = true;
      }
      else if (!strcmp("--medium",argv[readArg]))
      { printf("you're trying to create a medium mesh\n");
	meshType = 2;
	mediumMesh = true;
      }
      else if (!strcmp("--coarse",argv[readArg]))
      { printf("you're trying to create a coarse mesh\n");
	meshType = 4;
	coarseMesh = true;
      }
      else if (!strcmp("--axisym",argv[readArg]))
      { printf("you're trying to create an axi-symmetric mesh\n");
	axisym = true;
      }	
      else if (!strcmp("--scale",argv[readArg]))
      {
        printf("you're scaling your model with scale equal to %lg\n", atof(argv[readArg+1]));
        scaling=atof(argv[readArg+1]);
        readArg=readArg+1;//the folowing argument being the scale value, it mustn't be read 
      }
      else if (!strcmp("--cyclic", argv[readArg]))
      { printf("internal patch %d will be treated as a cyclic patch\n", atoi(argv[readArg+1]));
        int cyclic=atoi(argv[readArg+1]);
        readArg=readArg+1;//the folowing argument being the scale value, it mustn't be read 
        cyclicPatches.push_back(cyclic);
      }
      else if (!strcmp("--solid", argv[readArg]))
      { printf("internal patch %d will be treated as two solid patches\n", atoi(argv[readArg+1]));
        int solid=atoi(argv[readArg+1]);
        readArg=readArg+1;//the folowing argument being the scale value, it mustn't be read 
        solidPatches.push_back(solid);
      }
      else if (!strcmp("--boundaryZone", argv[readArg]))
      { printf("internal patch %d will be treated as boundary patches\n", atoi(argv[readArg+1]));
        int boundaryZone=atoi(argv[readArg+1]);
        readArg=readArg+1;//the folowing argument being the scale value, it mustn't be read 
        boundaryZonePatches.push_back(boundaryZone);
      }
      else
      { 
	printf("unknown command line option try --help\n");
        return 1;
      }
    }
  }
  
  printf(" --> Reading ....\n");
  timer.Restart();
  vector<Block*> blocks;
  Block *block;
  
  // file input error handling
  f=fopen(argv[1], "r");
  if (!f)
  { printf("Could not open: %s \n", argv[1]);
    exit(1);
  }
  char newName[255];
  sprintf(newName, "%s.cgns", argv[1]);


  string openFoamDir=string(argv[2]);


  // skip header starting with #
  int ch;
  while (ch=getc(f), ch=='#')
  { while (ch=getc(f), ch!='\n'&&ch!=EOF);
  }
  ungetc(ch, f);

  // read IJK and coordinates
  int I, J, K;
  while(fscanf(f, "%d%d%d", &I, &J, &K)==3)
  { block = new Block(I, J, K, blocks.size());
    block->ReadGridPro(f, meshType, axisym, scaling);
    block->CreateBoundaryZoneVector(boundaryZonePatches.size());
    blocks.push_back(block);
  }
  fclose(f);

  // now handle the connectivity
  sprintf(newName, "%s.conn", argv[1]);
  f=fopen(newName, "r");
  if (!f)
  { printf("Could not open: %s \n", newName);
    exit(1);
  }

  ////////////////// face BC /////////////////////////:
  // get the surfaces and the mappings between gridpro surfaces and openFOAM
  char line[255], label[255];
  int id;
  int nrSurfLabels=0;
  while (!GetLine(line,f))
  { 
    if (strstr(line, "surf labe") || strstr(line, "surface labe") )
    { sscanf(line, "%d", &nrSurfLabels);
      break;
    }
  }
  printf("Found %d surface labels\n", nrSurfLabels);
  map<int, string> bcMapping;
  //Get the type of boundary condition
  for (int i=0; i<nrSurfLabels; i++)
  { 
    GetLine(line,f);
    sscanf(line, "%s%d", label, &id);
    pair<int, string> a(id, string(label));
    bcMapping.insert(a);
    if (find(solidPatches.begin(), solidPatches.end(), id)!=solidPatches.end())
    { printf("Found id in connectivity file that has solid block interface\n");
      pair<int, string> a(5000+id, string(label)+"-bis");
      bcMapping.insert(a);
    } 
  }


  ////////////////// Block BC /////////////////////////:
  rewind(f);
  int nrBlockLabels=0;
  while (!GetLine(line,f))
  { if (isdigit(line[0]))
    {
      if (strstr(line, "block label"))
      { sscanf(line, "%d", &nrBlockLabels);
        break;
      }
    }
  }
  vector <int> blockLabel;
  map<int, string> blockMapping;
  for (int i=0; i<nrBlockLabels; i++)
  { 
    GetLine(line,f);
    sscanf(line, "%s%d", label, &id);
    pair<int, string> a(id, string(label));
    blockMapping.insert(a);
    blockLabel.push_back(id);
  }
  int nrBlockBC=blockMapping.size();


  rewind(f);
  GetLine(line,f);
  // error in connectivity file
  int nBlockConn=atoi(line);
  if (nBlockConn!=blocks.size())
  { printf("Connectivity file is not compatible, it has %d blocks while the grid contains %d blocks\n",
           nBlockConn, blocks.size());
    exit(1);
  }

  char faceName[255], interfaceName[255];
  int index[6], nChar, b, B, neighbor[6], transform[3];
  string axisMap[6];

  char axisMap0[4];
  char axisMap1[4];
  char axisMap2[4];
  char axisMap3[4];
  char axisMap4[4];
  char axisMap5[4];

  char faceType[6];


  printf(" --> %d blocks ....\n", blocks.size());
  printf(" --> Building patches ....\n");


  
  int nrPeriodicFaces=0;
  // list with BCs that are periodic
  list <int> periodicBCs;



  // vector containing the patches
  vector <Patch*> patches;
  Patch *patch;
  int blockBC, BC;
  for(int n=0; n<blocks.size();n++)
  {
    GetLine(line,f);
    nChar=sscanf(line, "%c %d %c %d %d %s %c %d %d %s %c %d %d %s %c %d %d %s %c %d %d %s %c %d %d %s %d",
        &B,&b,
        &faceType[0],&index[0],&neighbor[0],axisMap0,
        &faceType[1],&index[1],&neighbor[1],axisMap1,
        &faceType[2],&index[2],&neighbor[2],axisMap2,
        &faceType[3],&index[3],&neighbor[3],axisMap3,
        &faceType[4],&index[4],&neighbor[4],axisMap4,
        &faceType[5],&index[5],&neighbor[5],axisMap5,
        &blockBC);
    //printf("%c %d %c %d %d %s\n", B, b, faceType[0], index[0], neighbor[0], axisMap0);
    axisMap[0]=string(axisMap0);
    axisMap[1]=string(axisMap1);
    axisMap[2]=string(axisMap2);
    axisMap[3]=string(axisMap3);
    axisMap[4]=string(axisMap4);
    axisMap[5]=string(axisMap5);
    if(nChar!=27)
    { printf("Something is wrong in conn-file.\n");
      printf("%s\n",line);
      printf("%d\n", nChar);
      exit(1);
    }
    blocks[n]->SetBC(blockBC);
    int myRange[6], neighborRange[6];
    bool is2d =  blocks[n]->GetDimension() == 2;

    for (int iface=0; iface<6; iface++)
    { bool internal = (faceType[iface]=='b' || neighbor[iface]!=0);  // an internal patch
      bool internalSurface = internal && faceType[iface]=='s'; // an internal surface
      bool periodic = (faceType[iface]=='p');
      bool solidInternal = false;
      bool boundaryInternal = false;
      short periodicSide = (index[iface]<0) ? -1: 1;

      int indexBZ=-1;
      if (internal or periodic)
      { // Internal boundary condition
        BC = abs(index[iface]);
        if (internalSurface)
        { // check if this internal surface is defined as cyclic
          if (find(cyclicPatches.begin(), cyclicPatches.end(), BC)!=cyclicPatches.end())
          { periodic=1;
          }
          if (find(solidPatches.begin(), solidPatches.end(), BC)!=solidPatches.end())
          { solidInternal=true;
          }
          for (int nBZ=0; nBZ<boundaryZonePatches.size(); nBZ++)
          { if (boundaryZonePatches[nBZ]==index[iface])
            { boundaryInternal=true;
              indexBZ=nBZ;
              break;
            }
          }
        }
        if (solidInternal)
        { if (index[iface]<0) 
          { // the 'other' side, we change the sign since the first faces need to be internal (having id -1)
            BC = 5000 + abs(BC);
          }
          patch=new Patch(BC, blocks[n], iface, NULL, "", false, 0, is2d);
	  blocks[n]->AddExternalPatch(patch);
          patches.push_back(patch);
          printf("Added patch with BC %d\n", index[iface]);
          continue;
        }

        if (boundaryInternal)
	{ blocks[n]->AddBoundaryZone(iface, indexBZ);
        }       


        int neighborBlock=neighbor[iface]-1;
        // create a new patch
	if (periodic)
        { // see if this is available in BCmappings
          if (bcMapping.count(BC))
          { map<int, string>::iterator it=bcMapping.find(0);
            it= bcMapping.find(BC);
            pair<int, string> a(BC+9000, it->second);
            bcMapping.insert(a);
          }
          BC+=9000;
          patch=new Patch(BC, blocks[n], iface, blocks[neighborBlock], axisMap[iface], periodic, periodicSide, is2d);
          blocks[n]->AddExternalPatch(patch);
          nrPeriodicFaces+=patch->NrFaces();
          periodicBCs.push_back(BC);
        } else
        { patch=new Patch(0, blocks[n], iface, blocks[neighborBlock], axisMap[iface], periodic, periodicSide, is2d);
        }
      } else
      { // External boundaries to be done
	patch=new Patch(index[iface], blocks[n], iface, NULL, "", false, 0, is2d);
	blocks[n]->AddExternalPatch(patch);
        BC = index[iface];
        if (bcMapping.count(BC) == 0)
        { 
            pair<int, string> a(BC, "Unknown");
            bcMapping.insert(a);
        }
      }

      patches.push_back(patch);
    }
  }
  fclose(f);

  // Try to open a pty file for the block BCs
  sprintf(newName, "%s.pty", argv[1]);
  f=fopen(newName, "r");

  if (f)
  { printf(" --> A property file is found, block conditions are read from %s.pty\n", argv[1]);
    printf(" --> These overwrite the block conditions in the %s.conn file\n", argv[1]);
    while (!GetLine(line,f))
    { if (strstr(line, "3D properties"))
      { sscanf(line, "%d", &nrBlockLabels);
        break;
      }
    }
    blockMapping.clear();
    blockLabel.clear();

    printf(" --> %d block labels found\n", nrBlockLabels);
    //Get the type of boundary condition
    int ibl;
    for (int i=0; i<nrBlockLabels; i++)
    { 
      GetLine(line,f);
      sscanf(line, "%d%s", &id, label);
      pair<int, string> a(id, string(label));
      blockMapping.insert(a);
      blockLabel.push_back(id);
    }
    rewind(f);
    while (!GetLine(line,f))
    { if (line[0]=='B')
      { sscanf(line+1, "%d%d", &ibl, &BC);
        blocks[ibl-1]->SetBC(BC);
      }
    }
    fclose(f);
  }

  int pointCount=0, cellCount=0;
  for (int n=0; n<blocks.size(); n++)
  { blocks[n]->CountPoints(pointCount);
  }

  printf("Total number of points before merging the nodes is %d\n", pointCount);
  // Make points unique
  printf(" --> Making matching points unique ....\n");
  for (int n=0; n<patches.size(); n++)
  { patches[n]->MakeEdgePointsUnique();
  }
  
  // Writing of Points.txt
  printf(" --> Writing points ....\n");
  pointCount=0, cellCount=0;
  for (int n=0; n<blocks.size(); n++)
  {
   // for debugging purposes to print equivalent: blocks[n]->PrintEquivalent();
   blocks[n]->CountPoints(pointCount);
   blocks[n]->CountCells(cellCount, tetMesh);
  }

  printf("Total number of points is %d\n", pointCount);
  printf("Total number of cells is %d\n",  cellCount);

  f=fopen(argv[2], "w");
  if (!f)
  { printf("Could not open: %s \n", argv[2]);
    exit(1);
  }

  fprintf(f,"$MeshFormat\n");
  fprintf(f,"2.2 0 8\n");
  fprintf(f,"$EndMeshFormat\n\n");

  fprintf(f,"$Nodes\n");
  fprintf(f,"%d\n", pointCount);
  for (int n=0; n<blocks.size(); n++)
  { blocks[n]->WritePoints(f);
  }
  fprintf(f,"$EndNodes\n");

  fprintf(f,"$Elements\n");

  fprintf(f,"%d\n", cellCount);
  id=0;
  for (int n=0; n<blocks.size(); n++)
  { blocks[n]->WriteCells(f, tetMesh, id);
  }
  printf("id=%d\n", id);
  fprintf(f,"$EndElements\n");


/*
  int nrBC = bcMapping.size();
  fprintf(f,"NMARK=  %d\n", nrBC);

  map<int, string>::const_iterator itr;
  for(itr = bcMapping.begin(); itr != bcMapping.end(); ++itr)
  { int BC =  (*itr).first;
    printf("Output faces %d (%s)\n",  (*itr).first, (*itr).second.c_str());
    fprintf(f, "MARKER_TAG= %s\n",  (*itr).second.c_str());
    int count = 0; // count the number of faces for the current BC
    for (int n=0; n<patches.size(); n++)
    { count += patches[n]->Count(BC);
    }
    if (tetMesh)
    { count*=2;
    }
    fprintf(f, "MARKER_ELEMS= %d\n", count);
    for (int n=0; n<patches.size(); n++)
    { patches[n]->Write(f, BC, tetMesh);
    }
  }
*/

  fclose(f);
}
