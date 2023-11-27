cmake_minimum_required(VERSION 3.15)

project(MiniGames VERSION 0.0.1 LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6 6.2 COMPONENTS Quick REQUIRED)

qt_add_executable(mini_games
    resources.qrc
    main.cpp
)

qt_add_qml_module(mini_games
    URI MiniGames
    VERSION 1.0
    QML_FILES MainPage.qml TicTacToe.qml Memory.qml ConnectDots.qml Settings.qml SettingsData.qml MemoryData.qml
)

target_link_libraries(mini_games PRIVATE Qt6::Quick)

install(TARGETS mini_games
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)


#qul_add_target(loader)
#qul_target_qml_sources(loader MainPage.qml TicTacToe.qml Memory.qml ConnectDots.qml Settings.qml)
#app_target_setup_os(loader)
#app_target_default_entrypoint(loader MainPage)
#target_link_libraries(loader PRIVATE Qul::Shapes LoaderDataModule)

#if(NOT CMAKE_CROSSCOMPILING AND NOT WIN32)
#    add_custom_command(TARGET loader
#        COMMAND strip loader -o loader.stripped
#        DEPENDS loader)
#endif()

#if (QUL_BUILD_QMLPROJECT_EXAMPLES)
#    qul_add_target(loader_qmlproject QML_PROJECT mcu_loader.qmlproject)
#    app_target_setup_os(loader_qmlproject)
#    app_target_default_entrypoint(loader_qmlproject MainPage)
#    target_link_libraries(loader_qmlproject PRIVATE LoaderDataModule)

#    if(NOT CMAKE_CROSSCOMPILING AND NOT WIN32)
#        add_custom_command(TARGET loader_qmlproject
#            COMMAND strip loader_qmlproject -o loader.stripped
#            DEPENDS loader_qmlproject)
#    endif()
#endif()
