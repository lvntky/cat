#!/bin/bash

# Directory where libcat.so and its symbolic links are located
LIB_DIR="./build"

# Directory where libcat.so will be copied during installation
INSTALL_LIB_DIR="/usr/local/lib"

# Directory where header files will be copied during installation
INSTALL_INCLUDE_DIR="/usr/local/include/cat"

# Check if the LIB_DIR exists
if [ ! -d "$LIB_DIR" ]; then
    echo "Error: Directory $LIB_DIR does not exist. Please make sure the library is built."
    exit 1
fi

# Check if the INSTALL_INCLUDE_DIR exists, if not, create it
if [ ! -d "$INSTALL_INCLUDE_DIR" ]; then
    echo "Creating directory $INSTALL_INCLUDE_DIR..."
    mkdir -p "$INSTALL_INCLUDE_DIR"
fi

# Copy libcat.so to the installation library directory
echo "Copying libcat.so to $INSTALL_LIB_DIR..."
cp "$LIB_DIR/libcat.so" "$INSTALL_LIB_DIR/"

# Create symbolic links
echo "Creating symbolic links..."
ln -sf "$INSTALL_LIB_DIR/libcat.so" "$INSTALL_LIB_DIR/libcat.so.1"
ln -sf "$INSTALL_LIB_DIR/libcat.so" "$INSTALL_LIB_DIR/libcat.so.1.0.0"

# Copy header files to the installation include directory
echo "Copying header files to $INSTALL_INCLUDE_DIR..."
cp -r "$LIB_DIR/../include/"* "$INSTALL_INCLUDE_DIR/"

echo "libcat.so and headers installed successfully."
