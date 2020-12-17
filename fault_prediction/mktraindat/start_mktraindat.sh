#!/bin/sh

./mktraindat -c mktrain.conf -a in
./mktraindat -c mktrain.conf -a out
./mktraindat -c mktrain.conf -a mem
./mktraindat -c mktrain.conf -a swap
./mktraindat -c mktrain.conf -a fs
./mktraindat -c mktrain.conf -a temp
