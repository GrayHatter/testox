#define _DEFAULT_SOURCE
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <sodium.h>
#include "../toxcore/toxcore/DHT.h"
#include <endian.h>


//Test Names

/*
 * Distance Test
 *
 * Checks whether the xor-distance metric works correctly.
 *
 * Input:(length:type:contents)
 * 8:Int:Length of name
 * 8:Message Kind:Test name: "Distance"
 * 32:Public Key:Origin key
 * 32:Public Key:Alice key
 * 32:Public Key:Bob key
 *
 * Output:(lenght:type:contents)
 * 1:Tag:0x01 (Success)
 * 1:Ordering:Less, Equal, or Greater
 *
 * Ordering:(value:encoding:when)
 * Less:0x00:distance(Origin, Alice) < distance(Origin, Bob)
 * Equal:0x01:the distances are equal
 * Greater:0x02:distance(Origin, Alice) > distance(Origin, Bob).
 *
 */
#define DISTANCE                 "Distance"

/*
 * K-Bucket Index Test
 *
 * Checks whether the K-bucket index is computed correctly.
 *
 * Input: Two public keys for Self and Other. (length:type:contents)
 * 8:Int:Length of name
 * 12:Message Kind:Test name: "KBucketIndex"
 * 32:Public Key:Base key
 * 32:Public Key:Node key
 *
 * Output: either Nothing or Just i in a Success(success tag + payload) message.
 * 1:0x00:Base key == Node key: Nothing
 * 2:0x01, i:otherwise: Just i
 * The value of i is the k-bucket index of the Node key in a k-buckets
 * instance with the given Base key.
 *
 */
#define K_BUCKET_INDEX           "KBucketIndex"

#define K_BUCKET_NODES           "KBucketNodes"
#define NONCE_INCREMENT          "NonceIncrement"

#define BINARY_ENCODE_NODEINFO   "BinaryEncode NodeInfo"
#define BINARY_ENCODE_STRING     "BinaryEncode String"
#define BINARY_ENCODE_BYTESTRING "BinaryEncode ByteString"
#define BINARY_ENCODE_WORD32     "BinaryEncode Word32"

#define BINARY_DECODE_NODEINFO   "BinaryDecode NodeInfo"
#define BINARY_DECODE_STRING     "BinaryDecode String"
#define BINARY_DECODE_WORD32     "BinaryDecode Word32"

#define TEST_FAILURE             "Failuretest"
#define TEST_SUCCESS             "SuccessTest"
#define TEST_SKIPPED             "SkippedTest"

/*
 * The Result type is written to stdout. It is a single byte
 * for Failure (0x00), Success (0x01), and Skipped (0x02),
 * followed by the result data.
 * more; https://toktok.github.io/spec#result
 *
 */

#define RESULT_TAG_FAILURE       0x00
#define RESULT_TAG_SUCCESS       0x01
#define RESULT_TAG_SKIPPED       0x02

/*
 * Packed Node Format
 *
 * MSB bit transport protocol -> UDP=0, TCP=1
 * LSB 7 bit address family -> IPv4=4, IPv6=10
 * 4|16 bytes ip address -> IPv4=4, IPv6=16
 * 2 bytes port number
 * 32 bytes public key -> Node ID
 *
 * The following table is can be used to simplify the implementation.
 * (ip type:transport protocol:address family)
 * 2 (0x02):UDP:IPv4
 * 10 (0x0a):UDP:IPv6
 * 130 (0x82):TCP:IPv4
 * 138 (0x8a):TCP:IPv6
 *
 * more; https://toktok.github.io/spec#node-info-packed-node-format
 *
 */

typedef struct{
    char ip_type;
    unsigned char ip_address[16];
    uint16_t port_number;
    unsigned char public_key[32];
}CNodeInfo;

typedef struct{
    char is_tcp;
    char is_ipv6;
    unsigned char ip_address[16];
    uint16_t port_number;
    unsigned char public_key[32];
}DNodeInfo;



void test_kbucket(void);
void test_distance(void);
