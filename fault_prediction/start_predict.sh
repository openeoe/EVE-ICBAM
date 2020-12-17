#!/bin/sh

BINDIR=/usr/local/MOA/bin
READDIR=/home/smarteco/DATA/forecast

today=`date +'%Y%m%d'`

echo $today

cd $READDIR

/usr/bin/mkdir $today
/usr/bin/mkdir $today/in
/usr/bin/mkdir $today/out
/usr/bin/mkdir $today/mem
/usr/bin/mkdir $today/swap
/usr/bin/mkdir $today/fs
/usr/bin/mkdir $today/temp

cd $BINDIR

./mktraindat -c mktrain.conf -a in
./mktraindat -c mktrain.conf -a out
./mktraindat -c mktrain.conf -a mem
./mktraindat -c mktrain.conf -a swap
./mktraindat -c mktrain.conf -a fs
./mktraindat -c mktrain.conf -a temp

cd $READDIR
cd $today
cd mem

for i in *.dat;
do
	echo $today $i
	/usr/bin/python -W ignore /usr/local/MOA/bin/forecast_mem.py $today $i > $i.for
done

for i in *.for;
do
	echo $today $i
	/usr/local/MOA/bin/insforedat -c /usr/local/MOA/bin/insfore.conf -f $i -d $today -a mem
done

cd ../swap

for i in *.dat;
do
	echo $today $i
	/usr/bin/python -W ignore /usr/local/MOA/bin/forecast_swap.py $today $i > $i.for
done

for i in *.for;
do
	echo $today $i
	/usr/local/MOA/bin/insforedat -c /usr/local/MOA/bin/insfore.conf -f $i -d $today -a swap
done

cd ../fs

for i in *.dat;
do
	echo $today $i
	python -W ignore /usr/local/MOA/bin/forecast_fs.py $today $i > $i.for
done

for i in *.for;
do
	echo $today $i
	/usr/local/MOA/bin/insforedat -c /usr/local/MOA/bin/insfore.conf -f $i -d $today -a fs
done

cd ../temp

for i in *.dat;
do
	echo $today $i
	/usr/bin/python -W ignore /usr/local/MOA/bin/forecast_temp.py $today $i > $i.for
done

for i in *.for;
do
	echo $today $i
	/usr/local/MOA/bin/insforedat -c /usr/local/MOA/bin/insfore.conf -f $i -d $today -a temp
done

cd ../in

for i in *.dat;
do
	echo $today $i
	/usr/bin/python -W ignore /usr/local/MOA/bin/forecast_trin.py $today $i > $i.for
done

for i in *.for;
do
	echo $today $i
	/usr/local/MOA/bin/insforedat -c /usr/local/MOA/bin/insfore.conf -f $i -d $today -a in
done

cd ../out

for i in *.dat;
do
	echo $today $i
	/usr/bin/python -W ignore /usr/local/MOA/bin/forecast_trout.py $today $i > $i.for
done

for i in *.for;
do
	echo $today $i
	/usr/local/MOA/bin/insforedat -c /usr/local/MOA/bin/insfore.conf -f $i -d $today -a out
done

