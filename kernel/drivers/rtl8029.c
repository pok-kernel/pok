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

// Bellow, POK_NEEDS_PCI is kept for backward compatibility with code generators
#if (defined POK_NEEDS_RTL8029 || defined POK_NEEDS_PCI)
#include <drivers/rtl8029.h>

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
  outb((dev)->addr, (inb((dev)->addr + NE2000_CR) &			\
	~(NE2000_CR_PS0 | NE2000_CR_PS1)) | ((page) << 6))

static
size_t ne2000_write(const s_ne2000_dev* dev,
		     const void*	 buf,
		     unsigned short	 count,
		     unsigned short	 offset)
{
  const char* p = NULL;
  size_t ret = count;

  // Sets RD2 (abort/complete remote DMA)
  outb((dev)->addr,
	(inb((dev)->addr + NE2000_CR) & ~(NE2000_CR_RD0 | NE2000_CR_RD1)) | NE2000_CR_RD2);

  /* These two registers set the start address of remote DMA. */
  outb(dev->addr + NE2000_RSAR0, offset);
  outb(dev->addr + NE2000_RSAR1, offset >> 8);

  /* These two registers set the data byte counts of remote DMA. */
  outb(dev->addr + NE2000_RBCR0, count);
  outb(dev->addr + NE2000_RBCR1, count >> 8);

  // Sets RD1 (remote write)
  outb((dev)->addr,
       (inb((dev)->addr + NE2000_CR) & ~(NE2000_CR_RD0 | NE2000_CR_RD2)) | NE2000_CR_RD1);

  for (p = buf; count > 0; count--, p++)
  {
    outb(dev->addr + NE2000_DMA_PORT, *p);
  }
  
  return (ret);
}

static
size_t ne2000_read(const s_ne2000_dev*	dev,
		    void*		buf,
		    unsigned short	count,
		    unsigned short	offset)
{
  char* p = NULL;
  size_t ret = count;

  // Sets RD2 (abort/complete remote DMA)
  outb((dev)->addr,
	(inb((dev)->addr + NE2000_CR) & ~(NE2000_CR_RD0 | NE2000_CR_RD1)) | NE2000_CR_RD2);

  /* These two registers set the start address of remote DMA. */
  outb(dev->addr + NE2000_RSAR0, offset);
  outb(dev->addr + NE2000_RSAR1, offset >> 8);

  /* These two registers set the data byte counts of remote DMA. */
  outb(dev->addr + NE2000_RBCR0, count);
  outb(dev->addr + NE2000_RBCR1, count >> 8);

  // Sets RD0 (remote read)
  outb((dev)->addr,
	(inb((dev)->addr + NE2000_CR) & ~(NE2000_CR_RD1 | NE2000_CR_RD2)) | NE2000_CR_RD0);

  for (p = buf; count > 0; count--, p++)
  {
    *p = inb(dev->addr + NE2000_DMA_PORT);
  }
  
  return (ret);
}

extern uint8_t pok_global_ports_to_local_ports[POK_CONFIG_NB_GLOBAL_PORTS];

/**
 *  @brief Enqueues a packet in the appropriate queue
 *
 */
static inline
void rtl8029_enqueue (pok_packet_t *packet)
{

  pok_port_id_t port_id = pok_global_ports_to_local_ports[packet->udp.dst];
  
  pok_queue_t*	queue = dev.recv_buf + port_id;
  uint32_t	off = 0;
  uint32_t	i = 0;

  /* overflow? */
  if (queue->len + packet->udp.len > RECV_BUF_SZ)
  {
#ifdef POK_NEEDS_DEBUG
    printf("rtl8029_read: error: ring buffer %d overflow!\n", packet->udp.dst);
#endif
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
  dev.msg_nb++;
}

void rtl8029_poll_and_read (pok_port_id_t port_id, void* data, uint32_t len)
{
  
  unsigned char	state; // ISR state

  NE2000_SELECT_PAGE(&dev, 0);

  // do we have an interrupt flag set?
  state = inb(dev.addr + NE2000_ISR);
  
  if (state && (state & NE2000_ISR_PRX))
  {
    if ((inb(dev.addr + NE2000_RSR) & NE2000_RSR_PRX) == 0)
    {
      // error
    }
    
#ifdef POK_NEEDS_DEBUG
    printf("[*]\n");
#endif
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
      start = inb(dev.addr + NE2000_BNRY) + 1;
      
      /* This register points to the page address of the first receive
	 buffer page to be used for a packet reception. */
      NE2000_SELECT_PAGE(&dev, 1);
      end = inb(dev.addr + NE2000_CURR);
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
      
      if(pok_global_ports_to_local_ports[recv_packet.udp.dst] != invalid_local_port)
      {
#ifdef POK_NEEDS_DEBUG
	printf("ENQUEUE on (global) port %d\n", recv_packet.udp.dst);
#endif
	rtl8029_enqueue(&recv_packet);
      }
      else
      {
#ifdef POK_NEEDS_DEBUG
	printf("[RTL8029] ERROR, received msg from MAC ");
	display_mac(recv_packet.eth.src);
	printf("\tsrc global port %d with an invalid port id for the destination (%d)\n", recv_packet.udp.src, recv_packet.udp.dst);
#endif
	outb(dev.addr + NE2000_BNRY,
	     ne2000_hdr.next > NE2000_MEMSZ ? NE2000_RXBUF - 1 : ne2000_hdr.next - 1);
	break;
      }

      // update the BNRY register... almost forgot that
      outb(dev.addr + NE2000_BNRY,
	   ne2000_hdr.next > NE2000_MEMSZ ? NE2000_RXBUF - 1 : ne2000_hdr.next - 1);
    }

  }

  char	*dest = data;
  pok_queue_t* queue = dev.recv_buf + port_id;
  uint32_t	size = len < queue->len ? len : queue->len;
  uint32_t	copied = 0;
  
#ifdef POK_NEEDS_DEBUG
  printf ("[RTL8029] READ DATA FROM LOCAL PORT %d, "
	  "size=%d\n", port_id, size);
#endif
    
  /* Is there something to read on the device? */
  if(dev.msg_nb == 0)
  {
#ifdef POK_NEEDS_DEBUG
    printf("rtl8029_read: error: no message on driver!\n");
#endif
    outb(dev.addr + NE2000_ISR, NE2000_ISR_PRX); // Clear PRX flag
    return;
  }
  /* is there something to read in the queue ? */
  if (queue->len == 0)
  {
#ifdef POK_NEEDS_DEBUG
    printf("rtl8029_read: error: empty read ring buffer %d!\n", port_id);
#endif
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

  /* updating messages number on device */
  if(dev.msg_nb > 0)
  {
    dev.msg_nb--;
  }
  if(dev.msg_nb == 0)
    outb(dev.addr + NE2000_ISR, NE2000_ISR_PRX); // Clear PRX flag
}
/**
 *  @brief Reads data from the corresponding network stack
 *
 *  Reads enqueued data in the stack partition.
 */
void rtl8029_read (pok_port_id_t port_id, void* data, uint32_t len)
{
  char	*dest = data;
  pok_queue_t* queue = dev.recv_buf + port_id;
  uint32_t	size = len < queue->len ? len : queue->len;
  uint32_t	copied = 0;
  
#ifdef POK_NEEDS_DEBUG
  printf ("[RTL8029] READ DATA FROM LOCAL PORT %d, "
	  "size=%d\n", port_id, len);
#endif
  
  /* is there something to read ? */
  if (queue->len == 0)
  {
#ifdef POK_NEEDS_DEBUG
    printf("rtl8029_read: error: empty read ring buffer %d!\n", port_id);
#endif
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


#ifdef POK_NEEDS_DEBUG
void display_mac(char*mac)
{
  printf("%x:", mac[0]);
  printf("%x:", mac[1]);
  printf("%x:", mac[2]);
  printf("%x:", mac[3]);
  printf("%x:", mac[4]);
  printf("%x\n", mac[5]);
}
#endif

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
  unsigned char	  state; // ISR state

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
      uint32_t msg_len = len;
      memcpy(packet.eth.src, dev.mac, ETH_MAC_LEN);
#ifdef POK_NEEDS_MAC_ADDR
      uint8_t node_id = 0;
      pok_port_virtual_node(dest, &node_id);
      pok_node_mac_addr(&node_id, node2);
#endif

#ifdef POK_NEEDS_DEBUG
      printf ("[RTL8029] SEND DATA THROUGH NETWORK FROM LOCAL PORT %d "
	      "TO GLOBAL PORT %d, size=%d\n", port_id, dest, len);
      printf("[RTL8029] DESTINATION MAC ADDR: ");
      display_mac(node2);
#endif
      
      memcpy(packet.eth.dst, node2, ETH_MAC_LEN);
      packet.eth.ethertype = 0x4242;
      packet.udp.src = port_id;
      packet.udp.dst = dest;

      for (d = data; msg_len != 0; msg_len -= cpylen, d += cpylen)
      {
	// too short; let's cut
	if (msg_len <= NET_DATA_MINLEN)
	{
	  cpylen = msg_len;
	  sndlen = ETH_DATA_MINLEN + sizeof(eth_hdr_t);
	}
	else
	{
	  // too big; let's pad
	  if (msg_len >= NET_DATA_MAXLEN)
	  {
	    cpylen = NET_DATA_MAXLEN;
	    sndlen = ETH_DATA_MAXLEN + sizeof(eth_hdr_t);
	  }
	  // normal
	  else
	  {
	    cpylen = msg_len;
	    sndlen = sizeof(eth_hdr_t) + sizeof(udp_hdr_t) + cpylen;
	  }
	}

	packet.udp.len = cpylen;
	memcpy(&(packet.data), d, cpylen);

	ne2000_write(&dev, &packet, sndlen, NE2000_TXBUF * 256);

	do
	{
	  state = inb(dev.addr + NE2000_ISR);
	}
	while ((state & NE2000_ISR_RDC) != NE2000_ISR_RDC);

	/* This register sets the start page address of
	   the packet to the transmitted. */
	outb(dev.addr + NE2000_TPSR, NE2000_TXBUF); //?

	/* These two registers set the byte counts of
	   the packet to be transmitted. */
	outb(dev.addr + NE2000_TBCR0, sndlen);
	outb(dev.addr + NE2000_TBCR1, sndlen >> 8);

	/* This bit must be set to transmit a packet. */
	outb(dev.addr + NE2000_CR,
	      inb(dev.addr + NE2000_CR) | NE2000_CR_TXP);

	outb(dev.addr + NE2000_ISR,
	     NE2000_ISR_RDC); // Clear RDC bit
      }
    }
  }
}

/**
 *  @brief Polls rtl8029 device.
 *
 *  Watches for events, typically for receiving queued packets.
 */
void rtl8029_poll ()
{
  unsigned char	state; // ISR state

  NE2000_SELECT_PAGE(&dev, 0);

  // do we have an interrupt flag set?
  if ((state = inb(dev.addr + NE2000_ISR)) == 0)
    return;
  
  if (state & NE2000_ISR_PRX)
  {
    if ((inb(dev.addr + NE2000_RSR) & NE2000_RSR_PRX) == 0)
    {
      // error
    }

#ifdef POK_NEEDS_DEBUG
    printf("[*]\n");
#endif
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
      start = inb(dev.addr + NE2000_BNRY) + 1;

      /* This register points to the page address of the first receive
	 buffer page to be used for a packet reception. */
      NE2000_SELECT_PAGE(&dev, 1);
      end = inb(dev.addr + NE2000_CURR);
      NE2000_SELECT_PAGE(&dev, 0);

      if ((end % NE2000_MEMSZ) == (start % NE2000_MEMSZ) + 1)
      {
	outb(dev.addr + NE2000_ISR, NE2000_ISR_PRX); // Clear PRX flag
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
      outb(dev.addr + NE2000_BNRY,
	   ne2000_hdr.next > NE2000_MEMSZ ? NE2000_RXBUF - 1 : ne2000_hdr.next - 1);

    }

    outb(dev.addr + NE2000_ISR, NE2000_ISR_PRX); // Clear PRX flag
  }
}

void rtl8029_clear()
{
  unsigned char	state; // ISR state

  NE2000_SELECT_PAGE(&dev, 0);

  // do we have an interrupt flag set?
  if ((state = inb(dev.addr + NE2000_ISR)) == 0)
    return;
  
  if (state & NE2000_ISR_PTX)
  {
    outb(dev.addr + NE2000_ISR, NE2000_ISR_PTX); // Clear PTX flag
  }

  if (state & NE2000_ISR_RXE)
  {
    outb(dev.addr + NE2000_ISR, NE2000_ISR_RXE); // Clear RXE flag
  }

  if (state & NE2000_ISR_TXE)
  {
    outb(dev.addr + NE2000_ISR, NE2000_ISR_TXE); // Clear TXE flag
  }

  if (state & NE2000_ISR_OVW)
  {
    outb(dev.addr + NE2000_ISR, NE2000_ISR_OVW); // Clear OVW flag
  }

  if (state & NE2000_ISR_CNT)
  {
    outb(dev.addr + NE2000_ISR, NE2000_ISR_CNT); // Clear CNT flag
  }

  if (state & NE2000_ISR_RST)
  {
    outb(dev.addr + NE2000_ISR, NE2000_ISR_RST); // Clear RST bit
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
  static char init = 0;
  if(init)
    return;
  init++;

  dev.msg_nb = 0;
  dev.pci.vendorid = 0x10ec;
  dev.pci.deviceid = 0x8029;
  dev.pci.io_range = 0x10;

  if (pci_register(&(dev.pci), NULL) != 0)
  {
#ifdef POK_NEEDS_DEBUG
    printf("rtl8029: PCI init failed!\n");
#endif
    return;
  }

  dev.addr = dev.pci.bar[0] & (~0x1F);
  
  unsigned char	i = 0;
  unsigned char	buf[6 * 2]; // used for MAC address

  NE2000_SELECT_PAGE(&dev, 0);

  /* This bit is the STOP command. When it is set, no packets will be
     received or transmitted. POWER UP=1. */
  outb(dev.addr + NE2000_CR, NE2000_CR_STP);

  // Sets several options... Read the datasheet!
  outb(dev.addr + NE2000_TCR, 0x00);
  outb(dev.addr + NE2000_RCR, NE2000_RCR_AB);
  outb(dev.addr + NE2000_DCR, NE2000_DCR_LS | NE2000_DCR_FT1);

  /* The Page Start register sets the start page address
     of the receive buffer ring. */
  outb(dev.addr + NE2000_PSTART, NE2000_RXBUF);
  /* The Page Stop register sets the stop page address
     of the receive buffer ring. */
  outb(dev.addr + NE2000_PSTOP, NE2000_MEMSZ);
  /* This register is used to prevent overwrite of the receive buffer ring.
     It is typically used as a pointer indicating the last receive buffer
     page the host has read. */
  outb(dev.addr + NE2000_BNRY, NE2000_RXBUF);

  /* These two registers set the data byte counts of remote DMA. */
  outb(dev.addr + NE2000_RBCR0, 0);
  outb(dev.addr + NE2000_RBCR1, 0);

  NE2000_SELECT_PAGE(&dev, 1);

  /* This register points to the page address of the first receive buffer
     page to be used for a packet reception. */
  outb(dev.addr + NE2000_CURR, NE2000_RXBUF + 1);

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
  outb(dev.addr + NE2000_PAR0, dev.mac[0]);
  outb(dev.addr + NE2000_PAR1, dev.mac[1]);
  outb(dev.addr + NE2000_PAR2, dev.mac[2]);
  outb(dev.addr + NE2000_PAR3, dev.mac[3]);
  outb(dev.addr + NE2000_PAR4, dev.mac[4]);
  outb(dev.addr + NE2000_PAR5, dev.mac[5]);

#ifdef POK_NEEDS_DEBUG
  printf ("[RTL8029] INIT NODE WITH MAC ADDR: ");
  display_mac(dev.mac);
#endif
  
  NE2000_SELECT_PAGE(&dev, 0);

  // Start command
  outb(dev.addr + NE2000_CR, NE2000_CR_STA);

  // Reactivating interrupts
  /* ISR register must be cleared after power up. */
  outb(dev.addr + NE2000_ISR, 0xFF);
  /* All bits correspond to the bits in the ISR register. POWER UP=all 0s.
     Setting individual bits will enable the corresponding interrupts. */
  /* Since POK use polling, ALL interrupts are disabled */
  outb(dev.addr + NE2000_IMR, 0x00);

#if (defined POK_CONFIG_NB_INPUT_VIRTUALPORT) && (POK_CONFIG_NB_INPUT_VIRTUALPORT > 0)
  for (i = 0; i < POK_CONFIG_NB_INPUT_VIRTUALPORT; i++)
  {
    dev.recv_buf[i].len = 0;
    dev.recv_buf[i].off = 0;
  }
#endif
  
  return;
}

#endif
