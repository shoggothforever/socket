
#gcc ser.c -o server 
common="cd build/bin && ./echoser $1 &"
eval $common

