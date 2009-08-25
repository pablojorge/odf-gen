#!/usr/bin/env bash

if [ $# -ne 2 ]; then
    echo "usage: $0 input.xml output.ods"
    exit
fi

tmp_content=`mktemp`

if ! xsltproc ods.xslt $1 > $tmp_content; then
    rm $tmp_content
    exit 1
fi

tmp_dir=`mktemp -d`

mkdir $tmp_dir/META-INF
cat > $tmp_dir/META-INF/manifest.xml << __MANIFEST_END__
<?xml version="1.0" encoding="UTF-8"?>
  <manifest:manifest xmlns:manifest="urn:oasis:names:tc:opendocument:xmlns:manifest:1.0">
  <manifest:file-entry manifest:media-type="application/vnd.oasis.opendocument.spreadsheet" manifest:version="1.2" manifest:full-path="/"/>
  <manifest:file-entry manifest:media-type="text/xml" manifest:full-path="content.xml"/>
</manifest:manifest>
__MANIFEST_END__

mv $tmp_content $tmp_dir/content.xml
echo application/vnd.oasis.opendocument.spreadsheet > $tmp_dir/mimetype

pushd $tmp_dir; zip -r $tmp_dir.zip .; popd 
mv $tmp_dir.zip $2

rm -rf $tmp_dir
