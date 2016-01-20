#!/bin/bash

CLASS1=gov/nasa/larcfm

JSRC1=src/$CLASS1/Reliab/*.java

rm -rf lib/WinSURE.jar

if [ ! -e "bin" ]
then
mkdir bin
fi

arch=`uname -s | awk '{print tolower(substr($0,0,6))}' -`
case $arch in
     darwin) myarch=macos;;
     linux)  myarch=linux;;
     cygwin) myarch=cygwin;;
     *)      echo "Only run under Darwin, Linux, and Cygwin";exit 1
esac

if [ ! -e "bin" ]
then
mkdir bin
fi

rm -rf bin/*

if [[ "$myarch" == 'cygwin' ]]; then
javac -d bin $JSRC1 
else
javac -d bin -Xlint:unchecked $JSRC1 || exit 
fi

cd bin
jar cvf ../lib/WinSURE.jar $CLASS1/Reliab/*.class
cd ..
jar i lib/WinSURE.jar
echo "--------------- lib/WinSURE.jar CREATED -------------------------"
