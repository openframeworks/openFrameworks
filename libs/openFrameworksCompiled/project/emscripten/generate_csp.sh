#!/bin/bash

# Set script directory
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
cd $SCRIPT_DIR

# Files to hash
FILES=("index.js" "app.js")
OUTPUT_HTACCESS=".htaccess"

# Generate CSP header
CSP_HEADER="Content-Security-Policy: script-src 'self'"

# Generate hashes for each file
for FILE in "${FILES[@]}"; do
    if [ -f "$FILE" ]; then
        # Compute SHA-256 hash and convert to base64
        HASH=$(sha256sum "$FILE" | awk '{print $1}' | xxd -r -p | base64)
        CSP_HEADER="$CSP_HEADER 'sha256-$HASH'"
         echo "HASH: $FILE - [$CSP_HEADER]"
    else
        echo "Warning: $FILE not found!"
    fi
done

# Sanitize existing .htaccess
if grep -q "Content-Security-Policy:" "$OUTPUT_HTACCESS"; then
    echo "Existing CSP header found. Removing it..."
    sed -i.bak '/Content-Security-Policy:/d' "$OUTPUT_HTACCESS"
fi

# Append new CSP header
echo -e "\n# Automatically generated CSP header\n$CSP_HEADER;" >> "$OUTPUT_HTACCESS"
echo "CSP header added to $OUTPUT_HTACCESS."
