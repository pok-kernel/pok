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
 * Created by julien on Thu Jan 15 23:34:13 2009 
 */


#include <types.h>
#include <middleware/port.h>

uint8_t          pok_ports_nb_ports_by_partition[POK_CONFIG_NB_PARTITIONS] = {1,1};

uint8_t           pok_pr1_ports[1] = {dataout};

uint8_t           pok_pr2_ports[1] = {datain};

uint8_t*          pok_ports_by_partition[POK_CONFIG_NB_PARTITIONS] = {pok_pr1_ports, pok_pr2_ports};

char*             pok_ports_names[POK_CONFIG_NB_PORTS] = {"dataout","datain"};

uint8_t           pok_ports_identifiers[POK_CONFIG_NB_PORTS] = {dataout, datain};

uint8_t           pok_ports_nb_destinations[POK_CONFIG_NB_PORTS] = {1,0};

uint8_t           pok_ports_dataout_destinations[1] = {datain};

uint8_t           pok_ports_datain_destinations[0] = {};

uint8_t*          pok_ports_destinations[POK_CONFIG_NB_PORTS] = {pok_ports_dataout_destinations, NULL};

uint8_t           pok_ports_kind[POK_CONFIG_NB_PORTS] = {POK_PORT_KIND_QUEUEING,POK_PORT_KIND_QUEUEING};

