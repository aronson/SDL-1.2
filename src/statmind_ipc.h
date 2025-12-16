#include "SDL_events.h"
#include "SDL_keyboard.h"
#include "SDL_thread.h"
#include "SDL_timer.h"
#include <stdio.h> // For printf

// Magic number to identify the mailbox in memory
#define STATMIND_MAGIC 0x64ADFA4D
#define STATMIND_CHECK 0x79533ED8

typedef volatile struct {
    unsigned int magic;
    unsigned int check;
    char command; // 0 = None, 'M' = Move, 'K' = Key
    unsigned char data;
    char pad[2]; // Explicit padding to make size 8 bytes
} StatmindMailbox __attribute__((packed, aligned(4)));

// Global mailbox instance - accessible via memory scanning
__attribute__((dllexport))
__attribute__((used))
StatmindMailbox g_statmind_mailbox = {STATMIND_MAGIC, STATMIND_CHECK, 0, 0, {0, 0}};

// Struct for IPC thread status
typedef struct {
    unsigned int magic; // Magic number for this struct
    unsigned int check; // Magic number for this struct
    volatile int status; // 0 = not started, 1 = started
} StatmindThreadStatus;

#define THREAD_STATUS_MAGIC 0xBADBEEF1
#define THREAD_STATUS_CHECK 0x79533ED9

__attribute__((dllexport))
__attribute__((used))
StatmindThreadStatus g_ipc_thread_status = {THREAD_STATUS_MAGIC, THREAD_STATUS_CHECK, 0};

static int Statmind_IPCThread(void *data);

static int ipc_thread_running = 0;

static void Statmind_StartIPC()
{
    if (!ipc_thread_running) {
        printf("[Statmind_StartIPC] Creating IPC thread.\n");
        fflush(stdout);
        SDL_CreateThread(Statmind_IPCThread, NULL);
        ipc_thread_running = 1;
    } else {
        printf("[Statmind_StartIPC] IPC thread already running.\n");
        fflush(stdout);
    }
}

static int Statmind_IPCThread(void *data)
{
    (void)data; // Suppress unused parameter warning
    g_ipc_thread_status.status = 1; // Mark as started
    printf("[Statmind_IPCThread] IPC thread started. Mailbox magic: 0x%X\n", g_statmind_mailbox.magic);
    fflush(stdout);

    printf("[Statmind_IPCThread] Entering polling loop.\n");
    fflush(stdout);

    while (1)
    {
        printf("[Statmind_IPCThread] Polling. Mailbox command raw: %d\n", g_statmind_mailbox.command);
        fflush(stdout);

        if (g_statmind_mailbox.command != 0)
        {
            printf("[Statmind_IPCThread] Mailbox command raw value: %d\n", g_statmind_mailbox.command);
            fflush(stdout);
            char cmd = g_statmind_mailbox.command;
            unsigned char val = g_statmind_mailbox.data;

            printf("[Statmind_IPCThread] Command detected: cmd='%c', data=%u\n", cmd, val);
            fflush(stdout);

            SDL_Event event;
            // Clear event
            memset(&event, 0, sizeof(event));

            SDLKey sym = SDLK_UNKNOWN;

            if (cmd == 'M')
            { // Movement (Numpad)
                // Data is 1-9
                if (val >= 0 && val <= 9)
                {
                    sym = (SDLKey)(SDLK_KP0 + val);
                }
            }
            else if (cmd == 'K')
            { // Standard Key
                // Data is ASCII
                sym = (SDLKey)val;
            }

            if (sym != SDLK_UNKNOWN)
            {
                printf("[Statmind_IPCThread] Pushing KeyDown event for sym %d\n", sym);
                fflush(stdout);
                // KeyDown
                event.type = SDL_KEYDOWN;
                event.key.state = SDL_PRESSED;
                event.key.keysym.sym = sym;
                event.key.keysym.mod = KMOD_NONE;
                SDL_PushEvent(&event);

                // Small delay to ensure game registers it
                // SDL_Delay(10);

                printf("[Statmind_IPCThread] Pushing KeyUp event for sym %d\n", sym);
                fflush(stdout);
                // KeyUp
                event.type = SDL_KEYUP;
                event.key.state = SDL_RELEASED;
                event.key.keysym.sym = sym;
                event.key.keysym.mod = KMOD_NONE;
                SDL_PushEvent(&event);
            }

            // Acknowledge command execution by clearing the command
            g_statmind_mailbox.command = 0;
            printf("[Statmind_IPCThread] Command cleared.\n");
            fflush(stdout);
        }

        // SDL_Delay(10); // Poll every 10ms
    }
    return 0;
}
