#!/bin/sh
echo "Content-Type: text/html;charset=utf-8"
echo

if [ "$REQUEST_METHOD" = "POST" ]; then
	if [[ $CONTENT_LENGTH -gt 0 ]]; then
		LANG=C IFS= read -r -d '' -n $CONTENT_LENGTH BODY
		NAME=$(echo "$BODY" | cut -d '=' -f 1)
		ID=$(echo "$BODY" | cut -d '=' -f 2 )
	fi
	if [ "$NAME" = "*" ]; then
		REQUEST="DELETE /diktsamling/dikt/ HTTP/1.1\n"
		MESSAGE="Alle dine dikt er nå slettet!"
	else
		if [[ "$ID" = "" ]]; then
			echo "Velg et dikt før du prøver å slette!"
		else
			REQUEST="DELETE /diktsamling/dikt/$ID HTTP/1.1\n"
			MESSAGE="Dikt#$ID er slettet"

		fi
	fi
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
		<link rel="stylesheet" type="text/css" href="/defaultStyle.css" />
	</head>
	<body>
		$(/www/cgi-bin/dikt/header.cgi)
		<div class="main">
		$MESSAGE
		</div>
	</body>
</html>
EOF
