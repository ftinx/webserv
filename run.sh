#!/bin/bash

clear
echo ""
echo -e "\033[47;30m* WELCOME TO FTINX 0.1 :) *\033[0m"
echo ""

for config_file_list in `ls config $1`
do
        let I=I+1
        echo "$I) $config_file_list"
done
echo ""
echo -n "SELECT CONFIG FILE : "
read selected_file_num
file_name=`ls config $1 | sed -n "$selected_file_num"p`
# if [[ x{$select_file_num} == x ]];then
#         echo "here"
#         file_name="default.conf"
# else
#         echo "here2"
#         file_name=`ls config $1 | sed -n "$selected_file_num"p`
# fi

clear
echo -e "\033[47;30m* DELETE LOG *\033[0m"
echo ""

find . -type f -name "*.log" -exec rm {} \;
# 현재 폴더 아래의 log파일만 찾아서 삭제하는 명령어입니다. 수정 시 주의 요망!

echo -e "\033[47;30m* COMPILING... *\033[0m"
echo ""

make >& make.log
# make 하며 내용을 make.log에 저장
if [[ -z `grep "error" make.log` ]];then
        echo "\033[47;30m* COMPILE SUCCESS *\033[0m"
else
        echo -e "\033[41;30m* COMPILE ERROR *\033[0m"
        echo -e "\033[41;30m* -----------cat make.log----------- *\033[0m"
        cat make.log
        echo -e "\033[41;30m* ---------------------------------- *\033[0m"
        exit 1
fi
# make.log 파일 내부에 error라는 문자열이 있으면 스크립트 중단

clear
echo -e "\033[47;30m* RUN SERVER *\033[0m"
echo ""

./Webserv config/${file_name}
