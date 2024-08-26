#!/bin/bash

docker run --gpus all -it --rm -v $(pwd):/host_pwd -w /host_pwd nvcr.io/nvidia/nvhpc:23.7-devel-cuda_multi-ubuntu20.04
