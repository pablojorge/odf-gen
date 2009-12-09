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

class Element
{
public:
    Element( std::ostream &ostream = std::cout )
        : _ostream( ostream ),
          _closed( false )
    {}

    Element( Element& other )
        : _ostream( other._ostream ),
          _closed( false )
    {}
    
    virtual ~Element()
    {}

    void close() {
        if( !_closed ) {
            this->close_();
            _closed = true;
        }
    }

protected:
    virtual void close_() = 0; 
    
protected:
    std::ostream &_ostream;
private:
    bool _closed;
};

class Spreadsheet : public Element
{
public:
    Spreadsheet( std::ostream &ostream = std::cout )
        : Element( ostream )
    {
        _ostream << "<?xml version=\"1.0\"?>"
                 << "<spreadsheet>";
    }
    
    virtual void close_()
    {
        _ostream << "</spreadsheet>";
    }
    
    ~Spreadsheet()
    {
        close();
    }
};

class Sheet : public Element
{
public:
    Sheet( Spreadsheet &spreadsheet, 
           const std::string &name )
        : Element( spreadsheet ),
          _name( name ),
          _columns( 0 ),
          _rows( 0 )
    {
        _ostream << "<sheet name=\"" << _name << "\">";
    }
    
    virtual void close_()
    {
        _ostream << "</sheet>";
    }
    
    ~Sheet()
    {
        close();
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
    std::string _name;

    unsigned int _columns,
                 _rows;
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

    std::ostream& operator << ( std::ostream& ostream ) const
    {
        ostream << "'" << _sheet << "'" 
                << "." << (char)('A' + _column - 1) // XXX
                << _row;
        return ostream;
    }

private:
    std::string _sheet;
    unsigned int _column,
                 _row;
};

inline
std::ostream& operator << ( std::ostream &ostream,
                            const CellAddress& address ) 
{
    return address.operator << (ostream);
}

class CellRange 
{
public:
    CellRange( const CellAddress &start,
               const CellAddress &end )
        : _start( start ),
          _end( end )
    {}

    std::ostream& operator << ( std::ostream& ostream ) const
    {
        ostream << _start << ":" << _end;
        return ostream;
    }

private:
    CellAddress _start,
                _end;
};

inline 
std::ostream& operator << ( std::ostream &ostream,
                            const CellRange& range ) 
{
    return range.operator << (ostream);
}

class Style
{
public:
    enum StyleFlags {
        NONE           = 0,
        BORDER_BOTTOM  = (1 <<  0),
        BORDER_LEFT    = (1 <<  1),
        BORDER_RIGHT   = (1 <<  2),
        BORDER_TOP     = (1 <<  3),
        ALIGN_LEFT     = (1 <<  4),
        ALIGN_CENTER   = (1 <<  5),
        ALIGN_RIGHT    = (1 <<  6),
        TEXT_BOLD      = (1 <<  7),
        TEXT_ITALIC    = (1 <<  8),
        TEXT_UNDERLINE = (1 <<  9),
    };

    Style( mili::bitwise_enum< StyleFlags > flags = NONE ) 
        : _flags( flags )
    {}

    Style& operator = ( mili::bitwise_enum< StyleFlags > flags )
    {
        _flags = flags;
        return *this;
    }

    Style& operator |= ( const Style& other  )
    {
        _flags |= other._flags;
        return *this;
    }

    bool operator ! () const
    {
        return !_flags.has_bits();
    }

    bool operator & ( mili::bitwise_enum< StyleFlags > flags ) const
    {
        return (_flags & flags).has_bits();
    }

    std::ostream& operator << ( std::ostream& ostream ) const
    {
        if( this->operator & (BORDER_BOTTOM) )
            ostream << "border-bottom,";
        if( this->operator & (BORDER_LEFT) )
            ostream << "border-left,";
        if( this->operator & (BORDER_RIGHT) )
            ostream << "border-right,";
        if( this->operator & (BORDER_TOP) )
            ostream << "border-top,";
        if( this->operator & (ALIGN_LEFT) )
            ostream << "align-left,";
        if( this->operator & (ALIGN_CENTER) )
            ostream << "align-center,";
        if( this->operator & (ALIGN_RIGHT) )
            ostream << "align-right,";
        if( this->operator & (TEXT_BOLD) )
            ostream << "text-bold,";
        if( this->operator & (TEXT_ITALIC) )
            ostream << "text-italic,";
        if( this->operator & (TEXT_UNDERLINE)  )
            ostream << "text-underline,";
        
        return ostream;
    }

private:
    mili::bitwise_enum< StyleFlags > _flags;
};

inline
std::ostream& operator << ( std::ostream &ostream,
                            const Style& style ) 
{
    return style.operator << (ostream);
}

template < class K, class T >
class Span
{
public:
    Span( const T& value, unsigned int count )
        : _value( value ),
          _count( count )
    {}

    const T& value() const { return _value; }
    unsigned int count() const { return _count; }
    
private:
    T _value;
    unsigned int _count;
};

class ColumnK {};
class RowK {};

template < class T >
class ColumnSpan : public Span< ColumnK, T >
{
public:
    ColumnSpan( const T& value, unsigned int count )
        : Span< ColumnK, T >( value, count )
    {}
};

template < class T >
class RowSpan : public Span< RowK, T >
{
public:
    RowSpan( const T& value, unsigned int count )
        : Span< RowK, T >( value, count )
    {}
};

class CoveredCell {};
static const CoveredCell covered_cell = CoveredCell();

template < class T >
inline ColumnSpan< T > column_span( T value, unsigned int count )
{
    return ColumnSpan< T >( value, count );
}

template < class T >
inline RowSpan< T > row_span( T value, unsigned int count )
{
    return RowSpan< T >( value, count );
}

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
                  spanned_value.count() );
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

static const Style v_separator( Style::BORDER_LEFT );

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

    std::ostream& operator << ( std::ostream& ostream ) const
    {
        ostream << "#" 
                << std::hex 
                << std::setfill('0') 
                << std::setw(2) << static_cast< int >( _red )
                << std::setw(2) << static_cast< int >( _green )
                << std::setw(2) << static_cast< int >( _blue )
                << std::dec;
        return ostream;
    }

private:
    unsigned char _red,
                  _green,
                  _blue;
};

inline
std::ostream& operator << ( std::ostream &ostream, const Color& color ) 
{
    return color.operator << (ostream);
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

    std::ostream& operator << ( std::ostream& ostream ) const
    {
        ostream << "<series name-address=\"" << _name << "\""
                << "        x-range=\"" << _domain << "\""
                << "        y-range=\"" << _values << "\""
                << "        color=\"" << _color << "\""
                << "/>";
        return ostream;
    }

private:
    CellAddress _name;
    CellRange _domain,
              _values;
    Color _color;
};

inline
std::ostream& operator << ( std::ostream &ostream,
                            const Series& series ) 
{
    return series.operator << (ostream);
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

    std::ostream& operator << ( std::ostream& ostream ) const
    {
        ostream << "<chart name=\"" << _name << "\""
                << "       width=\"" << _width << "\""
                << "       height=\"" << _height << "\""
                << "       title=\"" << _title << "\""
                << "       subtitle=\"" << _subtitle << "\""
                << "       x-axis-label=\"" << _x_axis_label << "\""
                << "       y-axis-label=\"" << _y_axis_label << "\""
                << "       range=\"";
        
        for( std::list< CellRange >::const_iterator 
             it  = _range_list.begin();
             it != _range_list.end();
             it++ )
            ostream << *it << ";";
        
        ostream << "\">";
        
        for( std::list< Series >::const_iterator 
             it  = _series_list.begin();
             it != _series_list.end();
             it++ )
            ostream << *it;
        
        ostream << "</chart>";
        
        return ostream;
    }

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

inline
std::ostream& operator << ( std::ostream &ostream,
                            const Chart& chart ) 
{
    return chart.operator << (ostream);
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
