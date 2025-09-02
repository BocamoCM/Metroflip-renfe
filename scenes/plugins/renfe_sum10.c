#include <flipper_application.h>
#include "../../metroflip_i.h"

#include <nfc/protocols/mf_classic/mf_classic_poller_sync.h>
#include <nfc/protocols/mf_classic/mf_classic.h>
#include <nfc/protocols/mf_classic/mf_classic_poller.h>
#include "../../api/metroflip/metroflip_api.h"

#include <dolphin/dolphin.h>
#include <bit_lib.h>
#include <furi_hal.h>
#include <nfc/nfc.h>
#include <nfc/nfc_device.h>
#include <nfc/nfc_listener.h>
#include <storage/storage.h>
#include <ctype.h>
#include "../../api/metroflip/metroflip_api.h"
#include "../../metroflip_plugins.h"

#define TAG "Metroflip:Scene:RenfeSum10"

// Zone codes for Valencia SUMA 10 system
typedef enum {
    ZONE_A = 0,
    ZONE_B = 1,
    ZONE_C = 2,
    ZONE_D = 3,
    ZONE_E = 4,
    ZONE_F = 5,
    ZONE_UNKNOWN = 255
} RenfeSum10Zone;

// Forward declarations
static bool renfe_sum10_parse(FuriString* parsed_data, const MfClassicData* data);

static bool renfe_sum10_parse(FuriString* parsed_data, const MfClassicData* data) {
    if(!parsed_data || !data) {
        FURI_LOG_E(TAG, "Invalid parameters");
        return false;
    }

    furi_string_cat_printf(parsed_data, "\e#RENFE Suma 10\n");
    
    // 1. Extract and show UID
    if(data->iso14443_3a_data && data->iso14443_3a_data->uid_len > 0) {
        const uint8_t* uid = data->iso14443_3a_data->uid;
        size_t uid_len = data->iso14443_3a_data->uid_len;
        
        furi_string_cat_printf(parsed_data, "🔢 UID: ");
        for(size_t i = 0; i < uid_len; i++) {
            furi_string_cat_printf(parsed_data, "%02X", uid[i]);
            if(i < uid_len - 1) furi_string_cat_printf(parsed_data, " ");
        }
        furi_string_cat_printf(parsed_data, "\n");
    } else {
        // Try to extract UID from block 0 as fallback
        if(mf_classic_is_block_read(data, 0)) {
            const uint8_t* block0 = data->block[0].data;
            furi_string_cat_printf(parsed_data, "🔢 UID: %02X %02X %02X %02X\n", 
                                 block0[0], block0[1], block0[2], block0[3]);
        } else {
            furi_string_cat_printf(parsed_data, "🔢 UID: Not available\n");
        }
    }
    
    // 2. Zone extraction - Check block 5 for zone information
    if(mf_classic_is_block_read(data, 5)) {
        const uint8_t* block5 = data->block[5].data;
        
        // Extract zone information from block 5 (Valencia SUMA 10 format)
        uint8_t zone_byte = block5[0];
        RenfeSum10Zone zone = ZONE_UNKNOWN;
        
        if(zone_byte <= ZONE_F) {
            zone = (RenfeSum10Zone)zone_byte;
        }
        
        switch(zone) {
            case ZONE_A:
                furi_string_cat_printf(parsed_data, "🎯 Zone: A\n");
                break;
            case ZONE_B:
                furi_string_cat_printf(parsed_data, "🎯 Zone: B\n");
                break;
            case ZONE_C:
                furi_string_cat_printf(parsed_data, "🎯 Zone: C\n");
                break;
            case ZONE_D:
                furi_string_cat_printf(parsed_data, "🎯 Zone: D\n");
                break;
            case ZONE_E:
                furi_string_cat_printf(parsed_data, "🎯 Zone: E\n");
                break;
            case ZONE_F:
                furi_string_cat_printf(parsed_data, "🎯 Zone: F\n");
                break;
            default:
                furi_string_cat_printf(parsed_data, "🎯 Zone: Unknown (0x%02X)\n", zone_byte);
                break;
        }
    } else {
        furi_string_cat_printf(parsed_data, "🎯 Zone: Not available\n");
    }
    
    furi_string_cat_printf(parsed_data, "\n📍 Region: Valencia\n");
    furi_string_cat_printf(parsed_data, "💳 Format: SUMA 10\n");
    
    return true;
}

// Plugin event handlers
static void renfe_sum10_on_enter(Metroflip* app) {
    if(!app) {
        FURI_LOG_E(TAG, "renfe_sum10_on_enter: app is NULL");
        return;
    }
    
    FURI_LOG_I(TAG, "RENFE Suma 10 plugin entered");
    dolphin_deed(DolphinDeedNfcReadSuccess);
    
    // Initialize widget if available
    if(app->widget) {
        widget_reset(app->widget);
        widget_add_text_box_element(
            app->widget,
            0,
            0,
            128,
            64,
            AlignCenter,
            AlignCenter,
            "\e#RENFE Suma 10\n\n"
            "Place card near\n"
            "Flipper Zero to read",
            false);
        view_dispatcher_switch_to_view(app->view_dispatcher, MetroflipViewWidget);
    }
}

static bool renfe_sum10_on_event(Metroflip* app, SceneManagerEvent event) {
    bool consumed = false;
    
    if(!app) {
        FURI_LOG_E(TAG, "renfe_sum10_on_event: app is NULL");
        return false;
    }
    
    if(event.type == SceneManagerEventTypeCustom) {
        if(event.event == MetroflipEventWidget) {
            // Handle NFC reading events
            if(app->nfc_device && nfc_device_get_data(app->nfc_device, NfcProtocolMfClassic)) {
                const MfClassicData* mfc_data = nfc_device_get_data(app->nfc_device, NfcProtocolMfClassic);
                
                if(mfc_data) {
                    FuriString* parsed_data = furi_string_alloc();
                    Widget* widget = app->widget;

                    if(widget) {
                        widget_reset(widget);
                        if(!renfe_sum10_parse(parsed_data, mfc_data)) {
                            furi_string_printf(parsed_data, "\e#Unknown card\n");
                        }
                        widget_add_text_scroll_element(widget, 0, 0, 128, 52, furi_string_get_cstr(parsed_data));

                        widget_add_button_element(widget, GuiButtonTypeCenter, "Delete", metroflip_delete_widget_callback, app);
                        widget_add_button_element(widget, GuiButtonTypeRight, "Exit", metroflip_exit_widget_callback, app);
                        
                        view_dispatcher_switch_to_view(app->view_dispatcher, MetroflipViewWidget);
                    }
                    
                    furi_string_free(parsed_data);
                }
            }
            consumed = true;
        }
    } else if(event.type == SceneManagerEventTypeBack) {
        scene_manager_search_and_switch_to_previous_scene(app->scene_manager, MetroflipSceneStart);
        consumed = true;
    }

    return consumed;
}

static void renfe_sum10_on_exit(Metroflip* app) {
    if(!app) {
        FURI_LOG_E(TAG, "renfe_sum10_on_exit: app is NULL");
        return;
    }

    if(app->widget) {
        widget_reset(app->widget);
    }
}

// Plugin descriptor
static const MetroflipPlugin renfe_sum10_plugin = {
    .card_name = "RENFE Suma 10",
    .plugin_on_enter = renfe_sum10_on_enter,
    .plugin_on_event = renfe_sum10_on_event,
    .plugin_on_exit = renfe_sum10_on_exit,
};

static const FlipperAppPluginDescriptor renfe_sum10_plugin_descriptor = {
    .appid = METROFLIP_SUPPORTED_CARD_PLUGIN_APP_ID,
    .ep_api_version = METROFLIP_SUPPORTED_CARD_PLUGIN_API_VERSION,
    .entry_point = &renfe_sum10_plugin,
};

const FlipperAppPluginDescriptor* renfe_sum10_plugin_ep(void) {
    return &renfe_sum10_plugin_descriptor;
}