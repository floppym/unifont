/*
   unigenwidth - IEEE 1003.1-2008 setup to calculate wchar_t string widths.

   Author: Paul Hardy, 2013

   LICENSE:

      This program is free software: you can redistribute it and/or modify
      it under the terms of the GNU General Public License as published by
      the Free Software Foundation, either version 2 of the License, or
      (at your option) any later version.

      This program is distributed in the hope that it will be useful,
      but WITHOUT ANY WARRANTY; without even the implied warranty of
      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
      GNU General Public License for more details.

      You should have received a copy of the GNU General Public License
      along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXSTRING	256


int
main (int argc, char **argv)
{

   int i; /* loop variable */

   char teststring[MAXSTRING];
   int  loc;
   char *gstart;

   char plane0width[0x10000];

   FILE *infilefp;

   /*
      if (argc != 3) {
         fprintf (stderr, "\n\nUsage: %s <unifont-file.hex> <combining.txt>\n\n");
         exit (EXIT_FAILURE);
      }
   */

   /*
      Read the collection of hex glyphs.
   */
   if ((infilefp = fopen (argv[1],"r")) == NULL) {
      fprintf (stderr,"ERROR - hex input file %s not found.\n\n", argv[1]);
      exit (EXIT_FAILURE);
   }

   memset (plane0width, 0, 0x10000 * sizeof (char));

   teststring[MAXSTRING-1] = '\0';
   while (fgets (teststring, MAXSTRING-1, infilefp) != NULL) {
      sscanf (teststring, "%X", &loc);
      gstart = index (teststring,':') + 1;
      plane0width[loc] = strlen (gstart) <= 34 ? 1 : 2;
   }

   fclose (infilefp);

   /*
      Now read the combining character code points.  These have width of 0.
   */
   if ((infilefp = fopen (argv[2],"r")) == NULL) {
      fprintf (stderr,"ERROR - combining characters file %s not found.\n\n", argv[2]);
      exit (EXIT_FAILURE);
   }

   while (fscanf (infilefp, "%X", &loc) != EOF) plane0width[loc] = 0;

   fclose (infilefp);

   /*
      Code Points with Unusual Properties (Unicode Standard, Chapter 4)
   */
   plane0width[0]=0; /* NULL character */
   for (i = 0x0001; i <= 0x001F; i++) plane0width[i]=-1; /* Control Characters */
   for (i = 0x007F; i <= 0x009F; i++) plane0width[i]=-1; /* Control Characters */

   plane0width[0x034F]=0; /* combining grapheme joiner               */
   plane0width[0x180B]=0; /* Mongolian free variation selector one   */
   plane0width[0x180C]=0; /* Mongolian free variation selector two   */
   plane0width[0x180D]=0; /* Mongolian free variation selector three */
   plane0width[0x180E]=0; /* Mongolian vowel separator               */
   plane0width[0x200B]=0; /* zero width space                        */
   plane0width[0x200C]=0; /* zero width non-joiner                   */
   plane0width[0x200D]=0; /* zero width joiner                       */
   plane0width[0x200E]=0; /* left-to-right mark                      */
   plane0width[0x200F]=0; /* right-to-left mark                      */
   plane0width[0x202A]=0; /* left-to-right embedding                 */
   plane0width[0x202B]=0; /* right-to-left embedding                 */
   plane0width[0x202C]=0; /* pop directional formatting              */
   plane0width[0x202D]=0; /* left-to-right override                  */
   plane0width[0x202E]=0; /* right-to-left override                  */
   plane0width[0x2060]=0; /* word joiner                             */
   plane0width[0x2061]=0; /* function application                    */
   plane0width[0x2062]=0; /* invisible times                         */
   plane0width[0x2063]=0; /* invisible separator                     */
   plane0width[0x2064]=0; /* invisible plus                          */
   plane0width[0x206A]=0; /* inhibit symmetric swapping              */
   plane0width[0x206B]=0; /* activate symmetric swapping             */
   plane0width[0x206C]=0; /* inhibit arabic form shaping             */
   plane0width[0x206D]=0; /* activate arabic form shaping            */
   plane0width[0x206E]=0; /* national digit shapes                   */
   plane0width[0x206F]=0; /* nominal digit shapes                    */

   /*
      Ideographic Description Character Left to Right..
      Ideographic Description Character Overlaid
   */
   for (i = 0x2FF0; i <= 0x2FFB; i++) plane0width[i] = 0;

   plane0width[0x303E] = 0; /* ideographic variation indicator */

   /* Variation Selector-1 to Variation Selector-16 */
   for (i = 0xFE00; i <= 0xFE0F; i++) plane0width[i] = 0;

   plane0width[0xFEFF]=0; /* zero width no-break space         */
   plane0width[0xFFF9]=0; /* interlinear annotation anchor     */
   plane0width[0xFFFA]=0; /* interlinear annotation separator  */
   plane0width[0xFFFB]=0; /* interlinear annotation terminator */
   /*
      Let glyph widths represent 0xFFFC (object replacement character)
      and 0xFFFD (replacement character).
   */

   /*
      Hangul Jamo:

         Leading Consonant (Choseong): leave spacing as is.

         Hangul Choseong Filler (U+115F): set width to 2.

         Hangul Jungseong Filler, Hangul Vowel (Jungseong), and
         Final Consonant (Jongseong): set width to 0, because these
         combine with the leading consonant as one composite syllabic
         glyph.  As of Unicode 5.2, the Hangul Jamo block (U+1100..U+11FF)
         is completely filled.
   */
   for (i = 0x1160; i <= 0x11FF; i++) plane0width[i]=0; /* Vowels & Final Consonants */

   /*
      Private Use Area -- the width is undefined, but likely
      to be 2 charcells wide either from a graphic glyph or
      from a four-digit hexadecimal glyph representing the
      code point.  Therefore if any PUA glyph does not have
      a non-zero width yet, assign it a default width of 2.
      The Unicode Standard allows giving PUA characters
      default property values; see for example The Unicode
      Standard Version 5.0, p. 91.  This same default is
      used for higher plane PUA code points below.
   */
   for (i = 0xE000; i <= 0xF8FF; i++) {
      if (plane0width[i] == 0) plane0width[i]=2;
   }

   /*
      <not a character>
   */
   for (i = 0xFDD0; i <= 0xFDEF; i++) plane0width[i] = -1;
   plane0width[0xFFFE] = -1; /* Byte Order Mark */
   plane0width[0xFFFF] = -1; /* Byte Order Mark */

   /* Surrogate Code Points */
   for (i = 0xD800; i <= 0xDFFF; i++) plane0width[i]=-1;


   /*
      Now generate the output file.
   */
   printf ("/*\n");
   printf ("   wcwidth and wcswidth functions, as per IEEE 1003.1-2008\n");
   printf ("   System Interfaces, pp. 2241 and 2251.\n\n");
   printf ("   Author: Paul Hardy, 2013\n\n");
   printf ("   Copyright (c) 2013 Paul Hardy\n\n");
   printf ("   LICENSE:\n");
   printf ("\n");
   printf ("      This program is free software: you can redistribute it and/or modify\n");
   printf ("      it under the terms of the GNU General Public License as published by\n");
   printf ("      the Free Software Foundation, either version 2 of the License, or\n");
   printf ("      (at your option) any later version.\n");
   printf ("\n");
   printf ("      This program is distributed in the hope that it will be useful,\n");
   printf ("      but WITHOUT ANY WARRANTY; without even the implied warranty of\n");
   printf ("      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the\n");
   printf ("      GNU General Public License for more details.\n");
   printf ("\n");
   printf ("      You should have received a copy of the GNU General Public License\n");
   printf ("      along with this program.  If not, see <http://www.gnu.org/licenses/>.\n");
   printf ("*/\n\n");

   printf ("#include <wchar.h>\n\n");
   printf ("#define PLANE1_ZEROES 177\n\n");
   printf ("\n\n");
   printf ("/* wcwidth -- return charcell positions of one code point */\n");
   printf ("inline int\nwcwidth (wchar_t wc)\n{\n");
   printf ("   return (wcswidth (&wc, 1));\n");
   printf ("}\n");
   printf ("\n\n");
   printf ("int\nwcswidth (const wchar_t *pwcs, size_t n)\n{\n\n");
   printf ("   int i;                    /* loop variable                                      */\n");
   printf ("   unsigned codept;          /* Unicode code point of current character            */\n");
   printf ("   unsigned plane;           /* Unicode plane, 0x00..0x10                          */\n");
   printf ("   unsigned lower16;         /* lower 16 bits of Unicode code point                */\n");
   printf ("   int lowpt, midpt, highpt; /* for binary searching in plane1zeroes[]             */\n");
   printf ("   int found;                /* for binary searching in plane1zeroes[]             */\n");
   printf ("   int totalwidth;           /* total width of string, in charcells (1 or 2/glyph) */\n");
   printf ("   int illegalchar;          /* Whether or not tihs code point is illegal          */\n");
   putchar ('\n');

   /*
      Print the plane0width[] array for glyphs widths in the
      Basic Multilingual Plane (Plane 0).
   */
   printf ("   char plane0width[0x10000] = {");
   for (i = 0; i < 0x10000; i++) {
      if ((i & 0x1F) == 0)
         printf ("\n      /* U+%04X */ ", i);
      printf ("%d,", plane0width[i]);
   }
   printf ("};\n\n");

   /*
      Print wide zero-width glyph code points in the
      Supplemental Lingual Plane (Plane 1).
   */
   printf ("\n");
   printf ("   /*\n");
   printf ("      Zero-width code points in Supplemental Multilingual Plane\n");
   printf ("   */\n");
   printf ("   int plane1zeroes[PLANE1_ZEROES] = {\n");
   printf ("      /* Phaistos Disc */\n");
   printf ("      0x0101FD,\n");
   printf ("      /* Kharoshthi Combining Glyphs */\n");
   printf ("      0x010A01,0x010A02,0x010A03,0x010A05,0x010A06,0x010A0C,0x010A0D,\n");
   printf ("      0x010A0E,0x010A0F,0x010A38,0x010A39,0x010A3A,0x010A3F,\n");
   printf ("      /* Brahmi Combining Glyphs */\n");
   printf ("      0x011000,0x011001,0x011002,0x011038,0x011039,0x01103A,0x01103B,\n");
   printf ("      0x01103C,0x01103D,0x01103E,0x01103F,0x011040,0x011041,0x011042,\n");
   printf ("      0x011043,0x011044,0x011045,0x011046,\n");
   printf ("      /* Kaithi Combining Glyphs */\n");
   printf ("      0x011080,0x011081,0x011082,0x0110B0,0x0110B1,0x0110B2,0x0110B3,\n");
   printf ("      0x0110B4,0x0110B5,0x0110B6,0x0110B7,0x0110B8,0x0110B9,0x0110BA,\n");
   printf ("      /* Chakma Combining Glyphs */\n");
   printf ("      0x011100,0x011101,0x011102,0x011127,0x011128,0x011129,0x01112A,\n");
   printf ("      0x01112B,0x01112C,0x01112D,0x01112E,0x01112F,0x011130,0x011131,\n");
   printf ("      0x011132,0x011133,0x011134,\n");
   printf ("      /* Sharada Combining Glyphs */\n");
   printf ("      0x011180,0x011181,0x011182,0x0111B3,0x0111B4,0x0111B5,0x0111B6,\n");
   printf ("      0x0111B7,0x0111B8,0x0111B9,0x0111BA,0x0111BB,0x0111BC,0x0111BD,\n");
   printf ("      0x0111BE,0x0111BF,0x0111C0,\n");
   printf ("      /* Takri Combining Glyphs */\n");
   printf ("      0x0116AB,0x0116AC,0x0116AD,0x0116AE,0x0116AF,0x0116B0,0x0116B1,\n");
   printf ("      0x0116B2,0x0116B3,0x0116B4,0x0116B5,0x0116B6,0x0116B7,\n");
   printf ("      /* Miao Combining Glyphs */\n");
   printf ("      0x016F51,0x016F52,0x016F53,0x016F54,0x016F55,0x016F56,0x016F57,\n");
   printf ("      0x016F58,0x016F59,0x016F5A,0x016F5B,0x016F5C,0x016F5D,0x016F5E,\n");
   printf ("      0x016F5F,0x016F60,0x016F61,0x016F62,0x016F63,0x016F64,0x016F65,\n");
   printf ("      0x016F66,0x016F67,0x016F68,0x016F69,0x016F6A,0x016F6B,0x016F6C,\n");
   printf ("      0x016F6D,0x016F6E,0x016F6F,0x016F70,0x016F71,0x016F72,0x016F73,\n");
   printf ("      0x016F74,0x016F75,0x016F76,0x016F77,0x016F78,0x016F79,0x016F7A,\n");
   printf ("      0x016F7B,0x016F7C,0x016F7D,0x016F7E,\n");
   printf ("      0x016F8F,0x016F90,0x016F91,0x016F92,\n");
   printf ("      /* Musical Symbols Combining Glyphs */\n");
   printf ("      0x01D159,0x01D165,0x01D166,0x01D167,0x01D168,0x01D169,0x01D16D,\n");
   printf ("      0x01D16E,0x01D16F,0x01D170,0x01D171,0x01D172,0x01D17B,0x01D17C,\n");
   printf ("      0x01D17D,0x01D17E,0x01D17F,0x01D180,0x01D181,0x01D182,0x01D185,\n");
   printf ("      0x01D186,0x01D187,0x01D188,0x01D189,0x01D18A,0x01D18B,0x01D1AA,\n");
   printf ("      0x01D1AB,0x01D1AC,0x01D1AD,\n");
   printf ("      /* Ancient Greek Musical Notation */\n");
   printf ("      0x01D242,0x01D243,0x01D244\n");
   printf ("   };\n\n");

   /*
      Execution part of wcswidth.
   */
   printf ("\n");
   printf ("   illegalchar = totalwidth = 0;\n");
   printf ("   for (i = 0; !illegalchar && i < n; i++) {\n");
   printf ("      codept  = pwcs[i];\n");
   printf ("      plane   = codept >> 16;\n");
   printf ("      lower16 = codept & 0xFFFF;\n");
   printf ("      if (plane == 0) { /* the most common case */\n");
   printf ("         if (plane0width[lower16] < 0) illegalchar = 1;\n");
   printf ("         else totalwidth += plane0width[lower16];\n");
   printf ("      }\n");
   printf ("      else { /* a higher plane or beyond Unicode range */\n");
   printf ("         if  (lower16 == 0xFFFE || lower16 == 0xFFFF) {\n");
   printf ("            illegalchar = 1;\n");
   printf ("         }\n");
   printf ("         else if (plane > 0x10) {\n");
   printf ("            illegalchar = 1;\n");
   printf ("         }\n");
   printf ("         else if (plane == 1) { /* Supplementary Multilingual Plane */\n");
   printf ("            /*\n");
   printf ("               Perform a binary search in plane1zeroes[] sparse list for\n");
   printf ("               combining code points and other zero-width code points.\n");
   printf ("            */\n");
   printf ("            lowpt  = 0;\n");
   printf ("            highpt = PLANE1_ZEROES - 1;\n");
   printf ("\n");
   printf ("            while (lowpt < highpt) {\n");
   printf ("               midpt  = (lowpt + highpt) >> 1;\n");
   printf ("               if (plane1zeroes[midpt] < codept)\n");
   printf ("                  lowpt = midpt + 1;\n");
   printf ("               else if (plane1zeroes[midpt] > codept)\n");
   printf ("                  highpt = midpt - 1;\n");
   printf ("               else\n");
   printf ("                  lowpt = highpt = midpt; /* found the match */\n");
   printf ("            }\n");
   printf ("\n");
   printf ("            if (lowpt >= 0 && codept == plane1zeroes[lowpt]) {\n");
   printf ("               found = 1;\n");
   printf ("            }\n");
   printf ("            else if (highpt < PLANE1_ZEROES && codept == plane1zeroes[highpt]) {\n");
   printf ("               found = 1;\n");
   printf ("            }\n");
   printf ("            else {\n");
   printf ("               found = 0;\n");
   printf ("            }\n");
   printf ("\n");
   printf ("            if (!found) totalwidth += 2; /* default for SMP glyphs */\n");
   printf ("         }\n");
   printf ("         /* Other non-printing in higher planes; return -1 as per IEEE 1003.1-2008. */\n");
   printf ("         else if (/* language tags */\n");
   printf ("                  codept == 0x0E0001 || (codept >= 0x0E0020 && codept <= 0x0E007F) ||\n");
   printf ("                  /* variation selectors, 0x0E0100..0x0E01EF */\n");
   printf ("                  (codept >= 0x0E0100 && codept <= 0x0E01EF)) {\n");
   printf ("            illegalchar = 1;\n");
   printf ("         }\n");
   printf ("         /*\n");
   printf ("            Unicode plane 0x02..0x10 printing character\n");
   printf ("         */\n");
   printf ("         else {\n");
   printf ("            totalwidth += 2; /* default width of 2 charcells for legal glyphs */\n");
   printf ("         }\n");
   printf ("\n");
   printf ("      }\n");
   printf ("   }\n");
   printf ("   if (illegalchar) totalwidth = -1;\n");
   printf ("\n");
   printf ("   return (totalwidth);\n");
   printf ("\n");
   printf ("}\n");

   exit (EXIT_SUCCESS);
}
