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


#ifndef TIMER_HPP
#define TIMER_HPP

#include <time.h>
#include <sys/times.h>
#include <sys/param.h>
#include <sys/time.h>

class RTechTimer
{ private:
    double cpu_start, cpu_end;
    double wall_start, wall_end;
    double accumulator_cpu, accumulator_wall;
    int started, stopped;
  public:
    RTechTimer();
    void Reset();
    void Restart();
    void Start();
    void Stop();
    double CPU_sec();
    double CPU_microsec();
    double WALL_microsec();
};
#endif
