#!/bin/bash
set -ev

if [ -z ${OF_ROOT} ]; then
    OF_ROOT=${TRAVIS_BUILD_DIR:-"$( cd "$(dirname "$0")/../../.." ; pwd -P )"}
fi

# Trusty/14.04 builds don't have databases running - disable this
# echo "Memory usage debugging output"
# ps aux --sort -rss | head -n10
# echo "Stop services started by default to save memory on Travis"
# # use sudo /etc/init.d/foo stop instead of sudo service foo stop because that
# # fails on travis sometimes, see http://stackoverflow.com/a/27410479/599884
# # maybe works consistently on 14.04
# sudo service mysql stop
# sudo service postgresql stop

echo "checking assimp versions"
apt-cache policy libassimp-dev

sudo $OF_ROOT/scripts/linux/ubuntu/install_dependencies.sh -y;

#commenting out below so we can get it working with default in Ubuntu 18.04
#sudo add-apt-repository -y ppa:dns/gnu
##echo deb https://ci.openframeworks.cc/gcc_repo trusty main | sudo tee /etc/apt/sources.list.d/of_gcc_repo.list
#sudo apt-get update -q
#sudo apt-get install -y --allow-unauthenticated gcc-6 g++-6
#sudo apt-get install -y gperf coreutils libxrandr-dev libxinerama-dev libx11-dev libxcursor-dev libxi-dev
#sudo update-alternatives --install /usr/bin/g++ g++ /usr/bin/g++-6 100
#sudo update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-6 100

#uncomment to test gcc 8 ( github ci default currently uses 7.5.0 ) 
#sudo apt-get install -y --allow-unauthenticated gcc-8 g++-8
#sudo update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-8 800 --slave /usr/bin/g++ g++ /usr/bin/g++-8
#gcc -v
#g++ -v

if [ "$OPT" = "qbs" ] && [ ! -d "$TRAVIS_BUILD_DIR/linuxbrew/.linuxbrew" ]; then
    sh -c "$(curl -fsSL https://raw.githubusercontent.com/Linuxbrew/install/master/install.sh)"
    export PATH="$HOME/.linuxbrew/bin:$HOME/.linuxbrew/sbin:$PATH"
    export PATH="/home/linuxbrew/.linuxbrew/bin:$PATH"
    brew install qbs
    cp -rf /home/linuxbrew $TRAVIS_BUILD_DIR/

    # sudo apt-get install -y qt5-qmake
    # wget https://download.qt.io/official_releases/qbs/1.10.0/qbs-src-1.10.0.tar.gz
    # tar xzf qbs-src-1.10.0.tar.gz
    # rm -rf qbs
    # mv qbs-src-1.10.0 qbs
    # cd qbs
    # qmake qbs.pro
    # make
fi
