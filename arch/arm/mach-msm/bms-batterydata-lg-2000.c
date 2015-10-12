/* Copyright (c) 2012-2013, Xiaomi corporation. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 and
 * only version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#include <linux/mfd/pm8xxx/pm8921-bms.h>

static struct single_row_lut fcc_temp = {
	.x		= {-20, -10, 0, 5, 10, 15, 20, 25, 30, 40, 50, 60},
	.y		= {2066, 2069, 2073, 2074, 2074, 2074, 2072, 2070, 2068, 2061, 2053, 2039},
	.cols		= 12
};

static struct single_row_lut fcc_sf = {
	.x		= {0},
	.y		= {100},
	.cols		= 1
};

static struct sf_lut rbatt_sf = {
	.rows		= 28,
	.cols		= 12,
	.row_entries	= {-20, -10, 0, 5, 10, 15, 20, 25, 30, 40, 50, 60},
	.percent	= {100, 95, 90, 85, 80, 75, 70, 65, 60, 55, 50, 45, 40, 35, 30, 25, 20, 15, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1},
	.sf		= {
				{2212, 792, 325, 228, 169, 136, 115, 100, 90, 79, 73, 70},
				{2211, 793, 325, 228, 169, 136, 115, 100, 90, 79, 73, 70},
				{2178, 802, 330, 232, 176, 139, 118, 103, 93, 80, 74, 71},
				{2145, 817, 343, 242, 181, 145, 122, 107, 95, 82, 76, 72},
				{2146, 836, 355, 253, 190, 154, 128, 113, 101, 86, 78, 74},
				{2076, 863, 384, 273, 205, 163, 137, 118, 106, 89, 82, 76},
				{2090, 832, 387, 288, 219, 175, 145, 124, 111, 95, 84, 78},
				{2123, 840, 378, 293, 232, 189, 158, 134, 120, 100, 88, 82},
				{2155, 856, 362, 267, 223, 191, 165, 144, 127, 106, 93, 87},
				{2198, 873, 357, 250, 189, 154, 132, 115, 104, 91, 80, 75},
				{2254, 900, 363, 253, 189, 148, 124, 107, 95, 83, 77, 73},
				{2330, 937, 376, 260, 193, 152, 127, 111, 98, 84, 78, 74},
				{2422, 989, 391, 270, 201, 158, 130, 114, 104, 88, 80, 76},
				{2534, 1056, 415, 284, 208, 165, 135, 117, 106, 92, 82, 79},
				{2670, 1142, 453, 305, 220, 172, 140, 120, 106, 90, 80, 74},
				{2827, 1256, 500, 333, 236, 180, 144, 122, 105, 87, 79, 74},
				{3008, 1394, 550, 362, 252, 186, 148, 124, 106, 89, 80, 74},
				{3227, 1572, 613, 396, 269, 197, 156, 130, 110, 91, 80, 75},
				{3299, 1676, 705, 452, 299, 217, 168, 139, 119, 95, 84, 77},
				{3375, 1720, 734, 473, 314, 225, 175, 143, 122, 96, 84, 77},
				{3460, 1769, 766, 497, 328, 233, 181, 145, 123, 97, 84, 78},
				{3547, 1822, 791, 515, 343, 243, 184, 147, 123, 96, 84, 78},
				{3645, 1883, 815, 529, 346, 241, 180, 145, 122, 97, 84, 78},
				{3758, 1949, 842, 543, 352, 245, 184, 148, 126, 100, 87, 81},
				{3877, 2028, 878, 566, 369, 256, 194, 154, 131, 104, 91, 84},
				{4019, 2119, 928, 603, 393, 273, 205, 166, 139, 111, 97, 86},
				{4324, 2238, 997, 658, 432, 301, 227, 184, 156, 122, 104, 92},
				{5169, 2413, 1110, 756, 510, 356, 273, 217, 180, 142, 117, 104},
					}
};

static struct pc_temp_ocv_lut pc_temp_ocv = {
	.rows		= 29,
	.cols		= 12,
	.temp		= {-20, -10, 0, 5, 10, 15, 20, 25, 30, 40, 50, 60},
	.percent	= {100, 95, 90, 85, 80, 75, 70, 65, 60, 55, 50, 45, 40, 35, 30, 25, 20, 15, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0},
	.ocv		= {
				{4330, 4330, 4329, 4329, 4328, 4327, 4326, 4325, 4324, 4320, 4317, 4313},
				{4244, 4251, 4257, 4257, 4258, 4258, 4258, 4258, 4258, 4256, 4255, 4253},
				{4180, 4193, 4200, 4201, 4203, 4203, 4203, 4203, 4202, 4202, 4200, 4199},
				{4122, 4138, 4148, 4149, 4150, 4150, 4150, 4150, 4150, 4149, 4148, 4147},
				{4070, 4087, 4097, 4098, 4099, 4100, 4100, 4100, 4100, 4099, 4098, 4097},
				{4001, 4033, 4050, 4053, 4054, 4055, 4055, 4055, 4055, 4054, 4052, 4050},
				{3950, 3969, 3990, 3996, 4000, 4002, 4003, 4004, 4005, 4006, 4006, 4007},
				{3910, 3927, 3943, 3951, 3957, 3960, 3963, 3964, 3966, 3968, 3967, 3967},
				{3874, 3891, 3902, 3907, 3915, 3920, 3923, 3925, 3927, 3928, 3928, 3929},
				{3844, 3858, 3867, 3869, 3871, 3873, 3875, 3876, 3877, 3878, 3877, 3877},
				{3818, 3830, 3839, 3840, 3842, 3843, 3844, 3844, 3845, 3845, 3846, 3846},
				{3797, 3807, 3816, 3818, 3819, 3820, 3821, 3821, 3822, 3822, 3823, 3823},
				{3778, 3788, 3796, 3799, 3801, 3801, 3802, 3802, 3803, 3803, 3803, 3803},
				{3761, 3773, 3781, 3784, 3785, 3786, 3786, 3786, 3786, 3786, 3786, 3786},
				{3744, 3758, 3769, 3772, 3772, 3773, 3772, 3772, 3771, 3768, 3766, 3761},
				{3725, 3743, 3752, 3757, 3759, 3760, 3758, 3757, 3754, 3748, 3742, 3736},
				{3702, 3726, 3729, 3732, 3735, 3735, 3734, 3731, 3728, 3723, 3717, 3712},
				{3673, 3703, 3704, 3704, 3704, 3703, 3702, 3700, 3696, 3691, 3683, 3673},
				{3637, 3674, 3686, 3686, 3686, 3685, 3683, 3681, 3679, 3674, 3669, 3663},
				{3625, 3665, 3681, 3683, 3683, 3683, 3681, 3679, 3677, 3672, 3666, 3660},
				{3612, 3653, 3674, 3677, 3678, 3678, 3677, 3675, 3673, 3667, 3662, 3655},
				{3595, 3637, 3659, 3665, 3668, 3669, 3668, 3665, 3662, 3655, 3649, 3640},
				{3574, 3615, 3637, 3642, 3645, 3645, 3642, 3639, 3636, 3629, 3621, 3612},
				{3548, 3586, 3603, 3607, 3608, 3607, 3603, 3601, 3597, 3591, 3583, 3575},
				{3514, 3547, 3558, 3560, 3560, 3559, 3556, 3553, 3550, 3544, 3537, 3530},
				{3471, 3495, 3500, 3501, 3500, 3500, 3497, 3495, 3493, 3488, 3482, 3476},
				{3412, 3426, 3425, 3425, 3426, 3427, 3424, 3424, 3422, 3419, 3414, 3408},
				{3329, 3330, 3327, 3329, 3331, 3333, 3332, 3332, 3331, 3328, 3324, 3319},
				{3200, 3200, 3200, 3200, 3200, 3200, 3200, 3200, 3200, 3200, 3200, 3200}
	}
};

struct pm8921_bms_battery_data lg_2000_data = {
	.fcc			= 2030,
	.fcc_temp_lut		= &fcc_temp,
	.fcc_sf_lut		= &fcc_sf,
	.pc_temp_ocv_lut	= &pc_temp_ocv,
	.rbatt_sf_lut		= &rbatt_sf,
	.default_rbatt_mohm	= 146
};
