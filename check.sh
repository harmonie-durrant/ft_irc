# run ./ircserv 5674 test
# kill it with ctrl+c
# if exit code is 0, then it's good
# if exit code is not 0, return 1
# if still running, then kill it and return 1

./ircserv 5674 test &
PID=$!
sleep 1
kill -0 $PID
if [ $? -ne 0 ]; then
	exit 1
fi
kill $PID
wait $PID
if [ $? -ne 0 ]; then
	exit 1
fi
exit 0