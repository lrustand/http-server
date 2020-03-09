#!/bin/sh
echo "Content-Type: text/html;charset=utf-8"
echo

OUTPUT=$(wget -qO- http://127.0.0.1:3000/diktsamling/dikt/ )
echo "$OUTPUT"
grepAndStrip(){
	echo "$OUTPUT" | grep "diktid" | cut -d '"' -f 3 | sed -e "s/[: ,]//g"
	echo "$OUTPUT" | grep 'dikt"' | cut -d '"' -f 3 | sed -e "s/[: ,]//g"

}

grepAndStrip

cat << EOF
<html>
	<head>
		<meta charset="utf-8">
		<title>Dikt</title>
	</head>
	<body>
		<div class="header">
			<a href="visAlle_dikt.cgi"><b>Home<b/></a>
			<div class="header-right">
				<a href="opprett_dikt.cgi">Opprett Dikt</a>
				<a href="login.cgi">Login</a>
			</div>
		</div>
		
		<div class="main">
			<h1>Vis Alle Dikt :)</h1>
		</div>
	</body>
</html>
EOF
