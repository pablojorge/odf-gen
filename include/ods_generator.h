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

#ifndef ODS_GENERATOR_H
#define ODS_GENERATOR_H

#include <iostream>
#include <iomanip>
#include <list>
#include <cstdlib>

template <class T>
struct ODSType
{};

#define MAP_ODS_TYPE(cpptype, odstype) \
template<> \
struct ODSType<cpptype> \
{ \
    static const char* convert() { return #odstype; } \
};

MAP_ODS_TYPE(double, float);
MAP_ODS_TYPE(float, float);
MAP_ODS_TYPE(int, float);

MAP_ODS_TYPE(const char*, string);
MAP_ODS_TYPE(std::string, string);

template <int N>
struct ODSType<char[N]>
{
    static const char* convert() { return "string"; }
};

class ODSGenerator 
{
public:
    ODSGenerator( std::ostream &ostream ) 
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
    void begin_cell()
    {
        _ostream << "<cell type=\"" << ODSType< T >::convert() << "\">";
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
    void add_cell( const T& value )
    {
        begin_cell< T >();
        add_value( value );
        end_cell();
    }
        
private:
    std::ostream &_ostream;
};

class Spreadsheet 
{
public:
    Spreadsheet( std::ostream &ostream )
        : _generator( ostream )
    {
        _generator.begin_spreadsheet();
    }
    
    ~Spreadsheet()
    {
        _generator.end_spreadsheet();
    }

    ODSGenerator &generator()
    {
        return _generator;
    }
    
private:
    ODSGenerator _generator;
};

class Sheet 
{
public:
    Sheet( Spreadsheet &spreadsheet, 
           const std::string &name )
        : _generator( spreadsheet.generator() ),
          _name( name ),
          _columns( 0 ),
          _rows( 0 )
    {
        _generator.begin_sheet( _name );
    }
    
    ~Sheet()
    {
        _generator.end_sheet();
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

    void new_column()
    {
        ++_columns;
    }
    
    void new_row()
    {
        ++_rows;
    }
    
private:
    ODSGenerator &_generator;
    std::string _name;

    unsigned int _columns,
                 _rows;
};

class Row 
{
public:
    Row( Sheet &sheet )
        : _sheet( sheet ),
          _generator( _sheet.generator() ),
          _column( 0 ),
          _row( 0 )
    {
        _sheet.new_row();
        _row = _sheet.get_rows();
        _generator.begin_row();
    }
    
    ~Row()
    {
        _generator.end_row();
    }

    ODSGenerator &generator()
    {
        return _generator;
    }
    
    template < class T >
    void add_cell( const T& value )
    {
        _generator.add_cell( value );
        ++_column;
        if( _column > _sheet.get_columns() )
            _sheet.new_column();
    }

    template < class T >
    Row& operator << ( const T& value )
    {
        add_cell( value );
        return *this;
    }
        
private:
    Sheet &_sheet;
    ODSGenerator &_generator;

    unsigned int _column,
                 _row;
};

class CellAddress 
{
public:
    CellAddress( const char *sheet,
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
    Chart( const char *name,
           const char *width,
           const char *height,
           const CellRange &range )
        : _name( name ),
          _width( width ),
          _height( height ),
          _range( range )
    {}

    void add_series( const Series &series )
    {
        _list.push_back( series );
    }

    friend std::ostream& operator << ( std::ostream&, const Chart& );

private:
    std::string _name,
                _width,
                _height;
    CellRange _range;

    std::list< Series > _list;
};

MAP_ODS_TYPE(Chart, object);

std::ostream& operator << ( std::ostream &ostream,
                            const Chart& chart ) 
{
    ostream << "<chart name=\"" << chart._name << "\""
            << "       width=\"" << chart._width << "\""
            << "       height=\"" << chart._height << "\""
            << "       range=\"" << chart._range << "\">";

    for( std::list< Series >::const_iterator it = chart._list.begin();
         it != chart._list.end();
         it++ )
        ostream << *it;

    ostream << "</chart>";
    return ostream;
}

class AutoChart : public Chart
{
public:
    AutoChart( const char *name,
               const char *width,
               const char *height,
               const Sheet &sheet )
        : Chart( name, 
                 width, 
                 height, 
                 CellRange( CellAddress( sheet.get_name(), 1, 1 ),
                            CellAddress( sheet.get_name(),
                                         sheet.get_columns(),
                                         sheet.get_rows() ) ) )
    {
        srandom( time( NULL ) );

        for( unsigned int i = 2; i <= sheet.get_columns(); i++ ) 
        {
            CellAddress name( sheet.get_name(), i, 1 ),
                        domain_start( sheet.get_name(), 1, 2 ),
                        domain_end( sheet.get_name(), 1, sheet.get_rows() ),
                        values_start( sheet.get_name(), i, 2 ),
                        values_end( sheet.get_name(), i, sheet.get_rows() );
            CellRange domain( domain_start, domain_end ),
                      values( values_start, values_end );
            Color color( random() );

            add_series( Series( name, domain, values, color ) );
        }
    }
};

MAP_ODS_TYPE(AutoChart, object);

#endif // ODS_GENERATOR_H
