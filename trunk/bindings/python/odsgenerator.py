# odf-gen: Simple API to generate OpenDocument documents.
# Copyright (C) 2012  Pablo Jorge, FuDePAN
#
# This file is part of the odf-gen project.
#
# odf-gen is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# odf-gen is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with odf-gen.  If not, see <http://www.gnu.org/licenses/>.

# As a special exception, you may create a larger work that contains
# part of all of the odf-gen project and distribute that work under
# the terms of the GNU General Public License as published by the
# Free Software Foundation; version 2 of the License
#
# Alternatively, if you modify or redistribute the ODF generator
# itself, you may (ar your option) remove this special exception
#
# This special exception was added by Pablo Jorge in 
# version 1.3 of odf-gen. 

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
