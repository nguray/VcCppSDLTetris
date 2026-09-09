#!/bin/bash

# Check if a folder argument was provided
if [ -z "$1" ]; then
    echo "Usage: $0 /path/to/folder"
    exit 1
fi

# Store the provided directory path
TARGET_DIR="$1"

# Check if the provided path is actually a valid directory
if [ ! -d "$TARGET_DIR" ]; then
    echo "Error: Directory '$TARGET_DIR' does not exist."
    exit 1
fi

echo "Files in '$TARGET_DIR':"
FOLDER_NAME=$(basename "$TARGET_DIR")
echo "Folder name: $FOLDER_NAME"
echo "-----------------------------------"

rm asserts.cpp asserts.h 2>/dev/null
touch asserts.cpp asserts.h

echo "Generating asserts.cpp and asserts.h from files in '$TARGET_DIR'..."

echo "#pragma once" >> asserts.h
#echo "#include \"asserts.h\"" >> asserts.cpp

# Loop through all items in the specified folder
for item in "$TARGET_DIR"/*; do
    # Check if the item is a regular file (not a folder)

    if [ -f "$item" ]; then
        # Extract and print just the file name (removing the path)
        basename "$item"

        echo "extern unsigned char ${FOLDER_NAME}_$(basename "$item" | tr '.' '_');" >> asserts.h
        echo "extern unsigned int ${FOLDER_NAME}_$(basename "$item" | tr '.' '_')_len;" >> asserts.h      
        xxd -i "$item" >> asserts.cpp

    fi
done
