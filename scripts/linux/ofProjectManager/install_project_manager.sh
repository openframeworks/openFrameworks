zenity --info --title "openFrameworks project manager" --text "This script is going to install the openFrameworks project manager tool. To create a new project from a console, use:\n\nofProject create project_name\n\nto add addons to a project:\n\nofProject add project_name ofxAddonName"
gksudo -- bash -c 'apt-get install -y python2.5 python-amara  | zenity --width=400 --height=100 --progress --pulsate --auto-close --title="Installing openFrameworks project manager, please wait..."
mkdir /usr/local/ofProjectManager
cp * /usr/local/ofProjectManager
rm /usr/local/bin/ofProject
ln -s /usr/local/ofProjectManager/ofProjectManager.py /usr/local/bin/ofProject
zenity --info --title "openFrameworks project manager" --text "You will need to set your openFrameworks installation dir in ofEnvironment.py. Press ok to edit the file"
gedit /usr/local/ofProjectManager/ofEnvironment.py'
