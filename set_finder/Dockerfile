FROM panajot/cpp AS functional_plus
RUN cd /workdir && \
    git clone -b 'v0.2.18-p0' --single-branch --depth 1 https://github.com/Dobiasd/FunctionalPlus && \
    cd FunctionalPlus && \
    mkdir -p build && cd build && \
    cmake .. && make && make install

FROM panajot/cpp AS eigen
RUN cd /workdir && \
    git clone -b '3.4.0' --single-branch --depth 1 https://gitlab.com/libeigen/eigen.git && \
    cd eigen && \
    mkdir -p build && cd build && \
    cmake .. && make && sudo make install

FROM panajot/cpp AS json
RUN cd /workdir && \
    git clone -b 'v3.10.5' --single-branch --depth 1 https://github.com/nlohmann/json && \
    cd json && \
    mkdir -p build && cd build && \
    cmake -DJSON_BuildTests=OFF .. && make && make install

FROM panajot/cpp AS set_finder

### config
ARG DEBIAN_FRONTEND=noninteractive
ARG UID=1000
ARG USERNAME=dev

# frugally-deep dependencies
COPY --from=functional_plus /usr/local/include/fplus /usr/local/include/fplus
COPY --from=functional_plus /usr/local/lib/cmake/FunctionalPlus /usr/local/lib/cmake/FunctionalPlus
COPY --from=eigen /usr/local/include/eigen3 /usr/local/include/eigen3
COPY --from=eigen /usr/local/share/eigen3 /usr/local/share/eigen3
COPY --from=json /usr/local/include/nlohmann /usr/local/include/nlohmann
COPY --from=json /usr/local/lib/cmake/nlohmann_json /usr/local/lib/cmake/nlohmann_json

RUN sudo ln -s /usr/local/include/eigen3/Eigen /usr/local/include/Eigen && \
    cd /workdir && \
    git clone https://github.com/Dobiasd/frugally-deep && \
    cd frugally-deep && \
    mkdir -p build && cd build && \
    cmake .. && make && make install && \
    cd ../.. && rm -rf frugally-deep

### Qt dependencies
RUN apt-get update && apt-get install --no-install-recommends \
    libxcb-glx0 libx11-xcb-dev libxrender-dev libxkbcommon-x11-0 libfontconfig libxcb-image0 libxcb-render-util0 \
    libxcomposite-dev libwayland-cursor0 libglu1-mesa-dev libegl-dev libxcb-icccm4 libxcb-keysyms1 \
    libxcb-randr0 libxcb-shape0 libxcb-xinerama0 -y && \
    rm -rf /var/lib/apt/lists/*

### Qt for Android
# ADD qtaccount.ini /root/.local/share/Qt/qtaccount.ini
# ADD https://download.qt.io/archive/online_installers/4.5/qt-unified-linux-x64-4.5.0-online.run ./qtinstaller

# RUN chmod +x /workdir/qtinstaller && \
#     /workdir/qtinstaller in qt.qt6.641.android qt.qt6.641.addons.qtmultimedia qt.tools.qtcreator \
#     --auto-answer telemetry-question=Yes --default-answer --accept-licenses \
#     --no-force-installations --no-default-installations --accept-obligations --confirm-command \
#     -t /opt/Qt && \
#     rm /workdir/qtinstaller

# Qt license
# COPY .qt-license /workdir/.qt-license
# ENV QT_LICENSE_FILE=/workdir/.qt-license

# Workaround for bug: https://bugreports.qt.io/browse/QTCREATORBUG-24534
# RUN sed -e "s/, \"cmdline-tools;latest\"//g" -i.backup /opt/Qt/Tools/QtCreator/share/qtcreator/android/sdk_definitions.json

# JDK
RUN wget --no-check-certificate -c --header "Cookie: oraclelicense=accept-securebackup-cookie" \
    https://download.oracle.com/java/17/archive/jdk-17.0.1_linux-x64_bin.tar.gz && \
    tar -xzvf jdk-17.0.1_linux-x64_bin.tar.gz && \
    rm jdk-17.0.1_linux-x64_bin.tar.gz
ENV JAVA_HOME="/workdir/jdk-17.0.1/"

# SDK
RUN apt-get update && apt-get install --no-install-recommends \
    unzip -y && \
    rm -rf /var/lib/apt/lists/*
RUN mkdir sdk && cd sdk && \
    wget https://dl.google.com/android/repository/commandlinetools-linux-8092744_latest.zip && \
    unzip commandlinetools-linux-8092744_latest.zip && \
    rm commandlinetools-linux-8092744_latest.zip && \
    cd cmdline-tools/bin/ && \
    yes | ./sdkmanager --sdk_root=/workdir/sdk/ --install "cmdline-tools;latest" "platform-tools" \
    "platforms;android-31" "build-tools;31.0.0" "ndk;22.1.7171670" "ndk;23.1.7779620"

ENV ANDROID_SDK_ROOT="/workdir/sdk/cmdline-tools"
ENV ANDROID_NDK_ROOT="/workdir/sdk/ndk/23.1.7779620"

# https://wiki.archlinux.org/title/GTK#Suppress_warning_about_accessibility_bus
ENV NO_AT_BRIDGE=1

# solves Gtk-WARNING **: 17:49:02.692: Locale not supported by C library.
ENV LC_ALL C.UTF-8

# QStandardPaths: XDG_RUNTIME_DIR points to non-existing path '/tmp/runtime-panajot/', please create it with 0700 permissions.
RUN mkdir -m 700 /tmp/runtime-${USERNAME}/ && chown $UID /tmp/runtime-${USERNAME}/
ENV XDG_RUNTIME_DIR=/tmp/runtime-${USERNAME}/

### Android studio
RUN cd /workdir/ && \
    wget https://redirector.gvt1.com/edgedl/android/studio/ide-zips/2021.3.1.17/android-studio-2021.3.1.17-linux.tar.gz && \
    tar -xzvf android-studio-2021.3.1.17-linux.tar.gz && \
    rm android-studio-2021.3.1.17-linux.tar.gz

### opencv for android
RUN wget https://sourceforge.net/projects/opencvlibrary/files/4.6.0/opencv-4.6.0-android-sdk.zip && \
    unzip opencv-4.6.0-android-sdk.zip && \
    rm opencv-4.6.0-android-sdk.zip

ENV OPENCV_ANDROID="/workdir/OpenCV-android-sdk"

### opencv for x86_64-linux-gnu
# TODO: install the same version as above
RUN apt-get update && apt-get install libopencv-dev libcanberra-gtk3-module -y && \
    rm -rf /var/lib/apt/lists/*

# TODO: optimize chown
RUN chown -R ${USERNAME}:${USERNAME} /workdir/
ENV PATH="/workdir/jdk-17.0.1/bin:/opt/Qt/Tools/QtCreator/bin:${PATH}"
############################################################
