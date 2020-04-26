# RAYLTIME
##### Simple real-time ray tracer with text-based level editor

## Ongoing to-dos for this project (This section to be removed on completion)
- Clean up TODOs in code
- Draw closer objects first, stop when you've got a colour (do we need to bring back point squared distance??). Perhaps also set a default draw distance and add ability to change view distance (use max_t values, should increase performance, make it changeable) -> then add it to readme below. Maybe make light fall off with distance so it looks like it "fogs out" into darkness instead of just cuts off.
- Point light intensity fall-off using the t-values in blinnphong (use linear: ((-1/max_distance) * t + 1))
- UI: show "Show bounding boxes: ON/OFF", "Bounding box tree depth: _", and "Raytrace recursion depth: _".
- Support collision using the AABB tree (box around camera) (bring back box->intersect(box) code)
- One of the todos is moving sdl2.dll into the build folder automatically if it's not already there. Use the existing 32/64 env check at the top of main.cpp to know which dll to use. Helpful sample code:
```
// Check for and move SDL2.dll if necessary
std::ifstream  src("../sdl2/lib/x64/SDL2.dll", std::ios::binary);
std::ofstream  dst("SDL2.dll",   std::ios::binary);
dst << src.rdbuf();
```
- Get things on the GPU if possible, increase performance (& default resolution?)

#### "Maybe" features
- In-engine level selector at startup?
- Refraction
- Texture-mapped wall/floor/ceiling quads
- Support triangle soups/STLs ?

#### Useful links while working
- [SDL2 text display](https://stackoverflow.com/questions/22886500/how-to-render-text-in-sdl2)
- [Eigen linear solving](https://eigen.tuxfamily.org/dox/group__TutorialLinearAlgebra.html)

---
---

RAYLTIME is a simple interactive ray tracer built on top of the Eigen liner algebra library (no RTX). It allows you to move around raytraced scenes in real-time. It runs in a much lower resolution than other simple (usually offline) raytracers for useable performance, along with other optimizations. It features a text-based "level editing" system allowing you to quickly make new scenes to move around.

RAYLTIME uses the following to improve performance:
- Bounding volume hierarchies
- Parallel ray processing
- Phong specular highlights optimization based on [this paper](http://jcgt.org/published/0003/03/01/paper.pdf)
- Every-other-pixel colour averaging to reduce number of rays cast
- Runs on the GPU...? (TODO!)

## How to run

Use CMake to build and run `rayltime`.

Run `rayltime` and it will start with `defaultlevel.txt` as the default level. Other sample levels have also been provided in `levels/`. Load any level providing the level file path to `rayltime` as such:

`> build_dir/$ rayltime ../levels/my_level.txt`

"Levels" are grid-based scenes defined in plain text files. They can be loaded and edited, even without restarting the program. See below for instructions on how to edit and make your own.

> *Please note the following requirements:*
> - CMake 3.9 or higher is required. You may need to change the way targets are handled in CMakeLists.txt for non-Windows platforms.
> - C++11 compatible compiler is required.
> - OpenMP support is required.
> - After building, ensure `SDL2.dll` exists inside your build folder with the executable *or it won't run!* `SDL2.dll` will be copied by `rayltime`.

### Controls

- `Esc`: Quit
- `W` : Move forward
- `A` : Slide left
- `S` : Move backward
- `D` : Slide right
- `LEFT ARROW` : Turn left
- `RIGHT ARROW` : Turn right
- `B` : Toggle bounding box visualization
  - `N` : Show shallower depth of bounding box tree
  - `M` : Show deeper depth of bounding box tree
- `[` : Decrease raytracing reflection recursive depth (default 2, min 0)
- `]` : Increase raytracing reflection recursive depth (max 10)
- `-` : Decrease draw distance
- `=` : Increase draw distance
- `R` : Reload level (will load any changes made to level file; allows for "nearly live" editing)

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

This is a 5x7 level with a North-facing camera in the centre, a point light just ahead of it, a floating sphere just behind it, and blocks all around the border except for empty holes in the walls to the left and right of the camera.

This is the tile legend for making your own levels. Note that objects and lights get random colours on each level reset.
> - `B` = High block
> - `b` = Low block
> - `M` = High mirror block
> - `m` = Low mirror block
> - `.` = Floor tile
> - `t` = Floating spinning triangle
> - `o` = Small floating sphere
> - `O` = Large floating sphere
> - `D` = Directional light (random direction). Position is irrelevant, affects whole scene
> - `L` = Invisible floating point light
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
  - No graphics API used other than setting pixel colours based on raycast result.
- **OpenMP** (Multithreading)
  - Casting rays in parallel


