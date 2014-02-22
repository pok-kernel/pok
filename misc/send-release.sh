#!/bin/sh

VERSION=`date '+%Y%m%d'` 
HOST=ssh.tuxfamily.org
REMOTE_DIR=/home/pok/pok-repository/snapshots/
SCP_LOGIN=${TUXLOGIN:-$(whoami)}
echo "Sending pok-$VERSION to $SCP_LOGIN@$HOST:$REMOTE_DIR/"
scp pok-$VERSION.tgz $SCP_LOGIN@$HOST:$REMOTE_DIR

echo "Sending pok-$VERSION to $SCP_LOGIN@$HOST:$REMOTE_DIR/pok-current.tgz"
scp pok-$VERSION.tgz $SCP_LOGIN@$HOST:$REMOTE_DIR/pok-current.tgz

echo "Sending userguide to $SCP_LOGIN@$HOST:$REMOTE_DIR/pok-userguide.pdf"
scp pok-$VERSION.tgz $SCP_LOGIN@$HOST:$REMOTE_DIR/pok-current.tgz
scp doc/userguide/pok-user.pdf $SCP_LOGIN@$HOST:$REMOTE_DIR/pok-userguide-$VERSION.pdf
scp doc/userguide/pok-user.pdf $SCP_LOGIN@$HOST:$REMOTE_DIR/pok-userguide-current.pdf
scp doc/kernel-refman.pdf $SCP_LOGIN@$HOST:$REMOTE_DIR/pok-kernel-refman-$VERSION.pdf
scp doc/kernel-refman.pdf $SCP_LOGIN@$HOST:$REMOTE_DIR/pok-kernel-refman-current.pdf
scp doc/libpok-refman.pdf $SCP_LOGIN@$HOST:$REMOTE_DIR/pok-libpok-refman-$VERSION.pdf
scp doc/libpok-refman.pdf $SCP_LOGIN@$HOST:$REMOTE_DIR/pok-libpok-refman-current.pdf
scp -r doc/userguide/html $SCP_LOGIN@$HOST:$REMOTE_DIR/pok-userguide-current-html
scp -r doc/doxygen-kernel/html $SCP_LOGIN@$HOST:$REMOTE_DIR/kernel-refman-current-html
scp -r doc/doxygen-libpok/html $SCP_LOGIN@$HOST:$REMOTE_DIR/libpok-refman-current-html


