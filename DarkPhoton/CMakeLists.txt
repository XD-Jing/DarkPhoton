#----------------------------------------------------------------------------
# Setup the project
cmake_minimum_required(VERSION 2.6 FATAL_ERROR)
project(DarkPhoton)

#if ( CMAKE_COMPILER_IS_GNUCC )
    set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -w")
    #endif()


#----------------------------------------------------------------------------
# Find Geant4 package, activating all available UI and Vis drivers by default
# You can set WITH_GEANT4_UIVIS to OFF via the command line or ccmake/cmake-gui
# to build a batch mode only executable
#
option(WITH_GEANT4_UIVIS "Build example with Geant4 UI and Vis drivers" ON)
if(WITH_GEANT4_UIVIS)
  find_package(Geant4 REQUIRED ui_all vis_all)
else()
  find_package(Geant4 REQUIRED)
endif()

#----------------------------------------------------------------------------
# Setup Geant4 include directories and compile definitions
#
include(${Geant4_USE_FILE})

#----------------------------------------------------------------------------
# Find HepMC (required package)
#
find_package(HepMC REQUIRED)


#----------------------------------------------------------------------------
# Find ROOT (required package)
#
FIND_PACKAGE( ROOT REQUIRED)
IF( ROOT_FOUND )
    INCLUDE_DIRECTORIES( ${ROOT_INCLUDE_DIRS} )
    INCLUDE_DIRECTORIES( ${ROOT_INCLUDE_DIR} )
    LINK_LIBRARIES( ${ROOT_LIBRARIES} )
    LINK_DIRECTORIES( ${ROOT_LIBRARY_DIR} )
ENDIF()

MESSAGE( "${ROOT_INCLUDE_DIRS}")

#----------------------------------------------------------------------------
# Find Pythia6 (optional package)
#
find_package(Pythia6 QUIET)
if(PYTHIA6_FOUND)
  message(STATUS "G4 Examples: Pythia6 found. --> DP_simu example with Pythia6 enabled.")
  add_definitions(-DG4LIB_USE_PYTHIA)
else()
  set(PYTHIA6_LIBRARIES "")
endif()

#----------------------------------------------------------------------------
# Locate sources and headers for this project
#
include_directories(${PROJECT_SOURCE_DIR}/DarkPhoton
                    ${Geant4_INCLUDE_DIR}
                    ${HEPMC_INCLUDE_DIR})
file(GLOB sources ${PROJECT_SOURCE_DIR}/Root/*.cc)
file(GLOB headers ${PROJECT_SOURCE_DIR}/DarkPhoton/*.hh)

#----------------------------------------------------------------------------
# Add the executable, and link it to the Geant4 libraries
#
add_executable(simu utils/simu.cc ${sources} ${headers})
add_executable(Plots utils/Plots.cc)
add_executable(SlimROOT utils/SlimROOT.cc
    ${PROJECT_SOURCE_DIR}/DarkPhoton/SlimROOT.hh
    ${PROJECT_SOURCE_DIR}/Root/SlimROOT.cc)

target_link_libraries(simu ${Geant4_LIBRARIES}
                      ${HEPMC_LIBRARIES} ${HEPMC_FIO_LIBRARIES}
                      ${PYTHIA6_LIBRARIES})

# if pythia is compiled with g77, link with -lg2c instead.
#target_link_libraries(DP_simu ${Geant4_LIBRARIES}
#                      ${HEPMC_LIBRARIES} ${HEPMC_FIO_LIBRARIES}
#                      ${PYTHIA6_LIBRARIES} g2c)

#----------------------------------------------------------------------------
# Copy all scripts to the build directory, i.e. the directory in which we
# build DP_simu. This is so that we can run the executable directly because it
# relies on these scripts being in the current working directory.
#
set(simu_SCRIPTS
  macros/example.in
  macros/gui.mac
  macros/init.mac
  macros/init_vis.mac
  macros/vis.mac
  )


foreach(_script ${simu_SCRIPTS})
  configure_file(
    ${PROJECT_SOURCE_DIR}/${_script}
    ${PROJECT_BINARY_DIR}/${_script}
    COPYONLY
    )
endforeach()

#----------------------------------------------------------------------------
# Install the executable to 'bin' directory under CMAKE_INSTALL_PREFIX
#
install(TARGETS simu  SlimROOT DESTINATION bin)

