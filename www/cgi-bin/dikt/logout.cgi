#!/bin/sh
echo "Content-Type: text/html;charset=utf-8"

if [ "$REQUEST_METHOD" = "POST" ]; then
	REQUEST="DELETE /diktsamling/sesjon/ HTTP/1.1\n"
	REQUEST="${REQUEST}Cookie: $COOKIE\n"
	REQUEST="${REQUEST}\n"
	RESPONSE=$(echo -e -n "$REQUEST" | nc 127.0.0.1 3000)
	echo
	echo "Du har nå logget ut"
else
	echo
fi

cat << EOF
<html>
	<head>
		<meta charset='utf-8'>
		<title>Logg ut</title>
	</head>
	<body>
		$(/www/cgi-bin/dikt/header.cgi)
		<form action="" method='post'>
			<input type='hidden' name='session' value='$COOKIE'>
			<input type='submit' value='Logg ut'>
		</form>
	</body>
</html>
EOF
