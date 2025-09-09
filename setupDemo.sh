#!/bin/bash
echo "[DEBUG] Running git submodule update ..."
git submodule update --init --recursive

echo "[DEBUG] Activating python venv"
# Creates a python virtual environment
python3 -m venv fprime-venv
. fprime-venv/bin/activate

echo "[DEBUG] Inside of venv installing pip"
python3 -m pip install --upgrade pip

ehco "[DEBUG] Installing requirements"
# Installs requirements
pip install -r fprime/requirements.txt

# Need to make sure custom framer protocol gets updated (not tracked in git)
echo "[DEBUG] Copying files from Custom Framer Protocol into fprime folder"
cp CustomFramerProtocol/EndurosatProtocol.* fprime/Svc/FramingProtocol
cp CustomFramerProtocol/cmakelist_file.txt fprime/Svc/FramingProtocol/CMakeLists.txt

# Generate and Build project.
echo "[DEBUG] Generating and Building the project"
fprime-util generate
fprime-util build

