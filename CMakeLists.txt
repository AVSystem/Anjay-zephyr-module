# Copyright 2026 AVSystem <avsystem@avsystem.com>
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

if(CONFIG_ANJAY_ZEPHYR)
    zephyr_library()

    file(GLOB_RECURSE ANJAY_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/deps/anjay/src/*.c")
    file(GLOB_RECURSE AVS_COMMONS_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/deps/anjay/deps/avs_commons/src/*.c")
    file(GLOB_RECURSE AVS_COAP_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/deps/anjay/deps/avs_coap/src/*.c")

    zephyr_library_sources(
        ${ANJAY_SOURCES}
        ${AVS_COMMONS_SOURCES}
        ${AVS_COAP_SOURCES})

    zephyr_include_directories(${CMAKE_CURRENT_SOURCE_DIR}/deps/anjay/deps/avs_coap/include_public
                               ${CMAKE_CURRENT_SOURCE_DIR}/deps/anjay/deps/avs_commons/include_public
                               ${CMAKE_CURRENT_SOURCE_DIR}/deps/anjay/include_public
                               ${CMAKE_CURRENT_SOURCE_DIR}/compat/anjay_zephyr_logger
                               )

    target_include_directories(${ZEPHYR_CURRENT_LIBRARY}
                               PRIVATE
                               ${CMAKE_CURRENT_SOURCE_DIR}/deps/anjay/deps/avs_coap/src
                               ${CMAKE_CURRENT_SOURCE_DIR}/deps/anjay/deps/avs_commons/src
                               ${CMAKE_CURRENT_SOURCE_DIR}/deps/anjay/src
                               ${CMAKE_CURRENT_SOURCE_DIR}/compat
                               )

    zephyr_library_sources_ifdef(CONFIG_ANJAY_ZEPHYR_LOG_COMPAT_DEFAULT "${CMAKE_CURRENT_SOURCE_DIR}/compat/anjay_zephyr_logger/log_impl.c")
    zephyr_library_sources_ifdef(CONFIG_ANJAY_ZEPHYR_THREADING_COMPAT_DEFAULT "${CMAKE_CURRENT_SOURCE_DIR}/compat/threading_compat.c")
    zephyr_library_sources_ifdef(CONFIG_ANJAY_ZEPHYR_NET_COMPAT_DEFAULT "${CMAKE_CURRENT_SOURCE_DIR}/compat/net_impl.c")
    zephyr_library_sources_ifdef(CONFIG_ANJAY_ZEPHYR_MBEDTLS_COMPAT_DEFAULT "${CMAKE_CURRENT_SOURCE_DIR}/compat/mbedtls_compat.c")
    zephyr_library_sources_ifdef(CONFIG_ANJAY_ZEPHYR_TIME_COMPAT_DEFAULT "${CMAKE_CURRENT_SOURCE_DIR}/compat/time_compat.c")

endif()
