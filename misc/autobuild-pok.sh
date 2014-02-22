#!/bin/sh

#Script to build automatically POK
#The purpose of this script is to make nightly build

BUILDDIR=/tmp/autobuild-pok
VERSION=`date +%Y%m%d`
rm -rf ${BUILDDIR}
mkdir ${BUILDDIR}

cd ${BUILDDIR}
svn co svn co svn://svn.tuxfamily.org/svnroot/pok/pok . >/dev/null 2>&1

make release

if test $? != 0 ; then
   echo "Could not make a release"
   echo "Go on failblog.org to get a nice picture of failure"
   exit 1
fi


tar zxf pok-$VERSION.tgz

cd pok-$VERSION
make tests

if test $? != 0 ; then
   echo "Could not build POK tests"
   exit 1
fi

cd ..
scp pok-$VERSION.tgz ssh.tuxfamily.org:/home/pok/pok-repository/snapshots/

exit 0
