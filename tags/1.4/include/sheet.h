// odf-gen: Simple API to generate OpenDocument documents.
//
//          Copyright Pablo Jorge 2009 - 2013.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
//

#pragma once

#include <string>

#include "element.h"
#include "spreadsheet.h"

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

