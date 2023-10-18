#
# OMNeT++/OMNEST Makefile for Mobile6TiSCH-3-0-Chester
#
# This file was generated with the command:
#  opp_makemake -f --deep -I. -I../or-tools/include -L../or-tools/lib -lortools -e cc
#

# Name of target to be created (-o option)
TARGET_DIR = .
TARGET_NAME = Mobile6TiSCH-3-0-Chester$(D)
TARGET = $(TARGET_NAME)$(EXE_SUFFIX)
TARGET_IMPLIB = $(TARGET_NAME)$(IMPLIB_SUFFIX)
TARGET_IMPDEF = $(TARGET_NAME)$(IMPDEF_SUFFIX)
TARGET_FILES = $(TARGET_DIR)/$(TARGET)

# User interface (uncomment one) (-u option)
USERIF_LIBS = $(ALL_ENV_LIBS) # that is, $(QTENV_LIBS) $(CMDENV_LIBS)
#USERIF_LIBS = $(CMDENV_LIBS)
#USERIF_LIBS = $(QTENV_LIBS)

# C++ include paths (with -I)
INCLUDE_PATH = -I. -I../or-tools/include

# Additional object and library files to link with
EXTRA_OBJS =

# Additional libraries (-L, -l options)
LIBS = $(LDFLAG_LIBPATH)../or-tools/lib  -lortools

# Output directory
PROJECT_OUTPUT_DIR = ../out
PROJECTRELATIVE_PATH = src
O = $(PROJECT_OUTPUT_DIR)/$(CONFIGNAME)/$(PROJECTRELATIVE_PATH)

# Object files for local .cc, .msg and .sm files
OBJS = \
    $O/applications/base/ApplicationBase.o \
    $O/applications/convergecast/ConvergecastClient.o \
    $O/applications/convergecast/ConvergecastServer.o \
    $O/applications/reqres/ReqResClient.o \
    $O/applications/reqres/ReqResServer.o \
    $O/common/basemodule/BaseModule.o \
    $O/common/crosslayerdb/CrossLayerDB.o \
    $O/common/statscollector/StatsCollector.o \
    $O/environment/base/EnvironmentManagerBase.o \
    $O/linklayer/base/LinkLayerBase.o \
    $O/linklayer/ieee802154tsch/IEEE802154TSCH.o \
    $O/linklayer/ieee802154tsch/IEEE802154TSCHSchedule.o \
    $O/linklayer/ieee802154tsch/schedulingfunction/base/SchedulingFunctionBase.o \
    $O/linklayer/ieee802154tsch/schedulingfunction/shmg/centralized/CentralizedSF.o \
    $O/linklayer/ieee802154tsch/schedulingfunction/shmg/centralized/alice/ALICE.o \
    $O/linklayer/ieee802154tsch/schedulingfunction/shmg/centralized/amus/AMUS.o \
    $O/linklayer/ieee802154tsch/schedulingfunction/shmg/centralized/dummysf/DummySF.o \
    $O/linklayer/ieee802154tsch/schedulingfunction/shmg/centralized/lasa/LASA.o \
    $O/linklayer/ieee802154tsch/schedulingfunction/shmg/centralized/lasa/MCS.o \
    $O/linklayer/ieee802154tsch/schedulingfunction/shmg/centralized/lasa/lasar/LASAR.o \
    $O/linklayer/ieee802154tsch/schedulingfunction/shmg/centralized/orchestra/Orchestra.o \
    $O/linklayer/ieee802154tsch/schedulingfunction/shmg/centralized/sddu/SDDU.o \
    $O/middleware/base/MiddlewareBase.o \
    $O/middleware/shmg/brmiddleware/base/BRMiddlewareBase.o \
    $O/middleware/shmg/brmiddleware/lasa/LASABRMiddleware.o \
    $O/middleware/shmg/brmiddleware/lasa/lasar/LASARBRMiddleware.o \
    $O/middleware/shmg/mnmiddleware/base/MNMiddlewareBase.o \
    $O/middleware/shmg/mnmiddleware/lasa/LASAMNMiddleware.o \
    $O/middleware/shmg/mnmiddleware/lasa/lasar/LASARMNMiddleware.o \
    $O/middleware/shmg/ncmiddleware/base/DedupTable.o \
    $O/middleware/shmg/ncmiddleware/base/NCMiddlewareBase.o \
    $O/middleware/shmg/ncmiddleware/base/RSSITable.o \
    $O/middleware/shmg/ncmiddleware/lasa/LASANCMiddleware.o \
    $O/middleware/shmg/ncmiddleware/lasa/lasar/LASARNCMiddleware.o \
    $O/mobility/base/MobilityBase.o \
    $O/mobility/brmobility/BRMobility.o \
    $O/mobility/linearmobility/LinearMobility.o \
    $O/mobility/randommobility/RandomMobility.o \
    $O/mobility/staticmobility/StaticMobility.o \
    $O/physicallayer/radio/base/GlobalRadioRegister.o \
    $O/physicallayer/radio/base/RadioBase.o \
    $O/physicallayer/wired/base/WiredNICBase.o \
    $O/sensors/base/SensorBase.o \
    $O/sensors/orientation/OrientationSensor.o \
    $O/sensors/position/PositionSensor.o \
    $O/utils/algorithms.o \
    $O/utils/point/Point.o \
    $O/utils/point/Polygon.o \
    $O/utils/point/Segment.o \
    $O/utils/shmg/lasa/LASAUtils.o \
    $O/applications/convergecast/ConvergecastPkt_m.o \
    $O/applications/reqres/ReqResPkt_m.o \
    $O/linklayer/ieee802154tsch/IEEE802154TSCHAck_m.o \
    $O/linklayer/ieee802154tsch/IEEE802154TSCHPkt_m.o \
    $O/middleware/shmg/messages/BackboneCommand_m.o \
    $O/middleware/shmg/messages/BackboneDataInfo_m.o \
    $O/middleware/shmg/messages/SHMGNetworkPkt_m.o \
    $O/middleware/shmg/messages/lasa/LASANetworkPkt_m.o \
    $O/middleware/shmg/messages/lasa/lasar/ScheduleUpdate_m.o \
    $O/physicallayer/radio/base/RadioPkt_m.o \
    $O/physicallayer/radio/base/SignalDescriptor_m.o \
    $O/physicallayer/wired/base/WiredNICPkt_m.o

# Message files
MSGFILES = \
    applications/convergecast/ConvergecastPkt.msg \
    applications/reqres/ReqResPkt.msg \
    linklayer/ieee802154tsch/IEEE802154TSCHAck.msg \
    linklayer/ieee802154tsch/IEEE802154TSCHPkt.msg \
    middleware/shmg/messages/BackboneCommand.msg \
    middleware/shmg/messages/BackboneDataInfo.msg \
    middleware/shmg/messages/SHMGNetworkPkt.msg \
    middleware/shmg/messages/lasa/LASANetworkPkt.msg \
    middleware/shmg/messages/lasa/lasar/ScheduleUpdate.msg \
    physicallayer/radio/base/RadioPkt.msg \
    physicallayer/radio/base/SignalDescriptor.msg \
    physicallayer/wired/base/WiredNICPkt.msg

# SM files
SMFILES =

#------------------------------------------------------------------------------

# Pull in OMNeT++ configuration (Makefile.inc)

ifneq ("$(OMNETPP_CONFIGFILE)","")
CONFIGFILE = $(OMNETPP_CONFIGFILE)
else
CONFIGFILE = $(shell opp_configfilepath)
endif

ifeq ("$(wildcard $(CONFIGFILE))","")
$(error Config file '$(CONFIGFILE)' does not exist -- add the OMNeT++ bin directory to the path so that opp_configfilepath can be found, or set the OMNETPP_CONFIGFILE variable to point to Makefile.inc)
endif

include $(CONFIGFILE)

# Simulation kernel and user interface libraries
OMNETPP_LIBS = $(OPPMAIN_LIB) $(USERIF_LIBS) $(KERNEL_LIBS) $(SYS_LIBS)
ifneq ($(PLATFORM),win32)
LIBS += -Wl,-rpath,$(abspath ../or-tools/lib)
endif

COPTS = $(CFLAGS) $(IMPORT_DEFINES)  $(INCLUDE_PATH) -I$(OMNETPP_INCL_DIR)
MSGCOPTS = $(INCLUDE_PATH)
SMCOPTS =

# we want to recompile everything if COPTS changes,
# so we store COPTS into $COPTS_FILE (if COPTS has changed since last build)
# and make the object files depend on it
COPTS_FILE = $O/.last-copts
ifneq ("$(COPTS)","$(shell cat $(COPTS_FILE) 2>/dev/null || echo '')")
  $(shell $(MKPATH) "$O")
  $(file >$(COPTS_FILE),$(COPTS))
endif

#------------------------------------------------------------------------------
# User-supplied makefile fragment(s)
#------------------------------------------------------------------------------

# Main target
all: $(TARGET_FILES)

$(TARGET_DIR)/% :: $O/%
	@mkdir -p $(TARGET_DIR)
	$(Q)$(LN) $< $@
ifeq ($(TOOLCHAIN_NAME),clang-msabi)
	-$(Q)-$(LN) $(<:%.dll=%.lib) $(@:%.dll=%.lib) 2>/dev/null

$O/$(TARGET_NAME).pdb: $O/$(TARGET)
endif

$O/$(TARGET): $(OBJS)  $(wildcard $(EXTRA_OBJS)) Makefile $(CONFIGFILE)
	@$(MKPATH) $O
	@echo Creating executable: $@
	$(Q)$(CXX) $(LDFLAGS) -o $O/$(TARGET) $(OBJS) $(EXTRA_OBJS) $(AS_NEEDED_OFF) $(WHOLE_ARCHIVE_ON) $(LIBS) $(WHOLE_ARCHIVE_OFF) $(OMNETPP_LIBS)

.PHONY: all clean cleanall depend msgheaders smheaders

.SUFFIXES: .cc

$O/%.o: %.cc $(COPTS_FILE) | msgheaders smheaders
	@$(MKPATH) $(dir $@)
	$(qecho) "$<"
	$(Q)$(CXX) -c $(CXXFLAGS) $(COPTS) -o $@ $<

%_m.cc %_m.h: %.msg
	$(qecho) MSGC: $<
	$(Q)$(MSGC) -s _m.cc -MD -MP -MF $O/$(basename $<)_m.h.d $(MSGCOPTS) $?

%_sm.cc %_sm.h: %.sm
	$(qecho) SMC: $<
	$(Q)$(SMC) -c++ -suffix cc $(SMCOPTS) $?

msgheaders: $(MSGFILES:.msg=_m.h)

smheaders: $(SMFILES:.sm=_sm.h)

clean:
	$(qecho) Cleaning $(TARGET)
	$(Q)-rm -rf $O
	$(Q)-rm -f $(TARGET_FILES)
	$(Q)-rm -f $(call opp_rwildcard, . , *_m.cc *_m.h *_sm.cc *_sm.h)

cleanall:
	$(Q)$(CLEANALL_COMMAND)
	$(Q)-rm -rf $(PROJECT_OUTPUT_DIR)

help:
	@echo "$$HELP_SYNOPSYS"
	@echo "$$HELP_TARGETS"
	@echo "$$HELP_VARIABLES"
	@echo "$$HELP_EXAMPLES"

# include all dependencies
-include $(OBJS:%=%.d) $(MSGFILES:%.msg=$O/%_m.h.d)
