#!/bin/sh
#city=shanghai
cd /hhjt/reader/
export LD_LIBRARY_PATH=/hhjt/reader:$LD_LIBRARY_PATH
if [ -f ./progbak/maglev ]
then
	echo "\nprogram error! now back to the old version..."
	mv ./progbak/maglev .
else
	if [ -f ./prognew/update ]
	then
		echo "now update the reader application..."
		rm ./prognew/update
		cp maglev ./progbak/.
		mv ./prognew/maglev .
	fi
fi
echo "run the reader application...."
chmod 700 * -R
./maglev /dev/ttyAMA1 /dev/ttyAMA2 /dev/ttyAMA2
