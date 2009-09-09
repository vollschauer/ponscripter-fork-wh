/* -*- C++ -*-
 *
 *  PonscripterLabel.h - Execution block parser of Ponscripter
 *
 *  Copyright (c) 2001-2008 Ogapee (original ONScripter, of which this
 *  is a fork).
 *
 *  ogapee@aqua.dti2.ne.jp
 *
 *  This program is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License as
 *  published by the Free Software Foundation; either version 2 of the
 *  License, or (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
 *  02111-1307 USA
 */

#ifndef __PONSCRIPTER_LABEL_H__
#define __PONSCRIPTER_LABEL_H__

#include "ScriptParser.h"
#include "DirtyRect.h"
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>

#ifdef MP3_MAD
#include "MadWrapper.h"
#else
#include <smpeg.h>
#endif

#ifdef USE_OGG_VORBIS
#ifdef INTEGER_OGG_VORBIS
#include <tremor/ivorbisfile.h>
#else
#include <vorbis/vorbisfile.h>
#endif
#endif

#define DEFAULT_VIDEO_SURFACE_FLAG (SDL_SWSURFACE)

#define DEFAULT_BLIT_FLAG (0)

#define MAX_SPRITE_NUM 1000
#define MAX_SPRITE2_NUM 256
#define MAX_PARAM_NUM 100
#define CUSTOM_EFFECT_NO 100

#define ONS_MIX_CHANNELS 50
#define ONS_MIX_EXTRA_CHANNELS 4
#define MIX_WAVE_CHANNEL (ONS_MIX_CHANNELS + 0)
#define MIX_BGM_CHANNEL (ONS_MIX_CHANNELS + 1)
#define MIX_LOOPBGM_CHANNEL0 (ONS_MIX_CHANNELS + 2)
#define MIX_LOOPBGM_CHANNEL1 (ONS_MIX_CHANNELS + 3)

#ifndef DEFAULT_WM_TITLE
#define DEFAULT_WM_TITLE "Ponscripter"
#endif
#define DEFAULT_WM_ICON "Ponscripter"

#define NUM_GLYPH_CACHE 30

struct OVInfo {
    SDL_AudioCVT cvt;
    int cvt_len;
    int mult1;
    int mult2;
    unsigned char* buf;
    long decoded_length;
#ifdef USE_OGG_VORBIS
    ogg_int64_t length;
    ogg_int64_t pos;
    OggVorbis_File ovf;
#endif
};

struct Subtitle {
    int number;
    float time;
    pstring text;
};

class SubtitleDefs {
    struct Def {
	rgb_t colour;
	int pos;
	int alpha;
    };
    std::vector<Def> subs;
    std::deque<Subtitle> text;
    bool sorted;
    void sort();
public:
    operator bool() const { return !text.empty(); }
    int numdefs() const { return subs.size(); }
    void define(int n, rgb_t colour, int pos, int alpha);
    void add(int n, float t, pstring x);
    float next();
    Subtitle pop();
    rgb_t colour(int no) { return subs[no].colour; }
    int pos(int no) { return subs[no].pos; }
    int alpha(int no) { return subs[no].alpha; }
};

class PonscripterLabel : public ScriptParser {
public:
    typedef AnimationInfo::ONSBuf ONSBuf;

    PonscripterLabel();
    ~PonscripterLabel();
   
    // ----------------------------------------
    // start-up options
    void enableCDAudio();
    void setDebugMode();
    void setCDNumber(int cdrom_drive_number);
    void setRegistryFile(const char* filename);
    void setDLLFile(const char* filename);
    void setSavePath(const pstring& path);
    void setArchivePath(const pstring& path);

    bool hasArchivePath() const { return archive_path; }
    void setFullscreenMode();
    void setWindowMode();
    void enableButtonShortCut();
    void enableWheelDownAdvance();
    void disableRescale();
    void enableEdit();
    void setKeyEXE(const char* path);
    void setMaskType(int mask_type) { png_mask_type = mask_type; }
    
    int  init(const char* preferred_script);
    int  eventLoop();

    void reset(); // used if definereset
    void resetSub(); // used if reset

    bool skip_flag;
    void setSkipMode(bool mode);
    bool draw_one_page_flag;
    bool key_pressed_flag;
    int  shift_pressed_status;
    int  ctrl_pressed_status;

    /* ---------------------------------------- */
    /* Commands */
    // my extensions
    int haeleth_speedpercentCommand(const pstring& cmd);
    int haeleth_defwindowCommand(const pstring& cmd);
    int haeleth_usewindowCommand(const pstring& cmd);
    int haeleth_text_extentCommand(const pstring& cmd);
    int haeleth_text_heightCommand(const pstring& cmd);
    int haeleth_centre_lineCommand(const pstring& cmd);
    int haeleth_char_setCommand(const pstring& cmd);
    int haeleth_font_styleCommand(const pstring& cmd);
    int haeleth_map_fontCommand(const pstring& cmd);
    int haeleth_hinting_modeCommand(const pstring& cmd);
    int haeleth_ligate_controlCommand(const pstring& cmd);
    int haeleth_sayCommand(const pstring& cmd);
    int gettextspeedCommand(const pstring& cmd);
    int vsp_whenCommand(const pstring& cmd);
    
    // regular NScripter stuff
    int wavestopCommand(const pstring& cmd);
    int waveCommand(const pstring& cmd);
    int waittimerCommand(const pstring& cmd);
    int waitCommand(const pstring& cmd);
    int vspCommand(const pstring& cmd);
    int voicevolCommand(const pstring& cmd);
    int vCommand(const pstring& cmd);
    int trapCommand(const pstring& cmd);
    int textspeedCommand(const pstring& cmd);
    int textshowCommand(const pstring& cmd);
    int textonCommand(const pstring& cmd);
    int textoffCommand(const pstring& cmd);
    int texthideCommand(const pstring& cmd);
    int textclearCommand(const pstring& cmd);
    int texecCommand(const pstring& cmd);
    int tateyokoCommand(const pstring& cmd);
    int talCommand(const pstring& cmd);
    int tablegotoCommand(const pstring& cmd);
    int systemcallCommand(const pstring& cmd);
    int strspCommand(const pstring& cmd);
    int stopCommand(const pstring& cmd);
    int sp_rgb_gradationCommand(const pstring& cmd);
    int spstrCommand(const pstring& cmd);
    int spreloadCommand(const pstring& cmd);
    int splitCommand(const pstring& cmd);
    int spclclkCommand(const pstring& cmd);
    int spbtnCommand(const pstring& cmd);
    int skipoffCommand(const pstring& cmd);
    int sevolCommand(const pstring& cmd);
    int setwindow3Command(const pstring& cmd);
    int setwindow2Command(const pstring& cmd);
    int setwindowCommand(const pstring& cmd);
    int setcursorCommand(const pstring& cmd);
    int selectCommand(const pstring& cmd);
    int savetimeCommand(const pstring& cmd);
    int saveonCommand(const pstring& cmd);
    int saveoffCommand(const pstring& cmd);
    int savegameCommand(const pstring& cmd);
    int savefileexistCommand(const pstring& cmd);
    int savescreenshotCommand(const pstring& cmd);
    int resettimerCommand(const pstring& cmd);
    int resetCommand(const pstring& cmd);
    int repaintCommand(const pstring& cmd);
    int rndCommand(const pstring& cmd);
    int rmodeCommand(const pstring& cmd);
    int quakeCommand(const pstring& cmd);
    int puttextCommand(const pstring& cmd);
    int prnumclearCommand(const pstring& cmd);
    int prnumCommand(const pstring& cmd);
    int printCommand(const pstring& cmd);
    int playstopCommand(const pstring& cmd);
    int playonceCommand(const pstring& cmd);
    int playCommand(const pstring& cmd);
    int ofscopyCommand(const pstring& cmd);
    int negaCommand(const pstring& cmd);
    int mspCommand(const pstring& cmd);
    int mpegplayCommand(const pstring& cmd);
    int mp3volCommand(const pstring& cmd);
    int mp3fadeoutCommand(const pstring& cmd);
    int mp3Command(const pstring& cmd);
    int movemousecursorCommand(const pstring& cmd);
    int monocroCommand(const pstring& cmd);
    int menu_windowCommand(const pstring& cmd);
    int menu_fullCommand(const pstring& cmd);
    int menu_automodeCommand(const pstring& cmd);
    int lspCommand(const pstring& cmd);
    int loopbgmstopCommand(const pstring& cmd);
    int loopbgmCommand(const pstring& cmd);
    int lookbackflushCommand(const pstring& cmd);
    int lookbackbuttonCommand(const pstring& cmd);
    int logspCommand(const pstring& cmd);
    int locateCommand(const pstring& cmd);
    int loadgameCommand(const pstring& cmd);
    int ldCommand(const pstring& cmd);
    int jumpfCommand(const pstring& cmd);
    int jumpbCommand(const pstring& cmd);
    int ispageCommand(const pstring& cmd);
    int isfullCommand(const pstring& cmd);
    int isskipCommand(const pstring& cmd);
    int isdownCommand(const pstring& cmd);
    int inputCommand(const pstring& cmd);
    int indentCommand(const pstring& cmd);
    int humanorderCommand(const pstring& cmd);
    int getzxcCommand(const pstring& cmd);
    int getvoicevolCommand(const pstring& cmd);
    int getversionCommand(const pstring& cmd);
    int gettimerCommand(const pstring& cmd);
    int gettextCommand(const pstring& cmd);
    int gettagCommand(const pstring& cmd);
    int gettabCommand(const pstring& cmd);
    int getspsizeCommand(const pstring& cmd);
    int getspmodeCommand(const pstring& cmd);
    int getsevolCommand(const pstring& cmd);
    int getscreenshotCommand(const pstring& cmd);
    int getretCommand(const pstring& cmd);
    int getregCommand(const pstring& cmd);
    int getpageupCommand(const pstring& cmd);
    int getpageCommand(const pstring& cmd);
    int getmp3volCommand(const pstring& cmd);
    int getmouseposCommand(const pstring& cmd);
    int getlogCommand(const pstring& cmd);
    int getinsertCommand(const pstring& cmd);
    int getfunctionCommand(const pstring& cmd);
    int getenterCommand(const pstring& cmd);
    int getcursorposCommand(const pstring& cmd);
    int getcursorCommand(const pstring& cmd);
    int getcselstrCommand(const pstring& cmd);
    int getcselnumCommand(const pstring& cmd);
    int gameCommand(const pstring& cmd);
    int fileexistCommand(const pstring& cmd);
    int exec_dllCommand(const pstring& cmd);
    int exbtnCommand(const pstring& cmd);
    int erasetextwindowCommand(const pstring& cmd);
    int endCommand(const pstring& cmd);
    int dwavestopCommand(const pstring& cmd);
    int dwaveCommand(const pstring& cmd);
    int dvCommand(const pstring& cmd);
    int drawtextCommand(const pstring& cmd);
    int drawsp3Command(const pstring& cmd);
    int drawsp2Command(const pstring& cmd);
    int drawspCommand(const pstring& cmd);
    int drawfillCommand(const pstring& cmd);
    int drawclearCommand(const pstring& cmd);
    int drawbg2Command(const pstring& cmd);
    int drawbgCommand(const pstring& cmd);
    int drawCommand(const pstring& cmd);
    int delayCommand(const pstring& cmd);
    int deletescreenshotCommand(const pstring& cmd);    
    int defineresetCommand(const pstring& cmd);
    int cspCommand(const pstring& cmd);
    int cselgotoCommand(const pstring& cmd);
    int cselbtnCommand(const pstring& cmd);
    int clickCommand(const pstring& cmd);
    int clCommand(const pstring& cmd);
    int chvolCommand(const pstring& cmd);
    int checkpageCommand(const pstring& cmd);
    int cellCommand(const pstring& cmd);
    int captionCommand(const pstring& cmd);
    int btnwait2Command(const pstring& cmd);
    int btnwaitCommand(const pstring& cmd);
    int btntime2Command(const pstring& cmd);
    int btntimeCommand(const pstring& cmd);
    int btndownCommand(const pstring& cmd);
    int btndefCommand(const pstring& cmd);
    int btnCommand(const pstring& cmd);
    int brCommand(const pstring& cmd);
    int bltCommand(const pstring& cmd);
    int bgcopyCommand(const pstring& cmd);
    int bgCommand(const pstring& cmd);
    int barclearCommand(const pstring& cmd);
    int barCommand(const pstring& cmd);
    int aviCommand(const pstring& cmd);
    int automode_timeCommand(const pstring& cmd);
    int autoclickCommand(const pstring& cmd);
    int allspresumeCommand(const pstring& cmd);
    int allsphideCommand(const pstring& cmd);
    int allsp2resumeCommand(const pstring& cmd);
    int allsp2hideCommand(const pstring& cmd);
    int amspCommand(const pstring& cmd);

protected:
    /* ---------------------------------------- */
    /* Event related variables */
    enum { NOT_EDIT_MODE            = 0,
           EDIT_SELECT_MODE         = 1,
           EDIT_VARIABLE_INDEX_MODE = 2,
           EDIT_VARIABLE_NUM_MODE   = 3,
           EDIT_MP3_VOLUME_MODE     = 4,
           EDIT_VOICE_VOLUME_MODE   = 5,
           EDIT_SE_VOLUME_MODE      = 6 };

    int variable_edit_mode;
    int variable_edit_index;
    int variable_edit_num;
    int variable_edit_sign;

    int skip_to_wait;

    void variableEditMode(SDL_KeyboardEvent* event);
    void keyDownEvent(SDL_KeyboardEvent* event);
    void keyUpEvent(SDL_KeyboardEvent* event);
    void keyPressEvent(SDL_KeyboardEvent* event);
    void mousePressEvent(SDL_MouseButtonEvent* event);
    void mouseMoveEvent(SDL_MouseMotionEvent* event);
    void timerEvent();
    void flushEventSub(SDL_Event &event);
    void flushEvent();
    void startTimer(int count);
    void advancePhase(int count = 0);
    void trapHandler();
    void initSDL();
#if defined(PDA) && !defined(PSP)
    void openAudio(int freq=22050, Uint16 format=MIX_DEFAULT_FORMAT, int channels=MIX_DEFAULT_CHANNELS);
#else
    void openAudio(int freq=44100, Uint16 format=MIX_DEFAULT_FORMAT, int channels=MIX_DEFAULT_CHANNELS);
#endif

private:
    enum { NORMAL_DISPLAY_MODE = 0, TEXT_DISPLAY_MODE = 1 };
    enum { IDLE_EVENT_MODE   = 0,
           EFFECT_EVENT_MODE = 1,
           WAIT_BUTTON_MODE  = 2, // For select, btnwait and rmenu.
           WAIT_INPUT_MODE   = (4 | 8), // Can be skipped by a click.
           WAIT_SLEEP_MODE   = 16,      // Cannot be skipped by a click.
           WAIT_TIMER_MODE   = 32,
           WAIT_TEXTBTN_MODE = 64,
           WAIT_VOICE_MODE   = 128,
           WAIT_TEXT_MODE    = 256 // clickwait, newpage, select
    };
    typedef enum { COLOR_EFFECT_IMAGE  = 0,
                   DIRECT_EFFECT_IMAGE = 1,
                   BG_EFFECT_IMAGE     = 2,
                   TACHI_EFFECT_IMAGE  = 3 } EFFECT_IMAGE;
    enum { ALPHA_BLEND_CONST          = 1,
           ALPHA_BLEND_MULTIPLE       = 2,
           ALPHA_BLEND_FADE_MASK      = 3,
           ALPHA_BLEND_CROSSFADE_MASK = 4 };

    // ----------------------------------------
    // start-up options
    bool   cdaudio_flag;
    pstring registry_file;
    pstring dll_file;
    pstring getret_str;
    int    getret_int;
    bool   enable_wheeldown_advance_flag;
    bool   disable_rescale_flag;
    bool   edit_flag;
    pstring key_exe_file;

    // ----------------------------------------
    // Global definitions
    long internal_timer;
    bool automode_flag;
    void setAutoMode(bool mode);
    long automode_time;
    long autoclick_time;
    long remaining_time;

    bool saveon_flag;
    bool internal_saveon_flag; // to saveoff at the head of text
    int  yesno_caller;
    int  yesno_selected_file_no;

    bool  monocro_flag;
    rgb_t monocro_color;
    rgb_t monocro_color_lut[256];
    int nega_mode;

    enum { TRAP_NONE = 0,
           TRAP_LEFT_CLICK  = 1,
           TRAP_RIGHT_CLICK = 2,
           TRAP_NEXT_SELECT = 4,
           TRAP_STOP = 8 };
    int    trap_mode;
    pstring trap_dist;
    pstring wm_title_string;
    pstring wm_icon_string;
    pstring wm_edit_string;
    bool   fullscreen_mode;
    Uint32 fullscreen_flags;
    bool   window_mode;

    bool btntime2_flag;
    long btntime_value;
    long internal_button_timer;
    long btnwait_time;
    bool btndown_flag;

    void quit();

    /* ---------------------------------------- */
    /* Script related variables */
    enum { REFRESH_NONE_MODE   = 0,
           REFRESH_NORMAL_MODE = 1,
           REFRESH_SAYA_MODE   = 2,
           REFRESH_SHADOW_MODE = 4,
           REFRESH_TEXT_MODE   = 8,
           REFRESH_CURSOR_MODE = 16,
           REFRESH_COMP_MODE   = 32 };

    int refresh_shadow_text_mode;
    int current_refresh_mode;
    int display_mode;
    int event_mode;
    // Final image, i.e. picture_surface (+ shadow + text_surface):
    SDL_Surface* accumulation_surface;
    // Complementary final image, i.e. final image xor (shadow + text_surface):
    SDL_Surface* accumulation_comp_surface;
    // Text + Select_image + Tachi image + background:
    SDL_Surface* screen_surface;
    SDL_Surface* effect_dst_surface; // Intermediate source buffer for effect
    SDL_Surface* effect_src_surface; // Intermediate dest buffer for effect
    SDL_Surface* screenshot_surface; // Screenshot
    SDL_Surface* image_surface; // Reference for loadImage()

    /* ---------------------------------------- */
    /* Button related variables */
    AnimationInfo btndef_info;

    struct ButtonState {
        int x, y, button;
        bool down_flag;
	ButtonState() { button = 0; down_flag = false; }
    } current_button_state, volatile_button_state,
      last_mouse_state, shelter_mouse_state;

    struct ButtonElt {
        typedef std::map<int, ButtonElt> collection;
        typedef collection::iterator iterator;
	typedef collection::reverse_iterator reverse_iterator;
        
        enum BUTTON_TYPE {
            NORMAL_BUTTON     = 0,
            SPRITE_BUTTON     = 1,
            EX_SPRITE_BUTTON  = 2,
            LOOKBACK_BUTTON   = 3,
            TMP_SPRITE_BUTTON = 4
        };
        BUTTON_TYPE button_type;
        int sprite_no;
        pstring exbtn_ctl;
        SDL_Rect select_rect;
        SDL_Rect image_rect;
        AnimationInfo* anim[2];
        int show_flag; // 0: show nothing, 1: show anim[0], 2: show anim[1]

        bool isSprite() { return button_type == SPRITE_BUTTON
                              || button_type == EX_SPRITE_BUTTON; }
        bool isTmpSprite() { return button_type == TMP_SPRITE_BUTTON; }
        
        ButtonElt() {
            button_type = NORMAL_BUTTON;
            anim[0] = anim[1] = 0;
            show_flag = 0;
        }
        void destroy() {
            if ((button_type == NORMAL_BUTTON || isTmpSprite()) && anim[0]) {
                delete anim[0];
                anim[0] = 0;
            }
        }
    };
    ButtonElt::collection buttons, shelter_buttons;
    ButtonElt exbtn_d_button;

    void buttonsRemoveSprite(int no) {
        ButtonElt::iterator it = buttons.begin();
        while (it != buttons.end())
            if (it->second.sprite_no == no && it->second.isSprite()) {
                it->second.destroy();
                buttons.erase(it++);
            }
            else
                ++it;
    }
    
    void deleteButtons() {
        for (ButtonElt::iterator it = buttons.begin(); it != buttons.end();
             ++it)
            it->second.destroy();
        buttons.clear();
        exbtn_d_button.exbtn_ctl.trunc(0);
    }
    
    int current_over_button;

    bool getzxc_flag;
    bool gettab_flag;
    bool getpageup_flag;
    bool getpagedown_flag;
    bool getinsert_flag;
    bool getfunction_flag;
    bool getenter_flag;
    bool getcursor_flag;
    bool spclclk_flag;

    void resetSentenceFont();
    void refreshMouseOverButton();
    void refreshSprite(int sprite_no, bool active_flag, int cell_no,
                       SDL_Rect* check_src_rect, SDL_Rect* check_dst_rect);

    void decodeExbtnControl(const pstring& ctl_string,
                            SDL_Rect* check_src_rect = 0,
                            SDL_Rect* check_dst_rect = 0);

    void disableGetButtonFlag();
    int getNumberFromBuffer(const char** buf);

    /* ---------------------------------------- */
    /* General image-related variables */
    enum { PNG_MASK_AUTODETECT    = 0,
	   PNG_MASK_USE_ALPHA     = 1,
	   PNG_MASK_USE_NSCRIPTER = 2
    };
    int png_mask_type;

    /* ---------------------------------------- */
    /* Background related variables */
    AnimationInfo bg_info;
    EFFECT_IMAGE  bg_effect_image; // This is no longer used. Remove it later.

    /* ---------------------------------------- */
    /* Tachi-e related variables */
    /* 0 ... left, 1 ... center, 2 ... right */
    AnimationInfo tachi_info[3];
    int human_order[3];

    /* ---------------------------------------- */
    /* Sprite related variables */
    AnimationInfo* sprite_info;
    AnimationInfo* sprite2_info;
    bool all_sprite_hide_flag;
    bool all_sprite2_hide_flag;
    
    /* ---------------------------------------- */
    /* Parameter related variables */
    AnimationInfo* bar_info[MAX_PARAM_NUM];
    AnimationInfo* prnum_info[MAX_PARAM_NUM];

    /* ---------------------------------------- */
    /* Cursor related variables */
    enum { CURSOR_WAIT_NO    = 0,
           CURSOR_NEWPAGE_NO = 1 };
    AnimationInfo cursor_info[2];

    void loadCursor(int no, const char* str, int x, int y, bool abs_flag = 0);
    void saveAll();
    void loadEnvData();
    void saveEnvData();

    /* ---------------------------------------- */
    /* Lookback related variables */
    AnimationInfo lookback_info[4];

    /* ---------------------------------------- */
    /* Stored window related variables */
    struct WindowDef {
	typedef dictionary<pstring, WindowDef>::t dic;
	int left, top, width, height,
	    font_size, pitch_x, pitch_y,
	    speed, bold, shadow;
	pstring backdrop;
	int w_left, w_top, w_width, w_height;
    };
    WindowDef::dic stored_windows;
    
    /* ---------------------------------------- */
    /* Text related variables */
    AnimationInfo text_info;
    AnimationInfo sentence_font_info;
    int  erase_text_window_mode;
    bool text_on_flag; // suppress the effect of erase_text_window_mode
    bool draw_cursor_flag;
    int  textgosub_clickstr_state;
    int  indent_offset;
    int  line_enter_status; // 0 ... no enter, 1 ... pretext, 2 ... body
    int  global_speed_modifier;
    Glyph current_glyph;

    int  refreshMode();
    void DoSetwindow(WindowDef& def);
    void setwindowCore();

    Glyph renderGlyph(Font* font, Uint16 text, int size,
                             float x_fractional_part);
    void drawGlyph(SDL_Surface* dst_surface, Fontinfo* info, SDL_Color &color,
                   wchar unicode, float x, int y, bool shadow_flag,
                   AnimationInfo* cache_info, SDL_Rect* clip,
                   SDL_Rect &dst_rect);
    int  drawChar(const char* text, Fontinfo* info, bool flush_flag,
                  bool lookback_flag, SDL_Surface* surface,
                  AnimationInfo* cache_info, SDL_Rect* clip = 0);
    void drawString(const char* str, rgb_t color, Fontinfo* info,
                    bool flush_flag, SDL_Surface* surface, SDL_Rect* rect = 0,
                    AnimationInfo* cache_info = 0);

/*  void drawString(const pstring& str, rgb_t color, Fontinfo* info,
                    bool flush_flag, SDL_Surface* surface, SDL_Rect* rect = 0,
                    AnimationInfo* cache_info = 0)
        { /- for now -/ drawString(str, color, info, flush_flag,
				   surface, rect, cache_info); }*/
    
    void restoreTextBuffer();
    int  enterTextDisplayMode(bool text_flag = true);
    int  leaveTextDisplayMode();
    void doClickEnd();
    int  clickWait(bool display_char);
    int  clickNewPage(bool display_char);
    int  textCommand();
    int  processText();

    std::set<wchar> indent_chars;
    std::set<wchar> break_chars;
    bool is_indent_char(const wchar c) const
        { return indent_chars.find(c) != indent_chars.end(); }
    bool is_break_char(const wchar c) const
        { return break_chars.find(c) != break_chars.end(); }
    bool check_orphan_control();

    /* ---------------------------------------- */
    /* Effect related variables */
    DirtyRect dirty_rect, dirty_rect_tmp; // only this region is updated
    int effect_counter; // counter in each effect
    int effect_timer_resolution;
    int effect_start_time;
    int effect_start_time_old;

    int setEffect(const Effect& effect);
    int doEffect(Effect& effect, AnimationInfo* anim, int effect_image,
                  bool clear_dirty_region = true);
    void drawEffect(SDL_Rect* dst_rect, SDL_Rect* src_rect,
                    SDL_Surface* surface);
    void generateMosaic(SDL_Surface* src_surface, int level);

    /* ---------------------------------------- */
    /* Select related variables */
    enum {
        SELECT_GOTO_MODE  = 0,
        SELECT_GOSUB_MODE = 1,
        SELECT_NUM_MODE   = 2,
        SELECT_CSEL_MODE  = 3
    };
    struct SelectElt {
        typedef std::vector<SelectElt> vector;
        typedef vector::iterator iterator;
        pstring text, label;
        SelectElt(const pstring& t, const pstring& l) : text(t), label(l) {}
    };
    SelectElt::vector select_links, shelter_select_links;
    const char* select_label_next_script;
    ButtonElt::iterator shortcut_mouse_line;

    ButtonElt getSelectableSentence(const pstring& buffer, Fontinfo* info,
                                    bool flush_flag = true,
                                    bool nofile_flag = false);

    /* ---------------------------------------- */
    /* Sound related variables */
    enum {
        SOUND_NONE    = 0,
        SOUND_PRELOAD = 1,
        SOUND_WAVE    = 2,
        SOUND_OGG     = 4,
        SOUND_OGG_STREAMING = 8,
        SOUND_MP3     = 16,
        SOUND_MIDI    = 32,
        SOUND_OTHER   = 64
    };
    int cdrom_drive_number;
    pstring default_cdrom_drive;
    bool cdaudio_on_flag; // false if mute
    bool volume_on_flag; // false if mute
    SDL_AudioSpec audio_format;
    bool audio_open_flag;

    bool   wave_play_loop_flag;
    pstring wave_file_name;

    bool   midi_play_loop_flag;
    pstring midi_file_name;
    Mix_Music* midi_info;

    SDL_CD* cdrom_info;
    int    current_cd_track;
    bool   cd_play_loop_flag;
    bool   music_play_loop_flag;
    bool   mp3save_flag;
    pstring music_file_name;
    unsigned char* mp3_buffer;
    SMPEG*  mp3_sample;
    Uint32  mp3fadeout_start;
    Uint32  mp3fadeout_duration;
    OVInfo* music_ovi;
    Mix_Music* music_info;
    pstring loop_bgm_name[2];

    Mix_Chunk* wave_sample[ONS_MIX_CHANNELS + ONS_MIX_EXTRA_CHANNELS];

    pstring music_cmd;
    pstring midi_cmd;

    int playSound(const pstring& filename, int format, bool loop_flag,
                  int channel = 0);
    
    void playCDAudio();
    int playWave(Mix_Chunk* chunk, int format, bool loop_flag, int channel);
    int playMP3();
    int playOGG(int format, unsigned char* buffer, long length, bool loop_flag,
                int channel);
    int playExternalMusic(bool loop_flag);
    int playMIDI(bool loop_flag);

    SubtitleDefs parseSubtitles(pstring file);
    int playMPEG(const pstring& filename, bool click_flag,
		 SubtitleDefs& subtitles);
    void playAVI(const pstring& filename, bool click_flag);

    enum { WAVE_PLAY        = 0,
           WAVE_PRELOAD     = 1,
           WAVE_PLAY_LOADED = 2 };
    void stopBGM(bool continue_flag);
    void stopAllDWAVE();
    void playClickVoice();
    void setupWaveHeader(unsigned char* buffer, int channels, int rate,
                         unsigned long data_length);
    OVInfo* openOggVorbis(unsigned char* buf, long len, int &channels,
                          int &rate);
    int  closeOggVorbis(OVInfo* ovi);

    /* ---------------------------------------- */
    /* Text event related variables */
    bool new_line_skip_flag;
    int  text_speed_no;

    void shadowTextDisplay(SDL_Surface* surface, SDL_Rect &clip);
    void clearCurrentTextBuffer();
    void newPage(bool next_flag);

    void flush(int refresh_mode, SDL_Rect* rect = 0,
               bool clear_dirty_flag = true, bool direct_flag = false);
    void flushDirect(SDL_Rect &rect, int refresh_mode, bool updaterect = true);
    void executeLabel();
    SDL_Surface* loadImage(const pstring& file_name, bool* has_alpha = NULL);
    int parseLine();

    void mouseOverCheck(int x, int y);

    /* ---------------------------------------- */
    /* Animation */
    int  proceedAnimation();
    int  estimateNextDuration(AnimationInfo* anim, SDL_Rect &rect, int minimum);
    void resetRemainingTime(int t);
    void setupAnimationInfo(AnimationInfo* anim, Fontinfo* info = NULL);
    void parseTaggedString(AnimationInfo* anim);
    void drawTaggedSurface(SDL_Surface* dst_surface, AnimationInfo* anim,
                           SDL_Rect &clip);
    void stopAnimation(int click);

    /* ---------------------------------------- */
    /* File I/O */
    enum SaveFileType { NScripter, ONScripter, Ponscripter };
    
    void searchSaveFile(SaveFileInfo &info, int no);
    int  loadSaveFile(int no);
    void saveMagicNumber(bool output_flag);
    int  saveSaveFile(int no);

    int  loadSaveFile2(SaveFileType file_type, int file_version);
    void saveSaveFile2(bool output_flag);

    /* ---------------------------------------- */
    /* Image processing */
    unsigned char* resize_buffer;
    size_t resize_buffer_size;

    int  resizeSurface(SDL_Surface* src, SDL_Surface* dst);
    void shiftCursorOnButton(int diff);
    void alphaBlend(SDL_Surface* mask_surface, int trans_mode,
             Uint32 mask_value = 255, SDL_Rect* clip = 0);
    void alphaBlend32(SDL_Surface* dst_surface, SDL_Rect dst_rect,
             SDL_Surface* src_surface, SDL_Color &color, SDL_Rect* clip,
             bool rotate_flag);
    void makeNegaSurface(SDL_Surface* surface, SDL_Rect &clip);
    void makeMonochromeSurface(SDL_Surface* surface, SDL_Rect &clip);
    void refreshSurface(SDL_Surface* surface, SDL_Rect* clip_src,
             int refresh_mode = REFRESH_NORMAL_MODE);
    void createBackground();

    /* ---------------------------------------- */
    /* rmenu and system call */
    bool system_menu_enter_flag;
    int  system_menu_mode;

    int  shelter_event_mode;
    int  shelter_display_mode;
    bool shelter_draw_cursor_flag;
    TextBuffer* cached_text_buffer;

    void enterSystemCall();
    void leaveSystemCall(bool restore_flag = true);
    void executeSystemCall();

    void executeSystemMenu();
    void executeSystemSkip();
    void executeSystemAutomode();
    void executeSystemReset();
    void executeSystemEnd();
    void executeWindowErase();
    void createSaveLoadMenu(bool is_save);
    void executeSystemLoad();
    void executeSystemSave();
    void executeSystemYesNo();
    void setupLookbackButton();
    void executeSystemLookback();
};

#endif // __PONSCRIPTER_LABEL_H__