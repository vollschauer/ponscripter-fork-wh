#include "defs.h"

pstring parseTags(const pstring& src)
{
//fputs("parseTags: ", stdout);
//print_escaped(src, stdout, true);
    pstring rv;
    pstrIter it(src);
    if (it.get() == system_encoding->TextMarker()) {
//puts("TextMarker");
	rv += system_encoding->TextMarker();
	it.next();
    }
    while (it.get() >= 0) {
//fputs("  Char \"", stdout);
//wputc_escaped(it.get());
//putchar('"');
	if (system_encoding->UseTags() && it.get() == '~') {
	    // If end of string, or next char is also ~, treat as literal.
	    it.next();
	    if (it.get() < 0 || it.get() == '~') {
//puts(" is literal");
		rv += '~';
	    }
	    // Otherwise treat as tag group.
	    else {
//puts(" opens tag block:");
		// Parse all elements in group
		while (it.get() != '~') {
//fputs("    Tag starting \"", stdout);
//wputc_escaped(it.get());
//printf(" (%c)\n", *(it.getptr()));
		    int len;
		    rv += system_encoding->TranslateTag(it.getptr(), len);
		    it.forward(len);
		}
	    }
	}
	else {
//puts("");
	    rv += it.getstr();
	}
	it.next();
    }
//puts("  => return " + rv);
    return rv;
}

// Character width conversions stolen from rlvm

static const wchar han2zen_table[] = {
    0xfb, 0xf2, 0xa1, 0xa3, 0xa5, 0xa7, 0xa9, 0xe3,
    0xe5, 0xe7, 0xc3, 0xfc, 0xa2, 0xa4, 0xa6, 0xa8,   
    0xaa, 0xab, 0xad, 0xaf, 0xb1, 0xb3, 0xb5, 0xb7,   
    0xb9, 0xbb, 0xbd, 0xbf, 0xc1, 0xc4, 0xc6, 0xc8,   
    0xca, 0xcb, 0xcc, 0xcd, 0xce, 0xcf, 0xd2, 0xd5,   
    0xd8, 0xdb, 0xde, 0xdf, 0xe0, 0xe1, 0xe2, 0xe4,   
    0xe6, 0xe8, 0xe9, 0xea, 0xeb, 0xec, 0xed, 0xef,   
    0xf3, 0x9b, 0x0c
};

static const wchar zen2han_table[] = {
    0x9e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x67, 0x71,
    0x68, 0x72, 0x69, 0x73, 0x6a, 0x74, 0x6b, 0x75,
    0x76, 0x00, 0x77, 0x00, 0x78, 0x00, 0x79, 0x00,
    0x7a, 0x00, 0x7b, 0x00, 0x7c, 0x00, 0x7d, 0x00,
    0x7e, 0x00, 0x7f, 0x00, 0x80, 0x00, 0x81, 0x00,
    0x6f, 0x82, 0x00, 0x83, 0x00, 0x84, 0x00, 0x85,
    0x86, 0x87, 0x88, 0x89, 0x8a, 0x00, 0x00, 0x8b,
    0x00, 0x00, 0x8c, 0x00, 0x00, 0x8d, 0x00, 0x00,
    0x8e, 0x00, 0x00, 0x8f, 0x90, 0x91, 0x92, 0x93,
    0x6c, 0x94, 0x6d, 0x95, 0x6e, 0x96, 0x97, 0x98,
    0x99, 0x9a, 0x9b, 0x00, 0x9c, 0x00, 0x00, 0x66,
    0x9d, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x65, 0x70
};

pstring hantozen(const pstring& str)
{
    pstring rv;
    for (pstrIter it(str); it.get() >= 0; it.next()) {
	if (it.get() == ' ')
	    rv += system_encoding->Encode(0x3000);
	else if (it.get() >= '!' && it.get() <= '~')
	    rv += system_encoding->Encode(it.get() + 0xfee0);
	else if (it.get() >= 0xff65 && it.get() <= 0xff9f)
	    rv += system_encoding->Encode(0x3000 | han2zen_table[it.get() - 0xff65]);
	else
	    rv += it.getstr();
    }
    return rv;
}

pstring zentohan(const pstring& str)
{
    pstring rv;
    for (pstrIter it(str); it.get() >= 0; it.next()) {
	if (it.get() >= 0xff01 && it.get() <= 0xff5e)
	    rv += system_encoding->Encode(it.get() - 0xfee0);
	else if (it.get() == 0x3000)
	    rv += ' ';
	else if (it.get() >= 0x309b && it.get() <= 0x30fc &&
		 zen2han_table[it.get() - 0x309b])
	    rv += system_encoding->Encode(0xff00 | zen2han_table[it.get() - 0x309b]);
	else
	    rv += it.getstr();
    }
    return rv;
}