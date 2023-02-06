

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 8.01.0626 */
/* at Tue Jan 19 11:14:07 2038
 */
 /* Compiler settings for rasman.IDL:
     Oicf, W1, Zp8, env=Win64 (32b run), target_arch=AMD64 8.01.0626
     protocol : all , ms_ext, c_ext, robust
     error checks: allocation ref bounds_check enum stub_data
     VC __declspec() decoration level:
          __declspec(uuid()), __declspec(selectany), __declspec(novtable)
          DECLSPEC_UUID(), MIDL_INTERFACE()
 */
 /* @@MIDL_FILE_HEADING(  ) */



 /* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 500
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __RPCNDR_H_VERSION__
#error this stub requires an updated version of <rpcndr.h>
#endif /* __RPCNDR_H_VERSION__ */


#ifndef __rasman_h_h__
#define __rasman_h_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#ifndef DECLSPEC_XFGVIRT
#if _CONTROL_FLOW_GUARD_XFG
#define DECLSPEC_XFGVIRT(base, func) __declspec(xfg_virtual(base, func))
#else
#define DECLSPEC_XFGVIRT(base, func)
#endif
#endif

/* Forward Declarations */

#ifdef __cplusplus
extern "C" {
#endif 


#ifndef __DefaultIfName_INTERFACE_DEFINED__
#define __DefaultIfName_INTERFACE_DEFINED__

    /* interface DefaultIfName */
    /* [version][uuid] */

    typedef struct Struct_232_t
    {
        short StructMember0;
        long StructMember1;
        /* [size_is][unique] */ byte* StructMember2;
        long StructMember3;
        /* [size_is][unique] */ byte* StructMember4;
        long StructMember5;
        /* [size_is][unique] */ byte* StructMember6;
        long StructMember7;
        /* [size_is][unique] */ byte* StructMember8;
    } 	Struct_232_t;

    typedef /* [switch_type] */ /* [switch_type] */ union union_120
    {
        /* [case()] */ struct Struct_232_t unionMember_2;
        /* [case()][unique] */ struct Struct_408_t* unionMember_7;
        /* [case()] */ long unionMember_8;
        /* [default] */  /* Empty union arm */
    } 	union_120;

    typedef struct Struct_330_t
    {
        /* [string][unique] */ wchar_t* StructMember0;
    } 	Struct_330_t;

    typedef struct Struct_344_t
    {
        long StructMember0;
    } 	Struct_344_t;

    typedef /* [switch_type] */ /* [switch_type] */ union union_294
    {
        /* [case()] */ struct Struct_330_t unionMember_0;
        /* [case()] */ struct Struct_344_t unionMember_1;
        /* [case()] */ struct Struct_344_t unionMember_3;
        /* [case()] */  /* Empty union arm */
        /* [case()] */  /* Empty union arm */
    } 	union_294;




    typedef struct Struct_350_t
    {
        short StructMember0;
        /* [switch_is] */ union union_294 StructMember1;
    } 	Struct_350_t;

    typedef struct Struct_408_t
    {
        long StructMember0;
        /* [size_is][unique] */ struct Struct_350_t* StructMember1;
    } 	Struct_408_t;

    typedef struct Struct_424_t
    {
        /* [range] */ short StructMember0;
        long StructMember1;
        /* [switch_is] */ union union_120 StructMember2;
        hyper StructMember3;
    } 	Struct_424_t;

    typedef struct Struct_564_t
    {
        long StructMember0;
        /* [size_is][unique] */ long* StructMember1;
    } 	Struct_564_t;

    typedef struct Struct_586_t
    {
        byte StructMember0[16];
    } 	Struct_586_t;

    typedef struct Struct_638_t
    {
        long StructMember0;
        /* [size_is][unique] */ struct Struct_586_t* StructMember1;
    } 	Struct_638_t;

    typedef struct Struct_654_t
    {
        struct Struct_564_t StructMember0;
        struct Struct_638_t StructMember1;
        hyper StructMember2;
    } 	Struct_654_t;

    typedef struct Struct_726_t
    {
        long StructMember0;
        /* [string][size_is][unique] */ wchar_t** StructMember1;
    } 	Struct_726_t;

    typedef struct Struct_742_t
    {
        /* [string][unique] */ wchar_t* StructMember0;
        struct Struct_726_t StructMember1;
        struct Struct_726_t StructMember2;
    } 	Struct_742_t;

    typedef struct Struct_806_t
    {
        long StructMember0;
        /* [size_is][unique] */ struct Struct_742_t* StructMember1;
    } 	Struct_806_t;

    typedef struct Struct_822_t
    {
        struct Struct_806_t StructMember0;
        /* [string][unique] */ wchar_t* StructMember1;
    } 	Struct_822_t;

    typedef struct Struct_844_t
    {
        long StructMember0;
        byte StructMember1;
    } 	Struct_844_t;

    typedef struct Struct_898_t
    {
        long StructMember0;
        /* [size_is][unique] */ struct Struct_844_t* StructMember1;
    } 	Struct_898_t;

    typedef struct Struct_914_t
    {
        byte StructMember0[16];
        byte StructMember1;
    } 	Struct_914_t;

    typedef struct Struct_966_t
    {
        long StructMember0;
        /* [size_is][unique] */ struct Struct_914_t* StructMember1;
    } 	Struct_966_t;

    typedef struct Struct_982_t
    {
        struct Struct_898_t StructMember0;
        struct Struct_898_t StructMember1;
        struct Struct_966_t StructMember2;
        struct Struct_966_t StructMember3;
        long StructMember4;
    } 	Struct_982_t;

    typedef struct Struct_1132_t
    {
        long StructMember0;
        /* [size_is][unique] */ struct Struct_1082_t* StructMember1;
    } 	Struct_1132_t;

    typedef struct Struct_1148_t
    {
        short StructMember0;
        struct Struct_1132_t StructMember1;
    } 	Struct_1148_t;

    typedef struct Struct_1164_t
    {
        struct Struct_1148_t StructMember0;
        struct Struct_1148_t StructMember1;
    } 	Struct_1164_t;

    typedef /* [switch_type] */ /* [switch_type] */ union union_1064
    {
        /* [case()] */ struct Struct_1164_t unionMember_6;
        /* [case()] */ struct Struct_1164_t unionMember_17;
        /* [default] */  /* Empty union arm */
    } 	union_1064;

    typedef struct Struct_1082_t
    {
        short StructMember0;
        short StructMember1;
    } 	Struct_1082_t;





    typedef struct Struct_1182_t
    {
        long StructMember0;
        long StructMember1;
    } 	Struct_1182_t;

    typedef struct Struct_1232_t
    {
        long StructMember0;
        /* [size_is][unique] */ struct Struct_1182_t* StructMember1;
    } 	Struct_1232_t;

    typedef struct Struct_1248_t
    {
        byte StructMember0[16];
        byte StructMember1[16];
    } 	Struct_1248_t;

    typedef struct Struct_1304_t
    {
        long StructMember0;
        /* [size_is][unique] */ struct Struct_1248_t* StructMember1;
    } 	Struct_1304_t;

    typedef struct Struct_1320_t
    {
        long StructMember0;
        long StructMember1;
        struct Struct_898_t StructMember2;
        struct Struct_1232_t StructMember3;
        struct Struct_966_t StructMember4;
        struct Struct_1304_t StructMember5;
    } 	Struct_1320_t;

    typedef struct Struct_1460_t
    {
        /* [range] */ short StructMember0;
        short StructMember1;
        /* [string][range][unique] */ wchar_t* StructMember2;
        /* [string][range][unique] */ wchar_t* StructMember3;
        long StructMember4;
        /* [range] */ short StructMember5;
        /* [range] */ short StructMember6;
        /* [switch_is] */ union union_1064 StructMember7;
        struct Struct_1320_t StructMember8;
        struct Struct_1320_t StructMember9;
        /* [string][range][unique] */ wchar_t* StructMember10;
        /* [string][range][unique] */ wchar_t* StructMember11;
        /* [string][range][unique] */ wchar_t* StructMember12;
        /* [string][range][unique] */ wchar_t* StructMember13;
        /* [string][range][unique] */ wchar_t* StructMember14;
        /* [string][range][unique] */ wchar_t* StructMember15;
    } 	Struct_1460_t;

    typedef struct Struct_1580_t
    {
        long StructMember0;
        /* [size_is][unique] */ struct Struct_1460_t* StructMember1;
    } 	Struct_1580_t;

    typedef struct Struct_1596_t
    {
        struct Struct_1580_t StructMember0;
        long StructMember1;
        small StructMember2;
    } 	Struct_1596_t;

    typedef struct Struct_1684_t
    {
        struct Struct_726_t StructMember0;
        long StructMember1;
        /* [string][unique] */ wchar_t* StructMember2;
        long StructMember3;
        long StructMember4;
        long StructMember5;
    } 	Struct_1684_t;

    typedef struct Struct_1764_t
    {
        long StructMember0;
        short StructMember1;
        short StructMember2;
        byte StructMember3[8];
    } 	Struct_1764_t;


    typedef struct Struct_1934_t
    {
        /* [string][unique] */ wchar_t* StructMember0;
    } 	Struct_1934_t;


    typedef struct Struct_1948_t
    {
        struct Struct_726_t StructMember0;
    } 	Struct_1948_t;

    typedef struct Struct_1962_t
    {
        /* [string][unique] */ wchar_t* StructMember0;
    } 	Struct_1962_t;


    typedef struct Struct_1976_t
    {
        /* [string][unique] */ wchar_t* StructMember0;
    } 	Struct_1976_t;

    typedef /* [switch_type] */ /* [switch_type] */ union union_1898
    {
        /* [case()] */ struct Struct_1934_t unionMember_0;
        /* [case()] */ struct Struct_1948_t unionMember_1;
        /* [case()] */ struct Struct_1962_t unionMember_2;
        /* [case()] */ struct Struct_344_t unionMember_3;
        /* [case()] */ struct Struct_1976_t unionMember_4;
    } 	union_1898;






    typedef struct Struct_1990_t
    {
        short StructMember0;
        long StructMember1;
        /* [string][unique] */ wchar_t* StructMember2;
        /* [switch_is] */ union union_1898 StructMember3;
    } 	Struct_1990_t;

    typedef struct Struct_2052_t
    {
        long StructMember0;
        long StructMember1;
        /* [size_is][unique] */ struct Struct_1990_t* StructMember2;
    } 	Struct_2052_t;

    typedef struct Struct_2136_t
    {
        unsigned char StructMember0[6];
    } 	Struct_2136_t;

    typedef struct Struct_2168_t
    {
        byte StructMember0;
        byte StructMember1;
        struct Struct_2136_t StructMember2;
        /* [size_is] */ long StructMember3[1];
    } 	Struct_2168_t;

    typedef struct Struct_2236_t
    {
        /* [unique] */ struct Struct_2168_t* StructMember0;
        /* [unique] */ struct Struct_2168_t* StructMember1;
        /* [string][unique] */ wchar_t* StructMember2;
    } 	Struct_2236_t;

    typedef struct Struct_2302_t
    {
        long StructMember0;
        /* [size_is][unique] */ struct Struct_2236_t* StructMember1;
    } 	Struct_2302_t;

    long Proc0_Rpc_VpnProtEngOpen(
        /* [string][in] */ wchar_t* arg_1,
        /* [context_handle][out] */ void** arg_2);

    long Proc1_Rpc_VpnProtEngClose(
        /* [context_handle][out][in] */ void** arg_1);

    long Proc2_Rpc_VpnProtEngEventSubscribe(
        /* [string][in] */ wchar_t* arg_1,
        /* [context_handle][out][in] */ void** arg_2);

    long Proc3_Rpc_VpnProtEngEventSubscriptionGetLastEvent(
        /* [context_handle][in] */ void* arg_2,
        /* [ref][out] */ struct Struct_424_t** arg_3);

    long Proc4_Rpc_VpnProtEngEventUnsubscribe(
        /* [context_handle][out][in] */ void** arg_1);

    long Proc5_Rpc_VpnProtEngSendGetCredentialsRequest(
        /* [context_handle][in] */ void* arg_1,
        /* [in] */ short arg_2,
        /* [in] */ long arg_3,
        /* [in] */ long arg_4,
        /* [in] */ long arg_5,
        /* [size_is][unique][in] */ byte* arg_6);

    long Proc6_Rpc_VpnProtEngSendNegotiatingNetworkRequest(
        /* [context_handle][in] */ void* arg_1);

    long Proc7_Rpc_VpnProtEngSendInterfaceCreateRequest(
        /* [context_handle][in] */ void* arg_1,
        /* [in] */ struct Struct_654_t* arg_2,
        /* [in] */ long* arg_3,
        /* [in] */ struct Struct_586_t* arg_4,
        /* [unique][in] */ struct Struct_822_t* arg_5,
        /* [unique][in] */ struct Struct_982_t* arg_6,
        /* [unique][in] */ struct Struct_1596_t* arg_7,
        /* [in] */ long arg_8);

    long Proc8_Rpc_VpnProtEngSendInterfaceDestroyRequest(
        /* [context_handle][in] */ void* arg_1);

    long Proc9_Rpc_VpnProtEngDisconnectInternal(
        /* [context_handle][in] */ void* arg_1);

    long Proc10_Rpc_VpnProtEngSendConnectError(
        /* [context_handle][in] */ void* arg_1,
        /* [in] */ long arg_2,
        /* [in] */ long arg_3,
        /* [string][in] */ wchar_t* arg_4);

    long Proc11_Rpc_VpnProtEngGetConfiguration(
        /* [context_handle][in] */ void* arg_1,
        /* [ref][out] */ struct Struct_1684_t** arg_2,
        /* [string][size_is][out] */ wchar_t** arg_3,
        /* [out] */ struct Struct_1764_t* arg_4);

    long Proc12_Rpc_VpnProtEngGetBestCostInterface(
        /* [context_handle][in] */ void* arg_1,
        /* [out] */ struct Struct_1764_t* arg_2);

    long Proc13_Rpc_VpnProtEngGetStatementOfHealth(
        /* [context_handle][in] */ void* arg_1,
        /* [size_is][size_is][ref][out] */ byte** arg_2,
        /* [out] */ long* arg_3);

    long Proc14_Rpc_VpnProtEngSetKeepAliveFrequencyOverrideRequest(
        /* [context_handle][in] */ void* arg_1,
        /* [in] */ long arg_2);

    long Proc15_Rpc_VpnProtEngSendGetCustomPromptRequest(
        /* [context_handle][in] */ void* arg_1,
        /* [in] */ struct Struct_2052_t* arg_2);

    long Proc16_Rpc_VpnProtEngSendGetCertificateConsentBlankUiRequest(
        /* [context_handle][in] */ void* arg_1,
        /* [in] */ long arg_2);

    long Proc17_Rpc_VpnProtEngSendCloseCertificateConsentBlankUiRequest(
        /* [context_handle][in] */ void* arg_1,
        /* [in] */ long arg_2,
        /* [size_is][unique][in] */ byte* arg_3);

    long Proc18_Rpc_VpnProtEngPluginInstall(
        /* [in] */ handle_t IDL_handle,
        /* [string][in] */ wchar_t* arg_1,
        /* [string][in] */ wchar_t* arg_2,
        /* [in] */ struct Struct_2168_t* arg_3);

    long Proc19_Rpc_VpnProtEngPluginUninstall(
        /* [in] */ handle_t IDL_handle,
        /* [string][in] */ wchar_t* arg_1,
        /* [in] */ struct Struct_2168_t* arg_2);

    long Proc20_Rpc_VpnProtEngSetCostNetworkSettings(
        /* [in] */ handle_t IDL_handle,
        /* [in] */ long arg_1,
        /* [in] */ long arg_2);

    long Proc21_Rpc_VpnProtEngGetCostNetworkSettings(
        /* [in] */ handle_t IDL_handle,
        /* [out] */ long* arg_1,
        /* [out] */ long* arg_2);

    long Proc22_Rpc_VpnProtEngExecuteAndCaptureLogs(
        /* [in] */ handle_t IDL_handle,
        /* [string][in] */ wchar_t* arg_1);

    long Proc23_Rpc_VpnProtEngGetInterface(
        /* [in] */ handle_t IDL_handle,
        /* [in] */ long arg_1,
        /* [string][size_is][in] */ wchar_t** arg_2,
        /* [string][size_is][in] */ wchar_t** arg_3,
        /* [in] */ long arg_4,
        /* [out] */ long* arg_5,
        /* [out] */ long* arg_6);

    long Proc24_Rpc_VpnProtEngPluginEnumerate(
        /* [in] */ handle_t IDL_handle,
        /* [ref][out] */ struct Struct_2302_t** arg_1);

    long Proc25_Rpc_VpnProtEngAddProfileFromXml(
        /* [in] */ handle_t IDL_handle,
        /* [string][in] */ wchar_t* arg_1);

    long Proc26_Rpc_VpnProtEngWinRtConnect(
        /* [in] */ handle_t IDL_handle,
        /* [string][in] */ wchar_t* arg_1,
        /* [string][in] */ wchar_t* arg_2,
        /* [string][unique][in] */ wchar_t* arg_3,
        /* [string][unique][in] */ wchar_t* arg_4);

    long Proc27_Rpc_VpnProtEngWinRtDisconnect(
        /* [in] */ handle_t IDL_handle,
        /* [string][in] */ wchar_t* arg_1,
        /* [string][in] */ wchar_t* arg_2);

    long Proc28_Rpc_VpnProtEngWinRtEnumerate(
        /* [in] */ handle_t IDL_handle,
        /* [out] */ long* arg_1,
        /* [string][size_is][size_is][ref][out] */ wchar_t*** arg_2);

    long Proc29_Rpc_VpnProtEngWinRtGetEapBlob(
        /* [in] */ handle_t IDL_handle,
        /* [string][in] */ wchar_t* arg_1,
        /* [size_is][size_is][ref][out] */ byte** arg_2,
        /* [out] */ long* arg_3,
        /* [out] */ long* arg_4);

    long Proc30_Rpc_VpnProtEngWinRtGetEapXml(
        /* [in] */ handle_t IDL_handle,
        /* [size_is][in] */ byte* arg_1,
        /* [in] */ long arg_2,
        /* [in] */ long arg_3,
        /* [string][ref][out] */ wchar_t** arg_4);

    long Proc31_Rpc_VpnProtEngWinRtGetMoniker(
        /* [in] */ handle_t IDL_handle,
        /* [string][in] */ wchar_t* arg_1,
        /* [string][ref][out] */ wchar_t** arg_2);

    long Proc32_Rpc_VpnProtEngWinRtGetConnectionStatus(
        /* [in] */ handle_t IDL_handle,
        /* [string][in] */ wchar_t* arg_1,
        /* [string][in] */ wchar_t* arg_2,
        /* [out] */ long* arg_3);

    long Proc33_Rpc_VpnProtEngWinRtUpdateRegistryProfileList(
        /* [in] */ handle_t IDL_handle,
        /* [string][in] */ wchar_t* arg_1);

    long Proc34_Rpc_VpnProtEngGetProxyForUrlAndSingleSessionDeviceUser(
        /* [in] */ handle_t IDL_handle,
        /* [string][in] */ wchar_t* arg_1,
        /* [string][ref][out] */ wchar_t** arg_2);



    extern RPC_IF_HANDLE DefaultIfName_v1_0_c_ifspec;
    extern RPC_IF_HANDLE DefaultIfName_v1_0_s_ifspec;
#endif /* __DefaultIfName_INTERFACE_DEFINED__ */

    /* Additional Prototypes for ALL interfaces */

    /* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


