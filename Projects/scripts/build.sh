cd Projects/AdminPanel
mkdir build
cd build
cmake _DCMAKE_BUILD_TYPE=Release ..
make

cd ../../..

cd Projects/BowlingScore_POC
mkdir build
cd build
cmake _DCMAKE_BUILD_TYPE=Release ..
make

cd ../../..

cd Projects/PointsGenerator
mkdir build
cd build
cmake _DCMAKE_BUILD_TYPE=Release ..
make