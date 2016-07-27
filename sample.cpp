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


    #include <iostream>
    #include <list>
    using namespace std;

    // The List STL template requires overloading operators =, == and <.

    class AAA
    {
       friend ostream &operator<<(ostream &, const AAA &);

       public:
          int x;
          int y;
          float z;

          AAA();
          AAA(const AAA &);
          ~AAA(){};
          //AAA &operator=(const AAA &rhs);
          //int operator==(const AAA &rhs) const;
          int operator<(const AAA &rhs) const;
    };

    AAA::AAA()   // Constructor
    {
       x = 0;
       y = 0;
       z = 0;
    }

    AAA::AAA(const AAA &copyin)   // Copy constructor to handle pass by value.
    {                             
       x = copyin.x;
       y = copyin.y;
       z = copyin.z;
    }

    ostream &operator<<(ostream &output, const AAA &aaa)
    {
       output << aaa.x << ' ' << aaa.y << ' ' << aaa.z << endl;
       return output;
    }

/*
    AAA& AAA::operator=(const AAA &rhs)
    {
       this->x = rhs.x;
       this->y = rhs.y;
       this->z = rhs.z;
       return *this;
    }
    */

/*
    int AAA::operator==(const AAA &rhs) const
    {
       if( this->x != rhs.x) return 0;
       if( this->y != rhs.y) return 0;
       if( this->z != rhs.z) return 0;
       return 1;
    }
    */

    // This function is required for built-in STL list functions like sort
    int AAA::operator<(const AAA &rhs) const
    {  printf("Compare\n");
       if( this->x == rhs.x && this->y == rhs.y && this->z < rhs.z) return 1;
       if( this->x == rhs.x && this->y < rhs.y) return 1;
       if( this->x < rhs.x ) return 1;
       return 0;
    }

    main()
    {
       list<AAA> L;
       AAA Ablob ;

       Ablob.x=7;
       Ablob.y=2;
       Ablob.z=4.2355;
       L.push_back(Ablob);  // Insert a new element at the end

       Ablob.x=5;
       L.push_back(Ablob);  // Object passed by value. Uses default member-wise
                            // copy constructor
       Ablob.z=3.2355;
       L.push_back(Ablob); 

       Ablob.x=3;
       Ablob.y=7;
       Ablob.z=7.2355;
       L.push_back(Ablob); 

       list<AAA>::iterator i;

       cout << endl;      
       for(i=L.begin(); i != L.end(); ++i) cout << *i << " "; // print all
       cout << endl;

       cout << "Sorted: " << endl;
       L.sort();
       for(i=L.begin(); i != L.end(); ++i) cout << *i << " "; // print all
       cout << endl;

       return 0;
    }


/*
Output:

    7 5 5 3 
    7 2 4.2355
     5 2 4.2355
     5 2 3.2355
     3 7 7.2355
     
    Sorted:
    3 7 7.2355
     5 2 3.2355
     5 2 4.2355
     7 2 4.2355
*/
