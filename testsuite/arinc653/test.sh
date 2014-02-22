#!/bin/sh

testname=$1

if [ "$testname" = "" ]; then
   echo "Bad test name, use $0 test-name"
   exit 1
fi

if [ ! -d $testname ]; then
   echo "Test $testname does not exist"
   exit 2
fi

if [ ! -f $testname/desc ]; then
   echo "Test $testname does not have a valid description"
   exit 3
fi

if [ "$TMPDIR" = "" ]; then
   TMPDIR="/tmp/"
fi

TESTDIR=$TMPDIR/pok-testsuite
rm -rf $TESTDIR

mkdir $TESTDIR

model=`grep model $testname/desc|awk -F: '{print $2}'`
do_run=`grep run $testname/desc|awk -F: '{print $2}'`

cp -f common/$model  $TESTDIR
cp -f $testname/*.c  $TESTDIR
cp -f common/*.h    $TESTDIR

TOOLCHAIN_ARGS=""

if [ "$do_run" != "yes" ]; then
   TOOLCHAIN_ARGS="--no-run"
fi

( cd $TESTDIR && $POK_PATH/misc/pok-toolchain.pl --models=$model $TOOLCHAIN_ARGS)

