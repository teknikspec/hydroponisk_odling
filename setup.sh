#!/bin/bash

sudo apt-get install libserial-dev

cd Scripts
make
cd ..

mv $(pwd) ~/Desktop/

cd ~/Desktop/hydroponisk_odling/Server/Host
npm update express
cd ../..

cp $(pwd)/OBS_Settings/Untitled.json ~/.config/obs-studio/basic/scenes/Untitled.json
