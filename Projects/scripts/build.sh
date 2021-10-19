cd Projects/AdminPanel
if [[ ! -d "build" ]]
then
	mkdir build
fi
cd build
cmake _DCMAKE_BUILD_TYPE=Release ..
make

cd ../../..

cd Projects/BowlingScore
if [[ ! -d "build" ]]
then
	mkdir build
fi
cd build
cmake _DCMAKE_BUILD_TYPE=Release ..
make

cd ../../..

cd Projects/PointsGenerator
if [[ ! -d "build" ]]
then
	mkdir build
fi
cd build
cmake _DCMAKE_BUILD_TYPE=Release ..
make
