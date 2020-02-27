#!/bin/sh
echo "Content-Type: text/html;charset=utf-8"
echo

if [ "$REQUEST_METHOD" = "POST" ]; then
	LANG=C IFS= read -r -d '' -n $CONTENT_LENGTH BODY
	#if empty request -> delete all
	#else -> delete diktID
fi
cat << EOF
<html>
	<head>
		<meta charset='utf-8'>
		<title>Slett dikt</title>
	</head>
EOF

	REQUEST="GET /diktsamling/dikt/ HTTP/1.1\n\n"
	OUTPUT=$(echo -e -n "$REQUEST" | wget http://79.161.249.112:3000/diktsamling/dikt/ -qO- -S 2>&1 | grep diktid )

cat << EOF
	<body>
		<form action="" method='post'>
			<select name="dikt" size="4">
EOF
	IFS=","
	for LINE in $OUTPUT; do
		echo "<option value=\"$(echo "$LINE" | cut -d ':' -f 2 | tr -d '\n' | tr -d ' ' )\">$LINE</option>"
	done

cat << EOF
			</select> 
			<br> 
			<input type='submit' value='Slett dikt'>
		</form>
	</body>
</html>
EOF
