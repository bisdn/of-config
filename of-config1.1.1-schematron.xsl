<?xml version="1.0" standalone="yes"?>
<!--This XSLT was automatically generated from a Schematron schema.-->
<axsl:stylesheet xmlns:date="http://exslt.org/dates-and-times" xmlns:dyn="http://exslt.org/dynamic" xmlns:exsl="http://exslt.org/common" xmlns:math="http://exslt.org/math" xmlns:random="http://exslt.org/random" xmlns:regexp="http://exslt.org/regular-expressions" xmlns:set="http://exslt.org/sets" xmlns:str="http://exslt.org/strings" xmlns:axsl="http://www.w3.org/1999/XSL/Transform" xmlns:sch="http://www.ascc.net/xml/schematron" xmlns:iso="http://purl.oclc.org/dsdl/schematron" xmlns:ofc="urn:onf:of111:config:yang" xmlns:xdpd-mgmt="urn:xdpd:mgmt:yang" xmlns:nc="urn:ietf:params:xml:ns:netconf:base:1.0" extension-element-prefixes="date dyn exsl math random regexp set str" version="1.0"><!--Implementers: please note that overriding process-prolog or process-root is 
    the preferred method for meta-stylesheets to use where possible. -->
<axsl:param name="archiveDirParameter"/><axsl:param name="archiveNameParameter"/><axsl:param name="fileNameParameter"/><axsl:param name="fileDirParameter"/>

<!--PHASES-->


<!--PROLOG-->
<axsl:output xmlns:xs="http://www.w3.org/2001/XMLSchema" xmlns:schold="http://www.ascc.net/xml/schematron" xmlns:svrl="http://purl.oclc.org/dsdl/svrl" method="xml" omit-xml-declaration="no" standalone="yes" indent="yes"/>

<!--KEYS-->


<!--DEFAULT RULES-->


<!--MODE: SCHEMATRON-SELECT-FULL-PATH-->
<!--This mode can be used to generate an ugly though full XPath for locators-->
<axsl:template match="*" mode="schematron-select-full-path"><axsl:apply-templates select="." mode="schematron-get-full-path"/></axsl:template>

<!--MODE: SCHEMATRON-FULL-PATH-->
<!--This mode can be used to generate an ugly though full XPath for locators-->
<axsl:template match="*" mode="schematron-get-full-path"><axsl:apply-templates select="parent::*" mode="schematron-get-full-path"/><axsl:text>/</axsl:text><axsl:choose><axsl:when test="namespace-uri()=''"><axsl:value-of select="name()"/><axsl:variable name="p_1" select="1+    count(preceding-sibling::*[name()=name(current())])"/><axsl:if test="$p_1&gt;1 or following-sibling::*[name()=name(current())]">[<axsl:value-of select="$p_1"/>]</axsl:if></axsl:when><axsl:otherwise><axsl:text>*[local-name()='</axsl:text><axsl:value-of select="local-name()"/><axsl:text>' and namespace-uri()='</axsl:text><axsl:value-of select="namespace-uri()"/><axsl:text>']</axsl:text><axsl:variable name="p_2" select="1+   count(preceding-sibling::*[local-name()=local-name(current())])"/><axsl:if test="$p_2&gt;1 or following-sibling::*[local-name()=local-name(current())]">[<axsl:value-of select="$p_2"/>]</axsl:if></axsl:otherwise></axsl:choose></axsl:template><axsl:template match="@*" mode="schematron-get-full-path"><axsl:text>/</axsl:text><axsl:choose><axsl:when test="namespace-uri()=''">@<axsl:value-of select="name()"/></axsl:when><axsl:otherwise><axsl:text>@*[local-name()='</axsl:text><axsl:value-of select="local-name()"/><axsl:text>' and namespace-uri()='</axsl:text><axsl:value-of select="namespace-uri()"/><axsl:text>']</axsl:text></axsl:otherwise></axsl:choose></axsl:template>

<!--MODE: SCHEMATRON-FULL-PATH-2-->
<!--This mode can be used to generate prefixed XPath for humans-->
<axsl:template match="node() | @*" mode="schematron-get-full-path-2"><axsl:for-each select="ancestor-or-self::*"><axsl:text>/</axsl:text><axsl:value-of select="name(.)"/><axsl:if test="preceding-sibling::*[name(.)=name(current())]"><axsl:text>[</axsl:text><axsl:value-of select="count(preceding-sibling::*[name(.)=name(current())])+1"/><axsl:text>]</axsl:text></axsl:if></axsl:for-each><axsl:if test="not(self::*)"><axsl:text/>/@<axsl:value-of select="name(.)"/></axsl:if></axsl:template>

<!--MODE: GENERATE-ID-FROM-PATH -->
<axsl:template match="/" mode="generate-id-from-path"/><axsl:template match="text()" mode="generate-id-from-path"><axsl:apply-templates select="parent::*" mode="generate-id-from-path"/><axsl:value-of select="concat('.text-', 1+count(preceding-sibling::text()), '-')"/></axsl:template><axsl:template match="comment()" mode="generate-id-from-path"><axsl:apply-templates select="parent::*" mode="generate-id-from-path"/><axsl:value-of select="concat('.comment-', 1+count(preceding-sibling::comment()), '-')"/></axsl:template><axsl:template match="processing-instruction()" mode="generate-id-from-path"><axsl:apply-templates select="parent::*" mode="generate-id-from-path"/><axsl:value-of select="concat('.processing-instruction-', 1+count(preceding-sibling::processing-instruction()), '-')"/></axsl:template><axsl:template match="@*" mode="generate-id-from-path"><axsl:apply-templates select="parent::*" mode="generate-id-from-path"/><axsl:value-of select="concat('.@', name())"/></axsl:template><axsl:template match="*" mode="generate-id-from-path" priority="-0.5"><axsl:apply-templates select="parent::*" mode="generate-id-from-path"/><axsl:text>.</axsl:text><axsl:value-of select="concat('.',name(),'-',1+count(preceding-sibling::*[name()=name(current())]),'-')"/></axsl:template><!--MODE: SCHEMATRON-FULL-PATH-3-->
<!--This mode can be used to generate prefixed XPath for humans 
	(Top-level element has index)-->
<axsl:template match="node() | @*" mode="schematron-get-full-path-3"><axsl:for-each select="ancestor-or-self::*"><axsl:text>/</axsl:text><axsl:value-of select="name(.)"/><axsl:if test="parent::*"><axsl:text>[</axsl:text><axsl:value-of select="count(preceding-sibling::*[name(.)=name(current())])+1"/><axsl:text>]</axsl:text></axsl:if></axsl:for-each><axsl:if test="not(self::*)"><axsl:text/>/@<axsl:value-of select="name(.)"/></axsl:if></axsl:template>

<!--MODE: GENERATE-ID-2 -->
<axsl:template match="/" mode="generate-id-2">U</axsl:template><axsl:template match="*" mode="generate-id-2" priority="2"><axsl:text>U</axsl:text><axsl:number level="multiple" count="*"/></axsl:template><axsl:template match="node()" mode="generate-id-2"><axsl:text>U.</axsl:text><axsl:number level="multiple" count="*"/><axsl:text>n</axsl:text><axsl:number count="node()"/></axsl:template><axsl:template match="@*" mode="generate-id-2"><axsl:text>U.</axsl:text><axsl:number level="multiple" count="*"/><axsl:text>_</axsl:text><axsl:value-of select="string-length(local-name(.))"/><axsl:text>_</axsl:text><axsl:value-of select="translate(name(),':','.')"/></axsl:template><!--Strip characters--><axsl:template match="text()" priority="-1"/>

<!--SCHEMA METADATA-->
<axsl:template match="/"><svrl:schematron-output xmlns:svrl="http://purl.oclc.org/dsdl/svrl" xmlns:xs="http://www.w3.org/2001/XMLSchema" xmlns:schold="http://www.ascc.net/xml/schematron" title="" schemaVersion=""><axsl:comment><axsl:value-of select="$archiveDirParameter"/>   
		 <axsl:value-of select="$archiveNameParameter"/>  
		 <axsl:value-of select="$fileNameParameter"/>  
		 <axsl:value-of select="$fileDirParameter"/></axsl:comment><svrl:ns-prefix-in-attribute-values uri="http://exslt.org/dynamic" prefix="dyn"/><svrl:ns-prefix-in-attribute-values uri="urn:onf:of111:config:yang" prefix="ofc"/><svrl:ns-prefix-in-attribute-values uri="urn:xdpd:mgmt:yang" prefix="xdpd-mgmt"/><svrl:ns-prefix-in-attribute-values uri="urn:ietf:params:xml:ns:netconf:base:1.0" prefix="nc"/><svrl:active-pattern><axsl:attribute name="id">of-config1.1.1</axsl:attribute><axsl:attribute name="name">of-config1.1.1</axsl:attribute><axsl:apply-templates/></svrl:active-pattern><axsl:apply-templates select="/" mode="M5"/><svrl:active-pattern><axsl:attribute name="id">idm140727361412784</axsl:attribute><axsl:attribute name="name">idm140727361412784</axsl:attribute><axsl:apply-templates/></svrl:active-pattern><axsl:apply-templates select="/" mode="M6"/><svrl:active-pattern><axsl:attribute name="id">idm140727361411680</axsl:attribute><axsl:attribute name="name">idm140727361411680</axsl:attribute><axsl:apply-templates/></svrl:active-pattern><axsl:apply-templates select="/" mode="M7"/><svrl:active-pattern><axsl:attribute name="id">idm140727361410576</axsl:attribute><axsl:attribute name="name">idm140727361410576</axsl:attribute><axsl:apply-templates/></svrl:active-pattern><axsl:apply-templates select="/" mode="M8"/><svrl:active-pattern><axsl:attribute name="id">idm140727361409216</axsl:attribute><axsl:attribute name="name">idm140727361409216</axsl:attribute><axsl:apply-templates/></svrl:active-pattern><axsl:apply-templates select="/" mode="M9"/><svrl:active-pattern><axsl:attribute name="id">idm140727361408400</axsl:attribute><axsl:attribute name="name">idm140727361408400</axsl:attribute><axsl:apply-templates/></svrl:active-pattern><axsl:apply-templates select="/" mode="M10"/><svrl:active-pattern><axsl:attribute name="id">idm140727361281760</axsl:attribute><axsl:attribute name="name">idm140727361281760</axsl:attribute><axsl:apply-templates/></svrl:active-pattern><axsl:apply-templates select="/" mode="M11"/><svrl:active-pattern><axsl:attribute name="id">idm140727361226304</axsl:attribute><axsl:attribute name="name">idm140727361226304</axsl:attribute><axsl:apply-templates/></svrl:active-pattern><axsl:apply-templates select="/" mode="M12"/><svrl:active-pattern><axsl:attribute name="id">idm140727361395904</axsl:attribute><axsl:attribute name="name">idm140727361395904</axsl:attribute><axsl:apply-templates/></svrl:active-pattern><axsl:apply-templates select="/" mode="M13"/><svrl:active-pattern><axsl:attribute name="id">idm140727361371408</axsl:attribute><axsl:attribute name="name">idm140727361371408</axsl:attribute><axsl:apply-templates/></svrl:active-pattern><axsl:apply-templates select="/" mode="M14"/><svrl:active-pattern><axsl:attribute name="id">xdpd-mgmt</axsl:attribute><axsl:attribute name="name">xdpd-mgmt</axsl:attribute><axsl:apply-templates/></svrl:active-pattern><axsl:apply-templates select="/" mode="M15"/></svrl:schematron-output></axsl:template>

<!--SCHEMATRON PATTERNS-->
<axsl:param name="root" select="/nc:data"/>

<!--PATTERN of-config1.1.1-->


	<!--RULE -->
<axsl:template match="/nc:data/ofc:capable-switch/ofc:configuration-points/ofc:configuration-point" priority="1026" mode="M5"><svrl:fired-rule xmlns:svrl="http://purl.oclc.org/dsdl/svrl" context="/nc:data/ofc:capable-switch/ofc:configuration-points/ofc:configuration-point"/>

		<!--REPORT -->
<axsl:if test="preceding-sibling::ofc:configuration-point[ofc:id=current()/ofc:id]"><svrl:successful-report xmlns:svrl="http://purl.oclc.org/dsdl/svrl" xmlns:xs="http://www.w3.org/2001/XMLSchema" xmlns:schold="http://www.ascc.net/xml/schematron" test="preceding-sibling::ofc:configuration-point[ofc:id=current()/ofc:id]"><axsl:attribute name="location"><axsl:apply-templates select="." mode="schematron-get-full-path"/></axsl:attribute><svrl:text>Duplicate key "ofc:id"</svrl:text></svrl:successful-report></axsl:if><axsl:apply-templates select="*|comment()|processing-instruction()" mode="M5"/></axsl:template>

	<!--RULE -->
<axsl:template match="/nc:data/ofc:capable-switch/ofc:resources/ofc:port" priority="1025" mode="M5"><svrl:fired-rule xmlns:svrl="http://purl.oclc.org/dsdl/svrl" context="/nc:data/ofc:capable-switch/ofc:resources/ofc:port"/>

		<!--REPORT -->
<axsl:if test="preceding-sibling::ofc:port[ofc:resource-id=current()/ofc:resource-id]"><svrl:successful-report xmlns:svrl="http://purl.oclc.org/dsdl/svrl" xmlns:xs="http://www.w3.org/2001/XMLSchema" xmlns:schold="http://www.ascc.net/xml/schematron" test="preceding-sibling::ofc:port[ofc:resource-id=current()/ofc:resource-id]"><axsl:attribute name="location"><axsl:apply-templates select="." mode="schematron-get-full-path"/></axsl:attribute><svrl:text>Duplicate key "ofc:resource-id"</svrl:text></svrl:successful-report></axsl:if>

		<!--ASSERT -->
<axsl:choose><axsl:when test="ofc:features/ofc:current/ofc:rate != 'other' or (count(ofc:current-rate) = 1 and count(ofc:max-rate) = 1 and  ofc:current-rate &gt; 0 and ofc:max-rate &gt; 0)"/><axsl:otherwise><svrl:failed-assert xmlns:svrl="http://purl.oclc.org/dsdl/svrl" xmlns:xs="http://www.w3.org/2001/XMLSchema" xmlns:schold="http://www.ascc.net/xml/schematron" test="ofc:features/ofc:current/ofc:rate != 'other' or (count(ofc:current-rate) = 1 and count(ofc:max-rate) = 1 and ofc:current-rate &gt; 0 and ofc:max-rate &gt; 0)"><axsl:attribute name="location"><axsl:apply-templates select="." mode="schematron-get-full-path"/></axsl:attribute><svrl:text>current-rate and max-rate must be
specified and greater than 0 if rate equals 'other'</svrl:text></svrl:failed-assert></axsl:otherwise></axsl:choose><axsl:apply-templates select="*|comment()|processing-instruction()" mode="M5"/></axsl:template>

	<!--RULE -->
<axsl:template match="/nc:data/ofc:capable-switch/ofc:resources/ofc:port/ofc:current-rate" priority="1024" mode="M5"><svrl:fired-rule xmlns:svrl="http://purl.oclc.org/dsdl/svrl" context="/nc:data/ofc:capable-switch/ofc:resources/ofc:port/ofc:current-rate"/>

		<!--ASSERT -->
<axsl:choose><axsl:when test="ancestor-or-self::*[processing-instruction('dsrl')] or (../ofc:features/ofc:current/ofc:rate='other')"/><axsl:otherwise><svrl:failed-assert xmlns:svrl="http://purl.oclc.org/dsdl/svrl" xmlns:xs="http://www.w3.org/2001/XMLSchema" xmlns:schold="http://www.ascc.net/xml/schematron" test="ancestor-or-self::*[processing-instruction('dsrl')] or (../ofc:features/ofc:current/ofc:rate='other')"><axsl:attribute name="location"><axsl:apply-templates select="." mode="schematron-get-full-path"/></axsl:attribute><svrl:text>Node "ofc:current-rate" is only valid when "../ofc:features/ofc:current/ofc:rate='other'"</svrl:text></svrl:failed-assert></axsl:otherwise></axsl:choose><axsl:apply-templates select="*|comment()|processing-instruction()" mode="M5"/></axsl:template>

	<!--RULE -->
<axsl:template match="/nc:data/ofc:capable-switch/ofc:resources/ofc:port/ofc:max-rate" priority="1023" mode="M5"><svrl:fired-rule xmlns:svrl="http://purl.oclc.org/dsdl/svrl" context="/nc:data/ofc:capable-switch/ofc:resources/ofc:port/ofc:max-rate"/>

		<!--ASSERT -->
<axsl:choose><axsl:when test="ancestor-or-self::*[processing-instruction('dsrl')] or (../ofc:features/ofc:current/ofc:rate='other')"/><axsl:otherwise><svrl:failed-assert xmlns:svrl="http://purl.oclc.org/dsdl/svrl" xmlns:xs="http://www.w3.org/2001/XMLSchema" xmlns:schold="http://www.ascc.net/xml/schematron" test="ancestor-or-self::*[processing-instruction('dsrl')] or (../ofc:features/ofc:current/ofc:rate='other')"><axsl:attribute name="location"><axsl:apply-templates select="." mode="schematron-get-full-path"/></axsl:attribute><svrl:text>Node "ofc:max-rate" is only valid when "../ofc:features/ofc:current/ofc:rate='other'"</svrl:text></svrl:failed-assert></axsl:otherwise></axsl:choose><axsl:apply-templates select="*|comment()|processing-instruction()" mode="M5"/></axsl:template>

	<!--RULE -->
<axsl:template match="/nc:data/ofc:capable-switch/ofc:resources/ofc:queue" priority="1022" mode="M5"><svrl:fired-rule xmlns:svrl="http://purl.oclc.org/dsdl/svrl" context="/nc:data/ofc:capable-switch/ofc:resources/ofc:queue"/>

		<!--REPORT -->
<axsl:if test="preceding-sibling::ofc:queue[ofc:resource-id=current()/ofc:resource-id]"><svrl:successful-report xmlns:svrl="http://purl.oclc.org/dsdl/svrl" xmlns:xs="http://www.w3.org/2001/XMLSchema" xmlns:schold="http://www.ascc.net/xml/schematron" test="preceding-sibling::ofc:queue[ofc:resource-id=current()/ofc:resource-id]"><axsl:attribute name="location"><axsl:apply-templates select="." mode="schematron-get-full-path"/></axsl:attribute><svrl:text>Duplicate key "ofc:resource-id"</svrl:text></svrl:successful-report></axsl:if><axsl:apply-templates select="*|comment()|processing-instruction()" mode="M5"/></axsl:template>

	<!--RULE -->
<axsl:template match="/nc:data/ofc:capable-switch/ofc:resources/ofc:queue/ofc:port" priority="1021" mode="M5"><svrl:fired-rule xmlns:svrl="http://purl.oclc.org/dsdl/svrl" context="/nc:data/ofc:capable-switch/ofc:resources/ofc:queue/ofc:port"/>

		<!--REPORT -->
<axsl:if test="not($root/ofc:capable-switch/ofc:resources/ofc:port/ofc:resource-id=.)"><svrl:successful-report xmlns:svrl="http://purl.oclc.org/dsdl/svrl" xmlns:xs="http://www.w3.org/2001/XMLSchema" xmlns:schold="http://www.ascc.net/xml/schematron" test="not($root/ofc:capable-switch/ofc:resources/ofc:port/ofc:resource-id=.)"><axsl:attribute name="location"><axsl:apply-templates select="." mode="schematron-get-full-path"/></axsl:attribute><svrl:text>Leaf "/nc:data/ofc:capable-switch/ofc:resources/ofc:port/ofc:resource-id" does not exist for leafref value "<axsl:text/><axsl:value-of select="."/><axsl:text/>"</svrl:text></svrl:successful-report></axsl:if><axsl:apply-templates select="*|comment()|processing-instruction()" mode="M5"/></axsl:template>

	<!--RULE -->
<axsl:template match="/nc:data/ofc:capable-switch/ofc:resources/ofc:queue/ofc:properties/ofc:experimenter" priority="1020" mode="M5"><svrl:fired-rule xmlns:svrl="http://purl.oclc.org/dsdl/svrl" context="/nc:data/ofc:capable-switch/ofc:resources/ofc:queue/ofc:properties/ofc:experimenter"/>

		<!--REPORT -->
<axsl:if test=". = preceding-sibling::ofc:experimenter"><svrl:successful-report xmlns:svrl="http://purl.oclc.org/dsdl/svrl" xmlns:xs="http://www.w3.org/2001/XMLSchema" xmlns:schold="http://www.ascc.net/xml/schematron" test=". = preceding-sibling::ofc:experimenter"><axsl:attribute name="location"><axsl:apply-templates select="." mode="schematron-get-full-path"/></axsl:attribute><svrl:text>Duplicate leaf-list entry "<axsl:text/><axsl:value-of select="."/><axsl:text/>".</svrl:text></svrl:successful-report></axsl:if><axsl:apply-templates select="*|comment()|processing-instruction()" mode="M5"/></axsl:template>

	<!--RULE -->
<axsl:template match="/nc:data/ofc:capable-switch/ofc:resources/ofc:owned-certificate" priority="1019" mode="M5"><svrl:fired-rule xmlns:svrl="http://purl.oclc.org/dsdl/svrl" context="/nc:data/ofc:capable-switch/ofc:resources/ofc:owned-certificate"/>

		<!--REPORT -->
<axsl:if test="preceding-sibling::ofc:owned-certificate[ofc:resource-id=current()/ofc:resource-id]"><svrl:successful-report xmlns:svrl="http://purl.oclc.org/dsdl/svrl" xmlns:xs="http://www.w3.org/2001/XMLSchema" xmlns:schold="http://www.ascc.net/xml/schematron" test="preceding-sibling::ofc:owned-certificate[ofc:resource-id=current()/ofc:resource-id]"><axsl:attribute name="location"><axsl:apply-templates select="." mode="schematron-get-full-path"/></axsl:attribute><svrl:text>Duplicate key "ofc:resource-id"</svrl:text></svrl:successful-report></axsl:if><axsl:apply-templates select="*|comment()|processing-instruction()" mode="M5"/></axsl:template>

	<!--RULE -->
<axsl:template match="/nc:data/ofc:capable-switch/ofc:resources/ofc:external-certificate" priority="1018" mode="M5"><svrl:fired-rule xmlns:svrl="http://purl.oclc.org/dsdl/svrl" context="/nc:data/ofc:capable-switch/ofc:resources/ofc:external-certificate"/>

		<!--REPORT -->
<axsl:if test="preceding-sibling::ofc:external-certificate[ofc:resource-id=current()/ofc:resource-id]"><svrl:successful-report xmlns:svrl="http://purl.oclc.org/dsdl/svrl" xmlns:xs="http://www.w3.org/2001/XMLSchema" xmlns:schold="http://www.ascc.net/xml/schematron" test="preceding-sibling::ofc:external-certificate[ofc:resource-id=current()/ofc:resource-id]"><axsl:attribute name="location"><axsl:apply-templates select="." mode="schematron-get-full-path"/></axsl:attribute><svrl:text>Duplicate key "ofc:resource-id"</svrl:text></svrl:successful-report></axsl:if><axsl:apply-templates select="*|comment()|processing-instruction()" mode="M5"/></axsl:template>

	<!--RULE -->
<axsl:template match="/nc:data/ofc:capable-switch/ofc:resources/ofc:flow-table" priority="1017" mode="M5"><svrl:fired-rule xmlns:svrl="http://purl.oclc.org/dsdl/svrl" context="/nc:data/ofc:capable-switch/ofc:resources/ofc:flow-table"/>

		<!--REPORT -->
<axsl:if test="preceding-sibling::ofc:flow-table[ofc:resource-id=current()/ofc:resource-id]"><svrl:successful-report xmlns:svrl="http://purl.oclc.org/dsdl/svrl" xmlns:xs="http://www.w3.org/2001/XMLSchema" xmlns:schold="http://www.ascc.net/xml/schematron" test="preceding-sibling::ofc:flow-table[ofc:resource-id=current()/ofc:resource-id]"><axsl:attribute name="location"><axsl:apply-templates select="." mode="schematron-get-full-path"/></axsl:attribute><svrl:text>Duplicate key "ofc:resource-id"</svrl:text></svrl:successful-report></axsl:if><axsl:apply-templates select="*|comment()|processing-instruction()" mode="M5"/></axsl:template>

	<!--RULE -->
<axsl:template match="/nc:data/ofc:capable-switch/ofc:resources/ofc:flow-table/ofc:next-tables/ofc:table-id" priority="1016" mode="M5"><svrl:fired-rule xmlns:svrl="http://purl.oclc.org/dsdl/svrl" context="/nc:data/ofc:capable-switch/ofc:resources/ofc:flow-table/ofc:next-tables/ofc:table-id"/>

		<!--REPORT -->
<axsl:if test=". = preceding-sibling::ofc:table-id"><svrl:successful-report xmlns:svrl="http://purl.oclc.org/dsdl/svrl" xmlns:xs="http://www.w3.org/2001/XMLSchema" xmlns:schold="http://www.ascc.net/xml/schematron" test=". = preceding-sibling::ofc:table-id"><axsl:attribute name="location"><axsl:apply-templates select="." mode="schematron-get-full-path"/></axsl:attribute><svrl:text>Duplicate leaf-list entry "<axsl:text/><axsl:value-of select="."/><axsl:text/>".</svrl:text></svrl:successful-report></axsl:if><axsl:apply-templates select="*|comment()|processing-instruction()" mode="M5"/></axsl:template>

	<!--RULE -->
<axsl:template match="/nc:data/ofc:capable-switch/ofc:resources/ofc:flow-table/ofc:instructions/ofc:type" priority="1015" mode="M5"><svrl:fired-rule xmlns:svrl="http://purl.oclc.org/dsdl/svrl" context="/nc:data/ofc:capable-switch/ofc:resources/ofc:flow-table/ofc:instructions/ofc:type"/>

		<!--REPORT -->
<axsl:if test=". = preceding-sibling::ofc:type"><svrl:successful-report xmlns:svrl="http://purl.oclc.org/dsdl/svrl" xmlns:xs="http://www.w3.org/2001/XMLSchema" xmlns:schold="http://www.ascc.net/xml/schematron" test=". = preceding-sibling::ofc:type"><axsl:attribute name="location"><axsl:apply-templates select="." mode="schematron-get-full-path"/></axsl:attribute><svrl:text>Duplicate leaf-list entry "<axsl:text/><axsl:value-of select="."/><axsl:text/>".</svrl:text></svrl:successful-report></axsl:if><axsl:apply-templates select="*|comment()|processing-instruction()" mode="M5"/></axsl:template>

	<!--RULE -->
<axsl:template match="/nc:data/ofc:capable-switch/ofc:resources/ofc:flow-table/ofc:matches/ofc:type" priority="1014" mode="M5"><svrl:fired-rule xmlns:svrl="http://purl.oclc.org/dsdl/svrl" context="/nc:data/ofc:capable-switch/ofc:resources/ofc:flow-table/ofc:matches/ofc:type"/>

		<!--REPORT -->
<axsl:if test=". = preceding-sibling::ofc:type"><svrl:successful-report xmlns:svrl="http://purl.oclc.org/dsdl/svrl" xmlns:xs="http://www.w3.org/2001/XMLSchema" xmlns:schold="http://www.ascc.net/xml/schematron" test=". = preceding-sibling::ofc:type"><axsl:attribute name="location"><axsl:apply-templates select="." mode="schematron-get-full-path"/></axsl:attribute><svrl:text>Duplicate leaf-list entry "<axsl:text/><axsl:value-of select="."/><axsl:text/>".</svrl:text></svrl:successful-report></axsl:if><axsl:apply-templates select="*|comment()|processing-instruction()" mode="M5"/></axsl:template>

	<!--RULE -->
<axsl:template match="/nc:data/ofc:capable-switch/ofc:resources/ofc:flow-table/ofc:write-actions/ofc:type" priority="1013" mode="M5"><svrl:fired-rule xmlns:svrl="http://purl.oclc.org/dsdl/svrl" context="/nc:data/ofc:capable-switch/ofc:resources/ofc:flow-table/ofc:write-actions/ofc:type"/>

		<!--REPORT -->
<axsl:if test=". = preceding-sibling::ofc:type"><svrl:successful-report xmlns:svrl="http://purl.oclc.org/dsdl/svrl" xmlns:xs="http://www.w3.org/2001/XMLSchema" xmlns:schold="http://www.ascc.net/xml/schematron" test=". = preceding-sibling::ofc:type"><axsl:attribute name="location"><axsl:apply-templates select="." mode="schematron-get-full-path"/></axsl:attribute><svrl:text>Duplicate leaf-list entry "<axsl:text/><axsl:value-of select="."/><axsl:text/>".</svrl:text></svrl:successful-report></axsl:if><axsl:apply-templates select="*|comment()|processing-instruction()" mode="M5"/></axsl:template>

	<!--RULE -->
<axsl:template match="/nc:data/ofc:capable-switch/ofc:resources/ofc:flow-table/ofc:apply-actions/ofc:type" priority="1012" mode="M5"><svrl:fired-rule xmlns:svrl="http://purl.oclc.org/dsdl/svrl" context="/nc:data/ofc:capable-switch/ofc:resources/ofc:flow-table/ofc:apply-actions/ofc:type"/>

		<!--REPORT -->
<axsl:if test=". = preceding-sibling::ofc:type"><svrl:successful-report xmlns:svrl="http://purl.oclc.org/dsdl/svrl" xmlns:xs="http://www.w3.org/2001/XMLSchema" xmlns:schold="http://www.ascc.net/xml/schematron" test=". = preceding-sibling::ofc:type"><axsl:attribute name="location"><axsl:apply-templates select="." mode="schematron-get-full-path"/></axsl:attribute><svrl:text>Duplicate leaf-list entry "<axsl:text/><axsl:value-of select="."/><axsl:text/>".</svrl:text></svrl:successful-report></axsl:if><axsl:apply-templates select="*|comment()|processing-instruction()" mode="M5"/></axsl:template>

	<!--RULE -->
<axsl:template match="/nc:data/ofc:capable-switch/ofc:resources/ofc:flow-table/ofc:write-setfields/ofc:type" priority="1011" mode="M5"><svrl:fired-rule xmlns:svrl="http://purl.oclc.org/dsdl/svrl" context="/nc:data/ofc:capable-switch/ofc:resources/ofc:flow-table/ofc:write-setfields/ofc:type"/>

		<!--REPORT -->
<axsl:if test=". = preceding-sibling::ofc:type"><svrl:successful-report xmlns:svrl="http://purl.oclc.org/dsdl/svrl" xmlns:xs="http://www.w3.org/2001/XMLSchema" xmlns:schold="http://www.ascc.net/xml/schematron" test=". = preceding-sibling::ofc:type"><axsl:attribute name="location"><axsl:apply-templates select="." mode="schematron-get-full-path"/></axsl:attribute><svrl:text>Duplicate leaf-list entry "<axsl:text/><axsl:value-of select="."/><axsl:text/>".</svrl:text></svrl:successful-report></axsl:if><axsl:apply-templates select="*|comment()|processing-instruction()" mode="M5"/></axsl:template>

	<!--RULE -->
<axsl:template match="/nc:data/ofc:capable-switch/ofc:resources/ofc:flow-table/ofc:apply-setfields/ofc:type" priority="1010" mode="M5"><svrl:fired-rule xmlns:svrl="http://purl.oclc.org/dsdl/svrl" context="/nc:data/ofc:capable-switch/ofc:resources/ofc:flow-table/ofc:apply-setfields/ofc:type"/>

		<!--REPORT -->
<axsl:if test=". = preceding-sibling::ofc:type"><svrl:successful-report xmlns:svrl="http://purl.oclc.org/dsdl/svrl" xmlns:xs="http://www.w3.org/2001/XMLSchema" xmlns:schold="http://www.ascc.net/xml/schematron" test=". = preceding-sibling::ofc:type"><axsl:attribute name="location"><axsl:apply-templates select="." mode="schematron-get-full-path"/></axsl:attribute><svrl:text>Duplicate leaf-list entry "<axsl:text/><axsl:value-of select="."/><axsl:text/>".</svrl:text></svrl:successful-report></axsl:if><axsl:apply-templates select="*|comment()|processing-instruction()" mode="M5"/></axsl:template>

	<!--RULE -->
<axsl:template match="/nc:data/ofc:capable-switch/ofc:resources/ofc:flow-table/ofc:wildcards/ofc:type" priority="1009" mode="M5"><svrl:fired-rule xmlns:svrl="http://purl.oclc.org/dsdl/svrl" context="/nc:data/ofc:capable-switch/ofc:resources/ofc:flow-table/ofc:wildcards/ofc:type"/>

		<!--REPORT -->
<axsl:if test=". = preceding-sibling::ofc:type"><svrl:successful-report xmlns:svrl="http://purl.oclc.org/dsdl/svrl" xmlns:xs="http://www.w3.org/2001/XMLSchema" xmlns:schold="http://www.ascc.net/xml/schematron" test=". = preceding-sibling::ofc:type"><axsl:attribute name="location"><axsl:apply-templates select="." mode="schematron-get-full-path"/></axsl:attribute><svrl:text>Duplicate leaf-list entry "<axsl:text/><axsl:value-of select="."/><axsl:text/>".</svrl:text></svrl:successful-report></axsl:if><axsl:apply-templates select="*|comment()|processing-instruction()" mode="M5"/></axsl:template>

	<!--RULE -->
<axsl:template match="/nc:data/ofc:capable-switch/ofc:logical-switches/ofc:switch" priority="1008" mode="M5"><svrl:fired-rule xmlns:svrl="http://purl.oclc.org/dsdl/svrl" context="/nc:data/ofc:capable-switch/ofc:logical-switches/ofc:switch"/>

		<!--REPORT -->
<axsl:if test="preceding-sibling::ofc:switch[ofc:id=current()/ofc:id]"><svrl:successful-report xmlns:svrl="http://purl.oclc.org/dsdl/svrl" xmlns:xs="http://www.w3.org/2001/XMLSchema" xmlns:schold="http://www.ascc.net/xml/schematron" test="preceding-sibling::ofc:switch[ofc:id=current()/ofc:id]"><axsl:attribute name="location"><axsl:apply-templates select="." mode="schematron-get-full-path"/></axsl:attribute><svrl:text>Duplicate key "ofc:id"</svrl:text></svrl:successful-report></axsl:if><axsl:apply-templates select="*|comment()|processing-instruction()" mode="M5"/></axsl:template>

	<!--RULE -->
<axsl:template match="/nc:data/ofc:capable-switch/ofc:logical-switches/ofc:switch/ofc:controllers/ofc:controller" priority="1007" mode="M5"><svrl:fired-rule xmlns:svrl="http://purl.oclc.org/dsdl/svrl" context="/nc:data/ofc:capable-switch/ofc:logical-switches/ofc:switch/ofc:controllers/ofc:controller"/>

		<!--REPORT -->
<axsl:if test="preceding-sibling::ofc:controller[ofc:id=current()/ofc:id]"><svrl:successful-report xmlns:svrl="http://purl.oclc.org/dsdl/svrl" xmlns:xs="http://www.w3.org/2001/XMLSchema" xmlns:schold="http://www.ascc.net/xml/schematron" test="preceding-sibling::ofc:controller[ofc:id=current()/ofc:id]"><axsl:attribute name="location"><axsl:apply-templates select="." mode="schematron-get-full-path"/></axsl:attribute><svrl:text>Duplicate key "ofc:id"</svrl:text></svrl:successful-report></axsl:if><axsl:apply-templates select="*|comment()|processing-instruction()" mode="M5"/></axsl:template>

	<!--RULE -->
<axsl:template match="/nc:data/ofc:capable-switch/ofc:logical-switches/ofc:switch/ofc:controllers/ofc:controller/ofc:state/ofc:supported-versions" priority="1006" mode="M5"><svrl:fired-rule xmlns:svrl="http://purl.oclc.org/dsdl/svrl" context="/nc:data/ofc:capable-switch/ofc:logical-switches/ofc:switch/ofc:controllers/ofc:controller/ofc:state/ofc:supported-versions"/>

		<!--REPORT -->
<axsl:if test=". = preceding-sibling::ofc:supported-versions"><svrl:successful-report xmlns:svrl="http://purl.oclc.org/dsdl/svrl" xmlns:xs="http://www.w3.org/2001/XMLSchema" xmlns:schold="http://www.ascc.net/xml/schematron" test=". = preceding-sibling::ofc:supported-versions"><axsl:attribute name="location"><axsl:apply-templates select="." mode="schematron-get-full-path"/></axsl:attribute><svrl:text>Duplicate leaf-list entry "<axsl:text/><axsl:value-of select="."/><axsl:text/>".</svrl:text></svrl:successful-report></axsl:if><axsl:apply-templates select="*|comment()|processing-instruction()" mode="M5"/></axsl:template>

	<!--RULE -->
<axsl:template match="/nc:data/ofc:capable-switch/ofc:logical-switches/ofc:switch/ofc:resources/ofc:port" priority="1005" mode="M5"><svrl:fired-rule xmlns:svrl="http://purl.oclc.org/dsdl/svrl" context="/nc:data/ofc:capable-switch/ofc:logical-switches/ofc:switch/ofc:resources/ofc:port"/>

		<!--REPORT -->
<axsl:if test=". = preceding-sibling::ofc:port"><svrl:successful-report xmlns:svrl="http://purl.oclc.org/dsdl/svrl" xmlns:xs="http://www.w3.org/2001/XMLSchema" xmlns:schold="http://www.ascc.net/xml/schematron" test=". = preceding-sibling::ofc:port"><axsl:attribute name="location"><axsl:apply-templates select="." mode="schematron-get-full-path"/></axsl:attribute><svrl:text>Duplicate leaf-list entry "<axsl:text/><axsl:value-of select="."/><axsl:text/>".</svrl:text></svrl:successful-report></axsl:if>

		<!--REPORT -->
<axsl:if test="not($root/ofc:capable-switch/ofc:resources/ofc:port/ofc:resource-id=.)"><svrl:successful-report xmlns:svrl="http://purl.oclc.org/dsdl/svrl" xmlns:xs="http://www.w3.org/2001/XMLSchema" xmlns:schold="http://www.ascc.net/xml/schematron" test="not($root/ofc:capable-switch/ofc:resources/ofc:port/ofc:resource-id=.)"><axsl:attribute name="location"><axsl:apply-templates select="." mode="schematron-get-full-path"/></axsl:attribute><svrl:text>Leaf "/nc:data/ofc:capable-switch/ofc:resources/ofc:port/ofc:resource-id" does not exist for leafref value "<axsl:text/><axsl:value-of select="."/><axsl:text/>"</svrl:text></svrl:successful-report></axsl:if><axsl:apply-templates select="*|comment()|processing-instruction()" mode="M5"/></axsl:template>

	<!--RULE -->
<axsl:template match="/nc:data/ofc:capable-switch/ofc:logical-switches/ofc:switch/ofc:resources/ofc:queue" priority="1004" mode="M5"><svrl:fired-rule xmlns:svrl="http://purl.oclc.org/dsdl/svrl" context="/nc:data/ofc:capable-switch/ofc:logical-switches/ofc:switch/ofc:resources/ofc:queue"/>

		<!--REPORT -->
<axsl:if test=". = preceding-sibling::ofc:queue"><svrl:successful-report xmlns:svrl="http://purl.oclc.org/dsdl/svrl" xmlns:xs="http://www.w3.org/2001/XMLSchema" xmlns:schold="http://www.ascc.net/xml/schematron" test=". = preceding-sibling::ofc:queue"><axsl:attribute name="location"><axsl:apply-templates select="." mode="schematron-get-full-path"/></axsl:attribute><svrl:text>Duplicate leaf-list entry "<axsl:text/><axsl:value-of select="."/><axsl:text/>".</svrl:text></svrl:successful-report></axsl:if>

		<!--REPORT -->
<axsl:if test="not($root/ofc:capable-switch/ofc:resources/ofc:queue/ofc:resource-id=.)"><svrl:successful-report xmlns:svrl="http://purl.oclc.org/dsdl/svrl" xmlns:xs="http://www.w3.org/2001/XMLSchema" xmlns:schold="http://www.ascc.net/xml/schematron" test="not($root/ofc:capable-switch/ofc:resources/ofc:queue/ofc:resource-id=.)"><axsl:attribute name="location"><axsl:apply-templates select="." mode="schematron-get-full-path"/></axsl:attribute><svrl:text>Leaf "/nc:data/ofc:capable-switch/ofc:resources/ofc:queue/ofc:resource-id" does not exist for leafref value "<axsl:text/><axsl:value-of select="."/><axsl:text/>"</svrl:text></svrl:successful-report></axsl:if><axsl:apply-templates select="*|comment()|processing-instruction()" mode="M5"/></axsl:template>

	<!--RULE -->
<axsl:template match="/nc:data/ofc:capable-switch/ofc:logical-switches/ofc:switch/ofc:resources/ofc:certificate" priority="1003" mode="M5"><svrl:fired-rule xmlns:svrl="http://purl.oclc.org/dsdl/svrl" context="/nc:data/ofc:capable-switch/ofc:logical-switches/ofc:switch/ofc:resources/ofc:certificate"/>

		<!--REPORT -->
<axsl:if test="not($root/ofc:capable-switch/ofc:resources/ofc:owned-certificate/ofc:resource-id=.)"><svrl:successful-report xmlns:svrl="http://purl.oclc.org/dsdl/svrl" xmlns:xs="http://www.w3.org/2001/XMLSchema" xmlns:schold="http://www.ascc.net/xml/schematron" test="not($root/ofc:capable-switch/ofc:resources/ofc:owned-certificate/ofc:resource-id=.)"><axsl:attribute name="location"><axsl:apply-templates select="." mode="schematron-get-full-path"/></axsl:attribute><svrl:text>Leaf "/nc:data/ofc:capable-switch/ofc:resources/ofc:owned-certificate/ofc:resource-id" does not exist for leafref value "<axsl:text/><axsl:value-of select="."/><axsl:text/>"</svrl:text></svrl:successful-report></axsl:if><axsl:apply-templates select="*|comment()|processing-instruction()" mode="M5"/></axsl:template>

	<!--RULE -->
<axsl:template match="/nc:data/ofc:capable-switch/ofc:logical-switches/ofc:switch/ofc:resources/ofc:flow-table" priority="1002" mode="M5"><svrl:fired-rule xmlns:svrl="http://purl.oclc.org/dsdl/svrl" context="/nc:data/ofc:capable-switch/ofc:logical-switches/ofc:switch/ofc:resources/ofc:flow-table"/>

		<!--REPORT -->
<axsl:if test=". = preceding-sibling::ofc:flow-table"><svrl:successful-report xmlns:svrl="http://purl.oclc.org/dsdl/svrl" xmlns:xs="http://www.w3.org/2001/XMLSchema" xmlns:schold="http://www.ascc.net/xml/schematron" test=". = preceding-sibling::ofc:flow-table"><axsl:attribute name="location"><axsl:apply-templates select="." mode="schematron-get-full-path"/></axsl:attribute><svrl:text>Duplicate leaf-list entry "<axsl:text/><axsl:value-of select="."/><axsl:text/>".</svrl:text></svrl:successful-report></axsl:if>

		<!--REPORT -->
<axsl:if test="not($root/ofc:capable-switch/ofc:resources/ofc:flow-table/ofc:resource-id=.)"><svrl:successful-report xmlns:svrl="http://purl.oclc.org/dsdl/svrl" xmlns:xs="http://www.w3.org/2001/XMLSchema" xmlns:schold="http://www.ascc.net/xml/schematron" test="not($root/ofc:capable-switch/ofc:resources/ofc:flow-table/ofc:resource-id=.)"><axsl:attribute name="location"><axsl:apply-templates select="." mode="schematron-get-full-path"/></axsl:attribute><svrl:text>Leaf "/nc:data/ofc:capable-switch/ofc:resources/ofc:flow-table/ofc:resource-id" does not exist for leafref value "<axsl:text/><axsl:value-of select="."/><axsl:text/>"</svrl:text></svrl:successful-report></axsl:if><axsl:apply-templates select="*|comment()|processing-instruction()" mode="M5"/></axsl:template>

	<!--RULE -->
<axsl:template match="/nc:data/ofc:capable-switch/xdpd-mgmt:cross-connections/xdpd-mgmt:cross-connection" priority="1001" mode="M5"><svrl:fired-rule xmlns:svrl="http://purl.oclc.org/dsdl/svrl" context="/nc:data/ofc:capable-switch/xdpd-mgmt:cross-connections/xdpd-mgmt:cross-connection"/>

		<!--REPORT -->
<axsl:if test="preceding-sibling::xdpd-mgmt:cross-connection[xdpd-mgmt:name=current()/xdpd-mgmt:name]"><svrl:successful-report xmlns:svrl="http://purl.oclc.org/dsdl/svrl" xmlns:xs="http://www.w3.org/2001/XMLSchema" xmlns:schold="http://www.ascc.net/xml/schematron" test="preceding-sibling::xdpd-mgmt:cross-connection[xdpd-mgmt:name=current()/xdpd-mgmt:name]"><axsl:attribute name="location"><axsl:apply-templates select="." mode="schematron-get-full-path"/></axsl:attribute><svrl:text>Duplicate key "xdpd-mgmt:name"</svrl:text></svrl:successful-report></axsl:if><axsl:apply-templates select="*|comment()|processing-instruction()" mode="M5"/></axsl:template>

	<!--RULE -->
<axsl:template match="/nc:data/ofc:capable-switch/xdpd-mgmt:cross-connections/xdpd-mgmt:cross-connection/xdpd-mgmt:switches" priority="1000" mode="M5"><svrl:fired-rule xmlns:svrl="http://purl.oclc.org/dsdl/svrl" context="/nc:data/ofc:capable-switch/xdpd-mgmt:cross-connections/xdpd-mgmt:cross-connection/xdpd-mgmt:switches"/>

		<!--ASSERT -->
<axsl:choose><axsl:when test="preceding-sibling::xdpd-mgmt:switches or count(../xdpd-mgmt:switches)&gt;=2"/><axsl:otherwise><svrl:failed-assert xmlns:svrl="http://purl.oclc.org/dsdl/svrl" xmlns:xs="http://www.w3.org/2001/XMLSchema" xmlns:schold="http://www.ascc.net/xml/schematron" test="preceding-sibling::xdpd-mgmt:switches or count(../xdpd-mgmt:switches)&gt;=2"><axsl:attribute name="location"><axsl:apply-templates select="." mode="schematron-get-full-path"/></axsl:attribute><svrl:text>Number of leaf-list entries must be at least 2</svrl:text></svrl:failed-assert></axsl:otherwise></axsl:choose>

		<!--REPORT -->
<axsl:if test=". = preceding-sibling::xdpd-mgmt:switches"><svrl:successful-report xmlns:svrl="http://purl.oclc.org/dsdl/svrl" xmlns:xs="http://www.w3.org/2001/XMLSchema" xmlns:schold="http://www.ascc.net/xml/schematron" test=". = preceding-sibling::xdpd-mgmt:switches"><axsl:attribute name="location"><axsl:apply-templates select="." mode="schematron-get-full-path"/></axsl:attribute><svrl:text>Duplicate leaf-list entry "<axsl:text/><axsl:value-of select="."/><axsl:text/>".</svrl:text></svrl:successful-report></axsl:if>

		<!--ASSERT -->
<axsl:choose><axsl:when test="preceding-sibling::xdpd-mgmt:switches or count(../xdpd-mgmt:switches)&lt;=2"/><axsl:otherwise><svrl:failed-assert xmlns:svrl="http://purl.oclc.org/dsdl/svrl" xmlns:xs="http://www.w3.org/2001/XMLSchema" xmlns:schold="http://www.ascc.net/xml/schematron" test="preceding-sibling::xdpd-mgmt:switches or count(../xdpd-mgmt:switches)&lt;=2"><axsl:attribute name="location"><axsl:apply-templates select="." mode="schematron-get-full-path"/></axsl:attribute><svrl:text>Number of leaf-list entries must be at most 2</svrl:text></svrl:failed-assert></axsl:otherwise></axsl:choose>

		<!--REPORT -->
<axsl:if test="not($root/ofc:capable-switch/ofc:logical-switches/ofc:switch/ofc:id=.)"><svrl:successful-report xmlns:svrl="http://purl.oclc.org/dsdl/svrl" xmlns:xs="http://www.w3.org/2001/XMLSchema" xmlns:schold="http://www.ascc.net/xml/schematron" test="not($root/ofc:capable-switch/ofc:logical-switches/ofc:switch/ofc:id=.)"><axsl:attribute name="location"><axsl:apply-templates select="." mode="schematron-get-full-path"/></axsl:attribute><svrl:text>Leaf "/nc:data/ofc:capable-switch/ofc:logical-switches/ofc:switch/ofc:id" does not exist for leafref value "<axsl:text/><axsl:value-of select="."/><axsl:text/>"</svrl:text></svrl:successful-report></axsl:if><axsl:apply-templates select="*|comment()|processing-instruction()" mode="M5"/></axsl:template><axsl:template match="text()" priority="-1" mode="M5"/><axsl:template match="@*|node()" priority="-2" mode="M5"><axsl:apply-templates select="*|comment()|processing-instruction()" mode="M5"/></axsl:template>

<!--PATTERN idm140727361412784-->


	<!--RULE -->
<axsl:template match="/nc:data/ofc:capable-switch/ofc:resources/ofc:port/ofc:features/ofc:advertised/ofc:rate" priority="1001" mode="M6"><svrl:fired-rule xmlns:svrl="http://purl.oclc.org/dsdl/svrl" context="/nc:data/ofc:capable-switch/ofc:resources/ofc:port/ofc:features/ofc:advertised/ofc:rate"/>

		<!--REPORT -->
<axsl:if test=". = preceding-sibling::ofc:rate"><svrl:successful-report xmlns:svrl="http://purl.oclc.org/dsdl/svrl" xmlns:xs="http://www.w3.org/2001/XMLSchema" xmlns:schold="http://www.ascc.net/xml/schematron" test=". = preceding-sibling::ofc:rate"><axsl:attribute name="location"><axsl:apply-templates select="." mode="schematron-get-full-path"/></axsl:attribute><svrl:text>Duplicate leaf-list entry "<axsl:text/><axsl:value-of select="."/><axsl:text/>".</svrl:text></svrl:successful-report></axsl:if><axsl:apply-templates select="*|comment()|processing-instruction()" mode="M6"/></axsl:template>

	<!--RULE -->
<axsl:template match="/nc:data/ofc:capable-switch/ofc:resources/ofc:port/ofc:features/ofc:advertised/ofc:medium" priority="1000" mode="M6"><svrl:fired-rule xmlns:svrl="http://purl.oclc.org/dsdl/svrl" context="/nc:data/ofc:capable-switch/ofc:resources/ofc:port/ofc:features/ofc:advertised/ofc:medium"/>

		<!--REPORT -->
<axsl:if test=". = preceding-sibling::ofc:medium"><svrl:successful-report xmlns:svrl="http://purl.oclc.org/dsdl/svrl" xmlns:xs="http://www.w3.org/2001/XMLSchema" xmlns:schold="http://www.ascc.net/xml/schematron" test=". = preceding-sibling::ofc:medium"><axsl:attribute name="location"><axsl:apply-templates select="." mode="schematron-get-full-path"/></axsl:attribute><svrl:text>Duplicate leaf-list entry "<axsl:text/><axsl:value-of select="."/><axsl:text/>".</svrl:text></svrl:successful-report></axsl:if><axsl:apply-templates select="*|comment()|processing-instruction()" mode="M6"/></axsl:template><axsl:template match="text()" priority="-1" mode="M6"/><axsl:template match="@*|node()" priority="-2" mode="M6"><axsl:apply-templates select="*|comment()|processing-instruction()" mode="M6"/></axsl:template>

<!--PATTERN idm140727361411680-->


	<!--RULE -->
<axsl:template match="/nc:data/ofc:capable-switch/ofc:resources/ofc:port/ofc:features/ofc:supported/ofc:rate" priority="1001" mode="M7"><svrl:fired-rule xmlns:svrl="http://purl.oclc.org/dsdl/svrl" context="/nc:data/ofc:capable-switch/ofc:resources/ofc:port/ofc:features/ofc:supported/ofc:rate"/>

		<!--REPORT -->
<axsl:if test=". = preceding-sibling::ofc:rate"><svrl:successful-report xmlns:svrl="http://purl.oclc.org/dsdl/svrl" xmlns:xs="http://www.w3.org/2001/XMLSchema" xmlns:schold="http://www.ascc.net/xml/schematron" test=". = preceding-sibling::ofc:rate"><axsl:attribute name="location"><axsl:apply-templates select="." mode="schematron-get-full-path"/></axsl:attribute><svrl:text>Duplicate leaf-list entry "<axsl:text/><axsl:value-of select="."/><axsl:text/>".</svrl:text></svrl:successful-report></axsl:if><axsl:apply-templates select="*|comment()|processing-instruction()" mode="M7"/></axsl:template>

	<!--RULE -->
<axsl:template match="/nc:data/ofc:capable-switch/ofc:resources/ofc:port/ofc:features/ofc:supported/ofc:medium" priority="1000" mode="M7"><svrl:fired-rule xmlns:svrl="http://purl.oclc.org/dsdl/svrl" context="/nc:data/ofc:capable-switch/ofc:resources/ofc:port/ofc:features/ofc:supported/ofc:medium"/>

		<!--REPORT -->
<axsl:if test=". = preceding-sibling::ofc:medium"><svrl:successful-report xmlns:svrl="http://purl.oclc.org/dsdl/svrl" xmlns:xs="http://www.w3.org/2001/XMLSchema" xmlns:schold="http://www.ascc.net/xml/schematron" test=". = preceding-sibling::ofc:medium"><axsl:attribute name="location"><axsl:apply-templates select="." mode="schematron-get-full-path"/></axsl:attribute><svrl:text>Duplicate leaf-list entry "<axsl:text/><axsl:value-of select="."/><axsl:text/>".</svrl:text></svrl:successful-report></axsl:if><axsl:apply-templates select="*|comment()|processing-instruction()" mode="M7"/></axsl:template><axsl:template match="text()" priority="-1" mode="M7"/><axsl:template match="@*|node()" priority="-2" mode="M7"><axsl:apply-templates select="*|comment()|processing-instruction()" mode="M7"/></axsl:template>

<!--PATTERN idm140727361410576-->


	<!--RULE -->
<axsl:template match="/nc:data/ofc:capable-switch/ofc:resources/ofc:port/ofc:features/ofc:advertised-peer/ofc:rate" priority="1001" mode="M8"><svrl:fired-rule xmlns:svrl="http://purl.oclc.org/dsdl/svrl" context="/nc:data/ofc:capable-switch/ofc:resources/ofc:port/ofc:features/ofc:advertised-peer/ofc:rate"/>

		<!--REPORT -->
<axsl:if test=". = preceding-sibling::ofc:rate"><svrl:successful-report xmlns:svrl="http://purl.oclc.org/dsdl/svrl" xmlns:xs="http://www.w3.org/2001/XMLSchema" xmlns:schold="http://www.ascc.net/xml/schematron" test=". = preceding-sibling::ofc:rate"><axsl:attribute name="location"><axsl:apply-templates select="." mode="schematron-get-full-path"/></axsl:attribute><svrl:text>Duplicate leaf-list entry "<axsl:text/><axsl:value-of select="."/><axsl:text/>".</svrl:text></svrl:successful-report></axsl:if><axsl:apply-templates select="*|comment()|processing-instruction()" mode="M8"/></axsl:template>

	<!--RULE -->
<axsl:template match="/nc:data/ofc:capable-switch/ofc:resources/ofc:port/ofc:features/ofc:advertised-peer/ofc:medium" priority="1000" mode="M8"><svrl:fired-rule xmlns:svrl="http://purl.oclc.org/dsdl/svrl" context="/nc:data/ofc:capable-switch/ofc:resources/ofc:port/ofc:features/ofc:advertised-peer/ofc:medium"/>

		<!--REPORT -->
<axsl:if test=". = preceding-sibling::ofc:medium"><svrl:successful-report xmlns:svrl="http://purl.oclc.org/dsdl/svrl" xmlns:xs="http://www.w3.org/2001/XMLSchema" xmlns:schold="http://www.ascc.net/xml/schematron" test=". = preceding-sibling::ofc:medium"><axsl:attribute name="location"><axsl:apply-templates select="." mode="schematron-get-full-path"/></axsl:attribute><svrl:text>Duplicate leaf-list entry "<axsl:text/><axsl:value-of select="."/><axsl:text/>".</svrl:text></svrl:successful-report></axsl:if><axsl:apply-templates select="*|comment()|processing-instruction()" mode="M8"/></axsl:template><axsl:template match="text()" priority="-1" mode="M8"/><axsl:template match="@*|node()" priority="-2" mode="M8"><axsl:apply-templates select="*|comment()|processing-instruction()" mode="M8"/></axsl:template>

<!--PATTERN idm140727361409216-->


	<!--RULE -->
<axsl:template match="/nc:data/ofc:capable-switch/ofc:resources/ofc:port/ofc:tunnel" priority="1000" mode="M9"><svrl:fired-rule xmlns:svrl="http://purl.oclc.org/dsdl/svrl" context="/nc:data/ofc:capable-switch/ofc:resources/ofc:port/ofc:tunnel"/>

		<!--ASSERT -->
<axsl:choose><axsl:when test="ofc:local-endpoint-ipv4-adress[not(processing-instruction('dsrl'))] or ofc:remote-endpoint-ipv4-adress[not(processing-instruction('dsrl'))] or ofc:local-endpoint-ipv6-adress[not(processing-instruction('dsrl'))] or ofc:remote-endpoint-ipv6-adress[not(processing-instruction('dsrl'))] or ofc:local-endpoint-mac-adress[not(processing-instruction('dsrl'))] or ofc:remote-endpoint-mac-adress[not(processing-instruction('dsrl'))] or false()"/><axsl:otherwise><svrl:failed-assert xmlns:svrl="http://purl.oclc.org/dsdl/svrl" xmlns:xs="http://www.w3.org/2001/XMLSchema" xmlns:schold="http://www.ascc.net/xml/schematron" test="ofc:local-endpoint-ipv4-adress[not(processing-instruction('dsrl'))] or ofc:remote-endpoint-ipv4-adress[not(processing-instruction('dsrl'))] or ofc:local-endpoint-ipv6-adress[not(processing-instruction('dsrl'))] or ofc:remote-endpoint-ipv6-adress[not(processing-instruction('dsrl'))] or ofc:local-endpoint-mac-adress[not(processing-instruction('dsrl'))] or ofc:remote-endpoint-mac-adress[not(processing-instruction('dsrl'))] or false()"><axsl:attribute name="location"><axsl:apply-templates select="." mode="schematron-get-full-path"/></axsl:attribute><svrl:text>Node(s) from one case of mandatory choice "endpoints" must exist</svrl:text></svrl:failed-assert></axsl:otherwise></axsl:choose><axsl:apply-templates select="*|comment()|processing-instruction()" mode="M9"/></axsl:template><axsl:template match="text()" priority="-1" mode="M9"/><axsl:template match="@*|node()" priority="-2" mode="M9"><axsl:apply-templates select="*|comment()|processing-instruction()" mode="M9"/></axsl:template>

<!--PATTERN idm140727361408400-->


	<!--RULE -->
<axsl:template match="/nc:data/ofc:capable-switch/ofc:resources/ofc:port/ofc:ipgre-tunnel/ofc:key" priority="1000" mode="M10"><svrl:fired-rule xmlns:svrl="http://purl.oclc.org/dsdl/svrl" context="/nc:data/ofc:capable-switch/ofc:resources/ofc:port/ofc:ipgre-tunnel/ofc:key"/>

		<!--ASSERT -->
<axsl:choose><axsl:when test="ancestor-or-self::*[processing-instruction('dsrl')] or (../ofc:key-present='true')"/><axsl:otherwise><svrl:failed-assert xmlns:svrl="http://purl.oclc.org/dsdl/svrl" xmlns:xs="http://www.w3.org/2001/XMLSchema" xmlns:schold="http://www.ascc.net/xml/schematron" test="ancestor-or-self::*[processing-instruction('dsrl')] or (../ofc:key-present='true')"><axsl:attribute name="location"><axsl:apply-templates select="." mode="schematron-get-full-path"/></axsl:attribute><svrl:text>Node "key" is only valid when "../ofc:key-present='true'"</svrl:text></svrl:failed-assert></axsl:otherwise></axsl:choose><axsl:apply-templates select="*|comment()|processing-instruction()" mode="M10"/></axsl:template><axsl:template match="text()" priority="-1" mode="M10"/><axsl:template match="@*|node()" priority="-2" mode="M10"><axsl:apply-templates select="*|comment()|processing-instruction()" mode="M10"/></axsl:template>

<!--PATTERN idm140727361281760-->


	<!--RULE -->
<axsl:template match="/nc:data/ofc:capable-switch/ofc:resources/ofc:port/ofc:vxlan-tunnel" priority="1000" mode="M11"><svrl:fired-rule xmlns:svrl="http://purl.oclc.org/dsdl/svrl" context="/nc:data/ofc:capable-switch/ofc:resources/ofc:port/ofc:vxlan-tunnel"/>

		<!--ASSERT -->
<axsl:choose><axsl:when test="ofc:local-endpoint-ipv4-adress[not(processing-instruction('dsrl'))] or ofc:remote-endpoint-ipv4-adress[not(processing-instruction('dsrl'))] or ofc:local-endpoint-ipv6-adress[not(processing-instruction('dsrl'))] or ofc:remote-endpoint-ipv6-adress[not(processing-instruction('dsrl'))] or ofc:local-endpoint-mac-adress[not(processing-instruction('dsrl'))] or ofc:remote-endpoint-mac-adress[not(processing-instruction('dsrl'))] or false()"/><axsl:otherwise><svrl:failed-assert xmlns:svrl="http://purl.oclc.org/dsdl/svrl" xmlns:xs="http://www.w3.org/2001/XMLSchema" xmlns:schold="http://www.ascc.net/xml/schematron" test="ofc:local-endpoint-ipv4-adress[not(processing-instruction('dsrl'))] or ofc:remote-endpoint-ipv4-adress[not(processing-instruction('dsrl'))] or ofc:local-endpoint-ipv6-adress[not(processing-instruction('dsrl'))] or ofc:remote-endpoint-ipv6-adress[not(processing-instruction('dsrl'))] or ofc:local-endpoint-mac-adress[not(processing-instruction('dsrl'))] or ofc:remote-endpoint-mac-adress[not(processing-instruction('dsrl'))] or false()"><axsl:attribute name="location"><axsl:apply-templates select="." mode="schematron-get-full-path"/></axsl:attribute><svrl:text>Node(s) from one case of mandatory choice "endpoints" must exist</svrl:text></svrl:failed-assert></axsl:otherwise></axsl:choose><axsl:apply-templates select="*|comment()|processing-instruction()" mode="M11"/></axsl:template><axsl:template match="text()" priority="-1" mode="M11"/><axsl:template match="@*|node()" priority="-2" mode="M11"><axsl:apply-templates select="*|comment()|processing-instruction()" mode="M11"/></axsl:template>

<!--PATTERN idm140727361226304-->


	<!--RULE -->
<axsl:template match="/nc:data/ofc:capable-switch/ofc:resources/ofc:port/ofc:nvgre-tunnel" priority="1000" mode="M12"><svrl:fired-rule xmlns:svrl="http://purl.oclc.org/dsdl/svrl" context="/nc:data/ofc:capable-switch/ofc:resources/ofc:port/ofc:nvgre-tunnel"/>

		<!--ASSERT -->
<axsl:choose><axsl:when test="ofc:local-endpoint-ipv4-adress[not(processing-instruction('dsrl'))] or ofc:remote-endpoint-ipv4-adress[not(processing-instruction('dsrl'))] or ofc:local-endpoint-ipv6-adress[not(processing-instruction('dsrl'))] or ofc:remote-endpoint-ipv6-adress[not(processing-instruction('dsrl'))] or ofc:local-endpoint-mac-adress[not(processing-instruction('dsrl'))] or ofc:remote-endpoint-mac-adress[not(processing-instruction('dsrl'))] or false()"/><axsl:otherwise><svrl:failed-assert xmlns:svrl="http://purl.oclc.org/dsdl/svrl" xmlns:xs="http://www.w3.org/2001/XMLSchema" xmlns:schold="http://www.ascc.net/xml/schematron" test="ofc:local-endpoint-ipv4-adress[not(processing-instruction('dsrl'))] or ofc:remote-endpoint-ipv4-adress[not(processing-instruction('dsrl'))] or ofc:local-endpoint-ipv6-adress[not(processing-instruction('dsrl'))] or ofc:remote-endpoint-ipv6-adress[not(processing-instruction('dsrl'))] or ofc:local-endpoint-mac-adress[not(processing-instruction('dsrl'))] or ofc:remote-endpoint-mac-adress[not(processing-instruction('dsrl'))] or false()"><axsl:attribute name="location"><axsl:apply-templates select="." mode="schematron-get-full-path"/></axsl:attribute><svrl:text>Node(s) from one case of mandatory choice "endpoints" must exist</svrl:text></svrl:failed-assert></axsl:otherwise></axsl:choose><axsl:apply-templates select="*|comment()|processing-instruction()" mode="M12"/></axsl:template><axsl:template match="text()" priority="-1" mode="M12"/><axsl:template match="@*|node()" priority="-2" mode="M12"><axsl:apply-templates select="*|comment()|processing-instruction()" mode="M12"/></axsl:template>

<!--PATTERN idm140727361395904-->


	<!--RULE -->
<axsl:template match="/nc:data/ofc:capable-switch/ofc:resources/ofc:owned-certificate/ofc:private-key" priority="1000" mode="M13"><svrl:fired-rule xmlns:svrl="http://purl.oclc.org/dsdl/svrl" context="/nc:data/ofc:capable-switch/ofc:resources/ofc:owned-certificate/ofc:private-key"/>

		<!--ASSERT -->
<axsl:choose><axsl:when test="ofc:DSAKeyValue[not(processing-instruction('dsrl'))] or ofc:RSAKeyValue[not(processing-instruction('dsrl'))] or false()"/><axsl:otherwise><svrl:failed-assert xmlns:svrl="http://purl.oclc.org/dsdl/svrl" xmlns:xs="http://www.w3.org/2001/XMLSchema" xmlns:schold="http://www.ascc.net/xml/schematron" test="ofc:DSAKeyValue[not(processing-instruction('dsrl'))] or ofc:RSAKeyValue[not(processing-instruction('dsrl'))] or false()"><axsl:attribute name="location"><axsl:apply-templates select="." mode="schematron-get-full-path"/></axsl:attribute><svrl:text>Node(s) from one case of mandatory choice "key-type" must exist</svrl:text></svrl:failed-assert></axsl:otherwise></axsl:choose><axsl:apply-templates select="*|comment()|processing-instruction()" mode="M13"/></axsl:template><axsl:template match="text()" priority="-1" mode="M13"/><axsl:template match="@*|node()" priority="-2" mode="M13"><axsl:apply-templates select="*|comment()|processing-instruction()" mode="M13"/></axsl:template>

<!--PATTERN idm140727361371408-->


	<!--RULE -->
<axsl:template match="/nc:data/ofc:capable-switch/ofc:logical-switches/ofc:switch/ofc:capabilities/ofc:reserved-port-types/ofc:type" priority="1004" mode="M14"><svrl:fired-rule xmlns:svrl="http://purl.oclc.org/dsdl/svrl" context="/nc:data/ofc:capable-switch/ofc:logical-switches/ofc:switch/ofc:capabilities/ofc:reserved-port-types/ofc:type"/>

		<!--REPORT -->
<axsl:if test=". = preceding-sibling::ofc:type"><svrl:successful-report xmlns:svrl="http://purl.oclc.org/dsdl/svrl" xmlns:xs="http://www.w3.org/2001/XMLSchema" xmlns:schold="http://www.ascc.net/xml/schematron" test=". = preceding-sibling::ofc:type"><axsl:attribute name="location"><axsl:apply-templates select="." mode="schematron-get-full-path"/></axsl:attribute><svrl:text>Duplicate leaf-list entry "<axsl:text/><axsl:value-of select="."/><axsl:text/>".</svrl:text></svrl:successful-report></axsl:if><axsl:apply-templates select="*|comment()|processing-instruction()" mode="M14"/></axsl:template>

	<!--RULE -->
<axsl:template match="/nc:data/ofc:capable-switch/ofc:logical-switches/ofc:switch/ofc:capabilities/ofc:group-types/ofc:type" priority="1003" mode="M14"><svrl:fired-rule xmlns:svrl="http://purl.oclc.org/dsdl/svrl" context="/nc:data/ofc:capable-switch/ofc:logical-switches/ofc:switch/ofc:capabilities/ofc:group-types/ofc:type"/>

		<!--REPORT -->
<axsl:if test=". = preceding-sibling::ofc:type"><svrl:successful-report xmlns:svrl="http://purl.oclc.org/dsdl/svrl" xmlns:xs="http://www.w3.org/2001/XMLSchema" xmlns:schold="http://www.ascc.net/xml/schematron" test=". = preceding-sibling::ofc:type"><axsl:attribute name="location"><axsl:apply-templates select="." mode="schematron-get-full-path"/></axsl:attribute><svrl:text>Duplicate leaf-list entry "<axsl:text/><axsl:value-of select="."/><axsl:text/>".</svrl:text></svrl:successful-report></axsl:if><axsl:apply-templates select="*|comment()|processing-instruction()" mode="M14"/></axsl:template>

	<!--RULE -->
<axsl:template match="/nc:data/ofc:capable-switch/ofc:logical-switches/ofc:switch/ofc:capabilities/ofc:group-capabilities/ofc:capability" priority="1002" mode="M14"><svrl:fired-rule xmlns:svrl="http://purl.oclc.org/dsdl/svrl" context="/nc:data/ofc:capable-switch/ofc:logical-switches/ofc:switch/ofc:capabilities/ofc:group-capabilities/ofc:capability"/>

		<!--REPORT -->
<axsl:if test=". = preceding-sibling::ofc:capability"><svrl:successful-report xmlns:svrl="http://purl.oclc.org/dsdl/svrl" xmlns:xs="http://www.w3.org/2001/XMLSchema" xmlns:schold="http://www.ascc.net/xml/schematron" test=". = preceding-sibling::ofc:capability"><axsl:attribute name="location"><axsl:apply-templates select="." mode="schematron-get-full-path"/></axsl:attribute><svrl:text>Duplicate leaf-list entry "<axsl:text/><axsl:value-of select="."/><axsl:text/>".</svrl:text></svrl:successful-report></axsl:if><axsl:apply-templates select="*|comment()|processing-instruction()" mode="M14"/></axsl:template>

	<!--RULE -->
<axsl:template match="/nc:data/ofc:capable-switch/ofc:logical-switches/ofc:switch/ofc:capabilities/ofc:action-types/ofc:type" priority="1001" mode="M14"><svrl:fired-rule xmlns:svrl="http://purl.oclc.org/dsdl/svrl" context="/nc:data/ofc:capable-switch/ofc:logical-switches/ofc:switch/ofc:capabilities/ofc:action-types/ofc:type"/>

		<!--REPORT -->
<axsl:if test=". = preceding-sibling::ofc:type"><svrl:successful-report xmlns:svrl="http://purl.oclc.org/dsdl/svrl" xmlns:xs="http://www.w3.org/2001/XMLSchema" xmlns:schold="http://www.ascc.net/xml/schematron" test=". = preceding-sibling::ofc:type"><axsl:attribute name="location"><axsl:apply-templates select="." mode="schematron-get-full-path"/></axsl:attribute><svrl:text>Duplicate leaf-list entry "<axsl:text/><axsl:value-of select="."/><axsl:text/>".</svrl:text></svrl:successful-report></axsl:if><axsl:apply-templates select="*|comment()|processing-instruction()" mode="M14"/></axsl:template>

	<!--RULE -->
<axsl:template match="/nc:data/ofc:capable-switch/ofc:logical-switches/ofc:switch/ofc:capabilities/ofc:instruction-types/ofc:type" priority="1000" mode="M14"><svrl:fired-rule xmlns:svrl="http://purl.oclc.org/dsdl/svrl" context="/nc:data/ofc:capable-switch/ofc:logical-switches/ofc:switch/ofc:capabilities/ofc:instruction-types/ofc:type"/>

		<!--REPORT -->
<axsl:if test=". = preceding-sibling::ofc:type"><svrl:successful-report xmlns:svrl="http://purl.oclc.org/dsdl/svrl" xmlns:xs="http://www.w3.org/2001/XMLSchema" xmlns:schold="http://www.ascc.net/xml/schematron" test=". = preceding-sibling::ofc:type"><axsl:attribute name="location"><axsl:apply-templates select="." mode="schematron-get-full-path"/></axsl:attribute><svrl:text>Duplicate leaf-list entry "<axsl:text/><axsl:value-of select="."/><axsl:text/>".</svrl:text></svrl:successful-report></axsl:if><axsl:apply-templates select="*|comment()|processing-instruction()" mode="M14"/></axsl:template><axsl:template match="text()" priority="-1" mode="M14"/><axsl:template match="@*|node()" priority="-2" mode="M14"><axsl:apply-templates select="*|comment()|processing-instruction()" mode="M14"/></axsl:template>

<!--PATTERN xdpd-mgmt-->
<axsl:template match="text()" priority="-1" mode="M15"/><axsl:template match="@*|node()" priority="-2" mode="M15"><axsl:apply-templates select="*|comment()|processing-instruction()" mode="M15"/></axsl:template></axsl:stylesheet>
