cd ..				# Projects

cd AdminPanel		# Projects/AdminPanel
mkdir build
cd build
cmake _DCMAKE_BUILD_TYPE=Release ..
make

cd ../..

cd BowlingScore_POC	# Projects/BowlingScore_POC
mkdir build
cd build
cmake _DCMAKE_BUILD_TYPE=Release ..
make

cd ../..

cd PointsGenerator # Projects/PointsGenerator
mkdir build
cd build
cmake _DCMAKE_BUILD_TYPE=Release ..
make
