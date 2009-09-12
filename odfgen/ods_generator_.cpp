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
    class_<OutputStream>("OutputStream")
    ;

    class_<StdOut,
           bases<OutputStream>,
           boost::noncopyable>("StdOut")
    ;

    class_<StringStream,
           bases<OutputStream>,
           boost::noncopyable>("StringStream")
        .def("str", &StringStream::str)
    ;

    class_<File,
           bases<OutputStream>,
           boost::noncopyable>("File", init<const std::string&>())
        .def("close", &File::close) 
    ;

    class_<ODSGenerator>("ODSGenerator_", 
                         init<OutputStream&>())
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
    
    class_<Spreadsheet>("Spreadsheet_",
                        init<OutputStream&>())
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

    class_<CellAddress>("CellAddress",
                        init<const std::string&,
                             int,
                             int>())
    ;

    class_<CellRange>("CellRange",
                      init<const CellAddress&,
                           const CellAddress&>())
    ;
    
    class_<Row>("Row_", init<Sheet&>())
        .def("close", &Row::close)
        .def("add_string", &Row::add_cell<const std::string&>)
        .def("add_number", &Row::add_cell<double>)
        .def("add_chart", &Row::add_cell<const Chart&>)
    ;
    
    class_<Color>("Color_", init<unsigned char,
                                 unsigned char,
                                 unsigned char>())
    ;
    
    class_<ColorGenerator>("ColorGenerator_")
        .def("next", &ColorGenerator::next)
    ;
    
    class_<Series>("Series_", init<const CellAddress&,
                                   const CellRange&,
                                   const CellRange&,
                                   const Color&>())
    ;
    
    class_<Chart>("Chart_", init<const std::string&,
                                 const std::string&,
                                 const std::string&>())
        .def("add_range", &Chart::add_range)
        .def("add_series", &Chart::add_series)
        .def("set_title", &Chart::set_title)
        .def("set_subtitle", &Chart::set_subtitle)
        .def("set_x_axis_label", &Chart::set_x_axis_label)
        .def("set_y_axis_label", &Chart::set_y_axis_label)
    ;
    
    class_<AutoChart,
           bases<Chart> >("AutoChart_", init<const std::string&,
                                             const std::string&,
                                             const std::string&,
                                             const Sheet&>())
    ;
}
