// odf-gen: Simple API to generate OpenDocument documents.
//
//          Copyright Pablo Jorge 2009 - 2013.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
//

#pragma once

#include <iostream>

#include "odstype.h"

#include "element.h"
#include "sheet.h"

#include "celladdress.h"
#include "style.h"
#include "span.h"

class Row : public Element
{
public:
    Row( Sheet &sheet )
        : Element( sheet ),
          _sheet( sheet ),
          _column( 0 ),
          _row( 0 )
    {
        _sheet.add_row();
        _row = _sheet.get_rows();
        _ostream << "<row>";
    }
    
    virtual void close_()
    {
        if( !!_style )
            add_cell( "" );

        _ostream << "</row>";
    }

    ~Row()
    {
        close();
    }
    
    template < class T >
    void begin_cell( const Style& style,
                     unsigned int column_span,
                     unsigned int row_span )
    {
        _ostream << "<cell style=\"" << style << "\"" 
                 << "      column-span=\"" << column_span << "\""
                 << "      row-span=\"" << row_span << "\""
                 << "      type=\"" << ODSType< T >::convert() << "\">";
    }

    void end_cell() 
    {
        _ostream << "</cell>";
    }

    template < class T >
    void add_value( const T& value )
    {
        _ostream << value;
    }

    template < class T >
    CellAddress add_cell( const T& value,
                          unsigned int column_span,
                          unsigned int row_span )
    {
        // add the cell using the preset style
        begin_cell< T >( _style, column_span, row_span );
        add_value( value );
        end_cell();

        // reset style
        _style = Style::NONE;
        
        // increment current column count
        ++_column;
        
        // capture current cell address
        CellAddress address( _sheet.get_name(),
                             _column,
                             _row );

        // adjust (if necessary) the columns count
        _column += column_span ? column_span - 1 : 0;
        
        // if current column count > maximum column count
        // for all the rows in this sheet, update sheet's
        // column count
        if( _column > _sheet.get_columns() )
            _sheet.add_column();

        // return a valid CellAddress
        return address;
    }

    template < class T >
    CellAddress add_cell( const T& value )
    {
        return add_cell( value, 0, 0 );
    }
    
    void add_style( const Style& style )
    {
        _style |= style;
    }

    template < class T >
    Row& operator << ( const T& value )
    {
        add_cell( value );
        return *this;
    }

    Row& operator << ( const Style& style )
    {
        add_style( style );
        return *this;
    }
        
    template < class T >
    Row& operator << ( const ColumnSpan< T >& spanned_value )
    {
        add_cell( spanned_value.value(),
                  spanned_value.count(),
                  0 );
        return *this;
    }

    template < class T >
    Row& operator << ( const RowSpan< T >& spanned_value )
    {
        add_cell( spanned_value.value(),
                  0,
                  spanned_value.count() );
        return *this;
    }

    Row& operator << ( const CoveredCell& covered )
    {
        _ostream << "<cell covered=\"true\"/>";
        return *this;
    }

private:
    Sheet &_sheet;

    Style _style;
    unsigned int _column,
                 _row;
};

