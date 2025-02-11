#ifndef _SFRAME_H
#define _SFRAME_H

#define SFRAME_FUNC_FRE_TYPE(data)		((data) & 0xf)
#define SFRAME_FRE_OFFSET_COUNT(data)		(((data) >> 1) & 0xf)
#define SFRAME_FRE_OFFSET_SIZE(data)		(((data) >> 5) & 0x3)
#define SFRAME_VERSION_2			2
#define SFRAME_MAGIC				0xdee2

#define SFRAME_FRE_TYPE_ADDR1			0
#define SFRAME_FRE_TYPE_ADDR2   		1
#define SFRAME_FRE_TYPE_ADDR4   		2

struct sframe_preamble
{
	uint16_t sfp_magic;
	uint8_t sfp_version;
	uint8_t sfp_flags;
} __attribute__((__packed__));

struct sframe_header
{
	struct sframe_preamble sfh_preamble;
	uint8_t sfh_abi_arch;
	int8_t sfh_cfa_fixed_fp_offset;
	int8_t sfh_cfa_fixed_ra_offset;
	uint8_t sfh_auxhdr_len;
	uint32_t sfh_num_fdes;
	uint32_t sfh_num_fres;
	uint32_t sfh_fre_len;
	uint32_t sfh_fdeoff;
	uint32_t sfh_freoff;
} __attribute__((__packed__));

struct sframe_func_desc_entry
{
	int32_t sfde_func_start_address;
	uint32_t sfde_func_size;
	uint32_t sfde_func_start_fre_off;
	uint32_t sfde_func_num_fres;
	uint8_t sfde_func_info;
	uint8_t sfde_func_rep_size;
	uint16_t sfde_func_padding2;
} __attribute__((__packed__));

struct sframe_metadata {
	int64_t  fde_offset;
	uint64_t fre_start;
	uint64_t fre_size;
	struct rela *rela;
};

static inline unsigned int get_fres_size(uint8_t fre_type, char *address)
{
	unsigned int size = 0;
	uint8_t fre_info;

	/*
	 * Add the start_address size which could be 1, 2, or 4 bytes
	 */
	size += 1 << fre_type;

	/*
	 * Read the sframe_fre_info which is after the start_address and is of 1 byte.
	 */
	fre_info = *(uint8_t *)(address + size);

	/*
	 * Add 1 byte for sframe_fre_info
	 */
	size += 1;

	/*
	 * Add sizes for offsets.
	 */
	size += SFRAME_FRE_OFFSET_COUNT(fre_info) << SFRAME_FRE_OFFSET_SIZE(fre_info);

	return size;
}

#endif /* _SFRAME_H */
