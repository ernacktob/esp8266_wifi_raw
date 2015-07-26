#ifndef wdev_h
#define wdev_h

struct _wdev_ctrl_sub1 {
	unsigned:12		 f_b0;	/* length of the sniffer_buf ? */
	unsigned:12		 f_b12;	
	unsigned:8		 f_b24;	/* last packet in queue? */
	struct sniffer_buf	*f_4;	/* the sniffer buf data */
	struct _wdev_ctrl_sub1	*f_8;	/* pointer to next element in queue? */
};

/* total size of this struct should be 4 bytes */
struct _wdev_ctrl_sub3 {
	unsigned:8		 f_b0;	/* Thing that should be 0 or between 224 and 252 */
	unsigned:12		 f_b8;	/* length field? */
	unsigned:12		 f_b20;
};

/* This might actually be the same as struct _wdev_ctrl_sub1.
   There are cases where a pointer to one is used as pointer to the other.
   The only difference between these structs is that f_4 is a sniffer_buf
   or a pointer to an array of _wdev_ctrl_sub3. */
struct _wdev_ctrl_sub2 {
	unsigned:12		 f_b0;
	unsigned:12		 f_b12;	/* length field ?*/
	unsigned:8		 f_b24;
	struct _wdev_ctrl_sub3	*f_4;	/* Points to an array of these structs */
	struct _wdev_ctrl_sub2	*f_8;	/* pointer to next element in queue? */
};

/* It is possible that some of the fields are part of separate structs
   which have been declared as static, and are thus not associated with
   a name in the symbol table. I can't really determine this, so for
   now, every field referenced relatively to wDevCtrl is assumed to be
   a subfield of this hypothetical _wdev_ctrl struct. This remark
   also applies to most of the other struct reconstructions and pointer casts. */
struct _wdev_ctrl {
	/* stub... most of the fields I have no idea what they mean */
	uint16			 f_0;	/* some counter ? */
	uint16			 f_2;	/* other counter ? */
	uint8			 f_4;	/* counter or flag? */
	uint8			 f_5;	/* whether in promiscuous mode or not */
	struct _wdev_ctrl_sub1	*f_8;	/* possibly a queue for sniffer bufs */
	struct _wdev_ctrl_sub1	*f_12;
	struct _wdev_ctrl_sub1	*f_16;	/* also a queue, move from here to f_8 */
	struct _wdev_ctrl_sub1	*f_20;

	struct _wdev_ctrl_sub1	 f_24;	/* This could be a static struct... */

	struct _wdev_ctrl_sub2	*f_48;
	struct _wdev_ctrl_sub2	*f_52;
	uint32			 f_356;	/* counts number of full packets? */
	// bunch of counters, I'll add them later
	// other fields...
};

#endif
