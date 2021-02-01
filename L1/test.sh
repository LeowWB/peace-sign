for d in */; do
	echo "$d"
	gcc $d*.c
	for testin in $d*.in; do
		echo $testin
		./a.out < $testin > output.out
		diff ${testin%.*}.out output.out
	done
done
