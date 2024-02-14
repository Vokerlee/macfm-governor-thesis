#!/bin/sh

set -eou pipefail

script_path=$(dirname $(realpath $0))
target_tar="google-cloud-cli-464.0.0-linux-x86_64.tar.gz"
extract_folder="google-cloud-sdk"

cd /tmp
curl -O https://dl.google.com/dl/cloudsdk/channels/rapid/downloads/${target_tar}

rm -rf ${extract_folder}
tar -xf ${target_tar}
rm -rf ${target_tar}

sudo rm -rf /opt/${extract_folder}
sudo mv ${extract_folder} /opt/
cd /opt

sudo $(echo $SHELL) /opt/${extract_folder}/install.sh -q

echo "
if [ -f \"/opt/${extract_folder}/completion.bash.inc\" ]; then
    source /opt/${extract_folder}/completion.bash.inc
fi

if [ -f \"/opt/${extract_folder}/path.bash.inc\" ]; then
    source /opt/${extract_folder}/path.bash.inc
fi
" >> ~/.bashrc
