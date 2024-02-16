#! /bin/bash

echo "please enter a commit message: "
read COMMIT_MSG
git add .
git commit -m "clox: ${COMMIT_MSG}"