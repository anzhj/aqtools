#! /bin/bash

export LD_LIBRARY_PATH=.

PROGRAM=./authserver

#while true ; do
#	sleep 10
	
#	PRO_NOW=`ps aux | grep $PROGRAM | grep -v grep | wc -l`
#	if [ $PRO_NOW -lt 1 ]; then
#		$PROGRAM &
#		date >> ./info.log
#		echo "program start" >> ./info.log
#	fi

#	PRO_STAT=`ps aux | grep $PROGRAM | grep T | grep -v grep | wc -l`
#	if [ $PRO_STAT -gt 0 ]; then
#		killall -9 $PROGRAM
#		sleep 2
#		$PROGRAM &
#		date >> ./info.log
#		echo "program start" >> ./info.log
#	fi
#done

echo "Starting authserver:"

$PROGRAM &

exit 0
