// odf-gen: Simple API to generate OpenDocument documents.
//
//          Copyright Pablo Jorge 2009 - 2013.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
//

#pragma once

#include <iostream>

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

