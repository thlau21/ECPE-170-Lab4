#!/bin/bash
declare -a Sigma=(0.3 0.4 0.5 0.6 0.7 0.8 0.9 1.0 1.1)

for((i=0;i<${#Sigma[@]};i++))
do
	echo "./amplify IMAGES/Lenna_org_1024.pgm 11 ${Sigma[i]} 2"
	./amplify IMAGES/Lenna_org_1024.pgm 11 ${Sigma[i]} 2
done
