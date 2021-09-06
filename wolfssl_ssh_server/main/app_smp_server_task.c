/* app_smp_server_task.c
 *
 * Copyright (C) 2006-2021 wolfSSL Inc.
 *
 * This file is part of wolfSSL.
 *
 * wolfSSL is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * wolfSSL is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1335, USA
 */
 /* the usual suspects */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

/* socket includes */
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>

/* wolfSSL */
#include <wolfssl/wolfcrypt/settings.h>
#include <wolfssl/ssl.h>
#include <wolfssl/certs_test.h>

/* wolfSSL SSH */
#include <wolfssl/wolfcrypt/sha256.h>
#include <wolfssl/wolfcrypt/coding.h>
#include <wolfssh/ssh.h>
#include <wolfssh/test.h>
#include <wolfssl/wolfcrypt/ecc.h>


/* ESP specific */
#include "wifi_connect.h"

#include "server.h"

#ifdef WOLFSSL_TRACK_MEMORY
#include <wolfssl/wolfcrypt/mem_track.h>
#endif

int a = WS_CHANNEL_ID_SELF; // this is just a test to confirm we can access wolfssh

// static const char* const TAG = "app_server";

#if defined(DEBUG_WOLFSSL)

static void ShowCiphers(void)
{
    char ciphers[4096];

    int ret = wolfSSL_get_ciphers(ciphers, (int)sizeof(ciphers));

    if (ret == WOLFSSL_SUCCESS)
        printf("%s\n", ciphers);
}

#endif

#if defined(WOLFSSL_ESPWROOM32SE) && defined(HAVE_PK_CALLBACKS) \
                                  && defined(WOLFSSL_ATECC508A)

#include "wolfssl/wolfcrypt/port/atmel/atmel.h"

/* when you want to use a custom slot allocation */
/* enable the definition CUSTOM_SLOT_ALLOCATION. */

#if defined(CUSTOM_SLOT_ALLOCATION)

static byte mSlotList[ATECC_MAX_SLOT];

int atmel_set_slot_allocator(atmel_slot_alloc_cb alloc, atmel_slot_dealloc_cb dealloc);

/* initialize slot array */
void my_atmel_slotInit()
{
    int i;

    for (i = 0; i < ATECC_MAX_SLOT; i++) {
        mSlotList[i] = ATECC_INVALID_SLOT;
    }
}

/* allocate slot depending on slotType */
int my_atmel_alloc(int slotType)
{
    int i, slot = -1;

    switch (slotType) {
    case ATMEL_SLOT_ENCKEY:
        slot = 4;
        break;
    case ATMEL_SLOT_DEVICE:
        slot = 0;
        break;
    case ATMEL_SLOT_ECDHE:
        slot = 0;
        break;
    case ATMEL_SLOT_ECDHE_ENC:
        slot = 4;
        break;
    case ATMEL_SLOT_ANY:
        for (i = 0; i < ATECC_MAX_SLOT; i++) {
            if (mSlotList[i] == ATECC_INVALID_SLOT) {
                slot = i;
                break;
            }
        }
    }

    return slot;
}

/* free slot array       */
void my_atmel_free(int slotId)
{
    if (slotId >= 0 && slotId < ATECC_MAX_SLOT) {
        mSlotList[slotId] = ATECC_INVALID_SLOT;
    }
}
#endif /* CUSTOM_SLOT_ALLOCATION                                       */
#endif /* WOLFSSL_ESPWROOM32SE && HAVE_PK_CALLBACK && WOLFSSL_ATECC508A */

// this task name is hard-coded in our local wifi_connect.c
void app_smp_server_task()
{
    void* args = NULL;

    printf("WSTARTTCP\n");
    WSTARTTCP();
    printf("WSTARTTCP done!\n");

    // TODO what's this?
    // ChangeToWolfSshRoot();

#ifdef DEBUG_WOLFSSH
    //wolfSSH_Debugging_ON();
    printf("wolfSSH_Debugging_ON done!\n");
#endif

    wolfSSH_Init();
    printf("wolfSSH_Init done!\n");

    // wolfSSL_CTX_free(ctx);  /* Free the wolfSSL context object          */
    wolfSSL_Cleanup();      /* Cleanup the wolfSSL environment          */


    server_test(args);
    // wolfSSH_Init();

   // int r = main_alt(args, args);

}
