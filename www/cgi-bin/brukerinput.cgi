#!/bin/sh

export IFS="&"
for a in $QUERY_STRING; do
	var=$(echo $a | cut -d '=' -f 1)
	val=$(echo $a | cut -d '=' -f 2)
	if [ "$var" = "title" ]; then
		title=$val
	elif [ "$var" = "author" ]; then
		author=$val
	elif [ "$var" = "year" ]; then
		year=$val
	fi
done

cat <<EOF
Content-type: application/xml; charset=utf-8

<?xml version="1.0"?>
<?xml-stylesheet type="text/xsl" href="/styletest.xsl"?>

<book	xmlns="http://www.w3schools.com/RedsDevils"
		xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
		xsi:schemaLocation="http://www.w3schools.com/RedsDevils /xmlschematest.xsd">

	<title>$title</title>
	<author>$author</author>
	<year>$year</year>
</book>
EOF
