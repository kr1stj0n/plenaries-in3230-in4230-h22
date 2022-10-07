#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <arpa/inet.h>

#include "pdu.h"

struct pdu * alloc_pdu(void)
{
	struct pdu *pdu = (struct pdu *)malloc(sizeof(struct pdu));
	
	pdu->ethhdr = (struct eth_hdr *)malloc(sizeof(struct eth_hdr));
	pdu->ethhdr->ethertype = htons(0xFFFF);
	
	pdu->hiphdr = (struct hip_hdr *)malloc(sizeof(struct hip_hdr));
        pdu->hiphdr->dst = HIP_DST_ADDR;
        pdu->hiphdr->src = HIP_DST_ADDR ;
        pdu->hiphdr->version = HIP_VERSION;
        pdu->hiphdr->len = (1u<<8) - 1;
        pdu->hiphdr->type = HIP_TYPE_HI;

	return pdu;
}

void fill_pdu(struct pdu *pdu,
	      uint8_t *src_mac_addr,
	      uint8_t *dst_mac_addr,
	      uint8_t src_hip_addr,
	      uint8_t dst_hip_addr,
	      const char *sdu)
{
	size_t slen = 0;
	
	memcpy(pdu->ethhdr->dst_mac, dst_mac_addr, 6);
	memcpy(pdu->ethhdr->src_mac, src_mac_addr, 6);
	pdu->ethhdr->ethertype = htons(ETH_P_HIP);
	
        pdu->hiphdr->dst = dst_hip_addr;
        pdu->hiphdr->src = src_hip_addr;
        pdu->hiphdr->version = HIP_VERSION;

	if (strcmp(sdu, "hi") == 0)
		pdu->hiphdr->type = HIP_TYPE_HI;
	else if (strcmp(sdu, "hei") == 0)
		pdu->hiphdr->type = HIP_TYPE_HEI;
	else if (strcmp(sdu, "heihei") == 0)
		pdu->hiphdr->type = HIP_TYPE_HEIHEI;
	else if (strcmp(sdu, "hello") == 0)
		pdu->hiphdr->type = HIP_TYPE_HELLO;
	else if (strcmp(sdu, "aloha") == 0)
		pdu->hiphdr->type = HIP_TYPE_ALOHA;

	slen = strlen(sdu) + 1;

	if (slen % 4 != 0)
		slen = slen + (4 - (slen % 4));
	
        pdu->hiphdr->len = slen / 4;

	pdu->sdu = (uint8_t *)calloc(1, slen);
	memcpy(pdu->sdu, sdu, slen);
}

size_t hip_serialize_pdu(struct pdu *pdu, uint8_t *snd_buf)
{
	size_t snd_len = 0;

	/* Copy ethernet header */
	memcpy(snd_buf + snd_len, pdu->ethhdr, sizeof(struct eth_hdr));
	snd_len += ETH_HDR_LEN;

	/* Copy HIP header */
	uint32_t hiphdr = 0;
	hiphdr |= (uint32_t) pdu->hiphdr->dst << 24;
	hiphdr |= (uint32_t) pdu->hiphdr->src << 16;
	hiphdr |= (uint32_t) (pdu->hiphdr->version & 0xf) << 12;
	hiphdr |= (uint32_t) (pdu->hiphdr->len & 0xff) << 4;
	hiphdr |= (uint32_t) (pdu->hiphdr->type & 0xf);
	hiphdr = htonl(hiphdr);

	memcpy(snd_buf + snd_len, &hiphdr, HIP_HDR_LEN);
	snd_len += HIP_HDR_LEN;

	/* Attach SDU */
	memcpy(snd_buf + snd_len, pdu->sdu, pdu->hiphdr->len * 4);
	snd_len += pdu->hiphdr->len * 4;

	return snd_len;
}

size_t hip_deserialize_pdu(struct pdu *pdu, uint8_t *recvbuf)
{
	/* *pdu = malloc(sizeof(struct pdu)); */

	/* size_t recvlen = 0; */
	/* (*pdu)->ethhdr = malloc(ETHHDR_SIZE); */
	/* memcpy((*pdu)->ethhdr, recvbuf + recvlen, ETHHDR_SIZE); */
	/* recvlen += ETHHDR_SIZE; */

	/* (*pdu)->hiphdr = malloc(MIPHDR_SIZE); */
	/* uint32_t *tmp = (uint32_t *) (recvbuf + recvlen); */
	/* uint32_t header = ntohl(*tmp); */
	/* (*pdu)->hiphdr->dst = (uint8_t) (header >> 24); */
	/* (*pdu)->hiphdr->src = (uint8_t) (header >> 16); */
	/* (*pdu)->hiphdr->ttl = (uint8_t) ((header >> 12) & 0xf); */
	/* (*pdu)->hiphdr->len = (size_t) (((header >> 3) & 0x1ff) * 4); */
	/* (*pdu)->hiphdr->type = (uint8_t) (header & 0x7); */
	/* recvlen += MIPHDR_SIZE; */

	/* (*pdu)->sdu = malloc((*pdu)->miphdr->len); */
	/* memcpy((*pdu)->sdu, recvbuf + recvlen, (*pdu)->miphdr->len); */
	/* recvlen += (*pdu)->miphdr->len; */

	/* return recvlen; */
	return 0;
}


void destroy_pdu(struct pdu *pdu)
{
	free(pdu->ethhdr);
	free(pdu->hiphdr);
	free(pdu->sdu);
	free(pdu);
}
