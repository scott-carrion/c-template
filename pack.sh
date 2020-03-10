#!/bin/bash

# pack.sh
# DESCRIPTION: Packs the current directory and all subdirectories into a .tar archive
# 
# This program and any code herein is copyrighted by Scott Carlos Carrion, (c) 2020
# and may not be duplicated or used without express consent by the author
# 

# Exit on any error:
set -o errexit

# Define output path variable and usage info to print upon error
STR=$1
USAGE="usage: pack.sh [output_path]"

# If str is empty, print usage and exit
if [ ! -n "$STR" ]; then
	echo $USAGE
else
	# Make the .tar
	tar -cf $STR.tar -C $(pwd) .
	
	# Print contents of .tar
	echo "Packed these files: "
	tar -tf $STR.tar
fi
