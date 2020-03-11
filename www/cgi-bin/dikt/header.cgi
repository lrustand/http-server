#!/bin/sh
echo "Content-Type: text/html"
echo

cat << EOF

<html>
  <head>
    <style>
      ul {
        position: fixed;
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
  </head>
  <body>
    <ul>
      <li><a href="/cgi-bin/dikt/home.cgi">Hjem</a></li>
      <li style="float:right"><a href="/cgi-bin/dikt/login.cgi">Logg inn</a></li>
      <li style="float:right"><a href="/cgi-bin/dikt/logout.cgi">Logg ut</a></li>
    </ul>
  </body>
</html>
EOF
