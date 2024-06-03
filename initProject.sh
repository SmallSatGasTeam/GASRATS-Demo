# This file is used to simplify initializing the FSW repo for the first time
# Can be run using any Linux based command prompt using "bash initProject.sh"

# Initializes the fprime submodule
git submodule init
git submodule update

# Creates a python virtual environment in the directory the repo was cloned in
python3 -m venv fprime-venv
. fprime-venv/bin/activate

# Installs all requirements necessary and then runs preliminary generate and build on the project
pip install -r fprime/requirements.txt
fprime-util generate
fprime-util build