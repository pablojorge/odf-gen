// odf-gen: Simple API to generate OpenDocument documents.
//
//          Copyright Pablo Jorge 2009 - 2013.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
//

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
