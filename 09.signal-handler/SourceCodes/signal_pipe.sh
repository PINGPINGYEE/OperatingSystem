#!/bin/bash

signal_type=$1
pid=$(pgrep -x handler)

if [ ! -z "$pid" ]; then
    if [ "$signal_type" == "SIGTERM" ]; then
        kill -SIGTERM $pid
        echo "SIGTERM sent to process ID $pid"
    elif [ "$signal_type" == "SIGALRM" ]; then
        seconds=$2
        sleep $seconds
        kill -SIGALRM $pid
        echo "SIGALRM sent to process ID $pid after $seconds seconds"
    fi
else
    echo "No process found"
fi

