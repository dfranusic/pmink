#!/bin/bash

# declare pmink assoc array
declare -A PMINK_VARS
# get command line args
commandline_args=("$@")

# main init method
function pmink_init(){

	OIFS=$IFS
	for var in "${commandline_args[@]}"; do
    		IFS='='
    		arr=(${var[*]})
    		VAL="${arr[*]:1}"
    		PMINK_VARS["${arr[0]}"]=$VAL
	done

	IFS=$OIFS

}

# init
pmink_init
