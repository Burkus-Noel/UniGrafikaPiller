#include <stdbool.h>
#include <stdio.h>

typedef struct
{
    double x;
    double y;
    double z;
} Cube;

int cube_data_set(Cube *cube, double x, double y, double z)
{
    cube->x = x;
    cube->y = y;
    cube->z = z;
    if (x < 0 || y < 0 || z < 0) return 1;
    else return 0;
}

double cube_volume_calc(Cube *cube)
{
 return cube->x * cube->y * cube->z;
}

double cube_surface_calc(Cube *cube)
{
return  2 * ((cube->x * cube->z) + (cube->x * cube->y) + (cube->y * cube->z));
}

bool cube_is_it_sqaure_side(Cube *cube)
{
    return (cube->x == cube->y || cube->x == cube->z || cube->y == cube->z );
     
}

int main(int argc, char* argv[])
{
    Cube cube;
    cube_data_set(&cube,5,15,19);
    double volume = cube_volume_calc(&cube);
    double surface = cube_surface_calc(&cube);
    bool square = cube_is_it_sqaure_side(&cube);
    printf("Volume: %lf, Surface: %lf, Does it have square side? %d", volume, surface, square);
	
	return 0;
}