#!/bin/bash

# $1 = MinSize
# $2 = MaxSize
# $3 = Directory

if [ $# -eq 3 ]
then
	if [ ! -d $3 ]
	then
		{
			echo "$1 : this directory doesn't exist"
		}>&2
	else
		{
			find $3 -maxdepth 1 -size +$1c -size -$2c -type f -printf "%s %p\n" | sort -n 
		}
	fi
else
	{
		echo "First argument = Min Size."
		echo "Second argument = Max Size"
		echo "Third argument = Directory"
	}>&2
fi
