# ods_generator: Simple API to generate a ods-like xml file.
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

from ods_generator_ import *

class ODSGenerator(ODSGenerator_):
    pass

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

class Row(Row_):
    def __init__( self, sheet ):
        Row_.__init__( self, sheet )

    def __enter__( self ):
        return self
    
    def __exit__( self, type, value, traceback ):
        self.close()

class AutoChart(AutoChart_):
    pass

