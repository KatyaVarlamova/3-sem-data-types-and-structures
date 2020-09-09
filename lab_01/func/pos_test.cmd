if [[ $1 < 10 ]]
then 
{ 
../app.exe <pos_0$(( $1 ))_in.txt >out.txt
}
else
{
../app.exe <pos_$(( $1 ))_in.txt >out.txt
}
fi

if [[ $? > 0 ]]
then 
{ 
echo error
}
else
{
echo success
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