CUDA_DIR = /opt/cuda
NVCC=$(CUDA_DIR)/bin/nvcc
LIBS=$(CUDA_DIR)/lib64
INCL=$(CUDA_DIR)/include

SRC=function_cuda.cu
OUT=libfunction_cuda.so
OPT=--compiler-options '-fPIC' -shared

all:
	$(NVCC) $(OPT) $(SRC) -o $(OUT) -L$(LIBS) -I$(INCL) 
