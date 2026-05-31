set(COMMON_SOURCES
    EncodingManager.cpp
    EncodingSelector.cpp
    PasswordDialog.cpp
    LanguageSelector.cpp
    ArchiveBridge.cpp
)

set(COMMON_HEADERS
    EncodingManager.h
    EncodingSelector.h
    PasswordDialog.h
    LanguageSelector.h
    ArchiveBridge.h
)

add_library(lwzip_common STATIC ${COMMON_SOURCES} ${COMMON_HEADERS})

target_link_libraries(lwzip_common PUBLIC
    Qt6::Core
    Qt6::Widgets
)

target_include_directories(lwzip_common PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
