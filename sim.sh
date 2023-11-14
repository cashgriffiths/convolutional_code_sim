START=$1
END=$2
N=$3
G1=$4
G2=$5

INC=1
if (($START > $END))
then
    INC=-1
fi

for SNR in $(seq $START $INC $END)
do
    echo "SNR: $SNR dB"
    ./a.out $SNR $N $G1 $G2 | grep ^[bT] | cat
    echo
done
