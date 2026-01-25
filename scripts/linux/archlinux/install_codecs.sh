if [ $EUID != 0 ]; then
	echo "this script must be run as root"
	echo ""
	echo "usage:"
	echo "sudo ./install_codecs.sh"
	exit $exit_code
   exit 1
fi

pacman -S --needed mpg123 gst-plugins-ugly
