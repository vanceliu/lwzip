set(FM_SOURCES
    FileManagerWindow.cpp
    main.cpp
)

set(FM_HEADERS
    FileManagerWindow.h
)

add_executable(lwzip-fm ${FM_SOURCES} ${FM_HEADERS})

target_link_libraries(lwzip-fm PRIVATE
    lwzip_common
    7zip_core
    Qt6::Core
    Qt6::Widgets
)

if(APPLE)
    set_target_properties(lwzip-fm PROPERTIES
        MACOSX_BUNDLE TRUE
        MACOSX_BUNDLE_GUI_IDENTIFIER "com.lwzip.filemanager"
        MACOSX_BUNDLE_BUNDLE_NAME "LWZip File Manager"
        MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    )
endif()
