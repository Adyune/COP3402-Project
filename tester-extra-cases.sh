#!/bin/bash

make
compiled=$?
if [[ $compiled != 0 ]]; then
	echo "does not compile"
	exit 1
fi

echo "Compiles"

echo -n "Basic Test : "
./a.out basic.txt -a -s > output.txt
executed=$?
if [[ $executed !=  0 ]]; then
	echo ":'("
	exit 1
else
	diff -w -B output.txt bout.txt &> /dev/null
	correct=$?
	if [[ $correct != 0 ]]; then
		echo ":'("
		exit 1
	else
		echo "───==≡≡ΣΣ((( つºل͜º)つ"
	fi
fi


echo -n "Error 1 : "

./a.out error1.txt > output.txt
executed=$?
if [[ $executed !=  0 ]]; then
	echo ":'("
else
	diff -w -B output.txt out1.txt &> /dev/null
	correct=$?
	if [[ $correct != 0 ]]; then
		echo ":'("
	else
		echo "───==≡≡ΣΣ((( つºل͜º)つ"
	fi
fi

echo -n "Error 2 : "

./a.out error2.txt > output.txt
executed=$?
if [[ $executed !=  0 ]]; then
	echo ":'("
else
	diff -w -B output.txt out2.txt &> /dev/null
	correct=$?
	if [[ $correct != 0 ]]; then
		echo ":'("
	else
		echo "───==≡≡ΣΣ((( つºل͜º)つ"
	fi
fi

echo -n "Error 3 : "

./a.out error3.txt > output.txt
executed=$?
if [[ $executed !=  0 ]]; then
	echo ":'("
else
	diff -w -B output.txt out3.txt &> /dev/null
	correct=$?
	if [[ $correct != 0 ]]; then
		echo ":'("
	else
		echo "───==≡≡ΣΣ((( つºل͜º)つ"
	fi
fi

echo -n "Error 4 : "

./a.out error4.txt > output.txt
executed=$?
if [[ $executed !=  0 ]]; then
	echo ":'("
else
	diff -w -B output.txt out4.txt &> /dev/null
	correct=$?
	if [[ $correct != 0 ]]; then
		echo ":'("
	else
		echo "───==≡≡ΣΣ((( つºل͜º)つ"
	fi
fi

echo -n "Error 5 : "

./a.out error5.txt > output.txt
executed=$?
if [[ $executed !=  0 ]]; then
	echo ":'("
else
	diff -w -B output.txt out5.txt &> /dev/null
	correct=$?
	if [[ $correct != 0 ]]; then
		echo ":'("
	else
		echo "───==≡≡ΣΣ((( つºل͜º)つ"
	fi
fi

echo -n "Error 6 : "

./a.out error6.txt > output.txt
executed=$?
if [[ $executed !=  0 ]]; then
	echo ":'("
else
	diff -w -B output.txt out6.txt &> /dev/null
	correct=$?
	if [[ $correct != 0 ]]; then
		echo ":'("
	else
		echo "───==≡≡ΣΣ((( つºل͜º)つ"
	fi
fi

echo -n "Error 7 : "

./a.out error7.txt > output.txt
executed=$?
if [[ $executed !=  0 ]]; then
	echo ":'("
else
	diff -w -B output.txt out7.txt &> /dev/null
	correct=$?
	if [[ $correct != 0 ]]; then
		echo ":'("
	else
		echo "───==≡≡ΣΣ((( つºل͜º)つ"
	fi
fi

echo -n "Error 8 : "

./a.out error8.txt > output.txt
executed=$?
if [[ $executed !=  0 ]]; then
	echo ":'("
else
	diff -w -B output.txt out8.txt &> /dev/null
	correct=$?
	if [[ $correct != 0 ]]; then
		echo ":'("
	else
		echo "───==≡≡ΣΣ((( つºل͜º)つ"
	fi
fi

echo -n "Error 9 : "

./a.out error9.txt > output.txt
executed=$?
if [[ $executed !=  0 ]]; then
	echo ":'("
else
	diff -w -B output.txt out9.txt &> /dev/null
	correct=$?
	if [[ $correct != 0 ]]; then
		echo ":'("
	else
		echo "───==≡≡ΣΣ((( つºل͜º)つ"
	fi
fi

echo -n "Error 10 : "

./a.out error10.txt > output.txt
executed=$?
if [[ $executed !=  0 ]]; then
	echo ":'("
else
	diff -w -B output.txt out10.txt &> /dev/null
	correct=$?
	if [[ $correct != 0 ]]; then
		echo ":'("
	else
		echo "───==≡≡ΣΣ((( つºل͜º)つ"
	fi
fi

echo -n "Error 12 : "

./a.out error12.txt > output.txt
executed=$?
if [[ $executed !=  0 ]]; then
	echo ":'("
else
	diff -w -B output.txt out12.txt &> /dev/null
	correct=$?
	if [[ $correct != 0 ]]; then
		echo ":'("
	else
		echo "───==≡≡ΣΣ((( つºل͜º)つ"
	fi
fi

echo -n "Error 13 : "

./a.out error13.txt > output.txt
executed=$?
if [[ $executed !=  0 ]]; then
	echo ":'("
else
	diff -w -B output.txt out13.txt &> /dev/null
	correct=$?
	if [[ $correct != 0 ]]; then
		echo ":'("
	else
		echo "───==≡≡ΣΣ((( つºل͜º)つ"
	fi
fi

echo -n "Error 14 : "

./a.out error14.txt > output.txt
executed=$?
if [[ $executed !=  0 ]]; then
	echo ":'("
else
	diff -w -B output.txt out14.txt &> /dev/null
	correct=$?
	if [[ $correct != 0 ]]; then
		echo ":'("
	else
		echo "───==≡≡ΣΣ((( つºل͜º)つ"
	fi
fi

echo -n "Error 15 : "

./a.out error15.txt > output.txt
executed=$?
if [[ $executed !=  0 ]]; then
	echo ":'("
else
	diff -w -B output.txt out15.txt &> /dev/null
	correct=$?
	if [[ $correct != 0 ]]; then
		echo ":'("
	else
		echo "───==≡≡ΣΣ((( つºل͜º)つ"
	fi
fi

echo -n "Error 17 : "

./a.out error17.txt > output.txt
executed=$?
if [[ $executed !=  0 ]]; then
	echo ":'("
else
	diff -w -B output.txt out17.txt &> /dev/null
	correct=$?
	if [[ $correct != 0 ]]; then
		echo ":'("
	else
		echo "───==≡≡ΣΣ((( つºل͜º)つ"
	fi
fi

echo -n "Error 18 : "

./a.out error18.txt > output.txt
executed=$?
if [[ $executed !=  0 ]]; then
	echo ":'("
else
	diff -w -B output.txt out18.txt &> /dev/null
	correct=$?
	if [[ $correct != 0 ]]; then
		echo ":'("
	else
		echo "───==≡≡ΣΣ((( つºل͜º)つ"
	fi
fi

echo -n "Error A : "

./a.out errorA.txt > output.txt
executed=$?
if [[ $executed !=  0 ]]; then
	echo ":'("
else
	diff -w -B output.txt outA.txt &> /dev/null
	correct=$?
	if [[ $correct != 0 ]]; then
		echo ":'("
	else
		echo "───==≡≡ΣΣ((( つºل͜º)つ"
	fi
fi

echo -n "Error B : "

./a.out errorB.txt > output.txt
executed=$?
if [[ $executed !=  0 ]]; then
	echo ":'("
else
	diff -w -B output.txt outB.txt &> /dev/null
	correct=$?
	if [[ $correct != 0 ]]; then
		echo ":'("
	else
		echo "───==≡≡ΣΣ((( つºل͜º)つ"
	fi
fi

echo -n "Error C : "

./a.out errorC.txt > output.txt
executed=$?
if [[ $executed !=  0 ]]; then
	echo ":'("
else
	diff -w -B output.txt outC.txt &> /dev/null
	correct=$?
	if [[ $correct != 0 ]]; then
		echo ":'("
	else
		echo "───==≡≡ΣΣ((( つºل͜º)つ"
	fi
fi


echo -n "Really Complex Tipping Test : "

./a.out tip.txt -a -s <<< '1 10 51 17 2 10 51 17 0' > output.txt
executed=$?
if [[ $executed !=  0 ]]; then
	echo ":'("
else
	diff -w -B output.txt tipout.txt &> /dev/null
	correct=$?
	if [[ $correct != 0 ]]; then
		echo ":'("
	else
		echo "───==≡≡ΣΣ((( つºل͜º)つ"
	fi
fi
