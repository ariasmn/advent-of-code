// DISCLAIMER: Starting day 21, I've been just looking at solutions from another languages I know (Golang, PHP...)
// and basically doing it in my own way in C.
// I looked at different solutions and took ideas from all of them to make my own version, so I can't link to the solution directly,
// but all of the solutions can be found in the subreddit: https://www.reddit.com/r/adventofcode/.
// When I started AoC 2023, it was because I wanted to learn C and
// honestly, I wasn't in the mood to solve certain puzzles (I got a bit tired of it in AoC 2022).
// So, since I struggled A LOT in day 20, I've decided to just do it this way.

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <z3/z3.h>

#include "../helper.h"

#define MIN_BOUND 200000000000000.0L
#define MAX_BOUND 400000000000000.0L

typedef struct
{
    long double px, py, pz; // Position coordinates in 3D space.
    long double vx, vy, vz; // Velocity components in 3D space.
} Hailstone;

// The idea is that if two hailstones intersect each other, their paths must cross at some point.
// We can find this intersection by:
// 1. Converting each hailstone's path into a line equation (y = mx + b)
// 2. Finding where these lines intersect by solving the system of equations
// 3. Checking if this intersection point:
//    - Is reached by moving forward in time for both hailstones
//    - Falls within our target area bounds.
bool check_intersection(Hailstone a, Hailstone b, long double *x, long double *y, long double min_bound, long double max_bound)
{
    // Convert to straight line equation (y = mx + b).
    // Slope m is calculated as change in y over change in x (dy/dx).
    long double m1 = a.vy / a.vx;
    long double m2 = b.vy / b.vx;
    
    // Calculate y-intercepts using point-slope form: b = y - mx.
    long double b1 = a.py - m1 * a.px;
    long double b2 = b.py - m2 * b.px;

    // If slopes are equal, lines are parallel and never intersect.
    // Using this to check for floating point imprecision.
    if (fabsl(m1 - m2) < 1e-10L)
    {
        return false;
    }

    // Find intersection point by solving system of equations:
    // y = m1x + b1
    // y = m2x + b2
    // Therefore: m1x + b1 = m2x + b2
    // x = (b2 - b1)/(m1 - m2).
    *x = (b2 - b1) / (m1 - m2);
    *y = m1 * (*x) + b1;

    // Check if intersection happens in the future by verifying
    // if we need to move in the same direction as velocity to reach intersection.
    // This is because we just need to check if they intersect given the starting point, and not before that starting point.
    bool a_future = ((*x - a.px) / a.vx >= 0);
    bool b_future = ((*x - b.px) / b.vx >= 0);

    if (!a_future || !b_future)
    {
        return false;
    }

    // Check if intersection point lies within the test area bounds.
    return *x >= min_bound && *x <= max_bound &&
           *y >= min_bound && *y <= max_bound;
}

int part_1(char **file_content)
{
    int hailstones_arr_size = 0;
    Hailstone *hailstones = NULL;

    for (int i = 0; file_content[i] != NULL; i++)
    {
        hailstones = realloc(hailstones, (hailstones_arr_size + 1) * sizeof(Hailstone));
        
        double px, py, pz, vx, vy, vz;
        sscanf(file_content[i], "%lf, %lf, %lf @ %lf, %lf, %lf",
               &px, &py, &pz, &vx, &vy, &vz);

        // Convert parsed doubles to long doubles for better precision.
        hailstones[hailstones_arr_size].px = (long double)px;
        hailstones[hailstones_arr_size].py = (long double)py;
        hailstones[hailstones_arr_size].pz = (long double)pz;
        hailstones[hailstones_arr_size].vx = (long double)vx;
        hailstones[hailstones_arr_size].vy = (long double)vy;
        hailstones[hailstones_arr_size].vz = (long double)vz;
        hailstones_arr_size++;
    }

    int intersections = 0;
    long double x, y;

    for (int i = 0; i < hailstones_arr_size; i++)
    {
        for (int j = i + 1; j < hailstones_arr_size; j++)
        {
            if (check_intersection(hailstones[i], hailstones[j], &x, &y, MIN_BOUND, MAX_BOUND))
            {
                intersections++;
            }
        }
    }

    free(hailstones);
    return intersections;
}

// Obviously, I didn't even know how to start this.
// For this to work:
// 1. For each hailstone, there must be some time when our rock meets it at exactly the same point
// 2. Our rock moves in a straight line at constant speed (like the hailstones)
// 
// We can write this as equations:
// - rock_position + rock_velocity * time = hailstone_position + hailstone_velocity * time
// 
// We need:
// - One equation for each dimension (x, y, z)
// - One set of equations for each hailstone
// 
// Three hailstones give us 9 equations (3 dimensions × 3 hailstones) and 9 unknowns:
// - rock starting position (x, y, z)
// - rock velocity (vx, vy, vz)
// - collision times (t1, t2, t3)
//
// Z3 is used here because solving a system of 9 equations with 9 unknowns is very complex,
// especially when these equations are non-linear (they involve multiplication of variables, like time × velocity).
long long part_2(char **file_content) {
    // Create Z3 context and solver
    Z3_config cfg = Z3_mk_config();
    Z3_context ctx = Z3_mk_context(cfg);
    Z3_solver solver = Z3_mk_solver(ctx);
    Z3_solver_inc_ref(ctx, solver);
    
    // Create integer sort for our variables
    Z3_sort int_sort = Z3_mk_int_sort(ctx);
    
    // Create variables for rock's position and velocity
    Z3_ast x = Z3_mk_const(ctx, Z3_mk_string_symbol(ctx, "x"), int_sort);
    Z3_ast y = Z3_mk_const(ctx, Z3_mk_string_symbol(ctx, "y"), int_sort);
    Z3_ast z = Z3_mk_const(ctx, Z3_mk_string_symbol(ctx, "z"), int_sort);
    Z3_ast vx = Z3_mk_const(ctx, Z3_mk_string_symbol(ctx, "vx"), int_sort);
    Z3_ast vy = Z3_mk_const(ctx, Z3_mk_string_symbol(ctx, "vy"), int_sort);
    Z3_ast vz = Z3_mk_const(ctx, Z3_mk_string_symbol(ctx, "vz"), int_sort);
    
    Z3_inc_ref(ctx, x);
    Z3_inc_ref(ctx, y);
    Z3_inc_ref(ctx, z);
    Z3_inc_ref(ctx, vx);
    Z3_inc_ref(ctx, vy);
    Z3_inc_ref(ctx, vz);
    
    // Process first few hailstones (3 is enough to find solution)
    int count = 0;
    for (int i = 0; file_content[i] != NULL && count < 3; i++, count++) {
        long long px, py, pz, hvx, hvy, hvz;
        sscanf(file_content[i], "%lld, %lld, %lld @ %lld, %lld, %lld",
               &px, &py, &pz, &hvx, &hvy, &hvz);
        
        // Create time variable for this hailstone
        char tname[10];
        sprintf(tname, "t%d", count);
        Z3_ast t = Z3_mk_const(ctx, Z3_mk_string_symbol(ctx, tname), int_sort);
        Z3_inc_ref(ctx, t);
        
        // Create integer constants
        Z3_ast px_ast = Z3_mk_int64(ctx, px, int_sort);
        Z3_ast py_ast = Z3_mk_int64(ctx, py, int_sort);
        Z3_ast pz_ast = Z3_mk_int64(ctx, pz, int_sort);
        Z3_ast hvx_ast = Z3_mk_int64(ctx, hvx, int_sort);
        Z3_ast hvy_ast = Z3_mk_int64(ctx, hvy, int_sort);
        Z3_ast hvz_ast = Z3_mk_int64(ctx, hvz, int_sort);
        
        Z3_inc_ref(ctx, px_ast);
        Z3_inc_ref(ctx, py_ast);
        Z3_inc_ref(ctx, pz_ast);
        Z3_inc_ref(ctx, hvx_ast);
        Z3_inc_ref(ctx, hvy_ast);
        Z3_inc_ref(ctx, hvz_ast);
        
        // x + vx*t = px + hvx*t
        Z3_ast vxt = Z3_mk_mul(ctx, 2, (Z3_ast[]){vx, t});
        Z3_ast hvxt = Z3_mk_mul(ctx, 2, (Z3_ast[]){hvx_ast, t});
        Z3_ast lhs_x = Z3_mk_add(ctx, 2, (Z3_ast[]){x, vxt});
        Z3_ast rhs_x = Z3_mk_add(ctx, 2, (Z3_ast[]){px_ast, hvxt});
        Z3_ast eq_x = Z3_mk_eq(ctx, lhs_x, rhs_x);
        Z3_solver_assert(ctx, solver, eq_x);
        
        // y + vy*t = py + hvy*t
        Z3_ast vyt = Z3_mk_mul(ctx, 2, (Z3_ast[]){vy, t});
        Z3_ast hvyt = Z3_mk_mul(ctx, 2, (Z3_ast[]){hvy_ast, t});
        Z3_ast lhs_y = Z3_mk_add(ctx, 2, (Z3_ast[]){y, vyt});
        Z3_ast rhs_y = Z3_mk_add(ctx, 2, (Z3_ast[]){py_ast, hvyt});
        Z3_ast eq_y = Z3_mk_eq(ctx, lhs_y, rhs_y);
        Z3_solver_assert(ctx, solver, eq_y);
        
        // z + vz*t = pz + hvz*t
        Z3_ast vzt = Z3_mk_mul(ctx, 2, (Z3_ast[]){vz, t});
        Z3_ast hvzt = Z3_mk_mul(ctx, 2, (Z3_ast[]){hvz_ast, t});
        Z3_ast lhs_z = Z3_mk_add(ctx, 2, (Z3_ast[]){z, vzt});
        Z3_ast rhs_z = Z3_mk_add(ctx, 2, (Z3_ast[]){pz_ast, hvzt});
        Z3_ast eq_z = Z3_mk_eq(ctx, lhs_z, rhs_z);
        Z3_solver_assert(ctx, solver, eq_z);
        
        // Assert that time is positive
        Z3_ast zero = Z3_mk_int64(ctx, 0, int_sort);
        Z3_inc_ref(ctx, zero);
        Z3_ast time_pos = Z3_mk_gt(ctx, t, zero);
        Z3_solver_assert(ctx, solver, time_pos);
        
        // Cleanup temporary ASTs
        Z3_dec_ref(ctx, t);
        Z3_dec_ref(ctx, px_ast);
        Z3_dec_ref(ctx, py_ast);
        Z3_dec_ref(ctx, pz_ast);
        Z3_dec_ref(ctx, hvx_ast);
        Z3_dec_ref(ctx, hvy_ast);
        Z3_dec_ref(ctx, hvz_ast);
        Z3_dec_ref(ctx, zero);
    }
    
    long long result = -1;
    
    // Check if solution exists
    if (Z3_solver_check(ctx, solver) == Z3_L_TRUE) {
        Z3_model model = Z3_solver_get_model(ctx, solver);
        Z3_model_inc_ref(ctx, model);
        
        // Get values for x, y, z
        Z3_ast xval, yval, zval;
        Z3_model_eval(ctx, model, x, true, &xval);
        Z3_model_eval(ctx, model, y, true, &yval);
        Z3_model_eval(ctx, model, z, true, &zval);
        
        Z3_inc_ref(ctx, xval);
        Z3_inc_ref(ctx, yval);
        Z3_inc_ref(ctx, zval);
        
        int64_t rx, ry, rz;
        Z3_get_numeral_int64(ctx, xval, &rx);
        Z3_get_numeral_int64(ctx, yval, &ry);
        Z3_get_numeral_int64(ctx, zval, &rz);
        
        result = rx + ry + rz;
        
        Z3_dec_ref(ctx, xval);
        Z3_dec_ref(ctx, yval);
        Z3_dec_ref(ctx, zval);
        Z3_model_dec_ref(ctx, model);
    }
    
    // Cleanup
    Z3_dec_ref(ctx, x);
    Z3_dec_ref(ctx, y);
    Z3_dec_ref(ctx, z);
    Z3_dec_ref(ctx, vx);
    Z3_dec_ref(ctx, vy);
    Z3_dec_ref(ctx, vz);
    
    Z3_solver_dec_ref(ctx, solver);
    Z3_del_config(cfg);
    Z3_del_context(ctx);
    
    return result;
}

int main(int argc, char *argv[])
{
    FILE *fptr = fopen("input.txt", "r");
    char **file_content = read_file(fptr);

    if (file_content == NULL)
    {
        return 1;
    }

    printf("Part 1: %d\n", part_1(file_content));
    printf("Part 2: %lld\n", part_2(file_content));

    return 0;
}
