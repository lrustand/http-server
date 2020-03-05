#!/bin/sh
echo "Content-Type: text/html;charset=utf-8"
echo

if [ "$REQUEST_METHOD" = "POST" ]; then
	LANG=C IFS= read -r -d '' -n $CONTENT_LENGTH BODY
	if [[ ! -z "$BODY"  ]]; then
		ID=$(echo "$BODY" | cut -d '=' -f 2 )
	fi
	REQUEST="DELETE /diktsamling/dikt/$ID HTTP/1.1\n"
	if [[ ! -z "$COOKIE" ]]; then
		REQUEST="${REQUEST}Cookie: ${COOKIE}\n"
	fi
	REQUEST="${REQUEST}\n"
	RESPONSE=$(echo -e -n "$REQUEST" | nc 127.0.0.1 3000)
fi

cat << EOF
<html>
	<head>
		<meta charset="utf-8">
		<title>Slett dikt</title>
	</head>
	<body>
		<form action="" method="post">
			<select name="dikt" size="4">
EOF


OUTPUT=$(wget http://127.0.0.1:3000/diktsamling/bruker/ --header="Cookie: $COOKIE" -qO- -S 2>&1 | grep diktid )

IFS=","
for LINE in $OUTPUT; do
	DIKTID="$(echo "$LINE" | cut -d ':' -f 2 | tr -d '\n' | tr -d ' ')"
	echo -e "\t\t\t\t<option value=\"$DIKTID\">$DIKTID</option>"
done


cat << EOF
			</select>
			<br>
			<input type="submit" value="Slett dikt">
		</form>
	</body>
</html>
EOF
