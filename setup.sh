#!/bin/bash

cd $(pwd)/Server/Host
npm update express
cd ../..

cp $(pwd)/OBS_Settings/Untitled.json ~/.config/obs-studio/basic/scenes/Untitled.json
