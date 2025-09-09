# Docker Setup Instructions

Use these instructions unless you already have Linux or WSL.

1. Download [VS Code](https://code.visualstudio.com/download).
2. Download [Docker](https://docs.docker.com/get-started/get-docker/).
3. Download the [docker zip file](https://drive.google.com/file/d/1db0IgZ0olbIIAIe3irgGLI8xdNowJkdL/view?usp=sharing).
4. Unzip the docker folder.
5. Open the folder with the docker file in the terminal. If on windows, find the folder with the docker file, then right click and select *Open in Terminal*.
6. In the terminal type out and run this command: `docker build -t fprime-image .`
7. In the terminal type out and run this command: `docker create -it --name fprime-container fprime-image`.
8. Open up docker desktop.
9. Go to the container tab and start the container labeled _fprime-container_.
10. Open VS Code
11. Go to the extensions tab then search for and download the _docker_ and _Dev Containers_ extensions.
12. Go to the new docker tab, right click the fprime-container and select _Attach to VS Code_.
13. Once the new VS Code window opens, click the tab for _File_, then _Open Folder_, and then select the folder `/home/ubuntu/gas_container`.
14. Finally, run the command: `bash fprimeSetup.sh`.

# Linux and WSL Instructions

1. Download the [setup script](https://drive.google.com/file/d/1I5pNPaUdPW7tc2SMeh0j0IWNb_cGyUDq/view?usp=sharing)
2. Move the file to the desired folder and run it by running `bash fprimeBashSetup.sh`.

# Running Locally

1. Run the command: `cd GASRATS-Demo`.
2. Run the command: `. fprime-venv/bin/activate`.
3. Run the command: `bash startLocal.sh`.