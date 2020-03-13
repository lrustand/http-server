#!/bin/sh
echo "Content-Type: text/html;charset=utf-8"
echo

/www/cgi-bin/dikt/header.cgi

MINE_DIKT=$(wget --header "Cookie: $COOKIE" -qO- http://127.0.0.1:3000/diktsamling/bruker)

echo "<div class='main'>"

IFS="}"
for DIKT in $MINE_DIKT; do
	hent_felt ()
	{
		echo "$DIKT" | grep $1 |  cut -d : -f 2 | sed -e 's/[ ,"]//g'
	}
	DIKTID=$(hent_felt diktid)
	INNHOLD=$(hent_felt dikt\")

	if [[ ! -z "$DIKTID" ]]; then
		echo "<div class='dikt'>"
		echo "<h3><a href=vis_dikt.cgi?diktid=$DIKTID>Dikt #$DIKTID</a></h3>"
		echo "$INNHOLD"
		echo "<br><br>"
		echo "<form>"
		echo "<button formaction='endre_dikt.cgi' formmethod='get' name='diktid' value='$DIKTID' type='submit'>Endre</button>"
		echo "<button formaction='slett_dikt.cgi' formmethod='post' name='diktid' value='$DIKTID' type='submit'>Slett</button>"
		echo "</form>"
		echo "</div>"
	fi
done

echo "<br><hr><br>"
echo "<form>"
echo "<button formaction='slett_dikt.cgi' formmethod='post' name='*' value='' type='submit'>Slett alle mine dikt</button>"
echo "</form>"

echo "</div>"
