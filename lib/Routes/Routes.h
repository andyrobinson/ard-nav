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

// Platt fields pond
waypoint pfA = {"A0", {53.44580, -2.22515, 3.0}};
waypoint pfB = {"B0", {53.44530, -2.22550, 3.0}};
waypoint pfC = {"C0", {53.44552, -2.22630, 3.0}};
waypoint pfD = {"D0", {53.44550, -2.22570, 3.0}};
waypoint pfE = {"E0", {53.44525, -2.22600, 3.0}};
waypoint pfF = {"F0", {53.44546, -2.22510, 4.0}};
waypoint pfG = {"G0", {53.44541, -2.22589, 3.0}};
waypoint pfH = {"H0", {53.44580, -2.22695, 3.0}};
waypoint pfI = {"I0", {53.44626, -2.22675, 3.0}};
waypoint pfJ = {"J0", {53.44668, -2.22545, 3.0}};
waypoint pfK = {"K0", {53.44685, -2.22487, 3.0}};
waypoint pfL = {"L0", {53.44656, -2.22498, 3.0}};

waypoint pf0route[] = {pfA,pfB,pfA,pfB,pfF};
waypoint pf1route[] = {pfB,pfG,pfB,pfG,pfF};
waypoint pf2route[] = {pfB,pfC,pfD,pfE,pfB,pfC,pfD,pfE,pfF};
waypoint pf3route[] = {pfA,pfB,pfC,pfB,pfA,pfF};
waypoint pf4route[] = {pfL,pfA,pfB,pfC,pfH,pfI,pfJ,pfL};
waypoint pf5route[] = {pfB,pfC,pfH,pfI,pfJ,pfL,pfA,pfB,pfF};
waypoint pf6route[] = {pfL,pfJ,pfK,pfL,pfJ,pfK};
waypoint pf7route[] = {pfL,pfK,pfJ,pfL,pfK,pfJ};

route pf0 = {"LeftRt", 5, pf0route};
route pf1 = {"OutBack", 5, pf1route};
route pf2 = {"Web", 9, pf2route};
route pf3 = {"LShape", 6, pf3route};
route pf4 = {"RoundL", 8, pf4route};
route pf5 = {"RoundB", 9, pf5route};
route pf6 = {"ThreeCW", 6, pf6route};
route pf7 = {"ThreeAC", 6, pf7route};

route plattfields[] = {pf0,pf1,pf2,pf3,pf4,pf5,pf6,pf7};

#endif
