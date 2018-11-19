# usage ./scripts/generate_hardsort.sh HARDSORT_MAX

cd generate
g++ -std=c++11 generate_hardsort.cpp
./a.out $1
rm a.out
mv _hardsort.cpp ../src
