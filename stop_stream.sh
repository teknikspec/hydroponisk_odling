#!/bin/bash

pkill obs
pkill -f fetch.sh && pkill -f fetch_data
pkill -f server.sh && pkill node