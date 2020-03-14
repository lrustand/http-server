#!/bin/sh

# Fjerner dobbel backslash
fixBackslash(){
	echo "$1" | sed -e 's/\\\\n/\n/g' -e 's/\\\\t/\t/g' -e 's/\\\\.//g'
}

# Bytter ut + med mellomrom
urlDecode(){
	echo "$1" | sed 's/+/ /g'
}

# Formaterer dikt for visning. Decoder og unescaper spesialtegn
formaterForVisning(){
	urlDecode "$(fixBackslash $1)"
}
