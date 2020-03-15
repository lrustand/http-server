#!/bin/sh
echo "Content-Type: text/html;charset=utf-8"
echo

source /scripts/include.sh
/www/cgi-bin/dikt/header.cgi

MINE_DIKT=$(wget --header "Cookie: $COOKIE" -qO- http://127.0.0.1:3000/diktsamling/bruker)

cat << EOF
<html>
	<head>
		<meta charset='utf-8'>
		<title>Mine Dikt</title>
	</head>
	<body>
		<div class='main'>
EOF

LEST_DIKT=false
IFS="}"
for DIKT in $MINE_DIKT; do
	hent_felt ()
	{
		echo "$DIKT" | grep $1 |  cut -d : -f 2 | sed -e 's/[ ,"]//g'
	}
	DIKTID=$(hent_felt diktid)
	INNHOLD=$(hent_felt dikt\")
	INNHOLD=$(formaterForVisning "$INNHOLD")

	if [[ ! -z "$DIKTID" ]]; then
		echo "<div class='dikt'>"
		echo "<h3><a href=vis_dikt.cgi?diktid=$DIKTID>Dikt #$DIKTID</a></h3>"
		echo "<pre>$INNHOLD</pre>"
		echo "<br><br>"
		echo "<form>"
		echo "<button formaction='endre_dikt.cgi' formmethod='get' name='diktid' value='$DIKTID' type='submit'>Endre</button>"
		echo "<button formaction='slett_dikt.cgi' formmethod='post' name='diktid' value='$DIKTID' type='submit'>Slett</button>"
		echo "</form>"
		echo "</div>"
		LEST_DIKT=true
	fi
done

if [[ "$LEST_DIKT" = "true" ]]; then
	echo "<br><hr><br>"
	echo "<form>"
	echo "<button formaction='slett_dikt.cgi' formmethod='post' name='*' value='' type='submit'>Slett alle mine dikt</button>"
	echo "</form>"
else
	echo "Du har ingen dikt. Opprett et dikt for å se det her"
fi

cat << EOF
		</div>
	</body>
</html>
EOF
