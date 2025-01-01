include(GNUInstallDirs)
install(
    FILES
        boxmuller_randn.3
    DESTINATION ${CMAKE_INSTALL_MANDIR}/man3     # I'd prefer to use TYPE instead of DESTINATION but it seems I need the man3 subdirectory
)
