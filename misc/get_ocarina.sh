#! /bin/sh -e
## get_ocarina.sh
##
## Made by Laurent Lec
## Login   <laurent@lse.epita.fr>
##
## Started on  Mon Jun  8 10:48:11 2009 Laurent Lec
## Last update Mon Jun  8 11:38:20 2009 Laurent Lec
##


if [ $# -ne 1 -o "$1" = "-h" -o "$1" = "--help" ];
then
    echo "Usage: $0 [-h|--help] prefix" >&2
    exit 1
fi

BASE="http://download.tuxfamily.org/taste/snapshots/binaries/ocarina/"
FILE=`wget -qO - $BASE | grep "a href=.*-current" | sed "s/^.*href=\"\([^\"]*\)\".*$/\1/"`
PREFIX=$1
FOLDER="ocarina"

CUR=`pwd`
TMP=`mktemp -d`
cd $TMP || exit 1

echo -e "\e[1;34m+ \e[1;37mPrefix is '$PREFIX'\e[0;37m"

echo -e "\e[1;31m+ \e[1;37mFetching Ocarina...\e[0;37m"

wget $BASE/$FILE || exit 1

echo -e "\e[1;31m+ \e[1;37mInstalling Ocarina...\e[0;37m"

tar xzf $FILE || exit 1
cd $FOLDER || exit 1
mkdir -p $PREFIX || exit 1
find . -type d -exec mkdir -p $PREFIX/{} \; || exit 1
find . -type f -exec cp -f {} $PREFIX/{} \; || exit 1

cd $CUR || exit 1
rm -rf $TMP || exit 1

echo -e "\e[1;34m+ \e[1;37mSuccess!\e[0;37m"
