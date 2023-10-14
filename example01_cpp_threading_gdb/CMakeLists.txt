cmake_minimum_required(VERSION 3.16)
project(skanjet)

#set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS -pthread)
#find_package(Boost REQUIRED COMPONENTS
##        system
##        thread
##        ${USE_Boost_LIBRARIES}
##        log
#        )


set(PROJECT_SOURCE_PATH ${CMAKE_SOURCE_DIR})

#---------------------------------------#
# set(SRC_FILES_FOR_CONFIG
#         ${PROJECT_SOURCE_PATH}/configuration/mahda_modbus_config.cpp)

# set(SRC_FILES_FOR_LIB_MODBUS
#         ${PROJECT_SOURCE_PATH}/lib/modbus/mahda_modbus_master.cpp
#         ${PROJECT_SOURCE_PATH}/lib/modbus/mahda_modbus_slave.cpp)


# set(SRC_FILES_FOR_LIB_MAHDA_LOGGER
#         ${PROJECT_SOURCE_PATH}/lib/mahda_logger/mahda_logger.cpp)

#---------------------------------------#
# set(INCLUDE_PATH_FOR_CONFIG
#         ${PROJECT_SOURCE_PATH}/configuration/)

# set(INCLUDE_PATH_FOR_LIB_MAHDA_CORE
#         ${PROJECT_SOURCE_PATH}/lib/mahda_core
#         )

# set(INCLUDE_PATH_FOR_LIB_MAHDA_LOGGER
#         ${PROJECT_SOURCE_PATH}/lib/mahda_logger)

set(INCLUDE_PATH_FOR_LIB_MODBUS
        ${PROJECT_SOURCE_PATH}/lib/modbus
        )


macro(createCmakeSettingsForMainTarget target)

    target_sources(${target} PRIVATE

            ${SRC_FILES_FOR_CONFIG}
            ${SRC_FILES_FOR_LIB_MODBUS}
            ${SRC_FILES_FOR_LIB_MAHDA_LOGGER}
        #     applications/app0_timers/app_timers.cpp
            app4.cpp
        #     applications/app2_modbus_slave/app_modbus_slave.cpp
        #     applications/app3_modbus_master/app_modbus_master.cpp
            )


    target_include_directories(${target} PRIVATE
            #        ${Boost_INCLUDE_DIRS}
            .
            ${INCLUDE_PATH_FOR_CONFIG}
            ${INCLUDE_PATH_FOR_LIB_MODBUS}
            ${INCLUDE_PATH_FOR_LIB_MAHDA_LOGGER}
        #     applications/app_timers
            
        #     applications/app2_modbus_slave
        #     applications/app3_modbus_master
            #        lib/lib_matrix


            )

    target_include_directories(${target} PUBLIC
            $<BUILD_INTERFACE:/usr/local/include/modbus

            #        -DBOOST_LOG_DYN_LINK
            )

    target_compile_options(${target} PUBLIC
            -L/usr/local/lib
            #        -Llib_matrix
            #        -lfahw
            #        -w     # Disables all warnings
            #        -Wno-pointer-to-int-cast  # Disables only "pointer-to-int-cast" warning
            )


    target_link_libraries(${target}
            modbus
            #        fahw
            #        ${Boost_SYSTEM_LIBRARY}
            #        ${Boost_THREAD_LIBRARY}
            #        ${Boost_LOG_LIBRARY}
            #        boost_log_setup
            #        boost_log
            )
endmacro()

set(main_target skanjet_demo)
add_executable(${main_target} main.cpp)
createCmakeSettingsForMainTarget(${main_target})
#target_sources(${main_target} PRIVATE

#        )




