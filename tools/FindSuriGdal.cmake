cmake_minimum_required(VERSION 2.6)

IF(NOT GDAL_FOUND)
	MACRO(FIND_GDAL_DEPENDENCIES)
		FOREACH(LIBRARY ${ARGV})
			FIND_LIBRARY(${LIBRARY}_LIBRARY ${LIBRARY}
						PATHS
						ENV ${LIBRARY}_DIR
						/usr/local/lib
						/usr/lib
						/opt/local/lib
						~/opt/local/lib
						~/opt/lib
						c:/msys/1.0/local/lib
			)
			IF (${LIBRARY}_LIBRARY)
				SET(GDAL_LIBRARIES ${GDAL_LIBRARIES} ${${LIBRARY}_LIBRARY})
			ENDIF(${LIBRARY}_LIBRARY)
		ENDFOREACH(LIBRARY)
	ENDMACRO(FIND_GDAL_DEPENDENCIES)
	
	FIND_PATH(GDAL_INCLUDE_DIR gdal.h
		PATHS
		ENV GDAL_DIR
		/usr/local/include
		/usr/include
		/opt/local/include
		~/opt/local/include
		~/opt/include
		c:/msys/1.0/local/include
	)
	
	FIND_GDAL_DEPENDENCIES(gdal kml kmlbase kmldom kmlregionator kmlengine kmlconvenience minizip uriparser jasper hdf5 mfhdf df gif jpeg geotiff tiff png netcdf z geos_c expat png curl)
	IF (WIN32)
	ELSE(WIN32)
		FIND_GDAL_DEPENDENCIES(rt)
	ENDIF(WIN32)
	
	IF(GDAL_INCLUDE_DIR)
		IF(gdal_LIBRARY)
			MESSAGE(STATUS "Gdal Found: TRUE")
			SET(GDAL_FOUND TRUE)
			SET(GDAL_LIBRARIES ${GDAL_LIBRARIES} ${CMAKE_DL_LIBS})
			INCLUDE_DIRECTORIES(${GDAL_INCLUDE_DIR})
			ADD_DEFINITIONS(-D__GDAL__)
		ELSE(gdal_LIBRARY)
			MESSAGE(SEND_ERROR "Gdal Found: FALSE")
			SET(GDAL_FOUND FALSE)
		ENDIF(gdal_LIBRARY)
	ENDIF(GDAL_INCLUDE_DIR)
ENDIF(NOT GDAL_FOUND)
