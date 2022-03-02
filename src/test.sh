#!/bin/bash
make clean
make mysh

if [[ $1 == 1 ]] 
then
    echo "set test **************************"
    echo " "
    ./mysh < ../A1-testcases_public/set.txt
    echo " "
    echo "oneline test **************************"
    echo " "
    ./mysh < ../A1-testcases_public/oneline.txt
    echo " "
    echo "ls test **************************"
    echo " "
    ./mysh < ../A1-testcases_public/ls.txt
    echo " "
    echo "echo test **************************"
    echo " "
    ./mysh < ../A1-testcases_public/echo.txt
    echo " "
    echo "batch test **************************"
    ./mysh < ../A1-testcases_public/batch.txt
fi

if [[ $1 == 2 ]] 
then
    echo "set test **************************"
    echo " "
    printf 'run ../A1-testcases_public/set.txt' | ./mysh
    echo " "
    echo "oneline test **************************"
    echo " "
    printf 'run ../A1-testcases_public/oneline.txt' | ./mysh
    echo " "
    echo "ls test **************************"
    echo " "
    printf 'run ../A1-testcases_public/ls.txt' | ./mysh
    echo " "
    echo "echo test **************************"
    echo " "
    printf 'run ../A1-testcases_public/echo.txt' | ./mysh
    echo " "
    echo "batch test **************************"
    printf 'run ../A1-testcases_public/bash.txt' | ./mysh
fi

if [[ $1 == 3 ]] 
then
    echo "set test **************************"
    echo " "
    ./mysh < ../A2-testcases_public/AGING
    echo " "
    echo "oneline test **************************"
    echo " "
    ./mysh < ../A2-testcases_public/FCFS
    echo " "
    echo "ls test **************************"
    echo " "
    ./mysh < ../A2-testcases_public/FileNameTest
    echo " "
    echo "echo test **************************"
    echo " "
    ./mysh < ../A2-testcases_public/RR
    echo " "
    echo "batch test **************************"
    ./mysh < ../A2-testcases_public/SJF
fi

