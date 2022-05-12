#!/bin/bash

find ../ -type f -name '*.md' | while read FILE; do
	NEW_FILE="$(dirname "$FILE")/$(basename "$FILE" .md).pdf"
	pandoc --citeproc --bibliography ../bibliography.bib --csl university-of-york-apa.csl -o "$NEW_FILE" "$FILE"
done
