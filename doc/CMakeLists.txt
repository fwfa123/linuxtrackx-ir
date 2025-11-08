# doc/CMakeLists.txt
# Documentation installation

# Install documentation files
install(FILES
    README.ltr_pipe
    debug_flags.txt
    DESTINATION share/linuxtrack/doc
)

# Install FlightGear support files
install(FILES
    fgfs/Nasal/LinuxTrack/linuxtrack.nas
    DESTINATION share/linuxtrack/doc/fgfs/Nasal/LinuxTrack
)

install(FILES
    fgfs/Protocol/linuxtrack.nas.xml
    fgfs/Protocol/linuxtrack.xml
    DESTINATION share/linuxtrack/doc/fgfs/Protocol
)

install(FILES
    fgfs/Input/Joysticks/LinuxTrack/uinput-abs.xml
    DESTINATION share/linuxtrack/doc/fgfs/Input/Joysticks/LinuxTrack
)

install(FILES
    fgfs/README
    DESTINATION share/linuxtrack/doc/fgfs
)

