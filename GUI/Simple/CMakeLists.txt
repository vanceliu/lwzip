set(SIMPLE_SOURCES
    SimpleWindow.cpp
    main.cpp
)

set(SIMPLE_HEADERS
    SimpleWindow.h
)

add_executable(lwzip-simple ${SIMPLE_SOURCES} ${SIMPLE_HEADERS})

target_link_libraries(lwzip-simple PRIVATE
    lwzip_common
    7zip_core
    Qt6::Core
    Qt6::Widgets
)

if(APPLE)
    set_target_properties(lwzip-simple PROPERTIES
        MACOSX_BUNDLE TRUE
        MACOSX_BUNDLE_GUI_IDENTIFIER "com.lwzip.simple"
        MACOSX_BUNDLE_BUNDLE_NAME "LWZip"
        MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    )
endif()
