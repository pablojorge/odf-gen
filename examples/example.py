# odf-gen: Simple API to generate OpenDocument documents.
# Copyright (C) 2009  Pablo Jorge, FuDePAN
#
# This file is part of the odf-gen project.
#
# odf-gen is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# odf-gen is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with odf-gen.  If not, see <http://www.gnu.org/licenses/>.

import sys

from odfgen.odsgenerator import *

# using ODSGenerator directly
def test1():
    stream = StringStream()

    output = ODSGenerator(stream)
 
    output.begin_spreadsheet();
    output.begin_sheet( "test" );
    
    output.begin_row();
    output.add_cell( "x" );
    output.add_cell( "x * 2" );
    output.end_row();
 
    for i in range( 5 ):
        output.begin_row();
        output.add_cell( i );
        output.add_cell( i * 2 );
        output.end_row();

    output.end_sheet();
    output.end_spreadsheet();

    print stream.str()

# using the wrappers
def test2():
    stream = StringStream()

    with Spreadsheet( stream ) as spreadsheet:
        with Sheet( spreadsheet, "sheet1" ) as sheet1:
            sheet1.append_row(["x", "x * 2"])

            for i in range( 5 ):
                sheet1.append_row([i, i * 2])

            sheet1.append_row([AutoChart("chart1", 
                                         Centimeters(8), 
                                         Centimeters(8), 
                                         sheet1)])

    print stream.str()

if __name__ == "__main__":
    test2()
