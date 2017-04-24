#!/bin/bash
if [[ $# -eq 0 ]] ; then
	echo 'You need to pass frame rate as argument'
else 
	echo 'Creating video with $1 fps'
	ffmpeg -r $1 -pattern_type glob -i '*.jpg' trk.mp4
fi
