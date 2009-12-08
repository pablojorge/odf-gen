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

    const Style title_style( (Style::StyleFlags)(Style::ALIGN_CENTER | 
                                                 Style::TEXT_BOLD) ),
                italic( Style::TEXT_ITALIC ),
                border_bottom( Style::BORDER_BOTTOM );

    // main title
    Row( sheet1 ) << title_style << border_bottom << row_span( "x", 2 )
                  << title_style << italic << column_span( "Functions", 3 );
 
    // column titles
    Row( sheet1 ) << covered_cell << v_separator
                  << title_style << border_bottom << "f(x) = x ^ 2" 
                  << title_style << border_bottom << "f(x) = 2x" 
                  << title_style << border_bottom << "f(x) = -5x";
 
    // column values
    for( int x = -10; x <= 10; x++ )
        Row( sheet1 ) << v_separator
                      << x << v_separator << x * x << x * 2 << x * -5
                      << v_separator;

    // chart
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
