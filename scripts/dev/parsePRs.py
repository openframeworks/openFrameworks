#! /usr/bin/env python

import urllib2
import json
import os

f = open ("/home/arturo/.ofprojectgenerator/config")
path = f.readline()
if path[-1]=='\n':
    path=path[:-1]
os.chdir(path)
f.close()

prssocket = urllib2.urlopen('https://api.github.com/repos/openframeworks/openFrameworks/pulls')
prsObject = json.load(prssocket)
prssocket.close()
#print dir( prsObject )


print '<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" \n"http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">'
print '<html>'
print '<head>'

f = open( 'scripts/dev/head.template' )
print f.read()
f.close()

print '<link type="text/css" rel="stylesheet" href="style.css">'

print '</head>'
print '<body>'
print '<div id="content">'

f = open( 'scripts/dev/header.template' )
print f.read()
f.close()

print '<div id="body-wrap">'
print '<div class="page-left-wide">'

f = open( 'scripts/dev/ci.template' )
print f.read()
f.close()

for pr in prsObject:
    print '<br/><a href="' + pr['html_url'] + '">' + pr['title'] + '</a><br/>'
    print '<img src="' + pr['user']['avatar_url'] + '" width="64" title="' + pr['user']['login'] + '" class="avatar"/>'
    print '<ul class="prlist">'
    print '<li>by: ' + pr['user']['login'] + '</li>'
    
    prssocket = urllib2.urlopen(pr['url'])
    prObject = json.load(prssocket)
    prssocket.close()
    
    if prObject['mergeable']:
        print '<li class="ok">merges</li>'
    else:
        print '<li class="error">doesn\'t merge</li>'
    print '<li>' + pr['body'] + '</li>'
    print '</ul>'
    



print '</div>'
print '</div>'
print '</div>'
print '</body>'
print '</html>'
