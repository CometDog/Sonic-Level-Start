#define ANIM_DURATION 200
#define ANIM_BLUE_DELAY 600
#define ANIM_YELLOW_DELAY ANIM_BLUE_DELAY + ANIM_DURATION
#define ANIM_RED_DELAY ANIM_YELLOW_DELAY + ANIM_DURATION
#define ANIM_TIME_DELAY ANIM_RED_DELAY + ANIM_DURATION

static Window *s_main_window;
static Layer *s_red_parent, *s_blue_layer, *s_yellow_layer, *s_time_layer, *s_date_layer;
static BitmapLayer *s_red_layer;
static GBitmap *s_red_bitmap;
static TextLayer *s_time_label, *s_month_label, *s_day_label, *s_brand_label;
static EffectLayer *s_effect_layer;
static EffectOffset s_white_effect_offset, s_yellow_effect_offset;

static PropertyAnimation *s_red_anim, *s_blue_anim, *s_yellow_anim, *s_time_anim, *s_date_anim;

static GFont s_time_font, s_large_font, s_small_font;

static char s_time_buffer[] = "XX:XX";
static char s_month_buffer[] = "XXXX";
static char s_day_buffer[] = "XX";