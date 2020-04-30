#ifndef READ_LEVEL_H
#define READ_LEVEL_H

#include <vector>
#include <string>
#include <memory>

/*
Read a level description from a text file
*/
inline bool read_level(
    const std::string &filename,
    Camera &camera,
    std::vector<std::shared_ptr<Object>> &objects,
    std::vector<std::shared_ptr<Light>> &lights);

// Implementation below
#include "Object.h"
#include "Sphere.h"
#include "SphereAnim.h"
#include "Triangle.h"
#include "TriangleAnim.h"
#include "Quad.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "Material.h"
#include <Eigen/Geometry>
#include <fstream>
#include <iostream>
#include <cassert>
#include <random>

std::uniform_real_distribution<double> unif(0.0, 1.0);
std::default_random_engine re;

// Level scale constants
const int LEVEL_HEIGHT = 8;
const int UNIT_SIZE = 8;
const double POINT_LIGHT_RADIUS = 5.5 * (double)UNIT_SIZE;
const std::string FLAT = "FLAT";
const std::string REFLECTIVE = "REFLECTIVE";

void set_random_material(std::shared_ptr<Material> &mat, const std::string &type);
std::shared_ptr<Quad> new_floor_quad(int &u, int &w);
std::shared_ptr<Quad> new_light_quad(const char &c, int &u, int &w, Eigen::Vector3d colour);
void push_new_block(std::vector<std::shared_ptr<Object>> &objects, const char &c, int &u, int &w);
void push_new_light_block(std::vector<std::shared_ptr<Object>> &objects, std::vector<std::shared_ptr<Light>> &lights, const char &c, int &u, int &w);
void parse_char(const char &c, const int &y_pos, const int &x_pos, Camera &camera, std::vector<std::shared_ptr<Object>> &objects, std::vector<std::shared_ptr<Light>> &lights);

inline bool read_level(
    const std::string &filename,
    Camera &camera,
    std::vector<std::shared_ptr<Object>> &objects,
    std::vector<std::shared_ptr<Light>> &lights)
{
    objects.clear();
    objects.shrink_to_fit();
    lights.clear();
    lights.shrink_to_fit();

    std::ifstream infile(filename);
    if (!infile)
        return false;

    std::string s;
    std::getline(infile, s);
    infile.clear();
    infile.seekg(0, std::ios::beg);
    const int LEVEL_WIDTH = s.length();

    int num_lines = 0;
    while (std::getline(infile, s))
    {
        if (s.length() != LEVEL_WIDTH)
            return false;
        num_lines++;
    }
    const int LEVEL_LENGTH = num_lines;
    infile.clear();
    infile.seekg(0, std::ios::beg);

    int num_lights = 0;
    bool level_has_camera = false;
    for (int i = 0; i < LEVEL_LENGTH; i++)
    {
        infile >> s;

        for (int j = 0; j < LEVEL_WIDTH; j++)
        {
            if (s[j] == 'N' || s[j] == 'S' || s[j] == 'E' || s[j] == 'W')
            {
                if (!level_has_camera)
                    level_has_camera = true;
                else
                    return false; // Second camera found
            }
            else if (s[j] == 'L' || s[j] == 'D')
                num_lights++;
            parse_char(s[j], i, j, camera, objects, lights);
        }
    }
    if (!level_has_camera)
        return false;

    infile.close();

    // Apply tau constant for blinn-phong optimization.
    // Technique referenced from Linus Kallberg & Thomas Larsson @ Malardalen University
    double phong_epsilon;
    if (num_lights)
        phong_epsilon = 0.001 / (double)num_lights;
    for (int k = 0; k < objects.size(); k++)
    {
        Eigen::Vector3d ks = objects[k]->material->ks;
        double alpha = objects[k]->material->phong_exponent;          // Potential div-by-0
        double cy = 0.2126 * ks[0] + 0.7152 * ks[1] + 0.0722 * ks[2]; // Potential div-by-0
        if (num_lights && cy > 0.0)
            objects[k]->material->tau = pow(phong_epsilon / cy, 1.0 / alpha);
        else
            objects[k]->material->tau = 100.0; // If there are no lights, this makes sure we never bother with specularity for this object.
    }

    // Successful level parse and materials setup.
    return true;
}

void parse_char(const char &c,
                const int &y_pos,
                const int &x_pos,
                Camera &camera,
                std::vector<std::shared_ptr<Object>> &objects,
                std::vector<std::shared_ptr<Light>> &lights)
{
    int U = x_pos * UNIT_SIZE;
    int V = UNIT_SIZE * 0.5;
    int W = y_pos * UNIT_SIZE;

    if (c == '_') // EMPTY SPACE
    {
        // Do nothing, leave space empty
    }
    else if (c == 'B' || c == 'b' || c == 'M' || c == 'm') // BLOCK
    {
        push_new_block(objects, c, U, W);
    }
    else if (c == '.') // FLOOR QUAD
    {
        objects.push_back(new_floor_quad(U, W));
    }
    else if (c == 't' || c == 'T') // TRIANGLE ANIMATOR
    {
        double circumradius;
        if (c == 't')
            circumradius = 0.25;
        else
            circumradius = 0.5;

        std::shared_ptr<TriangleAnim> triangleanim(
            new TriangleAnim(
                Eigen::Vector3d(U, V, W),
                circumradius,
                LEVEL_HEIGHT,
                UNIT_SIZE));
        set_random_material(triangleanim->material, "REFLECTIVE");
        objects.push_back(triangleanim);
        objects.push_back(new_floor_quad(U, W));
    }
    else if (c == 'o' || c == 'O') // SPHERE ANIMATOR
    {
        int radius;
        if (c == 'o')
            radius = UNIT_SIZE / 8;
        else
            radius = UNIT_SIZE / 4;

        std::shared_ptr<SphereAnim> sphere(new SphereAnim(Eigen::Vector3d(U, V, W), radius, LEVEL_HEIGHT));
        set_random_material(sphere->material, "REFLECTIVE");
        objects.push_back(sphere);
        objects.push_back(new_floor_quad(U, W));
    }
    else if (c == 'L') // INVISIBLE POINT LIGHT
    {
        std::shared_ptr<PointLight> light(new PointLight(POINT_LIGHT_RADIUS));
        light->p = Eigen::Vector3d(U, V, W);
        light->I = Eigen::Vector3d(0.2 + unif(re) * 0.8, 0.2 + unif(re) * 0.8, 0.2 + unif(re) * 0.8);
        lights.push_back(light);
        objects.push_back(new_floor_quad(U, W));
    }
    else if (c == 'D') // RANDOM DIRECTIONAL LIGHT
    {
        std::shared_ptr<DirectionalLight> light(new DirectionalLight());
        light->light_direction = Eigen::Vector3d(unif(re), unif(re), unif(re)).normalized();
        light->I = Eigen::Vector3d(0.2 + unif(re) * 0.8, 0.2 + unif(re) * 0.8, 0.2 + unif(re) * 0.8);
        lights.push_back(light);
        objects.push_back(new_floor_quad(U, W));
    }
    else if (c == 'F' || c == 'C') // FLOOR/CEILING LIGHT
    {
        push_new_light_block(objects, lights, c, U, W);
        if (c == 'C')
            objects.push_back(new_floor_quad(U, W));
    }
    else if (c == 'N' || c == 'S' || c == 'E' || c == 'W') // CAMERA
    {
        objects.push_back(new_floor_quad(U, W));
        camera.d = 1.0;
        camera.e = Eigen::Vector3d(U, UNIT_SIZE * 0.4, W);
        camera.v = Eigen::Vector3d(0, 1, 0);
        camera.height = 1.0;
        camera.width = 1.7777777778;
        switch (c)
        {
        case 'N':
            camera.u = Eigen::Vector3d(1, 0, 0);
            camera.w = Eigen::Vector3d(0, 0, 1);
            break;
        case 'S':
            camera.u = Eigen::Vector3d(-1, 0, 0);
            camera.w = Eigen::Vector3d(0, 0, -1);
            break;
        case 'E':
            camera.u = Eigen::Vector3d(0, 0, 1);
            camera.w = Eigen::Vector3d(-1, 0, 0);
            break;
        case 'W':
            camera.u = Eigen::Vector3d(0, 0, -1);
            camera.w = Eigen::Vector3d(1, 0, 0);
            break;
        default:
            camera.u = Eigen::Vector3d(1, 0, 0);
            camera.w = Eigen::Vector3d(0, 0, 1);
            break;
        }
    }
}

void set_random_material(std::shared_ptr<Material> &mat, const std::string &type)
{
    std::shared_ptr<Material> new_mat(new Material());
    new_mat->ka = Eigen::Vector3d(unif(re), unif(re), unif(re));
    new_mat->kd = Eigen::Vector3d(unif(re), unif(re), unif(re));

    if (type == "REFLECTIVE")
    {
        new_mat->ks = Eigen::Vector3d(unif(re), unif(re), unif(re));
        new_mat->km = Eigen::Vector3d(0.5 + unif(re) * 0.5, 0.5 + unif(re) * 0.5, 0.5 + unif(re) * 0.5);
        new_mat->phong_exponent = 100; // 1000 + unif(re) * 1000;
    }
    else if (type == "FLAT")
    {
        new_mat->ks = Eigen::Vector3d(unif(re) * 0.1, unif(re) * 0.1, unif(re) * 0.1);
        new_mat->km = Eigen::Vector3d(0, 0, 0);
        new_mat->phong_exponent = unif(re);
    }
    mat = new_mat;
}

// Return new floor quad
std::shared_ptr<Quad> new_floor_quad(int &u, int &w)
{
    std::tuple<Eigen::Vector3d, Eigen::Vector3d, Eigen::Vector3d> floor_quad_tuple =
        std::make_tuple(
            Eigen::Vector3d(u - UNIT_SIZE / 2, 0, w + UNIT_SIZE / 2),
            Eigen::Vector3d(u + UNIT_SIZE / 2, 0, w + UNIT_SIZE / 2),
            Eigen::Vector3d(u - UNIT_SIZE / 2, 0, w - UNIT_SIZE / 2));
    std::shared_ptr<Quad> floor_quad(new Quad(floor_quad_tuple));
    set_random_material(floor_quad->material, "FLAT");
    return floor_quad;
}

std::shared_ptr<Quad> new_ceiling_quad(int &u, int &w)
{
    std::tuple<Eigen::Vector3d, Eigen::Vector3d, Eigen::Vector3d> ceiling_quad_tuple =
        std::make_tuple(
            Eigen::Vector3d(u - UNIT_SIZE / 2, LEVEL_HEIGHT, w + UNIT_SIZE / 2),
            Eigen::Vector3d(u + UNIT_SIZE / 2, LEVEL_HEIGHT, w + UNIT_SIZE / 2),
            Eigen::Vector3d(u - UNIT_SIZE / 2, LEVEL_HEIGHT, w - UNIT_SIZE / 2));
    std::shared_ptr<Quad> ceiling_quad(new Quad(ceiling_quad_tuple));
    set_random_material(ceiling_quad->material, "FLAT");
    return ceiling_quad;
}

std::shared_ptr<Quad> new_light_quad(const char &c, int &u, int &w, Eigen::Vector3d colour)
{
    std::tuple<Eigen::Vector3d, Eigen::Vector3d, Eigen::Vector3d> light_quad_tuple;
    if (c == 'F')
    {
        light_quad_tuple = std::make_tuple(
            Eigen::Vector3d(u - UNIT_SIZE / 2, 0, w + UNIT_SIZE / 2),
            Eigen::Vector3d(u + UNIT_SIZE / 2, 0, w + UNIT_SIZE / 2),
            Eigen::Vector3d(u - UNIT_SIZE / 2, 0, w - UNIT_SIZE / 2));
    }
    else if (c == 'C')
    {
        light_quad_tuple = std::make_tuple(
            Eigen::Vector3d(u - UNIT_SIZE / 2, LEVEL_HEIGHT, w + UNIT_SIZE / 2),
            Eigen::Vector3d(u + UNIT_SIZE / 2, LEVEL_HEIGHT, w + UNIT_SIZE / 2),
            Eigen::Vector3d(u - UNIT_SIZE / 2, LEVEL_HEIGHT, w - UNIT_SIZE / 2));
    }
    std::shared_ptr<Quad> light_quad(new Quad(light_quad_tuple));
    std::shared_ptr<Material> light_mat(new Material());
    light_mat->ka = colour;
    light_mat->kd = Eigen::Vector3d(1.0, 1.0, 1.0);
    light_mat->ks = colour;
    light_mat->km = Eigen::Vector3d(0.0, 0.0, 0.0); // Eigen::Vector3d(0.25, 0.25, 0.25);
    light_mat->phong_exponent = 100;
    light_mat->is_light = true;
    light_quad->material = light_mat;

    return light_quad;
}

// Creates a light block slightly protruding from either the 'F'loor or the 'C'eiling.
void push_new_light_block(std::vector<std::shared_ptr<Object>> &objects, std::vector<std::shared_ptr<Light>> &lights, const char &c, int &u, int &w)
{
    std::shared_ptr<PointLight> light(new PointLight(POINT_LIGHT_RADIUS));
    light->I = Eigen::Vector3d(0.2 + unif(re) * 0.8, 0.2 + unif(re) * 0.8, 0.2 + unif(re) * 0.8);

    double light_length = LEVEL_HEIGHT * 0.15;
    double side_start_height;
    if (c == 'F')
    {
        side_start_height = 0;
        light->p = Eigen::Vector3d(u, side_start_height + 0.01, w);
    }
    else
    {
        side_start_height = LEVEL_HEIGHT - light_length;
        light->p = Eigen::Vector3d(u, LEVEL_HEIGHT - 0.01, w);
    }

    // Light quad
    lights.push_back(light);
    objects.push_back(new_light_quad(c, u, w, light->I));

    // Sides of protruding light
    std::tuple<Eigen::Vector3d, Eigen::Vector3d, Eigen::Vector3d> qt_n =
        std::make_tuple(
            Eigen::Vector3d(u - UNIT_SIZE / 2, side_start_height, w - UNIT_SIZE / 2),
            Eigen::Vector3d(u + UNIT_SIZE / 2, side_start_height, w - UNIT_SIZE / 2),
            Eigen::Vector3d(u - UNIT_SIZE / 2, side_start_height + light_length, w - UNIT_SIZE / 2));
    std::tuple<Eigen::Vector3d, Eigen::Vector3d, Eigen::Vector3d> qt_s =
        std::make_tuple(
            Eigen::Vector3d(u - UNIT_SIZE / 2, side_start_height, w + UNIT_SIZE / 2),
            Eigen::Vector3d(u + UNIT_SIZE / 2, side_start_height, w + UNIT_SIZE / 2),
            Eigen::Vector3d(u - UNIT_SIZE / 2, side_start_height + light_length, w + UNIT_SIZE / 2));
    std::tuple<Eigen::Vector3d, Eigen::Vector3d, Eigen::Vector3d> qt_e =
        std::make_tuple(
            Eigen::Vector3d(u + UNIT_SIZE / 2, side_start_height, w + UNIT_SIZE / 2),
            Eigen::Vector3d(u + UNIT_SIZE / 2, side_start_height, w - UNIT_SIZE / 2),
            Eigen::Vector3d(u + UNIT_SIZE / 2, side_start_height + light_length, w + UNIT_SIZE / 2));
    std::tuple<Eigen::Vector3d, Eigen::Vector3d, Eigen::Vector3d> qt_w =
        std::make_tuple(
            Eigen::Vector3d(u - UNIT_SIZE / 2, side_start_height, w - UNIT_SIZE / 2),
            Eigen::Vector3d(u - UNIT_SIZE / 2, side_start_height, w + UNIT_SIZE / 2),
            Eigen::Vector3d(u - UNIT_SIZE / 2, side_start_height + light_length, w - UNIT_SIZE / 2));
    std::shared_ptr<Quad> quad_n(new Quad(qt_n));
    std::shared_ptr<Quad> quad_s(new Quad(qt_s));
    std::shared_ptr<Quad> quad_e(new Quad(qt_e));
    std::shared_ptr<Quad> quad_w(new Quad(qt_w));

    set_random_material(quad_n->material, "FLAT");
    set_random_material(quad_s->material, "FLAT");
    set_random_material(quad_e->material, "FLAT");
    set_random_material(quad_w->material, "FLAT");

    objects.push_back(quad_n);
    objects.push_back(quad_s);
    objects.push_back(quad_e);
    objects.push_back(quad_w);
}

// Insert a new block into 'objects'.
void push_new_block(std::vector<std::shared_ptr<Object>> &objects, const char &c, int &u, int &w)
{
    int block_height = LEVEL_HEIGHT;
    if (c == 'b' || c == 'm') // Half/small block
    {
        block_height = LEVEL_HEIGHT * 0.25;
        // Make quad for top of small block
        std::tuple<Eigen::Vector3d, Eigen::Vector3d, Eigen::Vector3d> qt_top =
            std::make_tuple(
                Eigen::Vector3d(u - UNIT_SIZE / 2, block_height, w + UNIT_SIZE / 2),
                Eigen::Vector3d(u + UNIT_SIZE / 2, block_height, w + UNIT_SIZE / 2),
                Eigen::Vector3d(u - UNIT_SIZE / 2, block_height, w - UNIT_SIZE / 2));
        std::shared_ptr<Quad> quad_top(new Quad(qt_top));
        if (c == 'm')
        {
            std::shared_ptr<Material> mirror_mat(new Material());
            mirror_mat->ka = Eigen::Vector3d(0.8, 0.8, 0.8);
            mirror_mat->kd = Eigen::Vector3d(0.8, 0.8, 0.8);
            mirror_mat->ks = Eigen::Vector3d(0.8, 0.8, 0.8);
            mirror_mat->km = Eigen::Vector3d(0.999, 0.999, 0.999);
            mirror_mat->phong_exponent = 20;
            quad_top->material = mirror_mat;
        }
        else
        {
            set_random_material(quad_top->material, "FLAT");
        }
        objects.push_back(quad_top);
    }

    // TODO: Check that all quads have the correct orientation/normals. N&S should, but test all and fix all anyway
    std::tuple<Eigen::Vector3d, Eigen::Vector3d, Eigen::Vector3d> qt_n =
        std::make_tuple(
            Eigen::Vector3d(u + UNIT_SIZE / 2, 0, w - UNIT_SIZE / 2),
            Eigen::Vector3d(u - UNIT_SIZE / 2, 0, w - UNIT_SIZE / 2),
            Eigen::Vector3d(u + UNIT_SIZE / 2, block_height, w - UNIT_SIZE / 2));
    std::tuple<Eigen::Vector3d, Eigen::Vector3d, Eigen::Vector3d> qt_s =
        std::make_tuple(
            Eigen::Vector3d(u - UNIT_SIZE / 2, 0, w + UNIT_SIZE / 2),
            Eigen::Vector3d(u + UNIT_SIZE / 2, 0, w + UNIT_SIZE / 2),
            Eigen::Vector3d(u - UNIT_SIZE / 2, block_height, w + UNIT_SIZE / 2));
    std::tuple<Eigen::Vector3d, Eigen::Vector3d, Eigen::Vector3d> qt_e =
        std::make_tuple(
            Eigen::Vector3d(u + UNIT_SIZE / 2, 0, w + UNIT_SIZE / 2),
            Eigen::Vector3d(u + UNIT_SIZE / 2, 0, w - UNIT_SIZE / 2),
            Eigen::Vector3d(u + UNIT_SIZE / 2, block_height, w + UNIT_SIZE / 2));
    std::tuple<Eigen::Vector3d, Eigen::Vector3d, Eigen::Vector3d> qt_w =
        std::make_tuple(
            Eigen::Vector3d(u - UNIT_SIZE / 2, 0, w - UNIT_SIZE / 2),
            Eigen::Vector3d(u - UNIT_SIZE / 2, 0, w + UNIT_SIZE / 2),
            Eigen::Vector3d(u - UNIT_SIZE / 2, block_height, w - UNIT_SIZE / 2));
    std::shared_ptr<Quad> quad_n(new Quad(qt_n));
    std::shared_ptr<Quad> quad_s(new Quad(qt_s));
    std::shared_ptr<Quad> quad_e(new Quad(qt_e));
    std::shared_ptr<Quad> quad_w(new Quad(qt_w));

    if (c == 'M' || c == 'm')
    {
        std::shared_ptr<Material> mirror_mat(new Material());
        mirror_mat->ka = Eigen::Vector3d(0.1, 0.1, 0.1);
        mirror_mat->kd = Eigen::Vector3d(0.1, 0.1, 0.1);
        mirror_mat->ks = Eigen::Vector3d(0.8, 0.8, 0.8);
        mirror_mat->km = Eigen::Vector3d(0.999, 0.999, 0.999);
        mirror_mat->phong_exponent = 100;
        quad_n->material = mirror_mat;
        quad_s->material = mirror_mat;
        quad_e->material = mirror_mat;
        quad_w->material = mirror_mat;
    }
    else
    {
        set_random_material(quad_n->material, "FLAT");
        set_random_material(quad_s->material, "FLAT");
        set_random_material(quad_e->material, "FLAT");
        set_random_material(quad_w->material, "FLAT");
    }

    objects.push_back(quad_n);
    objects.push_back(quad_s);
    objects.push_back(quad_e);
    objects.push_back(quad_w);
}

#endif
