#!/bin/sh

echo "================= longSubtraction:"
./longSubtraction 9837497341121330000348374 1000088883899997263
./longSubtraction 10000888838999972637236723671 0 
./longSubtraction 9837497341121330000348374 1
./longSubtraction 999999 999998
./longSubtraction 1000000000000000 98765
./longSubtraction 98374973411213300003483749837497341121330000348374 9837497341121330000348374983749734112133000034837
./longSubtraction 77 77
./longSubtraction 0 0
./longSubtraction 0983837173625122157642089528775373386064056365323188616731933959619021475868433756633160443496 983837173625122157642089528775373386064056365323188616731933959619021475868433756633160443400

echo "================= longSubtractionSigned:"
./longSubtractionSigned 9837497341121330000348374 10000888838999972637236723671
./longSubtractionSigned 0 10000888838999972637236723671
./longSubtractionSigned 9837497341121330000348374 1
./longSubtractionSigned 1 9999
./longSubtractionSigned 1000000000000000 98765
./longSubtractionSigned 98374973411213300003483749837497341121330000348374 100008888389999726372367236719837497341121330000348374
./longSubtractionSigned 77 77
./longSubtractionSigned 0 0
./longSubtractionSigned 09838371736251221576420895287753733860640563653231886167319339596190214758684337566331604434961364443876 09838371736251221576420895287753733860640563653231886167319339596190214758684337566331604434961364443876
