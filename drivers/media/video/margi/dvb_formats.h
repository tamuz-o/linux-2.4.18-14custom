/*
 * 
 * Copyright (C) 2000, 2001 Marcus Metzler 
 *            for convergence integrated media GmbH
 * 
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 * 

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 

 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 * Or, point your browser to http://www.gnu.org/copyleft/gpl.html
 * 

 * The author can be reached at marcus@convergence.de, 

 * the project's page is at http://linuxtv.org/dvb/
 */

#include <linux/kernel.h>
#include <linux/config.h>

#include <linux/slab.h>
#include <linux/vmalloc.h>
#include <linux/mm.h>
#include <linux/delay.h>
#include <linux/poll.h>
#include <linux/fs.h>
#include <linux/interrupt.h>
#include <asm/io.h>
#include <asm/byteorder.h>

#ifndef _DVB_FORMATS_H_
#define _DVB_FORMATS_H_


#define PROG_STREAM_MAP  0xBC
#ifndef PRIVATE_STREAM1
#define PRIVATE_STREAM1  0xBD
#endif
#define PADDING_STREAM   0xBE
#ifndef PRIVATE_STREAM2
#define PRIVATE_STREAM2  0xBF
#endif
#define AUDIO_STREAM_S   0xC0
#define AUDIO_STREAM_E   0xDF
#define VIDEO_STREAM_S   0xE0
#define VIDEO_STREAM_E   0xEF
#define ECM_STREAM       0xF0
#define EMM_STREAM       0xF1
#define DSM_CC_STREAM    0xF2
#define ISO13522_STREAM  0xF3
#define PROG_STREAM_DIR  0xFF

#define BUFFYSIZE    10*MAX_PLENGTH
//#define MAX_PTS      8192
#define MAX_FRAME    8192
#define MAX_PACK_L   4096
#define PS_HEADER_L1    14
#define PS_HEADER_L2    (PS_HEADER_L1+18)
#define MAX_H_SIZE   (PES_H_MIN + PS_HEADER_L1 + 5)
#define PES_MIN         7
#define PES_H_MIN       9

//flags2
#define PTS_DTS_FLAGS    0xC0
#define ESCR_FLAG        0x20
#define ES_RATE_FLAG     0x10
#define DSM_TRICK_FLAG   0x08
#define ADD_CPY_FLAG     0x04
#define PES_CRC_FLAG     0x02
#define PES_EXT_FLAG     0x01

//pts_dts flags 
#define PTS_ONLY         0x80
#define PTS_DTS          0xC0

#define TS_SIZE        188
#define TRANS_ERROR    0x80
#define PAY_START      0x40
#define TRANS_PRIO     0x20
#define PID_MASK_HI    0x1F
//flags
#define TRANS_SCRMBL1  0x80
#define TRANS_SCRMBL2  0x40
#define ADAPT_FIELD    0x20
#define PAYLOAD        0x10
#define COUNT_MASK     0x0F

// adaptation flags
#define DISCON_IND     0x80
#define RAND_ACC_IND   0x40
#define ES_PRI_IND     0x20
#define PCR_FLAG       0x10
#define OPCR_FLAG      0x08
#define SPLICE_FLAG    0x04
#define TRANS_PRIV     0x02
#define ADAP_EXT_FLAG  0x01

// adaptation extension flags
#define LTW_FLAG       0x80
#define PIECE_RATE     0x40
#define SEAM_SPLICE    0x20


#define MAX_PLENGTH 0xFFFF
#define MMAX_PLENGTH (4*MAX_PLENGTH)

#define IPACKS 2048

typedef struct ipack_s {
	int size;
	int found;
	uint8_t *buf;
	uint8_t cid;
	uint32_t plength;
	uint8_t plen[2];
	uint8_t flag1;
	uint8_t flag2;
	uint8_t hlength;
	uint8_t pts[5];
	uint16_t *pid;
	int mpeg;
	uint8_t check;
	int which;
	int done;
	void *data;
	void (*func)(uint8_t *buf,  int size, void *priv);
	int count;
} ipack;

void instant_repack (uint8_t *buf, int count, ipack *p);
void init_ipack(ipack *p, int size,
		void (*func)(uint8_t *buf,  int size, void *priv));
void free_ipack(ipack * p);
void setup_ts2pes( ipack *pa, ipack *pv, uint16_t *pida, uint16_t *pidv, 
		   void (*pes_write)(uint8_t *buf, int count, void *data),
		   void *priv);
void ts_to_pes( ipack *p, uint8_t *buf); // don't need count (=188)
uint16_t get_pid(uint8_t *pid);


#endif /* _DVB_FORMATS_H_*/
