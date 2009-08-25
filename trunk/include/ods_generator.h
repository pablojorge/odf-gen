/*
ods_generator: Simple API to generate a ods-like xml file.
    Copyright (C) 2008, 2009  Daniel Gutson, FuDePAN

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
    {}

    void begin_spreadsheet()
    {
        _ostream << "<?xml version=\"1.0\"?>";
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
    
    void end_sheet() {
        _ostream << "</sheet>";
    }
 
    void begin_row()
    {
        _ostream << "<row>";
    }
    
    void end_row() {
        _ostream << "</row>";
    }

    template < class T >
    void add_cell( const T& value )
    {
        _ostream << "<cell type=\"" << ODSType< T >::convert() << "\">" 
                 << value 
                 << "</cell>";
    }
        
private:
    std::ostream &_ostream;
};

class Spreadsheet {
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

class Sheet {
public:
    Sheet( Spreadsheet &spreadsheet, 
           const std::string &name )
        : _generator( spreadsheet.generator() )
    {
        _generator.begin_sheet( name );
    }
    
    ~Sheet()
    {
        _generator.end_sheet();
    }

    ODSGenerator &generator()
    {
        return _generator;
    }
    
private:
    ODSGenerator &_generator;
};

class Row {
public:
    Row( Sheet &sheet )
        : _generator( sheet.generator() )
    {
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
    }
        
private:
    ODSGenerator &_generator;
};

#endif // ODS_GENERATOR_H
