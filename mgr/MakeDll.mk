#
ifdef SO_LIB
   OUT_UP  := $(subst / ,,$(dir $(SO_LIB))  )
   OUT_UPP := $(subst / ,,$(dir $(OUT_UP))  )
   OUT_DIR := $(subst / ,,$(dir $(OUT_UPP)) )
endif
ifndef OUT_DIR
  OUT_DIR := $(shell pwd)
endif



###	Suppress all imlicit rules
.SUFFIXES:

#
ifndef STAF_MAKE_HOME
  STAF_MAKE_HOME := $(STAR)/mgr
endif

include $(STAF_MAKE_HOME)/MakeEnv.mk
include $(STAF_MAKE_HOME)/MakeArch.mk

#
#	INP_DIR & OUT_DIR could be declared in invoking
#
ifndef INP_DIR
  override INP_DIR := $(CWD)
endif
ifeq (,$(strip $(filter /%,$(INP_DIR))))
  override INP_DIR := $(CWD)/$(INP_DIR)
endif

#	ROOT DIR (Not a Rene ROOT)
PAMS    := $(findstring /pams,$(INP_DIR))
ifndef PAMS
  PAMS    := $(findstring /StRoot,$(INP_DIR))
endif
ROOT_DIR:= $(word 1,$(subst $(PAMS), ,$(INP_DIR)))


PKGNAME := $(notdir $(INP_DIR))

#
#	Define .src dir. If does not exist EMPTY
#
SRC_DIR := $(INP_DIR)
GEN_DIR := $(OUT_DIR)/.share/$(PKGNAME)

ifndef SO_LIB
  SO_LIB := $(LIB_DIR)/lib$(PKGNAME).$(SOEXT)
endif

SYS_DIR := $(OUT_DIR)/.$(STAR_SYS)
LIB_DIR := $(SYS_DIR)/lib
OBJ_DIR := $(SYS_DIR)/obj/$(PKGNAME)
STAR_OBJ_DIR := $(STAR)/obj/$(PKGNAME)
DEP_DIR := $(SYS_DIR)/dep/$(PKGNAME)
TMP_DIR := $(SYS_DIR)/tmp
SRC_DIR := $(INP_DIR)
#.
    check_out   := $(shell test -d $(OUT_DIR) || mkdir -p $(OUT_DIR)) 
    check_sys   := $(shell test -d $(SYS_DIR) || mkdir -p $(SYS_DIR)) 
    check_lib   := $(shell test -d $(LIB_DIR) || mkdir -p $(LIB_DIR))
    check_obj   := $(shell test -d $(OBJ_DIR) || mkdir -p $(OBJ_DIR))
    check_dep   := $(shell test -d $(DEP_DIR) || mkdir -p $(DEP_DIR))
    check_gen   := $(shell test -d $(GEN_DIR) || mkdir -p $(GEN_DIR))

#	Includes

# 	Define internal and external includes dirs
INC_NAMES := tables base StChain StModule xdf2root

INC_DIRS := $(addprefix  $(OUT_DIR)/.share/,$(INC_NAMES))
INC_DIRS += $(addprefix $(ROOT_DIR)/StRoot/,$(INC_NAMES))
INC_DIRS += $(addprefix     $(STAR)/.share/,$(INC_NAMES))
INC_DIRS += $(addprefix     $(STAR)/StRoot/,$(INC_NAMES))
INCINT := $(INC_DIRS)
INC_DIRS += $(SRC_DIR) $(GEN_DIR) $(ROOT_DIR)/inc  $(STAR)/inc 
#
INCINT := $(INC_DIRS)
INC_DIRS += $(OUT_DIR)/.share $(ROOT_DIR)/StRoot $(STAR)/.share $(STAR)/StRoot $(ROOTSYS)/include

INCLUDES := $(addprefix -I,$(INC_DIRS))
INCINT   := $(addprefix -I,$(INCINT))

CPPFLAGS += -D__ROOT__

#
#	If NO source , NOTHING to do
#	Skip up to the end
#
FILES_SRC := $(wildcard $(addprefix $(SRC_DIR)/, *.c *.cxx))
ifeq ($(PKGNAME),xdf2root)
  FILES_SRC  += $(wildcard $(STAR)/asps/staf/dsl/src/*.c)
  INPUT_DIRS := $(STAR)/asps/staf/dsl/src
endif
DOIT := $(strip $(FILES_SRC))
ifneq (,$(DOIT))


DEP_DIR := $(SYS_DIR)/dep/$(PKGNAME)

OUPUT_DIRS := $(LIB_DIR) $(OBJ_DIR) $(DEP_DIR) $(BIN_DIR) $(TMP_DIR) $(GEN_DIR) $(SRC_DIR) 
INPUT_DIRS += $(SRC_DIR) 

# 	Make dirs before make real work. Othervice VPATH does not see
#    	non existing directories
MAKEDIRS := $(shell mkdir -p $(OUPUT_DIRS))

VPATH =  $(INPUT_DIRS) $(OUPUT_DIRS)

FILES_SYM  := $(wildcard $(SRC_DIR)/St_Module.cxx)
FILES_SYT  := $(wildcard $(SRC_DIR)/St_Table.cxx)
FILES_TAB  := $(wildcard $(SRC_DIR)/St_*_Table.cxx)
FILES_MOD  := $(wildcard $(SRC_DIR)/St_*_Module.cxx)
FILES_DAT  := $(wildcard $(SRC_DIR)/St_DataSet.cxx)
FILES_XDF  := $(wildcard $(SRC_DIR)/St_XDFFile.cxx)
FILES_ALL  := $(filter-out %Cint.cxx,$(wildcard $(SRC_DIR)/St*.cxx))
FILES_ST   := $(FILES_SYM) $(FILES_SYT) $(FILES_TAB) $(FILES_MOD) $(FILES_DAT)
FILES_ALL  := $(filter-out $(FILES_ST),$(FILES_ALL))
FILES_ORD  := $(FILES_ALL)
ifdef FILES_SYM
  NAMES_SYM      := $(subst St_,,$(basename $(notdir $(FILES_SYM))))
  FILES_SYM_H    := $(addprefix $(SRC_DIR)/St_,$(addsuffix .h,$(NAMES_SYM)))
  FILES_CINT_SYM := $(addprefix $(GEN_DIR)/St_,$(addsuffix Cint.cxx,$(NAMES_SYM)))
endif

ifdef FILES_SYT
  NAMES_SYT      := $(subst St_,,$(basename $(notdir $(FILES_SYT))))
  FILES_SYT_H    := $(addprefix $(SRC_DIR)/St_,$(addsuffix .h,$(NAMES_SYT)))
  FILES_CINT_SYT := $(addprefix $(GEN_DIR)/St_,$(addsuffix Cint.cxx,$(NAMES_SYT)))
endif

ifdef FILES_TAB
  NAMES_TAB      := $(subst _Table,,$(subst St_,,$(basename $(notdir $(FILES_TAB)))))
  FILES_TAB_H    := $(addprefix $(SRC_DIR)/St_,$(addsuffix _Table.h,$(NAMES_TAB)))
  FILES_CINT_TAB := $(addprefix $(GEN_DIR)/St_,$(addsuffix _TableCint.cxx,$(NAMES_TAB)))
endif

ifdef FILES_MOD
  NAMES_MOD      := $(subst _Module,,$(subst St_,,$(basename $(notdir $(FILES_MOD)))))
  FILES_MOD_H    := $(addprefix $(SRC_DIR)/St_,$(addsuffix _Module.h,$(NAMES_MOD)))
  FILES_CINT_MOD := $(addprefix $(GEN_DIR)/St_,$(addsuffix _ModuleCint.cxx,$(NAMES_MOD)))
endif

ifdef FILES_ORD
  NAMES_ORD      := $(basename $(notdir $(FILES_ORD)))
  FILES_ORD_H    := $(subst .cxx,.h, $(FILES_ORD))
  FILES_CINT_ORD := $(addprefix $(GEN_DIR)/,$(addsuffix Cint.cxx, $(NAMES_ORD)))
endif

LINKDEF := $(GEN_DIR)/LinkDef.h



#	Define the common part of ROOTCINTs
define  COMMON_LINKDEF
	@test -f $(LINKDEF) &&  $(RM) $(LINKDEF);\
	echo "#ifdef __CINT__"                  	>  $(LINKDEF);\
	echo "#pragma link off all globals;"    	>> $(LINKDEF);\
	echo "#pragma link off all classes;"    	>> $(LINKDEF);\
	echo "#pragma link off all functions;"  	>> $(LINKDEF);\
	echo "#pragma link C++ class St_$(STEM)-;"	>> $(LINKDEF);
endef
define  ORD_LINKDEF
	@test -f $(LINKDEF) &&  $(RM) $(LINKDEF);\
	echo "#ifdef __CINT__"                  	>  $(LINKDEF);\
	echo "#pragma link off all globals;"    	>> $(LINKDEF);\
	echo "#pragma link off all classes;"    	>> $(LINKDEF);\
	echo "#pragma link off all functions;"  	>> $(LINKDEF);\
	echo "#pragma link C++ class $(STEM);"  	>> $(LINKDEF);\
	echo "#endif"					>> $(LINKDEF);
endef

FILES_O := $(FILES_SRC) $(FILES_CINT_SYM) $(FILES_CINT_SYT) $(FILES_CINT_TAB) $(FILES_CINT_MOD) $(FILES_ORD) $(FILES_CINT_ORD)
FILES_O := $(addprefix $(OBJ_DIR)/,$(addsuffix .o, $(notdir $(basename $(FILES_O)))))
FILES_O := $(sort $(FILES_O))
STAR_FILES_O := $(wildcard $(STAR_OBJ_DIR)/*.o)
FILTER  := $(addprefix %/,$(notdir $(FILES_O)))
STAR_FILES_O := $(filer-out $(FILTER),$(STAR_FILES_O))
FILES_D := $(addsuffix .d, $(addprefix $(DEP_DIR)/,$(basename $(notdir $(FILES_O)))))



ifeq (,$(FILES_O))
all: 
	@echo Nothing to do for $(PKG)
else

MY_SO  := $(SO_LIB)
  QWE  := $(wildcard $(MY_SO).*)
SL_NEW := $(MY_SO).1000
ifdef QWE
  NQWE := $(words $(QWE))
  QWE  := $(word $(NQWE),$(QWE))
  QWE  := $(subst $(MY_SO).,,$(QWE))
  QWE  := $(shell expr $(QWE) + 1)
  SL_NEW := $(MY_SO).$(QWE)
endif
MY_AR  := $(addsuffix .a, $(basename $(MY_SO)))
#

.PHONY : all  RootCint Libraries  DeleteDirs




all:   RootCint Libraries  DeleteDirs

RootCint : $(FILES_CINT_SYT) $(FILES_CINT_SYM) $(FILES_CINT_TAB) $(FILES_CINT_MOD)


$(FILES_CINT_SYT) : $(GEN_DIR)/St_%Cint.cxx : $(SRC_DIR)/St_%.h 
	$(COMMON_LINKDEF)
	@echo "#pragma link C++ class table_head_st-!;"	>> $(LINKDEF);
	@echo "#endif"					>> $(LINKDEF);
	@cat $(LINKDEF);
	cd $(GEN_DIR); cp $(1ST_DEPS) .; \
	rootcint -f $(notdir $(ALL_TAGS)) -c -DROOT_CINT $(INCINT) $(notdir $(1ST_DEPS)) $(LINKDEF); 


$(FILES_CINT_SYM) : $(GEN_DIR)/St_%Cint.cxx : $(SRC_DIR)/St_%.h 
	$(COMMON_LINKDEF)
	@echo "#pragma link C++ class St_DataSet;"       >> $(LINKDEF);
	@echo "#pragma link C++ enum EModuleTypes;"      >> $(LINKDEF);
	@echo "#endif"					 >> $(LINKDEF);
	@cat $(LINKDEF);
	cd $(GEN_DIR); cp $(1ST_DEPS) .; \
	rootcint -f $(notdir $(ALL_TAGS)) -c -DROOT_CINT $(INCINT)  $(notdir $(1ST_DEPS)) \
        St_DataSet.h $(notdir $(LINKDEF));

$(FILES_CINT_ORD) : $(GEN_DIR)/%Cint.cxx : $(SRC_DIR)/%.h   
	$(ORD_LINKDEF)
	@cat $(LINKDEF)
	cd $(GEN_DIR); cp $(1ST_DEPS) .; \
        rootcint -f $(notdir $(ALL_TAGS)) -c -DROOT_CINT $(INCINT) $(notdir $(1ST_DEPS)) \
         $(notdir $(LINKDEF));


#$(FILES_CINT_TAB) : 
$(GEN_DIR)/St_%_TableCint.cxx : $(SRC_DIR)/St_%_Table.h 
	$(COMMON_LINKDEF)
	@echo "#pragma link C++ class $(STEM)_st-!;"	>> $(LINKDEF);
	@echo "#endif"					>> $(LINKDEF);
	@cat $(LINKDEF);
	cd $(GEN_DIR); cd $(GEN_DIR); cp $(1ST_DEPS) .;\
	rootcint -f $(notdir $(ALL_TAGS)) -c -DROOT_CINT $(INCINT) $(notdir $(1ST_DEPS)) $(notdir $(LINKDEF));

#$(FILES_CINT_MOD) : 
$(GEN_DIR)/St_%_ModuleCint.cxx : $(GEN_DIR)/St_%_Module.h 
	$(COMMON_LINKDEF)
	@echo "#pragma link C++ global $(STEM);"	>> $(LINKDEF);
	@echo "#endif"					>> $(LINKDEF);
	@cat $(LINKDEF);
	cd $(GEN_DIR);\
        rootcint -f $(notdir $(ALL_TAGS)) -c -DROOT_CINT $(INCINT) $(notdir $(1ST_DEPS)) $(notdir $(LINKDEF));
#  $(INCLUDES)

Libraries : $(MY_SO) 


$(MY_SO) : $(FILES_O)
	cd $(OBJ_DIR); \
        $(SO) $(SOFLAGS) -o $(SL_NEW) $(notdir $(FILES_O)) $(STAR_FILES_O) $(LIBRARY); \
        $(RM) $(MY_SO); $(LN) $(SL_NEW) $(MY_SO)'
	@echo "           Shared library " $(MY_SO) " has been created"   

#_________________dependencies_____________________________
ifneq (, $(strip $(FILES_D))) 
include $(FILES_D)
endif                               #

$(OBJ_DIR)/%.o : %.c
	$(CC)  -c $(CPPFLAGS) $(CFLAGS)    $(INCLUDES) $(1ST_DEPS) -o $(ALL_TAGS)

$(OBJ_DIR)/%.o : %.cxx 
	$(CXX) -c $(CPPFLAGS) $(CXXFLAGS)  $(INCLUDES)  $(1ST_DEPS) -o $(ALL_TAGS)

$(DEP_DIR)/%Cint.d: %.h 
	$(RM) $(ALL_TAGS)
	$(GCC) $(MKDEPFLAGS) $(CPPFLAGS) $(INCLUDES) -x c $(1ST_DEPS) | sed -e \
's/$(notdir $(STEM))\.h\.o/$(subst .,\.,$(subst /,\/,$(OBJ_DIR)/$(STEM)Cint.o)) $(subst .,\.,$(subst /,\/,$(ALL_TAGS)))/g'\
        > $(ALL_TAGS)

$(DEP_DIR)/%.d: %.c 
	$(RM) $(ALL_TAGS)
	$(GCC) $(MKDEPFLAGS) $(CPPFLAGS) $(INCLUDES) $(1ST_DEPS) | sed -e \
's/$(notdir $(STEM))\.o/$(subst .,\.,$(subst /,\/,$(OBJ_DIR)/$(STEM).o)) $(subst .,\.,$(subst /,\/,$(ALL_TAGS)))/g'\
        > $(ALL_TAGS)

$(DEP_DIR)/%.d: %.cxx
	$(RM) $(ALL_TAGS)
	$(GCC) $(MKDEPFLAGS) $(CPPFLAGS) $(INCLUDES) $(1ST_DEPS) | sed -e \
's/$(notdir $(STEM))\.o/$(subst .,\.,$(subst /,\/,$(OBJ_DIR)/$(STEM).o)) $(subst .,\.,$(subst /,\/,$(ALL_TAGS)))/g'\
        > $(ALL_TAGS)



endif
DeleteDirs :
	$(RMDIR)  $(TMP_DIR)

clean :
	-$(RMDIR)  $(OUPUT_DIRS)


endif # end of DoIt

test: 
	@echo MAKE        := $(MAKE)
	@echo MAKEFLAGS   := $(MAKEFLAGS)
	@echo MAKEFILES   := $(MAKEFILES)
	@echo OUT_DIR     := $(OUT_DIR)
	@echo OUT_UP      := $(OUT_UP)
	@echo OUT_UPP     := $(OUT_UPP)
	@echo INP_DIR     := $(INP_DIR) 
	@echo PKGNAME     := $(PKGNAME) 
	@echo SRC_DIR     := $(SRC_DIR)
	@echo GEN_DIR := $(GEN_DIR) 
	@echo LIB_DIR     := $(LIB_DIR)
	@echo OBJ_DIR     := $(OBJ_DIR)
	@echo DEP_DIR     := $(DEP_DIR) 
	@echo TMP_DIR     := $(TMP_DIR)
	@echo SRC_DIR := $(SRC_DIR) 
	@echo BIN_DIR     := $(BIN_DIR) 
	@echo DOIT        := $(DOIT)
	@echo OUPUT_DIRS  := $(OUPUT_DIRS)
	@echo INPUT_DIRS  := $(INPUT_DIRS)
	@echo FILES_SRC   := $(FILES_SRC)
	@echo FILES_D     := $(FILES_D)
	@echo FILES_O     := $(FILES_O)
	@echo INCLUDES    := $(INCLUDES)
	@echo VPATH       := $(VPATH)
	@echo OSFID     := $(OSFID)

	@echo FILES_ORD := $(FILES_ORD)
	@echo FILES_SYM := $(FILES_SYM)
	@echo FILES_SYT := $(FILES_SYT)
	@echo FILES_TAB := $(FILES_TAB)
	@echo FILES_MOD := $(FILES_MOD) 

	@echo NAMES_ORD := $(NAMES_ORD) 
	@echo NAMES_SYT := $(NAMES_SYT) 
	@echo NAMES_SYM := $(NAMES_SYM) 
	@echo NAMES_TAB := $(NAMES_TAB)
	@echo NAMES_MOD := $(NAMES_MOD)


	@echo FILES_CINT_ORD := $(FILES_CINT_ORD) 
	@echo FILES_CINT_SYT := $(FILES_CINT_SYT) 
	@echo FILES_CINT_SYM := $(FILES_CINT_SYM) 
	@echo FILES_CINT_TAB := $(FILES_CINT_TAB) 
	@echo FILES_CINT_MOD := $(FILES_CINT_MOD)

	@echo FILES_ORD_H := $(FILES_ORD_H) 
	@echo FILES_SYT_H := $(FILES_SYT_H) 
	@echo FILES_SYM_H := $(FILES_SYM_H) 
	@echo FILES_TAB_H := $(FILES_TAB_H) 
	@echo FILES_MOD_H := $(FILES_MOD_H)

