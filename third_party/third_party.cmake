# Only include gtest if testing is enabled
if(BUILD_TESTING)
    include(third_party/gtest.cmake)
endif()

# Only include Qt if building the Qt application
if(BUILD_QT_APP)
    include(third_party/qt.cmake)
endif()
