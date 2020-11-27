#!/bin/sh
git add .
#git reset -- update.sh
git commit -m "$1"
git push origin master
git status
