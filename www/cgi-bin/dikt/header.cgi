#!/bin/sh
cat << EOF

    <style>
      ul {
        top: 0;
        left: 0;
        width: 100%;
        list-style-type: none;
        margin: 0;
        padding: 0;
        overflow: hidden;
        background-color: #333;
      }

      li {
        float: left;
      }
	  .white {
        color: gray;
        padding: 10px 15px;
        text-align: center;
	  }

      li a {
        display: block;
        color: white;
        text-align: center;
        padding: 10px 15px;
        text-decoration: none;
      }

      li a:hover {
        background-color: #111;
      }

      .active {
        background-color: #070;
      }
    </style>
    <ul>
      <li><a href="/cgi-bin/dikt/home.cgi">Hjem</a></li>
EOF

INNLOGGET_SOM=$(wget http://127.0.0.1:3000/diktsamling/sesjon --header "Cookie: $COOKIE" -qO- | cut -d '"' -f 4)
if [[ "$INNLOGGET_SOM" = "null" ]]; then
	echo '<li style="float:right"><a href="/cgi-bin/dikt/login.cgi">Logg inn</a></li>'
else
	echo '<li style="float:right"><a href="/cgi-bin/dikt/logout.cgi">Logg ut</a></li>'
	echo "<li style=\"float:right\" class='white'>Logget inn som $INNLOGGET_SOM </li>"
fi

echo "</ul>"
