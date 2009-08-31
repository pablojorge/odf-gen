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
    ret, output = run(['xsltproc', xslt, arg], input)

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

if __name__ == '__main__':
    if len(sys.argv) != 3:
        print "usage: %s input.xml output.ods" % (sys.argv[0])
        sys.exit(1)

    input, output = sys.argv[1:]

    print "Generating main XML file..."
    contents = xsltproc('xslt/ods-contents.xslt', input)

    print "Extracting charts..."
    charts = xsltproc('xslt/ods-charts-split.xslt', input)
    charts = charts.split('__SEP__')

    objects = []
    print "Generating objects..."
    for chart in charts:
        if chart and chart != '\n':
            objects.append(xsltproc('xslt/ods-chart.xslt', '-', chart))

    print "Creating '%s'..." % (output)
    zip = Zip(output)

    print "Adding 'content.xml'..."
    zip.add_string('content.xml', contents)

    print "Adding 'mimetype'..."
    mimetype = 'application/vnd.oasis.opendocument.spreadsheet'
    zip.add_string('mimetype', mimetype)

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
         </manifest:manifest>
    """

    print "Adding 'META-INF/manifest.xml'..."
    zip.add_string('META-INF/manifest.xml', manifest)

    print "Output complete"
    zip.close()

