C++/JavaScript hybrid WebGL demo
----

Hybrid C++ and JavaScript code to build WebGL apps using [Wajic]().

## Requirement

Put `clang`, `wasm-ld` and `wasm-opt` executables to `build` folder.

## Build and Run

Run `npm run build` and serve `public` folder.

## Why not Emscripten?

- minimal dependencies (only `clang`, `wasm-ld` and `wasm-opt` required)
- no need to install full Emscripten toolchain
- no complex APIs
