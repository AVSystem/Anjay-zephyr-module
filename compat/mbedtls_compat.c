/*
 * Copyright 2026 AVSystem <avsystem@avsystem.com>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <mbedtls/entropy.h>
#include <mbedtls/platform.h>
#include <mbedtls/timing.h>

#include <avsystem/commons/avs_defs.h>
#include <avsystem/commons/avs_errno.h>
#include <avsystem/commons/avs_memory.h>
#include <avsystem/commons/avs_time.h>

#include <version.h>
#include <zephyr/drivers/entropy.h>
#include <zephyr/random/random.h>

typedef struct anjay_mbedtls_timing_delay_context_struct {
    avs_time_monotonic_t timer;
    uint32_t int_ms;
    uint32_t fin_ms;
} anjay_mbedtls_timing_delay_context_t;

/*
 * Set delays to watch
 */
void mbedtls_timing_set_delay(void *data, uint32_t int_ms, uint32_t fin_ms) {
    anjay_mbedtls_timing_delay_context_t *ctx =
            (anjay_mbedtls_timing_delay_context_t *) data;

    ctx->int_ms = int_ms;
    ctx->fin_ms = fin_ms;

    if (fin_ms != 0) {
        ctx->timer = avs_time_monotonic_now();
    }
}

/*
 * Get number of delays expired
 */
int mbedtls_timing_get_delay(void *data) {
    anjay_mbedtls_timing_delay_context_t *ctx =
            (anjay_mbedtls_timing_delay_context_t *) data;

    if (ctx->fin_ms == 0) {
        return -1;
    }

    int64_t elapsed_ms_signed;
    if (avs_time_duration_to_scalar(
                &elapsed_ms_signed, AVS_TIME_MS,
                avs_time_monotonic_diff(avs_time_monotonic_now(),
                                        ctx->timer))) {
        return -1;
    }

    assert(elapsed_ms_signed >= 0);
    uint64_t elapsed_ms = (uint64_t) elapsed_ms_signed;
    if (elapsed_ms >= ctx->fin_ms) {
        return 2;
    } else if (elapsed_ms >= ctx->int_ms) {
        return 1;
    } else {
        return 0;
    }
}
