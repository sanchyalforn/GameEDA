PLAYER="Maincra"
NOMATCH = 100

rm -rf test2.txt test3.txt
touch test2.txt
for k in {1..$NOMATCH}
do
    number=$RANDOM
    echo $number
    r=$(($RANDOM % 4))
    echo $r
    if [ $r = 0 ]
    then
        ./Game $PLAYER Dummy Dummy Dummy -s $number -i default.cnf -o default.out 2>> test2.txt
    elif [ $r = 1 ]
    then
        ./Game Dummy $PLAYER Dummy Dummy -s $number -i default.cnf -o default.out 2>> test2.txt
    elif [ $r = 2 ]
    then
        ./Game Dummy Dummy $PLAYER Dummy -s $number -i default.cnf -o default.out 2>> test2.txt
    elif [ $r = 3 ]
    then
        ./Game Dummy Dummy Dummy $PLAYER -s $number -i default.cnf -o default.out 2>> test2.txt
    else
        ./Game Dummy Dummy Dummy $PLAYER -s $number -i default.cnf -o default.out 2>> test2.txt
    fi
done

grep "player(s) $PLAYER got top score" test2.txt > test3.txt

wc -l test3.txt

