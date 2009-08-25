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

class ODSGenerator:
    def __init__( self, output ):
        self.__output = output

    def begin_spreadsheet( self ):
        self.__output.write("<?xml version=\"1.0\"?>")
        self.__output.write("<spreadsheet>")

    def end_spreadsheet( self ):
        self.__output.write("</spreadsheet>")
        
    def begin_sheet( self, name ):
        self.__output.write( "<sheet name=\"" + str(name) + "\">" )
    
    def end_sheet( self ):
        self.__output.write( "</sheet>" )
 
    def begin_row( self ):
        self.__output.write( "<row>" )
    
    def end_row( self ):
        self.__output.write( "</row>" )

    def add_cell( self, value ):
        type_mapping = [
            ([int, float], "float"),
            ([str], "string")
        ]

        ods_type = reduce(lambda o, p: p[1] if type(value) in p[0] else o, 
                          type_mapping, 
                          None)

        if not ods_type:
            raise Exception("Unknown type")

        self.__output.write( "<cell type=\"" + ods_type + "\">" +
                             str(value) +
                             "</cell>" );
        
class Spreadsheet:
    def __init__( self, output ):
        self.__generator = ODSGenerator( output )

    def __enter__( self ):
        self.__generator.begin_spreadsheet()
        return self
    
    def __exit__( self, type, value, traceback ):
        self.__generator.end_spreadsheet()

    def generator( self ):
        return self.__generator
    
class Sheet:
    def __init__( self, spreadsheet, name ):
        self.__generator = spreadsheet.generator()
        self.__name = name

    def __enter__( self ):
        self.__generator.begin_sheet( self.__name );
        return self
    
    def __exit__( self, type, value, traceback ):
        self.__generator.end_sheet()

    def generator( self ):
        return self.__generator
    
class Row:
    def __init__( self, sheet ):
        self.__generator = sheet.generator()

    def __enter__( self ):
        self.__generator.begin_row()
        return self
    
    def __exit__( self, type, value, traceback ):
        self.__generator.end_row()
        
    def add_cell( self, value ):
        self.__generator.add_cell( value )
