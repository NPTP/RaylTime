#include "AABBTree.h"
#include "insert_box_into_box.h"
#include <limits>
#include "random_rgb_colour.h"

// Helper for AABB tree. Sets colour of bounding box for AABB visualization.
void set_random_aabb_colour(std::shared_ptr<Material>& vis_mat)
{
    std::shared_ptr<Material> new_mat(new Material());
    new_mat->ka = random_rgb_colour();
    new_mat->kd = Eigen::Vector3d::Zero();
    new_mat->ks = Eigen::Vector3d::Zero();
    new_mat->km = Eigen::Vector3d::Zero();
    new_mat->phong_exponent = 0;
    vis_mat = new_mat;
}

AABBTree::AABBTree(
    const std::vector<std::shared_ptr<Object> > & objects,
    int a_depth): 
    depth(std::move(a_depth)), 
    num_leaves(objects.size())
{
    set_random_aabb_colour(this->material);
    int n = objects.size();

    if (n == 1)
    {
        this->left = objects[0];
        this->right = NULL;
        insert_box_into_box(this->left->box, this->box);
    }
    else if (n == 2)
    {
        this->left = objects[0];
        this->right = objects[1];
        insert_box_into_box(this->left->box, this->box);
        insert_box_into_box(this->right->box, this->box);
    }
    else
    {
        // Find furthest points in objects list.
        double min_x, min_y, min_z;
        min_x = min_y = min_z = std::numeric_limits<double>::infinity();
        double max_x, max_y, max_z;
        max_x = max_y = max_z = -std::numeric_limits<double>::infinity();
        for (int i = 0; i < objects.size(); i++)
        {
            if (objects[i]->box.min_corner(0, 0) < min_x) min_x = objects[i]->box.min_corner(0, 0);
            if (objects[i]->box.min_corner(0, 1) < min_y) min_y = objects[i]->box.min_corner(0, 1);
            if (objects[i]->box.min_corner(0, 2) < min_z) min_z = objects[i]->box.min_corner(0, 2);
            if (objects[i]->box.max_corner(0, 0) > max_x) max_x = objects[i]->box.max_corner(0, 0);
            if (objects[i]->box.max_corner(0, 1) > max_y) max_y = objects[i]->box.max_corner(0, 1);
            if (objects[i]->box.max_corner(0, 2) > max_z) max_z = objects[i]->box.max_corner(0, 2);
        }

        // Find longest axis based on those points, and make split point along that axis.
        double x_dist = max_x - min_x;
        double y_dist = max_y - min_y;
        double z_dist = max_z - min_z;
        double max_dist = std::max( std::max(x_dist, y_dist), z_dist );
        int axis_index;
        double axis_split_point;
        if (max_dist == x_dist)
        {
            axis_index = 0;
            axis_split_point= min_x + (x_dist / 2);
        }
        else if (max_dist == y_dist)
        {
            axis_index = 1;
            axis_split_point= min_y + (y_dist / 2);
        }
        else // max_dist == z_dist
        { 
            axis_index = 2;
            axis_split_point= min_z + (z_dist / 2);
        }

        // Initialize a list of objects for the left child, and another list for the right child.
        std::vector<std::shared_ptr<Object>> left_objects;
        std::vector<std::shared_ptr<Object>> right_objects;

        // Push all objects with center <= split point in the left list, the rest in the right list.
        for (int i = 0; i < objects.size(); i++)
        {
            Eigen::RowVector3d obj_box_center = objects[i]->box.center();
            if (obj_box_center[axis_index] <= axis_split_point)
                left_objects.push_back(objects[i]);
            else
                right_objects.push_back(objects[i]);
        }

        // Check if we got a one-sided split, and move some objects around randomly if so.
        if (left_objects.size() == n)
        {
            for (int i = n / 2; i < n; i++)
            {
                right_objects.push_back(left_objects[i]);
            }
            left_objects.erase(left_objects.begin() + (n/2), left_objects.begin() + n);
        }
        else if (right_objects.size() == objects.size())
        {
            for (int i = 0; i < n / 2; i++)
            {
                left_objects.push_back(right_objects[i]);
            }
            right_objects.erase(right_objects.begin(), right_objects.begin() + (n/2));
        }

        // Initialize left child and right child with new AABBTrees using the left & right object lists.
        std::shared_ptr<AABBTree> left_child( new AABBTree(left_objects, depth+1));
        std::shared_ptr<AABBTree> right_child(new AABBTree(right_objects, depth+1));
        this->left  = left_child;
        this->right = right_child;

        // Add left and right AABBTrees to this box. Done!
        insert_box_into_box(this->left->box,  this->box);
        insert_box_into_box(this->right->box, this->box);
    }
}
