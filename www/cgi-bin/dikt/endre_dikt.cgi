#!/bin/sh
echo "Content-Type: text/html;charset=utf-8"
echo

unescapeDiktInnhold(){
	DIKT=$(echo "$DIKT" | sed 's/\\\\n/\n/g')
	DIKT=$(echo "$DIKT" | sed 's/\\\\t/\t/g')
	DIKT=$(echo "$DIKT" | sed 's/\\\\.//g')
}

DIKTID=$(echo -n "$QUERY_STRING" | cut -d "=" -f 2)
if [ "$REQUEST_METHOD" = "POST" ]; then
	LANG=C IFS= read -r -d '' -n $CONTENT_LENGTH BODY
	JSON=$(echo -n "{\"$BODY\"}" | sed -e 's/=/":"/g')
	DIKT=$(echo "$JSON" | cut -d '"' -f 4)
	DIKT=$(echo "$DIKT" | sed 's@+@ @g;s@%@\\x@g' | xargs -0 printf "%b")
	unescapeDiktInnhold

	if [[ ! -z "$COOKIE" ]]; then
		REQUEST="PUT /diktsamling/dikt/$DIKTID HTTP/1.1\n"
		REQUEST="${REQUEST}Content-Type: application/json\n"
		REQUEST="${REQUEST}Content-Length: $(echo $JSON | wc -c)\n"
		REQUEST="${REQUEST}Cookie: ${COOKIE}\n"
		REQUEST="${REQUEST}\n"
		REQUEST="${REQUEST}${JSON}"
		RESPONSE=$(echo -e -n "$REQUEST" | nc 127.0.0.1 3000)
	else
		ERR="<h1 style='color: red'>Ikke logget inn</h1>"
	fi
else
	DIKT=$(wget -qO- "http://127.0.0.1:3000/diktsamling/dikt/$DIKTID")
	DIKT=$(echo "$DIKT" | grep '"dikt"' | cut -d '"' -f 4)
	unescapeDiktInnhold
fi

cat << EOF
<html>
	<head>
		<meta charset='utf-8'>
		<title>Endre Dikt</title>
	</head>
	<body>
		$ERR
		<form action="" method='post'>
			<textarea name='dikt' rows='10' cols='30' placeholder='Skriv dikt her'>$DIKT</textarea>
			<br>
			<input type='submit' value='Endre dikt'>
		</form>
	</body>
</html>
EOF
