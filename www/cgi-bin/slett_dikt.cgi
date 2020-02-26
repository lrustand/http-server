#!/bin/sh
echo "Content-Type: text/html;charset=utf-8"
echo

if [ "$REQUEST_METHOD" = "POST" ]; then
	LANG=C IFS= read -r -d '' -n $CONTENT_LENGTH BODY
	JSON=$(echo -n "{\"$BODY\"}" | sed -e "s/\&/\"\,\"/g" -e "s/\=/\"\:\"/g")
	REQUEST="DELETE /diktsamling/dikt/ HTTP/1.1\n"
	REQUEST=$REQUEST"Content-Type: application/json\n"
	REQUEST=$REQUEST"Content-Length: $(echo -n "$JSON" | wc -c)\n\n"
	REQUEST="${REQUEST}${JSON}\r\n"
	echo -e -n "$REQUEST" | >&2 nc 127.0.0.1 3000
fi

	REQUEST="GET /diktsamling/dikt/ HTTP/1.1\n\n"
	echo -e -n "$REQUEST" | >&2 nc 127.0.0.1 3000 | grep diktid | cut -d " " -f 1

cat << EOF
<html>
	<head>
		<meta charset='utf-8'>
		<title>Slett dikt</title>
	</head>
	<body>
		<form action="" method='post'>
			<input type=text name='epostadresse' placeholder='epost'>
			<br>
			<select name="dikt" size="4">
			<br> 
			<input type='submit' value='Slett dikt'>
		</form>
	</body>
</html>
EOF
