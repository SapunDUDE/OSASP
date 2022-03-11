#!/bin/bash

if [ $# -eq 2 ]
then
	if [ ! -f $1 ]
	then
		{	
			echo "$1 : this file doesn't exist"
		}>&2
	else
		gcc $1 -o "$2.exe" && ./$2.exe
	fi
else
	{
		echo "First argument = C-file to compile."
		echo "Second argument = exe-file output path."
	}>&2
fi
		
		

