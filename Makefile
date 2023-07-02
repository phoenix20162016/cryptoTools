
release:
	#bazel build --cxxopt=-std=c++17 --define enable_sse=true //:libcryptoTools --sandbox_debug
	#bazel build --cxxopt=-std=c++17 --define enable_sse=true //:libcryptoTools
	#bazel build --cxxopt=-std=c++17 //:libcryptoTools
	bazel build --cxxopt=-std=c++17 //:frontend_cryptoTools
