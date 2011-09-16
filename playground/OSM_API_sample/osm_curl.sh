#!/bin/bash

echo '<?xml version="1.0" encoding="UTF-8"?>' 					 > set.xml
echo '<osm>'									>> set.xml
echo '	<changeset>'								>> set.xml
echo '	    <tag k="created_by" v="JOSM 1.61"/>'				>> set.xml
echo '	    <tag k="comment" v="Just adding some streetnames"/>'		>> set.xml
echo '	</changeset>'								>> set.xml
echo '</osm>'									>> set.xml

cat set.xml
changesetId=$(curl -u osll:osll123456 -T set.xml http://www.openstreetmap.org/api/0.6/changeset/create)

echo '<?xml version="1.0" encoding="UTF-8"?>' 					 > node1.xml
echo '<osm>'									>> node1.xml
echo "	<node changeset=\"${changesetId}\" lat=\"0.4\" lon=\"0.4\">"		>> node1.xml
echo '	<tag k="note" v="Point A"/>'						>> node1.xml
echo '	</node>'								>> node1.xml
echo '</osm>'									>> node1.xml

echo '<?xml version="1.0" encoding="UTF-8"?>' 					 > node2.xml
echo '<osm>'									>> node2.xml
echo "	<node changeset=\"${changesetId}\" lat=\"0.4\" lon=\"0.5\">"		>> node2.xml
echo '	<tag k="note" v="Point B"/>'						>> node2.xml
echo '	</node>'								>> node2.xml
echo '</osm>'									>> node2.xml

echo '<?xml version="1.0" encoding="UTF-8"?>' 					 > node3.xml
echo '<osm>'									>> node3.xml
echo "	<node changeset=\"${changesetId}\" lat=\"0.5\" lon=\"0.5\">"		>> node3.xml
echo '	<tag k="note" v="Point C"/>'						>> node3.xml
echo '	</node>'								>> node3.xml
echo '</osm>'									>> node3.xml						

cat node1.xml
cat node2.xml
cat node3.xml
idPoi1=$(curl -u osll:osll123456 -T node1.xml http://www.openstreetmap.org/api/0.6/node/create)
idPoi2=$(curl -u osll:osll123456 -T node2.xml http://www.openstreetmap.org/api/0.6/node/create)
idPoi3=$(curl -u osll:osll123456 -T node3.xml http://www.openstreetmap.org/api/0.6/node/create)


echo '<?xml version="1.0" encoding="UTF-8"?>'					 > way.xml
echo '<osm>'									>> way.xml
echo "	<way changeset=\"${changesetId}\">" 					>> way.xml
echo '		<tag k="note" v="Just a GF"/>' 					>> way.xml
echo "		<nd ref=\"${idPoi1}\"/>"					>> way.xml
echo "		<nd ref=\"${idPoi2}\"/>"					>> way.xml
echo "		<nd ref=\"${idPoi3}\"/>"					>> way.xml
echo '	</way>'									>> way.xml
echo '</osm>'									>> way.xml
idArea=$(curl -u osll:osll123456 -T way.xml http://www.openstreetmap.org/api/0.6/way/create)

echo "changeset id = ${changesetId}" 
echo "POI number one id = ${idPoi1}" 
echo "POI number two id = ${idPoi2}" 
echo "POI number three id = ${idPoi3}" 
echo "Area id = ${idArea}"
echo "" 
echo "" 


echo "REQUEST GEO DATA FROM SERVER" 
curl http://www.openstreetmap.org/api/0.6/changeset/${changesetId}
curl http://www.openstreetmap.org/api/0.6/node/${idPoi1}
curl http://www.openstreetmap.org/api/0.6/node/${idPoi2}
curl http://www.openstreetmap.org/api/0.6/node/${idPoi3}
curl http://www.openstreetmap.org/api/0.6/way/${idArea}

#curl http://www.openstreetmap.org/api/0.6/changesets?display_name=osll



