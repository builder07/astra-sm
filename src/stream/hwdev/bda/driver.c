/*
 * Astra Module: BDA
 *
 * Copyright (C) 2016, Artem Kharitonov <artem@3phase.pw>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/*
 * Driver Name:
 *      dvb_input
 *
 * Module Options:
 *      TODO
 *
 * Module Methods:
 *      TODO
 */

#include "bda.h"

const hw_driver_t hw_driver_bda =
{
    .name = "dvb_input",
    .description = "DVB Input (DirectShow BDA)",
    .init = NULL, /* TODO */
    .destroy = NULL, /* TODO */
    .enumerate = bda_enumerate,
};
