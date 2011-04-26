#!/bin/bash -x

if [ ${#} -eq 0 ]
then
	echo "Usage: $0 corpus2class"
	exit 1
fi

UPPER=`echo ${1} | tr '[:lower:]' '[:upper:]'`
LOWER=`echo ${1} | tr '[:upper:]' '[:lower:]'`
CLNAM=${1}
OUTFILE=libcorpus${LOWER}.i

cat swig_template_corpus2.template  | sed s/TEMPLATE/${UPPER}/g | sed s/Template/${CLNAM}/g | sed s/template/${LOWER}/g > ${OUTFILE}

echo -e "\e[1;32m${OUTFILE} was generated. Don't forget to add ${OUTFILE} to Makefile (or CMake)!\e[0m"