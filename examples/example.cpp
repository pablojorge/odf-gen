/*
odf-gen: Simple API to generate OpenDocument documents.
    Copyright (C) 2009  Pablo Jorge, FuDePAN

    This file is part of the odf-gen project.

    odf-gen is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    odf-gen is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with odf-gen.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <iostream>

#include <odfgen/odfgen.h>

// using ODSGenerator directly
void test1() 
{
    ODSGenerator output;
 
    output.begin_spreadsheet();
    output.begin_sheet( "test" );
    
    output.begin_row();
    output.add_cell( "x" );
    output.add_cell( "x * 2" );
    output.end_row();
 
    for( int i = 0; i < 5; i++ )
    {
        output.begin_row();
        output.add_cell( i );
        output.add_cell( i * 2 );
        output.end_row();
    }

    output.end_sheet();
    output.end_spreadsheet();
}

// using the wrappers
void test2() 
{
    Spreadsheet spreadsheet;

    Sheet sheet1( spreadsheet, "sheet1" );

    Row( sheet1 ) << column_span( "functions", 4 );
    
    Style border_bottom( Style::BORDER_BOTTOM );

    Row( sheet1 ) << border_bottom << "x"
                  << border_bottom << "f(x) = x ^ 2" 
                  << border_bottom << "f(x) = 2x" 
                  << border_bottom << "f(x) = -5x";
 
    for( int x = -10; x <= 10; x++ )
        Row( sheet1 ) << x << separator() << x * x << x * 2 << x * -5;

    Row( sheet1 ) << AutoChart( "chart1",        // name
                                Centimeters(14), // width
                                Centimeters(10), // height
                                sheet1 );        // sheet to extract series from
}

int main( int argc, char const* argv[] )
{
    test2();
    return 0;
}
