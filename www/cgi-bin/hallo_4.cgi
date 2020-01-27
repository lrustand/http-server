#!/bin/sh
echo "Content-type:text/html;charset=utf-8"
echo

cat << EOF
<!doctype html>
<html>
 <head>
  <meta charset='utf-8'>
  <title>Hallo_4</title>
 </head>
 <body>
  <form action=hallo_4.cgi>
   <input type=text name=fornavn>
   <br>
   <input type=text name=etternavn>
   <br>
   <input type=submit>
  </form>
  QUERY_STRING:
  <br>
  $QUERY_STRING
 </body>
</html>
EOF

