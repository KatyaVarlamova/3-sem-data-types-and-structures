args=$(cat pos_args.txt)
if [[ $1 < 10 ]]
then 
{ 
../app.exe $args <pos_0$(( $1 ))_in.txt >out.txt
}
else
{
../app.exe $args <pos_$(( $1 ))_in.txt >out.txt
}
fi

if [[ $? > 0 ]]
then 
{ 
echo -e "\x1b[32mERROR\x1b[0m"
}
else
{
echo -e "\x1b[32mSUCCESS\x1b[0m"
}
fi

if [[ $1 < 10 ]]
then 
{ 
diff out.txt pos_0$(( $1 ))_out.txt
}
else
{
diff out.txt pos_$(( $1 ))_out.txt
}
fi
rm out.txt