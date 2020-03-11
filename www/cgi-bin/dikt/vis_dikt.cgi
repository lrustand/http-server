#!/bin/sh
echo "Content-Type: text/html;charset=utf-8"
echo

unescapeDiktInnhold(){
	DIKT=$(echo "$DIKT" | sed 's/\\\\n/\n/g')
	DIKT=$(echo "$DIKT" | sed 's/\\\\t/\t/g')
	DIKT=$(echo "$DIKT" | sed 's/\\\\.//g')
}

DIKTID=$(echo -n "$QUERY_STRING" | cut -d "=" -f 2)
JSON=$(wget -qO- "http://127.0.0.1:3000/diktsamling/dikt/$DIKTID")
DIKT=$(echo "$JSON" | grep '"dikt"' | cut -d '"' -f 4)
unescapeDiktInnhold
FORNAVN=$(echo "$JSON" | grep '"fornavn"' | cut -d '"' -f 4)
ETTERNAVN=$(echo "$JSON" | grep '"etternavn"' | cut -d '"' -f 4)
if [[ -z "$FORNAVN$ETTERNAVN" ]]; then
	FORNAVN="Anonym"
	ETTERNAVN="bruker"
fi

if [[ -z "$DIKT" ]]; then
cat << EOF
<html>
	<head>
		<meta charset='utf-8'>
		<title>Vis Dikt</title>
	</head>
	<body>
		<h1 style='color: red'>Kunne ikke finne dikt #$DIKTID</h1>
	</body>
</html>
EOF
else
cat << EOF
<html>
	<head>
		<meta charset='utf-8'>
		<title>Vis Dikt</title>
	</head>
	<body>
		<h1>Dikt #$DIKTID</h1>
		<pre>$DIKT</pre>
		<p><i>- $FORNAVN $ETTERNAVN<i><p>
	</body>
</html>
EOF
fi