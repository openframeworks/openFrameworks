#!/usr/bin/env bash

echo "----------------------------"
echo "convert_to_github_repo.sh"
echo "  "
echo "  script useful in converting release packages to developer friendly git / github repo."
echo "  use this at first download of release package to avoid accidental loss of work as this script does a rebase"
echo "  or please backup release folder into a zip yourself before running - should be fine though"
echo "  "
echo "  "
echo "----------------------------"

REMOTE_REPO="https://github.com/openframeworks/openFrameworks.git"
BRANCH="master"

# Navigate to the directory above the script's location
cd "$(dirname "$0")/.."

# Check if .git directory already exists
if [ -d ".git" ]; then
    echo "Already a Git repository."
    exit 1
fi

# Initialize
git init
git remote add origin "$REMOTE_REPO"
git fetch origin
git checkout -b "$BRANCH" origin/"$BRANCH"

# Add all local files to the staging area
git add .
# Commit the local files - should find minimal changes from release
git commit -m "Initial commit with local files"
# Pull the latest changes from the remote repository and rebase to avoid conflicts
git pull --rebase origin "$BRANCH"
# Push the local changes to the remote repository
git push origin "$BRANCH"
echo "The directory has been converted into a Git repository and synchronized with the latest openFrameworks master branch."

exit 0
