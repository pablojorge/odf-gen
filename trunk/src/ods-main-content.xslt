<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet 
  version="1.0"
  xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
  xmlns:office="urn:oasis:names:tc:opendocument:xmlns:office:1.0" 
  xmlns:style="urn:oasis:names:tc:opendocument:xmlns:style:1.0" 
  xmlns:text="urn:oasis:names:tc:opendocument:xmlns:text:1.0" 
  xmlns:table="urn:oasis:names:tc:opendocument:xmlns:table:1.0" 
  xmlns:draw="urn:oasis:names:tc:opendocument:xmlns:drawing:1.0" 
  xmlns:fo="urn:oasis:names:tc:opendocument:xmlns:xsl-fo-compatible:1.0" 
  xmlns:xlink="http://www.w3.org/1999/xlink" 
  xmlns:dc="http://purl.org/dc/elements/1.1/" 
  xmlns:meta="urn:oasis:names:tc:opendocument:xmlns:meta:1.0" 
  xmlns:number="urn:oasis:names:tc:opendocument:xmlns:datastyle:1.0" 
  xmlns:presentation="urn:oasis:names:tc:opendocument:xmlns:presentation:1.0"
  xmlns:svg="urn:oasis:names:tc:opendocument:xmlns:svg-compatible:1.0" 
  xmlns:chart="urn:oasis:names:tc:opendocument:xmlns:chart:1.0" 
  xmlns:dr3d="urn:oasis:names:tc:opendocument:xmlns:dr3d:1.0" 
  xmlns:math="http://www.w3.org/1998/Math/MathML" 
  xmlns:form="urn:oasis:names:tc:opendocument:xmlns:form:1.0" 
  xmlns:script="urn:oasis:names:tc:opendocument:xmlns:script:1.0" 
  xmlns:ooo="http://openoffice.org/2004/office" 
  xmlns:ooow="http://openoffice.org/2004/writer" 
  xmlns:oooc="http://openoffice.org/2004/calc" 
  xmlns:dom="http://www.w3.org/2001/xml-events" 
  xmlns:xforms="http://www.w3.org/2002/xforms" 
  xmlns:xsd="http://www.w3.org/2001/XMLSchema" 
  xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" 
  xmlns:rpt="http://openoffice.org/2005/report" 
  xmlns:of="urn:oasis:names:tc:opendocument:xmlns:of:1.2" 
  xmlns:rdfa="http://docs.oasis-open.org/opendocument/meta/rdfa#"
  xmlns:field="urn:openoffice:names:experimental:ooxml-odf-interop:xmlns:field:1.0" 
  xmlns:formx="urn:openoffice:names:experimental:ooxml-odf-interop:xmlns:form:1.0" 
  xmlns="http://www.w3.org/1999/xhtml">
<!-- See "Extracting Unique Values with XSL" 
     (http://www.bernzilla.com/item.php?id=333) -->
<xsl:key name="cell-styles" match="cell" use="@style"/>
<xsl:output method="xml" indent="yes" encoding="UTF-8"/>
<xsl:template match="/spreadsheet">
<office:document-content 
  xmlns:office="urn:oasis:names:tc:opendocument:xmlns:office:1.0" 
  xmlns:style="urn:oasis:names:tc:opendocument:xmlns:style:1.0" 
  xmlns:text="urn:oasis:names:tc:opendocument:xmlns:text:1.0" 
  xmlns:table="urn:oasis:names:tc:opendocument:xmlns:table:1.0" 
  xmlns:draw="urn:oasis:names:tc:opendocument:xmlns:drawing:1.0" 
  xmlns:fo="urn:oasis:names:tc:opendocument:xmlns:xsl-fo-compatible:1.0" 
  xmlns:xlink="http://www.w3.org/1999/xlink" 
  xmlns:dc="http://purl.org/dc/elements/1.1/" 
  xmlns:meta="urn:oasis:names:tc:opendocument:xmlns:meta:1.0" 
  xmlns:number="urn:oasis:names:tc:opendocument:xmlns:datastyle:1.0" 
  xmlns:presentation="urn:oasis:names:tc:opendocument:xmlns:presentation:1.0"
  xmlns:svg="urn:oasis:names:tc:opendocument:xmlns:svg-compatible:1.0" 
  xmlns:chart="urn:oasis:names:tc:opendocument:xmlns:chart:1.0" 
  xmlns:dr3d="urn:oasis:names:tc:opendocument:xmlns:dr3d:1.0" 
  xmlns:math="http://www.w3.org/1998/Math/MathML" 
  xmlns:form="urn:oasis:names:tc:opendocument:xmlns:form:1.0" 
  xmlns:script="urn:oasis:names:tc:opendocument:xmlns:script:1.0" 
  xmlns:ooo="http://openoffice.org/2004/office" 
  xmlns:ooow="http://openoffice.org/2004/writer" 
  xmlns:oooc="http://openoffice.org/2004/calc" 
  xmlns:dom="http://www.w3.org/2001/xml-events" 
  xmlns:xforms="http://www.w3.org/2002/xforms" 
  xmlns:xsd="http://www.w3.org/2001/XMLSchema" 
  xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" 
  xmlns:rpt="http://openoffice.org/2005/report" 
  xmlns:of="urn:oasis:names:tc:opendocument:xmlns:of:1.2" 
  xmlns:rdfa="http://docs.oasis-open.org/opendocument/meta/rdfa#"
  xmlns:field="urn:openoffice:names:experimental:ooxml-odf-interop:xmlns:field:1.0" 
  xmlns:formx="urn:openoffice:names:experimental:ooxml-odf-interop:xmlns:form:1.0" 
  office:version="1.2">
<office:automatic-styles>
  <xsl:for-each select="//cell[generate-id() = generate-id(key('cell-styles',@style)[1])]">
    <style:style style:name="cell-style-{@style}" 
                 style:family="table-cell">
      <style:table-cell-properties>
        <xsl:choose>
          <xsl:when test="contains(@style,'border-bottom')">
            <xsl:attribute name="fo:border-bottom">
              <xsl:text>0.0008in solid #000000</xsl:text>
            </xsl:attribute>
          </xsl:when>
          <xsl:otherwise>
            <xsl:attribute name="fo:border-bottom">
              <xsl:text>none</xsl:text>
            </xsl:attribute>
          </xsl:otherwise>
        </xsl:choose>
        <xsl:choose>
          <xsl:when test="contains(@style,'border-left')">
            <xsl:attribute name="fo:border-left">
              <xsl:text>0.0008in solid #000000</xsl:text>
            </xsl:attribute>
          </xsl:when>
          <xsl:otherwise>
            <xsl:attribute name="fo:border-left">
              <xsl:text>none</xsl:text>
            </xsl:attribute>
          </xsl:otherwise>
        </xsl:choose>
        <xsl:choose>
          <xsl:when test="contains(@style,'border-right')">
            <xsl:attribute name="fo:border-right">
              <xsl:text>0.0008in solid #000000</xsl:text>
            </xsl:attribute>
          </xsl:when>
          <xsl:otherwise>
            <xsl:attribute name="fo:border-right">
              <xsl:text>none</xsl:text>
            </xsl:attribute>
          </xsl:otherwise>
        </xsl:choose>
        <xsl:choose>
          <xsl:when test="contains(@style,'border-top')">
            <xsl:attribute name="fo:border-top">
              <xsl:text>0.0008in solid #000000</xsl:text>
            </xsl:attribute>
          </xsl:when>
          <xsl:otherwise>
            <xsl:attribute name="fo:border-top">
              <xsl:text>none</xsl:text>
            </xsl:attribute>
          </xsl:otherwise>
        </xsl:choose>
      </style:table-cell-properties>
    </style:style>
  </xsl:for-each>
</office:automatic-styles>
<office:body>
<office:spreadsheet>
<xsl:apply-templates select="sheet"/>
</office:spreadsheet>
</office:body>
</office:document-content>
</xsl:template>

<xsl:template match="sheet">
  <table:table table:name="{@name}">
    <xsl:apply-templates select="row"/>
  </table:table>
</xsl:template>

<xsl:template match="row">
  <table:table-row>
    <xsl:for-each select="cell">
    <xsl:choose>
      <xsl:when test="@type='object'">
        <table:table-cell>
          <xsl:apply-templates select="."/>
        </table:table-cell>
      </xsl:when>
      <xsl:otherwise>
        <table:table-cell table:style-name="cell-style-{@style}"
                          office:value-type="{@type}" 
                          office:value="{.}">
          <text:p>
            <xsl:value-of select="."/>
          </text:p>
        </table:table-cell>
      </xsl:otherwise>
    </xsl:choose>
    </xsl:for-each>
  </table:table-row>
</xsl:template>

<xsl:template match="chart">
  <draw:frame svg:width="{@width}" 
              svg:height="{@height}">
    <draw:object xlink:href="./{@name}" 
                 xlink:type="simple" 
                 xlink:show="embed" 
                 xlink:actuate="onLoad"/>
  </draw:frame>
</xsl:template>

</xsl:stylesheet>
