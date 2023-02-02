#!/usr/bin/bash
awk "NR%$1==0" $2 | awk -F',' '{print NR "," $3 "," $4}'
