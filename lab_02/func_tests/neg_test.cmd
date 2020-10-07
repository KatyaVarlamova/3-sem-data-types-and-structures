if [[ $1 < 10 ]]
then 
{ 
args=$(cat neg_0$(( $1 ))_args.txt)
../app.exe $args <neg_0$(( $1 ))_in.txt >out.txt
}
else
{
args=$(cat neg_$(( $1 ))_args.txt)
../app.exe $args <neg_$(( $1 ))_in.txt >out.txt
}
fi

if [[ $? > 0 ]]
then 
{ 
echo -e "\x1b[32mSUCCESS\x1b[0m"
}
else
{
echo -e "\x1b[32mERROR\x1b[0m"
}
fi
rm out.txt