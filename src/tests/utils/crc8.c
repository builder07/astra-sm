/*
 * Astra Unit Tests
 * http://cesbo.com/astra
 *
 * Copyright (C) 2016-2017, Artem Kharitonov <artem@3phase.pw>
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

#include "../libastra.h"
#include <astra/utils/crc8.h>

/* T2-MI headers */
static const uint8_t test_headers[][10] =
{
    { 0x05, 0xe8, 0x00, 0xa2, 0x19, 0x34, 0x98, 0x88, 0x00, 0x00 },
    { 0x08, 0xe8, 0x00, 0x99, 0x9a, 0x34, 0x98, 0x20, 0x00, 0x00 },
    { 0x0a, 0xe8, 0x00, 0xb4, 0x8f, 0x34, 0x98, 0x01, 0x00, 0x00 },
    { 0x0b, 0xe8, 0x00, 0x87, 0x23, 0x3a, 0x70, 0x52, 0x00, 0x00 },
    { 0x0c, 0xe8, 0x00, 0x8d, 0x1c, 0x34, 0x98, 0xd6, 0x00, 0x00 },
    { 0x0d, 0xe8, 0x00, 0x92, 0xfe, 0x34, 0x98, 0x25, 0x00, 0x00 },
    { 0x0e, 0xe8, 0x00, 0x8f, 0x00, 0x34, 0x98, 0x42, 0x00, 0x00 },
    { 0x10, 0xe8, 0x00, 0x82, 0xb8, 0x34, 0x98, 0xae, 0x00, 0x00 },
    { 0x12, 0xe8, 0x00, 0x9e, 0x52, 0x34, 0x98, 0xaf, 0x00, 0x00 },
    { 0x13, 0xe8, 0x00, 0x90, 0x29, 0x34, 0x98, 0x03, 0x00, 0x00 },
    { 0x1b, 0xe8, 0x00, 0x9c, 0x38, 0x34, 0x98, 0x38, 0x00, 0x00 },
    { 0x27, 0xe8, 0x00, 0x9e, 0x1b, 0x34, 0x98, 0xa4, 0x00, 0x00 },
    { 0x29, 0xe8, 0x00, 0xa7, 0xdd, 0x3a, 0x70, 0x54, 0x00, 0x00 },
    { 0x31, 0xe8, 0x00, 0x93, 0xb8, 0x34, 0x98, 0xd0, 0x00, 0x00 },
    { 0x32, 0xe8, 0x00, 0xaa, 0x2d, 0x3a, 0x70, 0x82, 0x00, 0x00 },
    { 0x33, 0xe8, 0x00, 0x8a, 0x11, 0x3a, 0x70, 0xfc, 0x00, 0x00 },
    { 0x34, 0xe8, 0x00, 0xa1, 0x27, 0x34, 0x98, 0xd1, 0x00, 0x00 },
    { 0x36, 0xe8, 0x00, 0xb6, 0x72, 0x34, 0x98, 0x6e, 0x00, 0x00 },
    { 0x38, 0xe8, 0x00, 0x91, 0xd5, 0x34, 0x98, 0x64, 0x00, 0x00 },
    { 0x39, 0xe8, 0x00, 0x97, 0x7f, 0x34, 0x98, 0xa9, 0x00, 0x00 },
    { 0x3b, 0xe8, 0x00, 0x8b, 0x55, 0x3a, 0x70, 0x99, 0x00, 0x00 },
    { 0x3c, 0xe8, 0x00, 0xb1, 0xb9, 0x34, 0x98, 0xdf, 0x00, 0x00 },
    { 0x42, 0xe8, 0x00, 0x92, 0xc7, 0x34, 0x98, 0x1b, 0x00, 0x00 },
    { 0x44, 0xe8, 0x00, 0xb5, 0x80, 0x34, 0x98, 0xb8, 0x00, 0x00 },
    { 0x4a, 0xe8, 0x00, 0x95, 0xd3, 0x34, 0x98, 0x48, 0x00, 0x00 },
    { 0x4b, 0xe8, 0x00, 0x90, 0xe3, 0x34, 0x98, 0xae, 0x00, 0x00 },
    { 0x4c, 0xe8, 0x00, 0xba, 0x70, 0x34, 0x98, 0x51, 0x00, 0x00 },
    { 0x4e, 0xe8, 0x00, 0xa8, 0x14, 0x3a, 0x70, 0x5f, 0x00, 0x00 },
    { 0x50, 0xe8, 0x00, 0xb0, 0x0d, 0x34, 0x98, 0x7d, 0x00, 0x00 },
    { 0x52, 0xe8, 0x00, 0x92, 0x0c, 0x34, 0x98, 0x6f, 0x00, 0x00 },
    { 0x54, 0xe8, 0x00, 0x83, 0xe1, 0x34, 0x98, 0x6f, 0x00, 0x00 },
    { 0x55, 0xe8, 0x00, 0xa7, 0x07, 0x3a, 0x70, 0xcd, 0x00, 0x00 },
    { 0x5b, 0xe8, 0x00, 0xa3, 0xfc, 0x34, 0x98, 0xf4, 0x00, 0x00 },
    { 0x65, 0xe8, 0x00, 0x8a, 0x49, 0x3a, 0x70, 0x07, 0x00, 0x00 },
    { 0x67, 0xe8, 0x00, 0x8b, 0x1e, 0x3a, 0x70, 0x8e, 0x00, 0x00 },
    { 0x68, 0xe8, 0x00, 0xa0, 0xf0, 0x34, 0x98, 0xc7, 0x00, 0x00 },
    { 0x6f, 0xe8, 0x00, 0x94, 0xe1, 0x34, 0x98, 0x91, 0x00, 0x00 },
    { 0x71, 0xe8, 0x00, 0x88, 0x2f, 0x3a, 0x70, 0xe4, 0x00, 0x00 },
    { 0x74, 0xe8, 0x00, 0xb7, 0x9b, 0x34, 0x98, 0x2f, 0x00, 0x00 },
    { 0x75, 0xe8, 0x00, 0x87, 0xf8, 0x3a, 0x70, 0xda, 0x00, 0x00 },
    { 0x76, 0xe8, 0x00, 0xa3, 0xc5, 0x34, 0x98, 0xe9, 0x00, 0x00 },
    { 0x77, 0xe8, 0x00, 0x80, 0x1a, 0x34, 0x98, 0x96, 0x00, 0x00 },
    { 0x78, 0xe8, 0x00, 0xb9, 0x7e, 0x34, 0x98, 0x9b, 0x00, 0x00 },
    { 0x7c, 0xe8, 0x00, 0xa9, 0xf6, 0x3a, 0x70, 0x77, 0x00, 0x00 },
    { 0x7e, 0xe8, 0x00, 0x96, 0xc4, 0x34, 0x98, 0xfe, 0x00, 0x00 },
    { 0x80, 0xe8, 0x00, 0x86, 0xec, 0x3a, 0x70, 0x48, 0x00, 0x00 },
    { 0x82, 0xe8, 0x00, 0x80, 0xd5, 0x34, 0x98, 0x42, 0x00, 0x00 },
    { 0x84, 0xe8, 0x00, 0x83, 0xaa, 0x34, 0x98, 0x64, 0x00, 0x00 },
    { 0x86, 0xe8, 0x00, 0xa2, 0xd4, 0x34, 0x98, 0x33, 0x00, 0x00 },
    { 0x8c, 0xe8, 0x00, 0x95, 0x9c, 0x34, 0x98, 0x3d, 0x00, 0x00 },
    { 0x92, 0xe8, 0x00, 0x82, 0xef, 0x34, 0x98, 0xb8, 0x00, 0x00 },
    { 0x98, 0xe8, 0x00, 0xad, 0x38, 0x34, 0x98, 0x5b, 0x00, 0x00 },
    { 0x99, 0xe8, 0x00, 0x9f, 0x44, 0x34, 0x98, 0x65, 0x00, 0x00 },
    { 0x9a, 0xe8, 0x00, 0xa8, 0xe9, 0x3a, 0x70, 0xe6, 0x00, 0x00 },
    { 0x9c, 0xe8, 0x00, 0xbb, 0x62, 0x34, 0x98, 0x07, 0x00, 0x00 },
    { 0x9d, 0xe8, 0x00, 0xb2, 0xab, 0x34, 0x98, 0x95, 0x00, 0x00 },
    { 0x9f, 0xe8, 0x00, 0xb4, 0xc6, 0x34, 0x98, 0x0c, 0x00, 0x00 },
    { 0xa2, 0xe8, 0x00, 0x9a, 0x8b, 0x34, 0x98, 0xd6, 0x00, 0x00 },
    { 0xa4, 0xe8, 0x00, 0xaf, 0x1b, 0x34, 0x98, 0xc7, 0x00, 0x00 },
    { 0xa8, 0xe8, 0x00, 0x91, 0x1a, 0x34, 0x98, 0xb9, 0x00, 0x00 },
    { 0xab, 0xe8, 0x00, 0x8f, 0xf1, 0x34, 0x98, 0xf8, 0x00, 0x00 },
    { 0xac, 0xe8, 0x00, 0x98, 0xa8, 0x34, 0x98, 0x6a, 0x00, 0x00 },
    { 0xae, 0xe8, 0x00, 0x8f, 0x37, 0x34, 0x98, 0x4c, 0x00, 0x00 },
    { 0xb0, 0xe8, 0x00, 0x94, 0xaa, 0x34, 0x98, 0x87, 0x00, 0x00 },
    { 0xb3, 0xe8, 0x00, 0xa1, 0xe2, 0x34, 0x98, 0x7d, 0x00, 0x00 },
    { 0xb4, 0xe8, 0x00, 0x8e, 0x0e, 0x34, 0x98, 0x8c, 0x00, 0x00 },
    { 0xb6, 0xe8, 0x00, 0x89, 0x3c, 0x3a, 0x70, 0x75, 0x00, 0x00 },
    { 0xb7, 0xe8, 0x00, 0x8c, 0x2b, 0x34, 0x98, 0x1f, 0x00, 0x00 },
    { 0xbc, 0xe8, 0x00, 0x93, 0xef, 0x34, 0x98, 0xdb, 0x00, 0x00 },
    { 0xc1, 0xe8, 0x00, 0x81, 0xc6, 0x34, 0x98, 0xf8, 0x00, 0x00 },
    { 0xc2, 0xe8, 0x00, 0x98, 0x71, 0x34, 0x98, 0x5f, 0x00, 0x00 },
    { 0xc5, 0xe8, 0x00, 0xac, 0x0f, 0x34, 0x98, 0x9a, 0x00, 0x00 },
    { 0xc7, 0xe8, 0x00, 0x9d, 0x60, 0x34, 0x98, 0xf9, 0x00, 0x00 },
    { 0xc9, 0xe8, 0x00, 0x81, 0xfe, 0x34, 0x98, 0x02, 0x00, 0x00 },
    { 0xcb, 0xe8, 0x00, 0x89, 0x05, 0x3a, 0x70, 0x6b, 0x00, 0x00 },
    { 0xce, 0xe8, 0x00, 0x99, 0x63, 0x34, 0x98, 0x16, 0x00, 0x00 },
    { 0xd2, 0xe8, 0x00, 0xcc, 0x80, 0x3a, 0x70, 0x00, 0x00, 0x00 },
    { 0xd8, 0xe8, 0x00, 0xb5, 0xb7, 0x34, 0x98, 0xc2, 0x00, 0x00 },
    { 0xd9, 0xe8, 0x00, 0x9c, 0x6f, 0x34, 0x98, 0x43, 0x00, 0x00 },
    { 0xdd, 0xe8, 0x00, 0xa0, 0x36, 0x34, 0x98, 0x1b, 0x00, 0x00 },
    { 0xdf, 0xe8, 0x00, 0xad, 0xf3, 0x34, 0x98, 0x06, 0x00, 0x00 },
    { 0xe0, 0xe8, 0x00, 0xbe, 0x00, 0x34, 0x98, 0x1f, 0x00, 0x00 },
    { 0xe2, 0xe8, 0x00, 0xaf, 0xd6, 0x34, 0x98, 0x73, 0x00, 0x00 },
    { 0xe4, 0xe8, 0x00, 0xab, 0x03, 0x3a, 0x70, 0x09, 0x00, 0x00 },
    { 0xe5, 0xe8, 0x00, 0x81, 0x0c, 0x34, 0x98, 0x4c, 0x00, 0x00 },
    { 0xe8, 0xe8, 0x00, 0xbd, 0x45, 0x34, 0x98, 0x73, 0x00, 0x00 },
    { 0xea, 0xe8, 0x00, 0xab, 0x3a, 0x3a, 0x70, 0x13, 0x00, 0x00 },
    { 0xeb, 0xe8, 0x00, 0x96, 0x8d, 0x34, 0x98, 0xf3, 0x00, 0x00 },
    { 0xec, 0xe8, 0x00, 0x9b, 0x7d, 0x34, 0x98, 0x8d, 0x00, 0x00 },
    { 0xee, 0xe8, 0x00, 0x9b, 0x46, 0x34, 0x98, 0x82, 0x00, 0x00 },
    { 0xef, 0xe8, 0x00, 0x8c, 0x62, 0x34, 0x98, 0x2a, 0x00, 0x00 },
    { 0xf0, 0xe8, 0x00, 0xbf, 0x28, 0x34, 0x98, 0xe0, 0x00, 0x00 },
    { 0xf5, 0xe8, 0x00, 0xba, 0x39, 0x34, 0x98, 0x46, 0x00, 0x00 },
    { 0xf6, 0xe8, 0x00, 0xa6, 0xd0, 0x3a, 0x70, 0xc3, 0x00, 0x00 },
    { 0xfe, 0xe8, 0x00, 0x8d, 0x53, 0x34, 0x98, 0xe0, 0x00, 0x00 },
    { 0xff, 0xe8, 0x00, 0x97, 0xb6, 0x34, 0x98, 0xb4, 0x00, 0x00 },
};

START_TEST(t2mi_headers)
{
    for (size_t i = 0; i < ASC_ARRAY_SIZE(test_headers); i++)
    {
        const uint8_t *const buf = test_headers[i];
        const size_t buf_size = sizeof(test_headers[0]) - 1;

        const uint8_t expect = buf[0];
        const uint8_t crc = au_crc8(&buf[1], buf_size);
        ck_assert(crc == expect);
    }
}
END_TEST

START_TEST(catch_em_all)
{
    unsigned int got = 0, total = 0;
    bool map[256] = { false };

    while (got < sizeof(map))
    {
        const size_t buf_size = rand() % 10240;
        char *const buf = ASC_ALLOC(buf_size, char);

        for (size_t i = 0; i < buf_size; i++)
            buf[i] = rand();

        const uint8_t crc = au_crc8(buf, buf_size);
        if (!map[crc])
        {
            map[crc] = true;
            got++;
        }

        free(buf);
        total++;
    }

    for (size_t i = 0; i < ASC_ARRAY_SIZE(map); i++)
        ck_assert(map[i] != 0);

    asc_log_info("catch_em_all stats: %u over %u attempts", got, total);
}
END_TEST

/* zero length input */
START_TEST(zero_length)
{
    char empty[1] = { 0 };
    ck_assert(au_crc8(NULL, 0) == 0);
    ck_assert(au_crc8(empty, 0) == 0);
}
END_TEST

Suite *utils_crc8(void)
{
    Suite *const s = suite_create("utils/crc8");

    TCase *const tc = tcase_create("default");
    tcase_add_checked_fixture(tc, lib_setup, lib_teardown);

    tcase_add_test(tc, t2mi_headers);
    tcase_add_test(tc, catch_em_all);
    tcase_add_test(tc, zero_length);

    suite_add_tcase(s, tc);

    return s;
}