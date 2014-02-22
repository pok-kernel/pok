#!/bin/sh

POK_PATH=$HOME/wip/pok
DEBUG_FILE=/tmp/debugpok

#======================================
#Do not edit below this line
rm -f $DEBUG_FILE
echo "=============================================" >> $DEBUG_FILE
echo "config info" >> $DEBUG_FILE
echo "=============================================" >> $DEBUG_FILE
( uname -a ) >> $DEBUG_FILE 2>&1
( cat /etc/debian_version ) >> $DEBUG_FILE 2>&1
( cat /proc/cpuinfo ) >> $DEBUG_FILE 2>&1
( ls -l /bin/sh ) >> $DEBUG_FILE 2>&1
( make -v ) >> $DEBUG_FILE 2>&1
( gcc -v ) >> $DEBUG_FILE 2>&1
( which sh ) >> $DEBUG_FILE 2>&1



echo "=============================================" >> $DEBUG_FILE
echo "make configure" >> $DEBUG_FILE
echo "=============================================" >> $DEBUG_FILE

( cd $POK_PATH && make configure ) >> $DEBUG_FILE 2>&1

echo "=============================================" >> $DEBUG_FILE
echo "config.mk" >> $DEBUG_FILE
echo "=============================================" >> $DEBUG_FILE
( cat $POK_PATH/misc/mk/config.mk ) >> $DEBUG_FILE 2>&1

echo "=============================================" >> $DEBUG_FILE
echo "make partitions-threads" >> $DEBUG_FILE
echo "=============================================" >> $DEBUG_FILE
( cd $POK_PATH/examples/partitions-threads && make ) >> $DEBUG_FILE 2>&1

echo "=============================================" >> $DEBUG_FILE
echo "compile partitions-threads generated code" >> $DEBUG_FILE
echo "=============================================" >> $DEBUG_FILE
( cd $POK_PATH/examples/partitions-threads/generated-code/cpu && make ) >> $DEBUG_FILE 2>&1
