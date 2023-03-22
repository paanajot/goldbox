FROM ubuntu:22.04 AS base

# You use this mode when you need zero interaction while installing or upgrading the system via apt.
ARG DEBIAN_FRONTEND=noninteractive

RUN apt-get update && apt-get install --no-install-recommends \
    python3-pip -y && \
    rm -rf /var/lib/apt/lists/*

# opencv dependencies
RUN apt-get update && apt-get install --no-install-recommends \
    python3-opencv x11-xkb-utils libcanberra-gtk3-module python3-tk -y && \
    rm -rf /var/lib/apt/lists/*

RUN pip3 install black pytest tensorflow==2.10.0 matplotlib
ENV PATH="${PATH}:~/.local/bin"
############################################################
### install and configure dev utils
FROM base AS set_finder_poc

RUN apt-get update && apt-get install --no-install-recommends \
    sudo vim -y && \
    rm -rf /var/lib/apt/lists/*

# ### config
ARG UID=1000
ARG USERNAME=dev

RUN useradd -l -m $USERNAME -u $UID

# ### avoiding VS code extension reinstalls
RUN mkdir -p /home/$USERNAME/.vscode-server/extensions \
    /home/$USERNAME/.vscode-server-insiders/extensions && \
    chown -R $USERNAME \
    /home/$USERNAME/.vscode-server \
    /home/$USERNAME/.vscode-server-insiders

ENV LC_ALL C.UTF-8
ENV PYTHONPATH="${PYTHONPATH}:/repo/set_finder"
############################################################
