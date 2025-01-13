# This is a very fancy makefile. Don't touch it. It works like magic, even if
# you add more .cpp files and .hpp files to this project. It also
# auto-generates dependency lists as a side effect of compilation, so you'll
# basically never need to run `make clean`. The `clean` target is still there
# in case you want to use it, though.

CXX = g++ -g
LD_FLAGS = -shared -fPIC
DEP_FLAGS = -MT $@ -MMD -MP -MF $(DEP_DIR)/$*.d -fPIC
TARGET = multiplayer
BUILD_DIR = ..

# Get the commands specific to the operating system
SHARED_OBJ = $(TARGET).so
MAKE_DIR = mkdir -p $(1)
MAKE_FILE = touch
FIND_FILES = find $(1) -regex ".*\.$(2)"
FIND_PREFIX = $(2)
REM_DIR = rm -rf
REM_FILE = rm -rf
DIR_SLASH = /
ifeq ($(OS),Windows_NT)
	SHARED_OBJ = $(TARGET).dll
	MAKE_DIR = if not exist $(1) mkdir $(1)
	MAKE_FILE = echo. >
	FIND_FILES = dir "$(1)\*.$(2)" /b
	FIND_PREFIX = $(addprefix $(1)/,$(2))
	REM_DIR = rd /s/q
	REM_FILE = del
	DIR_SLASH = \$(strip)
endif

INC_DIRS = . ../..
INC = $(addprefix -I,$(INC_DIRS))

SRC_DIR = .
SRC = $(shell $(call FIND_FILES,$(SRC_DIR),cpp))

OBJ_DIR = .obj
OBJ = $(foreach dir,$(INC_DIRS),$(call FIND_PREFIX,$(dir)/$(OBJ_DIR),\
	$(shell $(call FIND_FILES,$(dir)/$(OBJ_DIR),o)))) \
	$(patsubst %.cpp,$(OBJ_DIR)/%.o,$(SRC))

DEP_DIR = .deps
DEP = $(patsubst %.cpp,$(DEP_DIR)/%.d,$(SRC))


$(BUILD_DIR)/$(SHARED_OBJ): $(OBJ)
	$(CXX) $(LD_FLAGS) -o $@ $^

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp $(DEP_DIR)/%.d \
		$(OBJ_DIR)/%.o.sentinel $(DEP_DIR)/%.d.sentinel
	$(CXX) $(DEP_FLAGS) $(INC) -c -o $@ $<

$(OBJ_DIR)/%.sentinel:
	@$(call MAKE_DIR,${@D})
	@$(MAKE_FILE) $@

$(DEP_DIR)/%.sentinel:
	@$(call MAKE_DIR,${@D})
	@$(MAKE_FILE) $@

$(DEP):

include $(wildcard $(DEP))

clean:
	$(REM_DIR) $(OBJ_DIR) $(DEP_DIR)
	$(REM_FILE) $(BUILD_DIR)$(DIR_SLASH)$(SHARED_OBJ)

.PHONY: clean
