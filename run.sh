#!/bin/bash

make clean && make && while ! sudo ./launch.sh; do echo "prøver igjen"; done
