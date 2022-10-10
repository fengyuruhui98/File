#!/bin/sh
city=suzhou
cd /hhjt/reader
export LD_LIBRARY_PATH=/hhjt/reader:$LD_LIBRARY_PATH
if [ -f ./progbak/$city ]
then
	echo "\nprogram error! now back to the old version..."
	mv ./progbak/$city .
else
	if [ -f ./prognew/update ]
	then
		echo "now update the reader application..."
		rm ./prognew/update
		cp $city ./progbak/.
		mv ./prognew/$city .
	fi
fi
echo "run the reader application...."
chmod 700 * -R
./$city /dev/ttyAMA1 /dev/ttyAMA2 