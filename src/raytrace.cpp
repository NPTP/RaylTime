#include "raytrace.h"

void raytrace(
    int *&rgb_image,
    const int height,
    const int width,
    const Camera &camera,
    const std::shared_ptr<AABBTree> &root,
    const std::vector<std::shared_ptr<Light>> &lights,
    SDL_Renderer *&renderer)
{

#pragma omp parallel for // Multithread the raytraces. Note 'collapse(2)' directive requires higher version of OMP
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            // Skip every other pixel in cross-hatch pattern
            if ((y % 2 == 0 && x % 2 == 1) || (y % 2 == 1 && x % 2 == 0))
                continue;

            // Compute viewing ray
            Ray ray;
            viewing_ray(camera, y, x, width, height, ray); // Pixels i,j map to y,x

            // Shoot ray and collect color.
            Eigen::Vector3d rgb(0, 0, 0);
            // First argument tells the ray that it is direct from the viewer (not a shading ray)
            raycolor(true, ray, 1.0, root, lights, 0, rgb);

            // Put pixel colors into array
            rgb_image[0 + 3 * (x + width * y)] = (int)(255.0 * clamp(rgb(0)));
            rgb_image[1 + 3 * (x + width * y)] = (int)(255.0 * clamp(rgb(1)));
            rgb_image[2 + 3 * (x + width * y)] = (int)(255.0 * clamp(rgb(2)));
        }
    }

    // Draw to screen. Average every other pixel.
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            int r = 0 + 3 * (x + width * y);
            int g = 1 + 3 * (x + width * y);
            int b = 2 + 3 * (x + width * y);

            if ((y % 2 == 0 && x % 2 == 1) || (y % 2 == 1 && x % 2 == 0))
            {
                double r_top, g_top, b_top, r_bottom, g_bottom, b_bottom,
                    r_left, g_left, b_left, r_right, g_right, b_right;
                r_top = g_top = b_top = r_bottom = g_bottom = b_bottom =
                    r_left = g_left = b_left = r_right = g_right = b_right = 0.0;
                double num_values = 0.0; // Won't get div by 0 with prescribed resolutions

                if (y > 0)
                {
                    r_top = rgb_image[r - 3 * width];
                    g_top = rgb_image[g - 3 * width];
                    b_top = rgb_image[b - 3 * width];
                    num_values += 1.0;
                }

                if (y < height - 1)
                {
                    r_bottom = rgb_image[r + 3 * width];
                    g_bottom = rgb_image[g + 3 * width];
                    b_bottom = rgb_image[b + 3 * width];
                    num_values += 1.0;
                }

                if (x > 0)
                {
                    r_left = rgb_image[r - 3];
                    g_left = rgb_image[g - 3];
                    b_left = rgb_image[b - 3];
                    num_values += 1.0;
                }

                if (x < width - 1)
                {
                    r_right = rgb_image[r + 3];
                    g_right = rgb_image[g + 3];
                    b_right = rgb_image[b + 3];
                    num_values += 1.0;
                }

                rgb_image[r] = (r_top + r_bottom + r_left + r_right) / num_values;
                rgb_image[g] = (g_top + g_bottom + g_left + g_right) / num_values;
                rgb_image[b] = (b_top + b_bottom + b_left + b_right) / num_values;
            }

            SDL_SetRenderDrawColor(renderer,
                                   rgb_image[r],
                                   rgb_image[g],
                                   rgb_image[b],
                                   255);
            SDL_RenderDrawPoint(renderer, x, y);
        }
    }
}