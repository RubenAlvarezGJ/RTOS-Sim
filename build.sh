set -e
mkdir -p build
cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
cmake --build .
echo -e "\nBuild complete."
echo "Run simulator with: ./build/rtos_kernel_sim"
echo "Run tests with:     ./build/tests"