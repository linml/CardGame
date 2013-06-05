//
//  Database.h
//  91.cube
//
//  Created by xianbei1987 on 13-5-23.
//
//

#ifndef _1_cube_Database_h
#define _1_cube_Database_h

#include <vector>
#include "gameStruct.h"

const int aaanCoordinate[6][5][3] = {
    //          left            right            up            down              in
    { { -1,  0,  0 }, {  1,  0,  0 }, { 0,  1,  0 }, { 0,  -1,  0 }, {  0,  0,  1 } }, //+z
    { {  1,  0,  0 }, { -1,  0,  0 }, { 0, -1,  0 }, { 0,   1,  0 }, {  0,  0, -1 } }, //-z
    { {  0,  0, -1 }, {  0,  0,  1 }, { 0,  1,  0 }, { 0,  -1,  0 }, { -1,  0,  0 } }, //-x
    { {  0,  0,  1 }, {  0,  0, -1 }, { 0,  1,  0 }, { 0,  -1,  0 }, {  1,  0,  0 } }, //+x
    { { -1,  0,  0 }, {  1,  0,  0 }, { 0,  0, -1 }, { 0,   0,  1 }, {  0,  1,  0 } }, //+y
    { { -1,  0,  0 }, {  1,  0,  0 }, { 0,  0,  1 }, { 0,   0, -1 }, {  0, -1,  0 } }  //-y
};

namespace PtGameData {
};

#endif
