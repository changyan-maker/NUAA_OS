if test 1 -lt 2
then
    echo true
else
    echo false
fi

i=0
while test $i -lt 3
do
    echo $i
    i=`expr $i + 1`
done
