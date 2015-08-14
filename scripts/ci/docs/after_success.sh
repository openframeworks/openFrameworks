#!/bin/bash
set -ev
echo "Uploading documentation to FTP server"
if [[ $TRAVIS_PULL_REQUEST == "false" && $FTP_USER ]]; then
    ncftpput -R -v -u $FTP_USER -p $FTP_PASSWORD 104.130.212.175 / output/*;
else
    echo "On a PR, skipping upload. Only direct commits are uploaded.";
fi
