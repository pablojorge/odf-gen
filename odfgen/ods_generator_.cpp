/*
ods_generator: Simple API to generate a ods-like xml file.
    Copyright (C) 2009  Pablo Jorge, FuDePAN

    This file is part of the odf-gen project.

    odf-gen is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    odf-gen is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with odf-gen.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <sstream>
#include <boost/python.hpp>
#include "include/ods_generator.h"

using namespace boost::python;

BOOST_PYTHON_MODULE(ods_generator_)
{
    class_<ODSGenerator>("ODSGenerator_")
        .def("begin_spreadsheet", &ODSGenerator::begin_spreadsheet)
        .def("end_spreadsheet", &ODSGenerator::end_spreadsheet)
        .def("begin_sheet", &ODSGenerator::begin_sheet)
        .def("end_sheet", &ODSGenerator::end_sheet)
        .def("begin_row", &ODSGenerator::begin_row)
        .def("end_row", &ODSGenerator::end_row)
        .def("add_string", &ODSGenerator::add_cell<const std::string&>)
        .def("add_number", &ODSGenerator::add_cell<double>)
        .def("add_chart", &ODSGenerator::add_cell<const Chart&>)
    ;
    
    class_<Spreadsheet>("Spreadsheet_")
        .def("close", &Spreadsheet::close)
    ;
    
    class_<Sheet>("Sheet_", init<Spreadsheet&, const std::string&>())
        .def("close", &Sheet::close)
        .def("get_name", &Sheet::get_name)
        .def("get_columns", &Sheet::get_columns)
        .def("get_rows", &Sheet::get_rows)
        .def("add_column", &Sheet::add_column)
        .def("add_row", &Sheet::add_row)
    ;
    
    class_<Row>("Row_", init<Sheet&>())
        .def("close", &Row::close)
        .def("add_string", &Row::add_cell<const std::string&>)
        .def("add_number", &Row::add_cell<double>)
        .def("add_chart", &Row::add_cell<const Chart&>)
    ;
    
    class_<Chart>("Chart_", init<const std::string&,
                                 const std::string&,
                                 const std::string&,
                                 const CellRange&>())
    ;
    
    class_<AutoChart,
           bases<Chart> >("AutoChart_", init<const std::string&,
                                             const std::string&,
                                             const std::string&,
                                             const Sheet&>())
    ;
}
