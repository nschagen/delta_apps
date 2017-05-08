#!/bin/bash

if [ $# -lt 2 ]; then
  echo "Provide app directory and input directory"
  exit 1
fi

app_dir=$(readlink -f $1)
input=$(readlink -f $2)

cd $app_dir

if [ ! -f "serverctl" ]; then
  echo "no serverctl found"
  exit 1
fi

if [ -n "$V" ]; then
  exec 3>&1
else
  exec 3>&1 &>/dev/null
fi

export DELTA_DIR=/tmp/delta_out_dir
mkdir -p $DELTA_DIR

# We don't use this now, but session_setup.py requires it
taint_file=/tmp/taint
echo "0" > $taint_file

test_input_on_server() {
    input_name=$(basename $1)

    echo -n "$input_name" >&3

    # Run server
    sudo DELTA_DIR=$DELTA_DIR RUN_DELTA=1 ./serverctl start &
    sleep 1

    # Deliver payload. When this hangs for over 10sec, we probably didn't write
    # enough data. Register as 'timeout'
    timeout 12s $app_dir/session_setup.py $1 $taint_file

    if [ $? -eq 124 ]; then
        echo -e "\ttimeout" >&3
        ./serverctl stop
        continue
    fi

    # Give chance to server to do the merge attempts
    sleep 2

    LOGFILES=${DELTA_DIR}/${BIN_NAME}*.log

    STATUS=0

    # egrep 'Delta execution aborted .* segmentation fault' $LOGFILES
    # egrep 'Delta execution aborted .*: timed out.' $LOGFILES
    # egrep '\(catching up\) write' $LOGFILES

    if egrep 'Delta execution aborted .* segmentation fault' $LOGFILES; then
        echo -ne "\tsegfault" >&3
        STATUS=1
    fi
    if egrep 'Delta execution aborted .*: timed out.' $LOGFILES; then
        echo -ne "\thanging_variant" >&3
        STATUS=1
    fi
    if egrep 'calls:\s*write\(.* similarity: same function' $LOGFILES; then
        # Different buffers are written
        echo -ne "\tdivergent_write" >&3
        STATUS=1
    fi
    if egrep '\(catching up\) write' $LOGFILES; then
        # write() is called by only one variant
        echo -ne "\tassymetric_write" >&3
        STATUS=1
    fi
    if egrep 'calls:\s*read\(.* similarity: same function' $LOGFILES; then
        echo -ne "\tdivergent_read" >&3
        STATUS=1
    fi
    if egrep 'merge: attempt failed' $LOGFILES; then
        echo -ne "\tfailed_merge" >&3
        STATUS=1
    fi
    if egrep 'succesfully merged in function' $LOGFILES; then
        echo -ne "\tsuccesful_merge" >&3
        STATUS=1
    fi

    if [ $STATUS -eq 0 ]; then
        echo -ne "\tunknown" >&3
    fi

    echo "" >&3

    ./serverctl stop
}

if [ -d ${input} ]; then
    echo "testing with input files from directory: $input"
    for input_file in "$input"/*
    do
        test_input_on_server ${input_file}
        rm -rf ${DELTA_DIR}
    done
else
    echo "testing with input file: $input"
    test_input_on_server ${input}
fi

rm $taint_file
