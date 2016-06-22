#ifndef SI_SCHEME_H_
#define SI_SCHEME_H_

/*
 * sophia database
 * sphia.org
 *
 * Copyright (c) Dmitry Simonenko
 * BSD License
*/

typedef struct sicompaction sicompaction;
typedef struct sischeme sischeme;

typedef enum {
	SI_SCACHE,
	SI_SANTI_CACHE,
	SI_SIN_MEMORY
} sistorage;

struct sicompaction {
	uint32_t mode;
	uint64_t checkpoint_wm;
	uint32_t compact_wm;
	uint32_t compact_mode;
	uint32_t branch_wm;
	uint32_t branch_age;
	uint32_t branch_age_period;
	uint64_t branch_age_period_us;
	uint32_t branch_age_wm;
	uint32_t snapshot_period;
	uint64_t snapshot_period_us;
	uint32_t anticache_period;
	uint64_t anticache_period_us;
	uint32_t expire_period;
	uint64_t expire_period_us;
	uint32_t gc_period;
	uint64_t gc_period_us;
	uint32_t gc_wm;
	uint32_t lru_period;
	uint64_t lru_period_us;
};

struct sischeme {
	uint32_t      id;
	char         *name;
	char         *path;
	char         *path_backup;
	uint32_t      mmap;
	sistorage     storage;
	sicompaction  compaction;
	char         *storage_sz;
	uint32_t      sync;
	uint64_t      memory_limit;
	uint64_t      memory_limit_anticache;
	uint64_t      node_size;
	uint32_t      node_page_size;
	uint32_t      node_page_checksum;
	uint32_t      node_compact_load;
	uint32_t      expire;
	uint32_t      compression_cold;
	char         *compression_cold_sz;
	ssfilterif   *compression_cold_if;
	uint32_t      compression_hot;
	char         *compression_hot_sz;
	ssfilterif   *compression_hot_if;
	uint32_t      temperature;
	uint32_t      amqf;
	uint64_t      lru;
	uint32_t      lru_step;
	uint32_t      buf_gc_wm;
	sfupsert      upsert;
	sfscheme      scheme;
	srversion     version;
	srversion     version_storage;
};

void si_schemeinit(sischeme*);
void si_schemefree(sischeme*, sr*);
int  si_schemedeploy(sischeme*, sr*);
int  si_schemerecover(sischeme*, sr*);

#endif
