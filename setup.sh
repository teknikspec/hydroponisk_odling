#!/bin/bash

mv $(pwd) ~/Desktop/

cd ~/Desktop/hydroponisk_odling/Server/Host
npm update express
cd ../..

cp $(pwd)/OBS_Settings/Untitled.json ~/.config/obs-studio/basic/scenes/Untitled.json
