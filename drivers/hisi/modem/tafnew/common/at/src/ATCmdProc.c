/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2012-2019. All rights reserved.
 * foss@huawei.com
 *
 * If distributed as part of the Linux kernel, the following license terms
 * apply:
 *
 * * This program is free software; you can redistribute it and/or modify
 * * it under the terms of the GNU General Public License version 2 and
 * * only version 2 as published by the Free Software Foundation.
 * *
 * * This program is distributed in the hope that it will be useful,
 * * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * * GNU General Public License for more details.
 * *
 * * You should have received a copy of the GNU General Public License
 * * along with this program; if not, write to the Free Software
 * * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307, USA
 *
 * Otherwise, the following license terms apply:
 *
 * * Redistribution and use in source and binary forms, with or without
 * * modification, are permitted provided that the following conditions
 * * are met:
 * * 1) Redistributions of source code must retain the above copyright
 * *    notice, this list of conditions and the following disclaimer.
 * * 2) Redistributions in binary form must reproduce the above copyright
 * *    notice, this list of conditions and the following disclaimer in the
 * *    documentation and/or other materials provided with the distribution.
 * * 3) Neither the name of Huawei nor the names of its contributors may
 * *    be used to endorse or promote products derived from this software
 * *    without specific prior written permission.
 *
 * * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 */

#include "ATCmdProc.h"
#include "securec.h"
#include "AtCheckFunc.h"
#include "ppp_interface.h"
#include "taf_mmi_str_parse.h"
#include "app_vc_api.h"
#include "AtDataProc.h"
#include "AtCmdMsgProc.h"
#include "TafStdlib.h"
#include "mn_comm_api.h"
#include "AtEventReport.h"

#include "at_common.h"

#include "at_mdrv_interface.h" /* AT_SysbootShutdown AT_GetTimerAccuracyTimestamp */


/*
 * 协议栈打印打点方式下的.C文件宏定义
 */
#define THIS_FILE_ID PS_FILE_ID_AT_CMDPROC_C

#define AT_VALID_COMMA_NUM 2
#define AT_SIMLOCK_VALID_PARA_NUM 2
#define AT_TPDU_TYPE_MAX_LEN 4
#define AT_SMS_INPUT_TYPE 1
#define AT_WAIT_SMS_INPUT_TYPE 2

#define AT_SIMLOCK_PLMN_MAX_NUM 61
#define AT_CMDTMP_MAX_LEN 100

#define AT_BLANK_ASCII_VALUE 0x20

#define AT_CLCC_PARA_MODE_VOICE 0 /* 语音呼叫模式 */
#define AT_CLCC_PARA_MODE_DATA  1 /* 数据呼叫模式 */
#define AT_CLCC_PARA_MODE_FAX   2 /* 传真模式 */

#define AT_SEND_DATA_BUFF_INDEX_1 1
#define AT_SEND_DATA_BUFF_INDEX_3 3


/*
 * The following tables maps the 256 characters of PBM8 to the corresponding
 * unicode characters.
 */
const AT_PB_ConversionTable g_iraToUnicode[AT_PB_IRA_MAX_NUM] = {
    { 0x00, 0, 0x0000 }, /* Null */
    { 0x01, 0, 0x0001 }, /* Start of heading */
    { 0x02, 0, 0x0002 }, /* Start of text */
    { 0x03, 0, 0x0003 }, /* End of text */
    { 0x04, 0, 0x0004 }, /* End of transmission */
    { 0x05, 0, 0x0005 }, /* Inquiry */
    { 0x06, 0, 0x0006 }, /* ACK */
    { 0x07, 0, 0x0007 }, /* Bell */
    { 0x08, 0, 0x0008 }, /* Backspace */
    { 0x09, 0, 0x0009 }, /* Tab */
    { 0x0A, 0, 0x000A }, /* Line feed */
    { 0x0B, 0, 0x000B }, /* Vertical tab */
    { 0x0C, 0, 0x000C }, /* Form feed */
    { 0x0D, 0, 0x000D }, /* Carriage return */
    { 0x0E, 0, 0x000E }, /* Shift out */
    { 0x0F, 0, 0x000F }, /* Shift in */
    { 0x10, 0, 0x0010 }, /* Data link escape */
    { 0x11, 0, 0x0011 }, /* Device control one */
    { 0x12, 0, 0x0012 }, /* Device control two */
    { 0x13, 0, 0x0013 }, /* Device control three */
    { 0x14, 0, 0x0014 }, /* Device control four */
    { 0x15, 0, 0x0015 }, /* NAK */
    { 0x16, 0, 0x0016 }, /* Synch */
    { 0x17, 0, 0x0017 }, /* End of block */
    { 0x18, 0, 0x0018 }, /* Cancel */
    { 0x19, 0, 0x0019 }, /* End of medium */
    { 0x1A, 0, 0x001A }, /* Substitute */
    { 0x1B, 0, 0x001B }, /* ESC */
    { 0x1C, 0, 0x001C }, /* File separator */
    { 0x1D, 0, 0x001D }, /* Group separator */
    { 0x1E, 0, 0x001E }, /* Record separator */
    { 0x1F, 0, 0x001F }, /* Unit separator */
    { 0x20, 0, 0x0020 }, /* Space */
    { 0x21, 0, 0x0021 }, /* Exclamation mark */
    { 0x22, 0, 0x0022 }, /* Quotation mark */
    { 0x23, 0, 0x0023 }, /* Number sign */
    { 0x24, 0, 0x0024 }, /* Dollar sign */
    { 0x25, 0, 0x0025 }, /* Percent sign */
    { 0x26, 0, 0x0026 }, /* Ampersand */
    { 0x27, 0, 0x0027 }, /* Apostrophe */
    { 0x28, 0, 0x0028 }, /* Left parenthesis */
    { 0x29, 0, 0x0029 }, /* Right parenthesis */
    { 0x2A, 0, 0x002A }, /* Asterisk */
    { 0x2B, 0, 0x002B }, /* Plus */
    { 0x2C, 0, 0x002C }, /* Comma */
    { 0x2D, 0, 0x002D }, /* Hyphen */
    { 0x2E, 0, 0x002E }, /* Full stop */
    { 0x2F, 0, 0x002F }, /* Solidus */
    { 0x30, 0, 0x0030 }, /* Zero */
    { 0x31, 0, 0x0031 }, /* One */
    { 0x32, 0, 0x0032 }, /* Two */
    { 0x33, 0, 0x0033 }, /* Three */
    { 0x34, 0, 0x0034 }, /* Four */
    { 0x35, 0, 0x0035 }, /* Five */
    { 0x36, 0, 0x0036 }, /* Six */
    { 0x37, 0, 0x0037 }, /* Seven */
    { 0x38, 0, 0x0038 }, /* Eight */
    { 0x39, 0, 0x0039 }, /* Nine */
    { 0x3A, 0, 0x003A }, /* Colon */
    { 0x3B, 0, 0x003B }, /* Semicolon */
    { 0x3C, 0, 0x003C }, /* < (Less than) */
    { 0x3D, 0, 0x003D }, /* = Equals */
    { 0x3E, 0, 0x003E }, /* > greater than */
    { 0x3F, 0, 0x003F }, /* Question mark */
    { 0x40, 0, 0x0040 }, /* AT Sign */
    { 0x41, 0, 0x0041 }, /* Latin A */
    { 0x42, 0, 0x0042 }, /* Latin B */
    { 0x43, 0, 0x0043 }, /* Latin C */
    { 0x44, 0, 0x0044 }, /* Latin D */
    { 0x45, 0, 0x0045 }, /* Latin E */
    { 0x46, 0, 0x0046 }, /* Latin F */
    { 0x47, 0, 0x0047 }, /* Latin G */
    { 0x48, 0, 0x0048 }, /* Latin H */
    { 0x49, 0, 0x0049 }, /* Latin I */
    { 0x4A, 0, 0x004A }, /* Latin J */
    { 0x4B, 0, 0x004B }, /* Latin K */
    { 0x4C, 0, 0x004C }, /* Latin L */
    { 0x4D, 0, 0x004D }, /* Latin M */
    { 0x4E, 0, 0x004E }, /* Latin N */
    { 0x4F, 0, 0x004F }, /* Latin O */
    { 0x50, 0, 0x0050 }, /* Latin P */
    { 0x51, 0, 0x0051 }, /* Latin Q */
    { 0x52, 0, 0x0052 }, /* Latin R */
    { 0x53, 0, 0x0053 }, /* Latin S */
    { 0x54, 0, 0x0054 }, /* Latin T */
    { 0x55, 0, 0x0055 }, /* Latin U */
    { 0x56, 0, 0x0056 }, /* Latin V */
    { 0x57, 0, 0x0057 }, /* Latin W */
    { 0x58, 0, 0x0058 }, /* Latin X */
    { 0x59, 0, 0x0059 }, /* Latin Y */
    { 0x5A, 0, 0x005A }, /* Latin Z */
    { 0x5B, 0, 0x005B }, /* Left square bracket */
    { 0x5C, 0, 0x005C }, /* Reverse solidus */
    { 0x5D, 0, 0x005D }, /* Right square bracket */
    { 0x5E, 0, 0x005E }, /* Circumflex accent */
    { 0x5F, 0, 0x005F }, /* Low line */
    { 0x60, 0, 0x0060 }, /* Grave accent */
    { 0x61, 0, 0x0061 }, /* Latin a */
    { 0x62, 0, 0x0062 }, /* Latin b */
    { 0x63, 0, 0x0063 }, /* Latin c */
    { 0x64, 0, 0x0064 }, /* Latin d */
    { 0x65, 0, 0x0065 }, /* Latin e */
    { 0x66, 0, 0x0066 }, /* Latin f */
    { 0x67, 0, 0x0067 }, /* Latin g */
    { 0x68, 0, 0x0068 }, /* Latin h */
    { 0x69, 0, 0x0069 }, /* Latin i */
    { 0x6A, 0, 0x006A }, /* Latin j */
    { 0x6B, 0, 0x006B }, /* Latin k */
    { 0x6C, 0, 0x006C }, /* Latin l */
    { 0x6D, 0, 0x006D }, /* Latin m */
    { 0x6E, 0, 0x006E }, /* Latin n */
    { 0x6F, 0, 0x006F }, /* Latin o */
    { 0x70, 0, 0x0070 }, /* Latin p */
    { 0x71, 0, 0x0071 }, /* Latin q */
    { 0x72, 0, 0x0072 }, /* Latin r */
    { 0x73, 0, 0x0073 }, /* Latin s */
    { 0x74, 0, 0x0074 }, /* Latin t */
    { 0x75, 0, 0x0075 }, /* Latin u */
    { 0x76, 0, 0x0076 }, /* Latin v */
    { 0x77, 0, 0x0077 }, /* Latin w */
    { 0x78, 0, 0x0078 }, /* Latin x */
    { 0x79, 0, 0x0079 }, /* Latin y */
    { 0x7A, 0, 0x007A }, /* Latin z */
    { 0x7B, 0, 0x007B }, /* Left curly bracket */
    { 0x7C, 0, 0x007C }, /* Vertical line */
    { 0x7D, 0, 0x007D }, /* Right curly bracket */
    { 0x7E, 0, 0x007E }, /* Tilde */
    { 0x7F, 0, 0x007F }, /* DEL */
    { 0x80, 0, 0x0080 }, /* control character 0x80 */
    { 0x81, 0, 0x0081 }, /* control character 0x81 */
    { 0x82, 0, 0x0082 }, /* control character 0x82 */
    { 0x83, 0, 0x0083 }, /* control character 0x83 */
    { 0x84, 0, 0x0084 }, /* control character 0x84 */
    { 0x85, 0, 0x0085 }, /* control character 0x85 */
    { 0x86, 0, 0x0086 }, /* control character 0x86 */
    { 0x87, 0, 0x0087 }, /* control character 0x87 */
    { 0x88, 0, 0x0088 }, /* control character 0x88 */
    { 0x89, 0, 0x0089 }, /* control character 0x89 */
    { 0x8A, 0, 0x008A }, /* control character 0x8A */
    { 0x8B, 0, 0x008B }, /* control character 0x8B */
    { 0x8C, 0, 0x008C }, /* control character 0x8C */
    { 0x8D, 0, 0x008D }, /* control character 0x8D */
    { 0x8E, 0, 0x008E }, /* control character 0x8E */
    { 0x8F, 0, 0x008F }, /* control character 0x8F */
    { 0x90, 0, 0x0394 }, /* Greek capital letter delta */
    { 0x91, 0, 0x20AC }, /* Euro sign */
    { 0x92, 0, 0x03A6 }, /* Greek capital letter phi */
    { 0x93, 0, 0x0393 }, /* Greek capital letter gamma */
    { 0x94, 0, 0x039B }, /* Greek capital letter lamda */
    { 0x95, 0, 0x03A9 }, /* Greek capital letter omega */
    { 0x96, 0, 0x03A0 }, /* Greek capital letter pi */
    { 0x97, 0, 0x03A8 }, /* Greek capital letter psi */
    { 0x98, 0, 0x03A3 }, /* Greek capital letter sigma */
    { 0x99, 0, 0x0398 }, /* Greek capital letter theta */
    { 0x9A, 0, 0x039E }, /* Greek capital letter xi */
    { 0x9B, 0, 0x009B }, /* control character 0x9B */
    { 0x9C, 0, 0x009C }, /* control character 0x9C */
    { 0x9D, 0, 0x009D }, /* control character 0x9D */
    { 0x9E, 0, 0x009E }, /* control character 0x9E */
    { 0x9F, 0, 0x009F }, /* control character 0x9F */
    { 0xA0, 0, 0x00A0 }, /* Non-Block Space */
    { 0xA1, 0, 0x00A1 }, /* Inverted exclamation mark */
    { 0xA2, 0, 0x00A2 }, /* Cent sign */
    { 0xA3, 0, 0x00A3 }, /* Pound sign */
    { 0xA4, 0, 0x00A4 }, /* Currency sign */
    { 0xA5, 0, 0x00A5 }, /* Yen sign */
    { 0xA6, 0, 0x00A6 }, /* Broken Vertical bar */
    { 0xA7, 0, 0x00A7 }, /* Section sign */
    { 0xA8, 0, 0x00A8 }, /* Diaeresis */
    { 0xA9, 0, 0x00A9 }, /* Copyright sign */
    { 0xAA, 0, 0x00AA }, /* Feminine ordinal indicator */
    { 0xAB, 0, 0x00AB }, /* Left-pointing double angle quotation mark */
    { 0xAC, 0, 0x00AC }, /* Not sign */
    { 0xAD, 0, 0x00AD }, /* Soft hyphen */
    { 0xAE, 0, 0x00AE }, /* Registered sign */
    { 0xAF, 0, 0x00AF }, /* Macron */
    { 0xB0, 0, 0x00B0 }, /* Degree sign */
    { 0xB1, 0, 0x00B1 }, /* Plus-minus sign */
    { 0xB2, 0, 0x00B2 }, /* Superscript two */
    { 0xB3, 0, 0x00B3 }, /* Superscript three */
    { 0xB4, 0, 0x00B4 }, /* Acute accent */
    { 0xB5, 0, 0x00B5 }, /* Micro sign */
    { 0xB6, 0, 0x00B6 }, /* Pilcrow sign */
    { 0xB7, 0, 0x00B7 }, /* Middle dot */
    { 0xB8, 0, 0x00B8 }, /* Cedilla */
    { 0xB9, 0, 0x00B9 }, /* Superscript one */
    { 0xBA, 0, 0x00BA }, /* Masculine ordinal indicator */
    { 0xBB, 0, 0x00BB }, /* Right-pointing double angle quotation mark */
    { 0xBC, 0, 0x00BC }, /* Fraction one quarter */
    { 0xBD, 0, 0x00BD }, /* Fraction one half */
    { 0xBE, 0, 0x00BE }, /* Fraction three quarters */
    { 0xBF, 0, 0x00BF }, /* Inverted question mark */
    { 0xC0, 0, 0x00C0 }, /* Latin A With grave */
    { 0xC1, 0, 0x00C1 }, /* Latin A With acute */
    { 0xC2, 0, 0x00C2 }, /* Latin A With circumflex */
    { 0xC3, 0, 0x00C3 }, /* Latin A With tilde */
    { 0xC4, 0, 0x00C4 }, /* Latin A With diaeresis */
    { 0xC5, 0, 0x00C5 }, /* Latin A With ring above */
    { 0xC6, 0, 0x00C6 }, /* Latin AE */
    { 0xC7, 0, 0x00C7 }, /* Latin C with cedilla */
    { 0xC8, 0, 0x00C8 }, /* Latin E with grave */
    { 0xC9, 0, 0x00C9 }, /* Latin E with acute */
    { 0xCA, 0, 0x00CA }, /* Latin E with circumflex */
    { 0xCB, 0, 0x00CB }, /* Latin E with diaeresis */
    { 0xCC, 0, 0x00CC }, /* Latin I with grave */
    { 0xCD, 0, 0x00CD }, /* Latin I with acute */
    { 0xCE, 0, 0x00CE }, /* Latin I with circumflex */
    { 0xCF, 0, 0x00CF }, /* Latin I with diaeresis */
    { 0xD0, 0, 0x00D0 }, /* Latin CAPITAL LETTER ETH (Icelandic) */
    { 0xD1, 0, 0x00D1 }, /* Latin CAPITAL LETTER N WITH TILDE */
    { 0xD2, 0, 0x00D2 }, /* Latin CAPITAL LETTER O WITH GRAVE */
    { 0xD3, 0, 0x00D3 }, /* Latin CAPITAL LETTER O WITH ACUTE */
    { 0xD4, 0, 0x00D4 }, /* Latin CAPITAL LETTER O WITH CIRCUMFLEX */
    { 0xD5, 0, 0x00D5 }, /* Latin CAPITAL LETTER O WITH TILDE */
    { 0xD6, 0, 0x00D6 }, /* Latin CAPITAL LETTER O WITH DIAERESIS */
    { 0xD7, 0, 0x00D7 }, /* MULTIPLICATION SIGN */
    { 0xD8, 0, 0x00D8 }, /* Latin CAPITAL LETTER O WITH STROKE */
    { 0xD9, 0, 0x00D9 }, /* Latin CAPITAL LETTER U WITH GRAVE */
    { 0xDA, 0, 0x00DA }, /* Latin CAPITAL LETTER U WITH ACUTE */
    { 0xDB, 0, 0x00DB }, /* Latin CAPITAL LETTER U WITH CIRCUMFLEX */
    { 0xDC, 0, 0x00DC }, /* Latin CAPITAL LETTER U WITH DIAERESIS */
    { 0xDD, 0, 0x00DD }, /* Latin CAPITAL LETTER Y WITH ACUTE */
    { 0xDE, 0, 0x00DE }, /* Latin CAPITAL LETTER THORN (Icelandic) */
    { 0xDF, 0, 0x00DF }, /* Latin SHARP S (German) */
    { 0xE0, 0, 0x00E0 }, /* Latin A WITH GRAVE */
    { 0xE1, 0, 0x00E1 }, /* Latin A WITH ACUTE */
    { 0xE2, 0, 0x00E2 }, /* Latin A WITH CIRCUMFLEX */
    { 0xE3, 0, 0x00E3 }, /* Latin A WITH TILDE */
    { 0xE4, 0, 0x00E4 }, /* Latin A WITH DIAERESIS */
    { 0xE5, 0, 0x00E5 }, /* Latin A WITH RING ABOVE */
    { 0xE6, 0, 0x00E6 }, /* Latin AE */
    { 0xE7, 0, 0x00E7 }, /* Latin C WITH CEDILLA */
    { 0xE8, 0, 0x00E8 }, /* Latin E WITH GRAVE */
    { 0xE9, 0, 0x00E9 }, /* Latin E WITH ACUTE */
    { 0xEA, 0, 0x00EA }, /* Latin E WITH CIRCUMFLEX */
    { 0xEB, 0, 0x00EB }, /* Latin E WITH DIAERESIS */
    { 0xEC, 0, 0x00EC }, /* Latin I WITH GRAVE */
    { 0xED, 0, 0x00ED }, /* Latin I WITH ACUTE */
    { 0xEE, 0, 0x00EE }, /* Latin I WITH CIRCUMFLEX */
    { 0xEF, 0, 0x00EF }, /* Latin I WITH DIAERESIS */
    { 0xF0, 0, 0x00F0 }, /* Latin ETH (Icelandic) */
    { 0xF1, 0, 0x00F1 }, /* Latin N WITH TILDE */
    { 0xF2, 0, 0x00F2 }, /* Latin O WITH GRAVE */
    { 0xF3, 0, 0x00F3 }, /* Latin O WITH ACUTE */
    { 0xF4, 0, 0x00F4 }, /* Latin O WITH CIRCUMFLEX */
    { 0xF5, 0, 0x00F5 }, /* Latin O WITH TILDE */
    { 0xF6, 0, 0x00F6 }, /* Latin O WITH DIAERESIS */
    { 0xF7, 0, 0x00F7 }, /* DIVISION SIGN */
    { 0xF8, 0, 0x00F8 }, /* Latin O WITH STROKE */
    { 0xF9, 0, 0x00F9 }, /* Latin U WITH GRAVE */
    { 0xFA, 0, 0x00FA }, /* Latin U WITH ACUTE */
    { 0xFB, 0, 0x00FB }, /* Latin U WITH CIRCUMFLEX */
    { 0xFC, 0, 0x00FC }, /* Latin U WITH DIAERESIS */
    { 0xFD, 0, 0x00FD }, /* Latin Y WITH ACUTE */
    { 0xFE, 0, 0x00FE }, /* Latin THORN (Icelandic) */
    { 0xFF, 0, 0x00FF }, /* Latin Y WITH DIAERESIS */
};

/*
 * The following table maps the gsm7 alphabet to the corresponding unicode
 * characters. This table is exactly 128 entries large.
 */
const AT_PB_ConversionTable g_gsmToUnicode[AT_PB_GSM_MAX_NUM] = {
    { 0x00, 0, 0x0040 }, /* COMMERCIAL AT */
    { 0x01, 0, 0x00A3 }, /* POUND SIGN */
    { 0x02, 0, 0x0024 }, /* DOLLAR SIGN */
    { 0x03, 0, 0x00A5 }, /* YEN SIGN */
    { 0x04, 0, 0x00E8 }, /* Latin E WITH GRAVE */
    { 0x05, 0, 0x00E9 }, /* Latin E WITH ACUTE */
    { 0x06, 0, 0x00F9 }, /* Latin U WITH GRAVE */
    { 0x07, 0, 0x00EC }, /* Latin I WITH GRAVE */
    { 0x08, 0, 0x00F2 }, /* Latin O WITH GRAVE */
    { 0x09, 0, 0x00E7 }, /* Latin C WITH CEDILLA */
    { 0x0A, 0, 0x000A }, /* LINE FEED */
    { 0x0B, 0, 0x00D8 }, /* Latin CAPITAL LETTER O WITH STROKE */
    { 0x0C, 0, 0x00F8 }, /* Latin O WITH STROKE */
    { 0x0D, 0, 0x000D }, /* CARRIAGE RETURN */
    { 0x0E, 0, 0x00C5 }, /* Latin CAPITAL LETTER A WITH RING ABOVE */
    { 0x0F, 0, 0x00E5 }, /* Latin A WITH RING ABOVE */
    { 0x10, 0, 0x0394 }, /* GREEK CAPITAL LETTER DELTA */
    { 0x11, 0, 0x005F }, /* LOW LINE */
    { 0x12, 0, 0x03A6 }, /* GREEK CAPITAL LETTER PHI */
    { 0x13, 0, 0x0393 }, /* GREEK CAPITAL LETTER GAMMA */
    { 0x14, 0, 0x039B }, /* GREEK CAPITAL LETTER LAMDA */
    { 0x15, 0, 0x03A9 }, /* GREEK CAPITAL LETTER OMEGA */
    { 0x16, 0, 0x03A0 }, /* GREEK CAPITAL LETTER PI */
    { 0x17, 0, 0x03A8 }, /* GREEK CAPITAL LETTER PSI */
    { 0x18, 0, 0x03A3 }, /* GREEK CAPITAL LETTER SIGMA */
    { 0x19, 0, 0x0398 }, /* GREEK CAPITAL LETTER THETA */
    { 0x1A, 0, 0x039E }, /* GREEK CAPITAL LETTER XI */
    { 0x1B, 0, 0x00A0 }, /* ESCAPE TO EXTENSION TABLE */
    { 0x1C, 0, 0x00C6 }, /* Latin CAPITAL LETTER AE */
    { 0x1D, 0, 0x00E6 }, /* Latin AE */
    { 0x1E, 0, 0x00DF }, /* Latin SHARP S (German) */
    { 0x1F, 0, 0x00C9 }, /* Latin CAPITAL LETTER E WITH ACUTE */
    { 0x20, 0, 0x0020 }, /* SPACE */
    { 0x21, 0, 0x0021 }, /* EXCLAMATION MARK */
    { 0x22, 0, 0x0022 }, /* QUOTATION MARK */
    { 0x23, 0, 0x0023 }, /* NUMBER SIGN */
    { 0x24, 0, 0x00A4 }, /* CURRENCY SIGN */
    { 0x25, 0, 0x0025 }, /* PERCENT SIGN */
    { 0x26, 0, 0x0026 }, /* AMPERSAND */
    { 0x27, 0, 0x0027 }, /* APOSTROPHE */
    { 0x28, 0, 0x0028 }, /* LEFT PARENTHESIS */
    { 0x29, 0, 0x0029 }, /* RIGHT PARENTHESIS */
    { 0x2A, 0, 0x002A }, /* ASTERISK */
    { 0x2B, 0, 0x002B }, /* PLUS SIGN */
    { 0x2C, 0, 0x002C }, /* COMMA */
    { 0x2D, 0, 0x002D }, /* HYPHEN-MINUS */
    { 0x2E, 0, 0x002E }, /* FULL STOP */
    { 0x2F, 0, 0x002F }, /* SOLIDUS */
    { 0x30, 0, 0x0030 }, /* DIGIT ZERO */
    { 0x31, 0, 0x0031 }, /* DIGIT ONE */
    { 0x32, 0, 0x0032 }, /* DIGIT TWO */
    { 0x33, 0, 0x0033 }, /* DIGIT THREE */
    { 0x34, 0, 0x0034 }, /* DIGIT FOUR */
    { 0x35, 0, 0x0035 }, /* DIGIT FIVE */
    { 0x36, 0, 0x0036 }, /* DIGIT SIX */
    { 0x37, 0, 0x0037 }, /* DIGIT SEVEN */
    { 0x38, 0, 0x0038 }, /* DIGIT EIGHT */
    { 0x39, 0, 0x0039 }, /* DIGIT NINE */
    { 0x3A, 0, 0x003A }, /* COLON */
    { 0x3B, 0, 0x003B }, /* SEMICOLON */
    { 0x3C, 0, 0x003C }, /* LESS-THAN SIGN */
    { 0x3D, 0, 0x003D }, /* EQUALS SIGN */
    { 0x3E, 0, 0x003E }, /* GREATER-THAN SIGN */
    { 0x3F, 0, 0x003F }, /* QUESTION MARK */
    { 0x40, 0, 0x00A1 }, /* INVERTED EXCLAMATION MARK */
    { 0x41, 0, 0x0041 }, /* Latin CAPITAL LETTER A */
    { 0x42, 0, 0x0042 }, /* Latin CAPITAL LETTER B */
    { 0x43, 0, 0x0043 }, /* Latin CAPITAL LETTER C */
    { 0x44, 0, 0x0044 }, /* Latin CAPITAL LETTER D */
    { 0x45, 0, 0x0045 }, /* Latin CAPITAL LETTER E */
    { 0x46, 0, 0x0046 }, /* Latin CAPITAL LETTER F */
    { 0x47, 0, 0x0047 }, /* Latin CAPITAL LETTER G */
    { 0x48, 0, 0x0048 }, /* Latin CAPITAL LETTER H */
    { 0x49, 0, 0x0049 }, /* Latin CAPITAL LETTER I */
    { 0x4A, 0, 0x004A }, /* Latin CAPITAL LETTER J */
    { 0x4B, 0, 0x004B }, /* Latin CAPITAL LETTER K */
    { 0x4C, 0, 0x004C }, /* Latin CAPITAL LETTER L */
    { 0x4D, 0, 0x004D }, /* Latin CAPITAL LETTER M */
    { 0x4E, 0, 0x004E }, /* Latin CAPITAL LETTER N */
    { 0x4F, 0, 0x004F }, /* Latin CAPITAL LETTER O */
    { 0x50, 0, 0x0050 }, /* Latin CAPITAL LETTER P */
    { 0x51, 0, 0x0051 }, /* Latin CAPITAL LETTER Q */
    { 0x52, 0, 0x0052 }, /* Latin CAPITAL LETTER R */
    { 0x53, 0, 0x0053 }, /* Latin CAPITAL LETTER S */
    { 0x54, 0, 0x0054 }, /* Latin CAPITAL LETTER T */
    { 0x55, 0, 0x0055 }, /* Latin CAPITAL LETTER U */
    { 0x56, 0, 0x0056 }, /* Latin CAPITAL LETTER V */
    { 0x57, 0, 0x0057 }, /* Latin CAPITAL LETTER W */
    { 0x58, 0, 0x0058 }, /* Latin CAPITAL LETTER X */
    { 0x59, 0, 0x0059 }, /* Latin CAPITAL LETTER Y */
    { 0x5A, 0, 0x005A }, /* Latin CAPITAL LETTER Z */
    { 0x5B, 0, 0x00C4 }, /* Latin CAPITAL LETTER A WITH DIAERESIS */
    { 0x5C, 0, 0x00D6 }, /* Latin CAPITAL LETTER O WITH DIAERESIS */
    { 0x5D, 0, 0x00D1 }, /* Latin CAPITAL LETTER N WITH TILDE */
    { 0x5E, 0, 0x00DC }, /* Latin CAPITAL LETTER U WITH DIAERESIS */
    { 0x5F, 0, 0x00A7 }, /* SECTION SIGN */
    { 0x60, 0, 0x00BF }, /* INVERTED QUESTION MARK */
    { 0x61, 0, 0x0061 }, /* Latin A */
    { 0x62, 0, 0x0062 }, /* Latin B */
    { 0x63, 0, 0x0063 }, /* Latin C */
    { 0x64, 0, 0x0064 }, /* Latin D */
    { 0x65, 0, 0x0065 }, /* Latin E */
    { 0x66, 0, 0x0066 }, /* Latin F */
    { 0x67, 0, 0x0067 }, /* Latin G */
    { 0x68, 0, 0x0068 }, /* Latin H */
    { 0x69, 0, 0x0069 }, /* Latin I */
    { 0x6A, 0, 0x006A }, /* Latin J */
    { 0x6B, 0, 0x006B }, /* Latin K */
    { 0x6C, 0, 0x006C }, /* Latin L */
    { 0x6D, 0, 0x006D }, /* Latin M */
    { 0x6E, 0, 0x006E }, /* Latin N */
    { 0x6F, 0, 0x006F }, /* Latin O */
    { 0x70, 0, 0x0070 }, /* Latin P */
    { 0x71, 0, 0x0071 }, /* Latin Q */
    { 0x72, 0, 0x0072 }, /* Latin R */
    { 0x73, 0, 0x0073 }, /* Latin S */
    { 0x74, 0, 0x0074 }, /* Latin T */
    { 0x75, 0, 0x0075 }, /* Latin U */
    { 0x76, 0, 0x0076 }, /* Latin V */
    { 0x77, 0, 0x0077 }, /* Latin W */
    { 0x78, 0, 0x0078 }, /* Latin X */
    { 0x79, 0, 0x0079 }, /* Latin Y */
    { 0x7A, 0, 0x007A }, /* Latin Z */
    { 0x7B, 0, 0x00E4 }, /* Latin A WITH DIAERESIS */
    { 0x7C, 0, 0x00F6 }, /* Latin O WITH DIAERESIS */
    { 0x7D, 0, 0x00F1 }, /* Latin N WITH TILDE */
    { 0x7E, 0, 0x00FC }, /* Latin U WITH DIAERESIS */
    { 0x7F, 0, 0x00E0 }, /* Latin A WITH GRAVE */
};

/*
 * GSM7BIT 向UCS2转换的扩展表,GSM7BIT扩展表扩展标志为0x1b；如下表当GSM7BIT编码中
 * 出现码字0x1B65时，其对应的UCS2编码为0x20AC，此码字代表欧元符
 */
const AT_PB_ConversionTable g_gsm7extToUnicode[AT_PB_GSM7EXT_MAX_NUM] = {
    { 0x65, 0, 0x20AC }, /* EURO SIGN */
    { 0x0A, 0, 0x000C }, /* FORM FEED */
    { 0x14, 0, 0x005E }, /* CIRCUMFLEX ACCENT */
    { 0x28, 0, 0x007B }, /* LEFT CURLY BRACKET */
    { 0x29, 0, 0x007D }, /* RIGHT CURLY BRACKET */
    { 0x2F, 0, 0x005C }, /* REVERSE SOLIDUS */
    { 0x3C, 0, 0x005B }, /* LEFT SQUARE BRACKET */
    { 0x3D, 0, 0x007E }, /* TILDE */
    { 0x3E, 0, 0x005D }, /* RIGHT SQUARE BRACKET */
    { 0x40, 0, 0x007C }, /* VERTICAL LINE */
};

const AT_STRING_Type g_atStringTab[] = {
    { AT_STRING_SM, (TAF_UINT8 *)"\"SM\"" },
    { AT_STRING_ME, (TAF_UINT8 *)"\"ME\"" },
    { AT_STRING_ON, (TAF_UINT8 *)"\"ON\"" },
    { AT_STRING_EN, (TAF_UINT8 *)"\"EN\"" },
    { AT_STRING_FD, (TAF_UINT8 *)"\"FD\"" },
    { AT_STRING_BD, (TAF_UINT8 *)"\"BD\"" },
    { AT_STRING_REC_UNREAD_TEXT, (TAF_UINT8 *)"\"REC UNREAD\"" },
    { AT_STRING_REC_READ_TEXT, (TAF_UINT8 *)"\"REC READ\"" },
    { AT_STRING_STO_UNSENT_TEXT, (TAF_UINT8 *)"\"STO UNSENT\"" },
    { AT_STRING_STO_SENT_TEXT, (TAF_UINT8 *)"\"STO SENT\"" },
    { AT_STRING_ALL_TEXT, (TAF_UINT8 *)"\"ALL\"" },
    { AT_STRING_REC_UNREAD_PDU, (TAF_UINT8 *)"0" },
    { AT_STRING_REC_READ_PDU, (TAF_UINT8 *)"1" },
    { AT_STRING_STO_UNSENT_PDU, (TAF_UINT8 *)"2" },
    { AT_STRING_STO_SENT_PDU, (TAF_UINT8 *)"3" },
    { AT_STRING_ALL_PDU, (TAF_UINT8 *)"4" },
    { AT_STRING_IP, (TAF_UINT8 *)"\"IP\"" },
    { AT_STRING_IPv4, (VOS_UINT8 *)"\"IPv4\"" },
    { AT_STRING_PPP, (TAF_UINT8 *)"\"PPP\"" },
    { AT_STRING_IPV6, (VOS_UINT8 *)"\"IPV6\"" },
    { AT_STRING_IPV4V6, (VOS_UINT8 *)"\"IPV4V6\"" },
    { AT_STRING_IPv6, (VOS_UINT8 *)"\"IPv6\"" },
    { AT_STRING_IPv4v6, (VOS_UINT8 *)"\"IPv4v6\"" },
    { AT_STRING_Ethernet, (VOS_UINT8 *)"\"Ethernet\"" },

    { AT_STRING_0E0, (TAF_UINT8 *)"\"0E0\"" },
    { AT_STRING_1E2, (TAF_UINT8 *)"\"1E2\"" },
    { AT_STRING_7E3, (TAF_UINT8 *)"\"7E3\"" },
    { AT_STRING_1E3, (TAF_UINT8 *)"\"1E3\"" },
    { AT_STRING_1E4, (TAF_UINT8 *)"\"1E4\"" },
    { AT_STRING_1E5, (TAF_UINT8 *)"\"1E5\"" },
    { AT_STRING_1E6, (TAF_UINT8 *)"\"1E6\"" },
    { AT_STRING_1E1, (TAF_UINT8 *)"\"1E1\"" },
    { AT_STRING_5E2, (TAF_UINT8 *)"\"5E2\"" },
    { AT_STRING_5E3, (TAF_UINT8 *)"\"5E3\"" },
    { AT_STRING_4E3, (TAF_UINT8 *)"\"4E3\"" },
    { AT_STRING_6E8, (TAF_UINT8 *)"\"6E8\"" },

    { AT_STRING_CREG, (TAF_UINT8 *)"+CREG: " },
    { AT_STRING_CGREG, (TAF_UINT8 *)"+CGREG: " },
#if (FEATURE_LTE == FEATURE_ON)
    { AT_STRING_CEREG, (TAF_UINT8 *)"+CEREG: " },
#endif
    { AT_STRING_SRVST, (TAF_UINT8 *)"^SRVST: " },
    { AT_STRING_MODE, (TAF_UINT8 *)"^MODE: " },
    { AT_STRING_RSSI, (TAF_UINT8 *)"^RSSI: " },
    { AT_STRING_TIME, (TAF_UINT8 *)"^TIME: " },
    { AT_STRING_CTZV, (TAF_UINT8 *)"+CTZV: " },
    { AT_STRING_CTZE, (TAF_UINT8 *)"+CTZE: " },
    { AT_STRING_ERRRPT, (TAF_UINT8 *)"^ERRRPT:" },

    { AT_STRING_CCALLSTATE, (VOS_UINT8 *)"^CCALLSTATE" },

    { AT_STRING_CERSSI, (VOS_UINT8 *)"^CERSSI:" },

    { AT_STRING_ACINFO, (VOS_UINT8 *)"^ACINFO:" },

    { AT_STRING_CS_CHANNEL_INFO, (VOS_UINT8 *)"^CSCHANNELINFO:" },

    { AT_STRING_RESET, (VOS_UINT8 *)"^RESET:" },

    { AT_STRING_REFCLKFREQ, (VOS_UINT8 *)"^REFCLKFREQ" },

    { AT_STRING_C5GNSSAA, (VOS_UINT8 *)"^C5GNSSAA" },

    { AT_STRING_CPENDINGNSSAI, (VOS_UINT8 *)"^CPENDINGNSSAI" },

    { AT_STRING_REJINFO, (VOS_UINT8 *)"^REJINFO: " },

    { AT_STRING_PLMNSELEINFO, (VOS_UINT8 *)"^PLMNSELEINFO:" },

    { AT_STRING_NETSCAN, (VOS_UINT8 *)"^NETSCAN: " },

#if (FEATURE_IMS == FEATURE_ON)
    { AT_STRING_CIREPH, (VOS_UINT8 *)"+CIREPH" },
    { AT_STRING_CIREPI, (VOS_UINT8 *)"+CIREPI" },
    { AT_STRING_CIREGU, (VOS_UINT8 *)"+CIREGU" },

    { AT_STRING_CALL_MODIFY_IND, (VOS_UINT8 *)"^CALLMODIFYIND:" },
    { AT_STRING_CALL_MODIFY_BEG, (VOS_UINT8 *)"^CALLMODIFYBEG:" },
    { AT_STRING_CALL_MODIFY_END, (VOS_UINT8 *)"^CALLMODIFYEND:" },

    { AT_STRING_ECONFSTATE, (VOS_UINT8 *)"^ECONFSTATE:" },

#endif

#if (FEATURE_UE_MODE_CDMA == FEATURE_ON)
    { AT_STRING_CDISP, (TAF_UINT8 *)"^CDISP: " },
    { AT_STRING_CCONNNUM, (TAF_UINT8 *)"^CCONNNUM: " },
    { AT_STRING_CCALLEDNUM, (TAF_UINT8 *)"^CCALLEDNUM: " },
    { AT_STRING_CCALLINGNUM, (TAF_UINT8 *)"^CCALLINGNUM: " },
    { AT_STRING_CREDIRNUM, (TAF_UINT8 *)"^CREDIRNUM: " },
    { AT_STRING_CSIGTONE, (TAF_UINT8 *)"^CSIGTONE: " },
    { AT_STRING_CLCTR, (TAF_UINT8 *)"^CLCTR: " },
    { AT_STRING_CCWAC, (TAF_UINT8 *)"^CCWAC: " },
#endif
    { AT_STRING_FILECHANGE, (VOS_UINT8 *)"^FILECHANGE:" },

#if ((FEATURE_LTE == FEATURE_ON) && (FEATURE_LTE_MBMS == FEATURE_ON))
    { AT_STRING_MBMSEV, (VOS_UINT8 *)"^MBMSEV:" },
#endif

    { AT_STRING_SRCHEDPLMNLIST, (VOS_UINT8 *)"^SRCHEDPLMNLIST:" },

    { AT_STRING_MCC, (TAF_UINT8 *)"^MCC:" },

    { AT_STRING_CMOLRE, (VOS_UINT8 *)"+CMOLRE: " },
    { AT_STRING_CMOLRN, (VOS_UINT8 *)"+CMOLRN: " },
    { AT_STRING_CMOLRG, (VOS_UINT8 *)"+CMOLRG: " },
    { AT_STRING_CMTLR, (VOS_UINT8 *)"+CMTLR: " },

#if (FEATURE_IMS == FEATURE_ON)
    { AT_STRING_DMCN, (VOS_UINT8 *)"^DMCN" },
#endif

    { AT_STRING_IMS_REG_FAIL, (VOS_UINT8 *)"^IMSREGFAIL: " },

    { AT_STRING_IMSI_REFRESH, (VOS_UINT8 *)"^IMSIREFRESH" },

    { AT_STRING_AFCCLKUNLOCK, (VOS_UINT8 *)"^AFCCLKUNLOCK: " },

    { AT_STRING_IMS_HOLD_TONE, (VOS_UINT8 *)"^IMSHOLDTONE" },

    { AT_STRING_LIMITPDPACT, (VOS_UINT8 *)"^LIMITPDPACT" },

    { AT_STRING_IMS_REG_ERR, (VOS_UINT8 *)"^IMSREGERR:" },

    { AT_STRING_BLOCK_CELL_MCC, (VOS_UINT8 *)"^REPORTBLOCKCELLMCC: " },

    { AT_STRING_CLOUD_DATA, (VOS_UINT8 *)"^REPORTCLOUDDATA: " },

    { AT_STRING_ECC_STATUS, (VOS_UINT8 *)"^ECCSTATUS: " },
    { AT_STRING_EPDUR, (VOS_UINT8 *)"^EPDUR: " },

    { AT_STRING_LCACELLEX, (VOS_UINT8 *)"^LCACELLEX: " },
    { AT_STRING_VT_FLOW_RPT, (VOS_UINT8 *)"^VTFLOWRPT: " },

    { AT_STRING_CALL_ALT_SRV, (VOS_UINT8 *)"^CALLALTSRV" },

    { AT_STRING_FINETIMEINFO, (VOS_UINT8 *)"^FINETIMEINFO: " },
    { AT_STRING_SFN, (VOS_UINT8 *)"^SFN: " },
    { AT_STRING_FINETIMEFAIL, (VOS_UINT8 *)"^FINETIMEFAIL: " },

    { AT_STRING_LRRCUECAPINFONTF, (VOS_UINT8 *)"^LRRCUECAPINFONTF: " },
    { AT_STRING_PHYCOMACK, (VOS_UINT8 *)"^PHYCOMACK: " },

    { AT_STRING_BOOSTERNTF, (VOS_UINT8 *)"^BOOSTERNTF: " },

    { AT_STRING_MTREATTACH, (VOS_UINT8 *)"^MTREATTACH" },

    { AT_STRING_IMPU, (VOS_UINT8 *)"^IMPU: " },

#if (FEATURE_DSDS == FEATURE_ON)
    { AT_STRING_DSDSSTATE, (VOS_UINT8 *)"^DSDSSTATE: " },
#endif

    { AT_STRING_TEMPPROTECT, (VOS_UINT8 *)"^TEMPPROTECT: " },

    { AT_STRING_DIALOGNTF, (VOS_UINT8 *)"^DIALOGNTF" },
    { AT_STRINT_RTTEVENT, (VOS_UINT8 *)"^RTTEVT" },
    { AT_STRINT_RTTERROR, (VOS_UINT8 *)"^RTTERR" },

    { AT_STRING_EMRSSIRPT, (VOS_UINT8 *)"^EMRSSIRPT: " },

    { AT_STRING_ELEVATOR, (VOS_UINT8 *)"^ELEVATOR" },

    { AT_STRING_ULFREQRPT, (VOS_UINT8 *)"^ULFREQRPT" },

    { AT_STRING_PSEUDBTS, (VOS_UINT8 *)"^PSEUDBTS" },
#if (FEATURE_UE_MODE_NR == FEATURE_ON)
    { AT_STRING_C5GREG, (TAF_UINT8 *)"+C5GREG: " },
    { AT_STRING_CSERSSI, (VOS_UINT8 *)"^CSERSSI:" },
    { AT_STRING_NRRCUECAPINFONTF, (VOS_UINT8 *)"^NRRCUECAPINFONTF: " },
    { AT_STRING_NRCACELLRPT, (VOS_UINT8 *)"^NRCACELLRPT" },
    { AT_STRING_NRNWCAP, (VOS_UINT8 *)"^NRNWCAP" },
#endif
    { AT_STRING_NDISSTAT, (VOS_UINT8 *)"^NDISSTAT" },
    { AT_STRING_NDISSTATEX, (VOS_UINT8 *)"^NDISSTATEX" },

    { AT_STRING_DETECTPLMN, (VOS_UINT8 *)"^DETECTPLMN" },

    { AT_STRING_BUTT, (TAF_UINT8 *)"\"\"" },
};

VOS_UINT8       g_atCrLf[]        = "\r\n";         /* 回车换行 */
const TAF_UINT8 g_atCmeErrorStr[] = "+CME ERROR: "; /* 错误提示字串 */
const TAF_UINT8 g_atCmsErrorStr[] = "+CMS ERROR: "; /* 错误提示字串 */

const TAF_UINT8 g_atTooManyParaStr[] = "TOO MANY PARAMETERS"; /* 参数太多提示字串 */

AT_CMD_FORMAT_TYPE g_atCmdFmtType = AT_CMD_TYPE_BUTT;

AT_SEND_DataBuffer g_atSendDataBuff;                       /* 单个命令的返回信息存储区 */
AT_SEND_DataBuffer g_atCombineSendData[AT_MAX_CLIENT_NUM]; /* 组合命令的返回信息存储区 */

/* g_atSendDataBuff.aucBuffer的第0项为MUX的帧头标志 */
TAF_UINT8 *g_atSndCrLfAddr    = &g_atSendDataBuff.buffer[AT_SEND_DATA_BUFF_INDEX_1];
TAF_UINT8 *g_atSndCodeAddress = &g_atSendDataBuff.buffer[AT_SEND_DATA_BUFF_INDEX_3];

TAF_LogPrivacyAtCmdUint32 g_logPrivacyAtCmd = TAF_LOG_PRIVACY_AT_CMD_BUTT;

/*
 * 调用底软的串口数据发送接口时，所传入的指针必需为4字节对齐格式。
 * 而g_atSndCrLfAddr和g_atSndCodeAddress都没有依照4字节对齐，所以需
 * 做一次拷贝操作。(AT命令处理对性能要求不高，故增加一次拷贝是可行的)
 */
TAF_UINT8 g_atSendDataBuffer[AT_CMD_MAX_LEN];

AT_V_TYPE    g_atVType    = AT_V_ENTIRE_TYPE; /* 指示命令返回码类型 */
AT_CSCS_TYPE g_atCscsType = AT_CSCS_IRA_CODE; /* 指示TE编码类型 */

AT_CMD_ECHO_TYPE g_atEType = AT_E_ECHO_CMD;
VOS_UINT32       g_atXType = 0; /* CONNECT <text> result code is given upon entering online data state.
         Dial tone and busy detection are disabled. */
VOS_UINT32 g_atQType = 0;       /* DCE transmits result codes */

MN_OPERATION_ID_T g_opId = 0;

MN_MSG_RawTsData   g_atMsgRawTsData[AT_MSG_MAX_MSG_SEGMENT_NUM];
MN_MSG_TsDataInfo  g_atMsgTsDataInfo;
MN_MSG_SendParm    g_atMsgDirectParm;
MN_MSG_WriteParm   g_atMsgWriteParm;
MN_MSG_SendAckParm g_atMsgAckParm;

AT_CSTA_NumTypeUint8 g_atCstaNumType = (0x80 | (MN_CALL_TON_INTERNATIONAL << 4) | (MN_CALL_NPI_ISDN));
AT_PB_Info           g_pbatInfo;

/* 记录闪电卡版本从上电到拨号成功启动时间，单位秒 */
VOS_UINT32 g_lcStartTime = 0;

TAF_UINT8 g_atPppIndexTab[AT_MAX_CLIENT_NUM]; /* PppId和Index的对应表，下标是PppId */

/* 记录是否解锁，初始值为未解锁，通过命令^DATALOCK解锁成功后修改该变量 */
VOS_BOOL g_atDataLocked = VOS_TRUE;

#if (FEATURE_SECURITY_SHELL == FEATURE_ON)
AT_SP_WordCtx g_spWordCtx = {0};
#endif

static const AT_PS_InternalTimeoutResult g_atInterTimeoutResultTab[] = {
    /* Inter Timer Name */ /* 消息处理函数 */
    { AT_S0_TIMER, AT_RcvTiS0Expired },
    { AT_HOLD_CMD_TIMER, AT_BlockCmdTimeOutProc },
    { AT_SIMLOCKWRITEEX_TIMER, AT_RcvTiSimlockWriteExExpired },
#if (FEATURE_AT_HSUART == FEATURE_ON)
    { AT_VOICE_RI_TIMER, AT_RcvTiVoiceRiExpired },
    { AT_SMS_RI_TIMER, AT_RcvTiSmsRiExpired },
#endif
#if (FEATURE_LTE == FEATURE_ON)
    { AT_SHUTDOWN_TIMER, AT_RcvTiShutDownExpired },
#endif
    { AT_AUTH_PUBKEY_TIMER, AT_RcvTiAuthPubkeyExpired },
#if (FEATURE_LTEV_WL == FEATURE_ON)
    { LEDTEST_TIMER_NAME, AT_RcvTiLedTestExpired },
    { SFPTEST_TIMER_NAME, AT_RcvTiSfpTestExpired },
#endif
};

/*
 * 3 函数、变量声明
 */
VOS_VOID At_AdjustLocalDate(TIME_ZONE_Time *uinversalTime, VOS_INT8 adjustValue, TIME_ZONE_Time *localTime);

VOS_UINT8 *AT_GetCrlf(VOS_VOID)
{
    return g_atCrLf;
}
TAF_UINT8 *AT_GetSendDataBuffer(VOS_VOID)
{
    return g_atSendDataBuffer;
}
AT_SEND_DataBuffer *AT_GetSendDataBuff(VOS_VOID)
{
    return &g_atSendDataBuff;
}
TAF_UINT8 *AT_GetSndCodeAddress(VOS_VOID)
{
    return g_atSndCodeAddress;
}

MN_OPERATION_ID_T At_GetOpId(VOS_VOID)
{
    g_opId++;
    g_opId %= MN_OP_ID_BUTT;
    if (g_opId == 0) {
        g_opId++;
    }
    return (g_opId);
}


TAF_UINT32 At_ClientIdToUserId(TAF_UINT16 clientId, TAF_UINT8 *indexId)
{
    TAF_UINT8 i;
    *indexId = 0;

    /* 判断是否为广播client id */
    /* MODEM 0的广播ClientId */
    if (clientId == AT_BROADCAST_CLIENT_ID_MODEM_0) {
        *indexId = AT_BROADCAST_CLIENT_INDEX_MODEM_0;
        return AT_SUCCESS;
    }

    /* MODEM 1的广播ClientId */
    if (clientId == AT_BROADCAST_CLIENT_ID_MODEM_1) {
        *indexId = AT_BROADCAST_CLIENT_INDEX_MODEM_1;
        return AT_SUCCESS;
    }

    /* MODEM 2的广播ClientId */
    if (clientId == AT_BROADCAST_CLIENT_ID_MODEM_2) {
        *indexId = AT_BROADCAST_CLIENT_INDEX_MODEM_2;
        return AT_SUCCESS;
    }

    /* 查找用户管理表 */
    for (i = 0; i < AT_MAX_CLIENT_NUM; i++) {
        if (g_atClientTab[i].used == AT_CLIENT_NULL) {
            continue;
        }
        if (g_atClientTab[i].clientId == clientId) {
            *indexId = i; /* 返回结果 */
            return AT_SUCCESS;
        }
    }

    return AT_FAILURE;
}


TAF_UINT32 At_ClientIdToUserBroadCastId(TAF_UINT16 clientId, TAF_UINT8 *indexId)
{
    VOS_UINT32    rst;
    ModemIdUint16 modemId;

    modemId = MODEM_ID_0;

    rst = AT_GetModemIdFromClient(clientId, &modemId);
    if (rst != VOS_OK) {
        AT_ERR_LOG1("At_ClientIdToUserBroadCastId:Get ModemID From ClientID fail,ClientID=%d", clientId);
        return AT_FAILURE;
    }

    if (modemId == MODEM_ID_0) {
        *indexId = AT_BROADCAST_CLIENT_INDEX_MODEM_0;
    }
#if (MULTI_MODEM_NUMBER > 1)
    else if (modemId == MODEM_ID_1) {
        *indexId = AT_BROADCAST_CLIENT_INDEX_MODEM_1;
    }
#if (MULTI_MODEM_NUMBER > 2)
    else {
        *indexId = AT_BROADCAST_CLIENT_INDEX_MODEM_2;
    }
#endif
#endif

    return AT_SUCCESS;
}


DMS_PortIdUint16 AT_GetDmsPortIdByClientId(AT_ClientIdUint16 clientId)
{
    return g_atClientTab[clientId].portNo;
}


TAF_VOID At_SendReportMsg(TAF_UINT8 type, TAF_UINT8 *data, TAF_UINT16 len)
{
    MN_AT_IndEvt *event = VOS_NULL_PTR;
    VOS_UINT_PTR  tmpAddr;
    errno_t       memResult;

    /* 增加自定义的ITEM，共4个字节 */
    /*lint -save -e516 */
    event = (MN_AT_IndEvt *)TAF_AllocMsgWithoutHeaderLen(WUEPS_PID_AT, (len + AT_MN_INTERFACE_MSG_HEADER_LEN));
    /*lint -restore */
    if (event == TAF_NULL_PTR) {
        AT_ERR_LOG("At_SendCmdMsg:ERROR:Alloc Msg");
        return;
    }

    TAF_CfgMsgHdr((MsgBlock *)event, AT_GetDestPid(((SI_PB_EventInfo *)data)->clientId, I0_WUEPS_PID_TAF), WUEPS_PID_AT,
                  (len + AT_MN_INTERFACE_MSG_HEADER_LEN));
    event->msgName = type;
    event->len     = len;

    tmpAddr = (VOS_UINT_PTR)event->content;
    if (len > 0) {
        memResult = memcpy_s((VOS_UINT8 *)tmpAddr, len, data, len);
        TAF_MEM_CHK_RTN_VAL(memResult, len, len);
    }

    /* 发送消息到AT_PID; */
    if (TAF_TraceAndSendMsg(WUEPS_PID_AT, event) != 0) {
        AT_ERR_LOG("At_SendReportMsg:ERROR");
        return;
    }

    return;
}

/*
 * Description: 扫描退格符
 * History:
 *  1.Date: 2005-04-19
 *    Author:
 *    Modification: Created function
 */
TAF_UINT32 At_ScanDelChar(TAF_UINT8 *data, TAF_UINT16 *len)
{
    TAF_UINT32 chkLen  = 0;
    TAF_UINT16 lenTemp = 0;
    TAF_UINT8 *write   = data;
    TAF_UINT8 *read    = data;

    if (*len == 0) {
        return AT_FAILURE;
    }

    /* 输入参数检查 */
    while (chkLen++ < *len) {
        if (g_atS5 == *read) { /* 删除上一字符 */
            if (lenTemp > 0) {
                write--;
                lenTemp--;
            }
        } else { /* 选择有效字符 */
            *write++ = *read;
            lenTemp++;
        }
        read++;
    }
    *len = lenTemp;
    return AT_SUCCESS;
}

/*
 * Description: 扫描控制符
 * History:
 *  1.Date: 2005-04-19
 *    Author:
 *    Modification: Created function
 */
TAF_UINT32 At_ScanCtlChar(TAF_UINT8 *data, TAF_UINT16 *len)
{
    TAF_UINT8 *write   = data;
    TAF_UINT8 *read    = data;
    TAF_UINT32 chkLen  = 0;
    TAF_UINT16 lenTemp = 0;

    if (*len == 0) {
        return AT_FAILURE;
    }

    /* 检查 */
    while (chkLen++ < *len) {
        /* 选择有效字符 */
        if (*read >= 0x20) {
            *write++ = *read;
            lenTemp++;
        }
        read++;
    }
    *len = lenTemp;
    return AT_SUCCESS;
}


TAF_UINT32 At_ScanBlankChar(TAF_UINT8 *data, TAF_UINT16 *len)
{
    TAF_UINT8 *check     = data;
    TAF_UINT8 *write     = data;
    TAF_UINT8 *read      = data;
    TAF_UINT32 chkQout   = 0;
    TAF_UINT32 chkLen    = 0;
    TAF_UINT32 qoutNum   = 0;
    TAF_UINT16 qoutCount = 0;
    TAF_UINT16 lenTemp   = 0;

    if (*len == 0) {
        return AT_FAILURE;
    }

    /* 检查引号 */
    while (qoutNum++ < *len) {
        if (*check++ == '"') {
            chkQout++;
        }
    }
    /* 其值只能为偶数，奇数返回错误 */
    if ((chkQout % 2) == 1) {
        return AT_FAILURE;
    }

    /* 检查 */
    while (chkLen++ < *len) {
        /* 当前是第几个引号状态 */
        if (*read == '\"') {
            qoutCount++;
        }

        /* 去掉引号外的空格,除2是为了判断奇偶性 */
        if (((*read != AT_BLANK_ASCII_VALUE) && ((qoutCount % 2) == 0)) || ((qoutCount % 2) == 1)) {
            *write++ = *read;
            lenTemp++;
        }
        read++;
    }
    *len = lenTemp;
    return AT_SUCCESS;
}


MN_MSG_SendParm* At_GetDirectSendMsgMem(TAF_VOID)
{
    (VOS_VOID)memset_s(&g_atMsgDirectParm, sizeof(g_atMsgDirectParm), 0x00, sizeof(g_atMsgDirectParm));
    return &g_atMsgDirectParm;
}


MN_MSG_TsDataInfo* At_GetMsgMem(VOS_VOID)
{
    (VOS_VOID)memset_s(&g_atMsgTsDataInfo, sizeof(g_atMsgTsDataInfo), 0x00, sizeof(g_atMsgTsDataInfo));
    return &g_atMsgTsDataInfo;
}


MN_MSG_RawTsData* At_GetLongMsgSegMem(TAF_VOID)
{
    (VOS_VOID)memset_s(g_atMsgRawTsData, sizeof(g_atMsgRawTsData), 0x00, (sizeof(g_atMsgRawTsData)));
    return g_atMsgRawTsData;
}


MN_MSG_WriteParm* At_GetWriteMsgMem(TAF_VOID)
{
    (VOS_VOID)memset_s(&g_atMsgWriteParm, sizeof(g_atMsgWriteParm), 0x00, sizeof(g_atMsgWriteParm));
    return &g_atMsgWriteParm;
}


MN_MSG_SendAckParm* At_GetAckMsgMem(VOS_VOID)
{
    (VOS_VOID)memset_s(&g_atMsgAckParm, sizeof(g_atMsgAckParm), 0x00, sizeof(g_atMsgAckParm));
    return &g_atMsgAckParm;
}


TAF_UINT32 At_SendPduMsgOrCmd(TAF_UINT8 indexNum, TAF_UINT8 *data, TAF_UINT32 len)
{
    errno_t            memResult;
    TAF_UINT32         ret;
    MN_MSG_SendParm   *sendDirectParm = VOS_NULL_PTR;
    TAF_UINT32         uAddrlLen      = 0;
    MN_OPERATION_ID_T  opId           = At_GetOpId();
    MN_MSG_TsDataInfo *tsDataInfo     = VOS_NULL_PTR;
    MN_MSG_RawTsData  *rawData        = VOS_NULL_PTR;
    AT_ModemSmsCtx    *smsCtx         = VOS_NULL_PTR;

    smsCtx = AT_GetModemSmsCtxAddrFromClientId(indexNum);

    if ((g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_CMGS_PDU_SET) &&
        (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_CMGC_PDU_SET)) {
        AT_WARN_LOG("At_SendPduMsgOrCmd: invalid command operation.");
        return AT_ERROR;
    }

    /* Refer to protocol 31102 4.2.25 */
    if (g_atClientTab[indexNum].atSmsData.pduLen > len) {
        AT_NORM_LOG("At_SendPduMsgOrCmd: invalid tpdu data length.");
        return AT_CMS_INVALID_PDU_MODE_PARAMETER;
    }

    sendDirectParm = At_GetDirectSendMsgMem();
    sendDirectParm->memStore   = MN_MSG_MEM_STORE_NONE;
    sendDirectParm->clientType = MN_MSG_CLIENT_NORMAL;

    /* sca */
    if (len > g_atClientTab[indexNum].atSmsData.pduLen) {
        ret = At_GetScaFromInputStr(data, &sendDirectParm->msgInfo.scAddr, &uAddrlLen);
        if (ret != MN_ERR_NO_ERROR) {
            return AT_CMS_INVALID_PDU_MODE_PARAMETER;
        }

        if (len != (g_atClientTab[indexNum].atSmsData.pduLen + uAddrlLen)) {
            AT_NORM_LOG("At_SendPduMsgOrCmd: invalid tpdu data length.");
            return AT_CMS_INVALID_PDU_MODE_PARAMETER;
        }
    }

    if (sendDirectParm->msgInfo.scAddr.bcdLen == 0) {
        if (smsCtx->cscaCsmpInfo.parmInUsim.scAddr.bcdLen == 0) {
            AT_NORM_LOG("At_SendPduMsgOrCmd: without sca.");
            return AT_CMS_SMSC_ADDRESS_UNKNOWN;
        }
        memResult = memcpy_s(&sendDirectParm->msgInfo.scAddr, sizeof(sendDirectParm->msgInfo.scAddr),
                             &(smsCtx->cscaCsmpInfo.parmInUsim.scAddr), sizeof(smsCtx->cscaCsmpInfo.parmInUsim.scAddr));
        TAF_MEM_CHK_RTN_VAL(memResult, sizeof(sendDirectParm->msgInfo.scAddr), sizeof(smsCtx->cscaCsmpInfo.parmInUsim.scAddr));
    }

    sendDirectParm->msgInfo.tsRawData.len = g_atClientTab[indexNum].atSmsData.pduLen;
    if (g_atClientTab[indexNum].atSmsData.pduLen > 0) {
        memResult = memcpy_s(sendDirectParm->msgInfo.tsRawData.data, sizeof(sendDirectParm->msgInfo.tsRawData.data),
            &data[uAddrlLen], g_atClientTab[indexNum].atSmsData.pduLen);
        TAF_MEM_CHK_RTN_VAL(memResult, sizeof(sendDirectParm->msgInfo.tsRawData.data), g_atClientTab[indexNum].atSmsData.pduLen);
    }

    sendDirectParm->msgInfo.tsRawData.tpduType = MN_MSG_TPDU_COMMAND;
    if (g_atClientTab[indexNum].cmdCurrentOpt == AT_CMD_CMGS_PDU_SET) {
        sendDirectParm->msgInfo.tsRawData.tpduType = MN_MSG_TPDU_SUBMIT;
    }

    /* 增加PDU码有效性检查，删除冗余字段 */
    tsDataInfo = At_GetMsgMem();
    ret        = MN_MSG_Decode(&sendDirectParm->msgInfo.tsRawData, tsDataInfo);
    if (ret != MN_ERR_NO_ERROR) {
        AT_NORM_LOG("At_SendPduMsgOrCmd: fail to decode.");
        return AT_CMS_INVALID_PDU_MODE_PARAMETER;
    }

    rawData = At_GetLongMsgSegMem();
    ret     = MN_MSG_Encode(tsDataInfo, rawData);
    if (ret != MN_ERR_NO_ERROR) {
        AT_NORM_LOG("At_SendPduMsgOrCmd: fail to encode.");
    } else {
        if (rawData->len != sendDirectParm->msgInfo.tsRawData.len) {
            AT_NORM_LOG1("At_SendPduMsgOrCmd: pstSendParm->stMsgInfo.stTsRawData.ulLen not match:",
                         sendDirectParm->msgInfo.tsRawData.len);
            AT_NORM_LOG1("At_SendPduMsgOrCmd: pstRawData->ulLen not match:", rawData->len);
            sendDirectParm->msgInfo.tsRawData.len = rawData->len;
        }
    }

    g_atClientTab[indexNum].opId = opId;
    ret                          = MN_MSG_Send(g_atClientTab[indexNum].clientId, opId, sendDirectParm);
    if (ret == MN_ERR_NO_ERROR) {
        g_atClientTab[indexNum].atSmsData.msgSentSmNum = 1;
        ret                                            = AT_WAIT_ASYNC_RETURN;
    } else {
        if (ret == MN_ERR_CLASS_SMS_NOAVAILDOMAIN) {
            ret = AT_CMS_UNKNOWN_ERROR;
        } else {
            ret = AT_ERROR;
        }
    }
    return ret;
}


TAF_UINT32 At_SendPduMsgAck(TAF_UINT8 indexNum, TAF_UINT8 *data, TAF_UINT32 len)
{
    errno_t             memResult;
    MN_MSG_SendAckParm *ackParm = VOS_NULL_PTR;
    TAF_UINT32          ret;
    MN_OPERATION_ID_T   opId       = At_GetOpId();
    MN_MSG_TsDataInfo  *tsDataInfo = VOS_NULL_PTR;

    if (len != g_atClientTab[indexNum].atSmsData.pduLen) {
        AT_NORM_LOG("At_SendPduMsgAck: the length of PDU is not consistent.");
        return AT_CMS_OPERATION_NOT_ALLOWED; /* 输入字串太长 */
    }

    /* 执行命令操作 */
    ackParm = At_GetAckMsgMem();

    /* g_atClientTab[ucIndex].AtSmsData.ucNumType为0的情况已经在命令设置时直接处理了，此处不考虑 */
    if (g_atClientTab[indexNum].atSmsData.cnmaType == 1) {
        ackParm->rpAck              = TAF_TRUE;
        ackParm->tsRawData.tpduType = MN_MSG_TPDU_DELIVER_RPT_ACK;
    } else {
        ackParm->rpAck              = TAF_FALSE;
        ackParm->rpCause            = MN_MSG_RP_CAUSE_PROTOCOL_ERR_UNSPECIFIED;
        ackParm->tsRawData.tpduType = MN_MSG_TPDU_DELIVER_RPT_ERR;
    }

    /* g_atClientTab[ucIndex].AtSmsData.ucPduLen为0的情况已经在命令设置时直接处理了，此处不考虑 */
    ackParm->tsRawData.len = (TAF_UINT32)g_atClientTab[indexNum].atSmsData.pduLen;
    if ((len > 0) && (len <= MN_MSG_MAX_LEN)) {
        memResult = memcpy_s(ackParm->tsRawData.data, sizeof(ackParm->tsRawData.data), data, len);
        TAF_MEM_CHK_RTN_VAL(memResult, sizeof(ackParm->tsRawData.data), len);
    }
    tsDataInfo = At_GetMsgMem();
    ret        = MN_MSG_Decode(&ackParm->tsRawData, tsDataInfo);
    if (ret != MN_ERR_NO_ERROR) {
        AT_NORM_LOG("At_SendPduMsgOrCmd: Fail to decode.");
        return AT_ERROR;
    }

    if ((ackParm->rpAck == TAF_FALSE) && (tsDataInfo->u.deliverRptErr.failCause == MN_MSG_TP_CAUSE_MEMORY_FULL)) {
        ackParm->rpCause = MN_MSG_RP_CAUSE_MEMORY_EXCEEDED;
    }

    g_atClientTab[indexNum].opId = opId;
    if (MN_MSG_SendAck(g_atClientTab[indexNum].clientId, opId, ackParm) == MN_ERR_NO_ERROR) {
        /* 设置当前操作类型 */
        g_atClientTab[indexNum].cmdCurrentOpt          = AT_CMD_CNMA_PDU_SET;
        g_atClientTab[indexNum].atSmsData.msgSentSmNum = 1;
        ret                                            = AT_WAIT_ASYNC_RETURN; /* 返回命令处理挂起状态 */
    } else {
        ret = AT_ERROR;
    }
    return ret;
}


VOS_UINT32 At_WritePduMsgToMem(TAF_UINT8 indexNum, TAF_UINT8 *data, TAF_UINT32 len)
{
    MN_MSG_WriteParm    *writeParm  = VOS_NULL_PTR;
    MN_MSG_TsDataInfo   *tsDataInfo = VOS_NULL_PTR;
    MN_MSG_RawTsData    *rawData    = VOS_NULL_PTR;
    AT_ModemSmsCtx      *smsCtx     = VOS_NULL_PTR;
    errno_t              memResult;
    MN_OPERATION_ID_T    opId    = At_GetOpId();
    TAF_UINT32           addrLen = 0;
    TAF_UINT32           ret;
    TAF_UINT32           rslt;
    MN_MSG_TpduTypeUint8 mtiMapTpduType[AT_TPDU_TYPE_MAX_LEN] = {
        MN_MSG_TPDU_DELIVER,
        MN_MSG_TPDU_SUBMIT,
        MN_MSG_TPDU_COMMAND,
        MN_MSG_TPDU_MAX
    };
    TAF_UINT8 fo;

    smsCtx = AT_GetModemSmsCtxAddrFromClientId(indexNum);

    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_CMGW_PDU_SET) {
        AT_WARN_LOG("At_WritePduMsgToMem: invalid command operation.");
        return AT_ERROR;
    }

    if (g_atClientTab[indexNum].atSmsData.pduLen > len) {
        AT_NORM_LOG("At_WritePduMsgToMem: invalid tpdu data length.");
        return AT_ERROR;
    }

    writeParm = At_GetWriteMsgMem();

    writeParm->writeMode = MN_MSG_WRITE_MODE_INSERT;
    writeParm->memStore  = smsCtx->cpmsInfo.memSendorWrite;
    writeParm->status    = g_atClientTab[indexNum].atSmsData.smState;

    /* sca */
    if (len > g_atClientTab[indexNum].atSmsData.pduLen) {
        ret = At_GetScaFromInputStr(data, &writeParm->msgInfo.scAddr, &addrLen);
        if (ret != MN_ERR_NO_ERROR) {
            AT_NORM_LOG("At_SendPduMsgOrCmd: fail to get sca from user input.");
            return AT_ERROR;
        }

        if (len != (g_atClientTab[indexNum].atSmsData.pduLen + addrLen)) {
            AT_NORM_LOG("At_SendPduMsgOrCmd: the length of <pdu> is not consistent with <length>.");
            return AT_ERROR;
        }
    }

    if (writeParm->msgInfo.scAddr.bcdLen == 0) {
        if (smsCtx->cscaCsmpInfo.parmInUsim.scAddr.bcdLen == 0) {
            AT_NORM_LOG("At_SendPduMsgOrCmd: without sca.");
        } else {
            memResult = memcpy_s(&writeParm->msgInfo.scAddr, sizeof(writeParm->msgInfo.scAddr),
                &(smsCtx->cscaCsmpInfo.parmInUsim.scAddr), sizeof(smsCtx->cscaCsmpInfo.parmInUsim.scAddr));
            TAF_MEM_CHK_RTN_VAL(memResult, sizeof(writeParm->msgInfo.scAddr), sizeof(smsCtx->cscaCsmpInfo.parmInUsim.scAddr));
        }
    }

    writeParm->msgInfo.tsRawData.len = g_atClientTab[indexNum].atSmsData.pduLen;
    if (g_atClientTab[indexNum].atSmsData.pduLen > 0) {
        memResult = memcpy_s(writeParm->msgInfo.tsRawData.data, sizeof(writeParm->msgInfo.tsRawData.data),
                             &data[addrLen], g_atClientTab[indexNum].atSmsData.pduLen);
        TAF_MEM_CHK_RTN_VAL(memResult, sizeof(writeParm->msgInfo.tsRawData.data),
                            g_atClientTab[indexNum].atSmsData.pduLen);
    }

    fo                                    = writeParm->msgInfo.tsRawData.data[0];
    writeParm->msgInfo.tsRawData.tpduType = mtiMapTpduType[(fo & 0x03)];

    /* 增加PDU码有效性检查，删除冗余字段 */
    tsDataInfo = At_GetMsgMem();
    ret        = MN_MSG_Decode(&writeParm->msgInfo.tsRawData, tsDataInfo);
    if (ret != MN_ERR_NO_ERROR) {
        AT_NORM_LOG("At_WritePduMsgToMem: Fail to decode.");
        return AT_ERROR;
    }

    rawData = At_GetLongMsgSegMem();
    ret     = MN_MSG_Encode(tsDataInfo, rawData);
    if (ret != MN_ERR_NO_ERROR) {
        AT_NORM_LOG("At_WritePduMsgToMem: fail to encode.");
    } else {
        if (rawData->len != writeParm->msgInfo.tsRawData.len) {
            AT_NORM_LOG1("At_WritePduMsgToMem: pstSendParm->stMsgInfo.stTsRawData.ulLen not match:",
                         writeParm->msgInfo.tsRawData.len);
            AT_NORM_LOG1("At_WritePduMsgToMem: pstRawData->ulLen not match:", rawData->len);
            writeParm->msgInfo.tsRawData.len = rawData->len;
        }
    }

    g_atClientTab[indexNum].opId = opId;
    ret                          = MN_MSG_Write(g_atClientTab[indexNum].clientId, opId, writeParm);
    rslt                         = AT_ERROR;
    if (ret == MN_ERR_NO_ERROR) {
        rslt = AT_WAIT_ASYNC_RETURN;
    }

    return rslt;
}


VOS_UINT32 AT_ConvertCharToHex(VOS_UINT8 charValue, VOS_UINT8 *hexValue)
{
    if ((charValue >= '0') && (charValue <= '9')) {
        *hexValue = charValue - '0';
    } else if ((charValue >= 'a') && (charValue <= 'f')) {
        /* 转换'a'到'f'为数字 */
        *hexValue = (charValue - 'a') + AT_DECIMAL_BASE_NUM; /* 将字符串转换成十进制数 */
    } else if ((charValue >= 'A') && (charValue <= 'F')) {
        /* 转换'a'到'f'为数字 */
        *hexValue = (charValue - 'A') + AT_DECIMAL_BASE_NUM; /* 将字符串转换成十进制数 */
    } else {
        return AT_FAILURE;
    }

    return AT_SUCCESS;
}


VOS_UINT32 AT_GetInvalidCharLengthForSms(VOS_UINT8 *pdu, VOS_UINT32 len)
{
    VOS_UINT32 loop;
    VOS_UINT32 ret;
    VOS_UINT32 invalidCharLengthTemp;
    VOS_UINT8  hex;
    VOS_UINT32 invalidCharLength;

    invalidCharLengthTemp = 0;
    for (loop = 0; loop < len; loop++) {
        ret = AT_ConvertCharToHex(pdu[loop], &hex);
        if (ret != AT_FAILURE) {
            break;
        }

        invalidCharLengthTemp++;
    }

    invalidCharLength = invalidCharLengthTemp;

    return invalidCharLength;
}


VOS_UINT32 AT_ProcAbnormalPdu(VOS_UINT8 *pdu, VOS_UINT32 len)
{
    VOS_UINT32 invalidCharLength;

    if (pdu[len] == AT_ESC) {
        return AT_CMS_INVALID_PDU_MODE_PARAMETER;
    }

    /*
     * 进入At_SmsPduProc处理函数只有结束符为回车,CTRL+Z和ESC三种情况，
     * At_SmsPduProc函数的第一个CASE处理了CTRL+Z,本函数前面流程处理了ESC情况
     * 这段代码是针对字符串结束符为回车的处理
     */
    invalidCharLength = AT_GetInvalidCharLengthForSms(pdu, len);
    if (invalidCharLength == len) {
        return AT_SUCCESS;
    } else {
        return AT_CMS_INVALID_PDU_MODE_PARAMETER;
    }
}


TAF_UINT32 At_SmsPduProc(TAF_UINT8 indexNum, TAF_UINT8 *data, TAF_UINT16 len)
{
    VOS_UINT32 invalidCharLength;
    TAF_UINT16 tmpLen = len;
    TAF_UINT32 ret;

    tmpLen -= 1; /* 找到最后一个字符 */
    switch (data[tmpLen]) {
        /* 根据最后一个字符决定下一步操作 */
        case AT_CTRL_Z:
            /* 根据当前用户的命令索引,发送/写/PDU[短信/命令] */
            invalidCharLength = AT_GetInvalidCharLengthForSms(data, tmpLen);
            data += invalidCharLength;
            tmpLen -= (VOS_UINT16)invalidCharLength;

            if (At_UnicodePrint2Unicode(data, &tmpLen) == AT_FAILURE) {
                return AT_CMS_INVALID_PDU_MODE_PARAMETER;
            }

            switch (g_atClientTab[indexNum].cmdCurrentOpt) {
                case AT_CMD_CMGS_PDU_SET:
                case AT_CMD_CMGC_PDU_SET:
                    ret = At_SendPduMsgOrCmd(indexNum, data, tmpLen);
                    break;

                case AT_CMD_CMGW_PDU_SET:
                    ret = At_WritePduMsgToMem(indexNum, data, tmpLen);
                    break;

                case AT_CMD_CNMA_PDU_SET:
                    ret = At_SendPduMsgAck(indexNum, data, tmpLen);
                    break;

                default:
                    ret = AT_ERROR; /*  返回错误 */
            }
            break;
        default:
            ret = AT_ProcAbnormalPdu(data, tmpLen); /*  返回错误 */
            break;
    }

    return ret;
}


VOS_UINT32 At_EncodeTextMsgTpUd(VOS_UINT8 indexNum, MN_MSG_MsgCodingUint8 msgCoding, MN_MSG_UserData *userData)
{
    VOS_UINT32 ret;
    errno_t    memResult;

    if (g_atClientTab[indexNum].smsTxtLen > MN_MSG_MAX_LEN) {
        AT_NORM_LOG("At_EncodeTextMsgTpUd: invalid text message length.");
        return AT_CMS_INVALID_TEXT_MODE_PARAMETER;
    }

    if ((g_atCscsType == AT_CSCS_IRA_CODE) && (msgCoding == MN_MSG_MSG_CODING_7_BIT)) {
        ret = TAF_STD_ConvertAsciiToDefAlpha(g_atClientTab[indexNum].atSmsData.buffer,
                                             g_atClientTab[indexNum].smsTxtLen, userData->orgData, &userData->len,
                                             MN_MSG_MAX_LEN);
        if (ret == MN_ERR_INVALIDPARM) {
            AT_NORM_LOG("At_EncodeTextMsgTpUd: TAF_STD_ConvertAsciiToDefAlpha fail.");
            return AT_CMS_INVALID_TEXT_MODE_PARAMETER;
        }

    } else if ((g_atCscsType == AT_CSCS_GSM_7Bit_CODE) && (msgCoding == MN_MSG_MSG_CODING_8_BIT)) {
        TAF_STD_ConvertDefAlphaToAscii(g_atClientTab[indexNum].atSmsData.buffer, g_atClientTab[indexNum].smsTxtLen,
                                       userData->orgData, &userData->len);
        return AT_SUCCESS;
    } else {
        userData->len = g_atClientTab[indexNum].smsTxtLen;
        if (userData->len > 0) {
            memResult = memcpy_s(userData->orgData, MN_MSG_MAX_LEN, g_atClientTab[indexNum].atSmsData.buffer,
                                 userData->len);
            TAF_MEM_CHK_RTN_VAL(memResult, MN_MSG_MAX_LEN, userData->len);
        }
    }

    return AT_SUCCESS;
}


TAF_UINT32 At_SendTextMsgOrCmd(TAF_UINT8 indexNum)
{
    errno_t            memResult;
    MN_MSG_TsDataInfo *tsDataInfo = VOS_NULL_PTR;
    TAF_UINT32         ret;
    MN_MSG_SendParm   *sendDirectParm = VOS_NULL_PTR;
    MN_OPERATION_ID_T  opId           = At_GetOpId();
    AT_ModemSmsCtx    *smsCtx         = VOS_NULL_PTR;

    if ((g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_CMGS_TEXT_SET) &&
        (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_CMGC_TEXT_SET)) {
        AT_WARN_LOG("At_SendTextMsgOrCmd: invalid command operation.");
        return AT_ERROR;
    }

    smsCtx = AT_GetModemSmsCtxAddrFromClientId(indexNum);

    tsDataInfo                 = At_GetMsgMem();
    sendDirectParm             = At_GetDirectSendMsgMem();
    sendDirectParm->memStore   = MN_MSG_MEM_STORE_NONE;
    sendDirectParm->clientType = MN_MSG_CLIENT_NORMAL;

    /* sc */
    if (smsCtx->cscaCsmpInfo.parmInUsim.scAddr.bcdLen == 0) {
        AT_NORM_LOG("At_SendTextMsgOrCmd: no SCA.");
        return AT_CMS_SMSC_ADDRESS_UNKNOWN;
    }
    memResult = memcpy_s(&sendDirectParm->msgInfo.scAddr, sizeof(sendDirectParm->msgInfo.scAddr),
        &(smsCtx->cscaCsmpInfo.parmInUsim.scAddr), sizeof(smsCtx->cscaCsmpInfo.parmInUsim.scAddr));
    TAF_MEM_CHK_RTN_VAL(memResult, sizeof(sendDirectParm->msgInfo.scAddr), sizeof(smsCtx->cscaCsmpInfo.parmInUsim.scAddr));

    if (g_atClientTab[indexNum].cmdCurrentOpt == AT_CMD_CMGS_TEXT_SET) {
        tsDataInfo->tpduType = MN_MSG_TPDU_SUBMIT;

        /* Fo */
        At_SendMsgFoAttr(indexNum, tsDataInfo);

        /* Mr填0,由MN修改 */

        /* Da */
        memResult = memcpy_s(&tsDataInfo->u.submit.destAddr, sizeof(tsDataInfo->u.submit.destAddr),
            &g_atClientTab[indexNum].atSmsData.asciiAddr, sizeof(g_atClientTab[indexNum].atSmsData.asciiAddr));
        TAF_MEM_CHK_RTN_VAL(memResult, sizeof(tsDataInfo->u.submit.destAddr), sizeof(g_atClientTab[indexNum].atSmsData.asciiAddr));

        /* PID */
        tsDataInfo->u.submit.pid = smsCtx->cscaCsmpInfo.parmInUsim.pid;

        /* DCS */
        ret = MN_MSG_DecodeDcs(smsCtx->cscaCsmpInfo.parmInUsim.dcs, &tsDataInfo->u.submit.dcs);
        if (ret != MN_ERR_NO_ERROR) {
            AT_NORM_LOG("At_SendTextMsgOrCmd: Fail to decode DCS.");
            return AT_ERROR;
        }

        /* VP */
        memResult = memcpy_s(&tsDataInfo->u.submit.validPeriod, sizeof(tsDataInfo->u.submit.validPeriod),
                             &(smsCtx->cscaCsmpInfo.vp), sizeof(smsCtx->cscaCsmpInfo.vp));
        TAF_MEM_CHK_RTN_VAL(memResult, sizeof(tsDataInfo->u.submit.validPeriod), sizeof(smsCtx->cscaCsmpInfo.vp));

        /* UDL UD */
        ret = At_EncodeTextMsgTpUd(indexNum, tsDataInfo->u.submit.dcs.msgCoding, &tsDataInfo->u.submit.userData);
        if (ret != AT_SUCCESS) {
            AT_WARN_LOG("At_WriteTextMsgToMem: Fail to get User Data.");
            return ret;
        }
    } else {
        tsDataInfo->tpduType = MN_MSG_TPDU_COMMAND;
        /* Fo */
        At_SendMsgFoAttr(indexNum, tsDataInfo);
        /* Mr填0,由MN修改 */
        /* PID */
        /* PID */
        tsDataInfo->u.command.pid = g_atClientTab[indexNum].atSmsData.pid;

        /* TP-CT */
        tsDataInfo->u.command.cmdType = g_atClientTab[indexNum].atSmsData.commandType;
        /* TP-MN */ /* 此处去掉了绝对编号类型TAF_SMS_CMD_MSG_NUM_ABSOLUTE,需确认 */
        tsDataInfo->u.command.msgNumber = g_atClientTab[indexNum].atSmsData.messageNumber;

        /* Da */
        memResult = memcpy_s(&tsDataInfo->u.command.destAddr, sizeof(tsDataInfo->u.command.destAddr),
            &g_atClientTab[indexNum].atSmsData.asciiAddr, sizeof(g_atClientTab[indexNum].atSmsData.asciiAddr));
        TAF_MEM_CHK_RTN_VAL(memResult, sizeof(tsDataInfo->u.command.destAddr),
                            sizeof(g_atClientTab[indexNum].atSmsData.asciiAddr));
        /* CDL CD */
        if (g_atClientTab[indexNum].smsTxtLen > MN_MSG_MAX_COMMAND_DATA_LEN) {
            AT_NORM_LOG("At_SendTextMsgOrCmd: invalid text command length.");
            return AT_ERROR;
        }

        /* 这里使用了g_atClientTab[ucIndex].usSmsTxtLen作为命令长度，与原程序不同，需确认 */
        tsDataInfo->u.command.commandDataLen = (TAF_UINT8)g_atClientTab[indexNum].smsTxtLen;
        if (g_atClientTab[indexNum].smsTxtLen > 0) {
            memResult = memcpy_s(tsDataInfo->u.command.cmdData, MN_MSG_MAX_COMMAND_DATA_LEN,
                                 g_atClientTab[indexNum].atSmsData.buffer, g_atClientTab[indexNum].smsTxtLen);
            TAF_MEM_CHK_RTN_VAL(memResult, MN_MSG_MAX_COMMAND_DATA_LEN, g_atClientTab[indexNum].smsTxtLen);
        }
    }

    ret = MN_MSG_Encode(tsDataInfo, &sendDirectParm->msgInfo.tsRawData);
    if (ret != MN_ERR_NO_ERROR) {
        AT_WARN_LOG("At_SendTextMsgOrCmd: Fail to encode sms message or command.");
        return At_ChgMnErrCodeToAt(indexNum, ret);
    }

    g_atClientTab[indexNum].opId = opId;
    ret = MN_MSG_Send(g_atClientTab[indexNum].clientId, g_atClientTab[indexNum].opId, sendDirectParm);
    if (ret != MN_ERR_NO_ERROR) {
        AT_ERR_LOG("At_SendTextMsgOrCmd: Fail to send sms message or command.");
        if (ret == MN_ERR_CLASS_SMS_NOAVAILDOMAIN) {
            ret = AT_CMS_UNKNOWN_ERROR;
        } else {
            ret = AT_ERROR;
        }
    } else {
        ret                                            = AT_WAIT_ASYNC_RETURN;
        g_atClientTab[indexNum].atSmsData.msgSentSmNum = 1;
    }
    return ret;
}


TAF_UINT32 At_WriteTextMsgToMem(TAF_UINT8 indexNum)
{
    errno_t            memResult;
    MN_MSG_TsDataInfo *tsDataInfo = VOS_NULL_PTR;
    TAF_UINT32         ret;
    MN_MSG_WriteParm  *writeParm = VOS_NULL_PTR;
    MN_MSG_DcsCode    *dcs       = VOS_NULL_PTR;
    AT_ModemSmsCtx    *smsCtx    = VOS_NULL_PTR;

    smsCtx = AT_GetModemSmsCtxAddrFromClientId(indexNum);

    if (g_atClientTab[indexNum].cmdCurrentOpt != AT_CMD_CMGW_TEXT_SET) {
        AT_WARN_LOG("At_WriteTextMsgToMem: invalid command operation.");
        return AT_ERROR;
    }

    tsDataInfo           = At_GetMsgMem();
    writeParm            = At_GetWriteMsgMem();
    writeParm->writeMode = MN_MSG_WRITE_MODE_INSERT;
    writeParm->memStore  = smsCtx->cpmsInfo.memSendorWrite;
    writeParm->status    = g_atClientTab[indexNum].atSmsData.smState;

    if (smsCtx->cscaCsmpInfo.parmInUsim.scAddr.bcdLen == 0) {
        AT_NORM_LOG("At_WriteTextMsgToMem: no SCA.");
        return AT_ERROR;
    }
    memResult = memcpy_s(&writeParm->msgInfo.scAddr, sizeof(writeParm->msgInfo.scAddr),
                         &(smsCtx->cscaCsmpInfo.parmInUsim.scAddr), sizeof(smsCtx->cscaCsmpInfo.parmInUsim.scAddr));
    TAF_MEM_CHK_RTN_VAL(memResult, sizeof(writeParm->msgInfo.scAddr), sizeof(smsCtx->cscaCsmpInfo.parmInUsim.scAddr));

    if ((g_atClientTab[indexNum].atSmsData.smState == MN_MSG_STATUS_MO_NOT_SENT) ||
        (g_atClientTab[indexNum].atSmsData.smState == MN_MSG_STATUS_MO_SENT)) {
        tsDataInfo->tpduType = MN_MSG_TPDU_SUBMIT;

        /* Fo */
        At_SendMsgFoAttr(indexNum, tsDataInfo);

        /* Mr填0,由MN修改 */
        tsDataInfo->u.submit.mr = 0xff;

        /* Da */
        memResult = memcpy_s(&tsDataInfo->u.submit.destAddr, sizeof(tsDataInfo->u.submit.destAddr),
            &g_atClientTab[indexNum].atSmsData.asciiAddr, sizeof(g_atClientTab[indexNum].atSmsData.asciiAddr));
        TAF_MEM_CHK_RTN_VAL(memResult, sizeof(tsDataInfo->u.submit.destAddr), sizeof(g_atClientTab[indexNum].atSmsData.asciiAddr));

        /* PID */
        tsDataInfo->u.submit.pid = smsCtx->cscaCsmpInfo.parmInUsim.pid;

        /* DCS */
        dcs = &tsDataInfo->u.submit.dcs;
        ret = MN_MSG_DecodeDcs(smsCtx->cscaCsmpInfo.parmInUsim.dcs, dcs);
        if (ret != MN_ERR_NO_ERROR) {
            AT_NORM_LOG("At_WriteTextMsgToMem: Fail to decode DCS.");
            return AT_ERROR;
        }

        /* VP */
        memResult = memcpy_s(&tsDataInfo->u.submit.validPeriod, sizeof(tsDataInfo->u.submit.validPeriod),
            &(smsCtx->cscaCsmpInfo.vp), sizeof(smsCtx->cscaCsmpInfo.vp));
        TAF_MEM_CHK_RTN_VAL(memResult, sizeof(tsDataInfo->u.submit.validPeriod), sizeof(smsCtx->cscaCsmpInfo.vp));

        /* UDL UD */
        ret = At_EncodeTextMsgTpUd(indexNum, tsDataInfo->u.submit.dcs.msgCoding, &tsDataInfo->u.submit.userData);
        if (ret != AT_SUCCESS) {
            AT_WARN_LOG("At_WriteTextMsgToMem: Fail to get User Data.");
            return ret;
        }
    } else {
        tsDataInfo->tpduType = MN_MSG_TPDU_DELIVER;

        /* Fo */
        At_SendMsgFoAttr(indexNum, tsDataInfo);

        /* Mr填0,由MN修改 */

        /* Da */
        memResult = memcpy_s(&tsDataInfo->u.deliver.origAddr, sizeof(tsDataInfo->u.submit.destAddr),
            &g_atClientTab[indexNum].atSmsData.asciiAddr, sizeof(g_atClientTab[indexNum].atSmsData.asciiAddr));
        TAF_MEM_CHK_RTN_VAL(memResult, sizeof(tsDataInfo->u.submit.destAddr), sizeof(g_atClientTab[indexNum].atSmsData.asciiAddr));

        /* PID */
        tsDataInfo->u.deliver.pid = smsCtx->cscaCsmpInfo.parmInUsim.pid;

        /* DCS */
        dcs = &tsDataInfo->u.deliver.dcs;
        ret = MN_MSG_DecodeDcs(smsCtx->cscaCsmpInfo.parmInUsim.dcs, dcs);
        if (ret != MN_ERR_NO_ERROR) {
            AT_NORM_LOG("At_WriteTextMsgToMem: Fail to decode DCS.");
            return AT_ERROR;
        }

        /* SCTS?? */

        /* UDL UD */
        ret = At_EncodeTextMsgTpUd(indexNum, tsDataInfo->u.deliver.dcs.msgCoding, &tsDataInfo->u.deliver.userData);

        if (ret != AT_SUCCESS) {
            AT_WARN_LOG("At_WriteTextMsgToMem: Fail to get User Data.");
            return ret;
        }
    }

    ret = MN_MSG_Encode(tsDataInfo, &writeParm->msgInfo.tsRawData);
    if (ret != MN_ERR_NO_ERROR) {
        AT_WARN_LOG("At_WriteTextMsgToMem: Fail to encode sms message or command.");
        return AT_ERROR;
    }

    g_atClientTab[indexNum].opId = At_GetOpId();
    ret = MN_MSG_Write(g_atClientTab[indexNum].clientId, g_atClientTab[indexNum].opId, writeParm);
    if (ret != MN_ERR_NO_ERROR) {
        ret = AT_ERROR;
    } else {
        ret = AT_WAIT_ASYNC_RETURN;
    }
    return ret;
}


TAF_UINT32 At_SmsTextProc(TAF_UINT8 indexNum, TAF_UINT8 *data, TAF_UINT16 len)
{
    TAF_UINT16 tmpLen      = len;
    TAF_UINT8  smsProcType = 0;
    TAF_UINT32 ret;
    errno_t    memResult;
    TAF_UINT32 cmdValidFLg;

    if (tmpLen < 1) {
        return AT_CMS_INVALID_TEXT_MODE_PARAMETER;
    }

    if (data[tmpLen - 1] == AT_CTRL_Z) {
        smsProcType = AT_SMS_INPUT_TYPE;

    } else if (g_atS3 == data[tmpLen - 1]) {
        smsProcType = AT_WAIT_SMS_INPUT_TYPE;

    } else {
        return AT_CMS_INVALID_TEXT_MODE_PARAMETER;
    }

    if (g_atCscsType != AT_CSCS_IRA_CODE) {
        tmpLen -= 1;

        if (At_UnicodePrint2Unicode(data, &tmpLen) == AT_FAILURE) {
            return AT_CMS_INVALID_TEXT_MODE_PARAMETER;
        }

        /* UNICODE 长度判断 */
        if ((g_atClientTab[indexNum].smsTxtLen + tmpLen) > AT_UNICODE_SMS_MAX_LENGTH) {
            return AT_CMS_INVALID_TEXT_MODE_PARAMETER;
        }
    } else {
        if (data[tmpLen - 1] == AT_CTRL_Z) {
            tmpLen -= 1;
        }

        /* ASCII 长度判断 */
        if ((g_atClientTab[indexNum].smsTxtLen + tmpLen) > AT_ASCII_SMS_MAX_LENGTH) {
            return AT_CMS_INVALID_TEXT_MODE_PARAMETER;
        }
    }

    cmdValidFLg = (g_atClientTab[indexNum].cmdCurrentOpt == AT_CMD_CMGS_TEXT_SET) ||
                  (g_atClientTab[indexNum].cmdCurrentOpt == AT_CMD_CMGW_TEXT_SET) ||
                  (g_atClientTab[indexNum].cmdCurrentOpt == AT_CMD_CMGC_TEXT_SET);

    if (cmdValidFLg == VOS_FALSE) {
        return AT_ERROR;
    }

    if (tmpLen > 0) {
        memResult = memcpy_s(&g_atClientTab[indexNum].atSmsData.buffer[g_atClientTab[indexNum].smsTxtLen],
                             AT_SMS_SEG_MAX_LENGTH - g_atClientTab[indexNum].smsTxtLen, data, tmpLen);
        TAF_MEM_CHK_RTN_VAL(memResult, AT_SMS_SEG_MAX_LENGTH - g_atClientTab[indexNum].smsTxtLen, tmpLen);
    }

    g_atClientTab[indexNum].smsTxtLen += tmpLen;

    /* 删除冗余else */
    if (smsProcType != 1) {
        return AT_WAIT_SMS_INPUT;
    }

    switch (g_atClientTab[indexNum].cmdCurrentOpt) {
        case AT_CMD_CMGS_TEXT_SET:
        case AT_CMD_CMGC_TEXT_SET:
            ret = At_SendTextMsgOrCmd(indexNum);
            break;

        default:
            ret = At_WriteTextMsgToMem(indexNum);
            break;
    }

    return ret;
}

TAF_UINT32 At_SmsProc(TAF_UINT8 indexNum, TAF_UINT8 *data, TAF_UINT16 len)
{
    AT_ModemSmsCtx *smsCtx = VOS_NULL_PTR;

    smsCtx = AT_GetModemSmsCtxAddrFromClientId(indexNum);

    /* 获取当前短信模式 */
    if (smsCtx->cmgfMsgFormat == AT_CMGF_MSG_FORMAT_TEXT) { /* TEXT */
        return At_SmsTextProc(indexNum, data, len);
    } else {
        /* 如果处理有问题，返回AT_ERROR */
        return At_SmsPduProc(indexNum, data, len); /* PDU */
    }
}


VOS_UINT32 At_ParseSimLockPara(VOS_UINT8 *data, VOS_UINT16 len, AT_ParseParaType *paralist, VOS_UINT32 paraCnt)
{
    VOS_UINT8 *paraStart = VOS_NULL_PTR;
    VOS_UINT32 paraIdx;
    VOS_UINT32 chkRslt;
    VOS_UINT32 i;

    if (paralist == VOS_NULL_PTR) {
        AT_ERR_LOG("At_PareseSimLockPara: pstParalist = VOS_NULL_PTR!");
        return AT_FAILURE;
    }

    paraStart = data;
    paraIdx   = 0;

    /* 从AT^simlock=pucData的pucData开始解析  */
    for (i = 0; i < len; i++) {
        if (data[i] == ',') {
            /* 当前参数的长度 */
            chkRslt = At_CheckAndParsePara((paralist + paraIdx), paraStart, (data + i));
            if (chkRslt != AT_OK) {
                return AT_FAILURE;
            }

            paraStart = (data + i) + 1;
            ++paraIdx;

            if (paraIdx >= paraCnt) {
                return AT_FAILURE;
            }
        }
    }

    /*  将最后一段参数修改copy到参数列表中 */
    chkRslt = At_CheckAndParsePara((paralist + paraIdx), paraStart, (data + len));
    if (chkRslt != AT_OK) {
        return AT_FAILURE;
    }

    ++paraIdx;

    if (paraIdx > paraCnt) {
        return AT_FAILURE;
    }

    return AT_SUCCESS;
}

VOS_UINT32 At_DispatchSimlockCmd(VOS_UINT8 indexNum, VOS_UINT32 paraCnt, AT_ParseParaType *paralist)
{
    VOS_UINT8 simLockOP;

    if ((paraCnt < 1) || (paralist == VOS_NULL_PTR)) {
        At_FormatResultData(indexNum, AT_ERROR);
        return AT_FAILURE;
    }

    if ((paralist[0].paraLen != 1) || (paralist[0].para[0] < '0') || (paralist[0].para[0] > '2')) {
        /* 输出错误 */
        At_FormatResultData(indexNum, AT_ERROR);
        return AT_FAILURE;
    }

    simLockOP = paralist[0].para[0] - '0';

    if (simLockOP == AT_SIMLOCK_OPRT_UNLOCK) {
        At_UnlockSimLock(indexNum, paraCnt, paralist);
    } else if (simLockOP == AT_SIMLOCK_OPRT_SET_PLMN_INFO) {
        At_SetSimLockPlmnInfo(indexNum, paraCnt, paralist);
    } else {
        AT_GetSimLockStatus(indexNum);
    }
    return AT_SUCCESS;
}


VOS_UINT32 At_ProcSimLockPara(VOS_UINT8 indexNum, VOS_UINT8 *data, VOS_UINT16 len)
{
    VOS_INT8          ret;
    VOS_UINT16        cmdLen;
    VOS_UINT8        *dataPara = TAF_NULL_PTR;
    VOS_UINT32        paraCnt;
    AT_ParseParaType *paralist = VOS_NULL_PTR;
    VOS_UINT32        rslt;
    errno_t           memResult;

    if (len == 0) {
        return AT_FAILURE;
    }
    /*lint -save -e516 */
    dataPara = (VOS_UINT8 *)PS_MEM_ALLOC(WUEPS_PID_AT, len);
    /*lint -restore */
    if (dataPara == VOS_NULL_PTR) {
        AT_ERR_LOG("At_ProcSimLockPara: pucDataPara Memory malloc failed!");
        return AT_FAILURE;
    }
    (VOS_VOID)memset_s(dataPara, len, 0x00, len);

    memResult = memcpy_s(dataPara, len, data, len);
    TAF_MEM_CHK_RTN_VAL(memResult, len, len);

    /* 待处理的字符串长度小于等于"AT^SIMLOCK"长度直接返回AT_FAILURE */
    cmdLen = (VOS_UINT16)VOS_StrLen("AT^SIMLOCK=");

    if (len <= cmdLen) {
        (VOS_VOID)memset_s(dataPara, len, 0x00, len);
        /*lint -save -e516 */
        PS_MEM_FREE(WUEPS_PID_AT, dataPara);
        /*lint -restore */
        return AT_FAILURE;
    }

    /* 待处理的字符串头部不是"AT^SIMLOCK"直接返回AT_FAILURE */
    ret = VOS_StrNiCmp((VOS_CHAR *)dataPara, "AT^SIMLOCK=", cmdLen);

    if (ret != 0) {
        (VOS_VOID)memset_s(dataPara, len, 0x00, len);
        /*lint -save -e516 */
        PS_MEM_FREE(WUEPS_PID_AT, dataPara);
        /*lint -restore */
        return AT_FAILURE;
    }

    AT_SaveCmdElementInfo(indexNum, (VOS_UINT8 *)"^SIMLOCK", AT_EXTEND_CMD_TYPE);

    /* 检测参数个数 */
    paraCnt = At_GetParaCnt(dataPara, len);

    if (paraCnt < 1) {
        (VOS_VOID)memset_s(dataPara, len, 0x00, len);
        /*lint -save -e516 */
        PS_MEM_FREE(WUEPS_PID_AT, dataPara);
        /*lint -restore */
        At_FormatResultData(indexNum, AT_ERROR);
        return AT_SUCCESS;
    }

    /* 如果参数是超过61个返回失败 */
    if (paraCnt > AT_SIMLOCK_PLMN_MAX_NUM) {
        (VOS_VOID)memset_s(dataPara, len, 0x00, len);
        /*lint -save -e516 */
        PS_MEM_FREE(WUEPS_PID_AT, dataPara);
        /*lint -restore */
        At_FormatResultData(indexNum, AT_SIMLOCK_PLMN_NUM_ERR);
        return AT_SUCCESS;
    }

    paralist = (AT_ParseParaType *)PS_MEM_ALLOC(WUEPS_PID_AT, (paraCnt * sizeof(AT_ParseParaType)));

    if (paralist == VOS_NULL_PTR) {
        AT_ERR_LOG("At_ProcSimLockPara: pstParalist Memory malloc failed!");
        (VOS_VOID)memset_s(dataPara, len, 0x00, len);
        /*lint -save -e516 */
        PS_MEM_FREE(WUEPS_PID_AT, dataPara);
        /*lint -restore */
        At_FormatResultData(indexNum, AT_ERROR);
        return AT_SUCCESS;
    } else {
        (VOS_VOID)memset_s(paralist, paraCnt * sizeof(AT_ParseParaType), 0x00, (VOS_SIZE_T)(paraCnt * sizeof(AT_ParseParaType)));
    }

    /* 将 At^simlock的参数解析到 At格式的参数列表中 */
    rslt = At_ParseSimLockPara((dataPara + cmdLen), (len - cmdLen), paralist, paraCnt);

    if (rslt == AT_SUCCESS) {
        /* 根据at^simlock=oprt,paralist中的oprt分发 Simlock的命令处理 */
        At_DispatchSimlockCmd(indexNum, paraCnt, paralist);
    } else {
        At_FormatResultData(indexNum, AT_ERROR);
    }

    /*lint -save -e516 */
    (VOS_VOID)memset_s(paralist, paraCnt * sizeof(AT_ParseParaType), 0x00, (VOS_SIZE_T)(paraCnt * sizeof(AT_ParseParaType)));
    (VOS_VOID)memset_s(dataPara, len, 0x00, len);
    PS_MEM_FREE(WUEPS_PID_AT, paralist);
    PS_MEM_FREE(WUEPS_PID_AT, dataPara);
    /*lint -restore */

    return AT_SUCCESS;
}


VOS_UINT32 AT_HandleFacAuthPubKeyExCmd(VOS_UINT8 indexNum, VOS_UINT8 *data, VOS_UINT16 len)
{
    AT_AUTH_PUBKEYEX_CMD_PROC_CTX *authPubKeyCtx = VOS_NULL_PTR;
    VOS_UINT8                     *dataPara      = VOS_NULL_PTR;
    AT_ParseCmdNameType            atCmdName;
    VOS_UINT32                     result;
    VOS_UINT32                     firstParaVal;
    VOS_UINT32                     secParaVal;
    VOS_UINT32                     timerName;
    VOS_UINT32                     tempIndex;
    errno_t                        memResult;
    VOS_UINT16                     cmdlen;
    VOS_UINT16                     pos;
    VOS_UINT16                     loop;
    VOS_UINT16                     commaCnt;
    VOS_UINT16                     firstCommaPos;
    VOS_UINT16                     secCommaPos;
    VOS_UINT16                     firstParaLen;
    VOS_UINT16                     secondParaLen;
    VOS_UINT16                     thirdParaLen;
    VOS_INT8                       ret;

    tempIndex = (VOS_UINT32)indexNum;
    timerName = AT_AUTH_PUBKEY_TIMER;
    timerName |= AT_INTERNAL_PROCESS_TYPE;
    timerName |= (tempIndex << 12);

    authPubKeyCtx = AT_GetAuthPubkeyExCmdCtxAddr();

    (VOS_VOID)memset_s(&atCmdName, sizeof(atCmdName), 0x00, sizeof(atCmdName));

    /* 局部变量初始化 */
    pos           = 0;
    dataPara      = VOS_NULL_PTR;
    loop          = 0;
    commaCnt      = 0;
    firstCommaPos = 0;
    secCommaPos   = 0;
    firstParaLen  = 0;
    secondParaLen = 0;
    firstParaVal  = 0;
    secParaVal    = 0;
    cmdlen        = (VOS_UINT16)VOS_StrLen("AT^FACAUTHPUBKEYEX=");

    if (len < cmdlen) {
        return AT_FAILURE;
    }

    /*lint -save -e516 */
    dataPara = (VOS_UINT8 *)PS_MEM_ALLOC(WUEPS_PID_AT, cmdlen);
    /*lint -restore */
    if (dataPara == VOS_NULL_PTR) {
        AT_ERR_LOG("AT_HandleFacAuthPubKeyExCmd: pucDataPara Memory malloc failed!");
        return AT_FAILURE;
    }
    (VOS_VOID)memset_s(dataPara, cmdlen, 0x00, cmdlen);

    /* 拷贝命令名，供后续比较使用 */
    memResult = memcpy_s(dataPara, cmdlen, data, cmdlen);
    TAF_MEM_CHK_RTN_VAL(memResult, cmdlen, cmdlen);

    /* AT命令头字符转大写 */
    At_UpString(dataPara, cmdlen);

    /* 待处理的字符串头部不是"AT^FACAUTHPUBKEYEX="直接返回AT_FAILURE */
    ret = VOS_StrNiCmp((VOS_CHAR *)dataPara, "AT^FACAUTHPUBKEYEX=", cmdlen);
    if (ret != 0) {
        /*lint -save -e516 */
        PS_MEM_FREE(WUEPS_PID_AT, dataPara);
        /*lint -restore */
        return AT_FAILURE;
    }

    AT_SaveCmdElementInfo(indexNum, (VOS_UINT8 *)"^FACAUTHPUBKEYEX", AT_EXTEND_CMD_TYPE);

    /* 获取命令(不包含命令前缀AT)名称及长度 */
    pos = (VOS_UINT16)VOS_StrLen("AT");

    atCmdName.cmdNameLen = (VOS_UINT16)VOS_StrLen("^FACAUTHPUBKEYEX");
    memResult            = memcpy_s(atCmdName.cmdName, sizeof(atCmdName.cmdName), (data + pos), atCmdName.cmdNameLen);
    TAF_MEM_CHK_RTN_VAL(memResult, sizeof(atCmdName.cmdName), atCmdName.cmdNameLen);
    atCmdName.cmdName[atCmdName.cmdNameLen] = '\0';
    pos += atCmdName.cmdNameLen;

    pos += (VOS_UINT16)VOS_StrLen("=");

    /* 获取命令中的逗号位置和个数 */
    for (loop = pos; loop < len; loop++) {
        if (*(data + loop) == ',') {
            commaCnt++;

            /* 记录下第一个逗号的位置 */
            if (firstCommaPos == 0) {
                firstCommaPos = loop + 1;
            } else {
                if (secCommaPos == 0) {
                    secCommaPos = loop + 1;
                }
            }
        }
    }

    /* 若逗号个数不为2，则AT命令结果返回失败 */
    if (commaCnt != AT_VALID_COMMA_NUM) {
        AT_WARN_LOG("AT_HandleFacAuthPubKeyExCmd: usCommaCnt != 2!");
        /*lint -save -e516 */
        PS_MEM_FREE(WUEPS_PID_AT, dataPara);
        /*lint -restore */
        At_FormatResultData(indexNum, AT_CME_INCORRECT_PARAMETERS);
        AT_ClearAuthPubkeyCtx();
        (VOS_VOID)AT_StopRelTimer(timerName, &(authPubKeyCtx->hAuthPubkeyProtectTimer));
        return AT_SUCCESS;
    }

    /* 计算参数的长度 */
    firstParaLen  = (firstCommaPos - pos) - (VOS_UINT16)VOS_StrLen(",");
    secondParaLen = secCommaPos - firstCommaPos - (VOS_UINT16)VOS_StrLen(",");
    thirdParaLen  = len - secCommaPos;

    /* 获取第一个参数值 */
    if (atAuc2ul(data + pos, firstParaLen, &firstParaVal) == AT_FAILURE) {
        AT_WARN_LOG("AT_HandleFacAuthPubKeyExCmd: ulFirstParaVal value invalid");
        /*lint -save -e516 */
        PS_MEM_FREE(WUEPS_PID_AT, dataPara);
        /*lint -restore */
        At_FormatResultData(indexNum, AT_CME_INCORRECT_PARAMETERS);
        AT_ClearAuthPubkeyCtx();
        (VOS_VOID)AT_StopRelTimer(timerName, &(authPubKeyCtx->hAuthPubkeyProtectTimer));
        return AT_SUCCESS;
    }

    /* 获取第二个参数值 */
    if (atAuc2ul(data + firstCommaPos, secondParaLen, &secParaVal) == AT_FAILURE) {
        AT_WARN_LOG("AT_HandleFacAuthPubKeyExCmd: ulSecParaVal value invalid");
        /*lint -save -e516 */
        PS_MEM_FREE(WUEPS_PID_AT, dataPara);
        /*lint -restore */
        At_FormatResultData(indexNum, AT_CME_INCORRECT_PARAMETERS);
        AT_ClearAuthPubkeyCtx();
        (VOS_VOID)AT_StopRelTimer(timerName, &(authPubKeyCtx->hAuthPubkeyProtectTimer));
        return AT_SUCCESS;
    }

    /* 设置命令类型，操作类型和参数个数 */
    g_atParseCmd.cmdOptType = AT_CMD_OPT_SET_PARA_CMD;
    g_atCmdFmtType          = AT_EXTEND_CMD_TYPE;

    AT_PR_LOGI("enter");

    result = AT_SetFacAuthPubkeyExPara(indexNum, firstParaVal, secParaVal, thirdParaLen, (data + secCommaPos));
    if (result != AT_WAIT_ASYNC_RETURN) {
        AT_PR_LOGI("return OK");

        At_FormatResultData(indexNum, result);
    }

    /*lint -save -e516 */
    PS_MEM_FREE(WUEPS_PID_AT, dataPara);
    /*lint -restore */
    return AT_SUCCESS;
}


VOS_UINT32 AT_HandleSimLockDataWriteCmd(VOS_UINT8 indexNum, VOS_UINT8 *data, VOS_UINT16 len)
{
    errno_t                     memResult;
    VOS_INT8                    ret;
    VOS_UINT16                  cmdlen;
    VOS_UINT16                  pos;
    VOS_UINT8                  *dataPara         = VOS_NULL_PTR;
    AT_SIMLOCKDATAWRITE_SetReq *simlockDataWrite = VOS_NULL_PTR;
    VOS_UINT32                  result;
    AT_ParseCmdNameType         atCmdName;

    (VOS_VOID)memset_s(&atCmdName, sizeof(atCmdName), 0x00, sizeof(atCmdName));

    /* 局部变量初始化 */
    dataPara         = VOS_NULL_PTR;
    simlockDataWrite = VOS_NULL_PTR;

    /* 为提高AT解析性能，在入口处判断命令长度是否为AT^SIMLOCKDATAWRITE设置命令的长度，若不是则直接退出 */
    if ((VOS_StrLen("AT^SIMLOCKDATAWRITE=") + AT_SIMLOCKDATAWRITE_PARA_LEN) != len) {
        return AT_FAILURE;
    }

    cmdlen = (VOS_UINT16)VOS_StrLen("AT^SIMLOCKDATAWRITE=");
    /*lint -save -e516 */
    dataPara = (VOS_UINT8 *)PS_MEM_ALLOC(WUEPS_PID_AT, cmdlen);
    /*lint -restore */
    if (dataPara == VOS_NULL_PTR) {
        AT_ERR_LOG("AT_HandleSimLockDataWriteCmd: pucDataPara Memory malloc failed!");
        return AT_FAILURE;
    }
    (VOS_VOID)memset_s(dataPara, cmdlen, 0x00, cmdlen);

    /* 拷贝命令名，供后续比较使用 */
    memResult = memcpy_s(dataPara, cmdlen, data, cmdlen);
    TAF_MEM_CHK_RTN_VAL(memResult, cmdlen, cmdlen);

    /* AT命令头字符转大写 */
    At_UpString(dataPara, cmdlen);

    /* 待处理的字符串头部不是"AT^SIMLOCKDATAWRITE="直接返回AT_FAILURE */
    ret = VOS_StrNiCmp((VOS_CHAR *)dataPara, "AT^SIMLOCKDATAWRITE=", cmdlen);
    if (ret != 0) {
        /*lint -save -e516 */
        PS_MEM_FREE(WUEPS_PID_AT, dataPara);
        /*lint -restore */
        return AT_FAILURE;
    }

    AT_SaveCmdElementInfo(indexNum, (VOS_UINT8 *)"^SIMLOCKDATAWRITE", AT_EXTEND_CMD_TYPE);

    /* 获取命令(不包含命令前缀AT)名称及长度 */
    pos                  = (VOS_UINT16)VOS_StrLen("AT");
    atCmdName.cmdNameLen = (VOS_UINT16)VOS_StrLen("^SIMLOCKDATAWRITE");
    memResult            = memcpy_s(atCmdName.cmdName, AT_CMD_NAME_LEN + 1, (data + pos), atCmdName.cmdNameLen);
    TAF_MEM_CHK_RTN_VAL(memResult, AT_CMD_NAME_LEN + 1, atCmdName.cmdNameLen);
    atCmdName.cmdName[atCmdName.cmdNameLen] = '\0';
    pos += atCmdName.cmdNameLen;

    pos += (VOS_UINT16)VOS_StrLen("=");

    /* 申请参数解析缓存结构 */
    simlockDataWrite = (AT_SIMLOCKDATAWRITE_SetReq *)PS_MEM_ALLOC(WUEPS_PID_AT, sizeof(AT_SIMLOCKDATAWRITE_SetReq));
    if (simlockDataWrite == VOS_NULL_PTR) {
        /*lint -save -e516 */
        PS_MEM_FREE(WUEPS_PID_AT, dataPara);
        /*lint -restore */
        At_FormatResultData(indexNum, AT_ERROR);
        return AT_SUCCESS;
    }
    (VOS_VOID)memset_s(simlockDataWrite, sizeof(AT_SIMLOCKDATAWRITE_SetReq), 0x00, sizeof(AT_SIMLOCKDATAWRITE_SetReq));

    /* 保存参数 */
    memResult = memcpy_s(simlockDataWrite->categoryData, AT_SIMLOCKDATAWRITE_PARA_LEN, (data + pos),
                         (VOS_SIZE_T)(len - pos));
    TAF_MEM_CHK_RTN_VAL(memResult, AT_SIMLOCKDATAWRITE_PARA_LEN, (VOS_SIZE_T)(len - pos));

    /* 设置命令类型，操作类型和参数个数 */
    g_atParseCmd.cmdOptType = AT_CMD_OPT_SET_PARA_CMD;
    g_atCmdFmtType          = AT_EXTEND_CMD_TYPE;

    AT_PR_LOGI("enter");

    result = AT_SetSimlockDataWritePara(indexNum, simlockDataWrite);
    if (result != AT_WAIT_ASYNC_RETURN) {
        AT_PR_LOGI("return OK");

        At_FormatResultData(indexNum, result);
    }
    /*lint -save -e516 */
    PS_MEM_FREE(WUEPS_PID_AT, dataPara);
    PS_MEM_FREE(WUEPS_PID_AT, simlockDataWrite);
    /*lint -restore */
    return AT_SUCCESS;
}

#if (defined(FEATURE_PHONE_ENG_AT_CMD) && (FEATURE_PHONE_ENG_AT_CMD == FEATURE_ON))

VOS_UINT32 AT_HandleApSndApduCmd(VOS_UINT8 indexNum, VOS_UINT8 *data, VOS_UINT16 len)
{
    VOS_UINT16 cmdlen;
    VOS_UINT16 pos;
    VOS_UINT16 loop;
    VOS_UINT16 commaCnt;
    VOS_UINT16 firstCommaPos;
    VOS_UINT16 firstParaLen;
    VOS_UINT16 secondParaLen;
    VOS_UINT32 lengthValue;
    VOS_UINT32 result;
    errno_t    memResult;
    VOS_UINT8  headChar;
    VOS_UINT8  tailChar;

    /* CISA命令只能从AP侧接收处理，其它端口不可以 */
    if (AT_IsApPort(indexNum) != VOS_TRUE) {
        return AT_FAILURE;
    }

    cmdlen = (VOS_UINT16)VOS_StrLen("AT^CISA=");

    /* 待处理的字符串头部不是"AT^CISA="直接返回AT_FAILURE */
    if (VOS_StrNiCmp((VOS_CHAR *)data, "AT^CISA=", cmdlen) != 0) {
        return AT_FAILURE;
    }

    /* 不支持AT^CISA=?命令 */
    if (VOS_StrNiCmp((VOS_CHAR *)data, "AT^CISA=?", VOS_StrLen("AT^CISA=?")) == 0) {
        At_FormatResultData(indexNum, AT_ERROR);

        return AT_SUCCESS;
    }

    /* 获取命令(不包含命令前缀AT)名称及长度 */
    pos = (VOS_UINT16)VOS_StrLen("AT");

    g_atParseCmd.cmdName.cmdNameLen = (VOS_UINT16)VOS_StrLen("^CISA");

    memResult = memcpy_s(g_atParseCmd.cmdName.cmdName, AT_CMD_NAME_LEN + 1, (data + pos),
                         g_atParseCmd.cmdName.cmdNameLen);
    TAF_MEM_CHK_RTN_VAL(memResult, AT_CMD_NAME_LEN + 1, g_atParseCmd.cmdName.cmdNameLen);

    g_atParseCmd.cmdName.cmdName[g_atParseCmd.cmdName.cmdNameLen] = '\0';

    pos += g_atParseCmd.cmdName.cmdNameLen;

    pos += (VOS_UINT16)VOS_StrLen("=");

    /* 获取命令中的逗号位置和个数 */
    commaCnt      = 0;
    firstCommaPos = 0;
    for (loop = pos; loop < len; loop++) {
        if (*(data + loop) == ',') {
            commaCnt++;

            /* 记录下第一个逗号的位置 */
            if (firstCommaPos == 0) {
                firstCommaPos = loop + 1;
            }
        }
    }

    /* 若逗号个数不为1，则AT命令结果返回失败 */
    if (commaCnt != 1) {
        AT_WARN_LOG("AT_HandleApSndApduCmd: usCommaCnt != 1!");

        At_FormatResultData(indexNum, AT_CME_INCORRECT_PARAMETERS);

        return AT_SUCCESS;
    }

    /* 第二个参数是由引号包含起来的 */
    headChar = *(data + firstCommaPos);
    tailChar = *(data + len - VOS_StrLen("\""));
    if ((headChar != '"') || (tailChar != '"')) {
        AT_WARN_LOG("AT_HandleApSndApduCmd: <command> not a string");

        At_FormatResultData(indexNum, AT_CME_INCORRECT_PARAMETERS);

        return AT_SUCCESS;
    }

    /* 计算两个参数的长度 */
    firstParaLen  = (firstCommaPos - pos) - (VOS_UINT16)VOS_StrLen(",");
    secondParaLen = len - firstCommaPos - (VOS_UINT16)(AT_DOUBLE_LENGTH * VOS_StrLen("\""));

    if (atAuc2ul(data + cmdlen, firstParaLen, &lengthValue) == AT_FAILURE) {
        AT_WARN_LOG("AT_HandleApSndApduCmd: <length> value invalid");

        At_FormatResultData(indexNum, AT_CME_INCORRECT_PARAMETERS);

        return AT_SUCCESS;
    }
    /* 设置命令类型，操作类型和参数个数 */
    g_atParseCmd.cmdOptType = AT_CMD_OPT_SET_PARA_CMD;
    g_atCmdFmtType          = AT_EXTEND_CMD_TYPE;

    /* 在扩展命令表中匹配此命令 */
    result = atMatchCmdName(indexNum, g_atParseCmd.cmdFmtType);

    if (result != ERR_MSP_SUCCESS) {
        At_FormatResultData(indexNum, result);

        return AT_SUCCESS;
    }

    result = AT_SetCISAPara(indexNum, lengthValue, (data + firstCommaPos + VOS_StrLen("\"")), secondParaLen);

    if (result != AT_WAIT_ASYNC_RETURN) {
        At_FormatResultData(indexNum, result);
    }

    return AT_SUCCESS;
}
#endif

#if (FEATURE_UE_MODE_CDMA == FEATURE_ON)

VOS_UINT32 AT_HandleApXsmsSndCmd(VOS_UINT8 indexNum, VOS_UINT8 *data, VOS_UINT16 len)
{
    VOS_UINT16 cmdlen;
    VOS_UINT16 pos;
    VOS_UINT16 loop;
    VOS_UINT16 commaCnt;
    VOS_UINT16 firstCommaPos;
    VOS_UINT16 firstParaLen;
    VOS_UINT16 secondParaLen;
    VOS_UINT32 lengthValue;
    VOS_UINT32 result;
    errno_t    memResult;
    VOS_UINT8  headChar;
    VOS_UINT8  tailChar;

    cmdlen = (VOS_UINT16)VOS_StrLen("AT^CCMGS=");

    /* 待处理的字符串头部不是"AT^CCMGS="直接返回AT_FAILURE */
    if (VOS_StrNiCmp((VOS_CHAR *)data, "AT^CCMGS=", cmdlen) != 0) {
        return AT_FAILURE;
    }

    /* 不支持AT^CCMGS=?命令 */
    if (VOS_StrNiCmp((VOS_CHAR *)data, "AT^CCMGS=?", VOS_StrLen("AT^CCMGS=?")) == 0) {
        At_FormatResultData(indexNum, AT_OK);

        return AT_SUCCESS;
    }

    /* 获取命令(不包含命令前缀AT)名称及长度 */
    pos = (VOS_UINT16)VOS_StrLen("AT");

    g_atParseCmd.cmdName.cmdNameLen = (VOS_UINT16)VOS_StrLen("^CCMGS");

    memResult = memcpy_s(g_atParseCmd.cmdName.cmdName, AT_CMD_NAME_LEN + 1, (data + pos),
                         g_atParseCmd.cmdName.cmdNameLen);
    TAF_MEM_CHK_RTN_VAL(memResult, AT_CMD_NAME_LEN + 1, g_atParseCmd.cmdName.cmdNameLen);

    g_atParseCmd.cmdName.cmdName[g_atParseCmd.cmdName.cmdNameLen] = '\0';

    pos += g_atParseCmd.cmdName.cmdNameLen;

    pos += (VOS_UINT16)VOS_StrLen("=");

    /* 获取命令中的逗号位置和个数 */
    commaCnt      = 0;
    firstCommaPos = 0;
    for (loop = pos; loop < len; loop++) {
        if (*(data + loop) == ',') {
            commaCnt++;

            /* 记录下第一个逗号的位置 */
            if (firstCommaPos == 0) {
                firstCommaPos = loop + 1;
            }
        }
    }

    /* 若逗号个数不为1，则AT命令结果返回失败 */
    if (commaCnt != 1) {
        AT_WARN_LOG("AT_HandleApXsmsSndCmd: usCommaCnt != 1!");

        At_FormatResultData(indexNum, AT_CME_INCORRECT_PARAMETERS);

        return AT_SUCCESS;
    }

    /* 第二个参数是由引号包含起来的 */
    headChar = *(data + firstCommaPos);
    tailChar = *(data + len - VOS_StrLen("\""));
    if ((headChar != '"') || (tailChar != '"')) {
        AT_WARN_LOG("AT_HandleApXsmsSndCmd: <PDU> not a string");

        At_FormatResultData(indexNum, AT_CME_INCORRECT_PARAMETERS);

        return AT_SUCCESS;
    }

    /* 计算两个参数的长度 */
    firstParaLen  = (firstCommaPos - pos) - (VOS_UINT16)VOS_StrLen(",");
    secondParaLen = len - firstCommaPos - (VOS_UINT16)(AT_DOUBLE_LENGTH * VOS_StrLen("\""));

    if (atAuc2ul(data + cmdlen, firstParaLen, &lengthValue) == AT_FAILURE) {
        AT_WARN_LOG("AT_HandleApXsmsSndCmd: <length> value invalid");

        At_FormatResultData(indexNum, AT_CME_INCORRECT_PARAMETERS);

        return AT_SUCCESS;
    }
    /* 设置命令类型，操作类型和参数个数 */
    g_atParseCmd.cmdOptType = AT_CMD_OPT_SET_PARA_CMD;
    g_atCmdFmtType          = AT_EXTEND_CMD_TYPE;

    /* 在扩展命令表中匹配此命令 */
    result = atMatchCmdName(indexNum, g_atParseCmd.cmdFmtType);

    if (result != ERR_MSP_SUCCESS) {
        At_FormatResultData(indexNum, result);

        return AT_SUCCESS;
    }

    result = AT_SetCcmgsPara(indexNum, lengthValue, (data + firstCommaPos + VOS_StrLen("\"")), secondParaLen);

    if (result != AT_WAIT_ASYNC_RETURN) {
        At_FormatResultData(indexNum, result);
    }

    return AT_SUCCESS;
}


VOS_UINT32 AT_HandleApXsmsWriteCmd(VOS_UINT8 indexNum, VOS_UINT8 *data, VOS_UINT16 len)
{
    VOS_UINT16 cmdlen;
    VOS_UINT16 pos;
    VOS_UINT16 loop;
    VOS_UINT16 commaCnt;
    VOS_UINT16 firstCommaPos;
    VOS_UINT16 secondCommaPos;
    VOS_UINT16 firstParaLen;
    VOS_UINT16 secondParaLen;
    VOS_UINT16 thirdParaLen;
    VOS_UINT32 lengthValue;
    VOS_UINT32 statValue;
    VOS_UINT32 result;
    errno_t    memResult;
    VOS_UINT8  headChar;
    VOS_UINT8  tailChar;

    cmdlen = (VOS_UINT16)VOS_StrLen("AT^CCMGW=");

    /* 待处理的字符串头部不是"AT^CCMGW="直接返回AT_FAILURE */
    if (VOS_StrNiCmp((VOS_CHAR *)data, "AT^CCMGW=", cmdlen) != 0) {
        return AT_FAILURE;
    }

    /* 不支持AT^CCMGS=?命令 */
    if (VOS_StrNiCmp((VOS_CHAR *)data, "AT^CCMGW=?", VOS_StrLen("AT^CCMGW=?")) == 0) {
        At_FormatResultData(indexNum, AT_OK);

        return AT_SUCCESS;
    }

    /* 获取命令(不包含命令前缀AT)名称及长度 */
    pos = (VOS_UINT16)VOS_StrLen("AT");

    g_atParseCmd.cmdName.cmdNameLen = (VOS_UINT16)VOS_StrLen("^CCMGW");

    memResult = memcpy_s(g_atParseCmd.cmdName.cmdName, AT_CMD_NAME_LEN + 1, (data + pos),
                         g_atParseCmd.cmdName.cmdNameLen);
    TAF_MEM_CHK_RTN_VAL(memResult, AT_CMD_NAME_LEN + 1, g_atParseCmd.cmdName.cmdNameLen);

    g_atParseCmd.cmdName.cmdName[g_atParseCmd.cmdName.cmdNameLen] = '\0';

    pos += g_atParseCmd.cmdName.cmdNameLen;

    pos += (VOS_UINT16)VOS_StrLen("=");

    /* 获取命令中的逗号位置和个数 */
    commaCnt       = 0;
    firstCommaPos  = 0;
    secondCommaPos = 0;

    for (loop = pos; loop < len; loop++) {
        if (*(data + loop) == ',') {
            commaCnt++;

            /* 记录下第一个逗号的位置 */
            if (firstCommaPos == 0) {
                firstCommaPos = loop + 1;
            }

            /* 记录下第二个逗号的位置 */
            if ((secondCommaPos == 0) && (commaCnt == AT_VALID_COMMA_NUM)) {
                secondCommaPos = loop + 1;
            }
        }
    }

    /* 若逗号个数不为2，则AT命令结果返回失败 */
    if (commaCnt != AT_VALID_COMMA_NUM) {
        AT_WARN_LOG("AT_HandleApXsmsWriteCmd: usCommaCnt != 1!");

        At_FormatResultData(indexNum, AT_CME_INCORRECT_PARAMETERS);

        return AT_SUCCESS;
    }

    /* 第三个参数是由引号包含起来的 */
    headChar = *(data + secondCommaPos);
    tailChar = *(data + len - VOS_StrLen("\""));
    if ((headChar != '"') || (tailChar != '"')) {
        AT_WARN_LOG("AT_HandleApXsmsWriteCmd: <PDU> not a string");

        At_FormatResultData(indexNum, AT_CME_INCORRECT_PARAMETERS);

        return AT_SUCCESS;
    }

    /* 计算三个参数的长度 */
    firstParaLen  = (firstCommaPos - pos) - (VOS_UINT16)VOS_StrLen(",");
    secondParaLen = secondCommaPos - firstCommaPos - (VOS_UINT16)VOS_StrLen(",");
    thirdParaLen  = len - secondCommaPos - (VOS_UINT16)(AT_DOUBLE_LENGTH * VOS_StrLen("\""));

    /* 解析参数1<len>的值 */
    if (atAuc2ul(data + cmdlen, firstParaLen, &lengthValue) == AT_FAILURE) {
        AT_WARN_LOG("AT_HandleApXsmsSndCmd: <length> value invalid");

        At_FormatResultData(indexNum, AT_CME_INCORRECT_PARAMETERS);

        return AT_SUCCESS;
    }

    /* 解析参数2<stat>的值 */
    if (atAuc2ul(data + firstCommaPos, secondParaLen, &statValue) == AT_FAILURE) {
        AT_WARN_LOG("AT_HandleApXsmsSndCmd: <length> value invalid");

        At_FormatResultData(indexNum, AT_CME_INCORRECT_PARAMETERS);

        return AT_SUCCESS;
    }

    /* 设置命令类型，操作类型和参数个数 */
    g_atParseCmd.cmdOptType = AT_CMD_OPT_SET_PARA_CMD;
    g_atCmdFmtType          = AT_EXTEND_CMD_TYPE;

    /* 在扩展命令表中匹配此命令 */
    result = atMatchCmdName(indexNum, g_atParseCmd.cmdFmtType);

    if (result != ERR_MSP_SUCCESS) {
        At_FormatResultData(indexNum, result);

        return AT_SUCCESS;
    }

    result = AT_SetCcmgwPara(indexNum, lengthValue, statValue, (data + secondCommaPos + VOS_StrLen("\"")),
                             thirdParaLen);

    if (result != AT_WAIT_ASYNC_RETURN) {
        At_FormatResultData(indexNum, result);
    }

    return AT_SUCCESS;
}
#endif

VOS_UINT32 At_HandleApModemSpecialCmd(VOS_UINT8 indexNum, VOS_UINT8 *data, VOS_UINT16 len)
{
    VOS_UINT8 *systemAppConfig = VOS_NULL_PTR;

    /* 获取当前产品形态 */
    systemAppConfig = AT_GetSystemAppConfigAddr();

    if ((*systemAppConfig != SYSTEM_APP_ANDROID)
#if (FEATURE_PHONE_SC == FEATURE_ON)
        && (*systemAppConfig != SYSTEM_APP_MP)
#endif
        ) {
        return AT_FAILURE;
    }

    /* 处理AT^FACAUTHPUBKEYEX=<index>,<total>,<pubkey>设置命令(参数<pubkey>超长) */
    if (AT_HandleFacAuthPubKeyExCmd(indexNum, data, len) == AT_SUCCESS) {
        return AT_SUCCESS;
    }

    /* 处理AT^IDENTIFYEND=<Rsa>设置命令(参数<Rsa>超长) */
    if (AT_HandleApModemSpecialCmdProc(indexNum, data, len, "^IDENTIFYEND") == AT_SUCCESS) {
        return AT_SUCCESS;
    }

    /* 处理AT^IDENTIFYOTAEND=<OtaRsa>设置命令(参数<OtaRsa>超长) */
    if (AT_HandleApModemSpecialCmdProc(indexNum, data, len, "^IDENTIFYOTAEND") == AT_SUCCESS) {
        return AT_SUCCESS;
    }

    /* 处理AT^PHONEPHYNUM=<type>,<PhynumRsa>,<Hmac>设置命令(参数<PhynumRsa>超长) */
    if (AT_HandleApModemSpecialCmdProc(indexNum, data, len, "^PHONEPHYNUM") == AT_SUCCESS) {
        return AT_SUCCESS;
    }

    /* 处理AT^SIMLOCKDATAWRITEEX=<layer>,<total>,<index>,<simlock_data>,[hmac]设置命令(参数<simlock_data>超长) */
    if (AT_HandleSimLockDataWriteExCmd(indexNum, data, len) == AT_SUCCESS) {
        return AT_SUCCESS;
    }

    /* 处理AT^SIMLOCKOTADATAWRITE=<layer>,<total>,<index>,<simlock_data>,[hmac]设置命令(参数<simlock_data>超长) */
    if (AT_HandleSimLockOtaDataWriteCmd(indexNum, data, len) == AT_SUCCESS) {
        return AT_SUCCESS;
    }

#if (FEATURE_SC_NETWORK_UPDATE == FEATURE_ON)
    /* 处理AT^SIMLOCKNWDATAWRITE=<layer>,<total>,<index>,<simlock_data>,[hmac]设置命令(参数<simlock_data>超长) */
    if (AT_HandleSimLockNWDataWriteCmd(indexNum, data, len) == AT_SUCCESS) {
        return AT_SUCCESS;
    }
#endif

    /* 处理AT^SIMLOCKDATAWRITE=<simlock_data_write>设置命令(参数<simlock_data_write>超长) */
    if (AT_HandleSimLockDataWriteCmd(indexNum, data, len) == AT_SUCCESS) {
        return AT_SUCCESS;
    }

#if (defined(FEATURE_PHONE_ENG_AT_CMD) && (FEATURE_PHONE_ENG_AT_CMD == FEATURE_ON))
    /* 处理AT^CISA=<length>,<command> */
    if (AT_HandleApSndApduCmd(indexNum, data, len) == AT_SUCCESS) {
        return AT_SUCCESS;
    }
#endif

#if (FEATURE_UE_MODE_CDMA == FEATURE_ON)
    if (AT_HandleApXsmsSndCmd(indexNum, data, len) == AT_SUCCESS) {
        return AT_SUCCESS;
    }

    if (AT_HandleApXsmsWriteCmd(indexNum, data, len) == AT_SUCCESS) {
        return AT_SUCCESS;
    }
#endif

    /* 删除对^EOPLMN设置命令的特殊解析 */

    return AT_FAILURE;
}


TAF_UINT32 At_CheckUsimStatus(TAF_UINT8 *cmdName, VOS_UINT8 indexNum)
{
    TAF_UINT32       rst;
    ModemIdUint16    modemId;
    VOS_UINT32       getModemIdRslt;
    AT_USIM_InfoCtx *usimInfoCtx = VOS_NULL_PTR;
    AT_ModemSmsCtx  *smsCtx      = VOS_NULL_PTR;

    NAS_NVIM_FollowonOpenspeedFlag quickStartFlg;

    quickStartFlg.quickStartSta = AT_QUICK_START_DISABLE;
    modemId                     = MODEM_ID_0;

    getModemIdRslt = AT_GetModemIdFromClient(indexNum, &modemId);

    if (getModemIdRslt != VOS_OK) {
        AT_ERR_LOG("At_CheckUsimStatus:Get Modem Id fail!");
        return AT_ERROR;
    }

    smsCtx      = AT_GetModemSmsCtxAddrFromModemId(modemId);
    usimInfoCtx = AT_GetUsimInfoCtxFromModemId(modemId);


    /* 快速开机模式，不检查 PIN 状态 */
    if (TAF_ACORE_NV_READ(modemId, NV_ITEM_FOLLOW_ON_OPEN_SPEED_FLAG, (VOS_UINT8 *)(&quickStartFlg),
                          sizeof(NAS_NVIM_FollowonOpenspeedFlag)) != NV_OK) {
        quickStartFlg.quickStartSta = AT_QUICK_START_DISABLE;
    }

    if (quickStartFlg.quickStartSta == AT_QUICK_START_ENABLE) {
        return AT_SUCCESS;
    }

    /* SMS相关命令掉卡流程需要在业务模块内部处理，此处仅处理CMGL，其他命令的处理待确认 */
    if (VOS_StrCmp((TAF_CHAR *)cmdName, "+CMGL") == 0) {
        if (smsCtx->cpmsInfo.usimStorage.totalRec != 0) {
            return AT_SUCCESS;
        }
    }

    switch (usimInfoCtx->cardStatus) {
        case USIMM_CARDAPP_SERVIC_AVAILABLE:
            rst = AT_SUCCESS;
            break;
        case USIMM_CARDAPP_SERVIC_SIM_PIN:
            rst = AT_CME_SIM_PIN_REQUIRED;
            break;
        case USIMM_CARDAPP_SERVIC_SIM_PUK:
            rst = AT_CME_SIM_PUK_REQUIRED;
            break;
        case USIMM_CARDAPP_SERVIC_UNAVAILABLE:
        case USIMM_CARDAPP_SERVIC_NET_LCOK:
        case USIMM_CARDAPP_SERVICE_IMSI_LOCK:
            rst = AT_CME_SIM_FAILURE;
            break;
        case USIMM_CARDAPP_SERVIC_ABSENT:
            rst = AT_CME_SIM_NOT_INSERTED;
            break;

        case USIMM_CARDAPP_SERVIC_BUTT:

            rst = AT_CME_SIM_BUSY;
            break;

        default:
            rst = AT_ERROR;
            break;
    }

    return rst;
}


VOS_VOID At_SetCmdSubMode(AT_ClientIdUint16 clientId, AT_CmdSubModeUint8 subMode)
{
    DMS_PortIdUint16        portId;
    DMS_PortModeUint8       mode;

    portId  = g_atClientTab[clientId].portNo;
    mode    = DMS_PORT_GetMode(portId);

    if  (mode != DMS_PORT_MODE_CMD &&
         mode != DMS_PORT_MODE_ONLINE_CMD) {
        AT_ERR_LOG1("At_SetCmdSubMode mode abnormal", mode);
        return;
    }

    g_parseContext[clientId].mode = subMode;
}


AT_CmdSubModeUint8 At_GetCmdSubMode(AT_ClientIdUint16 clientId)
{
    return g_parseContext[clientId].mode;
}


TAF_UINT32 AT_CountDigit(TAF_UINT8 *data, TAF_UINT32 len, TAF_UINT8 Char, TAF_UINT32 indexNum)
{
    TAF_UINT8 *tmp    = TAF_NULL_PTR;
    TAF_UINT8 *check  = TAF_NULL_PTR;
    TAF_UINT32 count  = 0;
    TAF_UINT32 chkLen = 0;
    TAF_UINT32 tmpLen = 0;

    tmp = data;

    /* 检查输入参数 */
    if ((Char == *tmp) || (Char == *((tmp + len) - 1))) {
        return 0;
    }

    check = data;
    while (tmpLen++ < len) {
        /* 判断是否是数字或者是特定字符 */
        if (((*check >= '0') && (*check <= '9')) || ((*check == Char) && (*(check + 1) != Char))) {
            check++;
        } else {
            return 0;
        }
    }

    while (chkLen++ < len) {
        if (Char == *tmp) {
            count++;
            if (count == indexNum) {
                return chkLen;
            }
        }
        tmp++;
    }

    return 0;
}

VOS_VOID At_InterTimerOutProc(VOS_UINT8 indexNum)
{
    AT_UartCtx          *uartCtx = VOS_NULL_PTR;
    AT_RreturnCodeUint32 result;

    uartCtx = AT_GetUartCtxAddr();
    result  = AT_FAILURE;

    /* 内部函数，不需要对输入参数进行检查 */
    switch (g_atClientTab[indexNum].cmdCurrentOpt) {
        case AT_CMD_COPS_SET_AUTOMATIC:
        case AT_CMD_COPS_SET_MANUAL:
        case AT_CMD_COPS_SET_DEREGISTER:
        case AT_CMD_COPS_SET_MANUAL_AUTOMATIC_MANUAL:
            result = AT_CME_NETWORK_TIMEOUT;
            break;

        case AT_CMD_CUSD_REQ:
            result = AT_CME_NETWORK_TIMEOUT;
            break;

        case AT_CMD_PPP_ORG_SET:
            AT_SendRelPppReq(g_atClientTab[indexNum].pppId, PPP_AT_CTRL_REL_PPP_REQ);
            /* 返回命令模式 */
            DMS_PORT_ResumeCmdMode(g_atClientTab[indexNum].portNo);

            /* 送NO CARRIER消息 */
            result = AT_NO_CARRIER;
            break;

        case AT_CMD_D_IP_CALL_SET:
            AT_SendRelPppReq(g_atClientTab[indexNum].pppId, PPP_AT_CTRL_REL_PPP_RAW_REQ);
            result = AT_ERROR;
            break;

        case AT_CMD_H_PS_SET:
        case AT_CMD_PS_DATA_CALL_END_SET:

            /* 返回命令模式 */
            DMS_PORT_ResumeCmdMode(g_atClientTab[indexNum].portNo);
            result = AT_ERROR;
            break;

#if (FEATURE_LTE == FEATURE_ON)
        case AT_CMD_SET_TMODE:
            result = AT_ERROR;
            break;
#endif

        case AT_CMD_COPS_ABORT_PLMN_LIST:
            result = AT_ABORT;
            break;

        case AT_CMD_NETSCAN_ABORT:
            result = AT_ABORT;
            break;

        case AT_CMD_CMGS_TEXT_SET:
        case AT_CMD_CMGS_PDU_SET:
        case AT_CMD_CMGC_TEXT_SET:
        case AT_CMD_CMGC_PDU_SET:
        case AT_CMD_CMSS_SET:
        case AT_CMD_CMST_SET:
            result = AT_CMS_UNKNOWN_ERROR;
            break;

        case AT_CMD_CPBR2_SET:
        case AT_CMD_CPBR_SET:
            if (AT_CheckHsUartUser(indexNum) == VOS_TRUE) {
                uartCtx->wmLowFunc = VOS_NULL_PTR;
            }

            result = AT_ERROR;
            break;

        case AT_CMD_VMSET_SET:
            AT_InitVmSetCtx();
            result = AT_ERROR;
            break;

        default:
            result = AT_ERROR;
            break;
    }

    g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_CURRENT_OPT_BUTT;
    g_parseContext[indexNum].clientStatus = AT_FW_CLIENT_STATUS_READY;
#if (FEATURE_IOT_ATNLPROXY == FEATURE_ON)
    /* usMagic 小于 1024即可 */
    g_atClientTab[indexNum].magic = 0;
    g_atClientTab[indexNum].canAbort = 0;
    if (indexNum == g_current_user_at) {
        g_atClientTab[AT_CLIENT_ID_APP].isWaitAts = 0;
        g_current_user_at = VOS_NULL_PARA;
    }
#endif
    At_FormatResultData(indexNum, result);
    return;
}


VOS_UINT32 AT_DockHandleCmd(VOS_UINT8 indexNum, VOS_UINT8 *data, VOS_UINT16 len)
{
    errno_t    memResult;
    VOS_UINT16 length = 0;
    VOS_UINT8  cmdTmp[AT_CMDTMP_MAX_LEN + 1] = {0};
    VOS_UINT8  i;

    if (len > AT_CMDTMP_MAX_LEN) {
        return AT_FAILURE;
    }

    /* 将该AT命令从应用转发给E5 */
    for (i = 0; i < AT_MAX_CLIENT_NUM; i++) {
        if (g_atClientTab[i].userType == AT_APP_USER) {
            (VOS_VOID)memset_s(cmdTmp, sizeof(cmdTmp), 0x00, sizeof(cmdTmp));
            if (len > 0) {
                memResult = memcpy_s(cmdTmp, sizeof(cmdTmp), data, len);
                TAF_MEM_CHK_RTN_VAL(memResult, sizeof(cmdTmp), len);
            }

            length += (VOS_UINT16)AT_FormatReportString(AT_CMD_MAX_LEN, (VOS_CHAR *)g_atSndCodeAddress,
                (VOS_CHAR *)g_atSndCodeAddress + length, "%s%s%s", g_atCrLf, cmdTmp, g_atCrLf);
            At_SendResultData(i, g_atSndCodeAddress, length);
            return AT_SUCCESS;
        }
    }

    return AT_FAILURE;
}

/*
 * Description: 命令名匹配
 * History:
 *  1.Date: 2005-04-19
 *    Author:
 *    Modification: Created function
 */
TAF_UINT32 At_MatchCmdName(TAF_UINT16 indexNum, TAF_UINT32 cmdType)
{
    /*   */
    return AT_FAILURE;
}


VOS_UINT32 AT_HandleDockSetCmd(VOS_UINT8 indexNum, VOS_UINT8 *data, VOS_UINT16 len)
{
    VOS_UINT32 result;

    result = AT_ParseSetDockCmd(data, len);
    if (result != AT_SUCCESS) {
        return AT_FAILURE;
    }

    if (At_MatchCmdName(indexNum, g_atCmdFmtType) == AT_FAILURE) {
        At_FormatResultData(indexNum, AT_CMD_NOT_SUPPORT);
        return AT_SUCCESS;
    }

    AT_SaveCmdElementInfo(indexNum, (VOS_UINT8 *)"^DOCK", AT_EXTEND_CMD_TYPE);

    result = At_SetParaCmd(indexNum);

    At_FormatResultData(indexNum, result);

    return AT_SUCCESS;
}


VOS_VOID AT_RcvTiS0Expired(REL_TimerMsgBlock *msg)
{
    AT_ModemCcCtx    *ccCtx = VOS_NULL_PTR;
    MN_CALL_SupsParam callMgmtParam;
    TAF_Ctrl          ctrl;
    VOS_UINT8         clientIndex;
    VOS_UINT16        reqClientId;
    ModemIdUint16     modemId;

    /* 根据timer name获取属于哪个index */
    clientIndex = (VOS_UINT8)(msg->name >> 12);

    ccCtx = AT_GetModemCcCtxAddrFromClientId(clientIndex);

    (VOS_VOID)memset_s(&callMgmtParam, sizeof(callMgmtParam), 0x00, sizeof(callMgmtParam));

    /* 目前只有voice支持自动接听功能，calltype 固定填为voice */
    callMgmtParam.callType    = MN_CALL_TYPE_VOICE;
    callMgmtParam.callSupsCmd = MN_CALL_SUPS_CMD_HOLD_ACT_ACPT_OTH;

    callMgmtParam.callId = (MN_CALL_ID_T)msg->para;

    /*
     * 来电ClientId为广播ClientId 0x3fff或者0x7fff，
     * AT在处理incoming事件时，判断已启动自动接听，启动定时器时ulName中带的是Client Index即0x3f或者0x7f
     * 所以此处调用MN_CALL_Sups时需要填ClientId
     */
    if (clientIndex == AT_BROADCAST_CLIENT_INDEX_MODEM_0) {
        reqClientId = AT_BROADCAST_CLIENT_ID_MODEM_0;
    } else if (clientIndex == AT_BROADCAST_CLIENT_INDEX_MODEM_1) {
        reqClientId = AT_BROADCAST_CLIENT_ID_MODEM_1;
    } else {
        reqClientId = AT_BROADCAST_CLIENT_ID_MODEM_2;
    }

    (VOS_VOID)memset_s(&ctrl, sizeof(ctrl), 0x00, sizeof(ctrl));

    ctrl.moduleId = WUEPS_PID_AT;
    ctrl.clientId = reqClientId;
    ctrl.opId     = 0;

    if (AT_GetModemIdFromClient(reqClientId, &modemId) != VOS_OK) {
        return;
    }

    /* AT向CCM发送补充业务请求 */
    if (TAF_CCM_CallCommonReq(&ctrl, &callMgmtParam, ID_TAF_CCM_CALL_SUPS_CMD_REQ, sizeof(callMgmtParam), modemId) !=
        VOS_OK) {
        AT_WARN_LOG("AT_ProcTimerS0: S0 Answer Fail");
    }
    AT_PR_LOGH("AT_RcvTiS0Expired: CallId = %d", callMgmtParam.callId);
    ccCtx->s0TimeInfo.timerStart = TAF_FALSE;
    ccCtx->s0TimeInfo.timerName  = 0;

    return;
}


VOS_VOID AT_RcvTiShutDownExpired(REL_TimerMsgBlock *tmrMsg)
{
#if (VOS_WIN32 == VOS_OS_VER || OSA_CPU_CCPU == VOS_OSA_CPU)
    AT_SysbootShutdown((sysboot_shutdown_reason_e)0);
#else
    AT_SysbootShutdown();
#endif
    return;
}


AT_PS_INTERNAL_TIMEOUT_FUNC AT_GetInternalTimeOutResultFunc(VOS_UINT32 interTimerName)
{
    const AT_PS_InternalTimeoutResult  *interTimeoutFuncTblPtr = VOS_NULL_PTR;
    AT_PS_INTERNAL_TIMEOUT_FUNC         interTimeoutFunc       = VOS_NULL_PTR;
    VOS_UINT32                          cnt;

    interTimeoutFuncTblPtr = AT_GET_INTERNAL_TIMEOUT_RSLT_FUNC_TBL_PTR();

    for (cnt = 0; cnt < AT_GET_INTERNAL_TIMEOUT_RSLT_FUNC_TBL_SIZE(); cnt++) {
        if (interTimerName == interTimeoutFuncTblPtr[cnt].interTimerName) {
            interTimeoutFunc = interTimeoutFuncTblPtr[cnt].internalTimeoutFunc;
            break;
        }
    }

    return interTimeoutFunc;
}


TAF_VOID At_TimeOutProc(REL_TimerMsgBlock *msg)
{
    /* 局部变量声明及初始化 */
    AT_PS_INTERNAL_TIMEOUT_FUNC interTimeoutFunc = VOS_NULL_PTR;
    VOS_UINT8                   indexNum;

    if (msg == VOS_NULL_PTR) {
        AT_WARN_LOG("At_TimeOutProc pMsg is NULL");
        return;
    }

    indexNum = (VOS_UINT8)msg->name;
    if ((msg->name & 0x00000f00) == AT_INTERNAL_PROCESS_TYPE) {
        interTimeoutFunc = AT_GetInternalTimeOutResultFunc(msg->name & 0x000000ff);

        /* 如果处理函数存在则调用 */
        if (interTimeoutFunc != VOS_NULL_PTR) {
            interTimeoutFunc(msg);
        } else {
            AT_ERR_LOG1("At_TimeOutProc: not find timeout func, Msg is ", msg->name);
        }
        return;
    } else {
        indexNum = (VOS_UINT8)msg->name;
        if (indexNum >= AT_MAX_CLIENT_NUM) {
            AT_WARN_LOG("At_TimeOutProc FAILURE");
            return;
        }

        if (g_atClientTab[indexNum].cmdCurrentOpt == AT_CMD_CURRENT_OPT_BUTT) {
            AT_WARN_LOG("At_TimeOutProc AT_CMD_CURRENT_OPT_BUTT");
            return;
        }

        AT_LOG1("At_TimeOutProc ucIndex:", indexNum);
        AT_LOG1("At_TimeOutProc g_atClientTab[ucIndex].CmdCurrentOpt:", g_atClientTab[indexNum].cmdCurrentOpt);

        At_InterTimerOutProc(indexNum);
    }
}
/*
 * History:
 *  1.Date: 2005-04-19
 *    Author:
 *    Modification: Created function
 */
TAF_UINT8* At_GetStrContent(TAF_UINT32 type)
{
    if (type < AT_STRING_BUTT) {
        return g_atStringTab[type].text;
    } else {
        return g_atStringTab[AT_STRING_BUTT].text;
    }
}

/* PC工程中AT从A核移到C核, At_sprintf有重复定义,故在此处添加条件编译宏 */


/*lint -e713 -e507 -e530*/
VOS_INT32 AT_FormatReportString(VOS_INT32 maxLength, VOS_CHAR *headAddr, VOS_CHAR *currAddr, const VOS_CHAR *fmt, ...)
{
    VOS_INT32  length   = 0;
    VOS_UINT32 num      = 0;
    VOS_INT32  numSrc   = 0; /* 原始数据 */
    VOS_INT32  numTemp  = 0; /* 转换后的数据 */
    VOS_UINT32 signFlag = VOS_FALSE;
    VOS_CHAR  *data     = NULL;
    VOS_CHAR  *sc       = NULL;
    VOS_CHAR  *s        = NULL;
    va_list    args;
    VOS_UINT32 len1 = 0; /* ulLen1:%后设置的长度; */
    VOS_UINT32 len2 = 0; /* ulLen2:实际读取的长度 */

    if ((headAddr == VOS_NULL_PTR) || (currAddr == VOS_NULL_PTR)) {
        AT_ERR_LOG("AT_FormatReportString: Parameter of the function is null.");
        return 0;
    }

    if (currAddr < headAddr) {
        return 0;
    }

    va_start(args, fmt);
    for (data = (VOS_CHAR *)fmt; *data != '\0'; ++data) {
        if (*data == '%') {
            len1 = 0;
            len2 = 0;

            data++;

            if ((*data) == '+' && *(data + 1) == 'd') {
                data++;
                signFlag = VOS_TRUE;
                len1     = 0; /* 有符号整型打印符号 */
            }

            if ((*data) == '0') {
                data++;
                len1 = 0; /* 指定的宽度，或填充 */
            }

            /* 目前，只支持小于等于9位的宽度 */
            if (((*data) >= '0') && ((*data) <= '9')) {
                len1 = (VOS_UINT32)(VOS_INT32)((*data) - '0'); /* 指定的宽度 */
                data++;
            }

            /* 目前，只会用到c,d,s,X,四种，其它一律认为出错 */
            switch (*data) {
                case 'c': {
                    length++;
                    break;
                }

                case 's':
                    s = va_arg(args, VOS_CHAR *);
                    if (s == NULL) {
                        length += (VOS_INT32)strlen("(null)");
                        break;
                    }

                    for (sc = s; *sc != '\0'; ++sc) {
                        length++;
                    }
                    break;

                case 'd':
                    numSrc  = va_arg(args, VOS_INT32);
                    numTemp = numSrc;

                    length++;
                    len2++; /* 实际的宽度 */

                    /* 有符号数十进制输出，当前数字大于9或小于-9时，需要通过除10计算数字宽度 */
                    while ((numTemp > AT_DEC_MAX_NUM) || (numTemp < AT_DEC_SIGNED_MIN_NUM)) {
                        numTemp = numTemp / 10;
                        length++;
                        len2++; /* 实际的宽度 */
                    }

                    if (len2 < len1) {
                        length += (VOS_INT32)(len1 - len2);
                    }
                    /* 如果ulLen2 >= ulLen1,且lNumSrc<0，则要加上负号的一个空间 */
                    else if (numSrc < 0 || signFlag == VOS_TRUE) {
                        length++;
                        signFlag = VOS_FALSE;
                    } else {
                    }
                    break;

                case 'u':
                    num = va_arg(args, VOS_UINT32);

                    length++;
                    len2++; /* 实际的宽度 */
                    /* 无符号数十进制输出，当前数字大于9时，需要通过除10计算数字宽度 */
                    while (num > AT_DEC_MAX_NUM) {
                        num = num / 10;
                        length++;
                        len2++; /* 实际的宽度 */
                    }

                    if (len2 < len1) {
                        length += (VOS_INT32)(len1 - len2);
                    }
                    break;

                case 'X':
                    /* continue */
                case 'x':
                    num = va_arg(args, VOS_UINT32);

                    length++;
                    len2++; /* 实际的宽度 */

                    while (num > 0x0f) {
                        /* 十六进制输出，当前数字大于0x0f时，需要通过除16计算数字宽度 */
                        num = num / 16;
                        length++;
                        len2++; /* 实际的宽度 */
                    }

                    if (len2 < len1) {
                        length += (VOS_INT32)(len1 - len2);
                    }
                    break;

                default:
                    va_end(args);
                    return 0;
            }
        } else {
            length++; /* 加1 */
        }
    }
    va_end(args);

    if (currAddr - headAddr + length >= (VOS_INT32)(maxLength)) {
        return 0;
    }

    va_start(args, fmt);

    if (length != vsnprintf_s(currAddr, (VOS_SIZE_T)(maxLength - (currAddr - headAddr)),
                              (VOS_SIZE_T)(maxLength - (currAddr - headAddr) - 1), fmt, args)) {
        va_end(args);
        return 0;
    }

    va_end(args);

    return length;
}

/*lint +e713 +e507 +e530*/


TAF_VOID AT_MapCallTypeModeToClccMode(MN_CALL_TypeUint8 callType, VOS_UINT8 *clccMode)
{
    switch (callType) {
        case MN_CALL_TYPE_VOICE:
        case MN_CALL_TYPE_EMERGENCY:
        case MN_CALL_TYPE_MIEC:
        case MN_CALL_TYPE_AIEC:
        case MN_CALL_TYPE_TEST:
        case MN_CALL_TYPE_RECFGURATION:
        case MN_CALL_TYPE_PSAP_ECALL:
            *clccMode = AT_CLCC_PARA_MODE_VOICE;
            break;
        case MN_CALL_TYPE_FAX:
            *clccMode = AT_CLCC_PARA_MODE_FAX;
            break;
        default:
            *clccMode = AT_CLCC_PARA_MODE_DATA;
            break;
    }
}


AT_CmdCurrentOpt At_GetMnOptType(MN_MMI_OperationTypeUint8 type)
{
    switch (type) {
        case TAF_MMI_CALL_ORIG: /* TAF_MMI_CALL_ORIG */
            return AT_CMD_D_CS_VOICE_CALL_SET;
        case TAF_MMI_CALL_CHLD_REQ: /* TAF_MMI_CALL_CHLD_REQ */
            return AT_CMD_CHLD_SET;
        case TAF_MMI_CHANGE_PIN: /* TAF_MMI_CHANGE_PIN */
            return AT_CMD_CPIN_UNBLOCK_SET;
        case TAF_MMI_CHANGE_PIN2: /* TAF_MMI_CHANGE_PIN2 */
            return AT_CMD_CPIN2_UNBLOCK_SET;
        case TAF_MMI_UNBLOCK_PIN: /* TAF_MMI_UNBLOCK_PIN */
            return AT_CMD_CPIN_VERIFY_SET;
        case TAF_MMI_UNBLOCK_PIN2: /* TAF_MMI_UNBLOCK_PIN2 */
            return AT_CMD_CPIN2_VERIFY_SET;
        case TAF_MMI_REGISTER_SS: /* TAF_MMI_REGISTER_SS */
            return AT_CMD_SS_REGISTER;
        case TAF_MMI_ERASE_SS: /* TAF_MMI_ERASE_SS */
            return AT_CMD_SS_ERASE;
        case TAF_MMI_ACTIVATE_SS: /* TAF_MMI_ACTIVATE_SS */
            return AT_CMD_SS_ACTIVATE;
        case TAF_MMI_DEACTIVATE_SS: /* TAF_MMI_DEACTIVATE_SS */
            return AT_CMD_SS_DEACTIVATE;
        case TAF_MMI_INTERROGATE_SS: /* TAF_MMI_INTERROGATE_SS */
            return AT_CMD_SS_INTERROGATE;
        case TAF_MMI_REGISTER_PASSWD: /* TAF_MMI_REGISTER_PASSWD */
            return AT_CMD_SS_REGISTER_PSWD;
        /* Delete TAF_MMI_GET_PASSWD分支 */
        case TAF_MMI_PROCESS_USSD_REQ: /* TAF_MMI_PROCESS_USSD_REQ */
            return AT_CMD_CUSD_REQ;
        case TAF_MMI_SUPPRESS_CLIP: /* TAF_MMI_SUPPRESS_CLIP */
            return AT_CMD_CLIP_READ;
        case TAF_MMI_INVOKE_CLIP: /* TAF_MMI_INVOKE_CLIP */
            return AT_CMD_CLIP_READ;

        case TAF_MMI_DEACTIVATE_CCBS: /* TAF_MMI_DEACTIVATE_CCBS */
            return AT_CMD_SS_DEACTIVE_CCBS;

        case TAF_MMI_INTERROGATE_CCBS:
            return AT_CMD_SS_INTERROGATE_CCBS;

        default:
            break;
    }

    return AT_CMD_INVALID;
}


VOS_UINT32 At_UnlockSimLock(VOS_UINT8 indexNum, VOS_UINT32 paraCnt, AT_ParseParaType *paralist)
{
    AT_ParseParaType       *pwdPara = VOS_NULL_PTR;
    DRV_AGENT_SimlockSetReq simlockReq;
    errno_t                 memResult;

    simlockReq.pwdLen = 0;

    if (paraCnt == AT_SIMLOCK_VALID_PARA_NUM) {
        pwdPara = (AT_ParseParaType *)(paralist + 1);

        if (pwdPara->paraLen == TAF_PH_ME_PERSONALISATION_PWD_LEN_MAX) {
            if (At_CheckNumString(pwdPara->para, TAF_PH_ME_PERSONALISATION_PWD_LEN_MAX) != AT_FAILURE) {
                simlockReq.pwdLen = TAF_PH_ME_PERSONALISATION_PWD_LEN_MAX;

                memResult = memcpy_s(simlockReq.pwd, sizeof(simlockReq.pwd), pwdPara->para,
                                     TAF_PH_ME_PERSONALISATION_PWD_LEN_MAX);
                TAF_MEM_CHK_RTN_VAL(memResult, sizeof(simlockReq.pwd), TAF_PH_ME_PERSONALISATION_PWD_LEN_MAX);
                simlockReq.pwd[simlockReq.pwdLen] = 0;
            }
        }
    }

    if (AT_FillAndSndAppReqMsg(g_atClientTab[indexNum].clientId, g_atClientTab[indexNum].opId,
                               DRV_AGENT_SIMLOCK_SET_REQ, (VOS_UINT8 *)&simlockReq, sizeof(simlockReq),
                               I0_WUEPS_PID_DRV_AGENT) == TAF_SUCCESS) {
        /* 设置当前操作模式 */
        g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_DRV_AGENT_SIMLOCK_SET_REQ;

        (VOS_VOID)memset_s(simlockReq.pwd, sizeof(simlockReq.pwd), 0x00, sizeof(simlockReq.pwd));
        /* 由于SIMLOCK特殊处理，这里即使返回等待异步消息解析器也不会起定时器 */
        if (At_StartTimer(AT_SET_PARA_TIME, indexNum) != AT_SUCCESS) {
            At_FormatResultData(indexNum, AT_ERROR);
            return VOS_ERR;
        }

        g_parseContext[indexNum].clientStatus = AT_FW_CLIENT_STATUS_PEND;

        return VOS_OK;
    }

    (VOS_VOID)memset_s(simlockReq.pwd, sizeof(simlockReq.pwd), 0x00, sizeof(simlockReq.pwd));
    At_FormatResultData(indexNum, AT_ERROR);
    return VOS_ERR;
}


VOS_UINT32 At_ProcXmlText(TAF_UINT8 indexNum, TAF_UINT8 *data, TAF_UINT16 len)
{
    errno_t                      memResult;
    VOS_UINT16                   tmpLen;
    AT_XmlProcTypeUint8          xmlProcType;
    AT_MTA_CposReq              *cposSetReq = VOS_NULL_PTR;
    VOS_UINT32                   result;
    MTA_AT_CposOperateTypeUint32 cposOpType;
    AT_ModemAgpsCtx             *agpsCtx = VOS_NULL_PTR;

    agpsCtx = AT_GetModemAgpsCtxAddrFromClientId(indexNum);

    /* 局部变量初始化 */
    tmpLen      = len;
    xmlProcType = AT_XML_PROC_TYPE_BUTT;
    cposOpType  = MTA_AT_CPOS_BUTT;

    /* 收到"Ctrl-Z"字符，发送本条消息 */
    if (data[tmpLen - 1] == AT_CTRL_Z) {
        tmpLen -= 1;
        xmlProcType = AT_XML_PROC_TYPE_FINISH;
        cposOpType  = MTA_AT_CPOS_SEND;
    }
    /* 收到"CR"字符，继续等待输入 */
    else if (data[tmpLen - 1] == AT_CR) {
        xmlProcType = AT_XML_PROC_TYPE_CONTINUE;
    }
    /* 收到"ESC"字符，取消本条消息发送 */
    else if (data[tmpLen - 1] == AT_ESC) {
        tmpLen -= 1;
        xmlProcType = AT_XML_PROC_TYPE_FINISH;
        cposOpType  = MTA_AT_CPOS_CANCEL;
    } else {
        return AT_ERROR;
    }

    /* 把缓冲区的字符加到XML码流中 */
    if (cposOpType != MTA_AT_CPOS_CANCEL) {
        /* XML码流长度判断 */
        if (((agpsCtx->xml.xmlTextCur - agpsCtx->xml.xmlTextHead) + tmpLen) > AT_XML_MAX_LEN) {
            return AT_ERROR;
        }

        if (tmpLen > 0) {
            memResult = memcpy_s(agpsCtx->xml.xmlTextCur,
                                 AT_XML_MAX_LEN - (unsigned int)(agpsCtx->xml.xmlTextCur - agpsCtx->xml.xmlTextHead),
                                 data, tmpLen);
            TAF_MEM_CHK_RTN_VAL(memResult,
                                AT_XML_MAX_LEN - (unsigned int)(agpsCtx->xml.xmlTextCur - agpsCtx->xml.xmlTextHead),
                                tmpLen);
        }

        agpsCtx->xml.xmlTextCur += tmpLen;
    }

    /* 下发XML码流或继续等待输入 */
    if (xmlProcType == AT_XML_PROC_TYPE_FINISH) {
        cposSetReq = (AT_MTA_CposReq *)PS_MEM_ALLOC(WUEPS_PID_AT, sizeof(AT_MTA_CposReq));
        if (cposSetReq == VOS_NULL_PTR) {
            AT_ERR_LOG("At_ProcXmlText: Memory malloc failed!");
            return AT_ERROR;
        }
        (VOS_VOID)memset_s(cposSetReq, sizeof(AT_MTA_CposReq), 0x00, sizeof(AT_MTA_CposReq));

        /* 填写消息结构 */
        cposSetReq->cposOpType = cposOpType;

        /* 当用户输入了Ctrl-Z才将码流发给MTA */
        if (cposOpType == MTA_AT_CPOS_SEND) {
            cposSetReq->xmlLength = (VOS_UINT32)(agpsCtx->xml.xmlTextCur - agpsCtx->xml.xmlTextHead);
            if (cposSetReq->xmlLength > 0) {
                memResult = memcpy_s(cposSetReq->xmlText, MTA_CPOS_XML_MAX_LEN, agpsCtx->xml.xmlTextHead,
                                     cposSetReq->xmlLength);
                TAF_MEM_CHK_RTN_VAL(memResult, MTA_CPOS_XML_MAX_LEN, cposSetReq->xmlLength);
            }
        }

        /* 发送到MTA模块 */
        result = AT_FillAndSndAppReqMsg(g_atClientTab[indexNum].clientId, g_atClientTab[indexNum].opId,
                                        ID_AT_MTA_CPOS_SET_REQ, (VOS_UINT8 *)cposSetReq, sizeof(AT_MTA_CposReq),
                                        I0_UEPS_PID_MTA);

        /* 释放XML码流结构 */
        /*lint -save -e516 */
        PS_MEM_FREE(WUEPS_PID_AT, cposSetReq);
        /*lint -restore */

        if (result != TAF_SUCCESS) {
            AT_ERR_LOG("At_ProcXmlText: AT_FillAndSndAppReqMsg fail.");
            return AT_ERROR;
        }

        /* 设置AT模块实体的状态为等待异步返回 */
        g_atClientTab[indexNum].cmdCurrentOpt = AT_CMD_CPOS_SET;

        return AT_WAIT_ASYNC_RETURN;
    }

    return AT_WAIT_XML_INPUT;
}

/* 删除^EOPLMN设置命令的参数解析函数，使用通用AT参数解析器解析 */
#if (FEATURE_UE_MODE_CDMA == FEATURE_ON)

TAF_UINT32 At_CheckCurrRatModeIsCL(VOS_UINT8 indexNum)
{
    ModemIdUint16 modemId;
    VOS_UINT32    getModemIdRslt;
    VOS_UINT8    *isCLMode = VOS_NULL_PTR;

    modemId = MODEM_ID_0;

    getModemIdRslt = AT_GetModemIdFromClient(indexNum, &modemId);

    /* 获取modemid失败，默认不拦截相应的AT命令 */
    if (getModemIdRslt != VOS_OK) {
        AT_ERR_LOG("At_CurrRatModeIsCL:Get Modem Id fail!");
        return VOS_FALSE;
    }

    isCLMode = AT_GetModemCLModeCtxAddrFromModemId(modemId);

    if (*isCLMode == VOS_TRUE) {
        return VOS_TRUE;
    } else {
        return VOS_FALSE;
    }
}
#endif

VOS_UINT32 AT_ConvertMtaResult(MTA_AT_ResultUint32 result)
{
    AT_RreturnCodeUint32 returnCode;

    switch (result) {
        case MTA_AT_RESULT_NO_ERROR:
            returnCode = AT_OK;
            break;
        case MTA_AT_RESULT_INCORRECT_PARAMETERS:
            returnCode = AT_CME_INCORRECT_PASSWORD;
            break;
        case MTA_AT_RESULT_OPTION_TIMEOUT:
            returnCode = AT_CME_NETWORK_TIMEOUT;
            break;
        case MTA_AT_RESULT_ERROR:
        default:
            returnCode = AT_ERROR;
            break;
    }

    return returnCode;
}

VOS_UINT32 AT_GetSeconds(VOS_VOID)
{
    VOS_UINT32 ulsecond;
    VOS_UINT32 ultimelow    = 0;
    VOS_UINT32 ultimehigh   = 0;
    VOS_UINT64 ulltimeCount = 0;

    if (AT_GetTimerAccuracyTimestamp(&ultimehigh, &ultimelow) == ERR_MSP_SUCCESS) {
        ulltimeCount = ((VOS_UINT64)ultimehigh << 32) | ((VOS_UINT64)ultimelow);
        ulltimeCount = ulltimeCount / SLICE_TO_SECOND_UINT;
    }

    /* 将U64的slice转换为秒之后，只需要取低32位即可。低32位的秒能统计的时间已经达到136年 */
    ulsecond = (VOS_UINT32)(ulltimeCount & SLICE_TO_MAX_SECOND);

    return ulsecond;
}


VOS_VOID AT_GetLiveTime(NAS_MM_InfoInd *aTtime, NAS_MM_InfoInd *newTime, VOS_UINT32 nwSecond)
{
    TIME_ZONE_Time newTimeInfo;
    VOS_UINT32     nowSecond;
    VOS_UINT32     seconds;
    VOS_UINT32     adjustDate;
    VOS_UINT32     indexNum;
    errno_t        memResult;

    (VOS_VOID)memset_s(&newTimeInfo, sizeof(TIME_ZONE_Time), 0x00, sizeof(TIME_ZONE_Time));

    /* 全局变量中保存有时间信息 */
    if ((aTtime->ieFlg & NAS_MM_INFO_IE_UTLTZ) == NAS_MM_INFO_IE_UTLTZ) {
        memResult = memcpy_s(newTime, sizeof(NAS_MM_InfoInd), aTtime, sizeof(NAS_MM_InfoInd));
        TAF_MEM_CHK_RTN_VAL(memResult, sizeof(NAS_MM_InfoInd), sizeof(NAS_MM_InfoInd));

        if (nwSecond == 0) {
            return;
        }

        nowSecond = AT_GetSeconds();

        /* 如果ulNowSecond 超过了slice表示的最大值则进会反转，反转之后需要加上最大值 */
        if (nowSecond >= nwSecond) {
            seconds = nowSecond - nwSecond;
        } else {
            /* 参考其余溢出流程实现, +1的原因是经过0的计数 */
            seconds = SLICE_TO_MAX_SECOND - nwSecond + nowSecond + 1;
        }

        if (seconds != 0) {
            memResult = memcpy_s(&newTimeInfo, sizeof(TIME_ZONE_Time), &aTtime->universalTimeandLocalTimeZone,
                                 sizeof(TIME_ZONE_Time));
            TAF_MEM_CHK_RTN_VAL(memResult, sizeof(TIME_ZONE_Time), sizeof(TIME_ZONE_Time));

            newTimeInfo.second += (VOS_UINT8)(seconds % AT_MINUTE_TO_SECONDS);                          /* 秒 */
            newTimeInfo.minute += (VOS_UINT8)((seconds / AT_MINUTE_TO_SECONDS) % AT_MINUTE_TO_SECONDS); /* 分 */
            newTimeInfo.hour +=
                (VOS_UINT8)((seconds / (AT_MINUTE_TO_SECONDS * AT_HOUR_TO_MINUTES)) % AT_DAY_TO_HOURS); /* 时 */
            adjustDate = (seconds / (AT_MINUTE_TO_SECONDS * AT_HOUR_TO_MINUTES * AT_DAY_TO_HOURS));     /* 天 */

            if (newTimeInfo.second >= AT_MINUTE_TO_SECONDS) {
                newTimeInfo.minute += newTimeInfo.second / AT_MINUTE_TO_SECONDS; /* 分 */
                newTimeInfo.second = newTimeInfo.second % AT_MINUTE_TO_SECONDS;  /* 实际的秒数 */
            }

            if (newTimeInfo.minute >= AT_HOUR_TO_MINUTES) {
                newTimeInfo.hour += newTimeInfo.minute / AT_HOUR_TO_MINUTES;  /* 小时 */
                newTimeInfo.minute = newTimeInfo.minute % AT_HOUR_TO_MINUTES; /* 实际的分数 */
            }

            if ((adjustDate > 0) || (newTimeInfo.hour >= AT_DAY_TO_HOURS)) {
                adjustDate += (newTimeInfo.hour / AT_DAY_TO_HOURS);    /* 天数 */
                newTimeInfo.hour = newTimeInfo.hour % AT_DAY_TO_HOURS; /* 实际的小时数 */

                memResult = memcpy_s(&newTime->universalTimeandLocalTimeZone, sizeof(TIME_ZONE_Time), &newTimeInfo,
                                     sizeof(TIME_ZONE_Time));
                TAF_MEM_CHK_RTN_VAL(memResult, sizeof(TIME_ZONE_Time), sizeof(TIME_ZONE_Time));

                for (indexNum = 0; indexNum < adjustDate; indexNum++) {
                    At_AdjustLocalDate(&newTimeInfo, 1, &newTime->universalTimeandLocalTimeZone);
                    memResult = memcpy_s(&newTimeInfo, sizeof(TIME_ZONE_Time), &newTime->universalTimeandLocalTimeZone,
                                         sizeof(TIME_ZONE_Time));
                    TAF_MEM_CHK_RTN_VAL(memResult, sizeof(TIME_ZONE_Time), sizeof(TIME_ZONE_Time));
                }
            } else {
                memResult = memcpy_s(&newTime->universalTimeandLocalTimeZone, sizeof(TIME_ZONE_Time), &newTimeInfo,
                                     sizeof(TIME_ZONE_Time));
                TAF_MEM_CHK_RTN_VAL(memResult, sizeof(TIME_ZONE_Time), sizeof(TIME_ZONE_Time));
            }
        }
    }
}

#if (FEATURE_UE_MODE_NR == FEATURE_ON)

LOCAL VOS_UINT32 At_ProcUePolicyInfo(VOS_UINT8 clientId, VOS_UINT8 *data, VOS_UINT16 length)
{
    errno_t                memResult;
    VOS_UINT32             allocLen;
    VOS_UINT16             tmpLen         = length - 1;
    AT_StreamProcTypeUint8 streamProcType = AT_STREAM_PROC_TYPE_BUTT;
    AT_ModemUePolicyCtx   *uePolicyCtx    = VOS_NULL_PTR;
    TAF_PS_5GUePolicyInfo *uePolicyInfo   = VOS_NULL_PTR;

    uePolicyCtx = AT_GetModemUePolicyCtxAddrFromClientId(clientId);

    /* 收到"Ctrl-Z"字符，发送本条消息 */
    if (data[tmpLen] == AT_CTRL_Z) {
        streamProcType = AT_STREAM_PROC_TYPE_FINISH;
    }
    /* 收到"CR"字符，继续等待输入 */
    else if (data[tmpLen] == AT_CR) {
        streamProcType = AT_STREAM_PROC_TYPE_CONTINUE;
    }
    /* 收到"ESC"字符，取消本条消息发送 */
    else if (data[tmpLen] == AT_ESC) {
        streamProcType = AT_STREAM_PROC_TYPE_CANCEL;
    } else {
        return AT_ERROR;
    }

    /* 把缓冲区的字符加到码流中 */
    if (streamProcType != AT_STREAM_PROC_TYPE_CANCEL) {
        /* 码流长度判断 */
        if ((VOS_UINT32)((uePolicyCtx->uePolicyCur - uePolicyCtx->uePolicyHead) + tmpLen) > uePolicyCtx->uePolicyLen) {
            AT_ERR_LOG("At_ProcUePolicyInfo: UE POLICY stream too long!");
            return AT_ERROR;
        }

        if (tmpLen > 0) {
            memResult = memcpy_s(uePolicyCtx->uePolicyCur,
                                 uePolicyCtx->uePolicyLen -
                                     (unsigned int)(uePolicyCtx->uePolicyCur - uePolicyCtx->uePolicyHead),
                                 data, tmpLen);
            TAF_MEM_CHK_RTN_VAL(memResult,
                                uePolicyCtx->uePolicyLen -
                                    (unsigned int)(uePolicyCtx->uePolicyCur - uePolicyCtx->uePolicyHead),
                                tmpLen);
        }

        uePolicyCtx->uePolicyCur += tmpLen;
    }

    /* 下发码流或继续等待输入 */
    if (streamProcType == AT_STREAM_PROC_TYPE_FINISH) {
        allocLen     = sizeof(TAF_PS_5GUePolicyInfo) + uePolicyCtx->uePolicyLen / 2;
        uePolicyInfo = (TAF_PS_5GUePolicyInfo *)PS_MEM_ALLOC(WUEPS_PID_AT, allocLen);

        if (uePolicyInfo == VOS_NULL_PTR) {
            AT_ERR_LOG("At_ProcUePolicyInfo: Memory malloc failed!");
            return AT_ERROR;
        }
        (VOS_VOID)memset_s(uePolicyInfo, allocLen, 0x00, allocLen);

        /* 填写消息结构 */
        uePolicyInfo->protocalVer     = uePolicyCtx->protocalVer;
        uePolicyInfo->pti             = uePolicyCtx->pti;
        uePolicyInfo->uePolicyMsgType = uePolicyCtx->uePolicyMsgType;
        uePolicyInfo->classMarkLen    = uePolicyCtx->classMarkLen;
        memResult = memcpy_s(uePolicyInfo->classMark, TAF_PS_UE_POLICY_CLASSMARK_MAX_LEN, uePolicyCtx->classMark,
                             TAF_PS_UE_POLICY_CLASSMARK_MAX_LEN);

        TAF_MEM_CHK_RTN_VAL(memResult, TAF_PS_UE_POLICY_CLASSMARK_MAX_LEN, TAF_PS_UE_POLICY_CLASSMARK_MAX_LEN);

        uePolicyInfo->ueOsIdLen = uePolicyCtx->ueOsIdLen;
        memResult = memcpy_s(uePolicyInfo->ueOsIdInfo, TAF_PS_UE_OS_ID_INFO_MAX_LEN, uePolicyCtx->ueOsIdInfo,
                             TAF_PS_UE_OS_ID_INFO_MAX_LEN);

        TAF_MEM_CHK_RTN_VAL(memResult, TAF_PS_UE_OS_ID_INFO_MAX_LEN, TAF_PS_UE_OS_ID_INFO_MAX_LEN);

        uePolicyInfo->infoLen = uePolicyCtx->uePolicyLen / 2;
        if (At_AsciiString2HexSimple(uePolicyInfo->content, uePolicyCtx->uePolicyHead,
                                     (VOS_UINT16)uePolicyCtx->uePolicyLen) != AT_SUCCESS) {
            PS_MEM_FREE(WUEPS_PID_AT, uePolicyInfo);
            return AT_ERROR;
        }

        /* 调用DSM提供的设置接口 */
        if (TAF_PS_SetCsUePolicy(WUEPS_PID_AT, AT_PS_BuildExClientId(g_atClientTab[clientId].clientId), 0,
                                 uePolicyInfo) != VOS_OK) {
            PS_MEM_FREE(WUEPS_PID_AT, uePolicyInfo);
            return AT_ERROR;
        }

        PS_MEM_FREE(WUEPS_PID_AT, uePolicyInfo);

        g_atClientTab[clientId].cmdCurrentOpt = AT_CMD_CSUEPOLICY_SET;
        return AT_WAIT_ASYNC_RETURN;
    } else if (streamProcType == AT_STREAM_PROC_TYPE_CANCEL) {
        return AT_OK;
    } else {
        return AT_WAIT_UE_POLICY_INPUT;
    }
}


LOCAL VOS_UINT32 At_ProcSendImsUrsp(VOS_UINT8 clientId)
{
    AT_ModemUePolicyCtx   *uePolicyCtx  = VOS_NULL_PTR;
    AT_IMSA_ImsUrspSetReq *urspSetReq   = VOS_NULL_PTR;
    VOS_UINT32             allocLen;
    VOS_UINT32             rst;

    uePolicyCtx = AT_GetModemUePolicyCtxAddrFromClientId(clientId);

    allocLen   = sizeof(AT_IMSA_ImsUrspSetReq) + uePolicyCtx->imsUrspLen / 2;
    urspSetReq = (AT_IMSA_ImsUrspSetReq *)PS_MEM_ALLOC(WUEPS_PID_AT, allocLen);

    if (urspSetReq == VOS_NULL_PTR) {
        AT_ERR_LOG("At_ProcSendImsUrsp: Memory malloc failed!");
        return AT_ERROR;
    }
    (VOS_VOID)memset_s(urspSetReq, allocLen, 0x00, allocLen);

    /* 填写消息结构 */
    urspSetReq->version = (VOS_UINT16)uePolicyCtx->imsUrspVer;

    urspSetReq->length = (VOS_UINT16)(uePolicyCtx->imsUrspLen / 2);
    if (At_AsciiString2HexSimple(urspSetReq->data, uePolicyCtx->imsUrspHead,
                                 (VOS_UINT16)uePolicyCtx->imsUrspLen) != AT_SUCCESS) {
        PS_MEM_FREE(WUEPS_PID_AT, urspSetReq);
        return AT_ERROR;
    }

    rst = AT_FillAndSndAppReqMsg(g_atClientTab[clientId].clientId, g_atClientTab[clientId].opId,
              ID_AT_IMSA_IMS_URSP_SET_REQ, (VOS_UINT8 *)(&urspSetReq->version),
              (VOS_UINT32)(sizeof(urspSetReq->version) + sizeof(urspSetReq->length) + urspSetReq->length),
              PS_PID_IMSA);

    if (rst != TAF_SUCCESS) {
        PS_MEM_FREE(WUEPS_PID_AT, urspSetReq);
        AT_ERR_LOG("At_ProcSendImsUrsp: AT_FillAndSndAppReqMsg FAIL");
        return AT_ERROR;
    }

    PS_MEM_FREE(WUEPS_PID_AT, urspSetReq);

    g_atClientTab[clientId].cmdCurrentOpt = AT_CMD_IMSURSP_SET;

    return AT_WAIT_ASYNC_RETURN;
}


LOCAL VOS_UINT32 At_ProcImsUrspInfo(VOS_UINT8 clientId, VOS_UINT8 *data, VOS_UINT16 length)
{
    AT_ModemUePolicyCtx   *uePolicyCtx  = VOS_NULL_PTR;
    errno_t                memRslt;
    VOS_UINT16             tmpLen         = length - 1;
    AT_StreamProcTypeUint8 streamProcType = AT_STREAM_PROC_TYPE_BUTT;

    uePolicyCtx = AT_GetModemUePolicyCtxAddrFromClientId(clientId);

    /* 收到"Ctrl-Z"字符，发送本条消息 */
    if (data[tmpLen] == AT_CTRL_Z) {
        streamProcType = AT_STREAM_PROC_TYPE_FINISH;
    }
    /* 收到"CR"字符，继续等待输入 */
    else if (data[tmpLen] == AT_CR) {
        streamProcType = AT_STREAM_PROC_TYPE_CONTINUE;
    }
    /* 收到"ESC"字符，取消本条消息发送 */
    else if (data[tmpLen] == AT_ESC) {
        return AT_OK;
    } else {
        return AT_ERROR;
    }

    /* 把缓冲区的字符加到码流中 */
    if ((VOS_UINT32)((uePolicyCtx->imsUrspCur - uePolicyCtx->imsUrspHead) + tmpLen) > uePolicyCtx->imsUrspLen) {
        AT_ERR_LOG("At_ProcImsUrspInfo: ims ursp stream too long!");
        return AT_ERROR;
    }

    if (tmpLen > 0) {
        memRslt = memcpy_s(uePolicyCtx->imsUrspCur,
                      uePolicyCtx->imsUrspLen - (unsigned int)(uePolicyCtx->imsUrspCur - uePolicyCtx->imsUrspHead),
                      data, tmpLen);
        TAF_MEM_CHK_RTN_VAL(memRslt,
           uePolicyCtx->imsUrspLen - (unsigned int)(uePolicyCtx->imsUrspCur - uePolicyCtx->imsUrspHead), tmpLen);
    }

    uePolicyCtx->imsUrspCur += tmpLen;

    /* 下发码流或继续等待输入 */
    if (streamProcType == AT_STREAM_PROC_TYPE_FINISH) {
        /* 发送码流 */
        return At_ProcSendImsUrsp(clientId);
    } else {
        return AT_WAIT_IMSA_URSP_INPUT;
    }
}


VOS_VOID At_ProcUePolicyInfoStream(VOS_UINT8 clientId, VOS_UINT8 *data, VOS_UINT16 length, AT_ParseContext *clientCxt)
{
    VOS_UINT32           result;
    AT_ModemUePolicyCtx *uePolicyCtx = VOS_NULL_PTR;

    uePolicyCtx = AT_GetModemUePolicyCtxAddrFromClientId(clientId);
    result      = At_ProcUePolicyInfo(clientId, data, length);

    /* 处理成功，输出提示符">" */
    if (result == AT_WAIT_UE_POLICY_INPUT) {
        At_FormatResultData(clientId, result);
        return;
    }

    /* 清空缓存区 */
    PS_MEM_FREE(WUEPS_PID_AT, uePolicyCtx->uePolicyHead); /* 清空 */
    (VOS_VOID)memset_s(uePolicyCtx, sizeof(AT_ModemUePolicyCtx), 0x00, sizeof(AT_ModemUePolicyCtx));

    At_SetCmdSubMode(clientId, AT_NORMAL_MODE); /* 置回命令状态 */

    if (result == AT_WAIT_ASYNC_RETURN) {
        if (clientCxt->cmdElement != NULL) {
            if (At_StartTimer(clientCxt->cmdElement->setTimeOut, clientId) != AT_SUCCESS) {
                AT_ERR_LOG("At_ProcImsUrspStream():ERROR:Start Timer Failed");
            }

            clientCxt->clientStatus = AT_FW_CLIENT_STATUS_PEND;
        }
        return;
    }

    At_FormatResultData(clientId, result);
}


VOS_VOID At_ProcImsUrspStream(VOS_UINT8 clientId, VOS_UINT8 *data, VOS_UINT16 length, AT_ParseContext *clientCxt)
{
    VOS_UINT32           result;
    AT_ModemUePolicyCtx *uePolicyCtx = VOS_NULL_PTR;

    uePolicyCtx = AT_GetModemUePolicyCtxAddrFromClientId(clientId);

    result = At_ProcImsUrspInfo(clientId, data, length);

    /* 处理成功，输出提示符">" */
    if (result == AT_WAIT_IMSA_URSP_INPUT) {
        At_FormatResultData(clientId, result);
        return;
    }

    /* 清空缓存区 */
    PS_MEM_FREE(WUEPS_PID_AT, uePolicyCtx->imsUrspHead); /* 清空 */
    (VOS_VOID)memset_s(uePolicyCtx, sizeof(AT_ModemUePolicyCtx), 0x00, sizeof(AT_ModemUePolicyCtx));

    At_SetCmdSubMode(clientId, AT_NORMAL_MODE); /* 置回命令状态 */

    if (result == AT_WAIT_ASYNC_RETURN) {
        if (clientCxt->cmdElement != NULL) {
            if (At_StartTimer(clientCxt->cmdElement->setTimeOut, clientId) != AT_SUCCESS) {
                AT_ERR_LOG("At_ProcImsUrspStream():ERROR:Start Timer Failed");
            }

            clientCxt->clientStatus = AT_FW_CLIENT_STATUS_PEND;
        }
        return;
    }

    At_FormatResultData(clientId, result);
}

#endif

