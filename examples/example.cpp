/*
ods_generator: Simple API to generate a ods-like xml file.
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

#include "include/ods_generator.h"

// using ODSGenerator directly
void test1() 
{
    ODSGenerator output( std::cout );
 
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
    Spreadsheet spreadsheet( std::cout );

    Sheet sheet1( spreadsheet, "sheet1" );
    
    Row( sheet1 ) << "x" << "f(x) = x ^ 2";
 
    for( int i = 0; i < 5; i++ )
        Row( sheet1 ) << i << i * i;

    Chart chart1( "chart1",
                  "8cm",
                  "8cm",
                  CellRange( CellAddress( "sheet1", 1, 2 ),
                             CellAddress( "sheet1", 2, 6 ) ) );

    chart1.add_series( Series( CellAddress( "sheet1", 2, 1 ), 
                               CellRange( CellAddress( "sheet1", 1, 2 ),
                                          CellAddress( "sheet1", 1, 6 ) ),
                               CellRange( CellAddress( "sheet1", 2, 2 ),
                                          CellAddress( "sheet1", 2, 6 ) ) ) );

    Row( sheet1 ) << chart1;
}

int main(int argc, char const* argv[])
{
    test2();
    return 0;
}
