// odf-gen: Simple API to generate OpenDocument documents.
//
//          Copyright Pablo Jorge 2009 - 2013.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
//

#pragma once

#include <iostream>

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

