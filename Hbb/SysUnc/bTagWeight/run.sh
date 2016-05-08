zero=0
one=1
nine=8
now1=0

while [ $now1 -le $one ]
do
	now2=0
	while [ $now2 -le $one ]
	do
		now3=0
		while [ $now3 -le $nine ]
		do
			./jecr $now1 $now3 $now2
			now3=$(( $now3 + 1 ))
		done
		now2=$(( $now2 + 1 ))
	done
	now1=$(( $now1 + 1 ))
done
