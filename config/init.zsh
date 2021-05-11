#!/bin/bash
# Similar to rest_yaml function

FILE=tmp*.yml

if test -f "$FILE"; then
  rm tmp*.yml;
fi


cp init_cocktail.yml tmp_cocktail.yml;
cp init_stock.yml tmp_stock.yml;

