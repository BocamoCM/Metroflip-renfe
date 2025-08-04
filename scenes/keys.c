#include "../metroflip_i.h"
#include "keys.h"
#include <bit_lib.h>
#include <nfc/protocols/mf_classic/mf_classic.h>
#include <nfc/protocols/mf_classic/mf_classic_poller.h>
#include <nfc/nfc.h>
#include <nfc/protocols/mf_classic/mf_classic_poller_sync.h>
#include <string.h>

#define TAG "keys_check"

const MfClassicKeyPair troika_1k_key[] = {
    {.a = 0x08b386463229},
};

const MfClassicKeyPair troika_4k_key[] = {
    {.a = 0x08b386463229},
};

const MfClassicKeyPair smartrider_verify_key[] = {
    {.a = 0x2031D1E57A3B},
};

const MfClassicKeyPair charliecard_1k_verify_key[] = {
    {.a = 0x5EC39B022F2B},
};

const MfClassicKeyPair bip_1k_verify_key[] = {
    {.a = 0x3a42f33af429},
};

const MfClassicKeyPair metromoney_1k_verify_key[] = {
    {.a = 0x9C616585E26D},
};
const MfClassicKeyPair renfe_suma10_1k_keys[] = {
    {.a = 0xA8844B0BCA06}, // Sector 0 - RENFE specific key from dumps
    {.a = 0xCB5ED0E57B08}, // Sector 1 - RENFE specific key from dumps  
    {.a = 0xffffffffffff}, // Default key for other sectors
    {.a = 0xa0a1a2a3a4a5}, // Alternative common key
    {.a = 0xC0C1C2C3C4C5}, // Key for sectors 10-15 from dumps
};

// RENFE Regular verification keys - extracted from dump analysis
const uint8_t gocard_verify_data[1][14] = {
    {0x16, 0x18, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F, 0x5A, 0x5B, 0x20, 0x21, 0x22, 0x23}};

const uint8_t gocard_verify_data2[1][14] = {
    {0x16, 0x18, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F, 0x20, 0x21, 0x22, 0x23, 0x01, 0x01}};

static bool charliecard_verify(Nfc* nfc, MfClassicData* mfc_data, bool data_loaded) {
    bool verified = false;
    FURI_LOG_I(TAG, "verifying charliecard..");
    const uint8_t verify_sector = 1;
    do {
        if(!data_loaded) {
            const uint8_t verify_block =
                mf_classic_get_first_block_num_of_sector(verify_sector) + 1;
            FURI_LOG_I(TAG, "Verifying sector %u", verify_sector);

            MfClassicKey key = {0};
            bit_lib_num_to_bytes_be(charliecard_1k_verify_key[0].a, COUNT_OF(key.data), key.data);

            MfClassicAuthContext auth_context;
            MfClassicError error = mf_classic_poller_sync_auth(
                nfc, verify_block, &key, MfClassicKeyTypeA, &auth_context);
            if(error != MfClassicErrorNone) {
                FURI_LOG_I(TAG, "Failed to read block %u: %d", verify_block, error);
                break;
            }

            verified = true;
        } else {
            MfClassicSectorTrailer* sec_tr =
                mf_classic_get_sector_trailer_by_sector(mfc_data, verify_sector);
            FURI_LOG_I(TAG, "%2x", sec_tr->key_a.data[1]);
            uint64_t key = bit_lib_bytes_to_num_be(sec_tr->key_a.data, 6);
            if(key != charliecard_1k_verify_key[0].a) {
                FURI_LOG_I(TAG, "not equall");
                break;
            }

            verified = true;
        }
    } while(false);

    return verified;
}

bool bip_verify(Nfc* nfc, MfClassicData* mfc_data, bool data_loaded) {
    bool verified = false;

    do {
        if(!data_loaded) {
            const uint8_t verify_sector = 0;
            uint8_t block_num = mf_classic_get_first_block_num_of_sector(verify_sector);
            FURI_LOG_I(TAG, "Verifying sector %u", verify_sector);

            MfClassicKey key = {};
            bit_lib_num_to_bytes_be(bip_1k_verify_key[0].a, COUNT_OF(key.data), key.data);

            MfClassicAuthContext auth_ctx = {};
            MfClassicError error =
                mf_classic_poller_sync_auth(nfc, block_num, &key, MfClassicKeyTypeA, &auth_ctx);

            if(error != MfClassicErrorNone) {
                FURI_LOG_I(TAG, "Failed to read block %u: %d", block_num, error);
                break;
            }

            verified = true;
        } else {
            MfClassicSectorTrailer* sec_tr = mf_classic_get_sector_trailer_by_sector(mfc_data, 0);

            uint64_t key = bit_lib_bytes_to_num_be(sec_tr->key_a.data, 6);
            if(key != bip_1k_verify_key[0].a) {
                break;
            }

            verified = true;
        }
    } while(false);

    return verified;
}

static bool metromoney_verify(Nfc* nfc, MfClassicData* mfc_data, bool data_loaded) {
    bool verified = false;
    const uint8_t ticket_sector_number = 1;
    do {
        if(!data_loaded) {
            const uint8_t ticket_block_number =
                mf_classic_get_first_block_num_of_sector(ticket_sector_number) + 1;
            FURI_LOG_D(TAG, "Verifying sector %u", ticket_sector_number);

            MfClassicKey key = {0};
            bit_lib_num_to_bytes_be(metromoney_1k_verify_key[0].a, COUNT_OF(key.data), key.data);

            MfClassicAuthContext auth_context;
            MfClassicError error = mf_classic_poller_sync_auth(
                nfc, ticket_block_number, &key, MfClassicKeyTypeA, &auth_context);
            if(error != MfClassicErrorNone) {
                FURI_LOG_D(TAG, "Failed to read block %u: %d", ticket_block_number, error);
                break;
            }

            verified = true;
        } else {
            MfClassicSectorTrailer* sec_tr =
                mf_classic_get_sector_trailer_by_sector(mfc_data, ticket_sector_number);

            uint64_t key = bit_lib_bytes_to_num_be(sec_tr->key_a.data, 6);
            if(key != metromoney_1k_verify_key[0].a) {
                break;
            }

            verified = true;
        }
    } while(false);

    return verified;
}

static bool smartrider_verify(Nfc* nfc, MfClassicData* mfc_data, bool data_loaded) {
    bool verified = false;

    do {
        if(!data_loaded) {
            const uint8_t block_number = mf_classic_get_first_block_num_of_sector(0) + 1;
            FURI_LOG_D(TAG, "Verifying sector 0");

            MfClassicKey key = {0};
            bit_lib_num_to_bytes_be(smartrider_verify_key[0].a, COUNT_OF(key.data), key.data);

            MfClassicAuthContext auth_context;
            MfClassicError error = mf_classic_poller_sync_auth(
                nfc, block_number, &key, MfClassicKeyTypeA, &auth_context);
            if(error != MfClassicErrorNone) {
                FURI_LOG_D(TAG, "Failed to read block %u: %d", block_number, error);
                break;
            }

            verified = true;
        } else {
            MfClassicSectorTrailer* sec_tr = mf_classic_get_sector_trailer_by_sector(mfc_data, 0);

            uint64_t key = bit_lib_bytes_to_num_be(sec_tr->key_a.data, 6);
            if(key != smartrider_verify_key[0].a) {
                break;
            }

            verified = true;
        }
    } while(false);

    return verified;
}

static bool troika_get_card_config(TroikaCardConfig* config, MfClassicType type) {
    bool success = true;

    if(type == MfClassicType1k) {
        config->data_sector = 11;
        config->keys = troika_1k_key;
    } else if(type == MfClassicType4k) {
        config->data_sector = 8; // Further testing needed
        config->keys = troika_4k_key;
    } else {
        success = false;
    }

    return success;
}

static bool
    troika_verify_type(Nfc* nfc, MfClassicData* mfc_data, bool data_loaded, MfClassicType type) {
    bool verified = false;

    do {
        if(!data_loaded) {
            TroikaCardConfig cfg = {};
            if(!troika_get_card_config(&cfg, type)) break;

            const uint8_t block_num = mf_classic_get_first_block_num_of_sector(cfg.data_sector);
            FURI_LOG_D(TAG, "Verifying sector %lu", cfg.data_sector);

            MfClassicKey key = {0};
            bit_lib_num_to_bytes_be(cfg.keys[0].a, COUNT_OF(key.data), key.data);

            MfClassicAuthContext auth_context;
            MfClassicError error = mf_classic_poller_sync_auth(
                nfc, block_num, &key, MfClassicKeyTypeA, &auth_context);
            if(error != MfClassicErrorNone) {
                FURI_LOG_D(TAG, "Failed to read block %u: %d", block_num, error);
                break;
            }
            FURI_LOG_D(TAG, "Verify success!");
            verified = true;
        } else {
            TroikaCardConfig cfg = {};
            if(!troika_get_card_config(&cfg, type)) break;
            MfClassicSectorTrailer* sec_tr =
                mf_classic_get_sector_trailer_by_sector(mfc_data, cfg.data_sector);

            uint64_t key = bit_lib_bytes_to_num_be(sec_tr->key_a.data, 6);
            if(key != cfg.keys[0].a) {
                break;
            }

            verified = true;
        }
    } while(false);

    return verified;
}

static bool troika_verify(Nfc* nfc, MfClassicData* mfc_data, bool data_loaded) {
    return troika_verify_type(nfc, mfc_data, data_loaded, MfClassicType1k) ||
           troika_verify_type(nfc, mfc_data, data_loaded, MfClassicType4k);
}


static bool gocard_verify(MfClassicData* mfc_data, bool data_loaded) {
    bool verified = false;
    FURI_LOG_I(TAG, "verifying charliecard..");
    do {
        if(data_loaded) {
            uint8_t* buffer = &mfc_data->block[1].data[1];
            size_t buffer_size = 14;

            if(memcmp(buffer, gocard_verify_data[0], buffer_size) == 0) {
                FURI_LOG_I(TAG, "Match!");
            } else {
                FURI_LOG_I(TAG, "No match.");
                if(memcmp(buffer, gocard_verify_data2[0], buffer_size) == 0) {
                    FURI_LOG_I(TAG, "Match!");
                } else {
                    FURI_LOG_I(TAG, "No match.");
                    break;
                }
            }

            verified = true;
        }
    } while(false);

    return verified;
}
static bool renfe_suma10_verify(Nfc* nfc, MfClassicData* mfc_data, bool data_loaded) {
    FURI_LOG_I(TAG, "renfe_suma10_verify: ENTRY - data_loaded=%s", data_loaded ? "true" : "false");
    bool verified = false;
    
    do {
        if(!data_loaded) {
           
            if(!nfc) {
                FURI_LOG_E(TAG, "renfe_suma10_verify: NFC is null, failing");
                break;
            }
            
            FURI_LOG_I(TAG, "renfe_suma10_verify: LIVE MODE - trying RENFE Suma 10 authentication...");
            
            const uint8_t block_num = 0; 
            MfClassicKey key = {0};
            bit_lib_num_to_bytes_be(renfe_suma10_1k_keys[0].a, COUNT_OF(key.data), key.data);

            MfClassicAuthContext auth_context;
            MfClassicError error = mf_classic_poller_sync_auth(
                nfc, block_num, &key, MfClassicKeyTypeA, &auth_context);
            
            if(error != MfClassicErrorNone) {
                FURI_LOG_I(TAG, "renfe_suma10_verify: RENFE key authentication failed on sector 0: %d", error);
                
                bit_lib_num_to_bytes_be(renfe_suma10_1k_keys[1].a, COUNT_OF(key.data), key.data);
                error = mf_classic_poller_sync_auth(
                    nfc, 4, &key, MfClassicKeyTypeA, &auth_context); 
                
                if(error != MfClassicErrorNone) {
                    FURI_LOG_I(TAG, "renfe_suma10_verify: RENFE sector 1 key also failed: %d", error);
                    
              
                    bit_lib_num_to_bytes_be(0xffffffffffff, COUNT_OF(key.data), key.data);
                    error = mf_classic_poller_sync_auth(
                        nfc, block_num, &key, MfClassicKeyTypeA, &auth_context);
                    
                    if(error != MfClassicErrorNone) {
                        FURI_LOG_I(TAG, "renfe_suma10_verify: Default key also failed: %d", error);
                        break;
                    }
                    
                    FURI_LOG_I(TAG, "renfe_suma10_verify: Default key worked, but not specific enough for RENFE");
                    
                    break;
                } else {
                    FURI_LOG_I(TAG, "renfe_suma10_verify: RENFE sector 1 key authentication SUCCESS!");
                    verified = true;
                }
            } else {
                FURI_LOG_I(TAG, "renfe_suma10_verify: RENFE sector 0 key authentication SUCCESS!");
                verified = true;
            }
            
            FURI_LOG_I(TAG, "renfe_suma10_verify: Live verification result = %s", verified ? "true" : "false");
            
        } else {
            if(!mfc_data) {
                FURI_LOG_E(TAG, "RENFE Suma 10 - mfc_data is null");
                break;
            }
            
            FURI_LOG_I(TAG, "RENFE Suma 10 - analyzing loaded data patterns");
            
            // RENFE Suma 10 cards should be treated as 1K even if they're Mifare Plus 2K
            // Force them to be treated as 1K cards for compatibility
            if(mfc_data->type != MfClassicType1k) {
                FURI_LOG_I(TAG, "RENFE Suma 10 - card type is %d, but RENFE Suma 10 should be treated as 1K", mfc_data->type);
                // For RENFE Suma 10, we'll accept any type but treat it as 1K
                // This handles Mifare Plus 2K configured as 1K
            }
            
            FURI_LOG_I(TAG, "RENFE Suma 10 - found card (actual type: %d), treating as 1K for RENFE compatibility...", mfc_data->type);
            
            bool has_renfe_sector0_key = false;
            bool has_renfe_sector1_key = false;
            bool has_renfe_block5_pattern = false;
            
            MfClassicSectorTrailer* sec_tr0 = mf_classic_get_sector_trailer_by_sector(mfc_data, 0);
            if(sec_tr0) {
                uint64_t key0 = bit_lib_bytes_to_num_be(sec_tr0->key_a.data, 6);
                FURI_LOG_I(TAG, "RENFE Suma 10 - sector 0 key: %012llX", key0);
                
                if(key0 == 0xA8844B0BCA06) {
                    FURI_LOG_I(TAG, "RENFE Suma 10 - sector 0 has RENFE key!");
                    has_renfe_sector0_key = true;
                }
            }
            
            MfClassicSectorTrailer* sec_tr1 = mf_classic_get_sector_trailer_by_sector(mfc_data, 1);
            if(sec_tr1) {
                uint64_t key1 = bit_lib_bytes_to_num_be(sec_tr1->key_a.data, 6);
                FURI_LOG_I(TAG, "RENFE Suma 10 - sector 1 key: %012llX", key1);
                
                if(key1 == 0xCB5ED0E57B08) {
                    FURI_LOG_I(TAG, "RENFE Suma 10 - sector 1 has RENFE key!");
                    has_renfe_sector1_key = true;
                }
            }
            
            FURI_LOG_I(TAG, "RENFE Suma 10 - checking block 5 pattern: %02X %02X %02X %02X", 
                       mfc_data->block[5].data[0], mfc_data->block[5].data[1], 
                       mfc_data->block[5].data[2], mfc_data->block[5].data[3]);
                       
            if(mfc_data->block[5].data[0] == 0x01 && 
               mfc_data->block[5].data[1] == 0x00 && 
               mfc_data->block[5].data[2] == 0x00 && 
               mfc_data->block[5].data[3] == 0x00) {
                FURI_LOG_I(TAG, "RENFE Suma 10 - block 5 has RENFE value pattern!");
                has_renfe_block5_pattern = true;
            }
            
            if((has_renfe_sector0_key || has_renfe_sector1_key) && has_renfe_block5_pattern) {
                FURI_LOG_I(TAG, "RENFE Suma 10 - VERIFIED! Has RENFE keys and block 5 pattern");
                verified = true;
            } else if(has_renfe_sector0_key && has_renfe_sector1_key) {
                FURI_LOG_I(TAG, "RENFE Suma 10 - VERIFIED! Has both RENFE sector keys");
                verified = true;
            } else {
                FURI_LOG_I(TAG, "RENFE Suma 10 - NOT VERIFIED: missing RENFE patterns (sector0_key=%s, sector1_key=%s, block5_pattern=%s)",
                           has_renfe_sector0_key ? "YES" : "NO",
                           has_renfe_sector1_key ? "YES" : "NO", 
                           has_renfe_block5_pattern ? "YES" : "NO");
                verified = false;
            }
        }
    } while(false);

    FURI_LOG_I(TAG, "renfe_suma10_verify: RESULT = %s", verified ? "SUCCESS" : "FAILED");
    return verified;
}

static bool renfe_regular_verify(Nfc* nfc, MfClassicData* mfc_data, bool data_loaded) {
    FURI_LOG_I(TAG, "renfe_regular_verify: ENTRY - data_loaded=%s", data_loaded ? "true" : "false");
    bool verified = false;
    
    do {
        if(!data_loaded) {
            if(!nfc) {
                FURI_LOG_E(TAG, "renfe_regular_verify: NFC is null, failing");
                break;
            }
            
            FURI_LOG_I(TAG, "renfe_regular_verify: LIVE MODE - trying RENFE Regular authentication...");
            
            const uint8_t block_num = 0; 
            MfClassicKey key = {0};
            bit_lib_num_to_bytes_be(0x37E0DB717D08, COUNT_OF(key.data), key.data);

            MfClassicAuthContext auth_context;
            MfClassicError error = mf_classic_poller_sync_auth(
                nfc, block_num, &key, MfClassicKeyTypeA, &auth_context);
            
            if(error != MfClassicErrorNone) {
                FURI_LOG_I(TAG, "renfe_regular_verify: RENFE Regular sector 0 key failed: %d", error);
                
                bit_lib_num_to_bytes_be(0x421BFA445657, COUNT_OF(key.data), key.data);
                error = mf_classic_poller_sync_auth(
                    nfc, 4, &key, MfClassicKeyTypeA, &auth_context); 
                
                if(error != MfClassicErrorNone) {
                    FURI_LOG_I(TAG, "renfe_regular_verify: RENFE Regular sector 1 key also failed: %d", error);
                    
               
                    bit_lib_num_to_bytes_be(0xffffffffffff, COUNT_OF(key.data), key.data);
                    error = mf_classic_poller_sync_auth(
                        nfc, block_num, &key, MfClassicKeyTypeA, &auth_context);
                    
                    if(error == MfClassicErrorNone) {
                        FURI_LOG_I(TAG, "renfe_regular_verify: Found Mifare Classic with default keys - could be RENFE Regular");
                      
                        verified = true;
                    } else {
                        bit_lib_num_to_bytes_be(0xa0a1a2a3a4a5, COUNT_OF(key.data), key.data);
                        error = mf_classic_poller_sync_auth(
                            nfc, block_num, &key, MfClassicKeyTypeA, &auth_context);
                        
                        if(error == MfClassicErrorNone) {
                            FURI_LOG_I(TAG, "renfe_regular_verify: Found Mifare Classic with common keys - could be RENFE Regular");
                            verified = true;
                        } else {
                            FURI_LOG_I(TAG, "renfe_regular_verify: No common key access - checking if this could be protected RENFE Regular");
                       
                            int auth_failures = 0;
                            bit_lib_num_to_bytes_be(0xffffffffffff, COUNT_OF(key.data), key.data);
                            
                            for(int test_sector = 0; test_sector < 4; test_sector++) {
                                int test_block = test_sector * 4;
                                MfClassicError test_error = mf_classic_poller_sync_auth(
                                    nfc, test_block, &key, MfClassicKeyTypeA, &auth_context);
                                if(test_error == MfClassicErrorAuth || test_error == MfClassicErrorTimeout) {
                                    auth_failures++;
                                }
                            }
                            
                            if(auth_failures >= 3) {
                                FURI_LOG_I(TAG, "renfe_regular_verify: Multiple sectors protected - likely RENFE Regular with unique keys");
                                verified = true;
                            } else {
                                FURI_LOG_I(TAG, "renfe_regular_verify: Card behavior doesn't match RENFE Regular");
                            }
                        }
                    }
                } else {
                    FURI_LOG_I(TAG, "renfe_regular_verify: RENFE Regular sector 1 key authentication SUCCESS!");
                    verified = true;
                }
            } else {
                FURI_LOG_I(TAG, "renfe_regular_verify: RENFE Regular sector 0 key authentication SUCCESS!");
                verified = true;
            }
            
        } else {
            if(!mfc_data) {
                FURI_LOG_E(TAG, "RENFE Regular - mfc_data is null");
                break;
            }
            
            FURI_LOG_I(TAG, "RENFE Regular - analyzing loaded data patterns");
            
            bool has_renfe_regular_sector0_key = false;
            bool has_renfe_regular_sector1_key = false;
            bool has_renfe_regular_pattern = false;
            bool has_protected_sectors = false;
            
            MfClassicSectorTrailer* sec_tr0 = mf_classic_get_sector_trailer_by_sector(mfc_data, 0);
            if(sec_tr0) {
                uint64_t key0 = bit_lib_bytes_to_num_be(sec_tr0->key_a.data, 6);
                FURI_LOG_I(TAG, "RENFE Regular - sector 0 key: %012llX", key0);
                
                if(key0 == 0x37E0DB717D08) {
                    FURI_LOG_I(TAG, "RENFE Regular - sector 0 has REAL RENFE Regular key!");
                    has_renfe_regular_sector0_key = true;
                } else if(key0 != 0xFFFFFFFFFFFF && key0 != 0xA0A1A2A3A4A5 && key0 != 0x000000000000) {
                    FURI_LOG_I(TAG, "RENFE Regular - sector 0 has non-default key, could be RENFE Regular");
                    has_protected_sectors = true;
                }
            }
            
            MfClassicSectorTrailer* sec_tr1 = mf_classic_get_sector_trailer_by_sector(mfc_data, 1);
            if(sec_tr1) {
                uint64_t key1 = bit_lib_bytes_to_num_be(sec_tr1->key_a.data, 6);
                FURI_LOG_I(TAG, "RENFE Regular - sector 1 key: %012llX", key1);
                
                if(key1 == 0x421BFA445657) {
                    FURI_LOG_I(TAG, "RENFE Regular - sector 1 has REAL RENFE Regular key!");
                    has_renfe_regular_sector1_key = true;
                } else if(key1 != 0xFFFFFFFFFFFF && key1 != 0xA0A1A2A3A4A5 && key1 != 0x000000000000) {
                    FURI_LOG_I(TAG, "RENFE Regular - sector 1 has non-default key, could be RENFE Regular");
                    has_protected_sectors = true;
                }
            }
         
            if(mf_classic_is_block_read(mfc_data, 1)) {
                const uint8_t* block1 = mfc_data->block[1].data;
                if(block1[0] == 0x42 && block1[1] == 0x1B && block1[2] == 0xFA && block1[3] == 0x44) {
                    FURI_LOG_I(TAG, "RENFE Regular - block 1 has REAL RENFE Regular pattern!");
                    has_renfe_regular_pattern = true;
                }
            }
            
            if(mf_classic_is_block_read(mfc_data, 2)) {
                const uint8_t* block2 = mfc_data->block[2].data;
                if(block2[0] == 0x06 && block2[1] == 0x00 && block2[2] == 0x5C && block2[3] == 0x9F) {
                    FURI_LOG_I(TAG, "RENFE Regular - block 2 has REAL RENFE network pattern!");
                    has_renfe_regular_pattern = true;
                }
            }
            
            if(mf_classic_is_block_read(mfc_data, 0)) {
                const uint8_t* block0 = mfc_data->block[0].data;
                if(block0[0] == 0x37 || block0[0] == 0xB7) { 
                    FURI_LOG_I(TAG, "RENFE Regular - UID matches known RENFE Regular pattern");
                    has_renfe_regular_pattern = true;
                }
            }
            
            if(has_renfe_regular_sector0_key || has_renfe_regular_sector1_key) {
                FURI_LOG_I(TAG, "RENFE Regular - VERIFIED! Has RENFE Regular keys");
                verified = true;
            } else if(has_protected_sectors && has_renfe_regular_pattern) {
                FURI_LOG_I(TAG, "RENFE Regular - VERIFIED! Has protected sectors with RENFE patterns");
                verified = true;
            } else if(has_protected_sectors) {
                FURI_LOG_I(TAG, "RENFE Regular - PROBABLY VERIFIED! Has protected sectors (likely RENFE Regular)");
                verified = true;
            } else {
                FURI_LOG_I(TAG, "RENFE Regular - NOT VERIFIED: missing RENFE Regular patterns");
                verified = false;
            }
        }
    } while(false);

    FURI_LOG_I(TAG, "renfe_regular_verify: RESULT = %s", verified ? "SUCCESS" : "FAILED");
    return verified;
}

CardType determine_card_type(Nfc* nfc, MfClassicData* mfc_data, bool data_loaded) {
    FURI_LOG_I(TAG, "checking keys..");
    UNUSED(bip_verify);

    if(bip_verify(nfc, mfc_data, data_loaded)) {
        return CARD_TYPE_BIP;
    } else if(metromoney_verify(nfc, mfc_data, data_loaded)) {
        return CARD_TYPE_METROMONEY;
    } else if(smartrider_verify(nfc, mfc_data, data_loaded)) {
        return CARD_TYPE_SMARTRIDER;
    } else if(renfe_suma10_verify(nfc, mfc_data, data_loaded)) {
        return CARD_TYPE_RENFE_SUM10;
    } else if(troika_verify(nfc, mfc_data, data_loaded)) {
        return CARD_TYPE_TROIKA;
    } else if(charliecard_verify(nfc, mfc_data, data_loaded)) {
        return CARD_TYPE_CHARLIECARD;
    } else if(gocard_verify(mfc_data, data_loaded)) {
        return CARD_TYPE_GOCARD;
    } else if(renfe_regular_verify(nfc, mfc_data, data_loaded)) {
        return CARD_TYPE_RENFE_REGULAR;
    } else {
        if(!data_loaded && nfc) {
            FURI_LOG_I(TAG, "No known cards detected - checking if this could be a protected RENFE Regular");
            
            MfClassicKey key = {0};
            bit_lib_num_to_bytes_be(0xffffffffffff, COUNT_OF(key.data), key.data);
            
            MfClassicAuthContext auth_context;
            MfClassicError error = mf_classic_poller_sync_auth(
                nfc, 0, &key, MfClassicKeyTypeA, &auth_context);
            
            if(error == MfClassicErrorTimeout || error == MfClassicErrorAuth) {
                FURI_LOG_I(TAG, "Card rejects default keys - could be protected RENFE Regular");
                
                int failed_auths = 0;
                for(int sector = 0; sector < 4; sector++) {
                    int block = sector * 4;
                    error = mf_classic_poller_sync_auth(
                        nfc, block, &key, MfClassicKeyTypeA, &auth_context);
                    if(error != MfClassicErrorNone) {
                        failed_auths++;
                    }
                }
                
                if(failed_auths >= 3) {
                    FURI_LOG_I(TAG, "Multiple sectors protected - likely RENFE Regular with unique keys");
                    return CARD_TYPE_RENFE_REGULAR;
                }
            }
        }
        
        FURI_LOG_I(TAG, "its unknown");
        return CARD_TYPE_UNKNOWN;
    }
}
