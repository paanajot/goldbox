FROM panajot/cpp_user AS set_finder

### config
ARG DEBIAN_FRONTEND=noninteractive
ARG UID=1000
ARG USERNAME=dev

WORKDIR /workdir
RUN chown -R ${USERNAME}:${USERNAME} $(pwd)

### Qt dependencies
RUN apt-get update && apt-get install --no-install-recommends \
    libxcb-glx0 libx11-xcb-dev libxrender-dev libxkbcommon-x11-0 libfontconfig libxcb-image0 libxcb-render-util0 \
    libxcomposite-dev libwayland-cursor0 libglu1-mesa-dev libegl-dev libxcb-icccm4 libxcb-keysyms1 \
    libxcb-randr0 libxcb-shape0 libxcb-xinerama0 -y && \
    rm -rf /var/lib/apt/lists/*

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
ENV SET_FINDER_CORE="/workdir/set_finder_core"
ENV YUV_CONVERTER="/workdir/libyuv_dir"

### opencv for x86_64-linux-gnu
# TODO: install the same version as above
RUN apt-get update && apt-get install libopencv-dev libcanberra-gtk3-module -y && \
    rm -rf /var/lib/apt/lists/*

### Debugger
RUN apt-get update && apt-get install --no-install-recommends gdb -y && \
    rm -rf /var/lib/apt/lists/*

### Conan
ADD conan2/profiles/ /workdir/conan2/profiles/
RUN pip install conan==2.0.10 && conan config install /workdir/conan2/

# libyuv - dependency to android project
RUN cd /workdir && \
    git clone -b 'stable' --single-branch --depth 1 https://chromium.googlesource.com/libyuv/libyuv

ADD libyuv_install_android.sh /workdir/libyuv/libyuv_install_android.sh
RUN cd /workdir/libyuv/ && \
    ./libyuv_install_android.sh && \
    rm -rf /workdir/libyuv/

ENV PATH="/workdir/jdk-17.0.1/bin:/home/${USERNAME}/.local/bin:${PATH}"
