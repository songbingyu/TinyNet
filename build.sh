cmake .
make 

find ./ -name CMakeCache.txt -delete
find ./ -name CMakeFiles | xargs rm -rf
find ./ -name *.cmake  | xargs rm -rf
find ./ -name Makefile  | xargs rm -r
