include(GNUInstallDirs)
install(
    FILES
        train.1
    DESTINATION ${CMAKE_INSTALL_MANDIR}/man1     # I'd prefer to use TYPE instead of DESTINATION but it seems I need the man1 subdirectory
)
