/* packet-r14p.c
 * Routines for r14p packet dissection
 *
 *
 * Wireshark - Network traffic analyzer
 * By Gerald Combs <gerald@wireshark.org>
 * Copyright 1998 Gerald Combs
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 */

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <glib.h>
#include <epan/packet.h>
#include <epan/conversation.h>
#include <epan/dissectors/packet-ber.h>

#include <stdio.h>
#include <string.h>

#include "packet-r14p.h"

#define PNAME  "R14P"
#define PSNAME "r14p"
#define PFNAME "r14p"
#define R14P_PORT 0    /* TCP port */

/* Initialize the protocol and registered fields */
static int proto_r14p = -1;
static int global_r14p_port = R14P_PORT;

#include "packet-r14p-hf.c"

/* Initialize the subtree pointers */
static int ett_r14p = -1;

#include "packet-r14p-ett.c"

#include "packet-r14p-fn.c"


static void
dissect_r14p(tvbuff_t *tvb, packet_info *pinfo, proto_tree *tree)
{
    proto_item      *r14p_item = NULL;
    proto_tree      *r14p_tree = NULL;

    /* make entry in the Protocol column on summary display */
    col_set_str(pinfo->cinfo, COL_PROTOCOL, PNAME);

    /* create the r14p protocol tree */
    if (tree) {
        r14p_item = proto_tree_add_item(tree, proto_r14p, tvb, 0, -1, FALSE);
        r14p_tree = proto_item_add_subtree(r14p_item, ett_r14p);
        dissect_R14PMessage_PDU(tvb, pinfo, r14p_tree, 0);
    }
}
/*--- proto_register_r14p -------------------------------------------*/
void proto_register_r14p(void) {

  /* List of fields */
  static hf_register_info hf[] = {

#include "packet-r14p-hfarr.c"
  };

  /* List of subtrees */
  static gint *ett[] = {
                  &ett_r14p,
#include "packet-r14p-ettarr.c"
  };


  /* Register protocol */
  proto_r14p = proto_register_protocol(PNAME, PSNAME, PFNAME);
  /* Register fields and subtrees */
  proto_register_field_array(proto_r14p, hf, array_length(hf));
  proto_register_subtree_array(ett, array_length(ett));
}


/*--- proto_reg_handoff_r14p ---------------------------------------*/
void
proto_reg_handoff_r14p(void)
{
  static gboolean initialized = FALSE;
  static dissector_handle_t r14p_handle;
  static guint r14p_port;

  if (!initialized) {
    r14p_handle = create_dissector_handle(dissect_r14p, proto_r14p);
    //dissector_add_handle("sctp.ppi", r14p_handle);
    dissector_add_for_decode_as("sctp.ppi", r14p_handle);
    dissector_add_uint("sctp.ppi", 48, r14p_handle);
    initialized = TRUE;
  } else {
    if (r14p_port != 0) {
      dissector_delete_uint("sctp.ppi", r14p_port, r14p_handle);
    }
  }
  if (global_r14p_port != 0) {
    dissector_add("sctp.ppi", global_r14p_port, r14p_handle);
  }
  r14p_port = global_r14p_port;
}
