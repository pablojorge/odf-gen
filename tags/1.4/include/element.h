// odf-gen: Simple API to generate OpenDocument documents.
//
//          Copyright Pablo Jorge 2009 - 2013.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
//

#pragma once

#include <iostream>

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

