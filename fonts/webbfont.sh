#!/bin/sh

usage() {
	echo "usage: $0 Fontname [size] > file.frfont"
	exit 1
}

[ -z "$1" ] && usage
[ -z "$2" ] && size="8" || size="$2"

mkdir -p /tmp/webbfont/

ASCII="! \" # $ % & \' ( ) \* + , - . / 0 1 2 3 4 5 6 7 8 9 : ; < = > ? @ A B C D E F G H I J K L M N O P Q R S T U V W X Y Z [ \ ] ^ _ \` a b c d e f g h i j k l m n o p q r s t u v w x y z { | } ~"

cat<< EOF
FORMAT FRFONT
DEFINES !"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\]^_\`abcdefghijklmnopqrstuvwxyz{|}~
SIZE $size
BLANK -
PIXEL +
EOF

for a in $(echo $ASCII | tr ' ' '\n')
do
	echo "DEF $a"
	convert -background white -fill black -font "$1" -pointsize 72 label:"$a" /tmp/webbfont/tmp.jpg
	img2txt -W $size -H $size -g 1000 /tmp/webbfont/tmp.jpg | ansifilter | \
		tr '[:graph:]' '+' | \
		tr ' ' '-'
done


