#!/bin/bash


if [ $# -eq 3 ]
then
	if [ ! -d $2 ]
	then
		{
			echo "$2 : this directory doesn't exist"
		}>&2
	else
		find $2 -maxdepth 1 -type f -name "*.$3" -printf "%f\n" | sort -d >$1
	fi
else
	{
		echo "First argument = outputfile path."
		echo "Second argument = directory path."
		echo "Third argument = file extension."
	}>&2
fi


