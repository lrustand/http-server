#!/bin/sh
echo "Content-Type: text/html;charset=utf-8"
echo

if [ "$REQUEST_METHOD" = "POST" ]; then
	LANG=C IFS= read -r -d '' -n $CONTENT_LENGTH BODY
	JSON=$(echo -n "{\"$BODY\"}" | sed -e 's/&/","/g' -e 's/=/":"/g')
	RESPONSE=$(wget --post-data="$JSON" http://127.0.0.1:3000/diktsamling/dikt/ --header "Content-Type: application/json" -qO- -S 2>&1)
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
