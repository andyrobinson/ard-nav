#ifndef Routes_h
#define Routes_h

#include "Waypoint.h"

// test
// waypoint chorlton[] = {{"Chorlton", {53.44096, -2.27235, 100.0}},
//                     {"Up North", {80.0, -2.27, 0.1}},
//                     {"Dn South", {10.0, -2.27, 0.1}}};

// Chorlton Park
// waypoint chorlton2[] = {{"Pk Beg", {53.43485, -2.26893, 5}},
//                     {"A",   {53.43553, -2.27068, 5}},
//                     {"B",   {53.43519, -2.27118, 5}},
//                     {"C",   {53.43452, -2.26970, 5}},
//                     {"D",   {53.43518, -2.26972, 5}},
//                     {"E",   {53.43476, -2.27023, 5}},
//                     {"Pk End", {53.43485, -2.26893, 5}}};

// Hough end
//waypoint houghend[] = {{"Hough", {53.43772, -2.24809, 5}},  //53 26' 18", -2 14' 53"
//                    {"B",     {53.43708, -2.24770, 5}},  //53 26' 14", -2 14' 52"
//                    {"C",     {53.43746, -2.24735, 5}},  //53 26' 15", -2 14' 51"
//                    {"D",     {53.43742, -2.24842, 5}},  //53 26' 15", -2 14' 54"
//                    {"Hough", {53.43772, -2.24809, 5}}}; //53 26' 18", -2 14' 53"

typedef struct {
   int length;
   waypoint waypoints[];
} route;


// Platt fields
waypoint pfA = {"A",{53.44580,-2.22515, 1.0}};
waypoint pfB = {"B",{53.44530,-2.22550, 1.0}};
waypoint pfC = {"C",{53.44552,-2.22630, 1.0}};
waypoint pfD = {"D",{53.44550,-2.22570, 1.0}};
waypoint pfE = {"E",{53.44525,-2.22600, 1.0}};
waypoint pfF = {"F",{53.44546,-2.22508, 1.0}};

route pf1 = {5,{pfA,pfB,pfA,pfB,pfF}};
route pf2 = {5,{pfB,pfC,pfB,pfC,pfF}};
route pf3 = {9, {pfB,pfC,pfD,pfE,pfB,pfC,pfD,pfE,pfF}};
route pf4 = {6, {pfA,pfB,pfC,pfB,pfA,pfF}};
route plattfields[] = {pf1,pf2,pf3,pf4};

#endif
