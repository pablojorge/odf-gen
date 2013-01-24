// odf-gen: Simple API to generate OpenDocument documents.
//
//          Copyright Pablo Jorge 2009 - 2013.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
//

#pragma once

#include <iostream>

#include "element.h"

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

