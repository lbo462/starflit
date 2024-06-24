#! /bin/bash

# Updates the OS
sudo apt update && sudo apt -y upgrade

# Creates the directory where the repo will get cloned
sudo mkdir /opt/starflit

# Changes ownership of the starflit directory
sudo chown ${USER} /opt/starflit 

# Clones the starflit repository
git clone https://github.com/lbo462/starflit.git /opt/starflit

# Creates a virtual env
virtualenv --system-site-packages /opt/starflit/starflit/python/venv

# Activates the virtual env
source /opt/starflit/starflit/python/venv/bin/activate

# Installs the requirements
pip install -r /opt/starflit/starflit/python/requirements.txt

# Creates the .env file based on the default.env file provided in the repository
mv /opt/starflit/starflit/python/default.env /opt/starflit/starflit.python/.env

# Deactivates the virtual env
deactivate
