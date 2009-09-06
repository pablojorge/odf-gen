import os
import sys

import time
import zipfile
import stat

def run(command, input=''):
    from subprocess import Popen, PIPE
    p = Popen(command, stdin=PIPE, stdout=PIPE)
    output = p.communicate(input)[0]
    return (p.returncode, output)

def xsltproc(xslt, arg, input=''):
    ret, output = run(['xsltproc', 
                        os.path.join(os.path.split(sys.argv[0])[0], xslt), 
                        arg], 
                      input)

    if ret:
        raise Exception('xsltproc failed (ret:%i)' % (ret))

    return output

class Zip:
    def __init__(self, name, mode='w'):
        self.__zip = zipfile.ZipFile(name, mode)

    def add_string(self, name, string):
        file = zipfile.ZipInfo(name)
        file.date_time = time.localtime(time.time())[:6]
        file.compress_type = zipfile.ZIP_DEFLATED
        file.external_attr = (stat.S_IRUSR | stat.S_IWUSR) << 16 
        self.__zip.writestr(file, string) 

    def close(self):
        self.__zip.close()

def chart_name(chart):
    str = '<chart name="'
    start = chart.find(str)

    if start < 0:
        raise Exception('Unable to extract the chart name')

    end = chart.find('"', start + len(str))

    return chart[(start + len(str)):end]
    
if __name__ == '__main__':
    if len(sys.argv) != 3:
        print "usage: %s input.xml output.ods" % (sys.argv[0])
        sys.exit(1)

    input, output = sys.argv[1:]

    print "Generating main XML file..."
    content = xsltproc('xslt/ods-main-content.xslt', input)

    print "Extracting charts..."
    charts = xsltproc('xslt/ods-extract-charts.xslt', input)
    charts = charts.split('__SEP__')

    print "Generating objects..."
    objects = []
    for chart in charts:
        if chart and chart != '\n':
            object = (chart_name(chart),
                      xsltproc('xslt/ods-chart-content.xslt', '-', chart))
            objects.append(object)

    print "Creating '%s'..." % (output)
    zip = Zip(output)

    print "Adding 'mimetype'..."
    mimetype = 'application/vnd.oasis.opendocument.spreadsheet'
    zip.add_string('mimetype', mimetype)

    print "Adding 'content.xml'..."
    zip.add_string('content.xml', content)

    for object in objects:
        path = object[0] + '/content.xml'
        print "Adding '%s'..." % (path)
        zip.add_string(path, object[1])

    manifest = \
    """<?xml version="1.0" encoding="UTF-8"?>
         <manifest:manifest 
           xmlns:manifest="urn:oasis:names:tc:opendocument:xmlns:manifest:1.0">
         <manifest:file-entry 
           manifest:media-type="application/vnd.oasis.opendocument.spreadsheet" 
           manifest:version="1.2" 
           manifest:full-path="/"/>
         <manifest:file-entry 
           manifest:media-type="text/xml" 
           manifest:full-path="content.xml"/>
    """

    for object in objects:
        manifest += \
          """<manifest:file-entry 
               manifest:media-type="text/xml" 
               manifest:full-path="%s/content.xml"/>
          """ % (object[0])

        manifest += \
          """<manifest:file-entry 
               manifest:media-type="application/vnd.oasis.opendocument.chart" 
               manifest:full-path="%s/"/>
          """ % (object[0])

    manifest += \
    """  </manifest:manifest>"""

    print "Adding 'META-INF/manifest.xml'..."
    zip.add_string('META-INF/manifest.xml', manifest)

    print "Output complete"
    zip.close()

