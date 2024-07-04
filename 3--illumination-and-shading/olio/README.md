## DEPENDENCIES

* Eigen3
* Boost
* TBB
* OpenMesh 9.0+ (https://www.openmesh.org/)
* jemalloc (Linux only)
* spdlog
* OpenCV
* glm
* glfw
* GLEW

### Installing Dependencies on Ubuntu (20.04):
#### Set Environment Variables:
```
export OLIO_THIRD_PARTY=~/olio_third_party
```

#### Install Dependencies with apt
```
sudo apt install build-essential cmake clang pkg-config libeigen3-dev libtbb-dev libjemalloc-dev libspdlog-dev libboost-dev libboost-program-options-dev libboost-filesystem-dev libboost-system-dev libboost-regex-dev libboost-thread-dev libopencv-dev libglm-dev libglfw3-dev libglew-dev
```

#### Build Rest of Dependencies
```
# OpenMesh
mkdir -p ${OLIO_THIRD_PARTY}/OpenMesh
cd ${OLIO_THIRD_PARTY}/OpenMesh
wget https://www.graphics.rwth-aachen.de/media/openmesh_static/Releases/9.0/OpenMesh-9.0.tar.gz
tar zxf OpenMesh-9.0.tar.gz
cd OpenMesh-9.0.0
mkdir build-release; cd build-release
cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_EXPORT_COMPILE_COMMANDS=1 ..
make
sudo make install
```


### Installing Dependencies on MacOS:
First, install Xcode from the App Store and Homebrew from https://brew.sh.

#### Set Environment Variables:
```
export OLIO_THIRD_PARTY=~/olio_third_party
```

#### Install Dependencies with apt
```
brew install cmake llvm pkg-config boost eigen tbb opencv spdlog glm glfw glew wget
```

#### Build Rest of Dependencies
```
# OpenMesh
mkdir -p ${OLIO_THIRD_PARTY}/OpenMesh
cd ${OLIO_THIRD_PARTY}/OpenMesh
wget https://www.graphics.rwth-aachen.de/media/openmesh_static/Releases/9.0/OpenMesh-9.0.tar.gz
tar zxf OpenMesh-9.0.tar.gz
cd OpenMesh-9.0.0
mkdir build-release; cd build-release
cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_EXPORT_COMPILE_COMMANDS=1 ..
make
sudo make install
```

## Build Olio
```
cd olio
mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
make
```
