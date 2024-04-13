make clean
make
./test < algor.algo > test.asm
asipro test.asm test
sipro test

