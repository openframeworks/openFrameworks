## download codeblocks from svn compile generate rpm and install

yum install subversion intltool wxGTK-devel libjpeg-devel libtiff-devel xterm
su -c'yum groupinstall "Development Tools"'

cd
mkdir rpm
echo "%_topdir $HOME/rpm" >> .rpmmacros
mkdir /tmp/$USER
mkdir /tmp/$USER/rpm
echo "%_tmppath /tmp/$USER/rpm" >> .rpmmacros
cd rpm
mkdir SPECS SOURCES BUILD RPMS SRPMS
RPM_TOPDIR=`rpm --eval %{_topdir}`

svn checkout svn://svn.berlios.de/codeblocks/trunk codeblocks
cd codeblocks
./bootstrap
./configure --enable-contrib
make dist
mv codeblocks-*.tar.gz $RPM_TOPDIR/SOURCES
rpmbuild -bs codeblocks.spec
cd ~/rpm/SRPMS
rpmbuild --rebuild codeblocks-*.src.rpm
cd ~/rpm/RPMS
su -c"rpm -Uvh codeblocks-*.i386.rpm"
