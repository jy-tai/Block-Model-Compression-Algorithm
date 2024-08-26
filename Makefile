# Determine the operating system
ifeq ($(shell uname), Linux)
	OS=Linux
else ifeq ($(shell uname), Darwin)
	OS=Mac
else ifeq ($(shell uname), MINGW64_NT-10.0)
	OS=Windows
else
	$(error Unknown operating system)
endif

# Set compiler and flags
ifeq ($(OS), Linux)
	CXX=g++
else ifeq ($(OS), Mac)
	CXX=g++
else ifeq ($(OS), Windows)
	CXX=x86_64-w64-mingw32-g++
else
	$(error Unknown operating system)
endif

CXXFLAGS=-g -Wall -Wextra -std=c++17 -m64 -O2
ifeq ($(OS), Windows)
	CXXFLAGS+=--static -fopenmp
endif

# Directories
COM_DIR=common
OBJ_DIR=obj
BIN_DIR=bin
SRC_DIR=src
# SUBDIRS=$(wildcard $(SRC_DIR)/*)
# SUBDIR=$(patsubst $(SRC_DIR)/%,%,$(SUBDIRS))
SUBDIRS=$(shell find $(SRC_DIR) -type d)	# path to src/subdirectories

#########################EDIT HERE################################

#common source files
PROCESS_INPUT_SRC=$(COM_DIR)/processInput.cpp
SUMMED_AREA_SRC=$(COM_DIR)/summedArea.cpp

#object files
PROCESS_INPUT_OBJ=$(OBJ_DIR)/processInput.o
SUMMED_AREA_OBJ=$(OBJ_DIR)/summedArea.o

#src source files
#josh
DNCV1_1_SRC=$(SRC_DIR)/joshInitial/DNCv1_1.cpp

# Executable
DNCV1_1_EXEC=$(BIN_DIR)/DNCV1_1


######################## functions ################################

#compiles source file to object file
define compile_object
	@echo "Building object $@..."
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c -o $@ $^
	@echo "Successfully built object $@."
endef

define compile_executable
	@echo "Building executable $@..."
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -o $@.exe $^
	@echo "Successfully built executable $@..."
endef


######################## src file compilation ################################

$(DNCV1_1_EXEC): $(DNCV1_1_SRC) $(PROCESS_INPUT_OBJ) $(SUMMED_AREA_OBJ)
	$(call compile_executable)

$(PROCESS_INPUT_OBJ): $(PROCESS_INPUT_SRC)
	$(call compile_object)

$(SUMMED_AREA_OBJ): $(SUMMED_AREA_SRC)
	$(call compile_object)


######################### targets ###############

compile:
	mkdir -p ./bin
	./Build.sh

py-lint:
	./lint.sh py

c-lint:
	./lint.sh cpp

install:
	pip install -r requirements.txt

test:
	$(CXX) ./testing/harness/dcmpn.cpp -o ./testing/harness/test

validate: $(wildcard $(BIN_DIR)/*/*.exe)
	@for file in $(wildcard $(BIN_DIR)/*/*.exe); do \
		./validateBinary.sh -f $$file; \
	done

# Phony targets
.PHONY: all clean

all: $(PROCESS_INPUT_OBJ) $(SUMMED_AREA_OBJ) compile
#	$(DNCV1_1_EXEC)
	@for dir in $(SUBDIRS); do \
		dir_name=$$(basename $$dir); \
		cpp_files=$$dir/*.cpp; \
		for cpp_file in $$cpp_files; do \
			file_name=$$(basename $$cpp_file); \
			# echo "Filename:"; \
			# echo $$cpp_file; \
			if [ -f $$cpp_file ]; then \
				echo "Building executable $$cpp_file to $(BIN_DIR)/$$dir_name/$${file_name%.cpp}.exe"; \
				$(CXX) $(CXXFLAGS) -o $(BIN_DIR)/$$dir_name/$$(basename $$file_name .cpp).exe $$cpp_file $(PROCESS_INPUT_OBJ) $(SUMMED_AREA_OBJ); \
				echo "Successfully built executable $$cpp_file."; \
			else \
				echo "File $$cpp_file does not exist."; \
			fi; \
		done; \
	done
	$(info BIN_DIR: $(BIN_DIR))
	@echo "Successfully built all executables."

clean:
	rm -rf ./bin/*
	rm -rf ./obj
	rm -rf $(OBJ_DIR) $(BIN_DIR)