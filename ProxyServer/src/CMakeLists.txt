set(EXECUTABLE_OUTPUT_DIR ${PROJECT_BINARY_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)

# set var CMAKE_INSTALL_PREFIX by -DCMAKE_INSTALL_PREFIX=your_install_path
# install(TARGETS xxx_executable xxx_lib xxx_static
#         RUNTIME DESTINATION bin
#         LIBRARY DESTINATION lib
#         ARCHIVE DESTINATION libstatic)
# it will install the targets into 
# ${CMAKE_INSTALL_PREFIX}/bin
# ${CMAKE_INSTALL_PREFIX}/lib
# ${CMAKE_INSTALL_PREFIX}/libstatic

# to install scripts or documents that are not executable
# use:
# install(PROGRAMS xxx_files | xxx_scripts DESTINATION <dir>)

# sometimes we need t