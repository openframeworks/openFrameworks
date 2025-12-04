#!/bin/bash

git submodule update --init --recursive
# Fetch the latest changes for each submodule and check out the default branch
git submodule foreach --recursive '
  default_branch=$(git remote show origin | grep "HEAD branch" | cut -d: -f2 | tr -d " ")
  echo "Updating submodule $name to latest commit on $default_branch branch"
  git fetch origin $default_branch
  git checkout $default_branch
  git pull origin $default_branch
'
