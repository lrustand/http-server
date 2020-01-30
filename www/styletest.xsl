<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
 
<xsl:template match="/">
	<html>
		<head>
		</head>

		<body>
			<h1>Bokliste</h1>
				<table border="1">
<tr>
<th>Tittel</th> <th>BokId</th> <th>ForfatterId</th>
</tr>
	<xsl:for-each select="bokliste/bok">
	<xsl:sort select="tittel"/>
<tr>
	<td><xsl:value-of select="tittel"/></td>
	<td><xsl:value-of select="bokid"/></td>
	<td><xsl:value-of select="forfatterId"/></td>
</tr>
</xsl:for-each>
</table>
</body>
</html>
</xsl:template>
</xsl:stylesheet>
