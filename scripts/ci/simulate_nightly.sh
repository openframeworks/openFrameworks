#!/bin/bash

SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
cd $SCRIPT_DIR

OF_SCRIPT_DIR=$SCRIPT_DIR/../
OF_ROOT=$SCRIPT_DIR/../../

# Define environment variables
TARGET="linux64"
LIBS="64gcc6"
RELEASE="nightly"  # Set default release value

# Parse command line arguments
while getopts "r:" opt; do
  case ${opt} in
    r )
      RELEASE=$OPTARG
      ;;
    \? )
      echo "Usage: cmd [-r release]"
      exit 1
      ;;
  esac
done

echo "OF_ROOT: $OF_ROOT"
echo "OF_SCRIPT_DIR: $OF_SCRIPT_DIR"

if [[ "$(uname -s)" == "Linux" ]]; then
# Install libunwind
# echo "Installing libunwind..."
sudo apt-get update
sudo apt-get install -y libunwind-dev
sudo apt-get install -y npm
sudo apt-get install -y curl
fi

# Cache packages
# echo "Caching packages..."
# This part would require a custom implementation or external tool as caching 
# packages is specific to GitHub Actions and not easily replicated in bash.

# Checkout the repository (make sure the script is executed in a repository context)
# echo "Checking out the repository..."
# git checkout master

# Run ccache (assumes ccache is installed and configured)
echo "Running ccache..."
ccache --show-stats

# Install dependencies
echo "Installing dependencies..."
$SCRIPT_DIR/$TARGET/install.sh

# Update submodules
echo "Updating submodules..."
$OF_ROOT/scripts/dev/init_submodules.sh

# Download libraries
echo "Downloading libraries..."
$OF_ROOT/scripts/linux/download_libs.sh -a $LIBS

# Create package
echo "Creating package..."
$SCRIPT_DIR/package_builds.sh $RELEASE

# List output directory
echo "Listing output directory..."
ls -lah $OF_ROOT/out/

# Update Release (assuming you have a script or tool to handle GitHub releases)
# echo "Updating release..."
# This part would require a custom implementation or external tool as updating 
# releases is specific to GitHub Actions and not easily replicated in bash.
# Replace this with your custom update release script or API call.

