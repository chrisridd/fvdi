#ifndef FVDI_H
#define FVDI_H
/*
 * fVDI structure declarations, by Johan Klockars.
 *
 * Most C files in the fVDI engine, as well as in its
 * device drivers, need to include this file.
 *
 * Since it would be difficult to do without this file when
 * writing new device drivers, and to make it possible for
 * some such drivers to be commercial, this file is put in
 * the public domain. It's not copyrighted or under any sort
 * of license.
 */

#ifdef __PUREC__
#define CDECL cdecl
#else
#define CDECL
#endif

#define FVDI_MAGIC 0x66564449L /* "fVDI" */

#define CHUNKY		1
#define TRUE_COLOUR	2
#define CHECK_PREVIOUS	4

#define Q_NEXT_DRIVER	1
#define Q_FILE		2
#define S_DEBUG		3
#define S_OPTION	4
#define S_DRVOPTION	5
#define Q_NAME		100
#define S_SCREEN	101
#define S_AESBUF	102
#define S_CACHEIMG	103
#define S_DOBLIT	104

#define MODULE_IF_VER   0x0020

#define HANDLES         32   /* Handles in default table (see also fvdi.s) */

/* vst_charmap/vst_map_mode modes */
#define MAP_BITSTREAM	0
#define MAP_ATARI		1
#define MAP_UNICODE		2

struct fVDI_log {
    short active;
    long *start;
    long *current;
    long *end;
};

struct Super_data {
    struct fVDI_log fvdi_log;
};

/* VDI structures */
/* -------------- */

#define WHITE 0
#define BLACK 1

typedef struct Control_ {
    short function;
    short l_ptsin;
    short l_ptsout;
    short l_intin;
    short l_intout;
    short subfunction;
    short handle;
    void *addr1;
    void *addr2;
} Control;

typedef struct VDIpars_ {
    Control *control;
    short *intin;
    short *ptsin;
    short *intout;
    short *ptsout;
} VDIpars;

typedef struct List_ {
    struct List_ *next;
    short type;
    void *value;
} List;


typedef struct {
    short x1;
    short y1;
    short x2;
    short y2;
} RECT16;


typedef struct {
    unsigned short reserved;
    unsigned short red;
    unsigned short green;
    unsigned short blue;
} COLOR_RGB;


typedef struct {
    unsigned short cyan;
    unsigned short magenta;
    unsigned short yellow;
    unsigned short black;
} COLOR_CMYK;


typedef union {
    COLOR_RGB  rgb;
    COLOR_CMYK cmyk;
} COLOR_ENTRY;


typedef struct {
    long magic;
    long length;
    long format;
    long reserved;
    long map_id;
    long color_space;
    long flags;
    long no_colors;
    long reserved1;
    long reserved2;
    long reserved3;
    long reserved4;
    COLOR_ENTRY colors[256];
} COLOR_TAB;


typedef struct {
    long magic;		/* Structure ID 'cbtm' */
    long length;		/* Structure length */
    long format;		/* Structure format (0) */
    long reserved;	/* Reserved (0) */
    unsigned char *addr;	/* Adress of bitmap */
    long width;		/* Width of a line in bytes */
    long bits;		/* Bit depth */
    unsigned long px_format;	/* Pixel format */
    long xmin;		/* Minimum discrete x-coordinate of bitmap */
    long ymin;		/* Minimum discrete y-coordinate of bitmap */
    long xmax;		/* Maximum discrete x-coordinate of bitmap + 1 */
    long ymax;		/* Maximum discrete y-coordinate if bitmap + 1 */
    COLOR_TAB  *ctab;	/* Pointer to the palette or 0 */
    void *itab;		/* Pointer to the inverse palette or 0 */
    long reserved0;	/* Reserved (must be 0) */
    long reserved1;	/* Reserved (must be 0) */
} GCBITMAP;


typedef struct RGB_ {
    short red;
    short green;
    short blue;
} RGB;

typedef struct Colour_ {
    RGB vdi;
    RGB hw;
    long real;
} Colour;

typedef struct Fgbg_ {
    short background;
    short foreground;
} Fgbg;


typedef struct DrvPalette_ {
    long first_pen;
    long count;
    short *requested;
    Colour *palette;
} DrvPalette;

typedef struct DrvLine_ {
    long x1;
    long y1;
    long x2;
    long y2;
    long pattern;
    long colour;
    long mode;
    long draw_last;
} DrvLine;

typedef long fix31;
#define SHORT_TO_FIX31(X)    ((long)(X) << 16)
#define FIX31_TO_SHORT(X)    (short)((((long)(X) + 0x8000L) >> 16))

typedef struct Fontcharmap_ {
    short id;			/*  charmap header ('00',01,02,10,11,12,TT,T1) */
    short map[224];		/*  mapping information d = map[s-32] like */
} Fontcharmap;

typedef struct Fontspdcharmap_ {
    Fontcharmap bs_int;
    Fontcharmap bs_symbol;
    Fontcharmap bs_dingbats;
    Fontcharmap ps_int;
    Fontcharmap ps_symbol;
    Fontcharmap ps_dingbats;
    Fontcharmap true_type;
    Fontcharmap type_1;
} Fontspdcharmap;

typedef struct Fontextra_ {
    struct distance1_ {		/* Calculated from the */
        short base;		/*  values given with */
        short half;		/*  the font for easier */
        short ascent;		/*  text positioning */
        short bottom;
        short descent;
        short top;
    } distance;
    struct unpacked_ {
        char *data;		/* 'Unpacked' font data */
        short format;		/* Byte/word per char */
    } unpacked;
    short *width_table;		/* Precalculated widths */
    struct Fontheader_ *next_size;
    struct Fontheader_ *first_size;

    short ref_count;		/* Reference count (dispose if 0) */

    fix31 size;         /* Font size */
    short format;       /* Returned by vqt_xfntinfo() and vqt_ext_name() */

    /* Vector fonts */
    char *filename;		/* Font filename for on-demand backends */
    short index;			/* FreeType2 font index */
    void *current;		/* Font current glyph */
    void *cache;			/* Glyph cache */
    void *scratch;		/* Glyph scratch .. */
    short effects;		/* Effect combination the font was rendered to */
    short underline_offset;	/* Offset or the underline stroke */
} Fontextra;


typedef struct Fontheader_ {
    short id;			/* Face identifier */
    short dummy;        /* See extra.size for the font size */
    char  name[32];		/* Face name */
    struct code_ {
        short low;		/* Lowest ASCII value in face */
        short high;		/* Highest ASCII value in face */
    } code;
    struct distance_ {
        short top;		/* Top line distance */
        short ascent;		/* Ascent line distance */
        short half;		/* Half line distance */
        short descent;		/* Descent line distance */
        short bottom;		/* Bottom line distance */
    } distance;
    struct widest_ {
        short character;	/* Width of widest char in font */
        short cell;		/* Width of widest char cell in font */
    } widest;
    struct offset_ {
        short left;		/* Left offset */
        short right;		/* Right offset */
    } offset;
    short thickening;	 	/* No. of pixels to widen chars */
    short underline;		/* Width in pixels of underline */
    short lightening; 		/* Mask used to drop pixels out */
    short skewing;		/* Mask used to determine skewing */
    short flags;			/* Font flags */
    struct table_ {
        short *horizontal; 	/* Pointer to horizontal offset table */
        short *character;	/* Pointer to char offset table */
    } table;
    char  *data;			/* Pointer to font data */
    short width;			/* Font width */
    short height;		/* Font height */
    struct Fontheader_ *next;	/* Pointer to next font */
    Fontextra extra;
} Fontheader;
/*
 * Fontheader flags
 */
#define FONTF_SYSTEM     0x0001            /* Default system font */
#define FONTF_HORTABLE   0x0002            /* Use horizontal offsets table */
#define FONTF_BIGENDIAN  0x0004            /* Font image is in byteswapped format */
#define FONTF_MONOSPACED 0x0008            /* Font is monospaced */
#define FONTF_EXTENDED   0x0020            /* Extended font header */
#define FONTF_COMPRESSED FONTF_EXTENDED
#define FONTF_FULLID     0x2000            /* Use 'full font ID' */
#define FONTF_SCALABLE   0x4000            /* fVDI internal: scalable font */
#define FONTF_EXTERNAL   0x8000            /* fVDI internal: external module being used (FT2) */

typedef struct vwk_ {
    struct wk_ *real_address;
    short standard_handle;
    struct text_ {
        short font;
        Fgbg colour;
        short rotation;
        struct alignment_ {
            short horizontal;
            short vertical;
        } alignment;
        struct character_ {
            short width;
            short height;
        } character;
        struct cell_ {
            short width;
            short height;
        } cell;
        short effects;
        short charmap;
        Fontheader *current_font;	/* Not in standard VDI */
    } text;
    struct line_ {
        short type;
        Fgbg colour;
        struct ends_ {
            short beginning;
            short end;
        } ends;
        short width;
        short user_mask;
    } line;
    struct bezier_ {
        short on;		/* Should these really be per vwk? */
        short depth_scale;
    } bezier;
    struct marker_ {
        short type;
        Fgbg colour;
        struct size_ {
            short width;
            short height;
        } size;
    } marker;
    struct fill_ {
        short interior;
        Fgbg colour;
        short style;
        short perimeter;
        struct user_ {
            struct pattern_ {
                short *in_use;
                short *extra;
            } pattern;
            short multiplane;
        } user;
    } fill;
    struct console_ {
        short cursor;
        short reversed;
        struct pos_ {
            short x;
            short y;
        } pos;
    } console;
    struct clip_ {
        short on;
        RECT16 rectangle;
    } clip;
    short mode;
    Colour *palette;		/* Odd when only negative (fg/bg) */
} Virtual;

/*
 * note: this represent the return values of vq_scrninfo()
 */
typedef struct dev_ {
    short format;
    short clut;
    short bit_depth;
    short dummy1;
    short colours;
    short byte_width;		/* Taken from wk instead? */
    void *address;		/*       -    "    -      */
    struct bits_ {
        short red;
        short green;
        short blue;
        short alpha;
        short genlock;
        short unused;
        short organization;
    } bits;
    short dummy2;
    union scrmap_ {
        short vdi2pix[256];
        struct bitnumber_ {
            short red[16];
            short green[16];
            short blue[16];
            short alpha[16];
            short genlock[16];
            short unused[32];
            short reserved[144];
        } bitnumber;
    } scrmap;			/* Should be anonymous, but that is not ANSI C */
} Device;

#define LAST_SCREEN_DEVICE 10

typedef struct Module_ {
    short id;				/* From FVDI.SYS (or elsewhere) */
    short flags;				/* From FVDI.SYS (and elsewhere) 1 - resident */
#define MOD_RESIDENT 0x0001
    char *file_name;			/* From FVDI.SYS */
    char *name;				/* Identification string */
    long CDECL (*initialize)(Virtual *vwk);		/* Called after fVDI is initialized */
    long CDECL (*setup)(long type, long value);	/* Called to modify settings */
    void CDECL (*shutdown)(Virtual *vwk);		/* Called when shutting down */
    void *priv;			/* Info the module wants to keep around */
} Module;

typedef struct Driver_ {
    Module module;
    Virtual *CDECL (*opnwk)(Virtual *vwk);	/* Called on v_opnwk() */
    void    CDECL (*clswk)(Virtual *vwk);	/* Called on v_clswk() */
    Virtual *default_vwk;		/* Used directly by fVDI v_opnvwk */
    Device *device;
} Driver;

typedef struct function_ {
    short retvals[2];
    void *code;
} Function;


typedef struct XFNT_INFO_ {
    long  size;
    short format;
    short id;
    short index;
    char  font_name[50];
    char  family_name[50];
    char  style_name[50];
    char  file_name1[200];
    char  file_name2[200];
    char  file_name3[200];
    short pt_cnt;
    short pt_sizes[64];
} XFNT_INFO;
/*
 * flags for vqt_xfntinfo()
 */
#define XFNT_INFO_FONT_NAME   0x0001
#define XFNT_INFO_FAMILY_NAME 0x0002
#define XFNT_INFO_STYLE_NAME  0x0004
#define XFNT_INFO_FILE_NAME1  0x0008
#define XFNT_INFO_FILE_NAME2  0x0010
#define XFNT_INFO_FILE_NAME3  0x0020
#define XFNT_INFO_SIZES       0x0100
#define XFNT_INFO_SIZES2      0x0200


typedef struct VQT_FHDR_ {
    char           fh_fmver[8];
    unsigned long  fh_fntsz;
    unsigned long  fh_fbfsz;
    unsigned short fh_cbfsz;
    unsigned short fh_hedsz;
    unsigned short fh_fntid;
    unsigned short fh_sfvnr;
    char           fh_fntnm[70];
    char           fh_mdate[10];
    char           fh_laynm[70];
    char           fh_cpyrt[78];
    unsigned short fh_nchrl;
    unsigned short fh_nchrf;
    unsigned short fh_fchrf;
    unsigned short fh_nktks;
    unsigned short fh_nkprs;
    char           fh_flags;
    char           fh_cflgs;
    char           fh_famcl;
    char           fh_frmcl;
    char           fh_sfntn[32];
    char           fh_sfacn[16];
    char           fh_fntfm[14];
    unsigned short fh_itang;
    unsigned short fh_orupm;
    /* There should really be a bunch of more things here! */
} VQT_FHDR;


typedef struct MFDB_ {
    short *address;
    short width;
    short height;
    short wdwidth;
    short standard;
    short bitplanes;
    short reserved[3];
} MFDB;

typedef struct Mouse_ {
    short type;
    short hide;
    short buttons;
    short forced;
    struct position_ {
        short x;
        short y;
    } position;
    struct hotspot_ {
        short x;
        short y;
    } hotspot;
    Fgbg colour;
    short mask[16];
    short data[16];
    void *extra_info;
} Mouse;

typedef struct wk_ {
    Driver *driver;
    struct screen_ {
        short type;
        short colour;
        short bkg_colours;
        short look_up_table;	/* non-direct colour (should be: _some_ kind of palette sw/hw available) */
        struct palette_ {
            short possibilities;
            short size;
            Colour *colours;
            char *transformation;
        } palette;
        MFDB mfdb;
        short wrap;
        void *linea;
        struct shadow_ {
            void *buffer;
            void *address;
            short wrap;
        } shadow;
        struct pixel_ {
            short width;
            short height;
        } pixel;
        struct coordinates_ {
            short course;	/* What's this? */
            short min_x;
            short min_y;
            short max_x;
            short max_y;
        } coordinates;
    } screen;
    struct writing_ {
        short fonts;
        Fontheader *first_font;	/* Font chain */
        short effects;
        struct rotation_ {
            short possible;
            short type;
        } rotation;
        short justification;
        struct size1_ {			/* From current font? */
            short possibilities;
            struct height_ {
                short min;
                short max;
            } height;
            struct width_ {
                short min;
                short max;
            } width;
        } size;
    } writing;
    struct drawing_ {
        struct primitives_ {
            short supported;
            long attributes;
        } primitives;
        short rubber_banding;
        short flood_fill;
        short writing_modes;
        struct fill1_ {
            short possible;
            short patterns;
            short hatches;
        } fill;
        struct marker1_ {
            short types;
            struct size2_ {
                short possibilities;
                struct height1_ {
                    short min;
                    short max;
                } height;
                struct width1_ {
                    short min;
                    short max;
                } width;
            } size;
        } marker;
        struct line1_ {
            short types;
            struct wide_ {
                struct width2_ {
                    short possibilities;
                    short max;
                    short min;
                } width;
                short types_possible;
                short writing_modes;
            } wide;
        } line;
        struct bezier1_ {
            short available;
            struct depth_scale_ {
                short min;
                short max;
            } depth_scale;
            struct depth_ {
                short min;
                short max;
            } depth;
        } bezier;
        struct cellarray_ {
            short available;
        } cellarray;
    } drawing;
    struct raster_ {
        short scaling;
        short performance;	/* 16x16ops-per-second; */
    } raster;
    struct various_ {
        short input_type;
        short inking;		/* 'nedsvartning' */
        short max_ptsin;
        short max_intin;
        short buttons;
        short cursor_movement;
        short number_entry;
        short selection;
        short typing;
        short workstation_type;
    } various;
    Mouse mouse;
    struct vector_ {
        void *motion;
        void *draw;
        void *button;
        void *wheel;
        void *vblank;
    } vector;
    struct vblank_ {
        short real;
        short frequency;
    } vblank;
    struct r_ {
        void CDECL (*set_palette)(Virtual *vwk, DrvPalette *palette_pars);
        long CDECL (*get_colour)(Virtual *vwk, long colours);
        void CDECL (*set_pixel)(Virtual *vwk, MFDB *mfdb, long x, long y, long colour);
        long CDECL (*get_pixel)(Virtual *vwk, MFDB *mfdb, long x, long y);
        long CDECL (*line)(Virtual *vwk, DrvLine *pars);
        void CDECL (*expand)(Virtual *vwk, MFDB *src, long src_x, long src_y, MFDB *dst, long dst_x, long dst_y, long w, long h, long operation, long colour);
        void CDECL (*fill)(Virtual *vwk, long x, long y, long w, long h, short *pattern, long colour, long mode, long interior_style);
        void CDECL (*fillpoly)(Virtual *vwk, short points[], long n, short index[], long moves, short *pattern, long colour, long mode, long interior_style);
        long CDECL (*blit)(Virtual *vwk, MFDB *src, long src_x, long src_y, MFDB *dst, long dst_x, long dst_y, long w, long h, long operation);
        void CDECL (*text)(Virtual *vwk, short *text, long length, long dst_x, long dst_y, short *offsets);
        void CDECL (*mouse)(struct wk_ *wk, long x, long y, Mouse *mouse);
    } r;
    Function dummy;		/* Table really extends to -1 */
    Function function[256];
    short opcode5_count;
    void *opcode5[24];
    short opcode11_count;
    void *opcode11[14];
} Workstation;


struct v_bez_pars {
    short num_pts;
    short *points;
    char  *bezarr;
    short *extent;
    short *totpoints;
    short *totmoves;
};

typedef struct {
    const char *name;
    union {
        void *v;
        short *s;
        long (*func)(const char **);
        long (*vfunc)(Virtual *, const char **);
    } var;
    short type;
} Option;

#endif
