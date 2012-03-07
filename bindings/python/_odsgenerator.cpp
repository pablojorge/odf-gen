/*
odf-gen: Simple API to generate OpenDocument documents.
    Copyright (C) 2012  Pablo Jorge, FuDePAN

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

/*  As a special exception, you may create a larger work that contains
    part of all of the odf-gen project and distribute that work under
    the terms of the GNU General Public License as published by the
    Free Software Foundation; version 2 of the License

    Alternatively, if you modify or redistribute the ODF generator
    itself, you may (ar your option) remove this special exception

    This special exception was added by Pablo Jorge in 
    version 1.3 of odf-gen. 
*/

#include <iostream>
#include <sstream>
#include <fstream>

#include <boost/python.hpp>

#include "odsgenerator.h"

using namespace boost::python;

BOOST_PYTHON_MODULE(_odsgenerator)
{
    class_<std::ostream,
           boost::noncopyable>("OutputStream",
                               no_init)
    ;

    class_<std::stringstream,
           bases<std::ostream>,
           boost::noncopyable>("StringStream")
        .def("str", (std::string (std::stringstream::*)() const)
                    (&std::stringstream::str))
    ;

    class_<std::ofstream,
           bases<std::ostream>,
           boost::noncopyable>("File", 
                               init<const char*>())
        .def("close", &std::ofstream::close) 
    ;

    class_<Spreadsheet,
           boost::noncopyable>("Spreadsheet",
                               init<std::ostream&>())
        .def("close", &Spreadsheet::close)
    ;
    
    class_<Sheet,
           boost::noncopyable>("Sheet", 
                               init<Spreadsheet&, 
                                    const std::string&>())
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

    class_<Style>("Style")
    ;

    class_<Row,
           boost::noncopyable>("Row", 
                               init<Sheet&>())
        .def("close", &Row::close)
        .def("add_string", (CellAddress (Row::*)(const std::string&))
                           (&Row::add_cell<const std::string&>))
        .def("add_number", (CellAddress (Row::*)(const double&))
                           (&Row::add_cell<const double&>))
        .def("add_chart", (CellAddress (Row::*)(const Chart&))
                          (&Row::add_cell<const Chart&>))
        .def("add_style", &Row::add_style)
    ;
    
    class_<Color>("Color", 
                  init<unsigned char,
                       unsigned char,
                       unsigned char>())
    ;
    
    class_<ColorGenerator>("ColorGenerator")
        .def("next", &ColorGenerator::next)
    ;
    
    class_<Length>("Length", 
                   no_init)
       .def("str", &Length::str)
    ;

    class_<Centimeters,
           bases<Length> >("Centimeters", 
                           init<unsigned int>())
    ;
    
    class_<Series>("Series", 
                   init<const CellAddress&,
                        const CellRange&,
                        const CellRange&,
                        const Color&>())
    ;

    class_<Chart>("Chart", 
                  init<const std::string&,
                       const Length&,
                       const Length&>())
        .def("add_range", &Chart::add_range)
        .def("add_series", &Chart::add_series)
        .def("set_title", &Chart::set_title)
        .def("set_subtitle", &Chart::set_subtitle)
        .def("set_x_axis_label", &Chart::set_x_axis_label)
        .def("set_y_axis_label", &Chart::set_y_axis_label)
    ;
    
    class_<AutoChart,
           bases<Chart> >("AutoChart", 
                          init<const std::string&,
                               const Length&,
                               const Length&,
                               const Sheet&>())
    ;
}
