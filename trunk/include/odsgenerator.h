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

#ifndef ODSGENERATOR_H
#define ODSGENERATOR_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <list>
#include <cstdlib>

#define MILI_NAMESPACE
#include <mili/mili.h>

template <class T>
struct ODSType
{
    static const char* convert() { return "float"; }
};

template <class T>
struct ODSType<const T&> : public ODSType<T>
{};

#define MAP_ODS_TYPE(cpptype, odstype) \
template<> \
struct ODSType<cpptype> \
{ \
    static const char* convert() { return #odstype; } \
};

MAP_ODS_TYPE(const char*, string);
MAP_ODS_TYPE(std::string, string);

template <int N>
struct ODSType<char[N]>
{
    static const char* convert() { return "string"; }
};

class Style;

std::ostream& operator << ( std::ostream &ostream,
                            const Style& style );

class ODSGenerator 
{
public:
    ODSGenerator( std::ostream &ostream = std::cout ) 
        : _ostream( ostream )
    {
        _ostream << "<?xml version=\"1.0\"?>";
    }

    void begin_spreadsheet()
    {
        _ostream << "<spreadsheet>";
    }

    void end_spreadsheet()
    {
        _ostream << "</spreadsheet>";
    }
        
    void begin_sheet( const std::string &name )
    {
        _ostream << "<sheet name=\"" << name << "\">";
    }
    
    void end_sheet() 
    {
        _ostream << "</sheet>";
    }
 
    void begin_row()
    {
        _ostream << "<row>";
    }
    
    void end_row() 
    {
        _ostream << "</row>";
    }

    template < class T >
    void begin_cell( const Style& style,
                     unsigned int column_span )
    {
        _ostream << "<cell style=\"" << style << "\"" 
                 << "      column-span=\"" << column_span << "\""
                 << "      type=\"" << ODSType< T >::convert() << "\">";
    }

    template < class T >
    void begin_cell( const Style& style )
    {
        begin_cell< T >( style, 0 );
    }

    template < class T >
    void begin_cell()
    {
        begin_cell< T >( Style(), 0 );
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
    void add_cell( const T& value,
                   const Style& style,
                   unsigned int column_span )
    {
        begin_cell< T >( style, column_span );
        add_value( value );
        end_cell();
    }

    template < class T >
    void add_cell( const T& value,
                   const Style& style )
    {
        add_cell( value, style, 0 );
    }
        
    template < class T >
    void add_cell( const T& value )
    {
        add_cell( value, Style(), 0 );
    }
        
private:
    std::ostream &_ostream;
};

template < class Container >
class TagHandler
{
public:
    TagHandler( Container& container ) 
        : _container( container ),
          _closed( false )
    {
        open();
    }
    
    ~TagHandler()
    {
        close();
    } 
    
    void open() {
        _container.open_();
    }
    
    void close() {
        if( !_closed ) {
            _container.close_();
            _closed = true;
        }
    }
    
private:
    Container& _container;
    bool _closed;
};

class Spreadsheet 
{
public:
    Spreadsheet( std::ostream &ostream = std::cout )
        : _generator( ostream ),
          _handler( *this )
    {}
    
    void open_()
    {
        _generator.begin_spreadsheet();
    }
    
    void close_()
    {
        _generator.end_spreadsheet();
    }
    
    void close()
    {
        _handler.close();
    }

    ODSGenerator &generator()
    {
        return _generator;
    }
    
private:
    ODSGenerator _generator;
    TagHandler< Spreadsheet > _handler;
};

class Sheet 
{
public:
    Sheet( Spreadsheet &spreadsheet, 
           const std::string &name )
        : _generator( spreadsheet.generator() ),
          _name( name ),
          _columns( 0 ),
          _rows( 0 ),
          _handler( *this )
    {}
    
    void open_()
    {
        _generator.begin_sheet( _name );
    }
    
    void close_()
    {
        _generator.end_sheet();
    }

    void close()
    {
        _handler.close();
    }

    ODSGenerator &generator()
    {
        return _generator;
    }

    const char* get_name() const
    {
        return _name.c_str();
    }

    unsigned int get_columns() const
    {
        return _columns;
    }
    
    unsigned int get_rows() const
    {
        return _rows;
    }

    void add_column()
    {
        ++_columns;
    }
    
    void add_row()
    {
        ++_rows;
    }
    
private:
    ODSGenerator &_generator;
    std::string _name;

    unsigned int _columns,
                 _rows;

    TagHandler< Sheet > _handler;
};

class CellAddress 
{
public:
    CellAddress( const std::string &sheet,
                 unsigned int column,
                 unsigned int row )
        : _sheet( sheet ),
          _column( column ),
          _row( row )
    {}

    friend std::ostream& operator << ( std::ostream&, const CellAddress& );

private:
    std::string _sheet;
    unsigned int _column,
                 _row;
};

std::ostream& operator << ( std::ostream &ostream,
                            const CellAddress& address ) 
{
    ostream << "'" << address._sheet << "'" 
            << "." << (char)('A' + address._column - 1) // XXX
            << address._row;
    return ostream;
}

class CellRange 
{
public:
    CellRange( const CellAddress &start,
               const CellAddress &end )
        : _start( start ),
          _end( end )
    {}

    friend std::ostream& operator << ( std::ostream&, const CellRange& );

private:
    CellAddress _start,
                _end;
};

std::ostream& operator << ( std::ostream &ostream,
                            const CellRange& range ) 
{
    ostream << range._start << ":" << range._end;
    return ostream;
}

class Style
{
public:
    typedef enum {
        NONE          = 0x0000,
        BORDER_BOTTOM = 0x0001,
        BORDER_LEFT   = 0x0002,
        BORDER_RIGHT  = 0x0004,
        BORDER_TOP    = 0x0008
    } StyleFlags ;

    Style( StyleFlags flags = NONE ) 
        : _flags( flags )
    {}

    Style& operator = ( StyleFlags flags )
    {
        _flags = flags;
        return *this;
    }

    Style& operator |= ( const Style& other  )
    {
        _flags |= other._flags;
        return *this;
    }

    friend std::ostream& operator << ( std::ostream&, const Style& );

private:
    mili::bitwise_enum< StyleFlags > _flags;
};

std::ostream& operator << ( std::ostream &ostream,
                            const Style& style ) 
{
    if( (style._flags & style.BORDER_BOTTOM).has_bits() )
        ostream << "border-bottom,";
    if( (style._flags & style.BORDER_LEFT).has_bits() )
        ostream << "border-left,";
    if( (style._flags & style.BORDER_RIGHT).has_bits() )
        ostream << "border-right,";
    if( (style._flags & style.BORDER_TOP).has_bits() )
        ostream << "border-top,";
    return ostream;
}

template < class T >
class ColumnSpan
{
public:
    ColumnSpan( const T& value, unsigned int count )
        : _value( value ),
          _count( count )
    {}

    const T& value() const { return _value; }
    unsigned int count() const { return _count; }
    
private:
    T _value;
    unsigned int _count;
};

template < class T >
inline ColumnSpan< T > column_span( T value, unsigned int count )
{
    return ColumnSpan< T >( value, count );
}

class Row 
{
public:
    Row( Sheet &sheet )
        : _sheet( sheet ),
          _generator( _sheet.generator() ),
          _column( 0 ),
          _row( 0 ),
          _handler( *this )
    {}
    
    void open_()
    {
        _sheet.add_row();
        _row = _sheet.get_rows();
        _generator.begin_row();
    }
    
    void close_()
    {
        _generator.end_row();
    }
    
    void close()
    {
        _handler.close();
    }

    ODSGenerator &generator()
    {
        return _generator;
    }
    
    template < class T >
    CellAddress add_cell( const T& value,
                          unsigned int column_span )
    {
        // add the cell using the preset style
        _generator.add_cell( value, _style, column_span );

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
        return add_cell( value, 0 );
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
                  spanned_value.count() );
        return *this;
    }

private:
    Sheet &_sheet;
    ODSGenerator &_generator;

    Style _style;
    unsigned int _column,
                 _row;

    TagHandler< Row > _handler;
};

inline Style separator() { return Style( Style::BORDER_LEFT ); }

class Color
{
public:
    Color( unsigned char red,
           unsigned char green,
           unsigned char blue )
        : _red( red ),
          _green( green ),
          _blue( blue )
    {}

    Color( unsigned int color )
        : _red( color >> 16 ),
          _green( color >> 8 ),
          _blue( color )
    {}

    friend std::ostream& operator << ( std::ostream&, const Color& );

private:
    unsigned char _red,
                  _green,
                  _blue;
};

std::ostream& operator << ( std::ostream &ostream, const Color& color ) 
{
    ostream << "#" 
            << std::hex 
            << std::setfill('0') 
            << std::setw(2) << static_cast< int >( color._red )
            << std::setw(2) << static_cast< int >( color._green )
            << std::setw(2) << static_cast< int >( color._blue )
            << std::dec;
    return ostream;
}

class ColorGenerator
{
public:
    ColorGenerator()
        : _idx( 0 )
    {
        srandom( time( NULL ) );
    }
    
    Color next()
    {
        static unsigned int palette[] = { 0x0000ff, 0xff0000, 0x00ff00,
                                          0xff00ff, 0xffff00, 0x00ffff };
        
        if( _idx < sizeof(palette) / sizeof(palette[0]) )
            return Color( palette[_idx++] );
            
        return Color( random() );
    }

private:
    int _idx;
};

class Length
{
public:
    Length( unsigned int value )
        : _value( value )
    {}
    
    virtual const char* unit() const
    {
        return "";
    }
    
    std::string str() const
    {
        std::stringstream stream;
        stream << _value << this->unit();
        return stream.str();
    }

private:
    unsigned int _value;
};

class Centimeters : public Length
{
public:
    Centimeters( unsigned int value )
        : Length( value )
    {}
    
    const char* unit() const
    {
        return "cm";
    }
};

class Series 
{
public:
    Series( const CellAddress& name,
            const CellRange& domain,
            const CellRange& values,
            const Color& color )
        : _name( name ),
          _domain( domain ),
          _values( values ),
          _color( color )
    {}

    friend std::ostream& operator << ( std::ostream&, const Series& );

private:
    CellAddress _name;
    CellRange _domain,
              _values;
    Color _color;
};

std::ostream& operator << ( std::ostream &ostream,
                            const Series& series ) 
{
    ostream << "<series name-address=\"" << series._name << "\""
            << "        x-range=\"" << series._domain << "\""
            << "        y-range=\"" << series._values << "\""
            << "        color=\"" << series._color << "\""
            << "/>";
    return ostream;
}

class Chart 
{
public:
    Chart( const std::string &name,
           const Length &width,
           const Length &height )
        : _name( name ),
          _width( width.str() ),
          _height( height.str() )
    {}

    void add_range( const CellRange &range )
    {
        _range_list.push_back( range );
    }

    void add_series( const Series &series )
    {
        _series_list.push_back( series );
    }
    
    void set_title( const std::string& title )
    {
        _title = title;
    }

    void set_subtitle( const std::string& subtitle )
    {
        _subtitle = subtitle;
    }

    void set_x_axis_label( const std::string& x_axis_label )
    {
        _x_axis_label = x_axis_label;
    }

    void set_y_axis_label( const std::string& y_axis_label )
    {
        _y_axis_label = y_axis_label;
    }

    friend std::ostream& operator << ( std::ostream&, const Chart& );

private:
    std::string _name,
                _width,
                _height,
                _title,
                _subtitle,
                _x_axis_label,
                _y_axis_label;
    std::list< CellRange > _range_list;
    std::list< Series > _series_list;
};

MAP_ODS_TYPE(Chart, object);

std::ostream& operator << ( std::ostream &ostream,
                            const Chart& chart ) 
{
    ostream << "<chart name=\"" << chart._name << "\""
            << "       width=\"" << chart._width << "\""
            << "       height=\"" << chart._height << "\""
            << "       title=\"" << chart._title << "\""
            << "       subtitle=\"" << chart._subtitle << "\""
            << "       x-axis-label=\"" << chart._x_axis_label << "\""
            << "       y-axis-label=\"" << chart._y_axis_label << "\""
            << "       range=\"";

    for( std::list< CellRange >::const_iterator 
         it  = chart._range_list.begin();
         it != chart._range_list.end();
         it++ )
        ostream << *it << ";";

    ostream << "\">";

    for( std::list< Series >::const_iterator 
         it  = chart._series_list.begin();
         it != chart._series_list.end();
         it++ )
        ostream << *it;

    ostream << "</chart>";
    
    return ostream;
}

class AutoChart : public Chart
{
public:
    AutoChart( const std::string &name,
               const Length &width,
               const Length &height,
               const Sheet &sheet )
        : Chart( name, 
                 width, 
                 height ) 
    {
        ColorGenerator generator;

        add_range( CellRange( CellAddress( sheet.get_name(), 1, 1 ),
                              CellAddress( sheet.get_name(),
                                           sheet.get_columns(),
                                           sheet.get_rows() ) ) );

        for( unsigned int i = 2; i <= sheet.get_columns(); i++ ) 
        {
            CellAddress name( sheet.get_name(), i, 1 ),
                        domain_start( sheet.get_name(), 1, 2 ),
                        domain_end( sheet.get_name(), 1, sheet.get_rows() ),
                        values_start( sheet.get_name(), i, 2 ),
                        values_end( sheet.get_name(), i, sheet.get_rows() );
            CellRange domain( domain_start, domain_end ),
                      values( values_start, values_end );
            Color color( generator.next() );

            add_series( Series( name, domain, values, color ) );
        }
    }
};

MAP_ODS_TYPE(AutoChart, object);

#endif // ODSGENERATOR_H
