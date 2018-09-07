/*
 * Copyright 2018 Oticon A/S
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#include <stdio.h>
#include "modem_magic_args.h"
#include "bs_tracing.h"
#include "bs_oswrap.h"
#include "bs_rand_main.h"
#include "bs_cmd_line_typical.h"

static char library_name[] = "Modem Magic";

void component_print_post_help(){
  fprintf(stdout,"This modem will receive anything no mattter the SNR\n"
      "(you can force a given BER or sync PER with -BER=<BER> -PER=<PER>\n");
}

static mo_magic_args_t *args_g;
static double ber, per;
static uint device_nbr_g;

static void cmd_ber_found(char * argv, int offset) {
  if ((ber < 0) || (ber > 1)) {
    bs_trace_error("modem: %i: (magic) args: BER shall be between 0 and 1 '%s'\n", device_nbr_g, ber, argv);
  }
  args_g->BER = ber*(double)RAND_PROB_1;
  bs_trace_raw(9,"modem: %i: (magic) BER set to %e (%u/%u)\n", device_nbr_g, ber, args_g->BER, RAND_PROB_1);
}

static void cmd_per_found(char * argv, int offset) {
  if ((per < 0) || (per > 1)) {
    bs_trace_error("modem: %i: (magic) args: PER shall be between 0 and 1 '%s'\n", device_nbr_g, per, argv);
  }
  args_g->sync_prob = (1.0-per)*(double)RAND_PROB_1;
  bs_trace_raw(9,"modem: %i: (magic) sync probability (per) set to %u/%u (%e)\n", device_nbr_g, args_g->sync_prob, RAND_PROB_1, per);
}

/**
 * Check the arguments provided in the command line: set args based on it
 * or defaults, and check they are correct
 */
void modem_magic_argparse(int argc, char *argv[], uint dev_nbr, mo_magic_args_t *args)
{
  args_g = args;
  device_nbr_g = dev_nbr;
  bs_args_struct_t args_struct[] = {
    /*manual,mandatory,switch,option,name , type, destination,  callback,       description */
    { false , false  , false, "BER", "ber", 'f', (void*)&ber, cmd_ber_found, "bit error rate (for any type of packet) (0)..1"},
    { false , false  , false, "PER", "per", 'f', (void*)&per, cmd_per_found, "synchronization error rate (for any type of packet), (0)..1"},
    ARG_TABLE_ENDMARKER
  };

  args->sync_prob = RAND_PROB_1;
  args->BER = 0;

  char trace_prefix[50]; //it will not be used as soon as we get out of this function
  snprintf(trace_prefix,50, "modem: %i: (magic) ",dev_nbr);

  bs_args_override_exe_name(library_name);
  bs_args_set_trace_prefix(trace_prefix);
  bs_args_parse_all_cmd_line(argc, argv, args_struct);
}
