# odf-gen: Simple API to generate OpenDocument documents.
#
#          Copyright Pablo Jorge 2009 - 2013.
# Distributed under the Boost Software License, Version 1.0.
#    (See accompanying file ../../LICENSE_1_0.txt or copy at
#          http://www.boost.org/LICENSE_1_0.txt)
#

from _odsgenerator import *

# type mapper (python style)
def type_map( value ):
    type_mapping = [
        ([int, float], "number"),
        ([str], "string"),
        ([Chart, AutoChart], "chart"),
        ([Style], "style")
    ]

    typestr = reduce(lambda o, p: p[1] if type(value) in p[0] else o, 
                     type_mapping, 
                     None)

    if not typestr:
        raise Exception("Unable to map type '%s'" % (type(value)))

    return typestr

## methods to be injected (duck punching)
# automatic scope handling:
def scope_enter( self ):
    return self

def scope_exit( self, type, value, traceback ):
    self.close()
        
# automatic type handling:
def add_cell( self, value ):
    method = "add_" + type_map(value)
    return getattr(self, method)(value)

def append_row( self, values ):
    with Row( self ) as row:
        for value in values:
            row.add_cell( value )

# Spreadsheet patching:
Spreadsheet.__enter__ = scope_enter
Spreadsheet.__exit__ = scope_exit

# Sheet patching:
Sheet.__enter__ = scope_enter
Sheet.__exit__ = scope_exit
Sheet.append_row = append_row

# Row patching:
Row.__enter__ = scope_enter
Row.__exit__ = scope_exit
Row.add_cell = add_cell
