// odf-gen: Simple API to generate OpenDocument documents.
//
//          Copyright Pablo Jorge 2009 - 2013.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
//

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
