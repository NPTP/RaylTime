# RaylTime

##### Simple real-time interactive ray tracer with text-based level editing

RaylTime is a simple interactive ray tracer running on the CPU and built on top of the Eigen linear algebra library. It allows you to move around raytraced scenes in real-time. It runs in a much lower resolution than other simple (usually offline) raytracers for useable performance, along with other optimizations. It features a text-based "level editing" system allowing you to quickly make new scenes to move around in.

RAYLTIME uses the following to improve performance:

- Bounding volume hierarchies (BVH)
- Parallel ray processing
- Phong specular highlights optimization based on [this paper](http://jcgt.org/published/0003/03/01/paper.pdf)
- Every-other-pixel colour averaging to reduce number of rays cast
- Modifiable draw distance (with linear intensity fall-off towards the maximum), past which no rays make BVH tree traversals

## How to run

Use CMake to build and run `rayltime`. Be sure to use a release build by setting up CMake appropriately:

```
cmake -DCMAKE_BUILD_TYPE=Release ..
```

After building, ensure the required DLL files exist inside your build folder with the executable _or it won't run!_
They can be found in the folders `Required DLLs x64` or `Required DLLs x86`, whichever is necessary for your system. Just copy them into your build folder.

Run `rayltime` from a build folder inside the repo and it will start with `defaultlevel.txt` as the default level. Other sample levels have also been provided in `levels/`. Load any level from any build location, providing the level file path to `rayltime`, e.g. :

```
rayltime ../levels/my_level.txt
```

"Levels" are grid-based scenes defined in plain text files. They can be loaded and edited, even without restarting the program. See below for instructions on how to edit and make your own.

> _Please note the following requirements:_
>
> - CMake 3.9 or higher is required. You may need to change the way targets are handled in CMakeLists.txt for non-Windows platforms.
> - C++11 compatible compiler is required.
> - OpenMP support is required for improved performance.

### Controls

- `Esc` : Quit
- `U` : Toggle UI text
- `W` : Move forward
- `S` : Move backward
- `A` : Move left
- `D` : Move right
- `LEFT ARROW` : Turn left
- `RIGHT ARROW` : Turn right
- `B` : Toggle bounding box visualization
  - `N` : Show shallower depth of bounding box tree
  - `M` : Show deeper depth of bounding box tree
- `[` : Decrease raytracing reflection recursive depth (default 2, min 0)
- `]` : Increase raytracing reflection recursive depth (max 10)
- `-` : Decrease draw distance (min 10)
- `=` : Increase draw distance (max 200)
- `1`-`5` : Change raycasting resolution from 1 (low) to 5 (high). See `set_logical_resolution()` in `main_header.h` for specific resolution sizes, easily changed.
- `R` : Reload level (will also load any changes made to level file; allows for "nearly live" editing)

Additional controls for **maximum exploration**:

- `UP ARROW` : Pitch up
- `DOWN ARROW` : Pitch down
- `Q` : Roll counter-clockwise
- `E` : Roll clockwise
- `SPACE` : Move up (relative to camera angle)
- `CTRL` : Move down (relative to camera angle)

### Level editing

Levels are grid based. Each "tile" of the grid must be filled with some object.

Levels must be rectangular; that is, they must have the same number of tiles on every row of the grid, but the number of rows can be anything and the number of tiles per row can be anything.

Each row is delimited by a line break, making it easy to visualize the grid from a "top down" view. Whitespace is ignored in the level parser, so all tiles of the grid must have some object in them to signify what's in the space. One of these objects MUST be a camera object, and there can only be one camera in the scene. Malformed levels will throw an error and quit. The smallest valid level is a single tile with a camera in it. The largest is whatever your computer can handle...

Example of a valid level:

```
BBBBBBB
B..L..B
_..N.._
B..o..B
BBBBBBB
```

This is a 5x7 level with a North-facing camera in the centre, a point light just ahead of it, a floating sphere just behind it, floor tiles inside the border and blocks all around the border except for empty holes in the walls to the left and right of the camera.

This is the tile legend for making your own levels. Note that objects and lights get random colours on each level reset.

> - `B` = High block
> - `b` = Low block
> - `M` = High mirror block
> - `m` = Low mirror block
> - `.` = Floor tile
> - `t` = Small spinning triangle
> - `T` = Large spinning triangle
> - `o` = Small floating sphere
> - `O` = Large floating sphere
> - `D` = Directional light (random direction). Position is irrelevant, affects whole scene
> - `L` = Invisible floating point light
> - `^` or `>` = Moving invisible floating point light. `^` will create a light that moves North/South. `>` will be East/West. The light will move 2 tiles away from its initial point before doubling back and covering the same distance from the start in the other direction, passing through any walls that have been placed in this range.
> - `C` = Ceiling light "fixture"
> - `F` = Floor light "fixture"
> - `N`, `S`, `E`, or `W` = Camera, with respective compass starting direction
> - `_` = Empty space (Nothing rendered in tile. Useful for fulfilling level size constraints or handling non-visible areas as it does not add any raytracing computation overhead)

Load up `rayltime`. You will see that `defaultlevel.txt` is loaded by default when no arguments are given. Try playing around with the text file, then save it and hit `R` in-program to see the results instantly! Note that reloading a level gives new random values for colours, so you can hit `R` at any time to get a new light & object colours even without editing the level.

---

## Libraries used

- **[Eigen](https://github.com/eigenteam/eigen-git-mirror)** (Linear algebra)
  - Math for raytracing & transformations
- **[SDL](https://www.libsdl.org/)** (Simple DirectMedia Layer)
  - Window
  - Keyboard input
  - No graphics API used other than setting pixel colours based on raytrace result for that pixel.
- **FINDSDL2_ttf** (Cmake module)
  - Used to link SDL2 ttf library for text UI in SDL window.
    - Required as CMake find_package() currently only supports SDL1.
    - Found at [this repo](https://github.com/aminosbh/sdl2-cmake-modules).
    - This CMake module is distributed under the OSI-approved BSD 3-Clause License. See SDL2_ttf_Copyright.txt for details.
- **OpenMP** (Multithreading)

  - Casting rays in parallel

  Thanks to [Grandoplex Produtions](http://grandchaos9000.deviantart.com/) for the "8-bit Operator" ttf font.
