CREATE TABLE IF NOT EXISTS public.temp_log (
    hash	character varying(64) PRIMARY KEY,
    filepath	character varying(256),
    filename	character varying(64)
);

CREATE TABLE IF NOT EXISTS public.temp_share (
    workinfoid	bigint,
    clientid	bigint,
    enonce1	character varying(8),
    nonce2	character varying(16),
    nonce	character varying(8),
    ntime	character varying(8),
    diff	real,
    sdiff	real,
    hash	character varying(64) PRIMARY KEY,
    result	boolean,
    reject_reason	character varying(16),
    errn	bigint,
    createdate  numeric(20,9),
    createby	character varying(8),
    createcode	character varying(16),
    createinet	character varying(40),
    workername	character varying(40),
    username	character varying(40),
    address	character varying(40),
    agent	character varying(16)
);

