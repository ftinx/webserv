#!/bin/bash

flag=1
while [ ${flag} -eq 1 ]
do
        clear
        echo -e "\033[47;30m* WELCOME TO FTINX :) *\033[0m"
        echo ""
        I=0
        for config_file_list in `ls config`
        do
                let I=I+1
                echo "$I) $config_file_list"
        done
        echo -e "\033[40;33mIf you do not enter any number, the default server runs\033[0m"
        echo ""
        echo -n "SELECT CONFIG FILE : "
        if [[ $# == 1 ]];then
                selected_file_num="$1"
        else
                read selected_file_num
        fi
        # 인자가 입력된 경우 config 파일 숫자를 대체

        for_check=${selected_file_num//[1-"$I"]/}
        if [[ $for_check ]];then
                flag=1
        else
                flag=0
        fi
        # 인자가 범위를 벗어나면 루프에 빠짐
done

if [[ $selected_file_num == "" ]];then
        file_name="default.conf"
else
        file_name=`ls config | sed -n "$selected_file_num"p`
fi
# 숫자를 입력하지 않는 경우 default.conf 파일 입력

clear
echo -e "\033[47;30m* DELETE LOG *\033[0m"
echo ""

find . -type f -name "*.log" -exec rm {} \;
find . -type f -name "console_log" -exec rm {} \;
# 현재 폴더 아래의 log파일만 찾아서 삭제하는 명령어입니다. 수정 시 주의 요망!

echo -e "\033[47;30m* COMPILING... *\033[0m"
echo ""

make >& make.log
# make 하며 내용을 make.log에 저장
if [[ -z `grep "error" make.log` ]];then
        echo -e "\033[47;30m* COMPILE SUCCESS *\033[0m"
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
