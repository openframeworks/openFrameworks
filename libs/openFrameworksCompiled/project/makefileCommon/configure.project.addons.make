ifdef MAKEFILE_DEBUG
    $(info =============================makefile.configure.project.addons================)
endif

# check to make sure OF_ROOT is defined
ifndef OF_ROOT
    $(error OF_ROOT is not defined)
endif

ifndef OF_ADDONS_PATH
    $(error OF_ADDONS_PATH is not defined)
endif

# check to make sure PLATFORM_LIB_SUBPATH is defined
ifndef PLATFORM_LIB_SUBPATH
    $(error PLATFORM_LIB_SUBPATH is not defined)
endif

##########################################################################################
## if platform addon exclusions are defined in the platform specific build files, add 'em
ADDON_EXCLUSIONS += $(PLATFORM_RROJECT_ADDON_EXCLUSIONS)
##########################################################################################

ifeq ($(findstring addons.make,$(wildcard *.make)),addons.make)
    # get a list of every addon in the addons directory
    EVERY_ADDON = $(subst $(OF_ADDONS_PATH)/,,$(wildcard $(OF_ADDONS_PATH)/*))

    # get a list of all addons in our directory
    ADDONS_ALL = $(strip $(shell cat addons.make))

    # compare our project addons list to the available addons and create a list of valid
    VALID_ADDONS = $(filter $(ADDONS_ALL),$(EVERY_ADDON))

    # make a list of invalid addons
    INVALID_ADDONS = $(filter-out $(VALID_ADDONS),$(ADDONS_ALL))


    # filter out all addon exclusions, from the platform and invalid
    ADDONS = $(filter-out $(ADDON_EXCLUSIONS) $(INVALID_ADDONS),$(ADDONS_ALL))

    # if any invalid addons are found, throw an info warning, but don't cause an error
    ifneq (,$(INVALID_ADDONS))
        $(info ---addons.make---)
        $(info ---Invalid Addons Found---) 
        $(foreach v, $(INVALID_ADDONS),$(info $(v)))
    endif

    # if the addons list is NOT empty ...
    ifneq ($(strip $(ADDONS)),)
        # construct the relative addon source directories
        ADDONS_REL_DIRS = $(addsuffix /src, $(ADDONS))
        # construct the relative addon lib directories
        ADDONS_LIBS_REL_DIRS = $(addsuffix /libs, $(ADDONS))
        # construct the full path of the addon source directories
        ADDONS_DIRS = $(addprefix $(OF_ADDONS_PATH)/, $(ADDONS_REL_DIRS) )

        # iterate through and throw a warning if the addon directory is not valid
        #$(foreach v, $(ADDONS_DIRS),ifneq(,$(info $(shell if [ ! -d $(v) ]; then echo 0; fi ))))

        # construct the full path of the addon lib directories
        ADDONS_LIBS_DIRS = $(addprefix $(OF_ADDONS_PATH)/, $(ADDONS_LIBS_REL_DIRS) )

        # construct the full path of the addon platform-specific library directories
        ADDONS_BIN_LIBS_DIRS = $(addsuffix /*/lib/$(PLATFORM_LIB_SUBPATH), $(ADDONS_LIBS_DIRS) )


        #### THE FOLLOWING TWO LINES SEEM TO DUPLICATE SOME STUFF
        # begin constructing a lis of addon includes (no cflags [-I] yet)
        # include the each of the addon base directories
        #ADDONS_INCLUDES =  $(ADDONS_DIRS)
        
        # include the each of the addon lib directories (sometimes the lib is not compiled)
        #ADDONS_INCLUDES += $(ADDONS_LIBS_DIRS)
        
        # include the each of the addon base directories recursively
        ADDONS_INCLUDES += $(shell find $(ADDONS_DIRS) -type d 2> /dev/null)
        # include the each of the addon libs directories recursively
        ADDONS_INCLUDES += $(shell find $(ADDONS_LIBS_DIRS) -type d 2> /dev/null)
        
        # create the fully qualified list of addon CFLAG include (i.e. -I../../myfolder)
        #ADDONSCFLAGS = $(addprefix -I,$(ADDONS_INCLUDES))
        # do this later

        # if the addons folder includes a libsorder.make file, then order the library's 
        # static libs according to the list.  this is important because some libraries
        # require a specific linking order (e.g. opencv).

        # TODO: if ANY of the addons have a libsorder.make file, then ALL of the 
        # addons will be taken through this order lib section.  Perhaps 
        # should use a foreach?
        ifeq ($(findstring libsorder.make,$(shell find $(ADDONS_BIN_LIBS_DIRS) -name libsorder.make 2> /dev/null)),libsorder.make)
            ADDONS_LIBS_W_ORDER = $(shell cat $(shell find $(ADDONS_BIN_LIBS_DIRS) -name libsorder.make 2> /dev/null))
            EXCLUDE_LIBS_FILTER = $(addprefix %,$(addsuffix .a,$(ADDONS_LIBS_W_ORDER)))
            ADDONS_LIBS_STATICS = $(filter-out $(EXCLUDE_LIBS_FILTER), $(shell find $(ADDONS_BIN_LIBS_DIRS) -name *.a))
            ADDONS_LIBS_STATICS += $(addprefix $(shell find $(ADDONS_BIN_LIBS_DIRS) -name libsorder.make 2> /dev/null | sed s/libsorder.make//g),$(ADDONS_LIBS_W_ORDER))
        else
            # if no libsorder file is included, just add them based on their filesystem order
            ADDONS_LIBS_STATICS = $(shell find $(ADDONS_BIN_LIBS_DIRS) -name *.a 2> /dev/null)
        endif

        # make a list of all shared libraries (ending with *.so)
        ADDONS_LIBS_SHARED = $(shell find $(ADDONS_BIN_LIBS_DIRS) -name *.so 2> /dev/null)
        
        # create a master list of both static and shared libraries
        ADDONSLIBS = $(ADDONS_LIBS_STATICS)
        ADDONSLIBS += $(ADDONS_LIBS_SHARED)

        # make a list of all source files in the addons directory (recursively)
        ADDONS_SOURCES = $(shell find $(ADDONS_DIRS) -name "*.cpp" -or -name "*.c" 2> /dev/null)
        # add any source files that are in the libs directory (sometimes these are not compiled)
        ADDONS_SOURCES += $(shell find $(ADDONS_LIBS_DIRS) -name "*.cpp" -or -name "*.c" -or -name "*.cc" -or -name "*.cxx" 2>/dev/null)

        # generate the object file list, for later use with the make
        ADDONS_OBJFILES = $(subst $(OF_ROOT)/, ,$(patsubst %.cc,%.o,$(patsubst %.c,%.o,$(patsubst %.cxx,%.o,$(patsubst %.cpp,%.o,$(ADDONS_SOURCES))))))

        ##########################
        ADDONS_INCLUDES_CFLAGS=$(addprefix -I,$(ADDONS_INCLUDES))

        # print debug information if so instructed
        ifdef MAKEFILE_DEBUG
            $(info ---ADDONS_INCLUDES---)
            $(foreach v, $(ADDONS_INCLUDES),$(info $(v)))
            $(info ---ADDONS_SOURCES---)
            $(foreach v, $(ADDONS_SOURCES),$(info $(v)))
            $(info ---ADDONSLIBS---)
            $(foreach v, $(ADDONSLIBS),$(info $(v)))
            $(info ---ADDONS_OBJFILES---)
            $(foreach v, $(ADDONS_OBJFILES),$(info $(v)))
        endif
    endif
endif
