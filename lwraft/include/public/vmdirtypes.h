/*
 * Copyright © 2012-2015 VMware, Inc.  All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the “License”); you may not
 * use this file except in compliance with the License.  You may obtain a copy
 * of the License at http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an “AS IS” BASIS, without
 * warranties or conditions of any kind, EITHER EXPRESS OR IMPLIED.  See the
 * License for the specific language governing permissions and limitations
 * under the License.
 */



/*
 * Module Name: VDIR
 *
 * Filename: vmdirtypes.h
 *
 * Abstract:
 *
 * Common types definition
 *
 */

#ifndef __VDIR_TYPES_H__
#define __VDIR_TYPES_H__

#ifdef __cplusplus
extern "C" {
#endif

#ifndef _DCE_IDL_

#if defined(_WIN32) || defined(WIN32_LEAN_AND_MEAN)
typedef     _Longlong             EntryId;
typedef     _Longlong             USN;
#else
typedef     int64_t               EntryId;
typedef     int64_t               USN;
#endif
////////////////////////////////////////////////////////////////////////////
// !!!!!!!!!!!!!!!!!!! IMPORTANT IMPORTANT IMPORTANT !!!!!!!!!!!!!!!!!!!!!!!
// !!!!!!!!!!!!!!!!!!! READ READ READ READ READ READ !!!!!!!!!!!!!!!!!!!!!!!
// make sure ENTRYID and USN are same as db_seq_t(int64_t) in BDB db.h
////////////////////////////////////////////////////////////////////////////
typedef EntryId               ENTRYID;

#ifndef VMDIR_MAX_LDAP_URI_LEN
#define VMDIR_MAX_LDAP_URI_LEN 256
#endif

#ifndef VMDIR_MAX_DN_LEN
#define VMDIR_MAX_DN_LEN 1024
#endif

#ifndef VMDIR_MAX_UPN_LEN
#define VMDIR_MAX_UPN_LEN       512
#endif

typedef enum
{
    VMDIRD_RAFT_ROLE_CANDIDATE = 0,
    VMDIRD_RAFT_ROLE_LEADER,
    VMDIRD_RAFT_ROLE_FOLLOWER
} VMDIR_RAFT_ROLE;

typedef struct _VMDIR_RAFT_NODE
{
    PSTR            pszName;
    VMDIR_RAFT_ROLE role;
    USN             iRaftTerm;
    USN             iLastLogIndex;
    USN             iLastAppliedIndex;
    BOOLEAN         bActive;
    struct _VMDIR_RAFT_NODE*    pNext;
} VMDIR_RAFT_NODE, *PVMDIR_RAFT_NODE;

typedef struct _VMDIR_RAFT_CLUSTER
{
    PSTR            pszLeader;
    DWORD           dwNumMmember;
    DWORD           dwNumActiveFollower;
    PVMDIR_RAFT_NODE    pNode;
} VMDIR_RAFT_CLUSTER, *PVMDIR_RAFT_CLUSTER;

typedef struct _VMDIR_REPL_PARTNER_INFO
{
    CHAR*       pszURI;
} VMDIR_REPL_PARTNER_INFO, *PVMDIR_REPL_PARTNER_INFO;

typedef struct _VMDIR_DC_INFO
{
    CHAR*       pszHostName;
    CHAR*       pszSiteName;
    CHAR**      ppPartners;
    DWORD       dwPartnerCount;
} VMDIR_DC_INFO, *PVMDIR_DC_INFO;

typedef struct _VMDIR_REPL_PARTNER_STATUS
{
    CHAR*       pszHost;
    BOOLEAN     bHostAvailable;
    BOOLEAN     bStatusAvailable;
    USN         targetUsn;
    USN         partnerUsn;
} VMDIR_REPL_PARTNER_STATUS, *PVMDIR_REPL_PARTNER_STATUS;

typedef struct _VMDIR_REPL_UTDVECTOR
{
    CHAR*   pszPartnerInvocationId;
    USN     maxOriginatingUSN;
    struct _VMDIR_REPL_UTDVECTOR*   next;
} VMDIR_REPL_UTDVECTOR, *PVMDIR_REPL_UTDVECTOR;

typedef struct _VMDIR_REPL_REPL_AGREEMENT
{
    CHAR*   pszPartnerName;
    USN     maxProcessedUSN;
    struct _VMDIR_REPL_REPL_AGREEMENT*  next;
} VMDIR_REPL_REPL_AGREEMENT, *PVMDIR_REPL_REPL_AGREEMENT;

typedef struct _VMDIR_REPL_STATE
{
    CHAR*       pszHost;
    CHAR*       pszInvocationId;
    DWORD       dwCycleCount;
    USN         maxVisibleUSN;
    USN         maxConsumableUSN;
    USN         maxOriginatingUSN;
    PVMDIR_REPL_UTDVECTOR       pReplUTDVec;
    PVMDIR_REPL_REPL_AGREEMENT  pReplRA;
} _VMDIR_REPL_STATE, *PVMDIR_REPL_STATE;

typedef struct _VMDIR_SERVER_INFO
{
    CHAR*       pszServerDN;
} VMDIR_SERVER_INFO, *PVMDIR_SERVER_INFO;

typedef struct VMDIR_DC_VERSION_INFO
{
    DWORD dwSize;
    PSTR *ppszServer;
    PSTR *ppszVersion;
    DWORD dwMaxDomainFuncLvl;
} VMDIR_DC_VERSION_INFO, * PVMDIR_DC_VERSION_INFO;

typedef struct _VMDIR_METADATA {
    PSTR  pszAttribute;
    USN   localUsn;
    DWORD dwVersion;
    PSTR  pszOriginatingId;
    PSTR  pszOriginatingTime;
    USN   originatingUsn;
} VMDIR_METADATA, *PVMDIR_METADATA;

typedef struct _VMDIR_METADATA_LIST {
    PVMDIR_METADATA *ppMetadataArray;
    DWORD dwCount;
} VMDIR_METADATA_LIST, *PVMDIR_METADATA_LIST;

// opaque type PVMDIR_LOG_CTX
typedef struct _VMDIR_LOG_CTX* PVMDIR_LOG_CTX;

typedef struct _VMDIR_CONNECTION* PVMDIR_CONNECTION;

typedef struct _VMDIR_SERVER_CONTEXT VMDIR_SERVER_CONTEXT, *PVMDIR_SERVER_CONTEXT;


typedef enum
{
    VMDIRD_STATE_UNDEFINED = 0,
    VMDIRD_STATE_STARTUP,
    VMDIRD_STATE_READ_ONLY,   // Process only read/search requests. Originating and replication updates fail with UNWILLING_TO_PERFORM error
    VMDIRD_STATE_NORMAL,      // Process read-write requests. The normal mode
    VMDIRD_STATE_SHUTDOWN,
    VMDIRD_STATE_FAILURE      // Server has failed in unrecoverable manner

} VDIR_SERVER_STATE;
    
typedef enum _VMDIR_RUNMODE
{
    VMDIR_RUNMODE_NORMAL,
    VMDIR_RUNMODE_STANDALONE,
    VMDIR_RUNMODE_RESTORE
} VMDIR_RUNMODE;
    
#ifndef ENUM_VMDIR_LOG_LEVEL
#define ENUM_VMDIR_LOG_LEVEL
typedef enum
{
    VMDIR_LOG_ERROR = 0,
    VMDIR_LOG_WARNING,
    VMDIR_LOG_INFO,
    VMDIR_LOG_VERBOSE,
    VMDIR_LOG_DEBUG

} VMDIR_LOG_LEVEL;
#endif

#ifndef VMDIR_LOG_MASK_ALL
#define VMDIR_LOG_MASK_ALL -1
#endif

#ifndef VMDIR_USER_CREATE_PARAMS_A_DEFINED
#define VMDIR_USER_CREATE_PARAMS_A_DEFINED 1

typedef struct _VMDIR_USER_CREATE_PARAMS_A
{
    CHAR* pszName;
    CHAR* pszAccount;
    CHAR* pszUPN;
    CHAR* pszFirstname;
    CHAR* pszLastname;
    CHAR* pszPassword;

} VMDIR_USER_CREATE_PARAMS_A, *PVMDIR_USER_CREATE_PARAMS_A;

#endif /* VMDIR_USER_CREATE_PARAMS_A_DEFINED */

#ifndef VMDIR_USER_CREATE_PARAMS_W_DEFINED
#define VMDIR_USER_CREATE_PARAMS_W_DEFINED 1

typedef struct _VMDIR_USER_CREATE_PARAMS_W
{
    WCHAR* pwszName;
    WCHAR* pwszAccount;
    WCHAR* pwszUPN;
    WCHAR* pwszFirstname;
    WCHAR* pwszLastname;
    WCHAR* pwszPassword;

} VMDIR_USER_CREATE_PARAMS_W, *PVMDIR_USER_CREATE_PARAMS_W;

#endif /* VMDIR_USER_CREATE_PARAMS_W_DEFINED */

#ifndef VMDIR_USER_INFO_0_A_DEFINED
#define VMDIR_USER_INFO_0_A_DEFINED 1

typedef struct _VMDIR_USER_INFO_0_A
{
    CHAR* pszAccount;
    CHAR* pszFirstname;
    CHAR* pszLastname;
} VMDIR_USER_INFO_0_A, *PVMDIR_USER_INFO_0_A;

#endif /* VMDIR_USER_INFO_0_DEFINED */

#ifndef VMDIR_USER_INFO_1_A_DEFINED
#define VMDIR_USER_INFO_1_A_DEFINED 1

typedef struct _VMDIR_USER_INFO_1_A
{
    CHAR*   pszAccount;
    CHAR*   pszFirstname;
    CHAR*   pszLastname;
    CHAR*   pszUserSID;
    BOOLEAN bEnabled;
    BOOLEAN bPasswordExpired;
    LONG64  passwordExpiry;
} VMDIR_USER_INFO_1_A, *PVMDIR_USER_INFO_1_A;

#endif /* VMDIR_USER_INFO_1_DEFINED */

#endif


#ifdef _DCE_IDL_

cpp_quote("#include <vmdirtypes.h>")
cpp_quote("#if 0")

#ifndef _WIN32
#include <lw/types.h>
#else
typedef byte BOOLEAN;
typedef unsigned long int UINT32;
typedef unsigned hyper int UINT64;
#endif

#endif

#ifndef VMDIR_WSTRING_DEFINED
#define VMDIR_WSTRING_DEFINED
typedef
#ifdef _DCE_IDL_
[ptr, string]
#endif
unsigned short *wstring;   /* wchar16_t */
#endif

typedef
#ifdef _DCE_IDL_
[context_handle]
#endif
void *vmdir_superlog_cookie_t;

typedef struct _VMDIR_SUPERLOG_SERVER_DATA
{
    UINT64 iServerStartupTime;
    UINT64 iAddCount;
    UINT64 iBindCount;
    UINT64 iDeleteCount;
    UINT64 iModifyCount;
    UINT64 iSearchCount;
    UINT64 iUnbindCount;
} VMDIR_SUPERLOG_SERVER_DATA, *PVMDIR_SUPERLOG_SERVER_DATA;

typedef struct _LDAP_SEARCH_INFO
{
    wstring pwszAttributes;
    wstring pwszBaseDN;
    wstring pwszScope;
    wstring pwszIndexResults;
    UINT32 dwScanned;
    UINT32 dwReturned;
} LDAP_SEARCH_INFO;

typedef
#ifdef _DCE_IDL_
[switch_type(UINT32)]
#endif
union _LDAP_OPERATION_INFO{
#ifdef _DCE_IDL_
[case(0x63U)] // LDAP_REQ_SEARCH
#endif
    LDAP_SEARCH_INFO searchInfo;
#ifdef _DCE_IDL_
[default] ;
#endif
} LDAP_OPERATION_INFO;

typedef struct _VMDIR_SUPERLOG_ENTRY_LDAPOPERATION
{
    wstring pwszLoginDN;
    wstring pwszClientIP;
    wstring pwszServerIP;
    wstring pwszOperation;
    wstring pwszString;
    UINT32 dwClientPort;
    UINT32 dwServerPort;
    UINT32 dwErrorCode;
    UINT64 iStartTime;
    UINT64 iEndTime;
    UINT32 opType;
#ifdef _DCE_IDL_
    [switch_is(opType)]
#endif
    LDAP_OPERATION_INFO opInfo;
} VMDIR_SUPERLOG_ENTRY_LDAPOPERATION, *PVMDIR_SUPERLOG_ENTRY_LDAPOPERATION;

typedef struct _VMDIR_SUPERLOG_ENTRY_LDAPOPERATION_ARRAY
{
    UINT32 dwCount;
#ifdef _DCE_IDL_
    [size_is(dwCount)]
#endif
    PVMDIR_SUPERLOG_ENTRY_LDAPOPERATION entries;
} VMDIR_SUPERLOG_ENTRY_LDAPOPERATION_ARRAY, *PVMDIR_SUPERLOG_ENTRY_LDAPOPERATION_ARRAY;

#define VMDIR_SUPERLOG_TABLE_COL_NUM   7

typedef enum
{
    LOGIN_DN,
    IP,
    PORT,
    OPERATION,
    STRING,
    ERROR_CODE,
    AVG_TIME
} VMDIR_SUPERLOG_TABLE_COLUMN;

typedef struct _VMDIR_SUPERLOG_TABLE_COLUMN_SET
{
    unsigned int isColumnSet[VMDIR_SUPERLOG_TABLE_COL_NUM];
} VMDIR_SUPERLOG_TABLE_COLUMN_SET, *PVMDIR_SUPERLOG_TABLE_COLUMN_SET;

typedef struct _VMDIR_SUPERLOG_TABLE_ROW
{
    char* colVals[VMDIR_SUPERLOG_TABLE_COL_NUM];
    UINT64 totalTime;
    UINT32 count;
} VMDIR_SUPERLOG_TABLE_ROW, *PVMDIR_SUPERLOG_TABLE_ROW;

typedef struct _VMDIR_SUPERLOG_TABLE
{
    UINT32 numRows;
    PVMDIR_SUPERLOG_TABLE_COLUMN_SET cols;
    PVMDIR_SUPERLOG_TABLE_ROW rows;
} VMDIR_SUPERLOG_TABLE, *PVMDIR_SUPERLOG_TABLE;

#ifndef MDB_STATE_OP
#define MDB_STATE_OP

typedef enum MDB_state_op {
        MDB_STATE_CLEAR = 0,
        MDB_STATE_READONLY,
        MDB_STATE_KEEPXLOGS,
        MDB_STATE_GETXLOGNUM
} MDB_state_op;

#endif

#define VMDIR_MDB_DATA_FILE_NAME "data.mdb"
#define VMDIR_MDB_LOCK_FILE_NAME "lock.mdb"
#define VMDIR_MDB_XLOGS_DIR_NAME "xlogs"

#ifdef _DCE_IDL_
cpp_quote("#endif")
#endif


#ifdef __cplusplus
}
#endif

#endif /* __VDIR_TYPES_H__ */
