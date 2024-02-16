#!/bin/bash

sudo apt-get install libserial-dev nodejs npm

cd Scripts
make
cd ..

mv $(pwd) ~/Desktop/

cd ~/Desktop/hydroponisk_odling/Server/Host
npm update express
cd ../..

cp $(pwd)/OBS_Settings/Untitled.json ~/.config/obs-studio/basic/scenes/Untitled.json

mkdir ~/Desktop/Data
