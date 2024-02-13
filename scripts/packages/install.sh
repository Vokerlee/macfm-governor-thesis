#!/bin/sh

script_path=$(dirname $(realpath $0))
. ${script_path}/common.sh

if [ -f /etc/os-release ]; then
    # freedesktop.org and systemd
    . /etc/os-release
    OS=$NAME
    VER=$VERSION_ID
    OS_ID=$ID
else
    echo_red "Can't detect OS distribution type."
    echo_red "Check REQUIREMENTS.md file and install all packages manually."
    exit 1
fi

# echo -e $GREEN  $DEFAULT
echo_green "Current OS: ${OS}"
echo_green "Version: ${VER}\n"

if [ $OS_ID == "arch" ]; then
    installation_script="${script_path}/distr/arch.sh"
elif [ $OS_ID == "ubuntu" ]; then
    installation_script="${script_path}/distr/ubuntu.sh"
else
    echo_red "Only \"Arch Linux\" and \"Ubuntu\" distributions are supported."
    echo_red "Check REQUIREMENTS.md file and install all packages manually."
    exit 1
fi

echo "Start installation..."

$(echo $SHELL) ${installation_script}

echo_green "Success!"
