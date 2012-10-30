
OF_ROOT=../../..

EXCLUDE_FROM_SOURCE="bin,.xcodeproj,obj"

NODEPS = clean
SED_EXCLUDE_FROM_PROJECT_SRC = $(shell echo  $(EXCLUDE_FROM_SOURCE) | sed 's/\,/\\|/g')
PROJECT_SOURCE_DIRS = $(shell find . -maxdepth 1 -mindepth 1 -type d | grep -v "$(SED_EXCLUDE_FROM_PROJECT_SRC)" | sed s/.\\///)
PROJECT_SOURCES = $(shell find $(PROJECT_SOURCE_DIRS) -name "*.cpp" -or -name "*.c" -or -name "*.cc" -or -name "*.cxx")
PROJECT_OBJFILES = $(patsubst %.c,%.o,$(patsubst %.cpp,%.o,$(patsubst %.cxx,%.o,$(patsubst %.cc,%.o,$(PROJECT_SOURCES)))))

    $(info ---PROJECT_SOURCES---)
    $(foreach v, $(PROJECT_SOURCES),$(info $(v)))
