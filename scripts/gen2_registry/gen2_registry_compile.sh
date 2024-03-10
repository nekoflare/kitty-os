CPP_FLAGS="-O3"
CPP="clang++"

$CPP $CPP_FLAGS gen2_registry.cpp -o gen2_reg

./gen2_reg