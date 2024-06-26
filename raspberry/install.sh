#! /bin/bash
#
#       ~ INSTALL SCRIPT FOR RPi ~
#
#   This is to be executed on a freshly flashed RPi.
#   It installs all the necessary component to control
#   the strandbeest to do some object recognition.
#
#   It also install the systemD so that the python
#   script that exhanges data with the Arduino gets
#   executed during the RPi boot.
#

export GIT_REPO_URL="https://github.com/lbo462/starflit.git"

# Updates the OS
sudo apt update && sudo apt -y upgrade

# Install git and python
sudo apt install -y git python3

# Creates the directory where the repo will get cloned
sudo mkdir /opt/starflit && sudo chown ${USER} /opt/starflit 

# Clones the starflit repository in the /opt/starflit
git clone -b 87-docker-and-systemd-service-for-rpi ${GIT_REPO_URL} /opt/starflit/

# Creates a virtual env for executing python code
python3 -m venv --system-site-packages /opt/starflit/venv

# Installs the requirements in the venv
/opt/starflit/venv/bin/pip install -r /opt/starflit/raspberry/python/requirements.txt

# Creates the .env file based on the default.env file provided in the repository
cp /opt/starflit/raspberry/python/default.env /opt/starflit/raspberry/python/.env

# Copy the service where it should land ...
sudo cp /opt/starflit/raspberry/starflit.service /etc/systemd/system/

# ... and enable it to run on boot.
sudo systemctl enable starflit.service

# And then start the service so that is runs directly
sudo systemctl start starflit.service