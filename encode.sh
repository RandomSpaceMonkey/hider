#!/bin/bash

cp linux.png linux.copy.png
gcc hider.c -o hider && ./hider encode linux.copy.png