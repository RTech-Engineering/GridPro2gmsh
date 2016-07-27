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


    #include <iostream>
    #include <set>

    using namespace std;

    int main()
    {
        string
            sa[] =
            {
                "alpha",
                "echo",
                "hotel",
                "mike",
                "romeo"
            };

        multiset<string>
            object(&sa[0], &sa[5]);

        object.insert("echo");
        object.insert("echo");

        multiset<string>::iterator
            it = object.find("echo");

        for (; it != object.end(); ++it)
            cout << *it << " ";
        cout << endl;

        cout << "Multiset::equal_range(\"ech\")\n";
        pair
        <
            multiset<string>::iterator,
            multiset<string>::iterator
        >
            itpair = object.equal_range("ech");

        if (itpair.first != object.end())
            cout << "lower_bound() points at " << *itpair.first << endl;
        for (; itpair.first != itpair.second; ++itpair.first)
            cout << *itpair.first << " ";

        cout << endl <<
                object.count("ech") << " occurrences of 'ech'" << endl;

        cout << "Multiset::equal_range(\"echo\")\n";
        itpair = object.equal_range("echo");

        for (; itpair.first != itpair.second; ++itpair.first)
            cout << *itpair.first << " ";

        cout << endl <<
                object.count("echo") << " occurrences of 'echo'" << endl;

        cout << "Multiset::equal_range(\"echoo\")\n";
        itpair = object.equal_range("echoo");

        for (; itpair.first != itpair.second; ++itpair.first)
            cout << *itpair.first << " ";

        cout << endl <<
                object.count("echoo") << " occurrences of 'echoo'" << endl;
    }
    /*
        Generated output:

        echo echo echo hotel mike romeo
        Multiset::equal_range("ech")
        lower_bound() points at echo

        0 occurrences of 'ech'
        Multiset::equal_range("echo")
        echo echo echo
        3 occurrences of 'echo'
        Multiset::equal_range("echoo")

        0 occurrences of 'echoo'
    */


