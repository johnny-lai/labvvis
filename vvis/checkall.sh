#!/bin/bash
APP_PATH="build"

# Check arguments
if [ ! $1 ]; then
	echo "`basename $0` <mode>"
	exit 1
fi

for n in image functor
do
	check $APP_PATH/$n test/empty.tin test/expected/$1/$n.expected
done
