#!/bin/bash

sudo apt install -y pipx
pipx ensurepath

pipx install meshtastic
meshtastic --info
