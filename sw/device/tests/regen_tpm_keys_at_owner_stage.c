// Copyright lowRISC contributors (OpenTitan project).
// Licensed under the Apache License, Version 2.0, see LICENSE for details.
// SPDX-License-Identifier: Apache-2.0

#include "sw/device/lib/arch/boot_stage.h"
#include "sw/device/lib/runtime/log.h"
#include "sw/device/lib/testing/test_framework/check.h"
#include "sw/device/lib/testing/test_framework/ottf_main.h"
#include "sw/device/silicon_creator/lib/attestation_key_diversifiers.h"
#include "sw/device/silicon_creator/lib/dice.h"
#include "sw/device/silicon_creator/lib/error.h"
#include "sw/device/silicon_creator/lib/otbn_boot_services.h"

#include "hw/top_earlgrey/sw/autogen/top_earlgrey.h"

OTTF_DEFINE_TEST_CONFIG();

static hmac_digest_t tpm_pubkey_id;
static attestation_public_key_t pubkey = {.x = {0}, .y = {0}};

static status_t test_tpm_keygen_at_owner_stage(void) {
  TRY_CHECK(kBootStage == kBootStageOwner);
  TRY(otbn_boot_app_load());
  TRY(dice_attestation_keygen(kDiceKeyTpmEk, &tpm_pubkey_id, &pubkey));
  TRY(otbn_boot_attestation_key_save(kTpmEkAttestationKeySeed,
                                     kOtbnBootAttestationKeyTypeTpm,
                                     kTpmEkKeymgrDiversifier));
  return OK_STATUS();
}

bool test_main(void) {
  CHECK_STATUS_OK(test_tpm_keygen_at_owner_stage());
  return true;
}
