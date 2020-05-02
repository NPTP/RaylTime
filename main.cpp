#include "main_header.h"
#ifdef _WIN64
#define ENV64
#elif _WIN32
#define ENV32
#else
if (sizeof(void *) == 8)
#define ENV64
    else if (sizeof(void *) == 4)
#define ENV32
        else
#error "ENV NOT DEFINED (32 OR 64 BIT)"
#endif

//Screen dimension constants (not pixel resolution)
#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720
// Movement constants
#define STEP_DISTANCE 0.25
#define ROT_ANGLE (M_PI / 96)
// Default starting raytrace recursion depth
#define DEFAULT_RAYTRACE_RECURSION_DEPTH 2
#define MAX_RAYTRACE_RECURSION_DEPTH 10
// Draw distance constants
#define DEFAULT_DRAW_DIST 75
#define MIN_DRAW_DIST 10
#define MAX_DRAW_DIST 200
// Global variables for technical settings
int G_aabb_tree_height = 0;
uint8_t G_show_boxes = 0x00;
int G_show_boxes_depth = 0;
int G_raytrace_recursion_depth = DEFAULT_RAYTRACE_RECURSION_DEPTH;
int G_max_t_draw_distance = DEFAULT_DRAW_DIST;

/* Main event loop */
int main(int argc, char *argv[])
{
    // Initialize the camera, objects and lights from a txt level
    Camera camera;
    std::vector<std::shared_ptr<Object>> objects;
    std::vector<std::shared_ptr<Light>> lights;
    std::shared_ptr<AABBTree> root;
    std::vector<std::shared_ptr<Animator>> animators;
    if (!read_and_reset_level(argc, argv, camera, objects, lights, root, animators))
        return -1;

    // Initialize SDL, renderer, and window
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Init(SDL_INIT_VIDEO);
    SDL_CreateWindowAndRenderer(
        WINDOW_WIDTH,
        WINDOW_HEIGHT,
        0, // SDL_WINDOW_RESIZABLE || SDL_WINDOW_INPUT_GRABBED,
        &window,
        &renderer);
    int rt_width, rt_height;
    int *rgb_image = new int[1]; // Initialize so rgb_image doesn't point at anything important
    set_logical_resolution(rgb_image, renderer, rt_width, rt_height, '3');

    SDL_Event event;
    bool quit = false;
    bool turning_left = false;
    bool turning_right = false;
    bool moving_forward = false;
    bool moving_backward = false;
    bool moving_left = false;
    bool moving_right = false;
    bool pitching_up = false;
    bool pitching_down = false;
    bool rolling_left = false;
    bool rolling_right = false;
    bool moving_up = false;
    bool moving_down = false;
    bool check_collision = false;

    while (!quit)
    {
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
            case SDL_KEYDOWN:
                switch (event.key.keysym.sym)
                {
                    /* [ESC] Quit */
                case SDLK_ESCAPE:
                    quit = true;
                    break;
                    /* [UP ARROW] Pitch down */
                case SDLK_UP:
                    pitching_down = true;
                    break;
                    /* [DOWN ARROW] Pitch up */
                case SDLK_DOWN:
                    pitching_up = true;
                    break;
                    /* [LEFT ARROW] Yaw left */
                case SDLK_LEFT:
                    turning_left = true;
                    break;
                    /* [RIGHT ARROW] Yaw right*/
                case SDLK_RIGHT:
                    turning_right = true;
                    break;
                    /* [Q] Roll CCW */
                case SDLK_q:
                    rolling_left = true;
                    break;
                    /* [E] Roll CW */
                case SDLK_e:
                    rolling_right = true;
                    break;
                    /* [W] Move forward */
                case SDLK_w:
                    moving_forward = true;
                    break;
                    /* [S] Move backward */
                case SDLK_s:
                    moving_backward = true;
                    break;
                    /* [A] Step left */
                case SDLK_a:
                    moving_left = true;
                    break;
                    /* [D] Step right */
                case SDLK_d:
                    moving_right = true;
                    break;
                    /* [CTRL] Move downwards */
                case SDLK_RCTRL:
                case SDLK_LCTRL:
                    moving_down = true;
                    break;
                    /* [SPACE] Move upwards */
                case SDLK_SPACE:
                    moving_up = true;
                    break;
                    /* [R] Re-load level/scene (allows changes) */
                case SDLK_r:
                    if (!read_and_reset_level(argc, argv, camera, objects, lights, root, animators))
                        return -1;
                    break;
                    /* [B] Visualize bounding boxes around objects in scene. */
                case SDLK_b:
                    G_show_boxes = G_show_boxes ^ 0x01;
                    break;
                    /* [N] Reduce AABB tree visualization depth. */
                case SDLK_n:
                    if (G_show_boxes && G_show_boxes_depth > 0)
                        G_show_boxes_depth--;
                    break;
                    /* [M] Increase AABB tree visualization depth. */
                case SDLK_m:
                    if (G_show_boxes && G_show_boxes_depth < G_aabb_tree_height)
                        G_show_boxes_depth++;
                    break;
                    /* [[] Reduce raytracing recursion depth. */
                case SDLK_LEFTBRACKET:
                    if (G_raytrace_recursion_depth > 0)
                        G_raytrace_recursion_depth--;
                    break;
                    /* []] Increase raytracing recursion depth. */
                case SDLK_RIGHTBRACKET:
                    if (G_raytrace_recursion_depth < MAX_RAYTRACE_RECURSION_DEPTH)
                        G_raytrace_recursion_depth++;
                    break;
                    /* [-] Reduce parametric draw distance. */
                case SDLK_MINUS:
                    if (G_max_t_draw_distance > MIN_DRAW_DIST)
                        G_max_t_draw_distance--;
                    break;
                    /* [=] Increase parametric draw distance. */
                case SDLK_EQUALS:
                    if (G_max_t_draw_distance < MAX_DRAW_DIST)
                        G_max_t_draw_distance++;
                    break;
                    /* [1-5] Set pixel resolution */
                case SDLK_1:
                    set_logical_resolution(rgb_image, renderer, rt_width, rt_height, '1');
                    break;
                case SDLK_2:
                    set_logical_resolution(rgb_image, renderer, rt_width, rt_height, '2');
                    break;
                case SDLK_3:
                    set_logical_resolution(rgb_image, renderer, rt_width, rt_height, '3');
                    break;
                case SDLK_4:
                    set_logical_resolution(rgb_image, renderer, rt_width, rt_height, '4');
                    break;
                case SDLK_5:
                    set_logical_resolution(rgb_image, renderer, rt_width, rt_height, '5');
                    break;
                default:
                    break;
                }
                break;

                // Key releases
            case SDL_KEYUP:
                switch (event.key.keysym.sym)
                {
                case SDLK_LEFT:
                    turning_left = false;
                    break;
                case SDLK_RIGHT:
                    turning_right = false;
                    break;
                case SDLK_w:
                    moving_forward = false;
                    break;
                case SDLK_s:
                    moving_backward = false;
                    break;
                case SDLK_a:
                    moving_left = false;
                    break;
                case SDLK_d:
                    moving_right = false;
                    break;
                case SDLK_UP:
                    pitching_down = false;
                    break;
                case SDLK_DOWN:
                    pitching_up = false;
                    break;
                case SDLK_q:
                    rolling_left = false;
                    break;
                case SDLK_e:
                    rolling_right = false;
                    break;
                case SDLK_RCTRL:
                case SDLK_LCTRL:
                    moving_down = false;
                    break;
                case SDLK_SPACE:
                    moving_up = false;
                    break;
                default:
                    break;
                }
                break;

            case SDL_QUIT:
                quit = true;
                break;

            default:
                break;
            }
        }

        // Actual movement branches

        // Non-colliding movements
        if (turning_left)
            change_yaw(camera, ROT_ANGLE);
        else if (turning_right)
            change_yaw(camera, -ROT_ANGLE);
        if (pitching_down)
            change_pitch(camera, -ROT_ANGLE);
        else if (pitching_up)
            change_pitch(camera, ROT_ANGLE);
        if (rolling_left)
            change_roll(camera, ROT_ANGLE);
        else if (rolling_right)
            change_roll(camera, -ROT_ANGLE);

        // Colliding movements
        Eigen::Vector3d initial_pos = camera.e;
        Eigen::Vector3d min_corner = camera.box.min_corner;
        Eigen::Vector3d max_corner = camera.box.max_corner;
        if (moving_forward)
        {
            move_camera(camera, "forward", STEP_DISTANCE);
            check_collision = true;
        }
        else if (moving_backward)
        {
            move_camera(camera, "backward", STEP_DISTANCE);
            check_collision = true;
        }
        if (moving_right)
        {
            move_camera(camera, "right", STEP_DISTANCE);
            check_collision = true;
        }
        else if (moving_left)
        {
            move_camera(camera, "left", STEP_DISTANCE);
            check_collision = true;
        }
        if (moving_down)
        {
            move_camera(camera, "down", STEP_DISTANCE);
            check_collision = true;
        }
        else if (moving_up)
        {
            move_camera(camera, "up", STEP_DISTANCE);
            check_collision = true;
        }

        // Short-circuit evaluation allows us to skip collision detection cost
        // when no viewer movements have been made.
        if (check_collision && collision_detect(camera.box, root))
        {
            camera.e = initial_pos;
            camera.box.min_corner = min_corner;
            camera.box.max_corner = max_corner;
        }

        check_collision = false;

        // Animate, raytrace and push to screen.
        animate_animators(animators);
        raytrace(rgb_image, rt_height, rt_width, camera, root, lights, renderer);
        SDL_RenderPresent(renderer);
    }

    // On a quit event, shut it all down.
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
