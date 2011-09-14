MESSAGE(STATUS "Looking for rt")
FIND_PATH(RT_INCLUDE_PATH 
	NAMES
		RT.h
	PATHS
		/usr/include
		/usr/local/include
		/sw/include
		/opt/local/include
		NO_DEFAULT_PATH)

FIND_LIBRARY(RT_LIBRARY_PATH
	NAMES
		RT
	PATHS
		/usr/lib64
		/usr/lib
		/usr/local/lib64
		/usr/local/lib
		/sw/lib
		/opt/local/lib
		NO_DEFAULT_PATH)

IF(RT_INCLUDE_PATH)
	SET(RT_FOUND 1 CACHE STRING "Set to 1 if rt is found, 0 otherwise")
	MESSAGE(STATUS "Looking for rt headers - found")
ELSE(RT_INCLUDE_PATH)
	SET(RT_FOUND 0 CACHE STRING "Set to 1 if rt is found, 0 otherwise")
	MESSAGE(FATAL_ERROR "Looking for rt headers - not found")
ENDIF(RT_INCLUDE_PATH)

IF(RT_LIBRARY_PATH)
	SET(RT_FOUND 1 CACHE STRING "Set to 1 if rt is found, 0 otherwise")
	MESSAGE(STATUS "Looking for rt library - found")
ELSE(RT_LIBRARY_PATH)
	SET(RT_FOUND 0 CACHE STRING "Set to 1 if rt is found, 0 otherwise")
	MESSAGE(FATAL_ERROR "Looking for rt library - not found")
ENDIF(RT_LIBRARY_PATH)

MARK_AS_ADVANCED(RT_FOUND)
