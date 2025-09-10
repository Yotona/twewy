#include <MSL/Common/ctype_api.h>

#define alph __msl_alpha
#define blnk __msl_blank
#define cntl __msl_cntrl
#define digi __msl_digit | __msl_graph | __msl_print
#define grph __msl_graph
#define lowc __msl_lower | __msl_alpha | __msl_graph | __msl_print
#define prnt __msl_print
#define punc __msl_punct | __msl_graph | __msl_print
#define spac __msl_space
#define uppc __msl_upper | __msl_alpha | __msl_graph | __msl_print
#define hexd __msl_xdigit
#define dhex hexd | digi
#define uhex hexd | uppc
#define lhex hexd | lowc
#define ctbl cntl | blnk
#define ctsp cntl | spac
#define sblp spac | blnk | prnt
#define csbl cntl | spac | blnk

const u16 __ctype_mapC[__msl_cmap_size] = {
    cntl, cntl, cntl, cntl, cntl, cntl, cntl, cntl, cntl, csbl, ctsp, ctsp, ctsp, ctsp, cntl, cntl, cntl, cntl, cntl,
    cntl, cntl, cntl, cntl, cntl, cntl, cntl, cntl, cntl, cntl, cntl, cntl, cntl, sblp, punc, punc, punc, punc, punc,
    punc, punc, punc, punc, punc, punc, punc, punc, punc, punc, dhex, dhex, dhex, dhex, dhex, dhex, dhex, dhex, dhex,
    dhex, punc, punc, punc, punc, punc, punc, punc, uhex, uhex, uhex, uhex, uhex, uhex, uppc, uppc, uppc, uppc, uppc,
    uppc, uppc, uppc, uppc, uppc, uppc, uppc, uppc, uppc, uppc, uppc, uppc, uppc, uppc, uppc, punc, punc, punc, punc,
    punc, punc, lhex, lhex, lhex, lhex, lhex, lhex, lowc, lowc, lowc, lowc, lowc, lowc, lowc, lowc, lowc, lowc, lowc,
    lowc, lowc, lowc, lowc, lowc, lowc, lowc, lowc, lowc, punc, punc, punc, punc, cntl,
};

const u8 __upper_mapC[__msl_cmap_size] = {
    0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10, 0x11, 0x12,
    0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F, ' ',  '!',  '"',  '#',  '$',  '%',
    '&',  '\'', '(',  ')',  '*',  '+',  ',',  '-',  '.',  '/',  '0',  '1',  '2',  '3',  '4',  '5',  '6',  '7',  '8',
    '9',  ':',  ';',  '<',  '=',  '>',  '?',  '@',  'A',  'B',  'C',  'D',  'E',  'F',  'G',  'H',  'I',  'J',  'K',
    'L',  'M',  'N',  'O',  'P',  'Q',  'R',  'S',  'T',  'U',  'V',  'W',  'X',  'Y',  'Z',  '[',  '\\', ']',  '^',
    '_',  '`',  'A',  'B',  'C',  'D',  'E',  'F',  'G',  'H',  'I',  'J',  'K',  'L',  'M',  'N',  'O',  'P',  'Q',
    'R',  'S',  'T',  'U',  'V',  'W',  'X',  'Y',  'Z',  '{',  '|',  '}',  '~',  0x7F,
};
