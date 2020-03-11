#!/bin/sh
echo "Content-Type: text/html;charset=utf-8"
echo

ALLE_DIKT=$(wget -qO- http://127.0.0.1:3000/diktsamling/dikt/)

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
	echo "<div class='dikt'>"
	echo "<h3><a href=vis_dikt.cgi?diktid=$DIKTID>Dikt #$DIKTID</a></h3>"
	echo "Skrevet av: $FORNAVN $ETTERNAVN"
	echo "</div>"
done
