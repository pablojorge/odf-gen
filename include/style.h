// odf-gen: Simple API to generate OpenDocument documents.
//
//          Copyright Pablo Jorge 2009 - 2013.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
//

#pragma once

#include <iostream>

#include <mili/mili.h>

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

    Style operator | ( const Style& other ) const
    {
        return Style(_flags | other._flags);
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

