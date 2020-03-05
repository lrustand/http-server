#!/bin/sh
echo "Content-Type: text/html;charset=utf-8"
echo

if [ "$REQUEST_METHOD" = "POST" ]; then
	LANG=C IFS= read -r -d '' -n $CONTENT_LENGTH BODY
	JSON=$(echo -n "{\"$BODY\"}" | sed -e 's/&/","/g' -e 's/=/":"/g')
	REQUEST="POST /diktsamling/dikt HTTP/1.1\n"
	REQUEST="${REQUEST}Content-Type: application/json\n"
	REQUEST="${REQUEST}Content-Length: $(echo $JSON | wc -c)\n"
	if [[ ! -z "$COOKIE" ]]; then
		REQUEST="${REQUEST}Cookie: ${COOKIE}\n"
	fi
	REQUEST="${REQUEST}\n"
	REQUEST="${REQUEST}${JSON}"
	RESPONSE=$(echo -e -n "$REQUEST" | nc 127.0.0.1 3000)
fi

cat << EOF
<html>
	<head>
		<meta charset='utf-8'>
		<title>Opprett Dikt</title>
	</head>
	<body>
		<form action="" method='post'>
			<textarea name='dikt' rows='10' cols='30' placeholder='Skriv dikt her'></textarea>
			<br>
			<input type='submit' value='Opprett dikt'>
		</form>
	</body>
</html>
EOF
