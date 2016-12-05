/*
 * Copyright (C) 1999-2001 Free Software Foundation, Inc.
 * This file is part of the GNU LIBICONV Library.
 *
 * The GNU LIBICONV Library is free software; you can redistribute it
 * and/or modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * The GNU LIBICONV Library is distributed in the hope that it will be
 * useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with the GNU LIBICONV Library; see the file COPYING.LIB.
 * If not, write to the Free Software Foundation, Inc., 59 Temple Place -
 * Suite 330, Boston, MA 02111-1307, USA.
 */
/* 
 * This file was contributed by Jeroen @ Mobile Tidings (http://mobiletidings.com)
 */
static ucs4_t gsmToUnicode[] =
{
    /* 0x00 */ 0x0040,   /* COMMERCIAL AT */
    /* 0x01 */ 0x00A3,   /* POUND SIGN */
    /* 0x02 */ 0x0024,   /* DOLLAR SIGN */
    /* 0x03 */ 0x00A5,   /* YEN SIGN */
    /* 0x04 */ 0x00E8,   /* LATIN SMALL LETTER E WITH GRAVE */
    /* 0x05 */ 0x00E9,   /* LATIN SMALL LETTER E WITH ACUTE */
    /* 0x06 */ 0x00F9,   /* LATIN SMALL LETTER U WITH GRAVE */
    /* 0x07 */ 0x00EC,   /* LATIN SMALL LETTER I WITH GRAVE */
    /* 0x08 */ 0x00F2,   /* LATIN SMALL LETTER O WITH GRAVE */
    /* 0x09 */ 0x00E7,   /* LATIN SMALL LETTER C WITH CEDILLA */
    /* 0x0A */ 0x000A,   /* LINE FEED */
    /* 0x0B */ 0x00D8,   /* LATIN CAPITAL LETTER O WITH STROKE */
    /* 0x0C */ 0x00F8,   /* LATIN SMALL LETTER O WITH STROKE */
    /* 0x0D */ 0x000D,   /* CARRIAGE RETURN */
    /* 0x0E */ 0x00C5,   /* LATIN CAPITAL LETTER A WITH RING ABOVE */
    /* 0x0F */ 0x00E5,   /* LATIN SMALL LETTER A WITH RING ABOVE */
    /* 0x10 */ 0x0394,   /* GREEK CAPITAL LETTER DELTA */
    /* 0x11 */ 0x005F,   /* LOW LINE */
    /* 0x12 */ 0x03A6,   /* GREEK CAPITAL LETTER PHI */
    /* 0x13 */ 0x0393,   /* GREEK CAPITAL LETTER GAMMA */
    /* 0x14 */ 0x039B,   /* GREEK CAPITAL LETTER LAMDA */
    /* 0x15 */ 0x03A9,   /* GREEK CAPITAL LETTER OMEGA */
    /* 0x16 */ 0x03A0,   /* GREEK CAPITAL LETTER PI */
    /* 0x17 */ 0x03A8,   /* GREEK CAPITAL LETTER PSI */
    /* 0x18 */ 0x03A3,   /* GREEK CAPITAL LETTER SIGMA */
    /* 0x19 */ 0x0398,   /* GREEK CAPITAL LETTER THETA */
    /* 0x1A */ 0x039E,   /* GREEK CAPITAL LETTER XI */
    /* 0x1B */ 0x00A0,   /* ESCAPE TO EXTENSION TABLE */
    /* 0x1C */ 0x00C6,   /* LATIN CAPITAL LETTER AE */
    /* 0x1D */ 0x00E6,   /* LATIN SMALL LETTER AE */
    /* 0x1E */ 0x00DF,   /* LATIN SMALL LETTER SHARP S (German) */
    /* 0x1F */ 0x00C9,   /* LATIN CAPITAL LETTER E WITH ACUTE */
    /* 0x20 */ 0x0020,   /* SPACE */
    /* 0x21 */ 0x0021,   /* EXCLAMATION MARK */
    /* 0x22 */ 0x0022,   /* QUOTATION MARK */
    /* 0x23 */ 0x0023,   /* NUMBER SIGN */
    /* 0x24 */ 0x00A4,   /* CURRENCY SIGN */
    /* 0x25 */ 0x0025,   /* PERCENT SIGN */
    /* 0x26 */ 0x0026,   /* AMPERSAND */
    /* 0x27 */ 0x0027,   /* APOSTROPHE */
    /* 0x28 */ 0x0028,   /* LEFT PARENTHESIS */
    /* 0x29 */ 0x0029,   /* RIGHT PARENTHESIS */
    /* 0x2A */ 0x002A,   /* ASTERISK */
    /* 0x2B */ 0x002B,   /* PLUS SIGN */
    /* 0x2C */ 0x002C,   /* COMMA */
    /* 0x2D */ 0x002D,   /* HYPHEN-MINUS */
    /* 0x2E */ 0x002E,   /* FULL STOP */
    /* 0x2F */ 0x002F,   /* SOLIDUS */
    /* 0x30 */ 0x0030,   /* DIGIT ZERO */
    /* 0x31 */ 0x0031,   /* DIGIT ONE */
    /* 0x32 */ 0x0032,   /* DIGIT TWO */
    /* 0x33 */ 0x0033,   /* DIGIT THREE */
    /* 0x34 */ 0x0034,   /* DIGIT FOUR */
    /* 0x35 */ 0x0035,   /* DIGIT FIVE */
    /* 0x36 */ 0x0036,   /* DIGIT SIX */
    /* 0x37 */ 0x0037,   /* DIGIT SEVEN */
    /* 0x38 */ 0x0038,   /* DIGIT EIGHT */
    /* 0x39 */ 0x0039,   /* DIGIT NINE */
    /* 0x3A */ 0x003A,   /* COLON */
    /* 0x3B */ 0x003B,   /* SEMICOLON */
    /* 0x3C */ 0x003C,   /* LESS-THAN SIGN */
    /* 0x3D */ 0x003D,   /* EQUALS SIGN */
    /* 0x3E */ 0x003E,   /* GREATER-THAN SIGN */
    /* 0x3F */ 0x003F,   /* QUESTION MARK */
    /* 0x40 */ 0x00A1,   /* INVERTED EXCLAMATION MARK */
    /* 0x41 */ 0x0041,   /* LATIN CAPITAL LETTER A */
    /* 0x42 */ 0x0042,   /* LATIN CAPITAL LETTER B */
    /* 0x43 */ 0x0043,   /* LATIN CAPITAL LETTER C */
    /* 0x44 */ 0x0044,   /* LATIN CAPITAL LETTER D */
    /* 0x45 */ 0x0045,   /* LATIN CAPITAL LETTER E */
    /* 0x46 */ 0x0046,   /* LATIN CAPITAL LETTER F */
    /* 0x47 */ 0x0047,   /* LATIN CAPITAL LETTER G */
    /* 0x48 */ 0x0048,   /* LATIN CAPITAL LETTER H */
    /* 0x49 */ 0x0049,   /* LATIN CAPITAL LETTER I */
    /* 0x4A */ 0x004A,   /* LATIN CAPITAL LETTER J */
    /* 0x4B */ 0x004B,   /* LATIN CAPITAL LETTER K */
    /* 0x4C */ 0x004C,   /* LATIN CAPITAL LETTER L */
    /* 0x4D */ 0x004D,   /* LATIN CAPITAL LETTER M */
    /* 0x4E */ 0x004E,   /* LATIN CAPITAL LETTER N */
    /* 0x4F */ 0x004F,   /* LATIN CAPITAL LETTER O */
    /* 0x50 */ 0x0050,   /* LATIN CAPITAL LETTER P */
    /* 0x51 */ 0x0051,   /* LATIN CAPITAL LETTER Q */
    /* 0x52 */ 0x0052,   /* LATIN CAPITAL LETTER R */
    /* 0x53 */ 0x0053,   /* LATIN CAPITAL LETTER S */
    /* 0x54 */ 0x0054,   /* LATIN CAPITAL LETTER T */
    /* 0x55 */ 0x0055,   /* LATIN CAPITAL LETTER U */
    /* 0x56 */ 0x0056,   /* LATIN CAPITAL LETTER V */
    /* 0x57 */ 0x0057,   /* LATIN CAPITAL LETTER W */
    /* 0x58 */ 0x0058,   /* LATIN CAPITAL LETTER X */
    /* 0x59 */ 0x0059,   /* LATIN CAPITAL LETTER Y */
    /* 0x5A */ 0x005A,   /* LATIN CAPITAL LETTER Z */
    /* 0x5B */ 0x00C4,   /* LATIN CAPITAL LETTER A WITH DIAERESIS */
    /* 0x5C */ 0x00D6,   /* LATIN CAPITAL LETTER O WITH DIAERESIS */
    /* 0x5D */ 0x00D1,   /* LATIN CAPITAL LETTER N WITH TILDE */
    /* 0x5E */ 0x00DC,   /* LATIN CAPITAL LETTER U WITH DIAERESIS */
    /* 0x5F */ 0x00A7,   /* SECTION SIGN */
    /* 0x60 */ 0x00BF,   /* INVERTED QUESTION MARK */
    /* 0x61 */ 0x0061,   /* LATIN SMALL LETTER A */
    /* 0x62 */ 0x0062,   /* LATIN SMALL LETTER B */
    /* 0x63 */ 0x0063,   /* LATIN SMALL LETTER C */
    /* 0x64 */ 0x0064,   /* LATIN SMALL LETTER D */
    /* 0x65 */ 0x0065,   /* LATIN SMALL LETTER E */
    /* 0x66 */ 0x0066,   /* LATIN SMALL LETTER F */
    /* 0x67 */ 0x0067,   /* LATIN SMALL LETTER G */
    /* 0x68 */ 0x0068,   /* LATIN SMALL LETTER H */
    /* 0x69 */ 0x0069,   /* LATIN SMALL LETTER I */
    /* 0x6A */ 0x006A,   /* LATIN SMALL LETTER J */
    /* 0x6B */ 0x006B,   /* LATIN SMALL LETTER K */
    /* 0x6C */ 0x006C,   /* LATIN SMALL LETTER L */
    /* 0x6D */ 0x006D,   /* LATIN SMALL LETTER M */
    /* 0x6E */ 0x006E,   /* LATIN SMALL LETTER N */
    /* 0x6F */ 0x006F,   /* LATIN SMALL LETTER O */
    /* 0x70 */ 0x0070,   /* LATIN SMALL LETTER P */
    /* 0x71 */ 0x0071,   /* LATIN SMALL LETTER Q */
    /* 0x72 */ 0x0072,   /* LATIN SMALL LETTER R */
    /* 0x73 */ 0x0073,   /* LATIN SMALL LETTER S */
    /* 0x74 */ 0x0074,   /* LATIN SMALL LETTER T */
    /* 0x75 */ 0x0075,   /* LATIN SMALL LETTER U */
    /* 0x76 */ 0x0076,   /* LATIN SMALL LETTER V */
    /* 0x77 */ 0x0077,   /* LATIN SMALL LETTER W */
    /* 0x78 */ 0x0078,   /* LATIN SMALL LETTER X */
    /* 0x79 */ 0x0079,   /* LATIN SMALL LETTER Y */
    /* 0x7A */ 0x007A,   /* LATIN SMALL LETTER Z */
    /* 0x7B */ 0x00E4,   /* LATIN SMALL LETTER A WITH DIAERESIS */
    /* 0x7C */ 0x00F6,   /* LATIN SMALL LETTER O WITH DIAERESIS */
    /* 0x7D */ 0x00F1,   /* LATIN SMALL LETTER N WITH TILDE */
    /* 0x7E */ 0x00FC,   /* LATIN SMALL LETTER U WITH DIAERESIS */
    /* 0x7F */ 0x00E0    /* LATIN SMALL LETTER A WITH GRAVE */
};
static struct
{
   unsigned char   from;
   ucs4_t          to;
} gsmEscapes[] =
{
    { 0x0A, 0x000C },   /* FORM FEED */
    { 0x14, 0x005E },   /* CIRCUMFLEX ACCENT */
    { 0x28, 0x007B },   /* LEFT CURLY BRACKET */
    { 0x29, 0x007D },   /* RIGHT CURLY BRACKET */
    { 0x2F, 0x005C },   /* REVERSE SOLIDUS */
    { 0x3C, 0x005B },   /* LEFT SQUARE BRACKET */
    { 0x3D, 0x007E },   /* TILDE */
    { 0x3E, 0x005D },   /* RIGHT SQUARE BRACKET */
    { 0x40, 0x007C },   /* VERTICAL LINE */
    { 0x65, 0x20AC },   /* EURO SIGN */
    { 0   , 0      }
};
static int
gsm7_mbtowc (conv_t conv, ucs4_t *pwc, const unsigned char *s, int n)
{
  int i = 0;
  if( *s > 0x7f )
     return RET_ILSEQ;
  else if( *s == 0x1B ) {
     if( n >= 2 ) {
        while( gsmEscapes[ i ].from ) {
           if( gsmEscapes[ i ].from == s[1] ) {
              *pwc = gsmEscapes[ i ].to;
              return 2;
           }
           i++;
        }
        return RET_ILSEQ;
     } else
        return RET_TOOFEW( 1 );
  }
  *pwc = gsmToUnicode[ *s ];
  return 1;
}
static int
gsm7_wctomb (conv_t conv, unsigned char *r, ucs4_t wc, int n)
{
   unsigned char s1, s2;
   s1 = (wc & 0xff00) >> 8;
   s2 = wc & 0x00ff;
   if( s1 == 0x00 ) {
      if( s2 == 0xA || s2 == 0xD ||
         (s2 >= 0x20 && s2 <= 0x23 ) || (s2 >= 0x25 && s2 <= 0x3f)
          || (s2 >= 0x41 && s2 <= 0x5A) || (s2 >= 0x61 && s2 <= 0x7A) ) {
         *r = s2;
         return 1;
      } else {
          switch( s2 ) {
          case 0x24: *r = 0x02; return 1;
          case 0x40: *r = 0x00; return 1;
          case 0x5b: if( n >= 2 ) {r[0] = 0x1b; r[1] = 0x3c; return 2; }
                     else return RET_TOOSMALL;
          case 0x5c: if( n >= 2 ) {r[0] = 0x1b; r[1] = 0x2f; return 2; }
                     else return RET_TOOSMALL;
          case 0x5d: if( n >= 2 ) {r[0] = 0x1b; r[1] = 0x3e; return 2; }
                     else return RET_TOOSMALL;
          case 0x5e: if( n >= 2 ) {r[0] = 0x1b; r[1] = 0x14; return 2; }
                     else return RET_TOOSMALL;
          case 0x5f: *r = 0x11; return 1;
          case 0x7b: if( n >= 2 ) {r[0] = 0x1b; r[1] = 0x28; return 2; }
                     else return RET_TOOSMALL;
          case 0x7c: if( n >= 2 ) {r[0] = 0x1b; r[1] = 0x40; return 2; }
                     else return RET_TOOSMALL;
          case 0x7d: if( n >= 2 ) {r[0] = 0x1b; r[1] = 0x29; return 2; }
                     else return RET_TOOSMALL;
          case 0x7e: if( n >= 2 ) {r[0] = 0x1b; r[1] = 0x3d; return 2; }
                     else return RET_TOOSMALL;
          case 0xa3: *r = 0x01; return 1;
          case 0xa4: *r = 0x24; return 1;
          case 0xa5: *r = 0x03; return 1;
          case 0xa7: *r = 0x5f; return 1;
          case 0xb0: *r = 0x24; return 1;
          case 0xbf: *r = 0x60; return 1;
          case 0xc5: *r = 0x0e; return 1;
          case 0xc6: *r = 0x1c; return 1;
          case 0xc7: *r = 0x09; return 1;
          case 0xc9: *r = 0x1f; return 1;
          case 0xc4: *r = 0x5b; return 1;
          case 0xd1: *r = 0x5d; return 1;
          case 0xd6: *r = 0x5c; return 1;
          case 0xd8: *r = 0x0b; return 1;
          case 0xdc: *r = 0x5e; return 1;
          case 0xdf: *r = 0x1e; return 1;
          case 0xe0: *r = 0x7f; return 1;
          case 0xe4: *r = 0x7b; return 1;
          case 0xe5: *r = 0x0f; return 1;
          case 0xe6: *r = 0x1d; return 1;
          case 0xe7: *r = 0x09; return 1;
          case 0xe8: *r = 0x04; return 1;
          case 0xe9: *r = 0x05; return 1;
          case 0xec: *r = 0x07; return 1;
          case 0xf1: *r = 0x7d; return 1;
          case 0xf2: *r = 0x08; return 1;
          case 0xf6: *r = 0x7c; return 1;
          case 0xf8: *r = 0x0c; return 1;
          case 0xf9: *r = 0x06; return 1;
          case 0xfc: *r = 0x7e; return 1;
          /* one way mappings */
          case 0xc0: *r = 0x41; return 1;
          case 0xc1: *r = 0x41; return 1;
          case 0xc2: *r = 0x41; return 1;
          case 0xc3: *r = 0x41; return 1;
          case 0xc8: *r = 0x45; return 1;
          case 0xca: *r = 0x45; return 1;
          case 0xcb: *r = 0x45; return 1;
          case 0xcc: *r = 0x49; return 1;
          case 0xcd: *r = 0x49; return 1;
          case 0xce: *r = 0x49; return 1;
          case 0xcf: *r = 0x49; return 1;
          case 0xd2: *r = 0x4f; return 1;
          case 0xd3: *r = 0x4f; return 1;
          case 0xd4: *r = 0x4f; return 1;
          case 0xd5: *r = 0x4f; return 1;
          case 0xd9: *r = 0x55; return 1;
          case 0xda: *r = 0x55; return 1;
          case 0xdb: *r = 0x55; return 1;
          case 0xdd: *r = 0x59; return 1;
          case 0xe1: *r = 0x61; return 1;
          case 0xe2: *r = 0x61; return 1;
          case 0xe3: *r = 0x61; return 1;
          case 0xea: *r = 0x65; return 1;
          case 0xeb: *r = 0x65; return 1;
          case 0xed: *r = 0x69; return 1;
          case 0xee: *r = 0x69; return 1;
          case 0xef: *r = 0x69; return 1;
          case 0xf3: *r = 0x6f; return 1;
          case 0xf4: *r = 0x6f; return 1;
          case 0xf5: *r = 0x6f; return 1;
          case 0xfa: *r = 0x75; return 1;
          case 0xfb: *r = 0x75; return 1;
          case 0xfd: *r = 0x79; return 1;
          case 0xff: *r = 0x79; return 1;
          }
       }
       return RET_ILUNI;
    } else if( s1 == 0x20 ) {
         if( s2 == 0xac ) {
            if( n >= 2 ) {
                r[0] = 0x1b; r[1] = 0x65; return 2;
             } else return RET_TOOSMALL;
         }
    } else if( s1 = 0x03 ) {
       switch( s2 ) {
       case 0x94: *r = 0x10; return 1;
       case 0xa6: *r = 0x12; return 1;
       case 0x93: *r = 0x13; return 1;
       case 0x9b: *r = 0x14; return 1;
       case 0xa9: *r = 0x15; return 1;
       case 0xa0: *r = 0x16; return 1;
       case 0xa8: *r = 0x17; return 1;
       case 0xa3: *r = 0x18; return 1;
       case 0x98: *r = 0x19; return 1;
       case 0x9e: *r = 0x1a; return 1;
       case 0x91: *r = 0x41; return 1;
       case 0x92: *r = 0x42; return 1;
       case 0x95: *r = 0x45; return 1;
       case 0x97: *r = 0x48; return 1;
       case 0x99: *r = 0x49; return 1;
       case 0x9a: *r = 0x4b; return 1;
       case 0x9c: *r = 0x4d; return 1;
       case 0x9d: *r = 0x4e; return 1;
       case 0x9f: *r = 0x4f; return 1;
       case 0xa1: *r = 0x50; return 1;
       case 0xa4: *r = 0x54; return 1;
       case 0xa5: *r = 0x55; return 1;
       case 0xa7: *r = 0x58; return 1;
       case 0x96: *r = 0x5a; return 1;
       }
       return RET_ILUNI;
    }
    return RET_ILUNI;
}

