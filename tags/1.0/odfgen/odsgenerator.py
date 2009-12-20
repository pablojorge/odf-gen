# odf-gen: Simple API to generate OpenDocument documents.
# Copyright (C) 2009  Pablo Jorge, FuDePAN
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

from odsgenerator_ import *

class ODSGenerator(ODSGenerator_):
    @staticmethod
    def type_map( value ):
        type_mapping = [
            ([int, float], "add_number"),
            ([str], "add_string"),
            ([Chart, AutoChart], "add_chart")
        ]

        method = reduce(lambda o, p: p[1] if type(value) in p[0] else o, 
                        type_mapping, 
                        None)

        if not method:
            raise Exception("Unable to map type '%s'" % (type(value)))

        return method
    
    def add_cell( self, value ):
        getattr(self, ODSGenerator.type_map(value))(value)

class Spreadsheet(Spreadsheet_):
    def __enter__( self ):
        return self
    
    def __exit__( self, type, value, traceback ):
        self.close()

class Sheet(Sheet_):
    def __init__( self, spreadsheet, name ):
        Sheet_.__init__( self, spreadsheet, name )

    def __enter__( self ):
        return self
    
    def __exit__( self, type, value, traceback ):
        self.close()
        
    def append_row( self, values ):
        with Row( self ) as row:
            for value in values:
                row.add_cell( value )

class Row(Row_):
    def __init__( self, sheet ):
        Row_.__init__( self, sheet )

    def __enter__( self ):
        return self
    
    def __exit__( self, type, value, traceback ):
        self.close()

    def add_cell( self, value ):
        return getattr(self, ODSGenerator.type_map(value))(value)

class Color(Color_):
    pass

class ColorGenerator(ColorGenerator_):
    pass

class Length(Length_):
    def __str__(self):
        return self.str()

class Centimeters(Centimeters_):
    pass
        
class Series(Series_):
    pass

class Chart(Chart_):
    pass

class AutoChart(AutoChart_):
    pass
