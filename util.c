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
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

void s_skip_line(FILE *fp)
{ int ch;
  do
  { ch=getc(fp);
  } while (ch!=EOF && ch!='\n');
  ungetc(ch,fp);
}

int s_skipblanks(FILE *fp)
{ int i=0, ch;
  do
  { ch=getc(fp);
    i++;
  } while (isspace(ch));
  ungetc(ch,fp);
  return ch;
}


int s_search_in_file_ncs(FILE *f, char *s)
/* Not case sensitive */
{ int l=0, ch;
  int is, ilen, found=0;

  ilen=strlen(s);
  do
  { ch=getc(f);
    is=0;
    while (tolower(ch)==tolower(s[is]))
    { ch=getc(f);
      if (ch==EOF) break;
      is++;
      if (is==ilen)
      { found=1;
        break;
      }
    }
  } while (!found && ch!=EOF);
  ungetc(ch,f);
  return found;
}


int s_search_in_file(FILE *f, char *s)
{ int l=0, ch;
  int is, ilen, found=0;

  ilen=strlen(s);
  do
  { ch=getc(f);
    is=0;
    while (ch==s[is])
    { ch=getc(f);
      if (ch==EOF) break;
      is++;
      if (is==ilen)
      { found=1;
        break;
      }
    }
  } while (!found && ch!=EOF);
  ungetc(ch,f);
  return found;
}
