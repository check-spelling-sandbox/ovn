/*
 * Copyright (c) 2023, Red Hat, Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at:
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#ifndef EN_PORT_GROUP_H
#define EN_PORT_GROUP_H 1

#include <stdint.h>

#include "lib/inc-proc-eng.h"
#include "lib/ovn-nb-idl.h"
#include "lib/ovn-sb-idl.h"
#include "openvswitch/hmap.h"

#include "sset.h"

/* Per logical switch port group information. */
struct ls_port_group_table {
    struct hmap entries; /* Stores struct ls_port_group. */
};

struct ls_port_group {
    struct hmap_node key_node;  /* Index on 'nbs->header_.uuid'. */

    const struct nbrec_logical_switch *nbs;
    int64_t sb_datapath_key; /* SB.Datapath_Binding.tunnel_key. */

    /* Port groups with ports attached to 'nbs'. */
    struct hmap nb_pgs; /* Stores struct ls_port_group_record. */
};

struct ls_port_group_record {
    struct hmap_node key_node;  /* Index on 'nb_pg->header_.uuid'. */

    const struct nbrec_port_group *nb_pg;
    struct sset ports;          /* Subset of 'nb_pg' ports in this record. */
};

void ls_port_group_table_init(struct ls_port_group_table *);
void ls_port_group_table_clear(struct ls_port_group_table *);
void ls_port_group_table_destroy(struct ls_port_group_table *);
struct ls_port_group *ls_port_group_table_find(
    const struct ls_port_group_table *,
    const struct nbrec_logical_switch *);

void ls_port_group_table_build(struct ls_port_group_table *ls_port_groups,
                               const struct nbrec_port_group_table *,
                               const struct hmap *ls_ports);
void ls_port_group_table_sync(const struct ls_port_group_table *ls_port_groups,
                              const struct sbrec_port_group_table *,
                              struct ovsdb_idl_txn *ovnsb_txn);
#endif /* EN_PORT_GROUP_H */