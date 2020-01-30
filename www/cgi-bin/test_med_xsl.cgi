#!/bin/sh
 
 cat <<EOF
 Content-type: application/xml; charset=utf-8
 
<?xml version="1.0"?>
 
	tabell>
		rad>
 EOF
 
 echo -n "
<bookstore>
  <book category="children">
    <title>Harry Potter</title>
    <author>J K. Rowling</author>
    <year>2005</year>
    <price>29.99</price>
  </book>
  <book category="web">
    <title>Learning XML</title>
    <author>Erik T. Ray</author>
    <year>2003</year>
    <price>39.95</price>
  </book>
</bookstore>"

cat <<EOF
  </rad>
</tabell>
EOF
