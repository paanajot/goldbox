############################################################
FROM ubuntu:22.04 AS base-qt

ARG GCC_VERSION=9
ARG DEBIAN_FRONTEND=noninteractive
ARG UID=1000
ARG USERNAME=dev

RUN useradd -l -m $USERNAME -u $UID

ENV CXX=/usr/bin/g++-${GCC_VERSION}
WORKDIR /workdir
RUN chown -R ${USERNAME}:${USERNAME} $(pwd)

### basic packages
RUN apt-get update && apt-get install --no-install-recommends \
    cmake g++-${GCC_VERSION} git build-essential ca-certificates sudo vim \
    x11-xkb-utils libfreetype-dev python3-pip gdb -y && \
    rm -rf /var/lib/apt/lists/*

### clang dependencies
RUN apt-get update && apt-get install --no-install-recommends \
    lsb-release wget software-properties-common gnupg -y && \
    rm -rf /var/lib/apt/lists/*

RUN wget https://apt.llvm.org/llvm.sh && chmod +x llvm.sh && \
    ./llvm.sh 13 all && \
    rm ./llvm.sh
ENV PATH="/usr/lib/llvm-13/bin:${PATH}"

### Qt dependencies
RUN apt-get update && apt-get install --no-install-recommends \
    libxcb-glx0 libx11-xcb-dev libxrender-dev libxkbcommon-x11-0 libxkbcommon-x11-dev  libfontconfig libxcb-image0 \
    libxcb-render-util0 libxcomposite-dev libwayland-cursor0 libglu1-mesa-dev libegl-dev libxcb-icccm4 libxcb-keysyms1 \
    libxcb-randr0 libxcb-shape0 libxcb-xinerama0 libxcb-cursor-dev -y && \
    rm -rf /var/lib/apt/lists/*

### Qt6 installation. qtaccount.ini with credentials required when building image
FROM base-qt AS qtinstaller

### Qt 6.2.4
ADD qtaccount.ini /root/.local/share/Qt/qtaccount.ini
ADD https://download.qt.io/archive/online_installers/4.0/qt-unified-linux-x64-4.0.1-1-online.run ./qtinstaller

RUN chmod +x ./qtinstaller && \
    ./qtinstaller in qt.qt6.650.gcc_64 qt.qt6.624.gcc_64 qt.qt6.624.addons.qtserialport \
    qt.qt6.624.addons.qtwebsockets qt.qt6.624.addons.qtimageformats \
    qt.tools.cmake qt.tools.qtcreator qt.tools.qtcreator_gui \
    qtdesignstudio preview.qt.tools.qtdesignstudio \
    --auto-answer telemetry-question=Yes --default-answer --accept-licenses \
    --no-force-installations --no-default-installations --accept-obligations --confirm-command \
    -t /opt/Qt && \
    rm /workdir/qtinstaller

############################################################
### final image
FROM base-qt AS qt
COPY --from=qtinstaller /opt/Qt /opt/Qt

ENV PATH="/opt/Qt/Tools/QtCreator/bin:/opt/Qt/6.2.4/gcc_64/bin:/opt/Qt/Tools/CMake/bin:${PATH}"
ENV CMAKE_PREFIX_PATH="/opt/Qt/6.2.4/gcc_64/lib/cmake"
ENV Qt6_DIR="/opt/Qt/6.2.4/gcc_64/lib/cmake"

# Fix warning:
# QStandardPaths: XDG_RUNTIME_DIR points to non-existing path '/tmp/runtime-panajot/', please create it with 0700 permissions.
RUN mkdir -m 700 /tmp/runtime-${USERNAME}/ && chown $UID /tmp/runtime-${USERNAME}/
ENV XDG_RUNTIME_DIR=/tmp/runtime-${USERNAME}/

### CodeChecker
# RUN apt-get update && apt-get install --no-install-recommends \
#     python3-dev -y && \
#     rm -rf /var/lib/apt/lists/*

# RUN pip3 install codechecker
# ENV PATH="/home/${USERNAME}/.local/bin:${PATH}"
