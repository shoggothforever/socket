#common="gnome-terminal -- ./server $2"
#eval $common
pos="build/bin"
sh echoser.sh $2 &
a=1
while (($a<=3))
do
	sh echocli.sh $1 $2
	let "a++"
done
