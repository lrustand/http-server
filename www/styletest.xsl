<?xml version="1.0" encoding="UTF-8"?>

<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">

	<xsl:template match="/">

		<html>
			<head>
				<link rel="stylesheet" type="text/css"	href="/hest.css" />
			</head>

			<body>
				<h1>Bokliste</h1>

					<table border="1">
						<tr>
							<th>Title</th> <th>Author</th> <th>Year</th>
						</tr>

						<xsl:for-each select="bookstore/book">

						<tr>
							<td><xsl:value-of select="title"	/></td>
							<td><xsl:value-of select="author"	/></td>
							<td><xsl:value-of select="year"		/></td>
						</tr>
						</xsl:for-each>
					</table>
			</body>
		</html>
	</xsl:template>

</xsl:stylesheet>
