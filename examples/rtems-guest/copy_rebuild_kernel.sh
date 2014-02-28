cp $1 ./generated-code/cpu/part1/part1.elf
cd ./generated-code/cpu/
make `pwd`/pok.elf
cd -
