/*
 * The HAVAL hashing function
 *
 * Public domain implementation by Paulo S.L.M. Barreto <pbarreto@nw.com.br>
 *
 * Version 1.1 (1998.04.19)
 *
 * Differences from Version 1.0 (1997.04.03):
 *
 * -  Added more parentheses to enforce operator precedence in the definitions
 *    of the nonlinear F functions, as some compilers do not automatically
 *    apply the standard precedence rules of the C language (Borland C++ 5.02
 *      being the culprit in mind)
 *
 * -    Similar changes in HavalUpdate() and in the ROTR macro for
 *      the same reason.
 *
 * -    Explicitly make casts when reducing precision of an integer value
 *      or from char to byte (= unsigned char) to silence compilation warnings
 *      from Borland C++
 *
 * -  Explicitly defined LITTLE_ENDIAN for compilation under MSVC++ and
 *    Borland C++.
 *
 * -  Fixed the sense of the ROTR macro
 *
 * -  Fixed behaviour on big-endian platforms by doing byte reversal when
 *    unpacking the final digest from the hash context structure to the
 *      digest argument of HavalFinal()
 *
 * -    Forcefully note that the code as is does not work on VMS
 *
 * We are grateful to Mr. Tines <tines@windsong.demon.co.uk> for pointing out
 * these problems, providing solutions for them, and testing the fixed code.
 *
 * -----------------------------------------------------------------------------
 *
 * Reference: Zheng, Y., Pieprzyk, J., Seberry, J.:
 * "HAVAL - a one-way hashing algorithm with variable length of output",
 * Advances in Cryptology (AusCrypt'92), LNCS 718 (1993), 83-104, Springer-Verlag.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ''AS IS'' AND ANY EXPRESS
 * OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
 * BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
 * OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/* (C) 2002, 2003, 2004 Michael Buesch :
 *   Haval hashing algorithm derived from libmhash 0.8.17.
 *   Modified/deleted/added some functions.
 */

/*
 * Test-vectors:
 *
 *
 * "abc"
 * HAVAL-3-128 : 9E40ED883FB63E985D299B40CDA2B8F2
 * HAVAL-3-160 : B21E876C4D391E2A897661149D83576B5530A089
 * HAVAL-3-192 : A7B14C9EF3092319B0E75E3B20B957D180BF20745629E8DE
 * HAVAL-3-224 : 5BC955220BA2346A948D2848ECA37BDD5ECA6ECCA7B594BD32923FAB
 * HAVAL-3-256 : 8699F1E3384D05B2A84B032693E2B6F46DF85A13A50D93808D6874BB8FB9E86C
 * HAVAL-4-128 : 6F2132867C9648419ADCD5013E532FA2
 * HAVAL-4-160 : 77ACA22F5B12CC09010AFC9C0797308638B1CB9B
 * HAVAL-4-192 : 7E29881ED05C915903DD5E24A8E81CDE5D910142AE66207C
 * HAVAL-4-224 : 124C43D2BA4884599D013E8C872BFEA4C88B0B6BF6303974CBE04E68
 * HAVAL-4-256 : 8F409F1BB6B30C5016FDCE55F652642261575BEDCA0B9533F32F5455459142B5
 * HAVAL-5-128 : D054232FE874D9C6C6DC8E6A853519EA
 * HAVAL-5-160 : AE646B04845E3351F00C5161D138940E1FA0C11C
 * HAVAL-5-192 : D12091104555B00119A8D07808A3380BF9E60018915B9025
 * HAVAL-5-224 : 8081027A500147C512E5F1055986674D746D92AF4841ABEB89DA64AD
 * HAVAL-5-256 : 976CD6254C337969E5913B158392A2921AF16FCA51F5601D486E0A9DE01156E7
 *
 *
 * "abcdefghijklmnopqrstuvwxyz"
 * HAVAL-3-128 : DC502247FB3EB8376109EDA32D361D82
 * HAVAL-3-160 : EBA9FA6050F24C07C29D1834A60900EA4E32E61B
 * HAVAL-3-192 : A25E1456E6863E7D7C74017BB3E098E086AD4BE0580D7056
 * HAVAL-3-224 : 06AE38EBC43DB58BD6B1D477C7B4E01B85A1E7B19B0BD088E33B58D1
 * HAVAL-3-256 : 72FAD4BDE1DA8C8332FB60561A780E7F504F21547B98686824FC33FC796AFA76
 * HAVAL-4-128 : B2A73B99775FFB17CD8781B85EC66221
 * HAVAL-4-160 : 1C7884AF86D11AC120FE5DF75CEE792D2DFA48EF
 * HAVAL-4-192 : 2E2E581D725E799FDA1948C75E85A28CFE1CF0C6324A1ADA
 * HAVAL-4-224 : A0AC696CDB2030FA67F6CC1D14613B1962A7B69B4378A9A1B9738796
 * HAVAL-4-256 : 124F6EB645DC407637F8F719CC31250089C89903BF1DB8FAC21EA4614DF4E99A
 * HAVAL-5-128 : 0EFFF71D7D14344CBA1F4B25F924A693
 * HAVAL-5-160 : 917836A9D27EED42D406F6002E7D11A0F87C404C
 * HAVAL-5-192 : 85F1F1C0ECA04330CF2DE5C8C83CF85A611B696F793284DE
 * HAVAL-5-224 : 1B360ACFF7806502B5D40C71D237CC0C40343D2000AE2F65CF487C94
 * HAVAL-5-256 : C9C7D8AFA159FD9E965CB83FF5EE6F58AEDA352C0EFF005548153A61551C38EE
 *
 *
 * "haval test vector"
 * HAVAL-3-128 : 3A32BABE845B476841D09615C049ED54
 * HAVAL-3-160 : DC597BA0B31181EAECD07D16A7BACB938B4535FE
 * HAVAL-3-192 : D303FAB309A48F588129E30D944AAFFB47E4312828F90A45
 * HAVAL-3-224 : DB787E0434DBC763752B38D0119B96AA6D3B171DF7B415909ED2D132
 * HAVAL-3-256 : 6760B03B9DAB7F46AF14B385B563C4E8EDDC1D102278F1296D76523AC444C4BA
 * HAVAL-4-128 : CABE71B3A20822AE457D4391D483DE76
 * HAVAL-4-160 : 9B4F0FADA7D58EB548F2A789CCA6C8DE42C2A047
 * HAVAL-4-192 : 1D2FA58831C4F7E219133DB04DEBF518E1029B757A0A8958
 * HAVAL-4-224 : 01566E57D13C43BEF667DCAA909B1A021FEACA43A6D855C0CE754133
 * HAVAL-4-256 : 6C4A6CF0BD92A99ED509B746B01FC6EC34503F83BD2ECD6F09B73C01029E8478
 * HAVAL-5-128 : AA673B41BC54D467969A41C0C06E9BEE
 * HAVAL-5-160 : 7857F533520BAB5689ED95C8019F0B84A5A8053B
 * HAVAL-5-192 : 459A0399ABAC23CEB4D6A89E7EB312A69ECA335FA180A504
 * HAVAL-5-224 : 5AD21F9AC2BBD8C906312DE06D554F10D1F058FE6D1EF397FACB0B69
 * HAVAL-5-256 : CDE2484665A560F21EAD246EFF408828FEC111381D92EDDCFB502B36969FEA25
 */

#include "haval.hpp"

#include <cstdio>
#include <cstring>
#include <stdexcept>
#include <sys/stat.h>

/* Nonlinear F functions */

/*#define F1(X6, X5, X4, X3, X2, X1, X0) \
	((X1) & (X4) ^ (X2) & (X5) ^ (X3) & (X6) ^ (X0) & (X1) ^ (X0))*/
#define F1(X6, X5, X4, X3, X2, X1, X0) \
	(((X1) & ((X4) ^ (X0))) ^ ((X2) & (X5)) ^ ((X3) & (X6)) ^ (X0))

/*#define F2(X6, X5, X4, X3, X2, X1, X0) \
	((X1) & (X2) & (X3) ^ (X2) & (X4) & (X5) ^ \
	(X1) & (X2) ^ (X1) & (X4) ^ (X2) & (X6) ^ (X3) & (X5) ^ \
	(X4) & (X5) ^ (X0) & (X2) ^ (X0))*/
#define F2(X6, X5, X4, X3, X2, X1, X0) \
	(((X2) & (((X1) & (~(X3))) ^ ((X4) & (X5)) ^ (X6) ^ (X0))) ^ \
	(((X4) & ((X1) ^ (X5))) ^ ((X3) & (X5)) ^ (X0)))

/*#define F3(X6, X5, X4, X3, X2, X1, X0) \
	((X1) & (X2) & (X3) ^ (X1) & (X4) ^ (X2) & (X5) ^ (X3) & (X6) ^ (X0) &
(X3) ^ (X0))*/
#define F3(X6, X5, X4, X3, X2, X1, X0) \
	(((X3) & (((X1) & (X2)) ^ (X6) ^ (X0))) ^ ((X1) & (X4)) ^ \
	((X2) & (X5)) ^ (X0))

/*#define F4(X6, X5, X4, X3, X2, X1, X0) \
	((X1) & (X2) & (X3) ^ (X2) & (X4) & (X5) ^ (X3) & (X4) & (X6) ^ \
	(X1) & (X4) ^ (X2) & (X6) ^ (X3) & (X4) ^ (X3) & (X5) ^ \
	(X3) & (X6) ^ (X4) & (X5) ^ (X4) & (X6) ^ (X0) & (X4) ^(X0))*/
#define F4(X6, X5, X4, X3, X2, X1, X0) \
	(((X4) & (((~(X2)) & (X5)) ^ ((X3) | (X6)) ^ (X1) ^ (X0))) ^ \
	((X3) & (((X1) & (X2)) ^ (X5) ^ (X6))) ^ ((X2) & (X6)) ^ (X0))

/*#define F5(X6, X5, X4, X3, X2, X1, X0) \
	((X1) & (X4) ^ (X2) & (X5) ^ (X3) & (X6) ^ \
	(X0) & (X1) & (X2) & (X3) ^ (X0) & (X5) ^ (X0))*/
#define F5(X6, X5, X4, X3, X2, X1, X0) \
	(((X1) & ((X4) ^ ((X0) & (X2) & (X3)))) ^ \
	(((X2) ^ (X0)) & (X5)) ^ ((X3) & (X6)) ^ (X0))


#define HAVAL_ROTR(v,n)   (((v) >> ((uint32_t)(n))) | ((v) << (32 - (uint32_t)(n))))

#define BYTE_REVERSE(out, in, n) \
{ \
	register int i; \
	for (i = 0; i < n; i++) { \
		((byte*)out)[4*i+3] = ((byte*)in)[4*i+0];\
		((byte*)out)[4*i+2] = ((byte*)in)[4*i+1];\
		((byte*)out)[4*i+1] = ((byte*)in)[4*i+2];\
		((byte*)out)[4*i+0] = ((byte*)in)[4*i+3];\
	} \
}

#define HAVAL_VERSION	1


void Haval::havalTransform3 ( uint32_t E[8], const byte D[128], uint32_t T[8] ) {

#ifndef BIG_ENDIAN_HOST
    uint32_t* W = ( uint32_t* )D;
#else
    uint32_t W[32];
    BYTE_REVERSE( W, D, 32 );
#endif

    /* PASS 1: */

    T[7] = HAVAL_ROTR ( F1 ( E[1], E[0], E[3], E[5], E[6], E[2], E[4] ), 7 ) + HAVAL_ROTR ( E[7], 11 ) + W[ 0];
    T[6] = HAVAL_ROTR ( F1 ( E[0], T[7], E[2], E[4], E[5], E[1], E[3] ), 7 ) + HAVAL_ROTR ( E[6], 11 ) + W[ 1];
    T[5] = HAVAL_ROTR ( F1 ( T[7], T[6], E[1], E[3], E[4], E[0], E[2] ), 7 ) + HAVAL_ROTR ( E[5], 11 ) + W[ 2];
    T[4] = HAVAL_ROTR ( F1 ( T[6], T[5], E[0], E[2], E[3], T[7], E[1] ), 7 ) + HAVAL_ROTR ( E[4], 11 ) + W[ 3];
    T[3] = HAVAL_ROTR ( F1 ( T[5], T[4], T[7], E[1], E[2], T[6], E[0] ), 7 ) + HAVAL_ROTR ( E[3], 11 ) + W[ 4];
    T[2] = HAVAL_ROTR ( F1 ( T[4], T[3], T[6], E[0], E[1], T[5], T[7] ), 7 ) + HAVAL_ROTR ( E[2], 11 ) + W[ 5];
    T[1] = HAVAL_ROTR ( F1 ( T[3], T[2], T[5], T[7], E[0], T[4], T[6] ), 7 ) + HAVAL_ROTR ( E[1], 11 ) + W[ 6];
    T[0] = HAVAL_ROTR ( F1 ( T[2], T[1], T[4], T[6], T[7], T[3], T[5] ), 7 ) + HAVAL_ROTR ( E[0], 11 ) + W[ 7];


    T[7] = HAVAL_ROTR ( F1 ( T[1], T[0], T[3], T[5], T[6], T[2], T[4] ), 7 ) + HAVAL_ROTR ( T[7], 11 ) + W[ 8];
    T[6] = HAVAL_ROTR ( F1 ( T[0], T[7], T[2], T[4], T[5], T[1], T[3] ), 7 ) + HAVAL_ROTR ( T[6], 11 ) + W[ 9];
    T[5] = HAVAL_ROTR ( F1 ( T[7], T[6], T[1], T[3], T[4], T[0], T[2] ), 7 ) + HAVAL_ROTR ( T[5], 11 ) + W[10];
    T[4] = HAVAL_ROTR ( F1 ( T[6], T[5], T[0], T[2], T[3], T[7], T[1] ), 7 ) + HAVAL_ROTR ( T[4], 11 ) + W[11];
    T[3] = HAVAL_ROTR ( F1 ( T[5], T[4], T[7], T[1], T[2], T[6], T[0] ), 7 ) + HAVAL_ROTR ( T[3], 11 ) + W[12];
    T[2] = HAVAL_ROTR ( F1 ( T[4], T[3], T[6], T[0], T[1], T[5], T[7] ), 7 ) + HAVAL_ROTR ( T[2], 11 ) + W[13];
    T[1] = HAVAL_ROTR ( F1 ( T[3], T[2], T[5], T[7], T[0], T[4], T[6] ), 7 ) + HAVAL_ROTR ( T[1], 11 ) + W[14];
    T[0] = HAVAL_ROTR ( F1 ( T[2], T[1], T[4], T[6], T[7], T[3], T[5] ), 7 ) + HAVAL_ROTR ( T[0], 11 ) + W[15];

    T[7] = HAVAL_ROTR ( F1 ( T[1], T[0], T[3], T[5], T[6], T[2], T[4] ), 7 ) + HAVAL_ROTR ( T[7], 11 ) + W[16];
    T[6] = HAVAL_ROTR ( F1 ( T[0], T[7], T[2], T[4], T[5], T[1], T[3] ), 7 ) + HAVAL_ROTR ( T[6], 11 ) + W[17];
    T[5] = HAVAL_ROTR ( F1 ( T[7], T[6], T[1], T[3], T[4], T[0], T[2] ), 7 ) + HAVAL_ROTR ( T[5], 11 ) + W[18];
    T[4] = HAVAL_ROTR ( F1 ( T[6], T[5], T[0], T[2], T[3], T[7], T[1] ), 7 ) + HAVAL_ROTR ( T[4], 11 ) + W[19];
    T[3] = HAVAL_ROTR ( F1 ( T[5], T[4], T[7], T[1], T[2], T[6], T[0] ), 7 ) + HAVAL_ROTR ( T[3], 11 ) + W[20];
    T[2] = HAVAL_ROTR ( F1 ( T[4], T[3], T[6], T[0], T[1], T[5], T[7] ), 7 ) + HAVAL_ROTR ( T[2], 11 ) + W[21];
    T[1] = HAVAL_ROTR ( F1 ( T[3], T[2], T[5], T[7], T[0], T[4], T[6] ), 7 ) + HAVAL_ROTR ( T[1], 11 ) + W[22];
    T[0] = HAVAL_ROTR ( F1 ( T[2], T[1], T[4], T[6], T[7], T[3], T[5] ), 7 ) + HAVAL_ROTR ( T[0], 11 ) + W[23];

    T[7] = HAVAL_ROTR ( F1 ( T[1], T[0], T[3], T[5], T[6], T[2], T[4] ), 7 ) + HAVAL_ROTR ( T[7], 11 ) + W[24];
    T[6] = HAVAL_ROTR ( F1 ( T[0], T[7], T[2], T[4], T[5], T[1], T[3] ), 7 ) + HAVAL_ROTR ( T[6], 11 ) + W[25];
    T[5] = HAVAL_ROTR ( F1 ( T[7], T[6], T[1], T[3], T[4], T[0], T[2] ), 7 ) + HAVAL_ROTR ( T[5], 11 ) + W[26];
    T[4] = HAVAL_ROTR ( F1 ( T[6], T[5], T[0], T[2], T[3], T[7], T[1] ), 7 ) + HAVAL_ROTR ( T[4], 11 ) + W[27];
    T[3] = HAVAL_ROTR ( F1 ( T[5], T[4], T[7], T[1], T[2], T[6], T[0] ), 7 ) + HAVAL_ROTR ( T[3], 11 ) + W[28];
    T[2] = HAVAL_ROTR ( F1 ( T[4], T[3], T[6], T[0], T[1], T[5], T[7] ), 7 ) + HAVAL_ROTR ( T[2], 11 ) + W[29];
    T[1] = HAVAL_ROTR ( F1 ( T[3], T[2], T[5], T[7], T[0], T[4], T[6] ), 7 ) + HAVAL_ROTR ( T[1], 11 ) + W[30];
    T[0] = HAVAL_ROTR ( F1 ( T[2], T[1], T[4], T[6], T[7], T[3], T[5] ), 7 ) + HAVAL_ROTR ( T[0], 11 ) + W[31];

    /* PASS 2: */

    T[7] = HAVAL_ROTR ( F2 ( T[4], T[2], T[1], T[0], T[5], T[3], T[6] ), 7 ) + HAVAL_ROTR ( T[7], 11 ) + W[ 5] + 0x452821E6UL;
    T[6] = HAVAL_ROTR ( F2 ( T[3], T[1], T[0], T[7], T[4], T[2], T[5] ), 7 ) + HAVAL_ROTR ( T[6], 11 ) + W[14] + 0x38D01377UL;
    T[5] = HAVAL_ROTR ( F2 ( T[2], T[0], T[7], T[6], T[3], T[1], T[4] ), 7 ) + HAVAL_ROTR ( T[5], 11 ) + W[26] + 0xBE5466CFUL;
    T[4] = HAVAL_ROTR ( F2 ( T[1], T[7], T[6], T[5], T[2], T[0], T[3] ), 7 ) + HAVAL_ROTR ( T[4], 11 ) + W[18] + 0x34E90C6CUL;
    T[3] = HAVAL_ROTR ( F2 ( T[0], T[6], T[5], T[4], T[1], T[7], T[2] ), 7 ) + HAVAL_ROTR ( T[3], 11 ) + W[11] + 0xC0AC29B7UL;
    T[2] = HAVAL_ROTR ( F2 ( T[7], T[5], T[4], T[3], T[0], T[6], T[1] ), 7 ) + HAVAL_ROTR ( T[2], 11 ) + W[28] + 0xC97C50DDUL;
    T[1] = HAVAL_ROTR ( F2 ( T[6], T[4], T[3], T[2], T[7], T[5], T[0] ), 7 ) + HAVAL_ROTR ( T[1], 11 ) + W[ 7] + 0x3F84D5B5UL;
    T[0] = HAVAL_ROTR ( F2 ( T[5], T[3], T[2], T[1], T[6], T[4], T[7] ), 7 ) + HAVAL_ROTR ( T[0], 11 ) + W[16] + 0xB5470917UL;

    T[7] = HAVAL_ROTR ( F2 ( T[4], T[2], T[1], T[0], T[5], T[3], T[6] ), 7 ) + HAVAL_ROTR ( T[7], 11 ) + W[ 0] + 0x9216D5D9UL;
    T[6] = HAVAL_ROTR ( F2 ( T[3], T[1], T[0], T[7], T[4], T[2], T[5] ), 7 ) + HAVAL_ROTR ( T[6], 11 ) + W[23] + 0x8979FB1BUL;
    T[5] = HAVAL_ROTR ( F2 ( T[2], T[0], T[7], T[6], T[3], T[1], T[4] ), 7 ) + HAVAL_ROTR ( T[5], 11 ) + W[20] + 0xD1310BA6UL;
    T[4] = HAVAL_ROTR ( F2 ( T[1], T[7], T[6], T[5], T[2], T[0], T[3] ), 7 ) + HAVAL_ROTR ( T[4], 11 ) + W[22] + 0x98DFB5ACUL;
    T[3] = HAVAL_ROTR ( F2 ( T[0], T[6], T[5], T[4], T[1], T[7], T[2] ), 7 ) + HAVAL_ROTR ( T[3], 11 ) + W[ 1] + 0x2FFD72DBUL;
    T[2] = HAVAL_ROTR ( F2 ( T[7], T[5], T[4], T[3], T[0], T[6], T[1] ), 7 ) + HAVAL_ROTR ( T[2], 11 ) + W[10] + 0xD01ADFB7UL;
    T[1] = HAVAL_ROTR ( F2 ( T[6], T[4], T[3], T[2], T[7], T[5], T[0] ), 7 ) + HAVAL_ROTR ( T[1], 11 ) + W[ 4] + 0xB8E1AFEDUL;
    T[0] = HAVAL_ROTR ( F2 ( T[5], T[3], T[2], T[1], T[6], T[4], T[7] ), 7 ) + HAVAL_ROTR ( T[0], 11 ) + W[ 8] + 0x6A267E96UL;

    T[7] = HAVAL_ROTR ( F2 ( T[4], T[2], T[1], T[0], T[5], T[3], T[6] ), 7 ) + HAVAL_ROTR ( T[7], 11 ) + W[30] + 0xBA7C9045UL;
    T[6] = HAVAL_ROTR ( F2 ( T[3], T[1], T[0], T[7], T[4], T[2], T[5] ), 7 ) + HAVAL_ROTR ( T[6], 11 ) + W[ 3] + 0xF12C7F99UL;
    T[5] = HAVAL_ROTR ( F2 ( T[2], T[0], T[7], T[6], T[3], T[1], T[4] ), 7 ) + HAVAL_ROTR ( T[5], 11 ) + W[21] + 0x24A19947UL;
    T[4] = HAVAL_ROTR ( F2 ( T[1], T[7], T[6], T[5], T[2], T[0], T[3] ), 7 ) + HAVAL_ROTR ( T[4], 11 ) + W[ 9] + 0xB3916CF7UL;
    T[3] = HAVAL_ROTR ( F2 ( T[0], T[6], T[5], T[4], T[1], T[7], T[2] ), 7 ) + HAVAL_ROTR ( T[3], 11 ) + W[17] + 0x0801F2E2UL;
    T[2] = HAVAL_ROTR ( F2 ( T[7], T[5], T[4], T[3], T[0], T[6], T[1] ), 7 ) + HAVAL_ROTR ( T[2], 11 ) + W[24] + 0x858EFC16UL;
    T[1] = HAVAL_ROTR ( F2 ( T[6], T[4], T[3], T[2], T[7], T[5], T[0] ), 7 ) + HAVAL_ROTR ( T[1], 11 ) + W[29] + 0x636920D8UL;
    T[0] = HAVAL_ROTR ( F2 ( T[5], T[3], T[2], T[1], T[6], T[4], T[7] ), 7 ) + HAVAL_ROTR ( T[0], 11 ) + W[ 6] + 0x71574E69UL;

    T[7] = HAVAL_ROTR ( F2 ( T[4], T[2], T[1], T[0], T[5], T[3], T[6] ), 7 ) + HAVAL_ROTR ( T[7], 11 ) + W[19] + 0xA458FEA3UL;
    T[6] = HAVAL_ROTR ( F2 ( T[3], T[1], T[0], T[7], T[4], T[2], T[5] ), 7 ) + HAVAL_ROTR ( T[6], 11 ) + W[12] + 0xF4933D7EUL;
    T[5] = HAVAL_ROTR ( F2 ( T[2], T[0], T[7], T[6], T[3], T[1], T[4] ), 7 ) + HAVAL_ROTR ( T[5], 11 ) + W[15] + 0x0D95748FUL;
    T[4] = HAVAL_ROTR ( F2 ( T[1], T[7], T[6], T[5], T[2], T[0], T[3] ), 7 ) + HAVAL_ROTR ( T[4], 11 ) + W[13] + 0x728EB658UL;
    T[3] = HAVAL_ROTR ( F2 ( T[0], T[6], T[5], T[4], T[1], T[7], T[2] ), 7 ) + HAVAL_ROTR ( T[3], 11 ) + W[ 2] + 0x718BCD58UL;
    T[2] = HAVAL_ROTR ( F2 ( T[7], T[5], T[4], T[3], T[0], T[6], T[1] ), 7 ) + HAVAL_ROTR ( T[2], 11 ) + W[25] + 0x82154AEEUL;
    T[1] = HAVAL_ROTR ( F2 ( T[6], T[4], T[3], T[2], T[7], T[5], T[0] ), 7 ) + HAVAL_ROTR ( T[1], 11 ) + W[31] + 0x7B54A41DUL;
    T[0] = HAVAL_ROTR ( F2 ( T[5], T[3], T[2], T[1], T[6], T[4], T[7] ), 7 ) + HAVAL_ROTR ( T[0], 11 ) + W[27] + 0xC25A59B5UL;

    /* PASS 3: */

    T[7] = HAVAL_ROTR ( F3 ( T[6], T[1], T[2], T[3], T[4], T[5], T[0] ), 7 ) + HAVAL_ROTR ( T[7], 11 ) + W[19] + 0x9C30D539UL;
    T[6] = HAVAL_ROTR ( F3 ( T[5], T[0], T[1], T[2], T[3], T[4], T[7] ), 7 ) + HAVAL_ROTR ( T[6], 11 ) + W[ 9] + 0x2AF26013UL;
    T[5] = HAVAL_ROTR ( F3 ( T[4], T[7], T[0], T[1], T[2], T[3], T[6] ), 7 ) + HAVAL_ROTR ( T[5], 11 ) + W[ 4] + 0xC5D1B023UL;
    T[4] = HAVAL_ROTR ( F3 ( T[3], T[6], T[7], T[0], T[1], T[2], T[5] ), 7 ) + HAVAL_ROTR ( T[4], 11 ) + W[20] + 0x286085F0UL;
    T[3] = HAVAL_ROTR ( F3 ( T[2], T[5], T[6], T[7], T[0], T[1], T[4] ), 7 ) + HAVAL_ROTR ( T[3], 11 ) + W[28] + 0xCA417918UL;
    T[2] = HAVAL_ROTR ( F3 ( T[1], T[4], T[5], T[6], T[7], T[0], T[3] ), 7 ) + HAVAL_ROTR ( T[2], 11 ) + W[17] + 0xB8DB38EFUL;
    T[1] = HAVAL_ROTR ( F3 ( T[0], T[3], T[4], T[5], T[6], T[7], T[2] ), 7 ) + HAVAL_ROTR ( T[1], 11 ) + W[ 8] + 0x8E79DCB0UL;
    T[0] = HAVAL_ROTR ( F3 ( T[7], T[2], T[3], T[4], T[5], T[6], T[1] ), 7 ) + HAVAL_ROTR ( T[0], 11 ) + W[22] + 0x603A180EUL;

    T[7] = HAVAL_ROTR ( F3 ( T[6], T[1], T[2], T[3], T[4], T[5], T[0] ), 7 ) + HAVAL_ROTR ( T[7], 11 ) + W[29] + 0x6C9E0E8BUL;
    T[6] = HAVAL_ROTR ( F3 ( T[5], T[0], T[1], T[2], T[3], T[4], T[7] ), 7 ) + HAVAL_ROTR ( T[6], 11 ) + W[14] + 0xB01E8A3EUL;
    T[5] = HAVAL_ROTR ( F3 ( T[4], T[7], T[0], T[1], T[2], T[3], T[6] ), 7 ) + HAVAL_ROTR ( T[5], 11 ) + W[25] + 0xD71577C1UL;
    T[4] = HAVAL_ROTR ( F3 ( T[3], T[6], T[7], T[0], T[1], T[2], T[5] ), 7 ) + HAVAL_ROTR ( T[4], 11 ) + W[12] + 0xBD314B27UL;
    T[3] = HAVAL_ROTR ( F3 ( T[2], T[5], T[6], T[7], T[0], T[1], T[4] ), 7 ) + HAVAL_ROTR ( T[3], 11 ) + W[24] + 0x78AF2FDAUL;
    T[2] = HAVAL_ROTR ( F3 ( T[1], T[4], T[5], T[6], T[7], T[0], T[3] ), 7 ) + HAVAL_ROTR ( T[2], 11 ) + W[30] + 0x55605C60UL;
    T[1] = HAVAL_ROTR ( F3 ( T[0], T[3], T[4], T[5], T[6], T[7], T[2] ), 7 ) + HAVAL_ROTR ( T[1], 11 ) + W[16] + 0xE65525F3UL;
    T[0] = HAVAL_ROTR ( F3 ( T[7], T[2], T[3], T[4], T[5], T[6], T[1] ), 7 ) + HAVAL_ROTR ( T[0], 11 ) + W[26] + 0xAA55AB94UL;

    T[7] = HAVAL_ROTR ( F3 ( T[6], T[1], T[2], T[3], T[4], T[5], T[0] ), 7 ) + HAVAL_ROTR ( T[7], 11 ) + W[31] + 0x57489862UL;
    T[6] = HAVAL_ROTR ( F3 ( T[5], T[0], T[1], T[2], T[3], T[4], T[7] ), 7 ) + HAVAL_ROTR ( T[6], 11 ) + W[15] + 0x63E81440UL;
    T[5] = HAVAL_ROTR ( F3 ( T[4], T[7], T[0], T[1], T[2], T[3], T[6] ), 7 ) + HAVAL_ROTR ( T[5], 11 ) + W[ 7] + 0x55CA396AUL;
    T[4] = HAVAL_ROTR ( F3 ( T[3], T[6], T[7], T[0], T[1], T[2], T[5] ), 7 ) + HAVAL_ROTR ( T[4], 11 ) + W[ 3] + 0x2AAB10B6UL;
    T[3] = HAVAL_ROTR ( F3 ( T[2], T[5], T[6], T[7], T[0], T[1], T[4] ), 7 ) + HAVAL_ROTR ( T[3], 11 ) + W[ 1] + 0xB4CC5C34UL;
    T[2] = HAVAL_ROTR ( F3 ( T[1], T[4], T[5], T[6], T[7], T[0], T[3] ), 7 ) + HAVAL_ROTR ( T[2], 11 ) + W[ 0] + 0x1141E8CEUL;
    T[1] = HAVAL_ROTR ( F3 ( T[0], T[3], T[4], T[5], T[6], T[7], T[2] ), 7 ) + HAVAL_ROTR ( T[1], 11 ) + W[18] + 0xA15486AFUL;
    T[0] = HAVAL_ROTR ( F3 ( T[7], T[2], T[3], T[4], T[5], T[6], T[1] ), 7 ) + HAVAL_ROTR ( T[0], 11 ) + W[27] + 0x7C72E993UL;

    E[7] += T[7] = HAVAL_ROTR ( F3 ( T[6], T[1], T[2], T[3], T[4], T[5], T[0] ), 7 ) + HAVAL_ROTR ( T[7], 11 ) + W[13] + 0xB3EE1411UL;
    E[6] += T[6] = HAVAL_ROTR ( F3 ( T[5], T[0], T[1], T[2], T[3], T[4], T[7] ), 7 ) + HAVAL_ROTR ( T[6], 11 ) + W[ 6] + 0x636FBC2AUL;
    E[5] += T[5] = HAVAL_ROTR ( F3 ( T[4], T[7], T[0], T[1], T[2], T[3], T[6] ), 7 ) + HAVAL_ROTR ( T[5], 11 ) + W[21] + 0x2BA9C55DUL;
    E[4] += T[4] = HAVAL_ROTR ( F3 ( T[3], T[6], T[7], T[0], T[1], T[2], T[5] ), 7 ) + HAVAL_ROTR ( T[4], 11 ) + W[10] + 0x741831F6UL;
    E[3] += T[3] = HAVAL_ROTR ( F3 ( T[2], T[5], T[6], T[7], T[0], T[1], T[4] ), 7 ) + HAVAL_ROTR ( T[3], 11 ) + W[23] + 0xCE5C3E16UL;
    E[2] += T[2] = HAVAL_ROTR ( F3 ( T[1], T[4], T[5], T[6], T[7], T[0], T[3] ), 7 ) + HAVAL_ROTR ( T[2], 11 ) + W[11] + 0x9B87931EUL;
    E[1] += T[1] = HAVAL_ROTR ( F3 ( T[0], T[3], T[4], T[5], T[6], T[7], T[2] ), 7 ) + HAVAL_ROTR ( T[1], 11 ) + W[ 5] + 0xAFD6BA33UL;
    E[0] += T[0] = HAVAL_ROTR ( F3 ( T[7], T[2], T[3], T[4], T[5], T[6], T[1] ), 7 ) + HAVAL_ROTR ( T[0], 11 ) + W[ 2] + 0x6C24CF5CUL;

}

void Haval::havalTransform4 ( uint32_t E[8], const byte D[128], uint32_t T[8] ) {
#ifndef BIG_ENDIAN_HOST
    uint32_t* W = ( uint32_t* )D;
#else
    uint32_t W[32];
    BYTE_REVERSE( W, D, 32 );
#endif

    /* PASS 1: */

    T[7] = HAVAL_ROTR ( F1 ( E[2], E[6], E[1], E[4], E[5], E[3], E[0] ), 7 ) + HAVAL_ROTR ( E[7], 11 ) + W[ 0];
    T[6] = HAVAL_ROTR ( F1 ( E[1], E[5], E[0], E[3], E[4], E[2], T[7] ), 7 ) + HAVAL_ROTR ( E[6], 11 ) + W[ 1];
    T[5] = HAVAL_ROTR ( F1 ( E[0], E[4], T[7], E[2], E[3], E[1], T[6] ), 7 ) + HAVAL_ROTR ( E[5], 11 ) + W[ 2];
    T[4] = HAVAL_ROTR ( F1 ( T[7], E[3], T[6], E[1], E[2], E[0], T[5] ), 7 ) + HAVAL_ROTR ( E[4], 11 ) + W[ 3];
    T[3] = HAVAL_ROTR ( F1 ( T[6], E[2], T[5], E[0], E[1], T[7], T[4] ), 7 ) + HAVAL_ROTR ( E[3], 11 ) + W[ 4];
    T[2] = HAVAL_ROTR ( F1 ( T[5], E[1], T[4], T[7], E[0], T[6], T[3] ), 7 ) + HAVAL_ROTR ( E[2], 11 ) + W[ 5];
    T[1] = HAVAL_ROTR ( F1 ( T[4], E[0], T[3], T[6], T[7], T[5], T[2] ), 7 ) + HAVAL_ROTR ( E[1], 11 ) + W[ 6];
    T[0] = HAVAL_ROTR ( F1 ( T[3], T[7], T[2], T[5], T[6], T[4], T[1] ), 7 ) + HAVAL_ROTR ( E[0], 11 ) + W[ 7];

    T[7] = HAVAL_ROTR ( F1 ( T[2], T[6], T[1], T[4], T[5], T[3], T[0] ), 7 ) + HAVAL_ROTR ( T[7], 11 ) + W[ 8];
    T[6] = HAVAL_ROTR ( F1 ( T[1], T[5], T[0], T[3], T[4], T[2], T[7] ), 7 ) + HAVAL_ROTR ( T[6], 11 ) + W[ 9];
    T[5] = HAVAL_ROTR ( F1 ( T[0], T[4], T[7], T[2], T[3], T[1], T[6] ), 7 ) + HAVAL_ROTR ( T[5], 11 ) + W[10];
    T[4] = HAVAL_ROTR ( F1 ( T[7], T[3], T[6], T[1], T[2], T[0], T[5] ), 7 ) + HAVAL_ROTR ( T[4], 11 ) + W[11];
    T[3] = HAVAL_ROTR ( F1 ( T[6], T[2], T[5], T[0], T[1], T[7], T[4] ), 7 ) + HAVAL_ROTR ( T[3], 11 ) + W[12];
    T[2] = HAVAL_ROTR ( F1 ( T[5], T[1], T[4], T[7], T[0], T[6], T[3] ), 7 ) + HAVAL_ROTR ( T[2], 11 ) + W[13];
    T[1] = HAVAL_ROTR ( F1 ( T[4], T[0], T[3], T[6], T[7], T[5], T[2] ), 7 ) + HAVAL_ROTR ( T[1], 11 ) + W[14];
    T[0] = HAVAL_ROTR ( F1 ( T[3], T[7], T[2], T[5], T[6], T[4], T[1] ), 7 ) + HAVAL_ROTR ( T[0], 11 ) + W[15];

    T[7] = HAVAL_ROTR ( F1 ( T[2], T[6], T[1], T[4], T[5], T[3], T[0] ), 7 ) + HAVAL_ROTR ( T[7], 11 ) + W[16];
    T[6] = HAVAL_ROTR ( F1 ( T[1], T[5], T[0], T[3], T[4], T[2], T[7] ), 7 ) + HAVAL_ROTR ( T[6], 11 ) + W[17];
    T[5] = HAVAL_ROTR ( F1 ( T[0], T[4], T[7], T[2], T[3], T[1], T[6] ), 7 ) + HAVAL_ROTR ( T[5], 11 ) + W[18];
    T[4] = HAVAL_ROTR ( F1 ( T[7], T[3], T[6], T[1], T[2], T[0], T[5] ), 7 ) + HAVAL_ROTR ( T[4], 11 ) + W[19];
    T[3] = HAVAL_ROTR ( F1 ( T[6], T[2], T[5], T[0], T[1], T[7], T[4] ), 7 ) + HAVAL_ROTR ( T[3], 11 ) + W[20];
    T[2] = HAVAL_ROTR ( F1 ( T[5], T[1], T[4], T[7], T[0], T[6], T[3] ), 7 ) + HAVAL_ROTR ( T[2], 11 ) + W[21];
    T[1] = HAVAL_ROTR ( F1 ( T[4], T[0], T[3], T[6], T[7], T[5], T[2] ), 7 ) + HAVAL_ROTR ( T[1], 11 ) + W[22];
    T[0] = HAVAL_ROTR ( F1 ( T[3], T[7], T[2], T[5], T[6], T[4], T[1] ), 7 ) + HAVAL_ROTR ( T[0], 11 ) + W[23];

    T[7] = HAVAL_ROTR ( F1 ( T[2], T[6], T[1], T[4], T[5], T[3], T[0] ), 7 ) + HAVAL_ROTR ( T[7], 11 ) + W[24];
    T[6] = HAVAL_ROTR ( F1 ( T[1], T[5], T[0], T[3], T[4], T[2], T[7] ), 7 ) + HAVAL_ROTR ( T[6], 11 ) + W[25];
    T[5] = HAVAL_ROTR ( F1 ( T[0], T[4], T[7], T[2], T[3], T[1], T[6] ), 7 ) + HAVAL_ROTR ( T[5], 11 ) + W[26];
    T[4] = HAVAL_ROTR ( F1 ( T[7], T[3], T[6], T[1], T[2], T[0], T[5] ), 7 ) + HAVAL_ROTR ( T[4], 11 ) + W[27];
    T[3] = HAVAL_ROTR ( F1 ( T[6], T[2], T[5], T[0], T[1], T[7], T[4] ), 7 ) + HAVAL_ROTR ( T[3], 11 ) + W[28];
    T[2] = HAVAL_ROTR ( F1 ( T[5], T[1], T[4], T[7], T[0], T[6], T[3] ), 7 ) + HAVAL_ROTR ( T[2], 11 ) + W[29];
    T[1] = HAVAL_ROTR ( F1 ( T[4], T[0], T[3], T[6], T[7], T[5], T[2] ), 7 ) + HAVAL_ROTR ( T[1], 11 ) + W[30];
    T[0] = HAVAL_ROTR ( F1 ( T[3], T[7], T[2], T[5], T[6], T[4], T[1] ), 7 ) + HAVAL_ROTR ( T[0], 11 ) + W[31];

    /* PASS 2: */

    T[7] = HAVAL_ROTR ( F2 ( T[3], T[5], T[2], T[0], T[1], T[6], T[4] ), 7 ) + HAVAL_ROTR ( T[7], 11 ) + W[ 5] + 0x452821E6UL;
    T[6] = HAVAL_ROTR ( F2 ( T[2], T[4], T[1], T[7], T[0], T[5], T[3] ), 7 ) + HAVAL_ROTR ( T[6], 11 ) + W[14] + 0x38D01377UL;
    T[5] = HAVAL_ROTR ( F2 ( T[1], T[3], T[0], T[6], T[7], T[4], T[2] ), 7 ) + HAVAL_ROTR ( T[5], 11 ) + W[26] + 0xBE5466CFUL;
    T[4] = HAVAL_ROTR ( F2 ( T[0], T[2], T[7], T[5], T[6], T[3], T[1] ), 7 ) + HAVAL_ROTR ( T[4], 11 ) + W[18] + 0x34E90C6CUL;
    T[3] = HAVAL_ROTR ( F2 ( T[7], T[1], T[6], T[4], T[5], T[2], T[0] ), 7 ) + HAVAL_ROTR ( T[3], 11 ) + W[11] + 0xC0AC29B7UL;
    T[2] = HAVAL_ROTR ( F2 ( T[6], T[0], T[5], T[3], T[4], T[1], T[7] ), 7 ) + HAVAL_ROTR ( T[2], 11 ) + W[28] + 0xC97C50DDUL;
    T[1] = HAVAL_ROTR ( F2 ( T[5], T[7], T[4], T[2], T[3], T[0], T[6] ), 7 ) + HAVAL_ROTR ( T[1], 11 ) + W[ 7] + 0x3F84D5B5UL;
    T[0] = HAVAL_ROTR ( F2 ( T[4], T[6], T[3], T[1], T[2], T[7], T[5] ), 7 ) + HAVAL_ROTR ( T[0], 11 ) + W[16] + 0xB5470917UL;

    T[7] = HAVAL_ROTR ( F2 ( T[3], T[5], T[2], T[0], T[1], T[6], T[4] ), 7 ) + HAVAL_ROTR ( T[7], 11 ) + W[ 0] + 0x9216D5D9UL;
    T[6] = HAVAL_ROTR ( F2 ( T[2], T[4], T[1], T[7], T[0], T[5], T[3] ), 7 ) + HAVAL_ROTR ( T[6], 11 ) + W[23] + 0x8979FB1BUL;
    T[5] = HAVAL_ROTR ( F2 ( T[1], T[3], T[0], T[6], T[7], T[4], T[2] ), 7 ) + HAVAL_ROTR ( T[5], 11 ) + W[20] + 0xD1310BA6UL;
    T[4] = HAVAL_ROTR ( F2 ( T[0], T[2], T[7], T[5], T[6], T[3], T[1] ), 7 ) + HAVAL_ROTR ( T[4], 11 ) + W[22] + 0x98DFB5ACUL;
    T[3] = HAVAL_ROTR ( F2 ( T[7], T[1], T[6], T[4], T[5], T[2], T[0] ), 7 ) + HAVAL_ROTR ( T[3], 11 ) + W[ 1] + 0x2FFD72DBUL;
    T[2] = HAVAL_ROTR ( F2 ( T[6], T[0], T[5], T[3], T[4], T[1], T[7] ), 7 ) + HAVAL_ROTR ( T[2], 11 ) + W[10] + 0xD01ADFB7UL;
    T[1] = HAVAL_ROTR ( F2 ( T[5], T[7], T[4], T[2], T[3], T[0], T[6] ), 7 ) + HAVAL_ROTR ( T[1], 11 ) + W[ 4] + 0xB8E1AFEDUL;
    T[0] = HAVAL_ROTR ( F2 ( T[4], T[6], T[3], T[1], T[2], T[7], T[5] ), 7 ) + HAVAL_ROTR ( T[0], 11 ) + W[ 8] + 0x6A267E96UL;

    T[7] = HAVAL_ROTR ( F2 ( T[3], T[5], T[2], T[0], T[1], T[6], T[4] ), 7 ) + HAVAL_ROTR ( T[7], 11 ) + W[30] + 0xBA7C9045UL;
    T[6] = HAVAL_ROTR ( F2 ( T[2], T[4], T[1], T[7], T[0], T[5], T[3] ), 7 ) + HAVAL_ROTR ( T[6], 11 ) + W[ 3] + 0xF12C7F99UL;
    T[5] = HAVAL_ROTR ( F2 ( T[1], T[3], T[0], T[6], T[7], T[4], T[2] ), 7 ) + HAVAL_ROTR ( T[5], 11 ) + W[21] + 0x24A19947UL;
    T[4] = HAVAL_ROTR ( F2 ( T[0], T[2], T[7], T[5], T[6], T[3], T[1] ), 7 ) + HAVAL_ROTR ( T[4], 11 ) + W[ 9] + 0xB3916CF7UL;
    T[3] = HAVAL_ROTR ( F2 ( T[7], T[1], T[6], T[4], T[5], T[2], T[0] ), 7 ) + HAVAL_ROTR ( T[3], 11 ) + W[17] + 0x0801F2E2UL;
    T[2] = HAVAL_ROTR ( F2 ( T[6], T[0], T[5], T[3], T[4], T[1], T[7] ), 7 ) + HAVAL_ROTR ( T[2], 11 ) + W[24] + 0x858EFC16UL;
    T[1] = HAVAL_ROTR ( F2 ( T[5], T[7], T[4], T[2], T[3], T[0], T[6] ), 7 ) + HAVAL_ROTR ( T[1], 11 ) + W[29] + 0x636920D8UL;
    T[0] = HAVAL_ROTR ( F2 ( T[4], T[6], T[3], T[1], T[2], T[7], T[5] ), 7 ) + HAVAL_ROTR ( T[0], 11 ) + W[ 6] + 0x71574E69UL;

    T[7] = HAVAL_ROTR ( F2 ( T[3], T[5], T[2], T[0], T[1], T[6], T[4] ), 7 ) + HAVAL_ROTR ( T[7], 11 ) + W[19] + 0xA458FEA3UL;
    T[6] = HAVAL_ROTR ( F2 ( T[2], T[4], T[1], T[7], T[0], T[5], T[3] ), 7 ) + HAVAL_ROTR ( T[6], 11 ) + W[12] + 0xF4933D7EUL;
    T[5] = HAVAL_ROTR ( F2 ( T[1], T[3], T[0], T[6], T[7], T[4], T[2] ), 7 ) + HAVAL_ROTR ( T[5], 11 ) + W[15] + 0x0D95748FUL;
    T[4] = HAVAL_ROTR ( F2 ( T[0], T[2], T[7], T[5], T[6], T[3], T[1] ), 7 ) + HAVAL_ROTR ( T[4], 11 ) + W[13] + 0x728EB658UL;
    T[3] = HAVAL_ROTR ( F2 ( T[7], T[1], T[6], T[4], T[5], T[2], T[0] ), 7 ) + HAVAL_ROTR ( T[3], 11 ) + W[ 2] + 0x718BCD58UL;
    T[2] = HAVAL_ROTR ( F2 ( T[6], T[0], T[5], T[3], T[4], T[1], T[7] ), 7 ) + HAVAL_ROTR ( T[2], 11 ) + W[25] + 0x82154AEEUL;
    T[1] = HAVAL_ROTR ( F2 ( T[5], T[7], T[4], T[2], T[3], T[0], T[6] ), 7 ) + HAVAL_ROTR ( T[1], 11 ) + W[31] + 0x7B54A41DUL;
    T[0] = HAVAL_ROTR ( F2 ( T[4], T[6], T[3], T[1], T[2], T[7], T[5] ), 7 ) + HAVAL_ROTR ( T[0], 11 ) + W[27] + 0xC25A59B5UL;

    /* PASS 3: */

    T[7] = HAVAL_ROTR ( F3 ( T[1], T[4], T[3], T[6], T[0], T[2], T[5] ), 7 ) + HAVAL_ROTR ( T[7], 11 ) + W[19] + 0x9C30D539UL;
    T[6] = HAVAL_ROTR ( F3 ( T[0], T[3], T[2], T[5], T[7], T[1], T[4] ), 7 ) + HAVAL_ROTR ( T[6], 11 ) + W[ 9] + 0x2AF26013UL;
    T[5] = HAVAL_ROTR ( F3 ( T[7], T[2], T[1], T[4], T[6], T[0], T[3] ), 7 ) + HAVAL_ROTR ( T[5], 11 ) + W[ 4] + 0xC5D1B023UL;
    T[4] = HAVAL_ROTR ( F3 ( T[6], T[1], T[0], T[3], T[5], T[7], T[2] ), 7 ) + HAVAL_ROTR ( T[4], 11 ) + W[20] + 0x286085F0UL;
    T[3] = HAVAL_ROTR ( F3 ( T[5], T[0], T[7], T[2], T[4], T[6], T[1] ), 7 ) + HAVAL_ROTR ( T[3], 11 ) + W[28] + 0xCA417918UL;
    T[2] = HAVAL_ROTR ( F3 ( T[4], T[7], T[6], T[1], T[3], T[5], T[0] ), 7 ) + HAVAL_ROTR ( T[2], 11 ) + W[17] + 0xB8DB38EFUL;
    T[1] = HAVAL_ROTR ( F3 ( T[3], T[6], T[5], T[0], T[2], T[4], T[7] ), 7 ) + HAVAL_ROTR ( T[1], 11 ) + W[ 8] + 0x8E79DCB0UL;
    T[0] = HAVAL_ROTR ( F3 ( T[2], T[5], T[4], T[7], T[1], T[3], T[6] ), 7 ) + HAVAL_ROTR ( T[0], 11 ) + W[22] + 0x603A180EUL;

    T[7] = HAVAL_ROTR ( F3 ( T[1], T[4], T[3], T[6], T[0], T[2], T[5] ), 7 ) + HAVAL_ROTR ( T[7], 11 ) + W[29] + 0x6C9E0E8BUL;
    T[6] = HAVAL_ROTR ( F3 ( T[0], T[3], T[2], T[5], T[7], T[1], T[4] ), 7 ) + HAVAL_ROTR ( T[6], 11 ) + W[14] + 0xB01E8A3EUL;
    T[5] = HAVAL_ROTR ( F3 ( T[7], T[2], T[1], T[4], T[6], T[0], T[3] ), 7 ) + HAVAL_ROTR ( T[5], 11 ) + W[25] + 0xD71577C1UL;
    T[4] = HAVAL_ROTR ( F3 ( T[6], T[1], T[0], T[3], T[5], T[7], T[2] ), 7 ) + HAVAL_ROTR ( T[4], 11 ) + W[12] + 0xBD314B27UL;
    T[3] = HAVAL_ROTR ( F3 ( T[5], T[0], T[7], T[2], T[4], T[6], T[1] ), 7 ) + HAVAL_ROTR ( T[3], 11 ) + W[24] + 0x78AF2FDAUL;
    T[2] = HAVAL_ROTR ( F3 ( T[4], T[7], T[6], T[1], T[3], T[5], T[0] ), 7 ) + HAVAL_ROTR ( T[2], 11 ) + W[30] + 0x55605C60UL;
    T[1] = HAVAL_ROTR ( F3 ( T[3], T[6], T[5], T[0], T[2], T[4], T[7] ), 7 ) + HAVAL_ROTR ( T[1], 11 ) + W[16] + 0xE65525F3UL;
    T[0] = HAVAL_ROTR ( F3 ( T[2], T[5], T[4], T[7], T[1], T[3], T[6] ), 7 ) + HAVAL_ROTR ( T[0], 11 ) + W[26] + 0xAA55AB94UL;

    T[7] = HAVAL_ROTR ( F3 ( T[1], T[4], T[3], T[6], T[0], T[2], T[5] ), 7 ) + HAVAL_ROTR ( T[7], 11 ) + W[31] + 0x57489862UL;
    T[6] = HAVAL_ROTR ( F3 ( T[0], T[3], T[2], T[5], T[7], T[1], T[4] ), 7 ) + HAVAL_ROTR ( T[6], 11 ) + W[15] + 0x63E81440UL;
    T[5] = HAVAL_ROTR ( F3 ( T[7], T[2], T[1], T[4], T[6], T[0], T[3] ), 7 ) + HAVAL_ROTR ( T[5], 11 ) + W[ 7] + 0x55CA396AUL;
    T[4] = HAVAL_ROTR ( F3 ( T[6], T[1], T[0], T[3], T[5], T[7], T[2] ), 7 ) + HAVAL_ROTR ( T[4], 11 ) + W[ 3] + 0x2AAB10B6UL;
    T[3] = HAVAL_ROTR ( F3 ( T[5], T[0], T[7], T[2], T[4], T[6], T[1] ), 7 ) + HAVAL_ROTR ( T[3], 11 ) + W[ 1] + 0xB4CC5C34UL;
    T[2] = HAVAL_ROTR ( F3 ( T[4], T[7], T[6], T[1], T[3], T[5], T[0] ), 7 ) + HAVAL_ROTR ( T[2], 11 ) + W[ 0] + 0x1141E8CEUL;
    T[1] = HAVAL_ROTR ( F3 ( T[3], T[6], T[5], T[0], T[2], T[4], T[7] ), 7 ) + HAVAL_ROTR ( T[1], 11 ) + W[18] + 0xA15486AFUL;
    T[0] = HAVAL_ROTR ( F3 ( T[2], T[5], T[4], T[7], T[1], T[3], T[6] ), 7 ) + HAVAL_ROTR ( T[0], 11 ) + W[27] + 0x7C72E993UL;

    T[7] = HAVAL_ROTR ( F3 ( T[1], T[4], T[3], T[6], T[0], T[2], T[5] ), 7 ) + HAVAL_ROTR ( T[7], 11 ) + W[13] + 0xB3EE1411UL;
    T[6] = HAVAL_ROTR ( F3 ( T[0], T[3], T[2], T[5], T[7], T[1], T[4] ), 7 ) + HAVAL_ROTR ( T[6], 11 ) + W[ 6] + 0x636FBC2AUL;
    T[5] = HAVAL_ROTR ( F3 ( T[7], T[2], T[1], T[4], T[6], T[0], T[3] ), 7 ) + HAVAL_ROTR ( T[5], 11 ) + W[21] + 0x2BA9C55DUL;
    T[4] = HAVAL_ROTR ( F3 ( T[6], T[1], T[0], T[3], T[5], T[7], T[2] ), 7 ) + HAVAL_ROTR ( T[4], 11 ) + W[10] + 0x741831F6UL;
    T[3] = HAVAL_ROTR ( F3 ( T[5], T[0], T[7], T[2], T[4], T[6], T[1] ), 7 ) + HAVAL_ROTR ( T[3], 11 ) + W[23] + 0xCE5C3E16UL;
    T[2] = HAVAL_ROTR ( F3 ( T[4], T[7], T[6], T[1], T[3], T[5], T[0] ), 7 ) + HAVAL_ROTR ( T[2], 11 ) + W[11] + 0x9B87931EUL;
    T[1] = HAVAL_ROTR ( F3 ( T[3], T[6], T[5], T[0], T[2], T[4], T[7] ), 7 ) + HAVAL_ROTR ( T[1], 11 ) + W[ 5] + 0xAFD6BA33UL;
    T[0] = HAVAL_ROTR ( F3 ( T[2], T[5], T[4], T[7], T[1], T[3], T[6] ), 7 ) + HAVAL_ROTR ( T[0], 11 ) + W[ 2] + 0x6C24CF5CUL;

    /* PASS 4: */

    T[7] = HAVAL_ROTR ( F4 ( T[6], T[4], T[0], T[5], T[2], T[1], T[3] ), 7 ) + HAVAL_ROTR ( T[7], 11 ) + W[24] + 0x7A325381UL;
    T[6] = HAVAL_ROTR ( F4 ( T[5], T[3], T[7], T[4], T[1], T[0], T[2] ), 7 ) + HAVAL_ROTR ( T[6], 11 ) + W[ 4] + 0x28958677UL;
    T[5] = HAVAL_ROTR ( F4 ( T[4], T[2], T[6], T[3], T[0], T[7], T[1] ), 7 ) + HAVAL_ROTR ( T[5], 11 ) + W[ 0] + 0x3B8F4898UL;
    T[4] = HAVAL_ROTR ( F4 ( T[3], T[1], T[5], T[2], T[7], T[6], T[0] ), 7 ) + HAVAL_ROTR ( T[4], 11 ) + W[14] + 0x6B4BB9AFUL;
    T[3] = HAVAL_ROTR ( F4 ( T[2], T[0], T[4], T[1], T[6], T[5], T[7] ), 7 ) + HAVAL_ROTR ( T[3], 11 ) + W[ 2] + 0xC4BFE81BUL;
    T[2] = HAVAL_ROTR ( F4 ( T[1], T[7], T[3], T[0], T[5], T[4], T[6] ), 7 ) + HAVAL_ROTR ( T[2], 11 ) + W[ 7] + 0x66282193UL;
    T[1] = HAVAL_ROTR ( F4 ( T[0], T[6], T[2], T[7], T[4], T[3], T[5] ), 7 ) + HAVAL_ROTR ( T[1], 11 ) + W[28] + 0x61D809CCUL;
    T[0] = HAVAL_ROTR ( F4 ( T[7], T[5], T[1], T[6], T[3], T[2], T[4] ), 7 ) + HAVAL_ROTR ( T[0], 11 ) + W[23] + 0xFB21A991UL;

    T[7] = HAVAL_ROTR ( F4 ( T[6], T[4], T[0], T[5], T[2], T[1], T[3] ), 7 ) + HAVAL_ROTR ( T[7], 11 ) + W[26] + 0x487CAC60UL;
    T[6] = HAVAL_ROTR ( F4 ( T[5], T[3], T[7], T[4], T[1], T[0], T[2] ), 7 ) + HAVAL_ROTR ( T[6], 11 ) + W[ 6] + 0x5DEC8032UL;
    T[5] = HAVAL_ROTR ( F4 ( T[4], T[2], T[6], T[3], T[0], T[7], T[1] ), 7 ) + HAVAL_ROTR ( T[5], 11 ) + W[30] + 0xEF845D5DUL;
    T[4] = HAVAL_ROTR ( F4 ( T[3], T[1], T[5], T[2], T[7], T[6], T[0] ), 7 ) + HAVAL_ROTR ( T[4], 11 ) + W[20] + 0xE98575B1UL;
    T[3] = HAVAL_ROTR ( F4 ( T[2], T[0], T[4], T[1], T[6], T[5], T[7] ), 7 ) + HAVAL_ROTR ( T[3], 11 ) + W[18] + 0xDC262302UL;
    T[2] = HAVAL_ROTR ( F4 ( T[1], T[7], T[3], T[0], T[5], T[4], T[6] ), 7 ) + HAVAL_ROTR ( T[2], 11 ) + W[25] + 0xEB651B88UL;
    T[1] = HAVAL_ROTR ( F4 ( T[0], T[6], T[2], T[7], T[4], T[3], T[5] ), 7 ) + HAVAL_ROTR ( T[1], 11 ) + W[19] + 0x23893E81UL;
    T[0] = HAVAL_ROTR ( F4 ( T[7], T[5], T[1], T[6], T[3], T[2], T[4] ), 7 ) + HAVAL_ROTR ( T[0], 11 ) + W[ 3] + 0xD396ACC5UL;

    T[7] = HAVAL_ROTR ( F4 ( T[6], T[4], T[0], T[5], T[2], T[1], T[3] ), 7 ) + HAVAL_ROTR ( T[7], 11 ) + W[22] + 0x0F6D6FF3UL;
    T[6] = HAVAL_ROTR ( F4 ( T[5], T[3], T[7], T[4], T[1], T[0], T[2] ), 7 ) + HAVAL_ROTR ( T[6], 11 ) + W[11] + 0x83F44239UL;
    T[5] = HAVAL_ROTR ( F4 ( T[4], T[2], T[6], T[3], T[0], T[7], T[1] ), 7 ) + HAVAL_ROTR ( T[5], 11 ) + W[31] + 0x2E0B4482UL;
    T[4] = HAVAL_ROTR ( F4 ( T[3], T[1], T[5], T[2], T[7], T[6], T[0] ), 7 ) + HAVAL_ROTR ( T[4], 11 ) + W[21] + 0xA4842004UL;
    T[3] = HAVAL_ROTR ( F4 ( T[2], T[0], T[4], T[1], T[6], T[5], T[7] ), 7 ) + HAVAL_ROTR ( T[3], 11 ) + W[ 8] + 0x69C8F04AUL;
    T[2] = HAVAL_ROTR ( F4 ( T[1], T[7], T[3], T[0], T[5], T[4], T[6] ), 7 ) + HAVAL_ROTR ( T[2], 11 ) + W[27] + 0x9E1F9B5EUL;
    T[1] = HAVAL_ROTR ( F4 ( T[0], T[6], T[2], T[7], T[4], T[3], T[5] ), 7 ) + HAVAL_ROTR ( T[1], 11 ) + W[12] + 0x21C66842UL;
    T[0] = HAVAL_ROTR ( F4 ( T[7], T[5], T[1], T[6], T[3], T[2], T[4] ), 7 ) + HAVAL_ROTR ( T[0], 11 ) + W[ 9] + 0xF6E96C9AUL;

    E[7] += T[7] = HAVAL_ROTR ( F4 ( T[6], T[4], T[0], T[5], T[2], T[1], T[3] ), 7 ) + HAVAL_ROTR ( T[7], 11 ) + W[ 1] + 0x670C9C61UL;
    E[6] += T[6] = HAVAL_ROTR ( F4 ( T[5], T[3], T[7], T[4], T[1], T[0], T[2] ), 7 ) + HAVAL_ROTR ( T[6], 11 ) + W[29] + 0xABD388F0UL;
    E[5] += T[5] = HAVAL_ROTR ( F4 ( T[4], T[2], T[6], T[3], T[0], T[7], T[1] ), 7 ) + HAVAL_ROTR ( T[5], 11 ) + W[ 5] + 0x6A51A0D2UL;
    E[4] += T[4] = HAVAL_ROTR ( F4 ( T[3], T[1], T[5], T[2], T[7], T[6], T[0] ), 7 ) + HAVAL_ROTR ( T[4], 11 ) + W[15] + 0xD8542F68UL;
    E[3] += T[3] = HAVAL_ROTR ( F4 ( T[2], T[0], T[4], T[1], T[6], T[5], T[7] ), 7 ) + HAVAL_ROTR ( T[3], 11 ) + W[17] + 0x960FA728UL;
    E[2] += T[2] = HAVAL_ROTR ( F4 ( T[1], T[7], T[3], T[0], T[5], T[4], T[6] ), 7 ) + HAVAL_ROTR ( T[2], 11 ) + W[10] + 0xAB5133A3UL;
    E[1] += T[1] = HAVAL_ROTR ( F4 ( T[0], T[6], T[2], T[7], T[4], T[3], T[5] ), 7 ) + HAVAL_ROTR ( T[1], 11 ) + W[16] + 0x6EEF0B6CUL;
    E[0] += T[0] = HAVAL_ROTR ( F4 ( T[7], T[5], T[1], T[6], T[3], T[2], T[4] ), 7 ) + HAVAL_ROTR ( T[0], 11 ) + W[13] + 0x137A3BE4UL;

}

void Haval::havalTransform5 ( uint32_t E[8], const byte D[128], uint32_t T[8] ) {
#ifndef BIG_ENDIAN_HOST
    uint32_t* W = ( uint32_t* )D;
#else
    uint32_t W[32];
    BYTE_REVERSE( W, D, 32 );
#endif

    /* PASS 1: */

    T[7] = HAVAL_ROTR ( F1 ( E[3], E[4], E[1], E[0], E[5], E[2], E[6] ), 7 ) + HAVAL_ROTR ( E[7], 11 ) + W[ 0];
    T[6] = HAVAL_ROTR ( F1 ( E[2], E[3], E[0], T[7], E[4], E[1], E[5] ), 7 ) + HAVAL_ROTR ( E[6], 11 ) + W[ 1];
    T[5] = HAVAL_ROTR ( F1 ( E[1], E[2], T[7], T[6], E[3], E[0], E[4] ), 7 ) + HAVAL_ROTR ( E[5], 11 ) + W[ 2];
    T[4] = HAVAL_ROTR ( F1 ( E[0], E[1], T[6], T[5], E[2], T[7], E[3] ), 7 ) + HAVAL_ROTR ( E[4], 11 ) + W[ 3];
    T[3] = HAVAL_ROTR ( F1 ( T[7], E[0], T[5], T[4], E[1], T[6], E[2] ), 7 ) + HAVAL_ROTR ( E[3], 11 ) + W[ 4];
    T[2] = HAVAL_ROTR ( F1 ( T[6], T[7], T[4], T[3], E[0], T[5], E[1] ), 7 ) + HAVAL_ROTR ( E[2], 11 ) + W[ 5];
    T[1] = HAVAL_ROTR ( F1 ( T[5], T[6], T[3], T[2], T[7], T[4], E[0] ), 7 ) + HAVAL_ROTR ( E[1], 11 ) + W[ 6];
    T[0] = HAVAL_ROTR ( F1 ( T[4], T[5], T[2], T[1], T[6], T[3], T[7] ), 7 ) + HAVAL_ROTR ( E[0], 11 ) + W[ 7];

    T[7] = HAVAL_ROTR ( F1 ( T[3], T[4], T[1], T[0], T[5], T[2], T[6] ), 7 ) + HAVAL_ROTR ( T[7], 11 ) + W[ 8];
    T[6] = HAVAL_ROTR ( F1 ( T[2], T[3], T[0], T[7], T[4], T[1], T[5] ), 7 ) + HAVAL_ROTR ( T[6], 11 ) + W[ 9];
    T[5] = HAVAL_ROTR ( F1 ( T[1], T[2], T[7], T[6], T[3], T[0], T[4] ), 7 ) + HAVAL_ROTR ( T[5], 11 ) + W[10];
    T[4] = HAVAL_ROTR ( F1 ( T[0], T[1], T[6], T[5], T[2], T[7], T[3] ), 7 ) + HAVAL_ROTR ( T[4], 11 ) + W[11];
    T[3] = HAVAL_ROTR ( F1 ( T[7], T[0], T[5], T[4], T[1], T[6], T[2] ), 7 ) + HAVAL_ROTR ( T[3], 11 ) + W[12];
    T[2] = HAVAL_ROTR ( F1 ( T[6], T[7], T[4], T[3], T[0], T[5], T[1] ), 7 ) + HAVAL_ROTR ( T[2], 11 ) + W[13];
    T[1] = HAVAL_ROTR ( F1 ( T[5], T[6], T[3], T[2], T[7], T[4], T[0] ), 7 ) + HAVAL_ROTR ( T[1], 11 ) + W[14];
    T[0] = HAVAL_ROTR ( F1 ( T[4], T[5], T[2], T[1], T[6], T[3], T[7] ), 7 ) + HAVAL_ROTR ( T[0], 11 ) + W[15];

    T[7] = HAVAL_ROTR ( F1 ( T[3], T[4], T[1], T[0], T[5], T[2], T[6] ), 7 ) + HAVAL_ROTR ( T[7], 11 ) + W[16];
    T[6] = HAVAL_ROTR ( F1 ( T[2], T[3], T[0], T[7], T[4], T[1], T[5] ), 7 ) + HAVAL_ROTR ( T[6], 11 ) + W[17];
    T[5] = HAVAL_ROTR ( F1 ( T[1], T[2], T[7], T[6], T[3], T[0], T[4] ), 7 ) + HAVAL_ROTR ( T[5], 11 ) + W[18];
    T[4] = HAVAL_ROTR ( F1 ( T[0], T[1], T[6], T[5], T[2], T[7], T[3] ), 7 ) + HAVAL_ROTR ( T[4], 11 ) + W[19];
    T[3] = HAVAL_ROTR ( F1 ( T[7], T[0], T[5], T[4], T[1], T[6], T[2] ), 7 ) + HAVAL_ROTR ( T[3], 11 ) + W[20];
    T[2] = HAVAL_ROTR ( F1 ( T[6], T[7], T[4], T[3], T[0], T[5], T[1] ), 7 ) + HAVAL_ROTR ( T[2], 11 ) + W[21];
    T[1] = HAVAL_ROTR ( F1 ( T[5], T[6], T[3], T[2], T[7], T[4], T[0] ), 7 ) + HAVAL_ROTR ( T[1], 11 ) + W[22];
    T[0] = HAVAL_ROTR ( F1 ( T[4], T[5], T[2], T[1], T[6], T[3], T[7] ), 7 ) + HAVAL_ROTR ( T[0], 11 ) + W[23];

    T[7] = HAVAL_ROTR ( F1 ( T[3], T[4], T[1], T[0], T[5], T[2], T[6] ), 7 ) + HAVAL_ROTR ( T[7], 11 ) + W[24];
    T[6] = HAVAL_ROTR ( F1 ( T[2], T[3], T[0], T[7], T[4], T[1], T[5] ), 7 ) + HAVAL_ROTR ( T[6], 11 ) + W[25];
    T[5] = HAVAL_ROTR ( F1 ( T[1], T[2], T[7], T[6], T[3], T[0], T[4] ), 7 ) + HAVAL_ROTR ( T[5], 11 ) + W[26];
    T[4] = HAVAL_ROTR ( F1 ( T[0], T[1], T[6], T[5], T[2], T[7], T[3] ), 7 ) + HAVAL_ROTR ( T[4], 11 ) + W[27];
    T[3] = HAVAL_ROTR ( F1 ( T[7], T[0], T[5], T[4], T[1], T[6], T[2] ), 7 ) + HAVAL_ROTR ( T[3], 11 ) + W[28];
    T[2] = HAVAL_ROTR ( F1 ( T[6], T[7], T[4], T[3], T[0], T[5], T[1] ), 7 ) + HAVAL_ROTR ( T[2], 11 ) + W[29];
    T[1] = HAVAL_ROTR ( F1 ( T[5], T[6], T[3], T[2], T[7], T[4], T[0] ), 7 ) + HAVAL_ROTR ( T[1], 11 ) + W[30];
    T[0] = HAVAL_ROTR ( F1 ( T[4], T[5], T[2], T[1], T[6], T[3], T[7] ), 7 ) + HAVAL_ROTR ( T[0], 11 ) + W[31];

    /* PASS 2: */

    T[7] = HAVAL_ROTR ( F2 ( T[6], T[2], T[1], T[0], T[3], T[4], T[5] ), 7 ) + HAVAL_ROTR ( T[7], 11 ) + W[ 5] + 0x452821E6UL;
    T[6] = HAVAL_ROTR ( F2 ( T[5], T[1], T[0], T[7], T[2], T[3], T[4] ), 7 ) + HAVAL_ROTR ( T[6], 11 ) + W[14] + 0x38D01377UL;
    T[5] = HAVAL_ROTR ( F2 ( T[4], T[0], T[7], T[6], T[1], T[2], T[3] ), 7 ) + HAVAL_ROTR ( T[5], 11 ) + W[26] + 0xBE5466CFUL;
    T[4] = HAVAL_ROTR ( F2 ( T[3], T[7], T[6], T[5], T[0], T[1], T[2] ), 7 ) + HAVAL_ROTR ( T[4], 11 ) + W[18] + 0x34E90C6CUL;
    T[3] = HAVAL_ROTR ( F2 ( T[2], T[6], T[5], T[4], T[7], T[0], T[1] ), 7 ) + HAVAL_ROTR ( T[3], 11 ) + W[11] + 0xC0AC29B7UL;
    T[2] = HAVAL_ROTR ( F2 ( T[1], T[5], T[4], T[3], T[6], T[7], T[0] ), 7 ) + HAVAL_ROTR ( T[2], 11 ) + W[28] + 0xC97C50DDUL;
    T[1] = HAVAL_ROTR ( F2 ( T[0], T[4], T[3], T[2], T[5], T[6], T[7] ), 7 ) + HAVAL_ROTR ( T[1], 11 ) + W[ 7] + 0x3F84D5B5UL;
    T[0] = HAVAL_ROTR ( F2 ( T[7], T[3], T[2], T[1], T[4], T[5], T[6] ), 7 ) + HAVAL_ROTR ( T[0], 11 ) + W[16] + 0xB5470917UL;

    T[7] = HAVAL_ROTR ( F2 ( T[6], T[2], T[1], T[0], T[3], T[4], T[5] ), 7 ) + HAVAL_ROTR ( T[7], 11 ) + W[ 0] + 0x9216D5D9UL;
    T[6] = HAVAL_ROTR ( F2 ( T[5], T[1], T[0], T[7], T[2], T[3], T[4] ), 7 ) + HAVAL_ROTR ( T[6], 11 ) + W[23] + 0x8979FB1BUL;
    T[5] = HAVAL_ROTR ( F2 ( T[4], T[0], T[7], T[6], T[1], T[2], T[3] ), 7 ) + HAVAL_ROTR ( T[5], 11 ) + W[20] + 0xD1310BA6UL;
    T[4] = HAVAL_ROTR ( F2 ( T[3], T[7], T[6], T[5], T[0], T[1], T[2] ), 7 ) + HAVAL_ROTR ( T[4], 11 ) + W[22] + 0x98DFB5ACUL;
    T[3] = HAVAL_ROTR ( F2 ( T[2], T[6], T[5], T[4], T[7], T[0], T[1] ), 7 ) + HAVAL_ROTR ( T[3], 11 ) + W[ 1] + 0x2FFD72DBUL;
    T[2] = HAVAL_ROTR ( F2 ( T[1], T[5], T[4], T[3], T[6], T[7], T[0] ), 7 ) + HAVAL_ROTR ( T[2], 11 ) + W[10] + 0xD01ADFB7UL;
    T[1] = HAVAL_ROTR ( F2 ( T[0], T[4], T[3], T[2], T[5], T[6], T[7] ), 7 ) + HAVAL_ROTR ( T[1], 11 ) + W[ 4] + 0xB8E1AFEDUL;
    T[0] = HAVAL_ROTR ( F2 ( T[7], T[3], T[2], T[1], T[4], T[5], T[6] ), 7 ) + HAVAL_ROTR ( T[0], 11 ) + W[ 8] + 0x6A267E96UL;

    T[7] = HAVAL_ROTR ( F2 ( T[6], T[2], T[1], T[0], T[3], T[4], T[5] ), 7 ) + HAVAL_ROTR ( T[7], 11 ) + W[30] + 0xBA7C9045UL;
    T[6] = HAVAL_ROTR ( F2 ( T[5], T[1], T[0], T[7], T[2], T[3], T[4] ), 7 ) + HAVAL_ROTR ( T[6], 11 ) + W[ 3] + 0xF12C7F99UL;
    T[5] = HAVAL_ROTR ( F2 ( T[4], T[0], T[7], T[6], T[1], T[2], T[3] ), 7 ) + HAVAL_ROTR ( T[5], 11 ) + W[21] + 0x24A19947UL;
    T[4] = HAVAL_ROTR ( F2 ( T[3], T[7], T[6], T[5], T[0], T[1], T[2] ), 7 ) + HAVAL_ROTR ( T[4], 11 ) + W[ 9] + 0xB3916CF7UL;
    T[3] = HAVAL_ROTR ( F2 ( T[2], T[6], T[5], T[4], T[7], T[0], T[1] ), 7 ) + HAVAL_ROTR ( T[3], 11 ) + W[17] + 0x0801F2E2UL;
    T[2] = HAVAL_ROTR ( F2 ( T[1], T[5], T[4], T[3], T[6], T[7], T[0] ), 7 ) + HAVAL_ROTR ( T[2], 11 ) + W[24] + 0x858EFC16UL;
    T[1] = HAVAL_ROTR ( F2 ( T[0], T[4], T[3], T[2], T[5], T[6], T[7] ), 7 ) + HAVAL_ROTR ( T[1], 11 ) + W[29] + 0x636920D8UL;
    T[0] = HAVAL_ROTR ( F2 ( T[7], T[3], T[2], T[1], T[4], T[5], T[6] ), 7 ) + HAVAL_ROTR ( T[0], 11 ) + W[ 6] + 0x71574E69UL;

    T[7] = HAVAL_ROTR ( F2 ( T[6], T[2], T[1], T[0], T[3], T[4], T[5] ), 7 ) + HAVAL_ROTR ( T[7], 11 ) + W[19] + 0xA458FEA3UL;
    T[6] = HAVAL_ROTR ( F2 ( T[5], T[1], T[0], T[7], T[2], T[3], T[4] ), 7 ) + HAVAL_ROTR ( T[6], 11 ) + W[12] + 0xF4933D7EUL;
    T[5] = HAVAL_ROTR ( F2 ( T[4], T[0], T[7], T[6], T[1], T[2], T[3] ), 7 ) + HAVAL_ROTR ( T[5], 11 ) + W[15] + 0x0D95748FUL;
    T[4] = HAVAL_ROTR ( F2 ( T[3], T[7], T[6], T[5], T[0], T[1], T[2] ), 7 ) + HAVAL_ROTR ( T[4], 11 ) + W[13] + 0x728EB658UL;
    T[3] = HAVAL_ROTR ( F2 ( T[2], T[6], T[5], T[4], T[7], T[0], T[1] ), 7 ) + HAVAL_ROTR ( T[3], 11 ) + W[ 2] + 0x718BCD58UL;
    T[2] = HAVAL_ROTR ( F2 ( T[1], T[5], T[4], T[3], T[6], T[7], T[0] ), 7 ) + HAVAL_ROTR ( T[2], 11 ) + W[25] + 0x82154AEEUL;
    T[1] = HAVAL_ROTR ( F2 ( T[0], T[4], T[3], T[2], T[5], T[6], T[7] ), 7 ) + HAVAL_ROTR ( T[1], 11 ) + W[31] + 0x7B54A41DUL;
    T[0] = HAVAL_ROTR ( F2 ( T[7], T[3], T[2], T[1], T[4], T[5], T[6] ), 7 ) + HAVAL_ROTR ( T[0], 11 ) + W[27] + 0xC25A59B5UL;

    /* PASS 3: */

    T[7] = HAVAL_ROTR ( F3 ( T[2], T[6], T[0], T[4], T[3], T[1], T[5] ), 7 ) + HAVAL_ROTR ( T[7], 11 ) + W[19] + 0x9C30D539UL;
    T[6] = HAVAL_ROTR ( F3 ( T[1], T[5], T[7], T[3], T[2], T[0], T[4] ), 7 ) + HAVAL_ROTR ( T[6], 11 ) + W[ 9] + 0x2AF26013UL;
    T[5] = HAVAL_ROTR ( F3 ( T[0], T[4], T[6], T[2], T[1], T[7], T[3] ), 7 ) + HAVAL_ROTR ( T[5], 11 ) + W[ 4] + 0xC5D1B023UL;
    T[4] = HAVAL_ROTR ( F3 ( T[7], T[3], T[5], T[1], T[0], T[6], T[2] ), 7 ) + HAVAL_ROTR ( T[4], 11 ) + W[20] + 0x286085F0UL;
    T[3] = HAVAL_ROTR ( F3 ( T[6], T[2], T[4], T[0], T[7], T[5], T[1] ), 7 ) + HAVAL_ROTR ( T[3], 11 ) + W[28] + 0xCA417918UL;
    T[2] = HAVAL_ROTR ( F3 ( T[5], T[1], T[3], T[7], T[6], T[4], T[0] ), 7 ) + HAVAL_ROTR ( T[2], 11 ) + W[17] + 0xB8DB38EFUL;
    T[1] = HAVAL_ROTR ( F3 ( T[4], T[0], T[2], T[6], T[5], T[3], T[7] ), 7 ) + HAVAL_ROTR ( T[1], 11 ) + W[ 8] + 0x8E79DCB0UL;
    T[0] = HAVAL_ROTR ( F3 ( T[3], T[7], T[1], T[5], T[4], T[2], T[6] ), 7 ) + HAVAL_ROTR ( T[0], 11 ) + W[22] + 0x603A180EUL;

    T[7] = HAVAL_ROTR ( F3 ( T[2], T[6], T[0], T[4], T[3], T[1], T[5] ), 7 ) + HAVAL_ROTR ( T[7], 11 ) + W[29] + 0x6C9E0E8BUL;
    T[6] = HAVAL_ROTR ( F3 ( T[1], T[5], T[7], T[3], T[2], T[0], T[4] ), 7 ) + HAVAL_ROTR ( T[6], 11 ) + W[14] + 0xB01E8A3EUL;
    T[5] = HAVAL_ROTR ( F3 ( T[0], T[4], T[6], T[2], T[1], T[7], T[3] ), 7 ) + HAVAL_ROTR ( T[5], 11 ) + W[25] + 0xD71577C1UL;
    T[4] = HAVAL_ROTR ( F3 ( T[7], T[3], T[5], T[1], T[0], T[6], T[2] ), 7 ) + HAVAL_ROTR ( T[4], 11 ) + W[12] + 0xBD314B27UL;
    T[3] = HAVAL_ROTR ( F3 ( T[6], T[2], T[4], T[0], T[7], T[5], T[1] ), 7 ) + HAVAL_ROTR ( T[3], 11 ) + W[24] + 0x78AF2FDAUL;
    T[2] = HAVAL_ROTR ( F3 ( T[5], T[1], T[3], T[7], T[6], T[4], T[0] ), 7 ) + HAVAL_ROTR ( T[2], 11 ) + W[30] + 0x55605C60UL;
    T[1] = HAVAL_ROTR ( F3 ( T[4], T[0], T[2], T[6], T[5], T[3], T[7] ), 7 ) + HAVAL_ROTR ( T[1], 11 ) + W[16] + 0xE65525F3UL;
    T[0] = HAVAL_ROTR ( F3 ( T[3], T[7], T[1], T[5], T[4], T[2], T[6] ), 7 ) + HAVAL_ROTR ( T[0], 11 ) + W[26] + 0xAA55AB94UL;

    T[7] = HAVAL_ROTR ( F3 ( T[2], T[6], T[0], T[4], T[3], T[1], T[5] ), 7 ) + HAVAL_ROTR ( T[7], 11 ) + W[31] + 0x57489862UL;
    T[6] = HAVAL_ROTR ( F3 ( T[1], T[5], T[7], T[3], T[2], T[0], T[4] ), 7 ) + HAVAL_ROTR ( T[6], 11 ) + W[15] + 0x63E81440UL;
    T[5] = HAVAL_ROTR ( F3 ( T[0], T[4], T[6], T[2], T[1], T[7], T[3] ), 7 ) + HAVAL_ROTR ( T[5], 11 ) + W[ 7] + 0x55CA396AUL;
    T[4] = HAVAL_ROTR ( F3 ( T[7], T[3], T[5], T[1], T[0], T[6], T[2] ), 7 ) + HAVAL_ROTR ( T[4], 11 ) + W[ 3] + 0x2AAB10B6UL;
    T[3] = HAVAL_ROTR ( F3 ( T[6], T[2], T[4], T[0], T[7], T[5], T[1] ), 7 ) + HAVAL_ROTR ( T[3], 11 ) + W[ 1] + 0xB4CC5C34UL;
    T[2] = HAVAL_ROTR ( F3 ( T[5], T[1], T[3], T[7], T[6], T[4], T[0] ), 7 ) + HAVAL_ROTR ( T[2], 11 ) + W[ 0] + 0x1141E8CEUL;
    T[1] = HAVAL_ROTR ( F3 ( T[4], T[0], T[2], T[6], T[5], T[3], T[7] ), 7 ) + HAVAL_ROTR ( T[1], 11 ) + W[18] + 0xA15486AFUL;
    T[0] = HAVAL_ROTR ( F3 ( T[3], T[7], T[1], T[5], T[4], T[2], T[6] ), 7 ) + HAVAL_ROTR ( T[0], 11 ) + W[27] + 0x7C72E993UL;

    T[7] = HAVAL_ROTR ( F3 ( T[2], T[6], T[0], T[4], T[3], T[1], T[5] ), 7 ) + HAVAL_ROTR ( T[7], 11 ) + W[13] + 0xB3EE1411UL;
    T[6] = HAVAL_ROTR ( F3 ( T[1], T[5], T[7], T[3], T[2], T[0], T[4] ), 7 ) + HAVAL_ROTR ( T[6], 11 ) + W[ 6] + 0x636FBC2AUL;
    T[5] = HAVAL_ROTR ( F3 ( T[0], T[4], T[6], T[2], T[1], T[7], T[3] ), 7 ) + HAVAL_ROTR ( T[5], 11 ) + W[21] + 0x2BA9C55DUL;
    T[4] = HAVAL_ROTR ( F3 ( T[7], T[3], T[5], T[1], T[0], T[6], T[2] ), 7 ) + HAVAL_ROTR ( T[4], 11 ) + W[10] + 0x741831F6UL;
    T[3] = HAVAL_ROTR ( F3 ( T[6], T[2], T[4], T[0], T[7], T[5], T[1] ), 7 ) + HAVAL_ROTR ( T[3], 11 ) + W[23] + 0xCE5C3E16UL;
    T[2] = HAVAL_ROTR ( F3 ( T[5], T[1], T[3], T[7], T[6], T[4], T[0] ), 7 ) + HAVAL_ROTR ( T[2], 11 ) + W[11] + 0x9B87931EUL;
    T[1] = HAVAL_ROTR ( F3 ( T[4], T[0], T[2], T[6], T[5], T[3], T[7] ), 7 ) + HAVAL_ROTR ( T[1], 11 ) + W[ 5] + 0xAFD6BA33UL;
    T[0] = HAVAL_ROTR ( F3 ( T[3], T[7], T[1], T[5], T[4], T[2], T[6] ), 7 ) + HAVAL_ROTR ( T[0], 11 ) + W[ 2] + 0x6C24CF5CUL;

    /* PASS 4: */

    T[7] = HAVAL_ROTR ( F4 ( T[1], T[5], T[3], T[2], T[0], T[4], T[6] ), 7 ) + HAVAL_ROTR ( T[7], 11 ) + W[24] + 0x7A325381UL;
    T[6] = HAVAL_ROTR ( F4 ( T[0], T[4], T[2], T[1], T[7], T[3], T[5] ), 7 ) + HAVAL_ROTR ( T[6], 11 ) + W[ 4] + 0x28958677UL;
    T[5] = HAVAL_ROTR ( F4 ( T[7], T[3], T[1], T[0], T[6], T[2], T[4] ), 7 ) + HAVAL_ROTR ( T[5], 11 ) + W[ 0] + 0x3B8F4898UL;
    T[4] = HAVAL_ROTR ( F4 ( T[6], T[2], T[0], T[7], T[5], T[1], T[3] ), 7 ) + HAVAL_ROTR ( T[4], 11 ) + W[14] + 0x6B4BB9AFUL;
    T[3] = HAVAL_ROTR ( F4 ( T[5], T[1], T[7], T[6], T[4], T[0], T[2] ), 7 ) + HAVAL_ROTR ( T[3], 11 ) + W[ 2] + 0xC4BFE81BUL;
    T[2] = HAVAL_ROTR ( F4 ( T[4], T[0], T[6], T[5], T[3], T[7], T[1] ), 7 ) + HAVAL_ROTR ( T[2], 11 ) + W[ 7] + 0x66282193UL;
    T[1] = HAVAL_ROTR ( F4 ( T[3], T[7], T[5], T[4], T[2], T[6], T[0] ), 7 ) + HAVAL_ROTR ( T[1], 11 ) + W[28] + 0x61D809CCUL;
    T[0] = HAVAL_ROTR ( F4 ( T[2], T[6], T[4], T[3], T[1], T[5], T[7] ), 7 ) + HAVAL_ROTR ( T[0], 11 ) + W[23] + 0xFB21A991UL;

    T[7] = HAVAL_ROTR ( F4 ( T[1], T[5], T[3], T[2], T[0], T[4], T[6] ), 7 ) + HAVAL_ROTR ( T[7], 11 ) + W[26] + 0x487CAC60UL;
    T[6] = HAVAL_ROTR ( F4 ( T[0], T[4], T[2], T[1], T[7], T[3], T[5] ), 7 ) + HAVAL_ROTR ( T[6], 11 ) + W[ 6] + 0x5DEC8032UL;
    T[5] = HAVAL_ROTR ( F4 ( T[7], T[3], T[1], T[0], T[6], T[2], T[4] ), 7 ) + HAVAL_ROTR ( T[5], 11 ) + W[30] + 0xEF845D5DUL;
    T[4] = HAVAL_ROTR ( F4 ( T[6], T[2], T[0], T[7], T[5], T[1], T[3] ), 7 ) + HAVAL_ROTR ( T[4], 11 ) + W[20] + 0xE98575B1UL;
    T[3] = HAVAL_ROTR ( F4 ( T[5], T[1], T[7], T[6], T[4], T[0], T[2] ), 7 ) + HAVAL_ROTR ( T[3], 11 ) + W[18] + 0xDC262302UL;
    T[2] = HAVAL_ROTR ( F4 ( T[4], T[0], T[6], T[5], T[3], T[7], T[1] ), 7 ) + HAVAL_ROTR ( T[2], 11 ) + W[25] + 0xEB651B88UL;
    T[1] = HAVAL_ROTR ( F4 ( T[3], T[7], T[5], T[4], T[2], T[6], T[0] ), 7 ) + HAVAL_ROTR ( T[1], 11 ) + W[19] + 0x23893E81UL;
    T[0] = HAVAL_ROTR ( F4 ( T[2], T[6], T[4], T[3], T[1], T[5], T[7] ), 7 ) + HAVAL_ROTR ( T[0], 11 ) + W[ 3] + 0xD396ACC5UL;

    T[7] = HAVAL_ROTR ( F4 ( T[1], T[5], T[3], T[2], T[0], T[4], T[6] ), 7 ) + HAVAL_ROTR ( T[7], 11 ) + W[22] + 0x0F6D6FF3UL;
    T[6] = HAVAL_ROTR ( F4 ( T[0], T[4], T[2], T[1], T[7], T[3], T[5] ), 7 ) + HAVAL_ROTR ( T[6], 11 ) + W[11] + 0x83F44239UL;
    T[5] = HAVAL_ROTR ( F4 ( T[7], T[3], T[1], T[0], T[6], T[2], T[4] ), 7 ) + HAVAL_ROTR ( T[5], 11 ) + W[31] + 0x2E0B4482UL;
    T[4] = HAVAL_ROTR ( F4 ( T[6], T[2], T[0], T[7], T[5], T[1], T[3] ), 7 ) + HAVAL_ROTR ( T[4], 11 ) + W[21] + 0xA4842004UL;
    T[3] = HAVAL_ROTR ( F4 ( T[5], T[1], T[7], T[6], T[4], T[0], T[2] ), 7 ) + HAVAL_ROTR ( T[3], 11 ) + W[ 8] + 0x69C8F04AUL;
    T[2] = HAVAL_ROTR ( F4 ( T[4], T[0], T[6], T[5], T[3], T[7], T[1] ), 7 ) + HAVAL_ROTR ( T[2], 11 ) + W[27] + 0x9E1F9B5EUL;
    T[1] = HAVAL_ROTR ( F4 ( T[3], T[7], T[5], T[4], T[2], T[6], T[0] ), 7 ) + HAVAL_ROTR ( T[1], 11 ) + W[12] + 0x21C66842UL;
    T[0] = HAVAL_ROTR ( F4 ( T[2], T[6], T[4], T[3], T[1], T[5], T[7] ), 7 ) + HAVAL_ROTR ( T[0], 11 ) + W[ 9] + 0xF6E96C9AUL;

    T[7] = HAVAL_ROTR ( F4 ( T[1], T[5], T[3], T[2], T[0], T[4], T[6] ), 7 ) + HAVAL_ROTR ( T[7], 11 ) + W[ 1] + 0x670C9C61UL;
    T[6] = HAVAL_ROTR ( F4 ( T[0], T[4], T[2], T[1], T[7], T[3], T[5] ), 7 ) + HAVAL_ROTR ( T[6], 11 ) + W[29] + 0xABD388F0UL;
    T[5] = HAVAL_ROTR ( F4 ( T[7], T[3], T[1], T[0], T[6], T[2], T[4] ), 7 ) + HAVAL_ROTR ( T[5], 11 ) + W[ 5] + 0x6A51A0D2UL;
    T[4] = HAVAL_ROTR ( F4 ( T[6], T[2], T[0], T[7], T[5], T[1], T[3] ), 7 ) + HAVAL_ROTR ( T[4], 11 ) + W[15] + 0xD8542F68UL;
    T[3] = HAVAL_ROTR ( F4 ( T[5], T[1], T[7], T[6], T[4], T[0], T[2] ), 7 ) + HAVAL_ROTR ( T[3], 11 ) + W[17] + 0x960FA728UL;
    T[2] = HAVAL_ROTR ( F4 ( T[4], T[0], T[6], T[5], T[3], T[7], T[1] ), 7 ) + HAVAL_ROTR ( T[2], 11 ) + W[10] + 0xAB5133A3UL;
    T[1] = HAVAL_ROTR ( F4 ( T[3], T[7], T[5], T[4], T[2], T[6], T[0] ), 7 ) + HAVAL_ROTR ( T[1], 11 ) + W[16] + 0x6EEF0B6CUL;
    T[0] = HAVAL_ROTR ( F4 ( T[2], T[6], T[4], T[3], T[1], T[5], T[7] ), 7 ) + HAVAL_ROTR ( T[0], 11 ) + W[13] + 0x137A3BE4UL;

    /* PASS 5: */

    T[7] = HAVAL_ROTR ( F5 ( T[2], T[5], T[0], T[6], T[4], T[3], T[1] ), 7 ) + HAVAL_ROTR ( T[7], 11 ) + W[27] + 0xBA3BF050UL;
    T[6] = HAVAL_ROTR ( F5 ( T[1], T[4], T[7], T[5], T[3], T[2], T[0] ), 7 ) + HAVAL_ROTR ( T[6], 11 ) + W[ 3] + 0x7EFB2A98UL;
    T[5] = HAVAL_ROTR ( F5 ( T[0], T[3], T[6], T[4], T[2], T[1], T[7] ), 7 ) + HAVAL_ROTR ( T[5], 11 ) + W[21] + 0xA1F1651DUL;
    T[4] = HAVAL_ROTR ( F5 ( T[7], T[2], T[5], T[3], T[1], T[0], T[6] ), 7 ) + HAVAL_ROTR ( T[4], 11 ) + W[26] + 0x39AF0176UL;
    T[3] = HAVAL_ROTR ( F5 ( T[6], T[1], T[4], T[2], T[0], T[7], T[5] ), 7 ) + HAVAL_ROTR ( T[3], 11 ) + W[17] + 0x66CA593EUL;
    T[2] = HAVAL_ROTR ( F5 ( T[5], T[0], T[3], T[1], T[7], T[6], T[4] ), 7 ) + HAVAL_ROTR ( T[2], 11 ) + W[11] + 0x82430E88UL;
    T[1] = HAVAL_ROTR ( F5 ( T[4], T[7], T[2], T[0], T[6], T[5], T[3] ), 7 ) + HAVAL_ROTR ( T[1], 11 ) + W[20] + 0x8CEE8619UL;
    T[0] = HAVAL_ROTR ( F5 ( T[3], T[6], T[1], T[7], T[5], T[4], T[2] ), 7 ) + HAVAL_ROTR ( T[0], 11 ) + W[29] + 0x456F9FB4UL;

    T[7] = HAVAL_ROTR ( F5 ( T[2], T[5], T[0], T[6], T[4], T[3], T[1] ), 7 ) + HAVAL_ROTR ( T[7], 11 ) + W[19] + 0x7D84A5C3UL;
    T[6] = HAVAL_ROTR ( F5 ( T[1], T[4], T[7], T[5], T[3], T[2], T[0] ), 7 ) + HAVAL_ROTR ( T[6], 11 ) + W[ 0] + 0x3B8B5EBEUL;
    T[5] = HAVAL_ROTR ( F5 ( T[0], T[3], T[6], T[4], T[2], T[1], T[7] ), 7 ) + HAVAL_ROTR ( T[5], 11 ) + W[12] + 0xE06F75D8UL;
    T[4] = HAVAL_ROTR ( F5 ( T[7], T[2], T[5], T[3], T[1], T[0], T[6] ), 7 ) + HAVAL_ROTR ( T[4], 11 ) + W[ 7] + 0x85C12073UL;
    T[3] = HAVAL_ROTR ( F5 ( T[6], T[1], T[4], T[2], T[0], T[7], T[5] ), 7 ) + HAVAL_ROTR ( T[3], 11 ) + W[13] + 0x401A449FUL;
    T[2] = HAVAL_ROTR ( F5 ( T[5], T[0], T[3], T[1], T[7], T[6], T[4] ), 7 ) + HAVAL_ROTR ( T[2], 11 ) + W[ 8] + 0x56C16AA6UL;
    T[1] = HAVAL_ROTR ( F5 ( T[4], T[7], T[2], T[0], T[6], T[5], T[3] ), 7 ) + HAVAL_ROTR ( T[1], 11 ) + W[31] + 0x4ED3AA62UL;
    T[0] = HAVAL_ROTR ( F5 ( T[3], T[6], T[1], T[7], T[5], T[4], T[2] ), 7 ) + HAVAL_ROTR ( T[0], 11 ) + W[10] + 0x363F7706UL;

    T[7] = HAVAL_ROTR ( F5 ( T[2], T[5], T[0], T[6], T[4], T[3], T[1] ), 7 ) + HAVAL_ROTR ( T[7], 11 ) + W[ 5] + 0x1BFEDF72UL;
    T[6] = HAVAL_ROTR ( F5 ( T[1], T[4], T[7], T[5], T[3], T[2], T[0] ), 7 ) + HAVAL_ROTR ( T[6], 11 ) + W[ 9] + 0x429B023DUL;
    T[5] = HAVAL_ROTR ( F5 ( T[0], T[3], T[6], T[4], T[2], T[1], T[7] ), 7 ) + HAVAL_ROTR ( T[5], 11 ) + W[14] + 0x37D0D724UL;
    T[4] = HAVAL_ROTR ( F5 ( T[7], T[2], T[5], T[3], T[1], T[0], T[6] ), 7 ) + HAVAL_ROTR ( T[4], 11 ) + W[30] + 0xD00A1248UL;
    T[3] = HAVAL_ROTR ( F5 ( T[6], T[1], T[4], T[2], T[0], T[7], T[5] ), 7 ) + HAVAL_ROTR ( T[3], 11 ) + W[18] + 0xDB0FEAD3UL;
    T[2] = HAVAL_ROTR ( F5 ( T[5], T[0], T[3], T[1], T[7], T[6], T[4] ), 7 ) + HAVAL_ROTR ( T[2], 11 ) + W[ 6] + 0x49F1C09BUL;
    T[1] = HAVAL_ROTR ( F5 ( T[4], T[7], T[2], T[0], T[6], T[5], T[3] ), 7 ) + HAVAL_ROTR ( T[1], 11 ) + W[28] + 0x075372C9UL;
    T[0] = HAVAL_ROTR ( F5 ( T[3], T[6], T[1], T[7], T[5], T[4], T[2] ), 7 ) + HAVAL_ROTR ( T[0], 11 ) + W[24] + 0x80991B7BUL;

    E[7] += T[7] = HAVAL_ROTR ( F5 ( T[2], T[5], T[0], T[6], T[4], T[3], T[1] ), 7 ) + HAVAL_ROTR ( T[7], 11 ) + W[ 2] + 0x25D479D8UL;
    E[6] += T[6] = HAVAL_ROTR ( F5 ( T[1], T[4], T[7], T[5], T[3], T[2], T[0] ), 7 ) + HAVAL_ROTR ( T[6], 11 ) + W[23] + 0xF6E8DEF7UL;
    E[5] += T[5] = HAVAL_ROTR ( F5 ( T[0], T[3], T[6], T[4], T[2], T[1], T[7] ), 7 ) + HAVAL_ROTR ( T[5], 11 ) + W[16] + 0xE3FE501AUL;
    E[4] += T[4] = HAVAL_ROTR ( F5 ( T[7], T[2], T[5], T[3], T[1], T[0], T[6] ), 7 ) + HAVAL_ROTR ( T[4], 11 ) + W[22] + 0xB6794C3BUL;
    E[3] += T[3] = HAVAL_ROTR ( F5 ( T[6], T[1], T[4], T[2], T[0], T[7], T[5] ), 7 ) + HAVAL_ROTR ( T[3], 11 ) + W[ 4] + 0x976CE0BDUL;
    E[2] += T[2] = HAVAL_ROTR ( F5 ( T[5], T[0], T[3], T[1], T[7], T[6], T[4] ), 7 ) + HAVAL_ROTR ( T[2], 11 ) + W[ 1] + 0x04C006BAUL;
    E[1] += T[1] = HAVAL_ROTR ( F5 ( T[4], T[7], T[2], T[0], T[6], T[5], T[3] ), 7 ) + HAVAL_ROTR ( T[1], 11 ) + W[25] + 0xC1A94FB6UL;
    E[0] += T[0] = HAVAL_ROTR ( F5 ( T[3], T[6], T[1], T[7], T[5], T[4], T[2] ), 7 ) + HAVAL_ROTR ( T[0], 11 ) + W[15] + 0x409F60C4UL;

}

Haval::Haval( uint16_t hashLength, uint16_t passes ) : context( new HAVAL_CONTEXT ), _passes( passes ), _length( hashLength )  {
    if ( passes != 3 && passes != 4 && passes != 5 )
        throw std::invalid_argument( "Invalid number of passes" );
    if ( hashLength != 128 && hashLength != 160 && hashLength != 192 &&
            hashLength != 224 && hashLength != 256 )
        throw std::invalid_argument( "Invalid hash length" );

}

Haval::~Haval() {
    delete context;
    context = NULL;
}

void Haval::init() {
    /* properly initialize HAVAL context: */
    mhash_bzero( context, sizeof ( HAVAL_CONTEXT ) );
    context->digest[0] = 0x243F6A88UL;
    context->digest[1] = 0x85A308D3UL;
    context->digest[2] = 0x13198A2EUL;
    context->digest[3] = 0x03707344UL;
    context->digest[4] = 0xA4093822UL;
    context->digest[5] = 0x299F31D0UL;
    context->digest[6] = 0x082EFA98UL;
    context->digest[7] = 0xEC4E6C89UL;
}

void Haval::write( const byte* dataBuffer, int dataLength ) {
    if ( dataBuffer == NULL || dataLength == 0 ) {
        return; /* nothing to do */
    }

    /* update bit count: Extra parentheses for Borland C++ --Tines*/
    if ( ( ( uint32_t )dataLength << 3 ) > ( 0xFFFFFFFFUL - context->bitCount[0] ) ) {
        context->bitCount[1]++;
    }
    context->bitCount[0] += ( uint32_t )dataLength << 3;

    /* if the data buffer is not enough to complete */
    /* the context data block, just append it: */
    if ( context->occupied + ( uint32_t )dataLength < 128 ) { /* caveat: typecast avoids 16-bit overflow */
        memcpy ( &context->block[context->occupied], dataBuffer, dataLength );
        context->occupied += dataLength;
        return; /* delay processing */
    }

    /* complete the context data block: */
    memcpy ( &context->block[context->occupied], dataBuffer, 128 - context->occupied );
    dataBuffer += 128 - context->occupied;
    dataLength -= 128 - context->occupied;

    switch ( _passes ) {
        case 3:
            /* process the completed context data block: */
            havalTransform3 ( context->digest, context->block, context->temp );
            /* process data in chunks of 128 bytes: */
            while ( dataLength >= 128 ) {
                havalTransform3 ( context->digest, dataBuffer, context->temp );
                dataBuffer += 128;
                dataLength -= 128;
            }
            break;
        case 4:
            /* process the completed context data block: */
            havalTransform4 ( context->digest, context->block, context->temp );
            /* process data in chunks of 128 bytes: */
            while ( dataLength >= 128 ) {
                havalTransform4 ( context->digest, dataBuffer, context->temp );
                dataBuffer += 128;
                dataLength -= 128;
            }
            break;
        case 5:
            /* process the completed context data block: */
            havalTransform5 ( context->digest, context->block, context->temp );
            /* process data in chunks of 128 bytes: */
            while ( dataLength >= 128 ) {
                havalTransform5 ( context->digest, dataBuffer, context->temp );
                dataBuffer += 128;
                dataLength -= 128;
            }
            break;
    }

    /* delay processing of remaining data: */
    memcpy ( context->block, dataBuffer, dataLength );
    context->occupied = dataLength; /* < 128 */
}


byte* Haval::final() {
    uint32_t w;

    /* append toggle to the context data block: */
    context->block[context->occupied] = 0x01; /* corrected from 0x80 */

    /* pad the message with null bytes to make it 944 (mod 1024) bits long: */
    if ( context->occupied++ >= 118 ) {
        /* no room for tail data on the current context block */
        mhash_bzero ( &context->block[context->occupied], 128 - context->occupied );
        /* process the completed context data block: */
        switch ( _passes ) {
            case 3:
                havalTransform3 ( context->digest, context->block, context->temp );
                break;
            case 4:
                havalTransform4 ( context->digest, context->block, context->temp );
                break;
            case 5:
                havalTransform5 ( context->digest, context->block, context->temp );
                break;
        }
        mhash_bzero ( context->block, 118 );
    } else {
        mhash_bzero ( &context->block[context->occupied], 118 - context->occupied );
    }
    /* append tail data and process last (padded) message block: */
    context->block[118] = ( byte )(
                              ( ( _length & 0x03U ) << 6 ) |
                              ( ( _passes & 0x07U ) << 3 ) |
                              ( HAVAL_VERSION           & 0x07U ) );
    context->block[119] = ( byte ) ( _length >> 2 );
    w = context->bitCount[0];
    context->block[120] = ( byte )( w );
    context->block[121] = ( byte )( w >>  8 );
    context->block[122] = ( byte )( w >> 16 );
    context->block[123] = ( byte )( w >> 24 );
    w = context->bitCount[1];
    context->block[124] = ( byte )( w );
    context->block[125] = ( byte )( w >>  8 );
    context->block[126] = ( byte )( w >> 16 );
    context->block[127] = ( byte )( w >> 24 );
    switch ( _passes ) {
        case 3:
            havalTransform3 ( context->digest, context->block, context->temp );
            break;
        case 4:
            havalTransform4 ( context->digest, context->block, context->temp );
            break;
        case 5:
            havalTransform5 ( context->digest, context->block, context->temp );
            break;
    }

    /* fold 256-bit digest to fit the desired hash length (blaargh!): */
    /* Byte reverse each 32-bit section while outputting if big-endian -- Tines*/
    switch ( _length ) {
        case 128:
            context->digest[3] +=
                ( ( context->digest[7] & 0xFF000000UL )
                  | ( context->digest[6] & 0x00FF0000UL )
                  | ( context->digest[5] & 0x0000FF00UL )
                  | ( context->digest[4] & 0x000000FFUL )
                );
            context->digest[2] +=
                ( ( ( context->digest[7] & 0x00FF0000UL )
                    | ( context->digest[6] & 0x0000FF00UL )
                    | ( context->digest[5] & 0x000000FFUL )
                  ) << 8 ) |
                ( ( context->digest[4] & 0xFF000000UL ) >> 24 );
            context->digest[1] +=
                ( ( ( context->digest[7] & 0x0000FF00UL )
                    | ( context->digest[6] & 0x000000FFUL ) ) << 16 ) |
                ( ( ( context->digest[5] & 0xFF000000UL )
                    | ( context->digest[4] & 0x00FF0000UL ) ) >> 16 );
            context->digest[0] +=
                ( ( ( context->digest[6] & 0xFF000000UL )
                    | ( context->digest[5] & 0x00FF0000UL )
                    | ( context->digest[4] & 0x0000FF00UL )
                  ) >> 8 ) |
                ( ( context->digest[7] & 0x000000FFUL ) << 24 );
            break;
        case 160:
            context->digest[4] +=
                ( ( context->digest[7] & 0xFE000000UL ) | ( context->digest[6] & 0x01F80000UL ) | ( context->digest[5] & 0x0007F000UL ) ) >> 12;
            context->digest[3] +=
                ( ( context->digest[7] & 0x01F80000UL ) | ( context->digest[6] & 0x0007F000UL ) | ( context->digest[5] & 0x00000FC0UL ) ) >> 6;
            context->digest[2] +=
                ( ( context->digest[7] & 0x0007F000UL ) | ( context->digest[6] & 0x00000FC0UL ) | ( context->digest[5] & 0x0000003FUL ) );
            context->digest[1] +=
                HAVAL_ROTR
                ( ( context->digest[7] & 0x00000FC0UL ) | ( context->digest[6] & 0x0000003FUL ) | ( context->digest[5] & 0xFE000000UL ), 25 );
            context->digest[0] +=
                HAVAL_ROTR
                ( ( context->digest[7] & 0x0000003FUL ) | ( context->digest[6] & 0xFE000000UL ) | ( context->digest[5] & 0x01F80000UL ), 19 );
            break;
        case 192:
            context->digest[5] +=
                ( ( context->digest[7] & 0xFC000000UL ) | ( context->digest[6] & 0x03E00000UL ) ) >> 21;
            context->digest[4] +=
                ( ( context->digest[7] & 0x03E00000UL ) | ( context->digest[6] & 0x001F0000UL ) ) >> 16;
            context->digest[3] +=
                ( ( context->digest[7] & 0x001F0000UL ) | ( context->digest[6] & 0x0000FC00UL ) ) >> 10;
            context->digest[2] +=
                ( ( context->digest[7] & 0x0000FC00UL ) | ( context->digest[6] & 0x000003E0UL ) ) >>  5;
            context->digest[1] +=
                ( ( context->digest[7] & 0x000003E0UL ) | ( context->digest[6] & 0x0000001FUL ) );
            context->digest[0] +=
                HAVAL_ROTR
                ( ( context->digest[7] & 0x0000001FUL ) | ( context->digest[6] & 0xFC000000UL ), 26 );
            break;
        case 224:
            context->digest[6] += ( context->digest[7]      ) & 0x0000000FUL;
            context->digest[5] += ( context->digest[7] >>  4 ) & 0x0000001FUL;
            context->digest[4] += ( context->digest[7] >>  9 ) & 0x0000000FUL;
            context->digest[3] += ( context->digest[7] >> 13 ) & 0x0000001FUL;
            context->digest[2] += ( context->digest[7] >> 18 ) & 0x0000000FUL;
            context->digest[1] += ( context->digest[7] >> 22 ) & 0x0000001FUL;
            context->digest[0] += ( context->digest[7] >> 27 ) & 0x0000001FUL;
            break;
    }
    return reinterpret_cast<byte*>( context->digest );
}

string Haval::calcHaval( const string &buf ) {
    init();
    write( reinterpret_cast<const byte*>( buf.c_str() ), buf.length() );
    byte* ret = final();

    return charToHex( reinterpret_cast<const char*>( ret ), hash_length() );
}
