#!/bin/bash

clear
echo -e "\033[47;30m* FTINX CONFIG GENERATOR :) *\033[0m"
echo ""

echo -e "\033[40;33mIf you do not enter, set to '1234'\033[0m"
echo -n "ENTER THE PASSWORD FOR BASIC AUTH : "
read password

if [[ $password == "" ]];then
        password="1234"
fi

cat config/.ftinx_sample.conf | sed -e 's=PWD='"$PWD"'=g' -e 's=AUTHID='"$USER"'=g' > config/"$USER".conf

clear
echo -e "\033[47;30m* AUTH FILE HAS BEEN CREATED SUCCESSFULLY *\033[0m"
echo 'id : '$USER '/ pw : '$password
echo -n $USER':'$password | base64 > .auth/.htpasswd_"$USER"

echo ""
echo -e "\033[47;30m* CONFIG FILE HAS BEEN CREATED SUCCESSFULLY *\033[0m"
echo 'path -> config/'"$USER"'.conf'

echo ""
echo -e "\033[47;30m* SUCCESS! *\033[0m"
