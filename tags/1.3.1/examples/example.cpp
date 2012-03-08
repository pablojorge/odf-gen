/*
odf-gen: Simple API to generate OpenDocument documents.
    Copyright (C) 2012  Pablo Jorge, FuDePAN

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

/*  As a special exception, you may create a larger work that contains
    part of all of the odf-gen project and distribute that work under
    the terms of the GNU General Public License as published by the
    Free Software Foundation; version 2 of the License

    Alternatively, if you modify or redistribute the ODF generator
    itself, you may (ar your option) remove this special exception

    This special exception was added by Pablo Jorge in 
    version 1.3 of odf-gen. 
*/

#include <iostream>

#include "odfgen.h"

void test() 
{
    Spreadsheet spreadsheet;

    Sheet sheet1( spreadsheet, "sheet1" );

    const Style centered( Style::ALIGN_CENTER ),
                bold( Style::TEXT_BOLD ),
                title_style( centered | bold ),
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
    test();
    return 0;
}
