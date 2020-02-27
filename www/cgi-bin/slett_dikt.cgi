#!/bin/sh
echo "Content-Type: text/html;charset=utf-8"
echo

if [ "$REQUEST_METHOD" = "POST" ]; then
	LANG=C IFS= read -r -d '' -n $CONTENT_LENGTH BODY
	if [[ ! -z "$BODY"  ]]; then
		ID=$(echo "$BODY" | cut -d '=' -f 2 )
		#wget --method=DELETE "http://127.0.0.1:3000/diktsamling/dikt/$ID"
		REQUEST="DELETE /diktsamling/dikt/$ID HTTP/1.1\n\n"
		echo -e -n "$REQUEST" | nc 127.0.0.1 3000
	else 
		echo "NO BODY"
		echo "BODY: "$BODY
	fi
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

	OUTPUT=$(wget http://127.0.0.1:3000/diktsamling/dikt/ -qO- -S 2>&1 | grep diktid )

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
