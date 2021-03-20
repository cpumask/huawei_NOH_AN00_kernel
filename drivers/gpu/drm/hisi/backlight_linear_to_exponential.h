/*
 **Copyright (c) 2013-2014, Hisilicon Tech. Co., Ltd. All rights reserved.
 **
 **This program is free software; you can redistribute it and/or modify
 **it under the terms of the GNU General Public License version 2 and
 **only version 2 as published by the Free Software Foundation.
 **
 **This program is distributed in the hope that it will be useful,
 **but WITHOUT ANY WARRANTY; without even the implied warranty of
 **MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 **GNU General Public License for more details.
 **
 */
#ifndef BACKLIGHT_LINEAR_TO_EXPONENTIAL_H
#define BACKLIGHT_LINEAR_TO_EXPONENTIAL_H
int level_map[801] = {
11,
11, 11, 11, 11, 12, 12, 12, 12, 13, 13, 13, 14, 14, 14, 14, 14,
14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 15, 15, 15, 15,
15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 16, 16, 16, 17, 17,
17, 17, 17, 17, 18, 18, 18, 18, 19, 19, 19, 19, 20, 20, 20, 21,
21, 22, 22, 23, 23, 24, 24, 25, 25, 26, 26, 27, 28, 29, 30, 30,
31, 31, 32, 32, 33, 33, 34, 34, 35, 35, 36, 36, 37, 37, 38, 38,
39, 39, 40, 41, 42, 43, 44, 45, 46, 46, 46, 47, 47, 48, 49, 50,
51, 51, 52, 52, 53, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63,
64, 65, 66, 67, 67, 68, 68, 69, 69, 70, 70, 71, 71, 72, 72, 72,
73, 73, 74, 74, 75, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85,
86, 86, 87, 87, 88, 89, 90, 90, 91, 91, 92, 93, 94, 95, 96, 97,
98, 99, 100, 101, 102, 103, 104, 105, 105, 106, 106, 107, 107, 108, 109, 110,
111, 112, 113, 114, 115, 116, 117, 118, 118, 119, 119, 120, 120, 121, 121, 122,
122, 123, 124, 125, 126, 127, 128, 129, 130, 130, 131, 132, 133, 134, 135, 135,
136, 136, 137, 138, 139, 140, 141, 142, 143, 144, 145, 145, 146, 146, 147, 147,
148, 149, 150, 151, 152, 153, 154, 155, 156, 157, 158, 159, 160, 161, 162, 163,
164, 165, 166, 167, 168, 169, 170, 171, 172, 173, 174, 174, 175, 175, 176, 176,
177, 178, 179, 180, 181, 182, 183, 184, 185, 186, 187, 188, 189, 190, 191, 191,
192, 193, 194, 195, 196, 197, 198, 199, 200, 201, 202, 203, 204, 205, 206, 207,
208, 209, 210, 211, 212, 213, 214, 215, 216, 217, 218, 219, 220, 221, 222, 223,
224, 225, 226, 227, 228, 229, 230, 231, 232, 233, 234, 235, 236, 237, 238, 238,
239, 240, 241, 242, 243, 244, 245, 246, 247, 248, 249, 250, 251, 252, 253, 254,
255, 256, 257, 258, 259, 260, 262, 263, 264, 265, 267, 268, 270, 271, 272, 273,
274, 275, 276, 277, 278, 279, 280, 282, 283, 284, 285, 286, 287, 289, 290, 292,
293, 294, 295, 296, 297, 298, 299, 300, 301, 302, 303, 304, 305, 306, 307, 308,
310, 312, 314, 316, 318, 320, 321, 322, 323, 324, 325, 326, 327, 328, 329, 329,
330, 332, 333, 335, 336, 337, 338, 339, 340, 341, 343, 344, 345, 346, 347, 348,
349, 350, 351, 352, 353, 354, 355, 356, 357, 358, 359, 360, 361, 362, 363, 364,
365, 366, 367, 368, 369, 370, 371, 372, 373, 374, 375, 376, 377, 378, 379, 380,
381, 382, 383, 384, 385, 386, 387, 388, 389, 390, 391, 392, 393, 394, 395, 395,
396, 397, 398, 399, 400, 401, 402, 403, 404, 405, 406, 407, 408, 409, 410, 411,
412, 413, 414, 415, 417, 418, 419, 420, 421, 423, 424, 426, 427, 428, 429, 430,
432, 433, 434, 435, 436, 437, 438, 439, 440, 442, 443, 445, 446, 447, 448, 449,
450, 451, 452, 453, 454, 455, 456, 457, 458, 459, 460, 462, 463, 465, 466, 467,
469, 470, 471, 472, 473, 474, 475, 477, 478, 479, 480, 482, 483, 484, 485, 486,
487, 488, 489, 490, 491, 493, 494, 495, 496, 497, 499, 501, 502, 503, 504, 505,
507, 509, 510, 511, 512, 513, 514, 515, 516, 518, 519, 520, 521, 522, 523, 524,
526, 527, 528, 529, 530, 531, 532, 535, 537, 538, 539, 541, 543, 544, 545, 546,
548, 549, 550, 551, 552, 553, 554, 555, 556, 557, 559, 560, 562, 563, 564, 565,
566, 567, 568, 569, 570, 571, 572, 573, 574, 575, 577, 579, 581, 583, 585, 587,
588, 589, 590, 591, 592, 593, 595, 596, 597, 598, 599, 600, 602, 603, 604, 605,
606, 608, 609, 610, 611, 613, 614, 615, 617, 618, 620, 621, 623, 624, 626, 627,
629, 630, 631, 632, 633, 634, 636, 637, 638, 639, 640, 641, 642, 643, 645, 646,
648, 649, 650, 651, 652, 653, 655, 656, 657, 658, 660, 662, 663, 665, 667, 668,
670, 671, 673, 674, 676, 677, 678, 679, 680, 682, 683, 685, 686, 688, 689, 690,
692, 694, 695, 696, 697, 698, 700, 701, 702, 703, 705, 706, 708, 709, 711, 712,
714, 715, 717, 718, 719, 720, 721, 722, 723, 725, 727, 728, 729, 731, 732, 734,
736, 738, 739, 741, 742, 744, 745, 747, 748, 750, 751, 752, 753, 754, 755, 756,
757, 759, 760, 762, 763, 765, 766, 767, 769, 771, 773, 774, 775, 776, 777, 778,
779, 781, 783, 785, 787, 789, 792, 793, 794, 795, 796, 797, 798, 799, 800, 800
};

#endif
