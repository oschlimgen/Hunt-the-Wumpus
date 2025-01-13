# This is a very fancy makefile. Don't touch it. It works like magic, even if
# you add more .cpp files and .hpp files to this project. It also
# auto-generates dependency lists as a side effect of compilation, so you'll
# basically never need to run `make clean`. The `clean` target is still there
# in case you want to use it, though.


CXX = g++ -g
DEP_FLAGS = -MT $@ -MMD -MP -MF $(DEP_DIR)/$*.d -fPIC
TARGET = wumpus

EXCLUDE_SUBDIRS = mods

# Get the commands specific to the operating system
LD_FLAGS = -ldl -lncurses
EXE = $(TARGET)
MAKE_DIR = mkdir -p $(1)
MAKE_FILE = touch
SEARCH_EXCLUDE = $(patsubst %,-not -path "*/%/*",$(EXCLUDE_SUBDIRS))
FIND_CPP = find $(SRC_DIR) $(SEARCH_EXCLUDE) -regex ".*\.cpp"
REM_DIR = rm -rf
REM_FILE = rm -rf
ifeq ($(OS),Windows_NT)
	LD_FLAGS =
	EXE = $(TARGET).exe
	MAKE_DIR = if not exist $(1) mkdir $(1)
	MAKE_FILE = echo. >
	ifdef EXCLUDE_SUBDIRS
		SEARCH_EXCLUDE = | findstr /v $(EXCLUDE_SUBDIRS)
	else
		SEARCH_EXCLUDE =
	endif
	FIND_CPP = dir /b/s "*.cpp" $(SEARCH_EXCLUDE)
	REM_DIR = rd /s/q
	REM_FILE = del
endif

SRC_DIR = .
SRC = $(shell $(FIND_CPP))

OBJ_DIR = .obj
OBJ = $(patsubst %.cpp,$(OBJ_DIR)/%.o,$(notdir $(SRC)))

DEP_DIR = .deps
DEP = $(patsubst %.cpp,$(DEP_DIR)/%.d,$(notdir $(SRC)))


$(TARGET): $(OBJ)
	$(CXX) $(LD_FLAGS) -o $@ $^

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp $(DEP_DIR)/%.d $(OBJ_DIR)/%.o.sentinel $(DEP_DIR)/%.d.sentinel
	$(CXX) $(DEP_FLAGS) -I . -c -o $@ $<

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
	$(REM_FILE) $(EXE)

.PHONY: clean
