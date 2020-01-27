#!/bin/sh

echo "Content-type:text/html;charset=utf-8"
echo

cat << EOF
<!doctype html>
<html>
 <head>
  <meta charset='utf-8'>
  <title>Hallo_3</title>
 </head>
 <body>
  Variabelen QUERY_STRING: $QUERY_STRING
 </body>
</html>
EOF
