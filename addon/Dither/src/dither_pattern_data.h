#pragma once
#ifndef DITHER_PATTERN_DATA_H
#define DITHER_PATTERN_DATA_H

const int tiles2x2[] = {0, 0,   // tile 1
                        0, 0,
                        0, 1,   // tile 2
                        1, 0,
                        0, 1,   // tile 3
                        0, 0,
                        1, 0,   // tile 4
                        1, 1,
                        1, 1,   // tile 5
                        1, 1};

const int tiles3x3_v1[] = {0, 0, 0, // tile 1
                           0, 0, 0,
                           0, 0, 0,

                           1, 0, 1, // tile 2
                           0, 0, 0,
                           1, 0, 1,

                           1, 1, 1, // tile 3
                           1, 0, 1,
                           1, 1, 1,

                           0, 0, 0, // tile 4
                           0, 1, 0,
                           0, 0, 0,

                           0, 1, 0, // tile 5
                           1, 1, 1,
                           0, 1, 0,

                           0, 1, 0, // tile 6
                           1, 0, 1,
                           0, 1, 0,

                           0, 0, 0, // tile 7
                           1, 1, 1,
                           0, 0, 0,
                           0, 1, 0, // tile 8
                           0, 1, 0,
                           0, 1, 0,

                           1, 0, 0, // tile 9
                           0, 0, 0,
                           0, 0, 1,
                           0, 0, 1, // tile 10
                           0, 0, 0,
                           1, 0, 0,
                           0, 0, 0, // tile 11
                           1, 0, 1,
                           0, 0, 0,
                           0, 1, 0, // tile 12
                           0, 0, 0,
                           0, 1, 0,

                           1, 1, 1, // tile 13
                           1, 1, 1,
                           1, 1, 1};

const int tiles3x3_v2[] = {1, 1, 1, // tile 1
                           1, 1, 1,
                           1, 1, 1,
                           0, 0, 0, // tile 2
                           0, 1, 0,
                           0, 0, 0,
                           0, 0, 0, // tile 3
                           0, 1, 1,
                           0, 0, 0,
                           0, 1, 0, // tile 4
                           0, 1, 1,
                           0, 0, 0,
                           0, 1, 1, // tile 5
                           0, 1, 1,
                           0, 0, 0,
                           0, 1, 1, // tile 6
                           0, 1, 1,
                           0, 1, 0,
                           0, 1, 1, // tile 7
                           1, 1, 1,
                           0, 1, 0,
                           0, 1, 1, // tile 8
                           1, 1, 1,
                           1, 1, 0,
                           1, 1, 1, // tile 9
                           1, 1, 1,
                           1, 1, 0,
                           0, 0, 0, // tile 10
                           0, 0, 0,
                           0, 0, 0};

const int tiles3x3_v3[] = {1, 1, 1, // tile 1
                           1, 1, 1,
                           1, 1, 1,
                           1, 0, 0, // tile 2
                           0, 0, 0,
                           0, 0, 0,
                           1, 0, 0, // tile 3
                           0, 0, 0,
                           0, 1, 0,
                           1, 0, 0, // tile 4
                           0, 0, 1,
                           0, 1, 0,
                           1, 0, 1, // tile 5
                           0, 0, 1,
                           0, 1, 0,
                           1, 0, 1, // tile 6
                           1, 0, 1,
                           0, 1, 0,
                           1, 0, 1, // tile 7
                           1, 0, 1,
                           1, 1, 0,
                           1, 1, 1, // tile 8
                           1, 0, 1,
                           1, 1, 0,
                           1, 1, 1, // tile 9
                           1, 1, 1,
                           1, 1, 0,
                           0, 0, 0, // tile 10
                           0, 0, 0,
                           0, 0, 0};

const int tiles4x4[] = {0, 0, 0, 0, // tile 1
                        0, 0, 0, 0,
                        0, 0, 0, 0,
                        0, 0, 0, 0,

                        1, 0, 0, 1, // tile 2
                        0, 0, 0, 0,
                        0, 0, 0, 0,
                        1, 0, 0, 1,

                        1, 0, 0, 1, // tile 3
                        0, 1, 1, 0,
                        0, 1, 1, 0,
                        1, 0, 0, 1,

                        0, 0, 0, 0, // tile 4
                        0, 1, 0, 0,
                        0, 0, 1, 0,
                        0, 0, 0, 0,

                        1, 1, 1, 1, // tile 5
                        1, 0, 0, 1,
                        1, 0, 0, 1,
                        1, 1, 1, 1,

                        1, 1, 1, 1, // tile 6
                        1, 1, 1, 1,
                        1, 1, 1, 1,
                        1, 1, 1, 1};

const int tiles5x2[] = {0, 0, 0, 0, 0,  // tile 1
                        0, 0, 0, 0, 0,

                        1, 1, 0, 0, 0,  // tile 2
                        1, 1, 0, 0, 0,

                        0, 1, 0, 1, 0,  // tile 3
                        0, 0, 1, 0, 0,

                        0, 0, 1, 0, 0,  // tile 4
                        0, 0, 1, 0, 0,

                        1, 1, 0, 1, 1,  // tile 5
                        1, 1, 0, 1, 1,

                        0, 1, 1, 1, 0,  // tile 6
                        0, 1, 1, 1, 0,

                        1, 1, 1, 1, 1,  // tile 7
                        1, 1, 1, 1, 1};

#endif  // DITHER_PATTERN_DATA_H