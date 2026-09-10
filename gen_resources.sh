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

DESTINATION_H="$(basename "$TARGET_DIR").h"
DESTINATION_CPP="$(basename "$TARGET_DIR").cpp"

rm "$DESTINATION_CPP" "$DESTINATION_H" 2>/dev/null
touch "$DESTINATION_CPP" "$DESTINATION_H"

echo "Generating from files in '$TARGET_DIR'..."

echo "#pragma once" >> "$DESTINATION_H"
echo "#include <cstdint>" >> "$DESTINATION_H"
echo "" >> "$DESTINATION_H"

echo "#include <cstdint>" >> "$DESTINATION_CPP"
echo "" >> "$DESTINATION_CPP"


# Loop through all items in the specified folder
for item in "$TARGET_DIR"/*; do
    # Check if the item is a regular file (not a folder)

    if [ -f "$item" ]; then
        # Extract and print just the file name (removing the path)
        basename "$item"
        echo "extern uint32_t $(basename "$item" | tr '.' '_');" >> "$DESTINATION_H"
        echo "extern uint32_t $(basename "$item" | tr '.' '_')_len;" >> "$DESTINATION_H"
        #xxd -i "$item" >> "$DESTINATION_CPP"
        python3 res_to_uint32_header.py "$item"
        cat $(basename "$item" | tr '.' '_').h >> "$DESTINATION_CPP"
        rm $(basename "$item" | tr '.' '_').h

    fi
done
