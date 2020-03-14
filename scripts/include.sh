#!/bin/sh

# Fjerner dobbel backslash
fixBackslash(){
	echo "$1" | sed -e 's/\\\\n/\n/g' -e 's/\\\\t/\t/g' -e 's/\\\\.//g'
}

# Bytter ut + med mellomrom
urlDecode(){
	echo "$1" | sed 's/+/ /g'
}

# Escaper < og > for å unngå html/javascript injection
escapeLtGt(){
	echo "$1" | sed -e 's/</\&lt;/g' -e 's/>/\&gt;/g'
}

# Formaterer dikt for visning. Decoder og unescaper spesialtegn
formaterForVisning(){
	urlDecode "$(fixBackslash $(escapeLtGt $1))"
}
