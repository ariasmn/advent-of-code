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

#include "../helper.h"

typedef struct
{
    int x, y, z;
} Point;

// A brick has two endpoints and tracks bricks above and below it.
typedef struct Brick
{
    int id;
    Point pos1;
    Point pos2;
    struct Brick **bricks_above;
    int above_count;
    int above_capacity;
    struct Brick **bricks_below;
    int below_count;
    int below_capacity;
} Brick;

bool ranges_overlap(int min1, int max1, int min2, int max2)
{
    return !(max1 < min2 || min1 > max2);
}

// Check if one brick is directly under another by checking x,y overlap.
bool is_under(Brick *lower, Brick *upper)
{
    bool x_overlap = ranges_overlap(
        MIN(lower->pos1.x, lower->pos2.x),
        MAX(lower->pos1.x, lower->pos2.x),
        MIN(upper->pos1.x, upper->pos2.x),
        MAX(upper->pos1.x, upper->pos2.x));

    bool y_overlap = ranges_overlap(
        MIN(lower->pos1.y, lower->pos2.y),
        MAX(lower->pos1.y, lower->pos2.y),
        MIN(upper->pos1.y, upper->pos2.y),
        MAX(upper->pos1.y, upper->pos2.y));

    return x_overlap && y_overlap;
}

int get_lowest_z(Brick *brick)
{
    return MIN(brick->pos1.z, brick->pos2.z);
}

int get_highest_z(Brick *brick)
{
    return MAX(brick->pos1.z, brick->pos2.z);
}

bool is_level_below(Brick *lower, Brick *upper)
{
    return get_highest_z(lower) == get_lowest_z(upper) - 1;
}

void add_brick_above(Brick *brick, Brick *above)
{
    if (brick->above_count == brick->above_capacity)
    {
        brick->above_capacity = brick->above_capacity == 0 ? 1 : brick->above_capacity * 2;
        brick->bricks_above = realloc(brick->bricks_above, brick->above_capacity * sizeof(Brick *));
    }
    brick->bricks_above[brick->above_count++] = above;
}

void add_brick_below(Brick *brick, Brick *below)
{
    if (brick->below_count == brick->below_capacity)
    {
        brick->below_capacity = brick->below_capacity == 0 ? 1 : brick->below_capacity * 2;
        brick->bricks_below = realloc(brick->bricks_below, brick->below_capacity * sizeof(Brick *));
    }
    brick->bricks_below[brick->below_count++] = below;
}

int compare_bricks(const void *a, const void *b)
{
    Brick *brick_a = *(Brick **)a;
    Brick *brick_b = *(Brick **)b;
    return get_lowest_z(brick_a) - get_lowest_z(brick_b);
}

// A brick is safe to disintegrate if none of the bricks above rely solely on it for support. In other words:
// For each brick above, count how many supports it has.
// If any brick above has only one support (this brick), then we can't remove it.
// Otherwise, it's safe to remove.
bool can_disintegrate(Brick *brick)
{
    for (int i = 0; i < brick->above_count; i++)
    {
        Brick *above = brick->bricks_above[i];
        int supported_by_current = 0;
        int total_supports = 0;

        // Count how many supports the brick above has.
        for (int j = 0; j < above->below_count; j++)
        {
            Brick *support = above->bricks_below[j];
            if (is_level_below(support, above))
            {
                total_supports++;
                if (support == brick)
                {
                    supported_by_current = 1;
                }
            }
        }

        // Can't disintegrate if we're the only support.
        if (supported_by_current && total_supports == 1)
        {
            return false;
        }
    }
    return true;
}

// Simulates a chain reaction by tracking which bricks will fall if a support is removed.
// For each brick:
// - Checks if it has any remaining non-disintegrated supports.
// - If it has no supports and isn't on the ground, it falls.
// - Continues checking until no more bricks can fall.
int count_chain_reaction(Brick **bricks, int bricks_count, Brick *initial_brick)
{
    bool *disintegrated = calloc(bricks_count, sizeof(bool));
    bool *will_fall = calloc(bricks_count, sizeof(bool));
    int fall_count = 0;

    disintegrated[initial_brick->id] = true;

    bool changed = true;
    while (changed)
    {
        changed = false;
        for (int i = 0; i < bricks_count; i++)
        {
            Brick *brick = bricks[i];
            if (!will_fall[brick->id] && !disintegrated[brick->id])
            {
                // Count how many non-disintegrated supports this brick has
                int supported_count = 0;
                for (int j = 0; j < brick->below_count; j++)
                {
                    Brick *support = brick->bricks_below[j];
                    if (is_level_below(support, brick) && !disintegrated[support->id])
                        supported_count++;
                }
                // If brick has no supports and isn't on ground level, it falls.
                if (supported_count == 0 && brick->below_count > 0)
                {
                    will_fall[brick->id] = true;
                    disintegrated[brick->id] = true;
                    fall_count++;
                    changed = true;
                }
            }
        }
    }

    free(disintegrated);
    free(will_fall);
    return fall_count;
}

int part_1(char **file_content)
{
    int bricks_arr_size = 0;
    Brick **bricks = malloc(bricks_arr_size * sizeof(Brick *));

    for (int i = 0; file_content[i] != NULL; i++)
    {
        Brick *brick = calloc(1, sizeof(Brick));
        brick->id = i;

        int nums[6];
        char *ptr = file_content[i];
        for (int j = 0; j < 6; j++)
        {
            while (*ptr && !isdigit(*ptr))
                ptr++;
            nums[j] = atoi(ptr);
            while (*ptr && isdigit(*ptr))
                ptr++;
        }

        brick->pos1 = (Point){nums[0], nums[1], nums[2]};
        brick->pos2 = (Point){nums[3], nums[4], nums[5]};

        bricks = realloc(bricks, (bricks_arr_size + 1) * sizeof(Brick *));
        bricks[bricks_arr_size] = brick;
        bricks_arr_size++;
    }

    // Sort the bricks by height.
    // By sorting bricks from lowest to highest z-coordinate, we ensure that when we process a brick to see where it lands:
    // All possible bricks it could land on have already been processed and are in their final resting positions.
    // No brick that's higher up can interfere with where this brick lands.
    qsort(bricks, bricks_arr_size, sizeof(Brick *), compare_bricks);

    // Let blocks fall and establish support relationships.
    for (int i = 0; i < bricks_arr_size; i++)
    {
        Brick *falling = bricks[i];
        if (get_lowest_z(falling) == 1)
            continue;

        int highest_point = 1;

        // Find all bricks that could support this one.
        for (int j = 0; j < i; j++)
        {
            Brick *lower = bricks[j];
            if (is_under(lower, falling))
            {
                add_brick_above(lower, falling);
                add_brick_below(falling, lower);
                highest_point = MAX(highest_point, get_highest_z(lower) + 1);
            }
        }

        // Drop the brick to rest on its highest support, either the ground (z=1) or another brick.
        if (get_lowest_z(falling) > highest_point)
        {
            int drop_amount = get_lowest_z(falling) - highest_point;
            falling->pos1.z -= drop_amount;
            falling->pos2.z -= drop_amount;
        }
    }

    // Count bricks that can be safely removed.
    int result = 0;
    for (int i = 0; i < bricks_arr_size; i++)
    {
        if (can_disintegrate(bricks[i]))
        {
            result++;
        }
    }

    return result;
}

int part_2(char **file_content)
{
    // Copy and paste parsing and sorting.
    int bricks_arr_size = 0;
    Brick **bricks = malloc(bricks_arr_size * sizeof(Brick *));

    for (int i = 0; file_content[i] != NULL; i++)
    {
        Brick *brick = calloc(1, sizeof(Brick));
        brick->id = i;

        int nums[6];
        char *ptr = file_content[i];
        for (int j = 0; j < 6; j++)
        {
            while (*ptr && !isdigit(*ptr))
                ptr++;
            nums[j] = atoi(ptr);
            while (*ptr && isdigit(*ptr))
                ptr++;
        }

        brick->pos1 = (Point){nums[0], nums[1], nums[2]};
        brick->pos2 = (Point){nums[3], nums[4], nums[5]};

        bricks = realloc(bricks, (bricks_arr_size + 1) * sizeof(Brick *));
        bricks[bricks_arr_size] = brick;
        bricks_arr_size++;
    }

    qsort(bricks, bricks_arr_size, sizeof(Brick *), compare_bricks);

    for (int i = 0; i < bricks_arr_size; i++)
    {
        Brick *falling = bricks[i];
        if (get_lowest_z(falling) == 1)
            continue;

        int highest_point = 1;

        for (int j = 0; j < i; j++)
        {
            Brick *lower = bricks[j];
            if (is_under(lower, falling))
            {
                add_brick_above(lower, falling);
                add_brick_below(falling, lower);
                highest_point = MAX(highest_point, get_highest_z(lower) + 1);
            }
        }

        if (get_lowest_z(falling) > highest_point)
        {
            int drop_amount = get_lowest_z(falling) - highest_point;
            falling->pos1.z -= drop_amount;
            falling->pos2.z -= drop_amount;
        }
    }

    // Everything is the same as part 1 until this point.
    int total_falls = 0;
    for (int i = 0; i < bricks_arr_size; i++)
    {
        total_falls += count_chain_reaction(bricks, bricks_arr_size, bricks[i]);
    }

    return total_falls;
}

int main()
{
    FILE *fptr = fopen("input.txt", "r");
    char **file_content = read_file(fptr);

    if (file_content == NULL)
    {
        return 1;
    }

    printf("Part 1: %d\n", part_1(file_content));
    printf("Part 2: %d\n", part_2(file_content));

    return 0;
}