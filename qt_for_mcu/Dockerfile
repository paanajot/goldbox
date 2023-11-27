############################################################
FROM panajot/cpp_user AS qt_for_mcu

ARG DEBIAN_FRONTEND=noninteractive
ARG UID=1000
ARG USERNAME=dev

WORKDIR /workdir/
RUN chown -R ${USERNAME}:${USERNAME} /workdir/

### Qt dependencies
RUN apt-get update && apt-get install --no-install-recommends \
    libxcb-glx0 libx11-xcb-dev libxrender-dev libxkbcommon-x11-0 libxkbcommon-x11-dev  libfontconfig libxcb-image0 \
    libxcb-render-util0 libxcomposite-dev libwayland-cursor0 libglu1-mesa-dev libegl-dev libxcb-icccm4 libxcb-keysyms1 \
    libxcb-randr0 libxcb-shape0 libxcb-xinerama0 libxcb-cursor-dev libpcre2-16-0 -y && \
    rm -rf /var/lib/apt/lists/*

### Qt for MCU installation (qtaccount.ini with credentials required when building image)
ADD qtaccount.ini /root/.local/share/Qt/qtaccount.ini
ADD https://download.qt.io/archive/online_installers/4.0/qt-unified-linux-x64-4.0.1-1-online.run ./qtinstaller

RUN chmod +x ./qtinstaller && \
    ./qtinstaller in mcu.0260.common_files mcu.0260.core_sources mcu.0260.stm32f769i_discovery \
    qt.qt6.624.gcc_64 qt.qt6.624.addons.qtserialport qt.qt6.624.addons.qtwebsockets qt.qt6.624.addons.qtimageformats \
    mcu.0260.mimxrt1170_evkb mcu.tools.arm_gcc_10_2021_10 mcu.tools.EVK_MIMXRT1170_SDK_2.13.1 \
    qt.tools.qtcreator_gui qt.tools.cmake qt.tools.qtdesignstudio \
    --auto-answer telemetry-question=Yes --default-answer --accept-licenses \
    --no-force-installations --no-default-installations --accept-obligations --confirm-command \
    -t /opt/Qt && \
    rm /workdir/qtinstaller

# Fix warning:
# QStandardPaths: XDG_RUNTIME_DIR points to non-existing path '/tmp/runtime-panajot/', please create it with 0700 permissions.
RUN mkdir -m 700 /tmp/runtime-${USERNAME}/ && chown $UID /tmp/runtime-${USERNAME}/
ENV XDG_RUNTIME_DIR=/tmp/runtime-${USERNAME}/

# solves Gtk-WARNING **: 17:49:02.692: Locale not supported by C library.
ENV LC_ALL C.UTF-8

ENV PATH="/opt/Qt/Tools/QtCreator/bin:/opt/Qt/6.2.4/gcc_64/bin:/opt/Qt/Tools/CMake/bin:/opt/Qt/Tools/QtDesignStudio/bin:${PATH}"
ENV Qt6_DIR="/opt/Qt/6.2.4/gcc_64/lib/cmake"
ENV CMAKE_PREFIX_PATH="/opt/Qt/6.2.4/gcc_64/lib/cmake"

# libcurl
RUN apt-get update && apt-get install --no-install-recommends \
    libcurl4-openssl-dev -y && \
    rm -rf /var/lib/apt/lists/*


# STM32 && NXP programmer
RUN apt-get update && apt-get install --no-install-recommends \
    libusb-1.0-0 libxtst-dev libsm6 udev libice6 -y && \
    rm -rf /var/lib/apt/lists/*


### Custom qt for mcu dependencies
RUN apt-get update && apt-get install --no-install-recommends \
    libclang-13-dev clang-13 llvm-13-dev llvm-13 -y && \
    rm -rf /var/lib/apt/lists/*

ENV QUL_VERSION=2.9
ENV QUL_INSTALL_DIR=/workdir/custom_qul
ENV QUL_BUILD_DIR=/workdir/custom_qul/build_custom_qul
ENV QUL_ROOT=/opt/Qt/QtMCUs/2.6.0
ENV QUL_TOOLS=/opt/Qt/Tools/QtMCUs

RUN mkdir -p $QUL_INSTALL_DIR && cp -r $QUL_ROOT/* $QUL_INSTALL_DIR && \
    mkdir -p $QUL_BUILD_DIR && cd $QUL_BUILD_DIR && \
    cmake $QUL_ROOT -DQul_ROOT=$QUL_ROOT -DCMAKE_BUILD_TYPE=MinSizeRel -DQUL_PLATFORM=qt \
    -DQUL_BUILD_FRAMEWORK=ON -DQUL_BUILD_TOOLS=OFF -DQUL_BUILD_PLATFORM=OFF \
    -DCMAKE_INSTALL_PREFIX=$QUL_INSTALL_DIR -DQUL_BUILD_DEMOS=OFF -DQUL_BUILD_EXAMPLES=OFF \
    -DQUL_GENERATORS=$QUL_ROOT/lib/cmake/Qul/QulGenerators.cmake -DQUL_VERSION=$QUL_VERSION \
    -DQUL_ENABLE_PERFORMANCE_LOGGING=ON -DQUL_ENABLE_PERFORMANCE_CONSOLE_OUTPUT=ON && \
    cp $QUL_ROOT/bin/qt.conf $QUL_ROOT/util/qt.conf && \
    cmake --build . && cmake --install .

ENV QUL_CUSTOM_ROOT=$QUL_INSTALL_DIR

# How to use custom qul:
# cmake .. -DQul_ROOT=$QUL_CUSTOM_ROOT -DCMAKE_BUILD_TYPE=MinSizeRel -DQUL_GENERATORS=$QUL_CUSTOM_ROOT/lib/cmake/Qul/QulGenerators.cmake -DQUL_PLATFORM=qt
