#include "pebble.h"
#include "gcolor_definitions.h"
#include "libs/pebble-assist.h"
#include "libs/EffectLayer/effect_layer.h"
#include "elements.h"

char *upcase(char *str)
{
    for (int i = 0; str[i] != 0; i++) {
        if (str[i] >= 'a' && str[i] <= 'z') {
            str[i] -= 0x20;
        }
    }

    return str;
}

static void do_animation() {
	GRect red_start, red_finish;
	GRect blue_start, blue_finish;
	GRect yellow_start, yellow_finish;
	GRect time_start, time_finish;
	GRect date_start, date_finish;
	
	red_start = GRect(-42,0,42,168);
	red_finish = GRect(0,0,42,168);
	
	blue_start = GRect(0,-168,144,168);
	blue_finish = GRect(0,0,144,168);
	
	yellow_start = GRect(144,120,144,48);
	yellow_finish = GRect(0,120,144,48);
	
	time_start = GRect(144,0,144,168);
	time_finish = GRect(0,0,144,168);
	
	date_start = GRect(-144,0,144,168);
	date_finish = GRect(0,0,144,168);
	
    s_red_anim = property_animation_create_layer_frame(s_red_parent, &red_start, &red_finish);
    animation_set_duration((Animation*)s_red_anim, ANIM_DURATION);
    animation_set_delay((Animation*)s_red_anim, ANIM_RED_DELAY);
    animation_set_curve((Animation*)s_red_anim, AnimationCurveLinear);
    animation_schedule((Animation*)s_red_anim);
	
	s_blue_anim = property_animation_create_layer_frame(s_blue_layer, &blue_start, &blue_finish);
    animation_set_duration((Animation*)s_blue_anim, ANIM_DURATION);
    animation_set_delay((Animation*)s_blue_anim, ANIM_BLUE_DELAY);
    animation_set_curve((Animation*)s_blue_anim, AnimationCurveLinear);
    animation_schedule((Animation*)s_blue_anim);
	
	s_yellow_anim = property_animation_create_layer_frame(s_yellow_layer, &yellow_start, &yellow_finish);
    animation_set_duration((Animation*)s_yellow_anim, ANIM_DURATION);
    animation_set_delay((Animation*)s_yellow_anim, ANIM_YELLOW_DELAY);
    animation_set_curve((Animation*)s_yellow_anim, AnimationCurveLinear);
    animation_schedule((Animation*)s_yellow_anim);
	
	s_time_anim = property_animation_create_layer_frame(s_time_layer, &time_start, &time_finish);
    animation_set_duration((Animation*)s_time_anim, ANIM_DURATION);
    animation_set_delay((Animation*)s_time_anim, ANIM_TIME_DELAY);
    animation_set_curve((Animation*)s_time_anim, AnimationCurveLinear);
    animation_schedule((Animation*)s_time_anim);
	
	s_date_anim = property_animation_create_layer_frame(s_date_layer, &date_start, &date_finish);
    animation_set_duration((Animation*)s_date_anim, ANIM_DURATION);
    animation_set_delay((Animation*)s_date_anim, ANIM_TIME_DELAY);
    animation_set_curve((Animation*)s_date_anim, AnimationCurveLinear);
    animation_schedule((Animation*)s_date_anim);
}

static void update_blue(Layer *layer, GContext *ctx) {
	graphics_context_set_fill_color(ctx, GColorBlueMoon);
	graphics_fill_rect(ctx, GRect(0,0,144,168), 0, GCornerNone);
}

static void update_yellow(Layer *layer, GContext *ctx) {
	graphics_context_set_fill_color(ctx, GColorYellow);
	graphics_fill_rect(ctx, GRect(0,0,144,48), 0, GCornerNone);
}

static void update_time() {
	time_t epoch = time(NULL);
	struct tm *t = localtime(&epoch);
	
	if (clock_is_24h_style() == true) {
		print_time(s_time_buffer, "%H:%M", t);
	}
	else {
		print_time(s_time_buffer, "%I:%M", t);
	}
	
	if (t->tm_mon == 5 || t->tm_mon == 6) {
		print_time(s_month_buffer, "%B", t);
	}
	else {
		print_time(s_month_buffer, "%b", t);	
	}
	print_time(s_day_buffer, "%d", t);
	
	text_layer_set_text(s_time_label, s_time_buffer);
	text_layer_set_text(s_month_label, s_month_buffer);
	text_layer_set_text(s_day_label, s_day_buffer);
}

static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
	layer_mark_dirty(window_get_root_layer(s_main_window));
}

static void bt_handler(bool connected) {
	if (connected) {
		vibes_short_pulse();
	}
	else {
		vibes_double_pulse();
	}
}

static void main_window_load(Window *window) {
	
	s_white_effect_offset.offset_x = 2;
	s_white_effect_offset.offset_y = 2;
	s_white_effect_offset.orig_color = GColorWhite;
	s_white_effect_offset.offset_color = GColorBlack;
	
	s_yellow_effect_offset.offset_x = 2;
	s_yellow_effect_offset.offset_y = 2;
	s_yellow_effect_offset.orig_color = GColorYellow;
	s_yellow_effect_offset.offset_color = GColorBlack;
	
	GRect bounds = window_get_bounds(window);
	
	window_set_background_color(window, GColorBlack);
	
	s_time_font = fonts_load_resource_font(RESOURCE_ID_GASLIGHT_30);
	s_large_font = fonts_load_resource_font(RESOURCE_ID_GASLIGHT_40);
	s_small_font = fonts_load_resource_font(RESOURCE_ID_VIPNAGOR_12);
	
	s_effect_layer = effect_layer_create(bounds);
	
	s_blue_layer = layer_create(GRect(0,-168,144,168));
	s_yellow_layer = layer_create(GRect(144,120,144,48));
	s_time_layer = layer_create(GRect(144,0,144,168));
	s_date_layer = layer_create(GRect(-144,0,144,168));
	
	s_red_parent = layer_create(GRect(-42,0,42,168));
	
	s_red_layer = bitmap_layer_create(GRect(0,0,42,168));
	
	s_time_label = text_layer_create(GRect(0,30,129,40));
	s_month_label = text_layer_create(GRect(0,60,64,40));
	s_day_label = text_layer_create(GRect(0,60,129,40));
	s_brand_label = text_layer_create(GRect(0,5,139,40));
	
	layer_set_update_proc(s_blue_layer, update_blue);
	layer_set_update_proc(s_yellow_layer, update_yellow);
	
	s_red_bitmap = gbitmap_create_with_resource(RESOURCE_ID_SONIC);
	
	bitmap_layer_set_bitmap(s_red_layer, s_red_bitmap);
	
	bitmap_layer_set_compositing_mode(s_red_layer, GCompOpSet);
	
	text_layer_set_colors(s_time_label, GColorWhite, GColorClear);
	text_layer_set_colors(s_month_label, GColorWhite, GColorClear);
	text_layer_set_colors(s_day_label, GColorYellow, GColorClear);
	text_layer_set_colors(s_brand_label, GColorBlack, GColorClear);
	
	text_layer_set_text_alignment(s_time_label, GTextAlignmentRight);
	text_layer_set_text_alignment(s_month_label, GTextAlignmentRight);
	text_layer_set_text_alignment(s_day_label, GTextAlignmentRight);
	text_layer_set_text_alignment(s_brand_label, GTextAlignmentRight);
	
	text_layer_set_font(s_time_label, s_time_font);
	text_layer_set_font(s_month_label, s_time_font);
	text_layer_set_font(s_day_label, s_large_font);
	text_layer_set_font(s_brand_label, s_small_font);
	
	text_layer_set_text(s_brand_label, "PEBBLE TIME");
	
	layer_add_to_window(s_blue_layer, window);
	layer_add_to_window(s_yellow_layer, window);
	
	layer_add_to_window(s_red_parent, window);
	
	bitmap_layer_add_to_layer(s_red_layer, s_red_parent);
	
	layer_add_to_window(s_time_layer, window);
	layer_add_to_window(s_date_layer, window);
	
	text_layer_add_to_layer(s_time_label, s_time_layer);
	text_layer_add_to_layer(s_month_label, s_date_layer);
	text_layer_add_to_layer(s_day_label, s_date_layer);
	text_layer_add_to_layer(s_brand_label, s_yellow_layer);
	
	effect_layer_add_effect(s_effect_layer, effect_shadow, &s_white_effect_offset);
	effect_layer_add_effect(s_effect_layer, effect_shadow, &s_yellow_effect_offset);
	
	layer_add_child(s_time_layer, effect_layer_get_layer(s_effect_layer));
	layer_add_child(s_date_layer, effect_layer_get_layer(s_effect_layer));
	
	do_animation();
	update_time();
}

static void main_window_unload(Window *window) {
	fonts_unload_custom_font(s_time_font);
	fonts_unload_custom_font(s_large_font);
	fonts_unload_custom_font(s_small_font);
	
	effect_layer_destroy(s_effect_layer);
	
	gbitmap_destroy_safe(s_red_bitmap);
	
	layer_destroy_safe(s_blue_layer);
	layer_destroy_safe(s_yellow_layer);
	layer_destroy_safe(s_red_parent);
	layer_destroy_safe(s_time_layer);
	layer_destroy_safe(s_date_layer);
	
	bitmap_layer_destroy_safe(s_red_layer);
	
	text_layer_destroy_safe(s_time_label);
	text_layer_destroy_safe(s_month_label);
	text_layer_destroy_safe(s_day_label);
	text_layer_destroy_safe(s_brand_label);
}

static void init() {
	s_main_window = window_create();
	window_handlers(s_main_window, main_window_load, main_window_unload);
	window_stack_push(s_main_window, true);
	
	tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);
	bluetooth_connection_service_subscribe(bt_handler);
}

static void deinit() {
	animation_unschedule_all();
	accel_tap_service_unsubscribe();
	bluetooth_connection_service_unsubscribe();
	window_destroy_safe(s_main_window);
}

int main(void) {
	init();
	app_event_loop();
	deinit();
}