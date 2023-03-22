include(GNUInstallDirs)

# set(CMAKE_INSTALL_PREFIX ${CMAKE_BINARY_DIR})

install(TARGETS lib_unique_ptr lib_metaprogramming
        PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/box
)

install(TARGETS cxx20 EXPORT GoldboxlTargets
        DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

install(EXPORT GoldboxlTargets
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${CMAKE_PROJECT_NAME}
        NAMESPACE ${CMAKE_PROJECT_NAME}::
)

install(FILES cmake/my_config.cmake config/iwyu_map_file.imp
        DESTINATION ${CMAKE_INSTALL_DATADIR}
)
