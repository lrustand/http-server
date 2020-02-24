#!/bin/sh
echo "Content-Type: text/html;charset=utf-8"

if [ "$REQUEST_METHOD" = "POST" ]; then
	LANG=C IFS= read -r -d '' -n $CONTENT_LENGTH BODY
	JSON=$(echo -n "{\"$BODY\"}" | sed -e "s/\&/\"\,\"/g" -e "s/\=/\"\:\"/g")
	REQUEST="POST /diktsamling/bruker/ HTTP/1.1\n"
	REQUEST=$REQUEST"Content-Type: application/json\n"
	REQUEST=$REQUEST"Content-Length: $(echo -n "$JSON" | wc -c)\n\n"
	REQUEST="${REQUEST}${JSON}\r\n"
	>&2 echo -e "$REQUEST"
	>&2 echo -e "$BODY"
	echo -e -n "$REQUEST" | nc 127.0.0.1 3000 | grep Set-Cookie
fi
echo

cat << EOF
<html>
	<head>
		<meta charset='utf-8'>
		<title>Logg Inn</title>
	</head>
	<body>
		<form action="" method='post'>
			<input type=text name='username' placeholder='epost'>
			<br>
			<input type=password name='password' placeholder=''>
			<br>
			<input type='submit' value='Logg inn'>
		</form>
	</body>
</html>
EOF
