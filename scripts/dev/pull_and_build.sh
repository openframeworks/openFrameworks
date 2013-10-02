#!/bin/bash
repo_url='https\:\/\/github\.com\/openframeworks\/openFrameworks\/blob\/master\/'
EMAILMESSAGE="/tmp/emailmessage.txt"
#EMAIL="of-dev@dev.openframeworks.cc"
EMAIL="arturo@openframeworks.cc"
SUBJECT="errors building OF"
FROM="ofadmin@ci.openframeworks.cc"
errors=0

function generateLog {
    error=0
    grep error ofbuild.log > /dev/null
    if [ $? -eq 0 ]; then 
	    echo '<label for="errorlog'${1}'" class="error toggle"> errors </label>' >> /var/www/ofbuild.html
	    echo '<input id="errorlog'${1}'" type="checkbox" checked="false" class="log error toggle"/>' >> /var/www/ofbuild.html
	    echo '<div class="error log">' >> /var/www/ofbuild.html
	    grep error ofbuild.log  | sed "s/^\(${4}\(.*\)\:\([0-9]*\)\:.*$\)//g" | sed "s/^\(${2}\(.*\)\:\([0-9]*\)\: error\:.*$\)/\<p\>\<a href=\"${repo_url}${3}\2#L\3\"\>\1\<\/a\>\<\/p\>/g" >> /var/www/ofbuild.html
	    grep error ofbuild.log  | sed "s/^\(${2}\(.*\)\:\([0-9]*\)\:.*$\)//g" | sed "s/^\(${4}\(.*\)\:\([0-9]*\)\: error\:.*$\)/\<p\>\<a href=\"${repo_url}${5}\2#L\3\"\>\1\<\/a\>\<\/p\>/g" >> /var/www/ofbuild.html
	    echo '</div>' >> /var/www/ofbuild.html
	    
	    
	    echo $6 >> $EMAILMESSAGE
	    grep error ofbuild.log  | sed "s/^\(${4}\(.*\)\:\([0-9]*\)\:.*$\)//g" | sed "s/^\(${2}\(.*\)\:\([0-9]*\)\: error\:.*$\)/\n \1\n ${repo_url}${3}\2#L\3/g" >> $EMAILMESSAGE
	    grep error ofbuild.log  | sed "s/^\(${2}\(.*\)\:\([0-9]*\)\:.*$\)//g" | sed "s/^\(${4}\(.*\)\:\([0-9]*\)\: error\:.*$\)/\n \1\n ${repo_url}${5}\2#L\3/g" >> $EMAILMESSAGE
	    echo >> $EMAILMESSAGE
	    echo >> $EMAILMESSAGE
	    error=1	
	    errors=1
    fi

    warning=0
    grep warning ofbuild.log > /dev/null
    if [ $? -eq 0 ]; then
	    echo '<label for="warninglog'${1}'" class="warning toggle"> warnings </label>' >> /var/www/ofbuild.html
	    echo '<input id="warninglog'${1}'" type="checkbox" checked="false" class="log warning toggle"/>' >> /var/www/ofbuild.html
	    echo '<div class="warning log">' >> /var/www/ofbuild.html
	    grep warning ofbuild.log  | sed "s/^\(${4}\(.*\)\:\([0-9]*\)\:.*$\)//g" | sed "s/^\(${2}\(.*\)\:\([0-9]*\)\: warning\:.*$\)/\<p\>\<a href=\"${repo_url}${3}\2#L\3\"\>\1\<\/a\>\<\/p\>/g" >> /var/www/ofbuild.html
	    grep warning ofbuild.log  | sed "s/^\(${2}\(.*\)\:\([0-9]*\)\:.*$\)//g" | sed "s/^\(${4}\(.*\)\:\([0-9]*\)\: warning\:.*$\)/\<p\>\<a href=\"${repo_url}${5}\2#L\3\"\>\1\<\/a\>\<\/p\>/g" >> /var/www/ofbuild.html
	    echo '</div>' >> /var/www/ofbuild.html
	    warning=1
    fi

    if [ $error -eq 0 ] && [ $warning -eq 0 ]; then
	    echo '<label for="oklog'${1}'" class="ok"> ok </label>' >> /var/www/ofbuild.html
    fi
    
    echo '<br/><br/>' >> /var/www/ofbuild.html
}


echo "To: ${EMAIL}" >> $EMAILMESSAGE
echo "Subject: ${SUBJECT}" >> $EMAILMESSAGE
echo "The following errors were found while compiling the master branch of openFrameworks" >> $EMAILMESSAGE
echo "." >> $EMAILMESSAGE
echo >> $EMAILMESSAGE

cd $(cat ~/.ofprojectgenerator/config)
git pull upstreamhttps master
make -C libs/openFrameworksCompiled/project/linux64 clean
echo '<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" 
    "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">'> /var/www/ofbuild.html
echo '<html>' >> /var/www/ofbuild.html
echo '<head>' >> /var/www/ofbuild.html
cat scripts/dev/head.template >> /var/www/ofbuild.html
echo '</head>' >> /var/www/ofbuild.html
echo '<body>' >> /var/www/ofbuild.html
echo '<div id="content">' >> /var/www/ofbuild.html
cat scripts/dev/header_development.template >> /var/www/ofbuild.html

echo '<div id="body-wrap">' >> /var/www/ofbuild.html
echo '<div class="page-left-wide">' >> /var/www/ofbuild.html
cat scripts/dev/ci.template >> /var/www/ofbuild.html
echo '</div>' >> /var/www/ofbuild.html

echo '<div class="page-wide">' >> /var/www/ofbuild.html

make -C libs/openFrameworksCompiled/project/linux64 2> ofbuild.log
echo '<br/><h2>OF core</h2>' >> /var/www/ofbuild.html
generateLog OF '\.\.\/\.\.\/\.\.\/' 'libs\/' 'src' 'src' 'OF core'

for category in $(ls * -d | grep -v ios | grep -v android); do
    for example in *; do
        rm ${category}/${example}/Makefile
        rm ${category}/${example}/config.make
    done
done
projectGenerator --allexamples
cd $(cat ~/.ofprojectgenerator/config)
cd examples
for category in $(ls * -d | grep -v ios | grep -v android); do
    cd $category
    echo '<br/><h2>'${category}'</h2><br/>' >> /var/www/ofbuild.html
    for example in *; do
        cd $example
        make clean > /dev/null
        make 2> ofbuild.log
        echo '<h3>'${example}'</h3>' >> /var/www/ofbuild.html
        generateLog ${example} '\.\.\/\.\.\/\.\.\/' '' 'src' examples'\/'${category}'\/'${example}'\/'src ${category}'/'${example}
        cd ..
    done
    cd ..
done

echo '</div>' >> /var/www/ofbuild.html
echo '</div>' >> /var/www/ofbuild.html
echo '</div>' >> /var/www/ofbuild.html
echo '</body>' >> /var/www/ofbuild.html
echo '</html>' >> /var/www/ofbuild.html


if [ $errors -eq 1 ]; then
    /usr/sbin/sendmail ${EMAIL} -f${FROM} < $EMAILMESSAGE
fi

rm $EMAILMESSAGE


