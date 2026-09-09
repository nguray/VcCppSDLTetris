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

echo "Generating from files in '$TARGET_DIR'..."

echo "#pragma once" >> asserts.h
echo "#include <cstdint>" >> asserts.h
echo "" >> asserts.h

echo "#include <cstdint>" >> asserts.cpp
echo "" >> asserts.cpp


# Loop through all items in the specified folder
for item in "$TARGET_DIR"/*; do
    # Check if the item is a regular file (not a folder)

    if [ -f "$item" ]; then
        # Extract and print just the file name (removing the path)
        basename "$item"
        echo "extern uint32_t $(basename "$item" | tr '.' '_');" >> asserts.h
        echo "extern uint32_t $(basename "$item" | tr '.' '_')_len;" >> asserts.h      
        #xxd -i "$item" >> asserts.cpp
        python3 res_to_uint32_header.py "$item"
        cat $(basename "$item" | tr '.' '_').h >> asserts.cpp
        rm $(basename "$item" | tr '.' '_').h

    fi
done
