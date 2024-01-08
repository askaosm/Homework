#!/bin/bash
echo 'Working directory'
read dirname #directory 이름을 입력을 저장

if [ -n "$dirname" ] #문자열의 길이가 1이상일때 True 
then
   cd $dirname
   if [ $? -ne 0 ]
   then
     echo "Error!" 
     exit 0
   fi
fi

for dir in *
do

   newname=`echo $dir | tr "[a-z] [A-Z]" "[A-Z] [a-z]"` #변수 newname은 dir의  대소문자를tr을 이용해서 바꾼것
   mv $dir $newname #$dir을 newname으로 바꾼다. 
done 

