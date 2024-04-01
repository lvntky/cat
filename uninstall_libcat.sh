#!/bin/bash

# Directory where libcat.so and its symbolic links are located
LIB_DIR="./build"

# Directory where libcat.so is installed
INSTALL_LIB_DIR="/usr/local/lib"

# Directory where header files are installed
INSTALL_INCLUDE_DIR="/usr/local/include/cat"

# Remove libcat.so and its symbolic links
echo "Removing libcat.so and symbolic links from $INSTALL_LIB_DIR..."
rm -f "$INSTALL_LIB_DIR/libcat.so" "$INSTALL_LIB_DIR/libcat.so.1" "$INSTALL_LIB_DIR/libcat.so.1.0.0"

# Remove header files
echo "Removing header files from $INSTALL_INCLUDE_DIR..."
rm -rf "$INSTALL_INCLUDE_DIR"

echo "Uninstallation completed successfully."
