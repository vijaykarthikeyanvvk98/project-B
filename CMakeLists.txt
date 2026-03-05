cmake_minimum_required(VERSION 3.19)
project(project-B LANGUAGES CXX)

# 👉 Point to your OpenCV install
set(OpenCV_DIR
    "C:/Users/vijay/Downloads/opencv-4.12.0/build2/install"
)

find_package(OpenCV REQUIRED CONFIG)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets Quick Core5Compat)
find_package(Qt6 REQUIRED COMPONENTS Core )

qt_standard_project_setup()

qt_add_executable(project-B
    WIN32 MACOSX_BUNDLE
    main.cpp
    mainwindow.cpp
    mainwindow.h
    videostreamer.h
    opencvimageprovider.h
    opencvimageprovider.cpp
    videostreamer.cpp
    link.h
    link.cpp
)
qt_add_qml_module(project-B
    URI project-B
    VERSION 1.0
    QML_FILES
    Main.qml
    RESOURCES images/dummy_template3.jpg
    RESOURCES images/boat.svg

)


target_link_libraries(project-B
    PRIVATE
        Qt::Core
        Qt::Widgets
        Qt::Quick
        Qt6::Core5Compat

        ${OPENCV_LIBS}
        opencv_core
        opencv_imgproc
        opencv_highgui
        opencv_videoio
)
target_link_libraries(project-B PRIVATE Qt6::Core)
target_link_libraries(project-B PRIVATE Qt6::Core)

include(GNUInstallDirs)

install(TARGETS project-B
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

qt_generate_deploy_app_script(
    TARGET project-B
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
#install(SCRIPT ${deploy_script})

if(WIN32)
    # 1. Define the OpenCV Binary Path
    # Since you already set OpenCV_DIR, we point to the 'bin' folder inside it
    set(OPENCV_BIN_DIR "${OpenCV_DIR}/x64/vc17/bin") # Adjust 'vc16' if using a different VS version

    # 2. Automated Copy: Post-Build
    # This copies the OpenCV DLLs (like opencv_world.dll) to your EXE folder
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${OPENCV_BIN_DIR}"
        "$<TARGET_FILE_DIR:${PROJECT_NAME}>"
        COMMENT "Copying OpenCV runtime binaries to build folder..."
    )
endif()
