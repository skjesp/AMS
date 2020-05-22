/*
 * ComponentFont.h
 *
 * Created: 5/20/2020 10:30:47 PM
 *  Author: Admin
 */ 


#ifndef COMPONENTFONT_H_
#define COMPONENTFONT_H_

#define TM_DOT          0x80

#define TM_MINUS        0x40
#define TM_PLUS         0x44
#define TM_BLANK        0x00
#define TM_DEGREES      0x63
#define TM_UNDERSCORE   0x08
#define TM_EQUALS       0x48
#define TM_CHAR_ERR     0x49

uint8_t inline TM1638_map_char(const char ch)
{
	uint8_t rc = 0;

	switch (ch)
	{
		case '-':
		rc = TM_MINUS;
		break;

		case '+':
		rc = TM_PLUS;
		break;

		case ' ':
		rc = TM_BLANK;
		break;

		case '^':
		rc = TM_DEGREES;
		break;

		case '_':
		rc = TM_UNDERSCORE;
		break;

		case '=':
		rc = TM_EQUALS;
		break;

		default:
		break;
	}

	return rc;
}

//      Bits:                 Hex:
//        -- 0 --               -- 01 --
//       |       |             |        |
//       5       1            20        02
//       |       |             |        |
//        -- 6 --               -- 40 --
//       |       |             |        |
//       4       2            10        04
//       |       |             |        |
//        -- 3 --  .7           -- 08 --   .80


#endif /* COMPONENTFONT_H_ */