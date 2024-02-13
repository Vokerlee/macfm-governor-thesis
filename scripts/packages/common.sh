#!/bin/sh

GREEN='\033[0;32m'
RED='\033[0;31m'
DEFAULT='\033[0;39m'

echo_green () {
   echo -e ${GREEN}$@${DEFAULT}
}

echo_red () {
   echo -e ${RED}$@${DEFAULT}
}
