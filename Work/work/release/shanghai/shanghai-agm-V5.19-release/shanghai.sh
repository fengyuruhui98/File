export LD_LIBRARY_PATH=/home/shanghai:$LD_LIBRARY_PATH
if [ -f ./progbak/shanghai ]
then
	echo "\nprogram error! now back to the old version..."
	mv ./progbak/shanghai .
else
	if [ -f ./prognew/update ]
	then
		echo "now update the reader application..."
		rm ./prognew/update
		cp shanghai ./progbak/.
		mv ./prognew/shanghai .
	fi
fi
echo "run the reader application...."
chmod 700 * -R
./shanghai /dev/ttyS1
