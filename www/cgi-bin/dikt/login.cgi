#!/bin/sh
echo "Content-Type: text/html;charset=utf-8"

if [ "$REQUEST_METHOD" = "POST" ]; then
	LANG=C IFS= read -r -d '' -n $CONTENT_LENGTH BODY
	JSON='{'$(echo -n "\"$BODY\"" | sed -e "s/\&/\"\,\"/g" -e "s/\=/\"\:\"/g")'}'
	COOKIE=$(wget --post-data="$JSON" http://127.0.0.1:3000/diktsamling/sesjon/ --header "Content-Type: application/json" -qO- -S 2>&1 | grep Set-Cookie | sed -e 's/Set-Cookie: //gi')
	if [ -z "$COOKIE" ]; then
		echo
		echo "Feil brukernavn eller passord!"
	else
		echo "Set-Cookie: $COOKIE"
		echo
		echo "Du er nå logget inn"

	fi
else
	echo
fi

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
