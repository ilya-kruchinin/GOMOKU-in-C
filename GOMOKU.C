/* GOMOKU 15x15 - Turbo C MS-DOS
Compile with Turbo C 2.0/3.0 (DOS)
Requires EGA/VGA graphics mode and BGI drivers
Updates:
Smarter AI with advanced pattern recognition (handles gaps), center control, and 1-ply lookahead.
Progressive AI difficulty: Different rival groups per level, Rematch option, and Infinite Elite Mode after Level 5.
*/
#include <graphics.h>
#include <dos.h>
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>

#define BOARD_SIZE 15
#define CELL_SIZE 30
#define MARGIN 40
#define EMPTY 0
#define STONE_BLACK 1
#define STONE_WHITE 2

/* Game State */
int board[BOARD_SIZE][BOARD_SIZE];
int cursor_x = 7, cursor_y = 7;
int current_player = STONE_BLACK;
int game_mode = 0; /* 1=PvP, 2=PvC */
int game_over = 0;
int winner = 0;    /* 0=draw, 1=STONE_BLACK, 2=STONE_WHITE */
int move_count = 0;
int ai_level = 1;  /* AI difficulty level */
char current_rival[50]; /* Stores the specific rival's name for rematches */

/* Different Rival Groups for Each Level */
char rival_names[6][6][20] = {
/*  Level 1: School Beginners */
{ "Novice Ken", "Rookie Aoi", "Beginner Jin", "Apprentice Mai", "Newbie Yuma", "Starter Sota" },
/*  Level 2: Club Amateurs */
{ "Student Haruto", "Amateur Mei", "Club Member Ren", "Scholar Naoki", "Trainee Kenta", "Hobbyist Shin" },
/*  Level 3: Regional Experts */
{ "Expert Hiroshi", "Master Yuki", "Veteran Satoshi", "Adept Chiyo", "Old Man Kuze", "Tactician Emi" },
/*  Level 4: National Masters */
{ "Pro Takumi", "Grandmaster Ryu", "Champion Kazuya", "Sensei Hideo", "Elite Ichiro", "Ace Yoshi" },
/*  Level 5: World Legends */
{ "Legend Akira", "Mythical Yuto", "God of Go Shota", "Immortal Sakura", "Supreme Minato", "Divine Yoko" },
/*  Level 6+: Infinite Elite (Harder) */
{ "Cyber Alpha", "Neural Deep", "Quantum Ren", "Matrix Yuki", "Omega Akira", "Singularity Jin" }
};

/* Bitmap font data for Japanese title (12x12 per character) */
/* 1 = pixel on, 0 = pixel off */
/* Kanji */
unsigned char kanji_bitmap[] = {
    /* 五 */
    0,0,0,0,0,0,0,0,0,0,0,0,
    1,1,1,1,1,1,1,1,1,1,1,0,
    0,0,0,0,1,0,0,0,0,0,0,0,
    0,0,0,0,1,0,0,0,0,0,0,0,
    0,0,0,1,1,1,1,1,1,0,0,0,
    0,0,0,1,0,0,0,0,1,0,0,0,
    0,0,0,1,0,0,0,0,1,0,0,0,
    0,0,1,0,0,0,0,0,1,0,0,0,
    1,1,1,1,1,1,1,1,1,1,1,0,
    0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,
    /* 目 */
    0,0,0,0,0,0,0,0,0,0,0,0,
    1,1,1,1,1,1,1,1,1,1,0,0,
    1,0,0,0,0,0,0,0,0,1,0,0,
    1,1,1,1,1,1,1,1,1,1,0,0,
    1,0,0,0,0,0,0,0,0,1,0,0,
    1,1,1,1,1,1,1,1,1,1,0,0,
    1,0,0,0,0,0,0,0,0,1,0,0,
    1,1,1,1,1,1,1,1,1,1,0,0,
    1,0,0,0,0,0,0,0,0,1,0,0,
    1,0,0,0,0,0,0,0,0,1,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,
    /* 並 */
    0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,1,0,0,0,0,1,0,0,0,
    0,0,0,0,1,0,0,1,0,0,0,0,
    1,1,1,1,1,1,1,1,1,1,1,0,
    0,0,0,0,1,0,0,1,0,0,0,0,
    0,1,0,0,1,0,0,1,0,0,1,0,
    0,0,1,0,1,0,0,1,0,1,0,0,
    0,0,0,0,1,0,0,1,0,0,0,0,
    1,1,1,1,1,1,1,1,1,1,1,0,
    0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,
    /* べ */
    0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,1,0,0,
    0,0,0,0,0,0,0,0,0,0,1,0,
    0,0,0,0,0,0,0,1,0,0,0,0,
    0,0,0,0,1,0,0,0,1,0,0,0,
    0,0,0,1,0,1,1,0,0,0,0,0,
    0,0,1,0,0,0,0,1,1,0,0,0,
    0,0,1,0,0,0,0,0,0,1,0,0,
    0,1,0,0,0,0,0,0,0,0,1,0,
    0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0
};

/* Hiragana */
unsigned char hira_bitmap[] = {
    /* ご */
    0,0,0,0,0,0,0,0,0,0,1,0,
    0,0,0,0,0,0,0,0,1,0,0,1,
    0,0,0,0,0,0,0,0,0,1,0,0,
    1,1,1,1,1,1,1,1,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,1,0,0,0,0,0,0,0,0,0,
    0,1,0,0,0,0,0,0,0,0,0,0,
    0,1,1,0,0,0,0,0,0,0,0,0,
    0,0,1,1,1,1,1,1,1,1,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,
    /* も */
    0,0,0,0,1,0,0,0,0,0,0,0,
    0,0,0,0,1,0,0,0,0,0,0,0,
    1,1,1,1,1,1,0,0,0,0,0,0,
    0,0,0,1,0,0,0,0,0,0,0,0,
    0,0,0,1,0,0,0,0,0,0,0,0,
    1,1,1,1,1,1,0,1,0,0,0,0,
    0,0,1,0,0,0,0,0,1,0,0,0,
    0,0,1,1,1,0,0,0,1,1,0,0,
    0,0,0,0,1,1,1,1,1,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,
    /* く */
    0,0,0,0,0,0,0,1,0,0,0,0,
    0,0,0,0,0,0,1,0,0,0,0,0,
    0,0,0,0,0,1,0,0,0,0,0,0,
    0,0,0,0,1,0,0,0,0,0,0,0,
    0,0,0,1,0,0,0,0,0,0,0,0,
    0,0,1,0,0,0,0,0,0,0,0,0,
    0,0,0,1,0,0,0,0,0,0,0,0,
    0,0,0,0,1,0,0,0,0,0,0,0,
    0,0,0,0,0,1,0,0,0,0,0,0,
    0,0,0,0,0,0,1,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,
    /* な */
    0,0,0,1,0,0,0,0,0,0,0,0,
    0,0,0,1,0,0,0,0,0,0,0,0,
    1,1,1,1,1,1,1,0,1,0,0,0,
    0,0,0,1,0,0,0,0,0,1,1,0,
    0,0,0,1,0,0,0,1,0,0,0,1,
    0,0,1,0,0,0,0,1,0,0,0,0,
    0,0,0,0,0,1,1,1,0,0,0,0,
    0,0,0,0,1,0,0,1,1,0,0,0,
    0,0,0,0,0,1,1,0,0,1,1,0,
    0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,
    /* ら */
    0,0,0,0,0,1,0,0,0,0,0,0,
    0,0,0,1,0,0,1,1,0,0,0,0,
    0,0,0,1,0,0,0,0,0,0,0,0,
    0,0,1,0,0,0,0,0,0,0,0,0,
    0,0,1,0,1,1,1,0,0,0,0,0,
    0,1,0,1,0,0,0,1,1,1,0,0,
    0,1,1,0,0,0,0,0,0,1,1,0,
    0,0,0,0,0,0,0,0,0,1,0,0,
    0,0,0,0,0,0,1,1,1,0,0,0,
    0,0,0,0,1,1,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,
    /* べ */
    0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,1,0,0,
    0,0,0,0,0,0,0,1,0,0,1,0,
    0,0,0,0,0,0,0,0,1,0,0,0,
    0,0,0,0,1,1,0,0,0,0,0,0,
    0,0,0,1,0,0,1,0,0,0,0,0,
    0,0,0,1,0,0,0,1,1,0,0,0,
    0,0,1,0,0,0,0,0,1,1,0,0,
    0,0,0,0,0,0,0,0,0,0,1,0,
    0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0
};

/* Function to draw bitmap text character by character */
void draw_bitmap_text(int x, int y, int scale, unsigned char *bitmap, int char_w, int char_h, int num_chars, int color) {
    int i, j, k, px, py;
    for (k = 0; k < num_chars; k++) {
        for (i = 0; i < char_h; i++) {
            for (j = 0; j < char_w; j++) {
                if (bitmap[k * char_h * char_w + i * char_w + j]) {
                    px = x + k * char_w * scale + j * scale;
                    py = y + i * scale;
                    setfillstyle(SOLID_FILL, color);
                    bar(px, py, px + scale - 1, py + scale - 1);
                }
            }
        }
    }
}

/* Function Prototypes */
void init_board(void);
void draw_status(void);
void draw_win_message(void);
void draw_congratulations(void);
void render_cell(int r, int c);
void draw_static_board(void);
void erase_cursor(int x, int y);
void draw_cursor(int x, int y);
int check_win(int r, int c, int player);
int is_draw(void);
long evaluate_move(int r, int c, int player);
long get_move_score(int r, int c, int player, int level);
int is_candidate(int r, int c);
void ai_move(void);
void init_graphics(void);
void show_menu(void);
void get_rival_name(int level, char *name_buf);

/* Get a random rival name based on the current level */
void get_rival_name(int level, char *name_buf) {
    int idx = random(6);
    int group = level;
    if (group > 6) group = 6; /* Cap at group 6 for infinite scaling */
    if (level > 6) {
        sprintf(name_buf, "%s (Lvl %d)", rival_names[group - 1][idx], level);
    } else {
        strcpy(name_buf, rival_names[group - 1][idx]);
    }
}

/* Initialize empty board */
void init_board(void) {
    int i, j;
    for (i = 0; i < BOARD_SIZE; i++)
        for (j = 0; j < BOARD_SIZE; j++)
            board[i][j] = EMPTY;
    move_count = 0;
}

/* Draw status labels on the RIGHT side column */
void draw_status(void) {
    char msg[80];
    /* Clear right panel area to prevent flicker (Fixed boundary: max Y is 479 for 640x480) */
    setfillstyle(SOLID_FILL, BLACK);
    
    /* FIX: Changed 495 to 480 to fully clear the area where text starts at X=490 */
    bar(480, 20, 639, 479); 
    
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 1);
    setcolor(YELLOW);
    if (game_over) {
        sprintf(msg, "Final Lvl: %d ", ai_level);
        outtextxy(490, 50, msg);
    } else {
        /* Turn Label */
        outtextxy(490, 50, "Turn: ");
        if (game_mode == 2) {
            if (current_player == STONE_BLACK) outtextxy(490, 70, "You (Black) ");
            else outtextxy(490, 70, current_rival);

            /* Rival Name Label (PvC only) */
            outtextxy(490, 120, "Rival: ");
            outtextxy(490, 140, current_rival);

            /* AI Level Display (PvC only) */
            outtextxy(490, 190, "Level: ");
            sprintf(msg, "%d ", ai_level);
            outtextxy(490, 210, msg);
        } else {
            if (current_player == STONE_BLACK) outtextxy(510, 70, "P1 (Black) ");
            else outtextxy(490, 70, "P2 (White) ");
        }
    }
}

/* Draw large win/draw message in the center of the screen */
void draw_win_message(void) {
    char msg[80];
    if (winner == STONE_BLACK) sprintf(msg, "Black Wins!");
    else if (winner == STONE_WHITE) sprintf(msg, "White Wins!");
    else sprintf(msg, "Draw!");
    /* Draw background box for visibility */
    setfillstyle(SOLID_FILL, DARKGRAY);
    bar(150, 220, 490, 280);
    setcolor(WHITE);
    rectangle(150, 220, 490, 280);

    settextstyle(DEFAULT_FONT, HORIZ_DIR, 2);
    outtextxy(320 - textwidth(msg)/2, 250 - textheight(msg)/2, msg);
}

/* Special message for conquering Level 5 */
void draw_congratulations(void) {
    setfillstyle(SOLID_FILL, DARKGRAY);
    bar(100, 200, 540, 300);
    setcolor(YELLOW);
    rectangle(100, 200, 540, 300);
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 2);
    outtextxy(320 - textwidth("You are the Gomoku Master!")/2, 225, "You are the Gomoku Master!");
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 1);
    setcolor(WHITE);
    outtextxy(320 - textwidth("The game continues with harder Elite rivals!")/2, 260, "The game continues with harder Elite rivals!");

    delay(3000); /* Show for 2.5 seconds */

    /* Restore the screen state */
    draw_win_message();
    draw_status();
}

/* Redraw a single cell (background, grid, star points, stone) */
void render_cell(int r, int c) {
    int cx = MARGIN + c * CELL_SIZE;
    int cy = MARGIN + r * CELL_SIZE;
    int left_x, right_x, top_y, bottom_y;
    /* Cell Background */
    setfillstyle(SOLID_FILL, BROWN);
    bar(cx - 15, cy - 15, cx + 15, cy + 15);

    /* Grid Lines */
    setcolor(BLACK);
    left_x = (c == 0) ? cx : cx - 15;
    right_x = (c == BOARD_SIZE - 1) ? cx : cx + 15;
    line(left_x, cy, right_x, cy);

    top_y = (r == 0) ? cy : cy - 15;
    bottom_y = (r == BOARD_SIZE - 1) ? cy : cy + 15;
    line(cx, top_y, cx, bottom_y);

    /* Star Points */
    if ((r==3 && c==3) || (r==3 && c==11) || (r==7 && c==7) || (r==11 && c==3) || (r==11 && c==11)) {
        setfillstyle(SOLID_FILL, BLACK);
        fillellipse(cx, cy, 4, 4);
    }

    /* Stones */
    if (board[r][c] == STONE_BLACK) {
        setfillstyle(SOLID_FILL, BLACK);
        setcolor(BLACK);
        fillellipse(cx, cy, CELL_SIZE/2 - 2, CELL_SIZE/2 - 2);
    } else if (board[r][c] == STONE_WHITE) {
        setfillstyle(SOLID_FILL, WHITE);
        setcolor(BLACK);
        fillellipse(cx, cy, CELL_SIZE/2 - 2, CELL_SIZE/2 - 2);
    }
}

/* Draw the static board elements once per game */
void draw_static_board(void) {
    int i, j;
    cleardevice();
    setfillstyle(SOLID_FILL, BROWN);
    /* Fixed boundary: max coordinate is 479 to prevent overflow in 640x480 VGA mode */
    bar(MARGIN - 20, MARGIN - 20,
        MARGIN + (BOARD_SIZE - 1) * CELL_SIZE + 19,
        MARGIN + (BOARD_SIZE - 1) * CELL_SIZE + 19);
    for (i = 0; i < BOARD_SIZE; i++) {
        for (j = 0; j < BOARD_SIZE; j++) {
            render_cell(i, j);
        }
    }
    draw_status();
}

void erase_cursor(int x, int y) {
    render_cell(y, x); /* Reverts the cell back to board state, erasing the cursor */
}

void draw_cursor(int x, int y) {
    setcolor(RED);
    rectangle(MARGIN + x * CELL_SIZE - 14, MARGIN + y * CELL_SIZE - 14,
              MARGIN + x * CELL_SIZE + 14, MARGIN + y * CELL_SIZE + 14);
}

/* Check if placing a stone at (r,c) creates 5 in a row */
int check_win(int r, int c, int player) {
    int dr[] = {0, 1, 1, 1};
    int dc[] = {1, 0, 1, -1};
    int k, step, count, nr, nc;
    for (k = 0; k < 4; k++) {
        count = 1;
        for (step = 1; step < 5; step++) {
            nr = r + dr[k] * step; nc = c + dc[k] * step;
            if (nr >= 0 && nr < BOARD_SIZE && nc >= 0 && nc < BOARD_SIZE && board[nr][nc] == player)
                count++;
            else break;
        }
        for (step = 1; step < 5; step++) {
	    nr = r - dr[k] * step; nc = c - dc[k] * step;
            if (nr >= 0 && nr < BOARD_SIZE && nc >= 0 && nc < BOARD_SIZE && board[nr][nc] == player)
                count++;
            else break;
        }
        if (count >= 5) return 1;
    }
    return 0;
}

int is_draw(void) {
    return (move_count >= BOARD_SIZE * BOARD_SIZE);
}

/* Advanced Evaluation: Uses a 5-cell window scan to detect patterns AND gaps */
long evaluate_move(int r, int c, int player) {
    long score = 0;
    int opp = (player == STONE_BLACK) ? STONE_WHITE : STONE_BLACK;
    int dr[] = {0, 1, 1, 1};
    int dc[] = {1, 0, 1, -1};
    int k, start, step, nr, nc, my_count, opp_count, valid, val;
    for (k = 0; k < 4; k++) {
        for (start = -4; start <= 0; start++) {
            my_count = 0;
            opp_count = 0;
            valid = 1;
	    for (step = 0; step < 5; step++) {
                nr = r + dr[k] * (start + step);
                nc = c + dc[k] * (start + step);
                if (nr < 0 || nr >= BOARD_SIZE || nc < 0 || nc >= BOARD_SIZE) {
                    valid = 0; break;
                }
                val = board[nr][nc];
                if (nr == r && nc == c) val = player; /* Simulate placing stone here */
                
                if (val == player) my_count++;
                else if (val == opp) opp_count++;
            }
            if (valid && opp_count == 0) {
		if (my_count == 5) score += 100000;
                else if (my_count == 4) score += 5000;
                else if (my_count == 3) score += 500;
                else if (my_count == 2) score += 50;
                else if (my_count == 1) score += 5;
            }
        }
    }
    return score;
}

/* Calculates score based on AI Difficulty Level */
long get_move_score(int r, int c, int player, int level) {
    long my_score = evaluate_move(r, c, player);
    long opp_score = evaluate_move(r, c, (player == STONE_BLACK) ? STONE_WHITE : STONE_BLACK);
    long total = my_score + opp_score;
    int center_dist;
    long resp_score, opp_best;
    /* Level 2+: Prioritize Center Control (Scales with level) */
    if (level >= 2) {
        center_dist = abs(r - 7) + abs(c - 7);
        if (level >= 6) {
            total += (14 - center_dist) * 15; /* Extreme center priority */
        } else if (level >= 5) {
            total += (14 - center_dist) * 10;
        } else {
	    total += (14 - center_dist) * 5;
        }
    }

    /* Level 3+: 1-Ply Lookahead (Predict Opponent's Best Response) */
    if (level >= 3) {
        int dr, dc, nr, nc;
        board[r][c] = player;
        opp_best = 0;
        for (dr = -2; dr <= 2; dr++) {
            for (dc = -2; dc <= 2; dc++) {
                nr = r + dr;
                nc = c + dc;
		if (nr >= 0 && nr < BOARD_SIZE && nc >= 0 && nc < BOARD_SIZE && board[nr][nc] == EMPTY) {
                    resp_score = evaluate_move(nr, nc, (player == STONE_BLACK) ? STONE_WHITE : STONE_BLACK);
                    if (resp_score > opp_best) opp_best = resp_score;
                }
            }
        }
        board[r][c] = EMPTY;
        total -= opp_best / 2; /* Penalize moves that give opponent a great reply */
    }

    /* Level 4+: Aggressive */
    if (level >= 6) {
        total += my_score; /* Maximum aggression for Infinite Elite */
    } else if (level >= 5) {
        total += (my_score * 3) / 4; /* High aggression for Legends */
    } else if (level >= 4) {
        total += my_score / 2; 
    }

    return total;
}

int is_candidate(int r, int c) {
    int dr, dc, nr, nc;
    if (board[r][c] != EMPTY) return 0;
    for (dr = -2; dr <= 2; dr++) {
	for (dc = -2; dc <= 2; dc++) {
            if (dr == 0 && dc == 0) continue;
            nr = r + dr;
            nc = c + dc;
            if (nr >= 0 && nr < BOARD_SIZE && nc >= 0 && nc < BOARD_SIZE) {
                if (board[nr][nc] != EMPTY) return 1;
            }
        }
    }
    return 0;
}

/* AI Move Selection */
void ai_move(void) {
    int r, c;
    int best_r = -1, best_c = -1;
    long best_score = -1;
    long score;
    /* 1. Check if AI can win immediately */
    for (r = 0; r < BOARD_SIZE; r++) {
        for (c = 0; c < BOARD_SIZE; c++) {
            if (is_candidate(r, c)) {
                board[r][c] = STONE_WHITE;
                if (check_win(r, c, STONE_WHITE)) {
                    best_r = r; best_c = c;
                    board[r][c] = EMPTY;
		    goto make_move;
                }
                board[r][c] = EMPTY;
            }
        }
    }

    /* 2. Check if Human can win immediately, and block */
    for (r = 0; r < BOARD_SIZE; r++) {
        for (c = 0; c < BOARD_SIZE; c++) {
            if (is_candidate(r, c)) {
                board[r][c] = STONE_BLACK;
                if (check_win(r, c, STONE_BLACK)) {
		    best_r = r; best_c = c;
                    board[r][c] = EMPTY;
                    goto make_move;
                }
                board[r][c] = EMPTY;
            }
        }
    }

    /* 3. Evaluate board for all candidates based on AI Level */
    for (r = 0; r < BOARD_SIZE; r++) {
        for (c = 0; c < BOARD_SIZE; c++) {
            if (is_candidate(r, c)) {
		score = get_move_score(r, c, STONE_WHITE, ai_level);
                score += random(5); /* Small random factor to vary gameplay */
                if (score > best_score) {
                    best_score = score;
                    best_r = r;
                    best_c = c;
                }
            }
        }
    }

    if (best_r == -1) {
        do {
	    best_r = random(BOARD_SIZE);
            best_c = random(BOARD_SIZE);
        } while (board[best_r][best_c] != EMPTY);
    }
make_move:
    if (best_r != -1) {
        cursor_x = best_c;
        cursor_y = best_r;
        board[best_r][best_c] = STONE_WHITE;
        move_count++;
        render_cell(best_r, best_c); /* Only redraw the changed cell! */
        if (check_win(best_r, best_c, STONE_WHITE)) {
            game_over = 1;
	    winner = STONE_WHITE;
        } else if (is_draw()) {
            game_over = 1;
            winner = 0;
        } else {
            current_player = STONE_BLACK;
        }
    }
}

void init_graphics(void) {
    int gd = DETECT, gm;

 /* Register the embedded BGI driver so EGAVGA.BGI is not needed at runtime */
    registerbgidriver(EGAVGA_driver);

    initgraph(&gd, &gm, "");
    if (graphresult() != grOk) {
        printf("Graphics initialization failed. Ensure BGI files are in path.\n");
        getch();
        exit(1);
    }
}

/* Main Menu */
void show_menu(void) {
    int kanji_x, kanji_y, kanji_scale;
    int char_w, char_h;
    int hira_x, hira_y, hira_scale;
    int go_daku_x, go_daku_y, be_daku_x, be_daku_y;

    setbkcolor(BLACK);
    cleardevice();
    
    /* Draw Kanji Title "五目並べ" in traditional calligraphy style (yellow) */
    /* 12x12 bitmap, scale 7 -> 84x84 pixels per character, centered */
    kanji_scale = 7;
    char_w = 12;
    char_h = 12;
    kanji_x = (640 - (4 * char_w * kanji_scale)) / 2; /* Center horizontally for 640px width */
    kanji_y = 50; /* Adjusted Y position for better vertical spacing */

    draw_bitmap_text(kanji_x, kanji_y, kanji_scale, kanji_bitmap, char_w, char_h, 4, YELLOW);

    /* Draw Hiragana Title "ごもくならべ" (white) */
    /* 12x12 bitmap, scale 4 -> 48x48 pixels per character, also centered to match */
    hira_scale = 4;
    hira_x = (640 - (6 * char_w * hira_scale)) / 2;
    hira_y = 150;

    draw_bitmap_text(hira_x, hira_y, hira_scale, hira_bitmap, char_w, char_h, 6, WHITE);

    /* Menu Options (shifted down slightly to accommodate the larger title) */
    setcolor(LIGHTGREEN);
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 2);
    outtextxy(160, 240, "1. Player vs Player");
    outtextxy(160, 280, "2. Player vs Computer");
    outtextxy(210, 320, "ESC to Quit");

    /* Credits moved to the bottom of the title screen */
    setcolor(LIGHTGRAY);
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 1);
    outtextxy(210, 440, "Game by I. Kruchinin, KIPSoft");

    while (1) {
        if (kbhit()) {
            char ch = getch();
            if (ch == '1') { 
                game_mode = 1; 
                break; 
            }
            if (ch == '2') {
                 game_mode = 2;
		ai_level = 1; /* Reset AI level for new game */
                get_rival_name(ai_level, current_rival);
                break;
            }
            if (ch == 27) exit(0);
        }
    }
    init_board();
    game_over = 0;
    winner = 0;
    current_player = (random(2) == 0) ? STONE_BLACK : STONE_WHITE;
    cursor_x = 7;
    cursor_y = 7;
}

int main(void) {
    int key;
    int old_x, old_y;
    char ch;
    char rival_msg[60];
    randomize();
    init_graphics();
    show_menu();
    draw_static_board();
    draw_cursor(cursor_x, cursor_y);

    while (1) {
	if (game_over) {
	    draw_status();
	    draw_win_message();

	    setcolor(LIGHTGREEN);
	    settextstyle(DEFAULT_FONT, HORIZ_DIR, 1);

	    /* Display Rematch option with specific rival name */
	    sprintf(rival_msg, "(%s)", current_rival);
	    outtextxy(490, 310, "R: Rematch");
	    outtextxy(490, 330, rival_msg);
	    outtextxy(490, 360, "C: Continue");
	    outtextxy(490, 380, "(Next Match)");
	    outtextxy(490, 410, "M: Main Menu ");

            while (1) {
                if (kbhit()) {
                    ch = getch();
                    if (ch == 'r' || ch == 'R') {
                        /* Rematch: Same rival, same level */
                        init_board();
                        game_over = 0;
                        winner = 0;
                        current_player = (random(2) == 0) ? STONE_BLACK : STONE_WHITE;
                        cursor_x = 7; cursor_y = 7;
                        draw_static_board();
			draw_cursor(cursor_x, cursor_y);
                        break;
                    } else if (ch == 'c' || ch == 'C') {
                        /* Continue: Increment level if player won, get new rival */
                        if (winner == STONE_BLACK) {
                            if (ai_level == 5) {
                                draw_congratulations(); /* Special message after beating Level 5 */
                            }
                            ai_level++;
                        }
                        get_rival_name(ai_level, current_rival);
                        
                        init_board();
			game_over = 0;
                        winner = 0;
                        current_player = (random(2) == 0) ? STONE_BLACK : STONE_WHITE;
                        cursor_x = 7; cursor_y = 7;
                        draw_static_board();
                        draw_cursor(cursor_x, cursor_y); 
                        break;
                    } else if (ch == 'm' || ch == 'M' || ch == 27) {
                        show_menu();
                        draw_static_board();
                        draw_cursor(cursor_x, cursor_y);
                        break;
                    }  
                }
            }
            continue;
        }

        /* Computer's turn */
           if (game_mode == 2  && current_player == STONE_WHITE) {
        draw_status(); 
        delay(600); 
        erase_cursor(cursor_x, cursor_y);
        ai_move();
        
        /* FIX: Only draw the cursor if the AI didn't just win */
        if (!game_over) {
            draw_cursor(cursor_x, cursor_y);
        }
        
        draw_status();  
        continue;
    }

        /* Player input */
        if (kbhit()) {
            key = getch(); 
            if (key == 0 || key == 224) {
                key = getch();
                old_x = cursor_x; old_y = cursor_y;
                switch (key) {
                    case 72: if (cursor_y > 0) cursor_y--; break; /* Up */
                    case 80: if (cursor_y < BOARD_SIZE - 1) cursor_y++; break; /* Down */
                    case 75: if (cursor_x > 0) cursor_x--; break; /* Left */
                    case 77: if (cursor_x < BOARD_SIZE - 1) cursor_x++; break; /* Right */
                }
                if (old_x != cursor_x || old_y != cursor_y) {
                    erase_cursor(old_x, old_y);
                    draw_cursor(cursor_x, cursor_y);
                }
            } else if (key == 13 || key == ' ') {
            if (board[cursor_y][cursor_x] == EMPTY) {
                erase_cursor(cursor_x, cursor_y);
                board[cursor_y][cursor_x] = current_player;
                move_count++;
                render_cell(cursor_y, cursor_x); 
                
                if (check_win(cursor_y, cursor_x, current_player)) {
                    game_over = 1;
                    winner = current_player;
                } else if (is_draw()) {
                    game_over = 1;
                    winner = 0;
                } else {
                    current_player = (current_player == STONE_BLACK) ? STONE_WHITE :  STONE_BLACK;
                }
                draw_status();
                
                /* FIX: Only draw the cursor if the player didn't just win */
                if (!game_over) {
                    draw_cursor(cursor_x, cursor_y);
                }
            }
        } else if (key == 27) {
            show_menu();
            draw_static_board();
            draw_cursor(cursor_x, cursor_y);
        }
    }
        delay(10); /* Prevent 100% CPU usage */
    }

    closegraph();
    return 0;
}