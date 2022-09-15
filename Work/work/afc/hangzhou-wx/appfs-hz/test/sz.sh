cd /hhjt/reader
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
./rc531-beep 0 8000000 &
./rc531-beep 1 8000000 &
