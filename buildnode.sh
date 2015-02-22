#!/bin/bash
./autogen.sh
./configure --with-incompatible-bdb
make
make check
mkdir ~/.voyacoin
echo "rpcuser=foo" > ~/.voyacoin/voyacoin.conf
echo "rpcpassword=bar" >> ~/.voyacoin/voyacoin.conf
cd src
./voyacoind -daemon
./voyacoind -daemon -testnet
./voyacoin-cli getinfo
./voyacoin-cli -testnet getinfo
