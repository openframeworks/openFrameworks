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
        ARCH=`uname -m`
	if [ "$ARCH" == "x86_64" ] ; then
		OS="linux64"
	elif [ "$ARCH" == "armv6l" ] ; then
		OS="linuxarmv6l"
	elif [ "$ARCH" == "armv7l" ] ; then
		OS="linuxarmv7l"
	elif [ "$ARCH" == "i386" ] ; then
		OS="linux"	
	else
		# A value of UNKNOWN will cause ./apothecary to fail.
		OS="UNKNOWN"	
	fi
fi

echo "$OS"
