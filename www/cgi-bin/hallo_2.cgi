#!/bin/sh

# Skriver ut 'http-header' og tom linje
echo "Content-type:text/plain;charset=utf-8"
echo

# Skriver ut 'http-kropp'
echo Variabelen QUERY_STRING:  $QUERY_STRING
