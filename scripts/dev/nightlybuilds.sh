#!/bin/bash

lastversion=$(date +%Y%m%d)
REPORT_MAIL=arturo@openframeworks.cc

echo "Building nightly builds $lastversion"

. $HOME/.profile

set -o pipefail  # trace ERR through pipes
set -o errtrace  # trace ERR through 'time command' and other functions
set -o nounset   # set -u : exit the script if you try to use an uninitialized variable
set -o errexit   # set -e : exit the script if any statement returns a non-true return value

error() {
  local parent_lineno="$1"
  if [[ "$#" = "3" ]] ; then
    local message="$2"
    local code="${3:-1}"
    msg="Error on or near line ${parent_lineno}: ${message}; exiting with status ${code}"
  else
    local code="${2:-1}"
    msg="Error on or near line ${parent_lineno}; exiting with status ${code}"
  fi
  echo $msg
  mail -s "Error creating nightly builds at $(date)." $REPORT_MAIL <<EOF
Error creating nightly builds
$(if [ -f /home/ofadmin/logs/nightlybuilds.log ]; then cat /home/ofadmin/logs/nightlybuilds.log fi)
$(if [ -f /home/ofadmin/logs/compilePG.log ]; then echo; echo; echo "PG compile log: "; cat /home/ofadmin/logs/compilePG.log fi)
EOF
  rm -f *.tar.gz
  rm -f *.zip
  exit "${code}"
}
trap 'error ${LINENO}' ERR

touch lasthash.txt
export PG_OF_PATH=$(cat ~/.ofprojectgenerator/config)
cd $(cat ~/.ofprojectgenerator/config)
git fetch upstreamhttps
git reset --hard upstreamhttps/master
scripts/dev/download_libs.sh

cd $(cat ~/.ofprojectgenerator/config)/scripts/dev
lasthash=$(cat lasthash.txt)
currenthash=$(git rev-parse HEAD)
if [ "$currenthash" = "$lasthash" ]; then
    echo "Exiting, no changes found"
    exit 0
fi

echo $currenthash>lasthash.txt
./create_package.sh linux64 $lastversion master gcc4
./create_package.sh linux64 $lastversion master gcc5
./create_package.sh linux64 $lastversion master gcc6
./create_package.sh msys2 $lastversion master
./create_package.sh vs2017 $lastversion master
./create_package.sh ios $lastversion master
./create_package.sh osx $lastversion master
./create_package.sh android $lastversion master
./create_package.sh linuxarmv6l $lastversion master
./create_package.sh linuxarmv7l $lastversion master

# delete older packages
rm -f /var/www/versions/nightly/of_v*_nightly.*

mv *.tar.gz /var/www/versions/nightly
mv *.zip /var/www/versions/nightly

mv /var/www/versions/nightly/of_v${lastversion}_linux64gcc4_release.tar.gz /var/www/versions/nightly/of_v${lastversion}_linux64gcc4_nightly.tar.gz
mv /var/www/versions/nightly/of_v${lastversion}_linux64gcc5_release.tar.gz /var/www/versions/nightly/of_v${lastversion}_linux64gcc5_nightly.tar.gz
mv /var/www/versions/nightly/of_v${lastversion}_linux64gcc6_release.tar.gz /var/www/versions/nightly/of_v${lastversion}_linux64gcc6_nightly.tar.gz
mv /var/www/versions/nightly/of_v${lastversion}_msys2_release.zip /var/www/versions/nightly/of_v${lastversion}_msys2_nightly.zip
mv /var/www/versions/nightly/of_v${lastversion}_vs2017_release.zip /var/www/versions/nightly/of_v${lastversion}_vs2017_nightly.zip
mv /var/www/versions/nightly/of_v${lastversion}_ios_release.zip /var/www/versions/nightly/of_v${lastversion}_ios_nightly.zip
mv /var/www/versions/nightly/of_v${lastversion}_osx_release.zip /var/www/versions/nightly/of_v${lastversion}_osx_nightly.zip
mv /var/www/versions/nightly/of_v${lastversion}_android_release.tar.gz /var/www/versions/nightly/of_v${lastversion}_android_nightly.tar.gz
mv /var/www/versions/nightly/of_v${lastversion}_linuxarmv6l_release.tar.gz /var/www/versions/nightly/of_v${lastversion}_linuxarmv6l_nightly.tar.gz
mv /var/www/versions/nightly/of_v${lastversion}_linuxarmv7l_release.tar.gz /var/www/versions/nightly/of_v${lastversion}_linuxarmv7l_nightly.tar.gz

echo '<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN"
    "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">'> /var/www/nightlybuilds.html
echo '<html>' >> /var/www/nightlybuilds.html
echo '<head>' >> /var/www/nightlybuilds.html
cat head.template >> /var/www/nightlybuilds.html
echo '</head>' >> /var/www/nightlybuilds.html
echo '<body id="nightlybuilds" style="background-color:#ffffff;text-align:left">' >> /var/www/nightlybuilds.html
cat nightlybuilds.template >> /var/www/nightlybuilds.html
echo '<p>This download corresponds to commit: <a href="https://github.com/openframeworks/openFrameworks/commit/'${currenthash}'">'${currenthash}'</a></p>' >> /var/www/nightlybuilds.html
prev_version=0
for pkg in $(ls -r /var/www/versions/nightly/ | grep -v latest); do
    version=$(echo $pkg | cut -c4-12)
    if [ "$version" != "$prev_version" ]; then
        echo '<br/><h3>'${version}'</h3>'  >> /var/www/nightlybuilds.html
    fi
    prev_version=$version
    echo '<a href="versions/nightly/'${pkg}'">'${pkg}'</a><br/>'  >> /var/www/nightlybuilds.html
done
echo '</body>' >> /var/www/nightlybuilds.html
echo '</html>' >> /var/www/nightlybuilds.html

#wget http://openframeworks.cc/nightly_hook.php?version=${lastversion} -O /dev/null

echo
echo
echo "Successfully created nightly builds for ${lastversion}"
echo

mail -s "Nightly builds $lastversion OK." $REPORT_MAIL <<EOF
Successfully created nightly builds for ${lastversion}

$(if [ -f /home/ofadmin/logs/nightlybuilds.log ]; then cat /home/ofadmin/logs/nightlybuilds.log; fi)
EOF

