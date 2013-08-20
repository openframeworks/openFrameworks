#!/bin/bash
DISTROVER=$(grep ^VERSION /etc/SuSE-release | tr -d ' ' | cut -d = -f 2)
zypper addrepo obs://devel:tools:ide/openSUSE_$DISTROVER devel:tools:ide
zypper refresh devel:tools:ide
zypper install codeblocks xterm
