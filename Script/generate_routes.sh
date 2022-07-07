#!/bin/bash

if [ $# -ne 2 ]; then
    echo "Usage: ./generate_routes.sh <net>.net.xml <realflows>.xml" 
    exit
fi

SUMO_HOME="/home/veins/src/sumo-1.11.0/"

NET="$1"
DETECTORS="$2"

BEGIN=0
END=3600
AG_TIME=3600

SCRIPT_PATH="${SUMO_HOME}tools/assign/"
JAR_PATH="${SUMO_HOME}bin/"

echo "Generating random trips using randomTrips.py"
python "${SUMO_HOME}tools/randomTrips.py" -n $NET --min-distance 500 --fringe-factor 25 -p 0.8 --lanes --speed-exponent 2 --trip-attributes="departLane=\"best\" departPos=\"last\"" -b $BEGIN -e $END --validate

echo "Generating routes using duaIterate.py"
python "${SCRIPT_PATH}duaIterate.py" -n $NET -t trips.trips.xml -a $AG_TIME -b $BEGIN -e $END --vehroute-file detailed

echo "Adjusting routes previously created using cadtys"
python "${SCRIPT_PATH}cadytsIterate.py" -n $NET -r 49/trips_049.rou.alt.xml -d $DETECTORS -a $AG_TIME -c "${JAR_PATH}utilities-MASTER-SNAPSHOT.jar:${JAR_PATH}cadyts-MASTER-SNAPSHOT.jar" --overridett --freezeit 18 --mincountstddev 8 -b $BEGIN -e $END
