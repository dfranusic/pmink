#!/bin/bash
git log  --decorate --tags --no-walk > CHANGELOG 2>&1 || { echo "pMINK git repository not found!" > CHANGELOG; }
