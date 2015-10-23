#include "utils.h"

int slb_db_open(MDB_env **env, char* path) {
	int rc;

	if(!path || path == '\0') {
		printf("no db path given");
		return -1;
	}

	if((rc = mdb_env_create(env)) != 0) {
		printf("[FAILED mdb_env_create] %s\n", mdb_strerror(rc));
		return -1;
	}

	if((rc = mdb_env_set_mapsize(*env, 10485760)) != 0) {
		printf("[FAILED mdb_env_set_mapsize] %s\n", mdb_strerror(rc));
		return -1;
	}

	// if((rc = mdb_env_set_maxreaders(*env, readers)) != 0){
	// 	printf("[FAILED mdb_env_set_maxreaders] readers: %d, error: %s\n", readers, mdb_strerror(rc));
	// }

	if((rc = mdb_env_open(*env, path, MDB_WRITEMAP, 0664)) != 0) {
		printf("[FAILED mdb_env_open] %s\n", mdb_strerror(rc));
		return -1;
	}

	return 0;
}


int slb_db_read(MDB_env **env, char *key, void **data) {
	int rc, code=0;
	MDB_txn *txn;
	MDB_dbi dbi;
	MDB_val k, v;
	char kval[MAXKEYSIZE];

	k.mv_size = sizeof(int);
	k.mv_data = kval;

	if(!key || key == '\0'){
		printf("[FAILED slb_db_read] no key given\n");
		return -1;
	}

	sprintf(kval, "%s", key);

	if((rc = mdb_txn_begin(*env, NULL, MDB_RDONLY, &txn)) != 0) {
		printf("[FAILED mdb_txn_begin] %s\n", mdb_strerror(rc));
		code = -1;
		goto slb_db_read_fail;
	}

	if((rc = mdb_open(txn, NULL, 0, &dbi)) != 0){
		printf("[FAILED mdb_open] %s\n", mdb_strerror(rc));
		code = -1;
		goto slb_db_read_fail;
	}

	if((rc = mdb_get(txn, dbi, &k, &v)) != 0) {
		if(rc == MDB_NOTFOUND) {
			*data = NULL;
			code = 1; // entry not found, time to fall back to the old gethostbyname
			goto slb_db_read_fail;
		}
		printf("[FAILED mdb_get] %s\n", mdb_strerror(rc));
		code = -1;
		goto slb_db_read_fail;
	}

	if(data && *data) {
		memcpy((char*)*data, (char*)v.mv_data, v.mv_size);
	}


	if((rc = mdb_txn_commit(txn)) != 0) {
		printf("[FAILED mdb_txn_commit] %s\n", mdb_strerror(rc));
		code = -1;
	}

	slb_db_read_fail:
		mdb_close(*env, dbi);

	return code;
}

int slb_db_write(MDB_env **env, char *key, void *value, size_t value_sz){
	int rc, code=0;
	MDB_txn *txn;
	MDB_dbi dbi;

	MDB_val k,v;

	char kval[MAXKEYSIZE];

	k.mv_size = sizeof(int);
	k.mv_data = kval;

	if(!key || key == '\0') {
		printf("[FAILED slb_db_write] no key given\n");
		return -1;
	}
	sprintf(kval, "%s", key);

	v.mv_size = value_sz;
	v.mv_data = value;

	if((rc = mdb_txn_begin(*env, NULL, 0, &txn)) != 0) {
		printf("[FAILED mdb_txn_begin] %s\n", mdb_strerror(rc));
		code = -1;
		goto slb_db_write_fail;
	}

	if((rc = mdb_open(txn, NULL, 0, &dbi)) != 0) {
		printf("[FAILED mdb_open] %s\n", mdb_strerror(rc));
		code = -1;
		goto slb_db_write_fail;
	}

	if((rc = mdb_put(txn, dbi, &k, &v, 0)) != 0) {
		printf("[FAILED mdb_put] %s\n", mdb_strerror(rc));
		code = -1;
		goto slb_db_write_fail;
	}

	if((rc = mdb_txn_commit(txn)) != 0) {
		printf("[FAILED mdb_txn_commit] %s\n", mdb_strerror(rc));
		code = -1;
		goto slb_db_write_fail;
	}
	slb_db_write_fail:
		mdb_close(*env, dbi);

	return code;
}

int slb_db_del(MDB_env **env, char *key){
	int rc;
	MDB_txn *txn;
	MDB_dbi dbi;
	MDB_val k;
	char kval[MAXKEYSIZE];

	if(!key || key == '\0') {
		printf("[FAILED slb_db_del] no key specified\n");
		return -1;
	}

	k.mv_size = sizeof(int);
	k.mv_data = kval;
	sprintf(kval, "%s", key);

	if((rc = mdb_txn_begin(*env, NULL, 0, &txn)) != 0) {
		printf("[FAILED mdb_txn_begin] %s\n", mdb_strerror(rc));
		return -1;
	}

	if((rc = mdb_dbi_open(txn, NULL, 0, &dbi)) != 0) {
		printf("[FAILED mdb_dbi_open] %s\n", mdb_strerror(rc));
		return -1;
	}

	if((rc = mdb_del(txn, dbi, &k, NULL)) != 0) {
		printf("[FAILED mdb_del] %s\n", mdb_strerror(rc));
		return -1;
	}

	if((rc = mdb_txn_commit(txn)) != 0) {
		printf("[FAILED mdb_txn_commit] %s\n", mdb_strerror(rc));
		return -1;
	}

	mdb_close(*env, dbi);

	return 0;
}

void slb_db_close(MDB_env **env) {
	mdb_env_close(*env);
}
