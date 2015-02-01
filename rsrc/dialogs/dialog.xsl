<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">

<xsl:template match='/'>

<html>
<head>
<title>Dialog Preview -
</title>
<script type='text/javascript'>
	document.querySelector('title').innerHTML += window.location.pathname.split("/").pop();
</script>
<link rel='stylesheet' type='text/css' href='dialog.css'/>
</head>

<body>
<xsl:if test='/dialog/@skin'>
	<xsl:attribute name='style'>
	background-image: url('img/bg/<xsl:value-of select='/dialog/@skin'/>.png');
	</xsl:attribute>
</xsl:if>
<xsl:if test='/dialog/@skin = "dark" or not(/dialog/@skin)'>
	<xsl:attribute name='class'>dark</xsl:attribute>
</xsl:if>
<div class='dialog'>
<xsl:for-each select='dialog/pict | dialog/stack/pict'>
	<div>
	<xsl:attribute name='class'>
		pict
		<xsl:if test='@framed = "true" or not(@framed)'>framed </xsl:if>
		<xsl:value-of select='./@type'/>
		<xsl:if test='@size = "large" and @type != "monst"'>large</xsl:if>
	</xsl:attribute>
	<xsl:if test='/dialog/@debug = "true" and @name'>
		<xsl:attribute name='title'>
			<xsl:value-of select='./@name'/>
		</xsl:attribute>
	</xsl:if>
	<xsl:choose>
		<xsl:when test='@type = "full"'>
			<xsl:attribute name='style'>
			background-image: url('img/other/<xsl:value-of select='./@num'/>.png');
			left: <xsl:value-of select='./@left'/>px; top: <xsl:value-of select='./@top'/>px;
			width: <xsl:value-of select='./@width'/>px; height: <xsl:value-of select='./@height'/>px;
			</xsl:attribute>
		</xsl:when>
		<xsl:when test='@type = "blank"'>
			<xsl:attribute name='style'>
			background-color: black;
			left: <xsl:value-of select='./@left'/>px; top: <xsl:value-of select='./@top'/>px;
			</xsl:attribute>
		</xsl:when>
		<xsl:when test='@size = "large" and @type != "monst"'>
			<xsl:attribute name='style'>
			background-image: url('img/<xsl:value-of select='./@type'/>/large<xsl:value-of select='./@num'/>.png');
			left: <xsl:value-of select='./@left'/>px; top: <xsl:value-of select='./@top'/>px;
			</xsl:attribute>
		</xsl:when>
		<xsl:otherwise>
			<xsl:attribute name='style'>
			background-image: url('img/<xsl:value-of select='./@type'/>/<xsl:value-of select='./@num'/>.png');
			left: <xsl:value-of select='./@left'/>px; top: <xsl:value-of select='./@top'/>px;
			</xsl:attribute>
		</xsl:otherwise>
	</xsl:choose>
	</div>
</xsl:for-each>

<xsl:for-each select='dialog/button | dialog/stack/button'>
	<div>
	<xsl:attribute name='class'>
		<xsl:if test='/dialog/@debug = "true" and @type = "tiny"'>debug</xsl:if>
		button <xsl:value-of select='./@type'/>
	</xsl:attribute>
	<xsl:attribute name='style'>
		background-image: url('img/button/<xsl:value-of select='./@type'/>.png');
		left: <xsl:value-of select='./@left'/>px; top: <xsl:value-of select='./@top'/>px;
		<xsl:if test='@type = "push" or @type = "tiny"'>width: <xsl:value-of select='./@width'/>px;</xsl:if>
	</xsl:attribute>
	<xsl:attribute name='title'>
		<xsl:if test='/dialog/@debug = "true" and @name'>
			<xsl:value-of select='./@name'/>
		</xsl:if>
		<xsl:if test='@def-key'>
			(<xsl:value-of select='./@def-key'/>)
		</xsl:if>
	</xsl:attribute>
	<xsl:value-of select='.'/>
	</div>
</xsl:for-each>

<xsl:for-each select='dialog/led | dialog/stack/led'>
	<div>
	<xsl:attribute name='class'>
		led 
		<xsl:if test='/dialog/@debug = "true"'>debug</xsl:if>
	</xsl:attribute>
	<xsl:if test='/dialog/@debug = "true" and @name'>
		<xsl:attribute name='title'>
			<xsl:value-of select='./@name'/>
		</xsl:attribute>
	</xsl:if>
	<xsl:attribute name='style'>
		color: <xsl:value-of select='./@color'/> <xsl:value-of select='/dialog/@fore'/>;
		font-family:
		<xsl:choose>
			<xsl:when test='@font = "plain"'>'DejaVu Sans', Geneva</xsl:when>
			<xsl:when test='@font = "bold"'>Capriola, Silom</xsl:when>
			<xsl:otherwise><xsl:value-of select='./@font'/></xsl:otherwise>
		</xsl:choose>;
		<xsl:if test='./@state'>
			background-image: url('img/button/led-<xsl:value-of select='./@state'/>.png');
		</xsl:if>
		background-position: left top;
		left: <xsl:value-of select='./@left'/>px; top: <xsl:value-of select='./@top'/>px;
		width: <xsl:value-of select='./@width'/>px;
		height: <xsl:value-of select='./@height'/>px;
	</xsl:attribute>
	<xsl:value-of select='.'/>
	</div>
</xsl:for-each>

<xsl:for-each select='dialog/group/led | dialog/stack/group/led'>
	<div>
	<xsl:attribute name='class'>
		led 
		<xsl:if test='/dialog/@debug = "true"'>debug</xsl:if>
	</xsl:attribute>
	<xsl:if test='/dialog/@debug = "true" and @name'>
		<xsl:attribute name='title'>
			<xsl:value-of select='./@name'/>
		</xsl:attribute>
	</xsl:if>
	<xsl:attribute name='style'>
		color: <xsl:value-of select='./@color'/> <xsl:value-of select='/dialog/@fore'/>;
		font-family:
		<xsl:choose>
			<xsl:when test='@font = "plain"'>'DejaVu Sans', Geneva</xsl:when>
			<xsl:when test='@font = "bold"'>Capriola, Silom</xsl:when>
			<xsl:otherwise><xsl:value-of select='./@font'/></xsl:otherwise>
		</xsl:choose>;
		border-color: red;
		<xsl:if test='./@state'>
			background-image: url('img/button/led-<xsl:value-of select='./@state'/>.png');
		</xsl:if>
		left: <xsl:value-of select='./@left'/>px; top: <xsl:value-of select='./@top'/>px;
		width: <xsl:value-of select='./@width'/>px;
	</xsl:attribute>
	<xsl:value-of select='.'/>
	</div>
</xsl:for-each>

<xsl:for-each select='dialog/text | dialog/stack/text'>
	<div>
	<xsl:attribute name='class'>
		text
		<xsl:if test='@framed = "true"'>framed </xsl:if>
		<xsl:if test='@size = "large"'>large </xsl:if>
		<xsl:if test='@size = "title"'>title </xsl:if>
		<xsl:if test='/dialog/@debug = "true"'>debug</xsl:if>
	</xsl:attribute>
	<xsl:if test='/dialog/@debug = "true" and @name'>
		<xsl:attribute name='title'>
			<xsl:value-of select='./@name'/>
		</xsl:attribute>
	</xsl:if>
	<xsl:attribute name='style'>
		color: <xsl:value-of select='./@color'/> <xsl:value-of select='/dialog/@fore'/>;
		font-family: <xsl:value-of select='./@font'/>;
		left: <xsl:value-of select='./@left'/>px; top: <xsl:value-of select='./@top'/>px;
		width: <xsl:value-of select='./@width'/>px; height: <xsl:value-of select='./@height'/>px;
	</xsl:attribute>
	<xsl:for-each select='./child::node()'>
		<xsl:choose>
			<xsl:when test="self::br">
				<br/>
			</xsl:when>
			<xsl:when test="self::text()">
				<xsl:value-of select='translate(.,"&#9;&#10;&#13;"," ")'/>
			</xsl:when>
		</xsl:choose>
	</xsl:for-each>
	</div>
</xsl:for-each>

<xsl:for-each select='dialog/field | dialog/stack/field'>
	<div class='tfield'>
	<xsl:attribute name='style'>
		left: <xsl:value-of select='./@left'/>px; top: <xsl:value-of select='./@top'/>px;
		width: <xsl:value-of select='./@width'/>px; height: <xsl:value-of select='./@height'/>px;
	</xsl:attribute>
	<div class='tfield-inner'>
	<xsl:attribute name='style'>
		width: <xsl:value-of select='./@width'/>px; height: <xsl:value-of select='./@height'/>px;
		font-family: 'DejaVu Sans', Geneva;
		font-size: 12px;
	</xsl:attribute>
	(<xsl:value-of select='@name'/>)
	</div>
	</div>
</xsl:for-each>
</div>
</body>
</html>

</xsl:template>

</xsl:stylesheet>