#!/bin/bash
set -e
## This script is to build the OpenFrameworks documentation automatically
echo "Current Branch: $TRAVIS_BRANCH"
### Only build documentation on the master branch.
if [ "$TRAVIS_BRANCH" == "master" ]; then
    if [ "$1" == "before_install" ]; then
        if [[ $TRAVIS_PULL_REQUEST == "false" ]]; then 
            echo "Documentation Before Install"
            brew update 
            brew install Caskroom/cask/libreoffice;
            brew install doxygen;
            brew install ncftp;
            gem install sass --version "=3.2.12";
            gem install compass --version "=0.12.2";
        fi
    elif [ "$1" == "script" ]; then
        if [[ $TRAVIS_PULL_REQUEST == "false" ]]; then
            echo "Documentation Building"
            cd scripts/; 
            git clone https://github.com/halfdanJ/ofDocGenerator; 
            cd ofDocGenerator; 
            npm install; 
            npm run start; 
        fi
    elif [ "$1" == "after_success" ]; then
        echo "Uploading to FTP"
        if [[ $TRAVIS_PULL_REQUEST == "false" && $FTP_USER ]]; then
            ncftpput -R -v -u $FTP_USER -p $FTP_PASSWORD 104.130.212.175 / output/*;
        fi
    fi
fi
