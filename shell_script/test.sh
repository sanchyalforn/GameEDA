#!/bin/bash

"Testing $1 times"

PLAYERNAME="Varo"
CONFIGFILE="default.cnf"
EXECUTABLE="./Game"
LOSECOLOR="\033[0;31m"
WINCOLOR="\033[0;32m"
NOCOLOR="\033[0m"

mkdir -p ./results/

for i in 1 2 3 .. $1
do
	SEED=$(( ( RANDOM % 400 )  + 1 ))
	echo "SEED: $SEED"
	echo -n "TL:"
	$EXECUTABLE $PLAYERNAME Dummy Dummy Dummy -s $SEED -i $CONFIGFILE -o ./results/$SEED-TL.res 2>&1 >/dev/null | grep "$PLAYERNAME got top score" >/dev/null
	rc=$?
	if [[ $rc == 0 ]]; then
		echo -e "$WINCOLOR won $NOCOLOR";
	else
		echo -e "$LOSECOLOR lost $NOCOLOR";
	fi

	echo -n "BL:"
	$EXECUTABLE Dummy $PLAYERNAME Dummy Dummy -s $SEED -i $CONFIGFILE  -o ./results/$SEED-BL.res 2>&1 >/dev/null | grep "$PLAYERNAME got top score" >/dev/null
	rc=$?
	if [[ $rc == 0 ]]; then
		echo -e "$WINCOLOR won $NOCOLOR";
	else
		echo -e "$LOSECOLOR lost $NOCOLOR";
	fi

	echo -n "BR:"
	$EXECUTABLE Dummy Dummy $PLAYERNAME Dummy -s $SEED -i $CONFIGFILE  -o ./results/$SEED-BR.res 2>&1 >/dev/null | grep "$PLAYERNAME got top score" >/dev/null
	rc=$?
	if [[ $rc == 0 ]]; then
		echo -e "$WINCOLOR won $NOCOLOR";
	else
		echo -e "$LOSECOLOR lost $NOCOLOR";
	fi

	echo -n "TR:"
	$EXECUTABLE Dummy Dummy Dummy $PLAYERNAME -s $SEED -i $CONFIGFILE -o ./results/$SEED-TR.res  2>&1 >/dev/null | grep "$PLAYERNAME got top score" >/dev/null
	rc=$?
	if [[ $rc == 0 ]]; then
		echo -e "$WINCOLOR won $NOCOLOR";
	else
		echo -e "$LOSECOLOR lost $NOCOLOR";
	fi
done
