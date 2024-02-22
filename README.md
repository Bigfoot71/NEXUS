
# NEXUS - C++ Graphic and Game Development Library

NEXUS is a powerful C++ library for developing graphical applications and games, built on top of SDL2 *(with planned support for SDL3 upon its stable release)*. While inspired by [raylib](https://www.raylib.com/), NEXUS distinguishes itself with a unique set of features and capabilities.

This is an ongoing project that is still far from a final version. I only started it in the summer of 2023 and have invested a lot of effort into it, hoping to continue improving and refining it for a long time to come. I hope you can appreciate and benefit from it! :D

## Features

- **Graphics API Support:** NEXUS provides support for OpenGL 1.1, 2.1, 3.3, 4.3, ES 2, and ES 3. Additionally, it offers a software rasterizer and plans to include Vulkan and Direct3D support in the future. Developers can choose to use one of the graphics APIs offered, the SDL's rendering system (`SDL_Renderer`) or to draw directly on the window surface, all in an object-oriented manner.

- **Image and Text Rendering:** NEXUS uses `SDL_image` for image loading and `SDL_ttf` for font rendering. It includes a complete `SDL_ttf` binding for generating text or glyphs and features Font classes with caching for rendering text on Renderer, OpenGL, and the software rasterizer.

- **Surface Manipulation:** NEXUS supports SDL surface manipulation, allowing drawing directly onto images. Pre-implemented functions make surface generation and drawing straightforward.

- **Optional Audio Support:** NEXUS provides audio management through [OpenAL Soft](https://github.com/kcat/openal-soft) and [libsndfile](https://github.com/libsndfile/libsndfile) for audio file loading. You have the option to use a simple sound playback class or a more comprehensive music playback class with asynchronous loading and playback capabilities. Both classes are derived from a base class that facilitates the application of effects and spatialization using OpenAL.

- **Optional 2D and 3D Extensions:** NEXUS offers optional extensions for 2D and 3D development, allowing simultaneous activation. Features include basic systems for sprites (2D or on billboards in 3D), 2D/3D particles, 3D lighting with support for specular mapping, bump mapping, parallax mapping, a skyboxes support, and more.

- **Optional Model Support:** NEXUS supports optional loading of 3D models with animation capabilities using [Assimp](https://github.com/assimp/assimp).

- **Optional Physics Support:** The library provides optional support for 2D physics using [Box2D](https://github.com/erincatto/box2d), 3D physics using [Bullet 3](https://github.com/bulletphysics/bullet3).

- **Optional Networking Support:** NEXUS includes a minimalist networking module with asymmetric key encryption, utilizing [ASIO](https://github.com/chriskohlhoff/asio) (*no BOOST*) and [LibSodium](https://github.com/jedisct1/libsodium).

- **Geometry and Collision:** NEXUS handles 2D polygon and 3D mesh generation with a collision shape system, where implemented geometric shapes provide collision tests for each other.

- **Math Module:** A comprehensive math module includes vectors, matrices, easing functions, and various other mathematical utilities.

- **Experimental Software Rasterizer:** NEXUS features an experimental software rasterizer supporting texture rendering, 3D model rendering, 3D model animation, software shaders, and TargetTexture. Contributions for optimization are welcome.

## Compatibility

NEXUS has been tested on Linux and Windows and is soon to be compatible with Android. Contributions for MacOS and iOS compatibility are actively sought.

## Getting Started

1. **Clone the Repository:** Start by cloning the NEXUS repository to your local machine, including all submodules:

   ```bash
   git clone --recurse-submodules https://github.com/Bigfoot71/NEXUS
   ```

   If you have already cloned the NEXUS repository without submodules, you can initialize and update the submodules separately:

   ```bash
   git submodule update --init --recursive
   ```

   Depending on your requirements, you may choose to clone only specific submodules for optional support. `SDL2`, `SDL2_image`, and `SDL2_ttf` are essential for the library and must be cloned along with NEXUS. You can clone additional submodules for optional features as needed.

   For example, to clone only the necessary submodules:

   ```bash
   git submodule update --init external/sdl2 external/sdl2_image external/sdl2_ttf
   ```

   By following these steps, you can clone the NEXUS repository with all necessary submodules and optionally select additional submodules for features you require.

2. **Set Build Options:** Open the CMakeOptions.txt file provided in the repository. This file contains various options that can be configured to customize the build process. Some of the key options include:

   - **GRAPHICS_API:** Specify the desired graphics API for rendering. Options include `GL 1.1`, `GL 2.1`, `GL 3.3`, `GL 4.3`, `GL ES2`, `GL ES3`, `OFF` for no API, or `ALL` for all available APIs *(`ALL` will only include version 3.3 for OpenGL)*.
   - **NEXUS_BUILD_SHARED:** Specify whether to build NEXUS as a shared library (`ON`) or a static library (`OFF`).
   - **NEXUS_BUILD_EXAMPLES:** Specify whether to build the example projects (`ON` or `OFF`).
   - **SUPPORT_AUDIO:** Enable optional support for audio (`ON` or `OFF`).
   - **SUPPORT_MODEL:** Enable optional support for 3D models (`ON` or `OFF`).
   - **SUPPORT_PHYSICS_2D:** Enable optional support for 2D physics (`ON` or `OFF`).
   - **SUPPORT_PHYSICS_3D:** Enable optional support for 3D physics (`ON` or `OFF`).
   - **SUPPORT_NETWORK:** Enable optional support for network (`ON` or `OFF`).
   - **EXTENSION_CORE:** Enable extensions for the nexus::core module (`ON` or `OFF`).
   - **EXTENSION_GFX:** Enable extensions for the nexus::gfx module (`ON` or `OFF`).
   - **EXTENSION_2D:** Enable 2D components extension for gfx and gapi modules (`ON` or `OFF`).
   - **EXTENSION_3D:** Enable 3D components extension for gapi modules (`ON` or `OFF`).

3. **Generate Build Files:** Use CMake to generate build files based on the specified options. Navigate to the root directory of the cloned repository and run the following commands:

   ```bash
   mkdir build
   cd build
   cmake ..
   ```

   This will generate build files in the `build` directory based on the options specified in the CMakeLists.txt file.

   **NOTE**: It is strongly recommended to use Ninja to build the library, this will greatly reduce compilation time, you can do this with the following command (*obviously make sure you have Ninja*): `cmake .. -G Ninja`

4. **Build the Library:** Once the build files have been generated, build the NEXUS library by running:

   ```bash
   cmake --build .
   ```

   This will compile the library according to the specified build options.

5. **Integrate into Your Project:** Once the library is built, you can integrate it into your own C++ projects by linking against the generated library files and including the necessary header files.

6. **Let's code:** To start programming with NEXUS, you can currently refer to the provided examples and extensively documented header files, which should be (*hopefully*) well-organized and informative. A comprehensive documentation is expected to be released in the future.

## License

NEXUS is licensed under an unmodified zlib/libpng license.
