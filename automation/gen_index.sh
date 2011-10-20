#!/bin/bash

#Source file
. $CATALINA_HOME/geo2tag-build

#Result file
index_html="/var/www/index.html"
#index_html="index.html"

#Reading and Printing the values.

echo '<p align="center">' > $index_html
echo '<img src="images/globe-150x150.jpg" border="0"><br>' >> $index_html
echo '<font size="7">Current build</font><br>' >> $index_html
echo "<font size=\"6\">Version: ${version}</font><br>" >> $index_html
echo "<font size=\"6\">Date of deploy: ${deployDate} </font><br>" >> $index_html
echo '<a href="http://tracks.osll.spb.ru:8080/geo2tag/"><font size="6">Demo server</font><br></a><br>' >> $index_html
echo '</p>' >> $index_html
