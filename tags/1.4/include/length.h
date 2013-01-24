// odf-gen: Simple API to generate OpenDocument documents.
//
//          Copyright Pablo Jorge 2009 - 2013.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
//

#pragma once

#include <iostream>
#include <string>
#include <sstream>

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

