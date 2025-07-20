set -e
mkdir -p build
cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
cmake --build .
echo -e "\n Build complete."
echo "Run simulator with: ./rtos_kernel_sim"
echo "Run tests with:     ./tests"