# odf-gen: Simple API to generate OpenDocument documents.
#
#          Copyright Pablo Jorge 2009 - 2013.
# Distributed under the Boost Software License, Version 1.0.
#    (See accompanying file ../LICENSE_1_0.txt or copy at
#          http://www.boost.org/LICENSE_1_0.txt)
#

from odfgen.odsgenerator import *

def test():
    stream = StringStream()

    with Spreadsheet( stream ) as spreadsheet:
        with Sheet( spreadsheet, "sheet1" ) as sheet1:
            sheet1.append_row(["x", "x * 2"])

            for i in range( 5 ):
                sheet1.append_row([i, i * 2])

            sheet1.append_row([AutoChart("chart1", 
                                         Centimeters(8), 
                                         Centimeters(8), 
                                         sheet1)])

    print stream.str()

if __name__ == "__main__":
    test()
