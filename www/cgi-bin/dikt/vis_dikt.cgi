#!/bin/sh
echo "Content-Type: text/html;charset=utf-8"
echo

source /scripts/include.sh

DIKTID=$(echo -n "$QUERY_STRING" | cut -d "=" -f 2)
JSON=$(wget -qO- "http://127.0.0.1:3000/diktsamling/dikt/$DIKTID")
DIKT=$(echo "$JSON" | grep '"dikt"' | cut -d '"' -f 4)
DIKT=$(formaterForVisning "$DIKT")
FORNAVN=$(echo "$JSON" | grep '"fornavn"' | cut -d '"' -f 4)
ETTERNAVN=$(echo "$JSON" | grep '"etternavn"' | cut -d '"' -f 4)
if [[ -z "$FORNAVN$ETTERNAVN" ]]; then
	FORNAVN="Anonym"
	ETTERNAVN="bruker"
fi

cat << EOF
<html>
	<head>
		<meta charset='utf-8'>
		<title>Vis Dikt</title>
	</head>
	<body>
		$(/www/cgi-bin/dikt/header.cgi)
		<div class='main'>
EOF

if [[ -z "$DIKT" ]]; then
	echo "<h1 style='color: red'>Kunne ikke finne dikt #$DIKTID</h1>"
else
	cat << EOF
		<h1>Dikt #$DIKTID</h1>
		<pre>$DIKT</pre>
		<p><i>- $FORNAVN $ETTERNAVN<i><p>
EOF
fi
echo "</div>"
echo "</body>"
echo "</html>"
