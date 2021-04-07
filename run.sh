#!/bin/bash

clear
echo ""
echo -e "\033[47;30m* WELCOME TO FTINX 0.1 :) *\033[0m"
echo ""

echo "CONFIG FILE LIST"
for config_file_list in `ls config $1`
do
        let I=I+1
        echo "$I) $config_file_list"
done
echo -n "Select config file : "
read selected_file_num
file_name=`ls config $1 | sed -n "$selected_file_num"p`

clear
echo ""
echo -e "\033[47;30m* DELETE LOG FILES *\033[0m"
echo ""

find . -type f -name "*.log" -exec rm {} \;
# 현재 폴더 아래의 log파일만 찾아서 삭제하는 명령어입니다. 수정 시 주의 요망!

echo ""
echo -e "\033[47;30m* COMPILE... *\033[0m"
echo ""

make >& make.log
# grep -r 'error' make_log.txt
if [[ -z `grep "error" make.log` ]]
        then echo "\033[47;30m* COMPILE SUCCESS *\033[0m"
        else {
                echo -e "\033[41;30m* COMPILE ERROR *\033[0m"
                echo -e "\033[41;30m* Check the error details (make.log) *\033[0m"
                exit 1
        }
fi
clear

echo ""
echo -e "\033[47;30m* RUN SERVER *\033[0m"
echo ""

./Webserv config/${file_name}
