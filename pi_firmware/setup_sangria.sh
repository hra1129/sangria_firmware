#!/bin/bash

apt-get update
apt-get upgrade

# Setup Japanese FEP
apt-get install uim-fep uim-anthy fonts-noto-mono fbterm

# Setup Editor and File Manager
apt-get install ne fdclone

# Setup SAMBA
apt-get install samba samba-common-bin
mkdir /share
chmod 777 /share
