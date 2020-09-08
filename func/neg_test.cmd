if [[ $1 < 10 ]]
then 
{ 
../app.exe <neg_0$(( $1 ))_in.txt >out.txt
}
else
{
../app.exe <neg_$(( $1 ))_in.txt >out.txt
}
fi

if [[ $? > 0 ]]
then 
{ 
echo success
}
else
{
echo error
}
fi

if [[ $1 < 10 ]]
then 
{ 
diff out.txt neg_0$(( $1 ))_out.txt
}
else
{
diff out.txt neg_$(( $1 ))_out.txt
}
fi
rm out.txt