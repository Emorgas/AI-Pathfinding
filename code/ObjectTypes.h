#ifndef _OBJECTTYPES_H_
#define _OBJECTTYPES_H_

#include <vector>
class Node;

#define MAX_VERTICES 15000
#define MAX_POLYGONS 15000

#define RADIANS_TO_DEGREES 57.2957795
#define NUDGE 0.5

typedef struct
{
	std::vector<Node*> path;
}Path;

// Our vertex type
typedef struct
{
	float x, y, z;
}vertex_type;

// The polygon (triangle), 3 numbers that aim 3 vertices
typedef struct
{
	int a, b, c;
}polygon_type;

// The mapcoord type, 2 texture coordinates for each vertex
typedef struct
{
	float u, v;
}mapcoord_type;

// The object type
typedef struct
{
	char name[20];

	int vertices_qty;
	int polygons_qty;

	vertex_type vertex[MAX_VERTICES];
	polygon_type polygon[MAX_POLYGONS];
	mapcoord_type mapcoord[MAX_VERTICES];
	int id_texture;
} obj_type, *obj_type_ptr;

typedef struct
{
	float index[3][3];
}matrix3x3_type;

//Coordinates in  heightmap.
typedef struct
{
	float x;
	float z;
}position;

inline bool operator==(const position pos1, position pos2) { return (pos1.x == pos2.x && pos1.z == pos2.z); }
inline bool operator!=(const position pos1, position pos2) { return !(pos1 == pos2); }

//Edge cost
typedef struct
{
	int waypointID_From;
	int waypointID_To;
	int cost;
}edge_type;

//Scene
#define SCENARIO_WALL_QUANTITY		209

#endif //_OBJECTTYPES_H_