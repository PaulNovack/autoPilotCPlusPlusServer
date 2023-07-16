#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Environment
MKDIR=mkdir
CP=cp
GREP=grep
NM=nm
CCADMIN=CCadmin
RANLIB=ranlib
CC=gcc
CCC=g++
CXX=g++
FC=gfortran
AS=as

# Macros
CND_PLATFORM=GNU-Linux
CND_DLIB_EXT=so
CND_CONF=Debug
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/AppConfig.o \
	${OBJECTDIR}/BoatMotorDriver.o \
	${OBJECTDIR}/DataStore.o \
	${OBJECTDIR}/GPSFileReader.o \
	${OBJECTDIR}/GPSParser.o \
	${OBJECTDIR}/GPSReader.o \
	${OBJECTDIR}/GPSUtils.o \
	${OBJECTDIR}/Navigation.o \
	${OBJECTDIR}/WebServer.o \
	${OBJECTDIR}/main.o \
	${OBJECTDIR}/mySQLConnectionPool.o


# C Compiler Flags
CFLAGS=

# CC Compiler Flags
CCFLAGS=
CXXFLAGS=

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=-lm -lpthread -ldl `pkg-config --libs libmariadb`  

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/autopilotcplusplusserver

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/autopilotcplusplusserver: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.cc} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/autopilotcplusplusserver ${OBJECTFILES} ${LDLIBSOPTIONS} -lmysqlcppconn

${OBJECTDIR}/AppConfig.o: AppConfig.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g `pkg-config --cflags libmariadb` -std=c++14  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/AppConfig.o AppConfig.cpp

${OBJECTDIR}/BoatMotorDriver.o: BoatMotorDriver.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g `pkg-config --cflags libmariadb` -std=c++14  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/BoatMotorDriver.o BoatMotorDriver.cpp

${OBJECTDIR}/DataStore.o: DataStore.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g `pkg-config --cflags libmariadb` -std=c++14  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/DataStore.o DataStore.cpp

${OBJECTDIR}/GPSFileReader.o: GPSFileReader.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g `pkg-config --cflags libmariadb` -std=c++14  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/GPSFileReader.o GPSFileReader.cpp

${OBJECTDIR}/GPSParser.o: GPSParser.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g `pkg-config --cflags libmariadb` -std=c++14  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/GPSParser.o GPSParser.cpp

${OBJECTDIR}/GPSReader.o: GPSReader.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g `pkg-config --cflags libmariadb` -std=c++14  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/GPSReader.o GPSReader.cpp

${OBJECTDIR}/GPSUtils.o: GPSUtils.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g `pkg-config --cflags libmariadb` -std=c++14  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/GPSUtils.o GPSUtils.cpp

${OBJECTDIR}/Navigation.o: Navigation.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g `pkg-config --cflags libmariadb` -std=c++14  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/Navigation.o Navigation.cpp

${OBJECTDIR}/WebServer.o: WebServer.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g `pkg-config --cflags libmariadb` -std=c++14  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/WebServer.o WebServer.cpp

${OBJECTDIR}/main.o: main.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g `pkg-config --cflags libmariadb` -std=c++14  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/main.o main.cpp

${OBJECTDIR}/mySQLConnectionPool.o: mySQLConnectionPool.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g `pkg-config --cflags libmariadb` -std=c++14  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/mySQLConnectionPool.o mySQLConnectionPool.cpp

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
