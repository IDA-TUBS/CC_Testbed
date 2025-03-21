#!/bin/bash

echo "Start Donkey Car"
#python3 -m venv env --system-site-packages
#source ~/.bashrc
source ~/env/bin/activate
python /home/truck_pi/RoamingDemonstrator/TruckControl/mycar/manage.py drive --js
read -n 1 -s
