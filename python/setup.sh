#! /bin/bash

sudo apt update && sudo apt -y upgrade

#sudo apt install -y subversion

sudo mkdir /opt/starflit

sudo chown ${USER} /opt/starflit 

cd /opt/starflit

pwd

#svn export https://github.com/lbo462/starflit/trunk/python 

git clone https://github.com/lbo462/starflit.git

cd starflit

cd python

virtualenv --system-site-packages venv

source venv/bin/activate

pip install -r requirements.txt

mv default.env .env

python3 main.py --model mobilenet_v2.tflite --label labels.txt --obj "stop sign"
