#!/bin/bash

(./Bash/fetch.sh) &
(./Bash/server.sh) &
obs --startstreaming