#!/bin/sh
echo "Content-Type: text/html;charset=utf-8"
echo

/www/cgi-bin/dikt/header.cgi

ALLE_DIKT=$(wget -qO- http://127.0.0.1:3000/diktsamling/dikt/)

cat << EOF
<html>
	<head>
		<meta charset='utf-8'>
		<title>Alle Dikt</title>
	</head>
	<body>
		<div class='main'>
EOF

IFS="}"
for DIKT in $ALLE_DIKT; do
	hent_felt ()
	{
		echo "$DIKT" | grep $1 |  cut -d : -f 2 | sed -e 's/[ ,"]//g'
	}
	DIKTID=$(hent_felt diktid)
	INNHOLD=$(hent_felt dikt\")
	FORNAVN=$(hent_felt fornavn)
	ETTERNAVN=$(hent_felt etternavn)
	if [[ "$FORNAVN" = "null" ]]; then
		FORNAVN=
	fi
	if [[ "$ETTERNAVN" = "null" ]]; then
		ETTERNAVN=
	fi
	if [[  "${FORNAVN}${ETTERNAVN}" = "" ]]; then
		FORNAVN=Anonym
		ETTERNAVN=bruker
	fi

	if [[ ! -z "$DIKTID" ]]; then
		echo "<div class='dikt'>"
		echo "<h3><a href=vis_dikt.cgi?diktid=$DIKTID>Dikt #$DIKTID</a></h3>"
		echo "Skrevet av: $FORNAVN $ETTERNAVN"
		echo "</div>"
	fi
done

cat << EOF
		</div>"
	</body>
</html>
EOF
