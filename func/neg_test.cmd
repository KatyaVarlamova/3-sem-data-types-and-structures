../app.exe <neg_0$(( $1 ))_in.txt >out.txt
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
diff out.txt neg_0$(( $1 ))_out.txt
rm out.txt