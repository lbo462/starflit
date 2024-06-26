#! /bin/bash
#
#       ~ UNINSTALL SCRIPT FOR RPi ~
#
#   This will uninstall all the components of
#   the starflit project from the RPi, as they
#   were installed via install.sh
#

# Stop and disable the starflit service
sudo systemctl stop starflit.service
sudo systemctl disable starflit.service

# Remove the starflit folder from /opt
rm -vfr /opt/starflit/*