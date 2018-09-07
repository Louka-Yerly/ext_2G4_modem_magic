/*
 * Copyright 2018 Oticon A/S
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#ifndef BS_MODEM_MAGIC_ARGS_H
#define BS_MODEM_MAGIC_ARGS_H

#include "bs_types.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct{
  uint32_t sync_prob;
  uint32_t BER;
  uint32_t nbr_devices;
} mo_magic_args_t;

void modem_magic_argparse(int argc, char *argv[], uint dev_nbr, mo_magic_args_t *args);

#ifdef __cplusplus
}
#endif

#endif
