/*
 *                               POK header
 *
 * The following file is a part of the POK project. Any modification should
 * made according to the POK licence. You CANNOT use this file or a part of
 * this file is this part of a file for your own project
 *
 * For more information on the POK licence, please see our LICENCE FILE
 *
 * Please follow the coding guidelines described in doc/CODING_GUIDELINES
 *
 *                                      Copyright (c) 2007-2009 POK team
 *
 * Created by julien on Mon May 18 18:44:51 2009
 */

/**
 * @file
 * @author Laurent
 * @brief  RTL8029 driver
 * @date   PFE GISTR 2010
 */

#ifdef POK_NEEDS_RTL8029
#include "rtl8029.h"

#include <middleware/port.h>

// global since there is no way to get device data otherwise...
static s_ne2000_dev dev;

/*
 * We *always* assume page 0 to be selected.
 * Two exceptions: initialization and polling.
 * Therefore, each time we need to switch to page 1,
 * the card is switched to page 0 again when we're done...
 */
#define NE2000_SELECT_PAGE(dev, page)					\
  outb((pok_inb((dev)->addr + NE2000_CR) &				\
	~(NE2000_CR_PS0 | NE2000_CR_PS1)) | ((page) << 6), (dev)->addr)

static
ssize_t ne2000_write(const s_ne2000_dev* dev,
		     const void*	 buf,
		     unsigned short	 count,
		     unsigned short	 offset)
{
  const char* p = NULL;
  ssize_t ret = count;

  // Sets RD2 (abort/complete remote DMA)
  outb((pok_inb((dev)->addr + NE2000_CR) & ~(NE2000_CR_RD0 | NE2000_CR_RD1)) |
       NE2000_CR_RD2, (dev)->addr);

  /* These two registers set the start address of remote DMA. */
  outb(offset, dev->addr + NE2000_RSAR0);
  outb(offset >> 8, dev->addr + NE2000_RSAR1);

  /* These two registers set the data byte counts of remote DMA. */
  outb(count, dev->addr + NE2000_RBCR0);
  outb(count >> 8, dev->addr + NE2000_RBCR1);

  // Sets RD1 (remote write)
  outb((pok_inb((dev)->addr + NE2000_CR) & ~(NE2000_CR_RD0 | NE2000_CR_RD2)) |
       NE2000_CR_RD1, (dev)->addr);

  for (p = buf; count > 0; count--, p++)
  {
    outb(*p, dev->addr + NE2000_DMA_PORT);
  }

  return (ret);
}

static
ssize_t ne2000_read(const s_ne2000_dev*	dev,
		    void*		buf,
		    unsigned short	count,
		    unsigned short	offset)
{
  char* p = NULL;
  ssize_t ret = count;

  // Sets RD2 (abort/complete remote DMA)
  outb((pok_inb((dev)->addr + NE2000_CR) & ~(NE2000_CR_RD0 | NE2000_CR_RD1)) |
       NE2000_CR_RD2, (dev)->addr);

  /* These two registers set the start address of remote DMA. */
  outb(offset, dev->addr + NE2000_RSAR0);
  outb(offset >> 8, dev->addr + NE2000_RSAR1);

  /* These two registers set the data byte counts of remote DMA. */
  outb(count, dev->addr + NE2000_RBCR0);
  outb(count >> 8, dev->addr + NE2000_RBCR1);

  // Sets RD0 (remote read)
  outb((pok_inb((dev)->addr + NE2000_CR) & ~(NE2000_CR_RD1 | NE2000_CR_RD2)) |
       NE2000_CR_RD0, (dev)->addr);

  for (p = buf; count > 0; count--, p++)
  {
    *p = pok_inb(dev->addr + NE2000_DMA_PORT);
  }

  return (ret);
}


/**
 *  @brief Enqueues a packet in the appropriate queue
 *
 */
static inline
void rtl8029_enqueue (pok_packet_t *packet)
{
  pok_queue_t*	queue = dev.recv_buf + packet->udp.dst;
  uint32_t	off = 0;
  uint32_t	i = 0;

  /* overflow? */
  if (queue->len + packet->udp.len > RECV_BUF_SZ)
  {
    printf("rtl8029_read: error: ring buffer %d overflow!\n", packet->udp.dst);
    return;
  }

  /* at which offset should we start writing? */
  off = (queue->off + queue->len) % RECV_BUF_SZ;

  /* copying data from the packet to the circular buffer in the queue */
  for (i = 0; i < packet->udp.len; i++)
  {
    queue->data[off] = packet->data[i];
    off = (off + 1) % RECV_BUF_SZ;
  }

  /* updating data length in this queue */
  queue->len += packet->udp.len;
}

/**
 *  @brief Reads data from the corresponding network stack
 *
 *  Reads enqueued data in the stack partition.
 */
void rtl8029_read (pok_port_id_t port_id, void* data, uint32_t len)
{
  pok_port_id_t global;
  pok_ret_t     ret;

  ret = pok_port_virtual_get_global (port_id, &global);

  if (ret == POK_ERRNO_OK)
  {
    char	*dest = data;
    pok_queue_t* queue = dev.recv_buf + global;
    uint32_t	size = len < queue->len ? len : queue->len;
    uint32_t	copied = 0;

    printf ("[RTL8029] READ DATA FROM LOCAL PORT %d "
	    "GLOBAL_PORT=%d), size=%d\n", port_id, global, len);

    /* is there something to read ? */
    if (queue->len == 0)
    {
      printf("rtl8029_read: error: empty read ring buffer %d!\n", port_id);
      return;
    }

    /* copy from the queue to the buffer */
    for (copied = 0; copied < size; copied++)
    {
      dest[copied % RECV_BUF_SZ] = queue->data[queue->off];
      queue->off = (queue->off + 1) % RECV_BUF_SZ;
    }

    /* updating data length in this queue */
    queue->len -= size;
  }
}

/**
 *  @brief Send data to the interface
 *
 *  Writes data to be sent to network.
 */
void rtl8029_write (pok_port_id_t port_id, const void* data, uint32_t len)
{
  uint32_t        nbdest;
  uint32_t        tmp;
  uint32_t        dest;
  pok_ret_t       ret;
  char            node2[6] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };
  pok_packet_t    packet;
  const char*     d;
  size_t          cpylen = 0;
  size_t          sndlen = 0;
  unsigned char	state; // ISR state

  ret = pok_port_virtual_nb_destinations (port_id, &nbdest);
  if (ret != POK_ERRNO_OK)
  {
    return;
  }

  for (tmp = 0 ; tmp < nbdest ; tmp++)
  {
    ret = pok_port_virtual_destination (port_id, tmp, &dest);
    if (ret == POK_ERRNO_OK)
    {
      printf ("[RTL8029] SEND DATA THROUGH NETWORK FROM LOCAL PORT %d "
	      "TO GLOBAL PORT %d, size=%d\n", port_id, dest, len);

      memcpy(packet.eth.src, dev.mac, ETH_MAC_LEN);
      memcpy(packet.eth.dst, node2, ETH_MAC_LEN);
      packet.eth.ethertype = 0x4242;
      packet.udp.src = port_id;
      packet.udp.dst = dest;

      for (d = data; len != 0; len -= cpylen, data += cpylen)
      {
	// too short; let's cut
	if (len <= NET_DATA_MINLEN)
	{
	  cpylen = len;
	  sndlen = ETH_DATA_MINLEN + sizeof(eth_hdr_t);
	}
	else
	{
	  // too big; let's pad
	  if (len >= NET_DATA_MAXLEN)
	  {
	    cpylen = NET_DATA_MAXLEN;
	    sndlen = ETH_DATA_MAXLEN + sizeof(eth_hdr_t);
	  }
	  // normal
	  else
	  {
	    cpylen = len;
	    sndlen = sizeof(eth_hdr_t) + sizeof(udp_hdr_t) + cpylen;
	  }
	}

	packet.udp.len = cpylen;
	memcpy(&(packet.data), data, cpylen);

	ne2000_write(&dev, &packet, sndlen, NE2000_TXBUF * 256);

	do
	{
	  state = pok_inb(dev.addr + NE2000_ISR);
	}
	while ((state & NE2000_ISR_RDC) != NE2000_ISR_RDC);

	/* This register sets the start page address of
	   the packet to the transmitted. */
	outb(NE2000_TXBUF, dev.addr + NE2000_TPSR); //?

	/* These two registers set the byte counts of
	   the packet to be transmitted. */
	outb(sndlen, dev.addr + NE2000_TBCR0);
	outb(sndlen >> 8, dev.addr + NE2000_TBCR1);

	/* This bit must be set to transmit a packet. */
	outb(pok_inb(dev.addr + NE2000_CR) | NE2000_CR_TXP,
	     dev.addr + NE2000_CR);

	outb(NE2000_ISR_RDC, dev.addr + NE2000_ISR); // Clear RDC bit
      }
    }
  }
}

/**
 *  @brief Polls rtl8029 device.
 *
 *  Watches for events, typically for receiving queued packets.
 */
void rtl8029_polling ()
{
  unsigned char	state; // ISR state

  NE2000_SELECT_PAGE(&dev, 0);

  while (1)
  {
    // do we have an interrupt flag set?
    if ((state = pok_inb(dev.addr + NE2000_ISR)) == 0)
      continue;

    if (state & NE2000_ISR_PRX)
    {
      if ((pok_inb(dev.addr + NE2000_RSR) & NE2000_RSR_PRX) == 0)
      {
	// error
      }

      printf("[*]\n");

      /* no errors */
      s_ne2000_header	ne2000_hdr;	// ne2000 packet header
      unsigned short	offset;		// dma offset
      unsigned char	start, end;	// pointers for the ring buffer
      pok_packet_t	recv_packet;

      while (1)
      {

	/* This register is used to prevent overwrite of the receive buffer ring.
	   It is typically used as a pointer indicating the last receive buffer
	   page the host has read.*/
	start = pok_inb(dev.addr + NE2000_BNRY) + 1;

	/* This register points to the page address of the first receive
	   buffer page to be used for a packet reception. */
	NE2000_SELECT_PAGE(&dev, 1);
	end = pok_inb(dev.addr + NE2000_CURR);
	NE2000_SELECT_PAGE(&dev, 0);

	if ((end % NE2000_MEMSZ) == (start % NE2000_MEMSZ) + 1)
	{
	  break;
	}

	/* et on decapsule! */
	offset = start << 8;
	// ne2000 header
	offset += ne2000_read(&dev, &ne2000_hdr, sizeof(s_ne2000_header),
			      offset);

	ne2000_read(&dev, &recv_packet,
		    ne2000_hdr.size - sizeof(s_ne2000_header), offset);
	rtl8029_enqueue(&recv_packet);

	// update the BNRY register... almost forgot that
	outb(ne2000_hdr.next > NE2000_MEMSZ ?
	     NE2000_RXBUF - 1 : ne2000_hdr.next - 1, dev.addr + NE2000_BNRY);

      }

      outb(NE2000_ISR_PRX, dev.addr + NE2000_ISR); // Clear PRX flag
    }

    if (state & NE2000_ISR_PTX)
    {
      outb(NE2000_ISR_PTX, dev.addr + NE2000_ISR); // Clear PTX flag
    }

    if (state & NE2000_ISR_RXE)
    {
      outb(NE2000_ISR_RXE, dev.addr + NE2000_ISR); // Clear RXE flag
    }

    if (state & NE2000_ISR_TXE)
    {
      outb(NE2000_ISR_TXE, dev.addr + NE2000_ISR); // Clear TXE flag
    }

    if (state & NE2000_ISR_OVW)
    {
      outb(NE2000_ISR_OVW, dev.addr + NE2000_ISR); // Clear OVW flag
    }

    if (state & NE2000_ISR_CNT)
    {
      outb(NE2000_ISR_CNT, dev.addr + NE2000_ISR); // Clear CNT flag
    }

    if (state & NE2000_ISR_RST)
    {
      outb(NE2000_ISR_RST, dev.addr + NE2000_ISR); // Clear RST bit
    }
  }
}

/**
 *  @brief Initializes rtl8029 device.
 *
 *  Seeks and registers PCI interface, set configuration and fills the
 *  dev structure.
 */
void rtl8029_init ()
{
  dev.pci.vendorid = 0x10ec;
  dev.pci.deviceid = 0x8029;
  dev.pci.io_range = 0x10;

  if (pci_register(&(dev.pci)) != 0)
  {
    printf("rtl8029: PCI init failed!\n");
    return;
  }

  dev.addr = dev.pci.bar[0] & (~0x1F);

  unsigned char	i = 0;
  unsigned char	buf[6 * 2]; // used for MAC address

  NE2000_SELECT_PAGE(&dev, 0);

  /* This bit is the STOP command. When it is set, no packets will be
     received or transmitted. POWER UP=1. */
  outb(NE2000_CR_STP, dev.addr + NE2000_CR);

  // Sets several options... Read the datasheet!
  outb(0x00, dev.addr + NE2000_TCR);
  outb(NE2000_RCR_AB, dev.addr + NE2000_RCR);
  outb(NE2000_DCR_LS | NE2000_DCR_FT1, dev.addr + NE2000_DCR);

  /* The Page Start register sets the start page address
     of the receive buffer ring. */
  outb(NE2000_RXBUF, dev.addr + NE2000_PSTART);
  /* The Page Stop register sets the stop page address
     of the receive buffer ring. */
  outb(NE2000_MEMSZ, dev.addr + NE2000_PSTOP);
  /* This register is used to prevent overwrite of the receive buffer ring.
     It is typically used as a pointer indicating the last receive buffer
     page the host has read. */
  outb(NE2000_RXBUF, dev.addr + NE2000_BNRY);

  /* These two registers set the data byte counts of remote DMA. */
  outb(0, dev.addr + NE2000_RBCR0);
  outb(0, dev.addr + NE2000_RBCR1);

  NE2000_SELECT_PAGE(&dev, 1);

  /* This register points to the page address of the first receive buffer
     page to be used for a packet reception. */
  outb(NE2000_RXBUF + 1, dev.addr + NE2000_CURR);

  // Init mac address
  /* Here's something I do not understand... Section 6.2.2 of the datasheet
     says bytes 00H-05H of the PROM corresponds to the Ethernet ID. But it
     looks like each byte of the MAC address is written twice...
     Therefore I read 2 * sizeof(mac) and select one of the two bytes
     corresponding to the MAC... Weird... Really... */
  ne2000_read(&dev, buf, 6 * 2, 0);
  for (i = 0; i < 6; i++)
    dev.mac[i] = buf[i * 2];

  /* These registers contain my Ethernet node address and are used to compare
    the destination address of incoming packets for acceptation or rejection.*/
  outb(dev.mac[0], dev.addr + NE2000_PAR0);
  outb(dev.mac[1], dev.addr + NE2000_PAR1);
  outb(dev.mac[2], dev.addr + NE2000_PAR2);
  outb(dev.mac[3], dev.addr + NE2000_PAR3);
  outb(dev.mac[4], dev.addr + NE2000_PAR4);
  outb(dev.mac[5], dev.addr + NE2000_PAR5);

  NE2000_SELECT_PAGE(&dev, 0);

  // Start command
  outb(NE2000_CR_STA, dev.addr + NE2000_CR);

  // Reactivating interrupts
  /* ISR register must be cleared after power up. */
  outb(0xFF, dev.addr + NE2000_ISR);
  /* All bits correspond to the bits in the ISR register. POWER UP=all 0s.
     Setting individual bits will enable the corresponding interrupts. */
  /* Since POK use polling, ALL interrupts are disabled */
  outb(0x00, dev.addr + NE2000_IMR);

  for (i = 0; i < 20; i++) /* TODO: random constant */
  {
    dev.recv_buf[i].len = 0;
    dev.recv_buf[i].off = 0;
  }

  return;
}

#endif
