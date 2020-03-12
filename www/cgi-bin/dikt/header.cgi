#!/bin/sh
cat << EOF
<link rel="stylesheet" type="text/css" href="/defaultStyle.css" />
<ul>
<li><a href="/cgi-bin/dikt/visAlle_dikt.cgi">Hjem</a></li>
EOF

INNLOGGET_SOM=$(wget http://127.0.0.1:3000/diktsamling/sesjon --header "Cookie: $COOKIE" -qO- | cut -d '"' -f 4)
if [[ "$INNLOGGET_SOM" = "null" ]]; then
	echo '<li style="float:right"><a href="/cgi-bin/dikt/login.cgi">Logg inn</a></li>'
else
	echo "<li><a href=opprett_dikt.cgi>Opprett dikt</a>"
	echo "<li><a href=slett_dikt.cgi>Slett dikt</a>"
	echo '<li style="float:right"><a href="/cgi-bin/dikt/logout.cgi">Logg ut</a></li>'
	echo "<li style=\"float:right\" class='white'>Logget inn som $INNLOGGET_SOM </li>"
fi

echo "</ul>"
