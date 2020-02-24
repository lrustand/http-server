#!/bin/sh
echo "Content-Type: text/html;charset=utf-8"
echo

if [ "$REQUEST_METHOD" = "POST" ]; then
	LANG=C IFS= read -r -d '' -n $CONTENT_LENGTH BODY
	JSON=$(echo "{\"$BODY\"}" | sed -e "s/\&/\"\,\"/g" -e "s/\=/\"\:\"/g")
	REQUEST="POST /diktsamling/dikt/ HTTP/1.1\n"
	REQUEST=$REQUEST"Content-Type: application/json\n"
	REQUEST=$REQUEST"Content-Length: $(echo $JSON | wc -c)\n\n"
	REQUEST=${REQUEST}${JSON}
	>&2 echo -e "$REQUEST"
	>&2 echo -e "$BODY"
	echo -e -n "$REQUEST" | >&2 nc 127.0.0.1 3000
fi

cat << EOF
<html>
	<head>
		<meta charset='utf-8'>
		<title>Opprett Dikt</title>
	</head>
	<body>
		<form action="" method='post'>
			<input type=text name='epostadresse' placeholder='epost'>
			<br>
			<textarea name='dikt' rows='10' cols='30' placeholder='Skriv dikt her'></textarea>
			<br>
			<input type='submit' value='Opprett dikt'>
		</form>
	</body>
</html>
EOF
