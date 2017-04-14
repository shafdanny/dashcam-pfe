
# Root directory of QPULib repository
ROOT = ./Lib

# Compiler and default flags
CXX = g++
CXX_FLAGS = -Wconversion -std=c++0x -I $(ROOT)

# Object directory
OBJ_DIR = obj

# Debug mode
ifeq ($(DEBUG), 1)
  CXX_FLAGS += -DDEBUG
  OBJ_DIR := $(OBJ_DIR)-debug
endif

# QPU or emulation mode
ifeq ($(QPU), 1)
  CXX_FLAGS += -DQPU_MODE
  OBJ_DIR := $(OBJ_DIR)-qpu
else
  CXX_FLAGS += -DEMULATION_MODE
endif

# Object files
OBJ =                         \
  Kernel.o                    \
  Source/Syntax.o             \
  Source/Int.o                \
  Source/Float.o              \
  Source/Stmt.o               \
  Source/Pretty.o             \
  Source/Translate.o          \
  Source/Interpreter.o        \
  Source/Gen.o                \
  Target/Syntax.o             \
  Target/SmallLiteral.o       \
  Target/Pretty.o             \
  Target/RemoveLabels.o       \
  Target/CFG.o                \
  Target/Liveness.o           \
  Target/RegAlloc.o           \
  Target/ReachingDefs.o       \
  Target/Subst.o              \
  Target/LiveRangeSplit.o     \
  Target/Satisfy.o            \
  Target/LoadStore.o          \
  Target/Emulator.o           \
  Target/Encode.o             \
  VideoCore/Mailbox.o         \
  VideoCore/Invoke.o          \
  VideoCore/VideoCore.o       

# Top-level targets

.PHONY: top clean

top:
	@echo Please supply a target to build, e.g. \'make GCD\'
	@echo

clean:
	rm -rf obj obj-debug obj-qpu obj-debug-qpu
	rm -f AutoTest *.o
	rm -f Encrypt
	rm -f Encrypt2
	rm -f Test
	rm -f TriviumKeygen
	rm -f TriviumKeygenCPU

LIB = $(patsubst %,$(OBJ_DIR)/%,$(OBJ))

Test: Test.o $(LIB) RPiGPUEncrypt.o
	@echo Linking...
	@$(CXX) $^ -o $@ $(CXX_FLAGS)

AutoTest: AutoTest.o $(LIB)
	@echo Linking...
	@$(CXX) $^ -o $@ $(CXX_FLAGS)

Encrypt: Encrypt.o $(LIB)
	@echo Linking...
	@$(CXX) $^ -o $@ $(CXX_FLAGS)

Encrypt2: Encrypt_optimized.o $(LIB)
	@echo Linking...
	@$(CXX) $^ -o $@ $(CXX_FLAGS)

Trivium: Trivium_encryption.o $(LIB)
	@echo Linking...
	@$(CXX) $^ -o $@ $(CXX_FLAGS)
	
Trivium_pad: Trivium_pad_encryption.o $(LIB)
	@echo Linking...
	@$(CXX) $^ -o $@ $(CXX_FLAGS)
	
TriviumKeygen: TriviumKeygen.o GPUEncryption.o trivium.o utils.o $(LIB)
	@echo Linking...
	@$(CXX) $^ -o $@ $(CXX_FLAGS) -pthread
    
TriviumKeygenCPU: TriviumKeygenCPU.o CPUEncryption.o GPUEncryption.o trivium.o utils.o $(LIB)
	@echo Linking...
	@$(CXX) $^ -o $@ $(CXX_FLAGS) -pthread

# Intermediate targets

$(OBJ_DIR)/%.o: $(ROOT)/%.cpp $(OBJ_DIR)
	@echo Compiling $<
	@$(CXX) -c -o $@ $< $(CXX_FLAGS)

%.o: %.cpp
	@echo Compiling $<
	@$(CXX) -c -o $@ $< $(CXX_FLAGS)

$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)
	@mkdir -p $(OBJ_DIR)/Source
	@mkdir -p $(OBJ_DIR)/Target
	@mkdir -p $(OBJ_DIR)/VideoCore
