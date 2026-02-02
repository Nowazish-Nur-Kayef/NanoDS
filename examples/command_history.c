/**
 * @file command_history.c
 * @brief Real-world example: Command-line history using NanoVector
 * 
 * Demonstrates: 
 * - Vector usage for sequential data
 * - Custom struct storage
 * - Search and filtering
 */

#define NANODS_IMPLEMENTATION
#include "../nanods.h"
#include <stdio.h>
#include <string.h>

#define MAX_CMD_LEN 128

typedef struct {
    char cmd[MAX_CMD_LEN];
    int timestamp;
} Command;

NANODS_DEFINE_VECTOR(Command)

void add_command(NanoVector_Command* history, const char* cmd, int timestamp) {
    Command c;
    strncpy(c.cmd, cmd, MAX_CMD_LEN - 1);
    c.cmd[MAX_CMD_LEN - 1] = '\0';
    c.timestamp = timestamp;
    nv_push_Command(history, c);
}

void display_history(const NanoVector_Command* history) {
    printf("\nCommand History:\n");
    printf("─────────────────────────────────────────────\n");
    
    for (size_t i = 0; i < nv_size_Command(history); i++) {
        Command cmd;
        nv_get_Command(history, i, &cmd);
        printf("%3zu [T=%04d] %s\n", i + 1, cmd.timestamp, cmd.cmd);
    }
    
    printf("─────────────────────────────────────────────\n");
    printf("Total commands: %zu\n\n", nv_size_Command(history));
}

void search_history(const NanoVector_Command* history, const char* search_term) {
    printf("Searching for: '%s'\n", search_term);
    printf("─────────────────────────────────────────────\n");
    
    int found = 0;
    for (size_t i = 0; i < nv_size_Command(history); i++) {
        Command cmd;
        nv_get_Command(history, i, &cmd);
        
        if (strstr(cmd.cmd, search_term) != NULL) {
            printf("%3zu [T=%04d] %s\n", i + 1, cmd.timestamp, cmd.cmd);
            found++;
        }
    }
    
    if (found == 0) {
        printf("No matches found.\n");
    } else {
        printf("─────────────────────────────────────────────\n");
        printf("Found:  %d matches\n", found);
    }
    printf("\n");
}

void get_recent_commands(const NanoVector_Command* history, int n) {
    printf("Last %d commands:\n", n);
    printf("─────────────────────────────────────────────\n");
    
    size_t total = nv_size_Command(history);
    size_t start = total > (size_t)n ? total - n : 0;
    
    for (size_t i = start; i < total; i++) {
        Command cmd;
        nv_get_Command(history, i, &cmd);
        printf("%3zu [T=%04d] %s\n", i + 1, cmd.timestamp, cmd.cmd);
    }
    printf("\n");
}

void clear_old_commands(NanoVector_Command* history, int threshold) {
    NanoVector_Command new_history;
    nv_init_Command(&new_history);
    
    for (size_t i = 0; i < nv_size_Command(history); i++) {
        Command cmd;
        nv_get_Command(history, i, &cmd);
        
        if (cmd.timestamp >= threshold) {
            nv_push_Command(&new_history, cmd);
        }
    }
    
    size_t removed = nv_size_Command(history) - nv_size_Command(&new_history);
    
    nv_free_Command(history);
    *history = new_history;
    
    printf("🗑️  Removed %zu old commands (older than T=%d)\n\n", removed, threshold);
}

int main(void) {
    printf("==============================================\n");
    printf("  Command History Manager (NanoDS v%s)\n", NANODS_VERSION);
    printf("==============================================\n\n");
    
    nanods_seed_init(0);
    
    NanoVector_Command history;
    nv_init_Command(&history);
    
    printf("Simulating user commands...\n\n");
    
    add_command(&history, "ls -la", 1000);
    add_command(&history, "cd /home/user", 1005);
    add_command(&history, "git status", 1010);
    add_command(&history, "git add .", 1015);
    add_command(&history, "git commit -m 'Initial commit'", 1020);
    add_command(&history, "git push origin main", 1025);
    add_command(&history, "make clean", 1030);
    add_command(&history, "make all", 1035);
    add_command(&history, "./program", 1040);
    add_command(&history, "git log --oneline", 1045);
    
    display_history(&history);
    search_history(&history, "git");
    get_recent_commands(&history, 3);
    clear_old_commands(&history, 1020);
    display_history(&history);
    
    printf("📊 Statistics:\n");
    printf("   Size:      %zu commands\n", nv_size_Command(&history));
    printf("   Capacity: %zu slots\n", history.capacity);
    printf("   Memory:   %zu bytes\n\n", history.capacity * sizeof(Command));
    
    nv_free_Command(&history);
    
    printf("✅ Done!\n");
    return 0;
}