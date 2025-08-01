# Agrupar archivos por tipo
file(GLOB_RECURSE HEADER_FILES
    "*.hpp"
    "*.h"
)

file(GLOB_RECURSE SOURCE_FILES
    "*.cpp"
    "*.c"
)

file(GLOB_RECURSE RESOURCES_FILES
    "*.rc"
    "*.ico"
)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} PREFIX "Header Files" FILES ${HEADER_FILES})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} PREFIX "Source Files" FILES ${SOURCE_FILES})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} PREFIX "Resources Files" FILES ${RESOURCES_FILES})

if (NOT ANDROID)
# Crear el ejecutable
add_executable(${PROJECT_NAME} ${HEADER_FILES} ${SOURCE_FILES} ${RESOURCES_FILES})

# Enlace de bibliotecas
target_link_libraries(${PROJECT_NAME} SDL2::SDL2 ZLIB::ZLIB OpenGL::GL ${HASH_LIBRARY_TGT_NAME})

# Enlace de OpenAL según el sistema operativo
if (CMAKE_SYSTEM_NAME STREQUAL "Windows")
    target_link_libraries(${PROJECT_NAME} OpenAL::OpenAL ${OPENGL_gl_LIBRARY})
elseif (CMAKE_SYSTEM_NAME STREQUAL "Linux")
    target_link_libraries(${PROJECT_NAME} OpenGL::GL ${OPENAL_LIBRARY})
    # Verifica si realmente necesitas una compilación estática
    target_link_options(${PROJECT_NAME} PRIVATE -static-libgcc -static-libstdc++)
endif()
else ()
    add_library(${PROJECT_NAME} SHARED "${HEADER_FILES}" "${SOURCE_FILES}")
    target_link_libraries(${PROJECT_NAME} SDL2::SDL2 ZLIB::ZLIB ${OPENGL_opengl_LIBRARY}
            OpenAL::OpenAL ${HASH_LIBRARY_TGT_NAME} ${SDL2_TTF_LIBRARY} ${TRANSLATOR_LIBRARY} log)
endif ()