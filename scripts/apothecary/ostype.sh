# uses uname to get a more grained os type
# from http://stackoverflow.com/questions/394230/detect-the-os-from-a-bash-script

function lowercase() {
	echo "$1" | sed "y/ABCDEFGHIJKLMNOPQRSTUVWXYZ/abcdefghijklmnopqrstuvwxyz/"
}

OS=`lowercase \`uname\``

if [ "$OS" == "darwin" ]; then
	OS="osx"
elif [ "$OS" == "windowsnt" -o "${OS:0:5}" == "mingw" ] ; then
	OS="windows"
elif [ "$OS" == "linux" ]; then
	OS="linux"
fi

echo "$OS"
