#!/bin/bash

# include pmink utils
source ./src/cli/pmink_utils.sh

echo "PMINK_VARS..."
for i in "${!PMINK_VARS[@]}"
do
  echo "$i = ${PMINK_VARS[$i]}"
done

