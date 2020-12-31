/*********************
*      INCLUDES
*********************/
#include "CalculatorPage.h"
#include "EvaluatorLib/EE.h"

#ifdef __cplusplus
extern "C" {
#endif
    LV_FONT_DECLARE(Font_Bahnschrift_60);
    LV_FONT_DECLARE(Font_Bahnschrift_45);
    LV_IMG_DECLARE(image_TextAreaBG);
#ifdef __cplusplus
}
#endif

#define btn1_ColorDe 0x999999
#define btn2_ColorDe 0x333333
#define btn3_ColorDe 0x4d4d4d
#define btn1_ColorPr 0xb3b3b3
#define btn2_ColorPr 0x4d4d4d
#define btn3_ColorPr 0x666666


#define bgColor 0xe6e6e6

#define textAreaWidth 424
#define textAreaHeight 250
#define btn_Hight 70
#define btn_Widgh 100
static lv_obj_t* textArea;
static lv_obj_t* btGrp;

typedef struct
{
    const char* text;
    const uint32_t color;
    const uint32_t color_pr;
} BtnGrp_TypeDef;

static const BtnGrp_TypeDef BtnGrp[] =
{
    {"ANS", btn1_ColorDe,btn1_ColorPr},{"CE",  btn1_ColorDe,btn1_ColorPr},{"C",   btn1_ColorDe,btn1_ColorPr},{"<",   btn1_ColorDe,btn1_ColorPr},
    {"%",   btn1_ColorDe,btn1_ColorPr},{"(",   btn1_ColorDe,btn1_ColorPr},{")",   btn1_ColorDe,btn1_ColorPr},{"/",   btn1_ColorDe,btn1_ColorPr},
    {"7",   btn2_ColorDe,btn2_ColorPr},{"8",   btn2_ColorDe,btn2_ColorPr},{"9",   btn2_ColorDe,btn2_ColorPr},{"*",   btn1_ColorDe,btn1_ColorPr},
    {"4",   btn2_ColorDe,btn2_ColorPr},{"5",   btn2_ColorDe,btn2_ColorPr},{"6",   btn2_ColorDe,btn2_ColorPr},{"-",   btn1_ColorDe,btn1_ColorPr},
    {"1",   btn2_ColorDe,btn2_ColorPr},{"2",   btn2_ColorDe,btn2_ColorPr},{"3",   btn2_ColorDe,btn2_ColorPr},{"+",   btn1_ColorDe,btn1_ColorPr},
    {"-",   btn2_ColorDe,btn2_ColorPr},{"0",   btn2_ColorDe,btn2_ColorPr},{".",   btn2_ColorDe,btn2_ColorPr},{"=",   btn3_ColorDe,btn3_ColorPr},
};

static void BtnGrp_EventHandler(lv_obj_t* obj, lv_event_t event)
{
    static char last_ans[128] = "\0";
#define IS_STR(txt, str) (strcmp((txt), (str)) == 0)
    if (event == LV_EVENT_CLICKED) {
        const char* btn_text = lv_obj_get_style_value_str(obj, LV_BTN_PART_MAIN);
        const char* ta_text = lv_textarea_get_text(textArea);
        if (IS_STR(ta_text, "Error Format"))
        {
            lv_textarea_set_text(textArea, "");
        }
        if (IS_STR(btn_text, "C") || IS_STR(btn_text, "CE"))
        {
            lv_textarea_set_text(textArea, "");
        }
        else if (IS_STR(btn_text, "<"))
        {
            lv_textarea_del_char(textArea);
        }
        else if (IS_STR(btn_text, "ANS"))
        {
            lv_textarea_add_text(textArea, last_ans);
        }
        else if (IS_STR(btn_text, "=") || IS_STR(btn_text, "%"))
        {
            if (IS_STR(ta_text, ""))
            {
                return;
            }

            bool is_persent = IS_STR(btn_text, "%");

            //evaluate_run_expression(ta_text, is_persent);
            char result[128] = {0};
            //evaluate_get_result(result, sizeof(result));
            EE_CalculatorToStr((const char*)ta_text, result, sizeof(result));

            if (result[0] >= '0' && result[0] <= '9')
            {
                strncpy(last_ans, result, sizeof(last_ans));
            }

            lv_textarea_set_text(textArea, result);
        }
        else
        {
            size_t len = strlen(ta_text);
            if (len >= 1)
            {
                if (ta_text[len - 1] == '/' && IS_STR(btn_text, "0"))
                {
                    return;
                }
            }
            lv_textarea_add_text(textArea, btn_text);
        }
    }
}

static void BtnGrp_Create(lv_obj_t* par)
{
    lv_obj_t* cont = lv_cont_create(par,NULL);
    lv_obj_set_size(cont, 450, 480);
    lv_obj_align(cont,par,LV_ALIGN_IN_TOP_MID, 0, textAreaHeight+27+25);
    lv_obj_set_style_local_border_width(cont, LV_CONT_PART_MAIN, LV_STATE_DEFAULT, 0);
    lv_obj_set_style_local_bg_opa(cont, LV_CONT_PART_MAIN, LV_STATE_DEFAULT, LV_OPA_TRANSP);
    lv_cont_set_layout(cont, LV_LAYOUT_PRETTY_TOP);
    
    lv_obj_set_style_local_pad_all(cont, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, 10);
    lv_obj_set_style_local_pad_inner(cont, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, 8);

    static lv_style_t style;
    lv_style_init(&style);
    lv_style_set_radius(&style, LV_STATE_DEFAULT, 12);
    lv_style_set_border_width(&style, LV_STATE_DEFAULT, 0);
    lv_style_set_outline_width(&style, LV_STATE_FOCUSED,0);
    lv_style_set_value_align(&style, LV_STATE_DEFAULT, LV_ALIGN_CENTER);
    lv_style_set_value_ofs_y(&style,LV_STATE_DEFAULT, 6);
    lv_style_set_value_color(&style, LV_STATE_DEFAULT, lv_color_hex(0xe6e6e6));
    lv_style_set_value_color(&style, LV_STATE_PRESSED, lv_color_hex(0xe6e6e6));
    lv_style_set_value_font(&style, LV_STATE_DEFAULT, &Font_Bahnschrift_45);
    lv_style_set_shadow_width(&style, LV_STATE_DEFAULT, 20);
    lv_style_set_shadow_width(&style, LV_STATE_PRESSED, 0);
    lv_style_set_shadow_color(&style, LV_STATE_DEFAULT, LV_COLOR_BLACK);
    lv_style_set_shadow_opa(&style, LV_STATE_DEFAULT, LV_OPA_COVER);
    
    static lv_anim_path_t path;
    lv_anim_path_init(&path);
    lv_anim_path_set_cb(&path, lv_anim_path_ease_in_out);
    lv_style_set_transition_path(&style, LV_STATE_PRESSED, &path);
    lv_style_set_transition_path(&style, LV_STATE_DEFAULT, &path);
    lv_style_set_transition_time(&style, LV_STATE_DEFAULT, 50);
    lv_style_set_transition_prop_1(&style, LV_STATE_DEFAULT, LV_STYLE_BG_COLOR);
    lv_style_set_transition_prop_1(&style, LV_STATE_DEFAULT, LV_STYLE_SHADOW_WIDTH);
    for (int i = 0; i < 24; i++)
    {
        uint32_t color = BtnGrp[i].color;
        uint32_t color_pr = BtnGrp[i].color_pr;
        const char* text = BtnGrp[i].text;

        lv_obj_t* btn = lv_btn_create(cont, NULL);
        lv_obj_set_size(btn, btn_Widgh, btn_Hight);
        lv_obj_set_event_cb(btn, BtnGrp_EventHandler);

        lv_obj_add_style(btn, LV_BTN_PART_MAIN, &style);
        lv_obj_set_style_local_value_str(btn, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, text);

        lv_obj_set_style_local_bg_color(btn, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, lv_color_hex(color));
        lv_obj_set_style_local_bg_color(btn, LV_BTN_PART_MAIN, LV_STATE_PRESSED, lv_color_hex(color_pr));
    }
}

static void TextArea_Create(lv_obj_t* par)
{
    lv_obj_t* imgBG = lv_img_create(par, NULL);
    lv_img_set_src(imgBG, &image_TextAreaBG);
    lv_obj_align(imgBG, NULL, LV_ALIGN_IN_TOP_MID, 0, 27);

    lv_obj_t* textarea = lv_textarea_create(imgBG, NULL);
    lv_obj_set_width(textarea, textAreaWidth);
    lv_obj_set_height(textarea, textAreaHeight);
    lv_obj_align(textarea, NULL, LV_ALIGN_CENTER, 0,0);

    lv_obj_set_style_local_bg_color(textarea, LV_TEXTAREA_PART_BG, LV_STATE_DEFAULT, lv_color_hex(0x333333));
    lv_obj_set_style_local_bg_opa(textarea, LV_TEXTAREA_PART_BG, LV_STATE_DEFAULT, 0);
    lv_obj_set_style_local_text_font(textarea, LV_TEXTAREA_PART_BG, LV_STATE_DEFAULT, &Font_Bahnschrift_60);
    lv_obj_set_style_local_text_color(textarea, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, lv_color_hex(0xe6e6e6));
    lv_obj_set_style_local_border_width(textarea, LV_TEXTAREA_PART_BG, LV_STATE_DEFAULT, 0);
    lv_textarea_set_text_align(textarea, LV_LABEL_ALIGN_RIGHT);
    lv_textarea_set_text(textarea, "");

    textArea = textarea;
}

void CalculatorPage_Create()
{
    lv_obj_set_style_local_bg_color(lv_scr_act(), LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_BLACK);
    lv_obj_t* base = lv_obj_create(lv_scr_act(),NULL);
    lv_obj_set_size(base,LV_HOR_RES,LV_VER_RES);
	lv_obj_set_style_local_bg_color(base, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, lv_color_hex(bgColor));
    lv_obj_set_style_local_border_width(base, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, 10);
    lv_obj_set_style_local_border_color(base, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, lv_color_hex(0x999999));
    lv_obj_set_style_local_radius(base, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, 12);
    TextArea_Create(base);
    BtnGrp_Create(base);
}